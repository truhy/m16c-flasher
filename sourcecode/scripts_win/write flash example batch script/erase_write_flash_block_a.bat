@ECHO OFF
CALL env_win.bat

:: Run
SET runcmd=%APP% program path=%SERIALPATH% id_addr=0x0FFFDF id=00000000000000 erase=y from_addr=0x00F000 to_addr=0x00FFFF file="dump-block-a.bin"
ECHO %runcmd%
%runcmd% & IF %errorlevel% NEQ 0 GOTO :err_handler

:: Pause if run from double-click
IF /I %0 EQU "%~dpnx0" PAUSE

GOTO :end_of_script

:err_handler
:: Pause if run from double-click
IF /I %0 EQU "%~dpnx0" PAUSE

:end_of_script
