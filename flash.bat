@echo off
set PROJECT_PATH=%~dp0
cd /d %PROJECT_PATH%
call %PROJECT_PATH%\config.bat

REM 设置串口号
set COM_PORT=%1
If "X%COM_PORT%"=="X" goto :no_com_port

REM 检查rtthread.bin是否存在。
IF NOT EXIST  %PROJECT_PATH%\rtthread.bin goto :no_rtthread_bin

%PROJECT_PATH%/pack/soc_download.exe  air105_download %COM_PORT% 3000000 "." "pack/bootloader.bin" 01001000 "rtthread.bin" 01010000 "pack/script.bin" 01300000 0 0 01380000 0

goto :eof
:no_com_port
echo please set comport. e.g:%0 1 = use com1 for flash.
goto :eof
:no_rtthread_bin
echo rthtread.bin don't exist,please execute scons .
goto :eof