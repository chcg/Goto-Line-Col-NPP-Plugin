@ECHO OFF

SET DM_DIR=DM_includes\x64
SET BIN_DIR=bin64
SET NOTEPAD_DIR=E:\Portable\Notepad++

COPY license.txt %BIN_DIR%
COPY %DM_DIR%\NPP_Plugin_Darkmode.dll %BIN_DIR%
COPY %BIN_DIR%\*.dll "%NOTEPAD_DIR%\plugins\GotoLineCol\"

START /D "%NOTEPAD_DIR%" Notepa~1.exe
EXIT 0
