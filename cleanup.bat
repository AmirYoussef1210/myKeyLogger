@echo off
echo Terminating KeyLogger process...
taskkill /f /im sys_driver.exe
taskkill /f /im myKeyLogger.exe

echo Removing Registry Persistence...
reg delete "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run" /v "WindowsDriverUpdate" /f

echo Deleting installed files and logs...
del /f /q "%APPDATA%\Microsoft\Windows\sys_driver.exe"
rmdir /s /q "%LOCALAPPDATA%\Microsoft\Windows\Updates"

echo Cleanup complete.
pause
