@echo off
call set_env.bat

echo coping binary files...
copy "%PROBOX%\dian_matrix\bin\dian_matrix.exe" "%DSTBOX%"

echo coping resources...
copy "%SRCBOX%\resources\HZK16" "%DSTBOX%"
