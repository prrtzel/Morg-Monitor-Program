@echo off
powershell -nop -c "& {sleep -m 10}"
wscript "save_morg_bin.vbs"
taskkill /f /im easybin.exe
exit