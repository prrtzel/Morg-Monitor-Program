@echo off
start /b    "" call_easybin.bat
start /b /w "" save_morg_bin.bat
start /b       call_xgpro.bat
start /b /w "" open_morg_bin.bat
exit  /b