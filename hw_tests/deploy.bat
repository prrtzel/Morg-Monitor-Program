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

start /b "" easybin %name%/%name%.S68 /r /e

powershell -nop -c "& {sleep -m 10}"
wscript "save_bin.vbs" %1.BIN
taskkill /f /im easybin.exe

start xgpro

powershell -nop -c "& {sleep -m 100}"
wscript "open_bin_xgpro.vbs" %1.BIN

exit /b