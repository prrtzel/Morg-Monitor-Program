Set WshShell = WScript.CreateObject("WScript.Shell")
WshShell.SendKeys "{ENTER}"
for i = 1 to 5
    WshShell.SendKeys "{TAB}"
next
WshShell.SendKeys "{ENTER}"
WScript.sleep 500
WshShell.SendKeys "morg"
WScript.sleep 100
WshShell.SendKeys "{ENTER}"
WshShell.SendKeys "{ENTER}"
WScript.quit