@echo off

:argactionstart
if -%1-==-- goto argactionend
set a=%1
echo %a:./*/=./%
shift
goto argactionstart
:argactionend