#include "Utils.h"

extern HINSTANCE _gModule;

// ***************** PRIVATE *****************

enum fontStyle {
   FS_BOLD,
   FS_ITALIC,
   FS_UNDERLINE
};

bool changeFontStyle(HWND hDlg, int controlID, fontStyle style) {
   HWND hwndCtrl = GetDlgItem(hDlg, controlID);
   LOGFONT lf{ 0 };

   HFONT hFont = (HFONT)SendMessage(hwndCtrl, WM_GETFONT, NULL, NULL);

   if (!GetObject(hFont, sizeof(lf), &lf))
      return FALSE;

   switch (style) {
   case FS_BOLD:
      lf.lfWeight = FW_BOLD;
      break;

   case FS_ITALIC:
      lf.lfItalic = TRUE;
      break;

   case FS_UNDERLINE:
      lf.lfUnderline = TRUE;
      break;

   default:
      break;
   }

   hFont = CreateFontIndirect(&lf);
   SendMessage(hwndCtrl, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(true, 0));

   return TRUE;
}


// ***************** PUBLIC *****************

HWND Utils::addTooltip(HWND hDlg, int controlID, LPWSTR pTitle, LPWSTR pMessage) {
   if (!controlID || !hDlg || !pMessage)
      return FALSE;

   // Get the window of the tool.
   HWND hwndCtrl = GetDlgItem(hDlg, controlID);

   // Create the tooltip.
   HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
      WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      hDlg, NULL,
      _gModule, NULL);

   if (!hwndCtrl || !hwndTip)
      return (HWND)NULL;

   // Associate the tooltip with the tool.
   TOOLINFO toolInfo{};
   toolInfo.cbSize = sizeof(toolInfo);
   toolInfo.hwnd = hDlg;
   toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
   toolInfo.uId = (UINT_PTR)hwndCtrl;
   toolInfo.lpszText = pMessage;
   SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
   SendMessage(hwndTip, TTM_SETTITLE, TTI_INFO, (LPARAM)pTitle);
   SendMessage(hwndTip, TTM_SETMAXTIPWIDTH, 0, (LPARAM)PREFS_TIP_MAX_WIDTH);

   return hwndTip;
}

bool Utils::checkBaseOS(winVer os) {
   return (nppMessage(NPPM_GETWINDOWSVERSION, NULL, NULL) >= os);
}

wstring Utils::getVersionInfo(LPCWSTR key) {
   wstring sVersionInfo;
   wchar_t sModuleFilePath[MAX_PATH];
   DWORD  verHandle{};
   DWORD  verSize{};
   UINT   querySize{};
   LPBYTE lpBuffer{};
   wchar_t qVal[2000]{};

   struct LANGANDCODEPAGE {
      WORD wLanguage;
      WORD wCodePage;
   } *lpTranslate;

   GetModuleFileName(_gModule, sModuleFilePath, MAX_PATH);
   verSize = GetFileVersionInfoSize(sModuleFilePath, &verHandle);

   if (verSize) {
      LPSTR verData = new char[verSize];

      if (GetFileVersionInfo(sModuleFilePath, verHandle, verSize, verData)) {

         VerQueryValue(verData, L"\\VarFileInfo\\Translation", (VOID FAR * FAR*)& lpTranslate, & querySize);

         swprintf(qVal, 2000, L"\\StringFileInfo\\%04X%04X\\%s",
            lpTranslate[0].wLanguage, lpTranslate[0].wCodePage, key);

         if (VerQueryValue(verData, wstring(qVal).c_str(), (VOID FAR * FAR*) & lpBuffer, &querySize)) {
            if (querySize) {
               sVersionInfo = wstring((LPCTSTR)lpBuffer);
            }
         }
      }
      delete[] verData;
   }

   return sVersionInfo;
}

void Utils::loadBitmap(HWND hDlg, int controlID, int resource) {
   HWND hwndCtrl = GetDlgItem(hDlg, controlID);
   HBITMAP hBitmap = LoadBitmap(_gModule, MAKEINTRESOURCE(resource));

   if (hBitmap) {
      SendMessage(hwndCtrl, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
   }
   DeleteObject(hBitmap);
}

void Utils::setFont(HWND hDlg, int controlID, wstring &name, int height, int weight, bool italic, bool underline) {
   HWND hwndCtrl = GetDlgItem(hDlg, controlID);
   LOGFONT lf{ 0 };

   wcscpy(lf.lfFaceName, name.c_str());

   HDC hdc = GetDC(hDlg);
   lf.lfHeight = -MulDiv(height, GetDeviceCaps(hdc, LOGPIXELSY), 72);
   ReleaseDC(hDlg, hdc);

   lf.lfWeight = weight;
   lf.lfItalic = italic;
   lf.lfUnderline = underline;

   HFONT hFont = CreateFontIndirect(&lf);

   SendMessage(hwndCtrl, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(true, 0));
}

bool Utils::setFontBold(HWND hDlg, int controlID) {
   return changeFontStyle(hDlg, controlID, FS_BOLD);
}

bool Utils::setFontItalic(HWND hDlg, int controlID) {
   return changeFontStyle(hDlg, controlID, FS_ITALIC);
}

bool Utils::setFontUnderline(HWND hDlg, int controlID) {
   return changeFontStyle(hDlg, controlID, FS_UNDERLINE);
}
