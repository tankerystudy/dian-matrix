@echo off
call set_env.bat
call clear_built.bat

cd %PROBOX%
qmake dian_matrix_app.pro
nmake

cd %CUR_DIR%
call copy_binary.bat

pause
