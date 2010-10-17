@echo off
call set_env.bat

cd %SRCBOX%\vs2008

echo cleaning vs2008 built files......
del /f /s /q .\*.pch
del /f /s /q .\*.sbr
del /f /s /q .\*.bsc
del /f /s /q .\*.idb
del /f /s /q .\*.res
del /f /s /q .\*.pdb
del /f /s /q .\*.obj
del /f /s /q .\*.ilk
del /f /s /q .\*.ncb
del /f /s /q .\*.dep
del /f /s /q .\*.plg
del /f /s /q .\*.aps

rem cleaning vs2008 files
del /f /s /q .\*.htm
del /f /s /q .\*.manifest
del /f /s /q .\*.cd
del /f /s /q .\*.user
del /f /s /q .\*.xml

cd %PROBOX%

echo cleaning QT project built files......
del /f /s /q .\*.pdb
del /f /s /q .\*.obj
del /f /s /q .\*.ilk

del /f /s /q .\makefile*
del /f /s /q .\moc_*
del /f /s /q .\ui_*

rem cleaning vs2008 files
del /f /s /q .\*.manifest
