@echo off

rem Check if the directory exists
set "hasDir=false"

for /f "delims=" %%a in ('dir /ad /b') do (
    if %%a==%1 (
        set name=%1
        set "hasDir=true"
    )
)

if "%hasDir%"=="false" (
    echo "No such directory"
    exit /b
) 

echo "Deploying %name%..."

rem Start easybin and save the file as a .bin
start /b "" easybin %name%/%name%.S68 /r /e
powershell -nop -c "& {sleep -m 10}"
wscript "save_bin.vbs" %1.BIN
taskkill /f /im easybin.exe

rem Start xgpro and open the file 
start xgpro
powershell -nop -c "& {sleep -m 100}"
wscript "open_bin_xgpro.vbs" %1.BIN
powershell -nop -c "& {sleep -m 100}"

rem Start impact and go through pop up window
start impact
powershell -nop -c "& {sleep -m 100}"
wscript "impact_nav.vbs"

echo "Done!"

exit /b