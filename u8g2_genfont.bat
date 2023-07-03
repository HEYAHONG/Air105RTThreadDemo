@echo off
set PROJECT_PATH=%~dp0
cd /d %PROJECT_PATH%
call %PROJECT_PATH%\config.bat

@REM 检查字体map文件
if NOT exist u8g2_font.map goto :eof

@REM 检查U8G2路径文件
if NOT exist packages\u8g2-official-latest\tools\font\bdfconv goto :eof

@REM 设置相应变量
set PATH=%PATH%:%PROJECT_PATH%\packages\u8g2-official-latest\tools\font\bdfconv\
set BDFCONVPATH=%PROJECT_PATH%\packages\u8g2-official-latest\tools\font\bdfconv\
set BDFPATH=%PROJECT_PATH%\packages\u8g2-official-latest\tools\font\bdf\

@REM 生成字体文件
"%BDFCONVPATH%\bdfconv.exe" "%BDFPATH%/wenquanyi_9pt.bdf" -b 0 -f 1 -M  %PROJECT_PATH%\u8g2_font.map -n u8g2_font_wqy12_t_user -o _u8g2_font_wqy12_t_user.h~
"%BDFCONVPATH%\bdfconv.exe" "%BDFPATH%/wenquanyi_10pt.bdf" -b 0 -f 1 -M %PROJECT_PATH%\u8g2_font.map -n u8g2_font_wqy13_t_user -o _u8g2_font_wqy13_t_user.h~
"%BDFCONVPATH%\bdfconv.exe" "%BDFPATH%/wenquanyi_13px.bdf" -b 0 -f 1 -M %PROJECT_PATH%\u8g2_font.map -n u8g2_font_wqy14_t_user -o _u8g2_font_wqy14_t_user.h~
"%BDFCONVPATH%\bdfconv.exe" "%BDFPATH%/wenquanyi_11pt.bdf" -b 0 -f 1 -M %PROJECT_PATH%\u8g2_font.map -n u8g2_font_wqy15_t_user -o _u8g2_font_wqy15_t_user.h~
"%BDFCONVPATH%\bdfconv.exe" "%BDFPATH%/wenquanyi_12pt.bdf" -b 0 -f 1 -M %PROJECT_PATH%\u8g2_font.map -n u8g2_font_wqy16_t_user -o _u8g2_font_wqy16_t_user.h~

@copy /B   _u8g2_font_wqy12_t_user.h~+_u8g2_font_wqy13_t_user.h~+_u8g2_font_wqy14_t_user.h~+_u8g2_font_wqy15_t_user.h~+_u8g2_font_wqy16_t_user.h~  "%PROJECT_PATH%\applications\appstack\u8g2app\u8g2_font_user.h" >nul
@del /s /q _u8g2_font*~ >nul