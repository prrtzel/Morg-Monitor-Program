rem @echo off

echo "Deploying %name%..."

rem Start easybin and save the file as a .bin
start /b "" easybin ../bin/%1.S68 /r /e
powershell -nop -c "& {sleep -m 10}"
wscript "save_bin.vbs" %1.BIN
taskkill /f /im easybin.exe

rem Start xgpro and open the file 
start xgpro
powershell -nop -c "& {sleep -m 100}"
wscript "open_bin_xgpro.vbs" %1.BIN
powershell -nop -c "& {sleep -m 100}"

rem Start impact and go through pop up window
rem start impact
rem powershell -nop -c "& {sleep -m 100}"
rem wscript "impact_nav.vbs"

echo "Done!"

exit /b