//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "GoToLineColPanel.h"
#include <time.h>
#include <wchar.h>

INT_PTR CALLBACK GotoLineColPanel::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) {
   switch (message) {
   case WM_COMMAND :
   {
      switch LOWORD(wParam) {
      case IDC_GOLINE_EDIT:
         updateColumnRangeText(getInputLineValidated());
         break;

      case IDOK :
         navigateToColPos();
         break;

      case IDCANCEL:
      case IDCLOSE :
         setFocusOnEditor();
         ShowGotoLineColPanel(false);
         break;

      case IDC_GOLINECOL_PREFS:
         ::SetFocus(_hSelf);
         ShowPreferencesDialog();
         break;
      }

      break;
   }

   case WM_LBUTTONDOWN:
   case WM_MBUTTONDOWN:
   case WM_RBUTTONDOWN:
      ::SetFocus(_hSelf);
      break;

   case WM_SETFOCUS :
   {
      if (allPrefs.fillOnFocus)
         updatePanelColPos();
      break;
   }

   default :
      return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
   }

   return FALSE;
}

void GotoLineColPanel::localize() {
   ::SetDlgItemText(_hSelf, IDC_GOLINE_STATIC, GOLINECOL_LABEL_GOLINE);
   ::SetDlgItemText(_hSelf, IDC_GOCOL_STATIC, GOLINECOL_LABEL_GOCOL);
   ::SetDlgItemText(_hSelf, IDOK, GOLINECOL_LABEL_BTN_GO);
   ::SetDlgItemText(_hSelf, IDCLOSE, GOLINECOL_LABEL_BTN_CLOSE);
   ::SetDlgItemText(_hSelf, IDC_GOLINECOL_PREFS, MENU_PREFERENCES);
}

void GotoLineColPanel::display(bool toShow) {
   DockingDlgInterface::display(toShow);

   if (toShow) {
      ::SetFocus(::GetDlgItem(_hSelf, IDC_GOLINE_EDIT));
   }
};

void GotoLineColPanel::setParent(HWND parent2set) {
   _hParent = parent2set;
};

void GotoLineColPanel::loadPreferences() {
   HWND hScintilla = getCurrentScintilla();
   if (!hScintilla)
      return;

   allPrefs = _prefsIO.loadPreferences();

   wchar_t note[100];
   swprintf(note, 100, GOLINECOL_EXPAND_TABS_NOTE,
                     allPrefs.expandTabs ? L"" : GOLINECOL_EXPAND_TABS_STATE,
                     (int)::SendMessage(hScintilla, SCI_GETTABWIDTH, 0, 0));
   ::SetDlgItemText(_hSelf, IDC_GOLINECOL_EXPAND_TABS, note);
}

void GotoLineColPanel::updatePanelColPos() {
   HWND hScintilla = getCurrentScintilla();
   if (!hScintilla)
      return;

   int pos = (int)::SendMessage(hScintilla, SCI_GETCURRENTPOS, 0, 0);
   int line = (int)::SendMessage(hScintilla, SCI_LINEFROMPOSITION, pos, 0) + 1;
   int col = getDocumentColumn(hScintilla, pos, line);

   ::SetDlgItemText(_hSelf, IDC_GOLINE_EDIT, TO_LPCWSTR(line));
   updateLineRangeText();

   ::SetDlgItemText(_hSelf, IDC_GOCOL_EDIT, TO_LPCWSTR(col));
   updateColumnRangeText(line);

   // Clear Idem Potent key if it's still set from a premature program termination
   idemPotentKey = FALSE;
}

void GotoLineColPanel::clearCalltip() {
   HWND hScintilla = getCurrentScintilla();
   if (!hScintilla)
      return;

   ::SendMessage(hScintilla, SCI_CALLTIPCANCEL, NULL, NULL);
}

/// *** Private Functions: *** ///

HWND GotoLineColPanel::getCurrentScintilla() {
   int which = -1;
   ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)& which);
   if (which < 0)
      return (HWND)FALSE;
   return (HWND)(which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
}

int GotoLineColPanel::getLineCount() {
   HWND hScintilla = getCurrentScintilla();
   if (!hScintilla)
      return -1;

   return (int)::SendMessage(hScintilla, SCI_GETLINECOUNT, 0, 0);
};

int GotoLineColPanel::setFocusOnEditor() {
   HWND hScintilla = getCurrentScintilla();
   if (!hScintilla)
      return -1;

   return (int)::SendMessage(hScintilla, SCI_GRABFOCUS, 0, 0);
};

