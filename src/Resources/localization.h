#ifndef DIALOG_DEFS_H
#define DIALOG_DEFS_H


// Common
#define MENU_PANEL_NAME             L"Goto Line, Column"
#define MENU_SHOW_PANEL             L"&Show Panel"
#define MENU_PREFERENCES            L"&Preferences"
#define MENU_ABOUT                  L"&About"


// Goto Line, Column Panel
#define GOLINECOL_LABEL_GOLINE      L"Goto line:"
#define GOLINECOL_LABEL_GOCOL       L"At column:"
#define GOLINECOL_LABEL_BTN_GO      L"&Go"
#define GOLINECOL_LABEL_BTN_CLOSE   L"&Close"

#define GOLINECOL_MAX_FOR_FILE      L"[Max for file: "
#define GOLINECOL_MAX_FOR_LINE      L"[Max for line: "
#define GOLINECOL_EXPAND_TABS_STATE L" NOT"
#define GOLINECOL_EXPAND_TABS_NOTE  L"*column positioning will%ws count width of tabs [%d spaces]"


// Preferences Dialog
#define PREFS_DIALOG_TITLE          L"Preferences"
#define PREFS_LABEL_AF_ONFOCUS      L"Auto-fill on Panel Focus"
#define PREFS_LABEL_AF_ONTABCHANGE  L"Auto-fill on Document Tab Change"
#define PREFS_LABEL_SHOW_CALLTIP    L"Display character code at destination"
#define PREFS_LABEL_BRACE_HILITE    L"Highlight character at destination"
#define PREFS_LABEL_EXPAND_TABS     L"Expand TABS for column value computation"
#define PREFS_LABEL_CENTER_CARET    L"Cursor is centered in the editor at destination"
#define PREFS_LABEL_EDGE_BUFFER     L"Edge Buffer at destination:"
#define PREFS_LABEL_CARET_FLASH     L"Cursor Flash duration (in seconds)"

#define PREFS_BTN_LABEL_OK          L"&OK"
#define PREFS_BTN_LABEL_CANCEL      L"&Cancel"
#define PREFS_BTN_LABEL_DEFAULTS    L"&Defaults"
#define PREFS_LABEL_TOOLTIP_BOX     L"Tooltips (for this dialog only)"
#define PREFS_LABEL_TOOLTIP_SHOW    L"Show"
#define PREFS_LABEL_TOOLTIP_DUR     L"Duration (in seconds):"


// Preferences Dialog Tooltips
#define PREFS_TIP_MAX_WIDTH         400
#define PREFS_TIP_AF_ONFOCUS        L"When the Goto panel regains focus, the line and column fields in it will be refreshed with the current document's current cursor position.\r\n\r\nEffective only when the first click is on the panel's title bar or a blank area in the panel or the Preferences button. This refresh is suppressed when the first click is within the line or column input fields or on the Goto button.\r\n\r\nUncheck this option if your workflow needs the Goto panel's line and column values to stay fixed regardless of changes in the current document's cursor position."
#define PREFS_TIP_AF_ONTABCHANGE    L"When a different document becomes active either via a tab switch or file open or close, the line and column fields in the Goto panel will be refreshed with the newly active document's current cursor position.\r\n\r\nIn some situations -- when perhaps inspecting similar datafiles, it may be preferrable to have the line and column values stay fixed across the files. Uncheck this option for such workflows."
#define PREFS_TIP_SHOW_CALLTIP      L"Uncheck this option if you do not wish to view, within a calltip, the hex code of the character at the destination position.\r\n\r\n[The calltip will disappear when the cursor is manually moved inside the document.]"
#define PREFS_TIP_BRACE_HILITE      L"Uncheck this option if you do not wish to have the character highlighted (after the cursor flash -- see below) at the destination position.\r\n\r\n[The highlighting will disappear when the cursor is manually moved inside the document.]"
#define PREFS_TIP_EXPAND_TABS       L"When this option is checked, all TAB characters on the destination line will be counted using their expanded width -- a number specified in Notepad++ Preferences/Language section. In fact, Notepad++ also expands TABs in its column computation.\r\n\r\nHowever, if you are viewing files with fixed-width format data, you may want to leave this option unchecked."
#define PREFS_TIP_CENTER_CURSOR     L"When this option is checked, the cursor will try to position as close as possible to the center of the editor view area.\r\n\r\nIf the destination position is already within the prior view area, there will likely be no centering."
#define PREFS_TIP_EDGE_BUFFER       L"[Effective only when Centered Cursor option is unchecked]\r\n\r\nWhen the destination character shows up with the cursor stuck at either the left or right edge of the Notepad++ document view area, it will be hard to spot the cursor and the character. Hence a buffer near the edges, when possible, is desirable.\r\n\nSpecify the number of characters to have as a buffer from either of the edges for the character at the destination position."
#define PREFS_TIP_CARET_FLASH       L"When the cursor is set at its default width of '1', it can be hard to spot the cursor at its new position. To aid quick spotting, the cursor is briefly set to block mode at the destination position.\r\n\r\nSpecify the number of seconds to have the cursor flashing in block mode at the destination position."

#define PREFS_TIP_SHOW_TOOLTIPS     L"A change to this setting will be effective immediately for this Preference dialog session.\r\n\r\nThe setting will be saved for future sessions only when OK is clicked."
#define PREFS_TIP_TOOLTIP_DURATION  L"A change to this setting will be effective for this Preference dialog session only after tabbing out of the input field.\r\n\r\nThe setting will be saved for future sessions only when OK is clicked."


// About Dialog
#define ABOUT_DIALOG_TITLE          L"About"
#define ABOUT_PLUGIN_NAME           L"Goto Line, Column Plugin for Notepad++"

#endif
