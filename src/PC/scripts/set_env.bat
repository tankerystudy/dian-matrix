@echo off

call set_dev_env.bat

set CUR_DIR=%~dp0
set SRCBOX=%CUR_DIR%\..
set DSTBOX=%CUR_DIR%\..\binaries
set PROBOX=%CUR_DIR%\..\qtpro
set SLNBOX=%CUR_DIR%\..\vs2008