int GotoLineColPanel::getLineMaxPos(int line) {
   HWND hScintilla = getCurrentScintilla();
   if (!hScintilla)
      return -1;

   int endPos = (int)::SendMessage(hScintilla, SCI_GETLINEENDPOSITION, line - 1, 0);

   int col = (allPrefs.expandTabs) ?
               (int)::SendMessage(hScintilla, SCI_GETCOLUMN, endPos, 0) :
               endPos - (int)::SendMessage(hScintilla, SCI_POSITIONFROMLINE, line - 1, 0);

   return col + 1;
};

int GotoLineColPanel::getDocumentColumn(HWND hScintilla, int pos, int line) {
   int col = (allPrefs.expandTabs) ?
               (int)::SendMessage(hScintilla, SCI_GETCOLUMN, pos, 0) :
               pos - (int)::SendMessage(hScintilla, SCI_POSITIONFROMLINE, line - 1, 0);

   return col + 1;
}

int GotoLineColPanel::setDocumentColumn(HWND hScintilla, int line, int lineStartPos, int lineMaxPos, int column) {
   column = (column < 1) ? 1 :
            (column > lineMaxPos) ? lineMaxPos : column;

   int gotoPos = (allPrefs.expandTabs) ?
                  (int)::SendMessage(hScintilla, SCI_FINDCOLUMN, line - 1, column - 1) :
                  lineStartPos + column - 1;

   ::SendMessage(hScintilla, SCI_GOTOPOS, gotoPos, 0);
   return gotoPos;
}

int GotoLineColPanel::getInputLineValidated() {
   BOOL isSuccessful;

   int line = ::GetDlgItemInt(_hSelf, IDC_GOLINE_EDIT, &isSuccessful, FALSE);
   if (!isSuccessful)
      return 1;

   int lineCount = getLineCount();
   return ((line < 0) ? 1 : ((line > lineCount) ? lineCount : line));
};

int GotoLineColPanel::getInputColumn() const {
   BOOL isSuccessful;

   int col = ::GetDlgItemInt(_hSelf, IDC_GOCOL_EDIT, &isSuccessful, FALSE);
   return (isSuccessful ? col : 1);
};

void GotoLineColPanel::updateLineRangeText() {
   ::SetDlgItemText(_hSelf, IDC_GOLINE_RANGE, (GOLINECOL_MAX_FOR_FILE + TO_WSTR(getLineCount()) + L"]").c_str());
}

void GotoLineColPanel::updateColumnRangeText(int line) {
   ::SetDlgItemText(_hSelf, IDC_GOCOL_RANGE, (GOLINECOL_MAX_FOR_LINE + TO_WSTR(getLineMaxPos(line)) + L"]").c_str());
}

int GotoLineColPanel::navigateToColPos() {
   HWND hScintilla = getCurrentScintilla();
   if (!hScintilla)
      return FALSE;

   int line = getInputLineValidated();

   ::SendMessage(hScintilla, SCI_ENSUREVISIBLE, line - 1, 0);

   int lineMaxPos = getLineMaxPos(line);
   int lineStartPos = (int)::SendMessage(hScintilla, SCI_POSITIONFROMLINE, line - 1, 0);

   int column = getInputColumn();

   if (allPrefs.centerCaret) {
      ::SendMessage(hScintilla, SCI_SETXCARETPOLICY, CARET_JUMPS | CARET_EVEN, (LPARAM)0);
      ::SendMessage(hScintilla, SCI_SETYCARETPOLICY, CARET_JUMPS | CARET_EVEN, (LPARAM)0);
   }
   else {
      // Clear a buffer of edgebuffer positions on either side if possible so that
      // the cursor is not stuck while being aligned with the side margins
      ::SendMessage(hScintilla, SCI_SETXCARETPOLICY, 0, (LPARAM)0);
      ::SendMessage(hScintilla, SCI_SETYCARETPOLICY, 0, (LPARAM)0);
      setDocumentColumn(hScintilla, line, lineStartPos, lineMaxPos, column - allPrefs.edgeBuffer);
      setDocumentColumn(hScintilla, line, lineStartPos, lineMaxPos, column + allPrefs.edgeBuffer);
   }

   int gotoPos = setDocumentColumn(hScintilla, line, lineStartPos, lineMaxPos, column);
   setFocusOnEditor();

   // Display call tip
   if (allPrefs.showCalltip) {
      showCalltip(hScintilla, line, column, gotoPos);
   }

   // Flash caret
   HANDLE hThread = ::CreateThread(NULL, 0, threadPositionHighlighter, 0, 0, NULL);
   if (hThread > 0)
      ::CloseHandle(hThread);

   return TRUE;
}

