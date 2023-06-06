@echo off
set PROJECT_PATH=%~dp0
cd /d %PROJECT_PATH%
set PROJECT_PATH_FIX=%PROJECT_PATH:\=/%
set PATH=%PROJECT_PATH%\tools\bin;%PATH%

set RTTHREADDIR=%PROJECT_PATH_FIX%/rt-thread-no-bsp/rt-thread-no-bsp
set RTTHREADBSPDIR=%PROJECT_PATH_FIX%/
set WIN32=Y