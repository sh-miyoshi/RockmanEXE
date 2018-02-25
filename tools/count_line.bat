@echo off
set folder_path=..\RockmanEXE\source
dir /b %folder_path% > tmp.txt
for /F %%i in ( tmp.txt ) do type "%folder_path%\%%i" | find /c /v "" >> num.txt
set n=0
for /F "usebackq" %%i in (num.txt) do set /a n=n+%%i
echo ‡Œv”: %n%
PAUSE
del tmp.txt
del num.txt