void GotoLineColPanel::showCalltip(HWND hScintilla, int line, int column, int atPos)
{
   unsigned char atChar;
   int colPos;
   char colPosText[100]{""};

   colPos = (int)::SendMessage(hScintilla, SCI_GETCOLUMN, atPos, 0) + 1;
   if (colPos != column)
      sprintf(colPosText, "Char Column: %u\n", colPos);

   atChar = (unsigned char)::SendMessage(hScintilla, SCI_GETCHARAT, atPos, 0);
   sprintf(callTip, "       Line: %u\n%sByte Column: %u\n\n  ANSI Byte: 0x%X [%u]",
      line, colPosText, column, atChar, atChar);

   if ((atChar & 0x80) != 0) {
      int utf8Start{ atPos };
      unsigned char utf8Char{ atChar };

      int unicodeHead{ 0 }, unicodeVal{ 0 };
      char unicodePoint[10];

      while ((utf8Char & 0xC0) == 0x80 && atPos - utf8Start < 3) {
         utf8Start--;
         utf8Char = (unsigned char)::SendMessage(hScintilla, SCI_GETCHARAT, utf8Start, 0);
      }

      sprintf(callTip, "%s\nUTF-8 Bytes: %s0x%X%s", callTip,
         (utf8Start == atPos ? "<" : ""), utf8Char, (utf8Start == atPos ? ">" : ""));

      int utf8BytePos{ utf8Start + 1 };
      unsigned char utf8ByteChar;

      if ((utf8Char & 0xC0) == 0xC0) {
         utf8ByteChar = (unsigned char)::SendMessage(hScintilla, SCI_GETCHARAT, utf8BytePos, 0);
         sprintf(callTip, "%s %s0x%X%s", callTip,
            (utf8BytePos == atPos ? "<" : ""), utf8ByteChar, (utf8BytePos == atPos ? ">" : ""));

         unicodeHead = (utf8Char & 31) << 6;
         unicodeVal = (utf8ByteChar & 63);
      }

      if ((utf8Char & 0xE0) == 0xE0) {
         utf8BytePos++;
         utf8ByteChar = (unsigned char)::SendMessage(hScintilla, SCI_GETCHARAT, utf8BytePos, 0);
         sprintf(callTip, "%s %s0x%X%s", callTip,
            (utf8BytePos == atPos ? "<" : ""), utf8ByteChar, (utf8BytePos == atPos ? ">" : ""));

         unicodeHead = (utf8Char & 15) << 12;
         unicodeVal = (unicodeVal << 6) + (utf8ByteChar & 63);
      }

      if ((utf8Char & 0xF0) == 0xF0) {
         utf8BytePos++;
         utf8ByteChar = (unsigned char)::SendMessage(hScintilla, SCI_GETCHARAT, utf8BytePos, 0);
         sprintf(callTip, "%s %s0x%X%s", callTip,
            (utf8BytePos == atPos ? "<" : ""), utf8ByteChar, (utf8BytePos == atPos ? ">" : ""));

         unicodeHead = (utf8Char & 7) << 18;
         unicodeVal = (unicodeVal << 6) + (utf8ByteChar & 63);
      }

      sprintf(unicodePoint, "%X", (unicodeHead + unicodeVal));

      sprintf(callTip, "%s\n    Unicode: U+%s%s",
         callTip, ((strlen(unicodePoint) % 2 == 0) ? "" : "0"), unicodePoint);
   }

   ::PostMessage(hScintilla, SCI_CALLTIPSHOW, atPos, (LPARAM)callTip);
}

DWORD WINAPI GotoLineColPanel::threadPositionHighlighter(void*) {
   ALL_PREFERENCES allPrefs = _prefsIO.loadPreferences();

   // Get the current scintilla
   int which = -1;
   ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)& which);
   if (which == -1)
      return FALSE;

   HWND hScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

   // Look for Idem Potency Hold
   if (idemPotentKey)
      //  Idem Potency check failed. Another thread is processing this same function. Return immediately.
      return FALSE;
   else
      // OK to continue. Set Idem Potency Hold
      idemPotentKey = TRUE;

   // Modify caret style briefly to highlight the new position
   int currCaret = (int)::SendMessage(hScintilla, SCI_GETCARETSTYLE, 0, 0);
   ::SendMessage(hScintilla, SCI_SETCARETSTYLE, CARETSTYLE_BLOCK, 0);
   Sleep(allPrefs.caretFlashSeconds * 1000);
   ::SendMessage(hScintilla, SCI_SETCARETSTYLE, currCaret, 0);

   if (allPrefs.braceHilite) {
      int pos = (int)::SendMessage(hScintilla, SCI_GETCURRENTPOS, 0, 0);
      ::SendMessage(hScintilla, SCI_BRACEHIGHLIGHT, pos, -1);
   }

   // Clear Idem Potency Hold
   idemPotentKey = FALSE;

   return TRUE;
}
