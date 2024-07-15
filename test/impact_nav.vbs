Set WshShell = WScript.CreateObject("WScript.Shell")
Wscript.sleep 1000
for i = 1 to 5
    WshShell.SendKeys "{TAB}"
next
WshShell.SendKeys "{ENTER}"
WScript.quit