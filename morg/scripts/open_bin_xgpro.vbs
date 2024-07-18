Set WshShell = WScript.CreateObject("WScript.Shell")
WshShell.AppActivate("Xgpro")
WScript.sleep 100

REM --------------------------
REM Selects the T48 option.

WshShell.SendKeys "{TAB}"
WshShell.SendKeys "{DOWN}"
WshShell.SendKeys "{ENTER}"
WScript.sleep 100

REM --------------------------

REM Open file dialog
WshShell.SendKeys "^{o}"
WScript.sleep 100

REM input the path to the bin file
For i = 1 To 9
    WshShell.SendKeys "{TAB}"
Next
WshShell.SendKeys "{ENTER}"

REM Select the morg.BIN file
WshShell.SendKeys Wscript.Arguments(0)
WshShell.SendKeys "{ENTER}"

REM Select lower byte option
For i = 1 To 3
    WshShell.SendKeys "{TAB}"
Next
WshShell.SendKeys "{DOWN}"

REM Navigate to "ok" button
For i = 1 To 4
    WshShell.SendKeys "{TAB}"
Next
WshShell.SendKeys "{ENTER}"
WScript.quit