@echo off

rem Move the regular expression into a named variable.
set reg=%1
shift

rem ^ matches the start of the string
rem . matches all but newlines, except in brackets.
rem [] match anything inside.
rem - match a range. Special coding will probably be needed for this.
rem [^ ] match anything but what is inside.
rem $ match end.
rem * match preceding expression 0 or more times.
rem + match preceding expression 1 or more times.
rem ? match preceding expression 0 or 1 times.
rem () marked subexpression.

rem compile the regular expression.
call :strlen len reg
set count=0
set brackets=false
set notbrackets=false
set escaped=false
set lower=a b c d e f g h i j k l m n o p q r s t u v w x y z
set upper=A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
set number=0 1 2 3 4 5 6 7 8 9 0

set tail=%reg%

for /l %%i in (0, 1, %len%) do (
	if %%i==0 (
		set lastchar=start
	) else (
		set lastchar=%curr%
	)
	set curr=%tail:~0,1%
	set tail=%tail:~1,-1%
	
	if %brackets%==true (
		set compiled_%%i=!compiled_%%i! curr
	)
	if %notbrackets%=true (
		set compiled_%%i=!compiled_%%i! curr
	)
	if %curr%==^[ (
		set brackets=true
	)
	if %curr%==^^ (
		if %lastchar%==^[ (
			set brackets=false
			set notbrackets=true
	
	


rem start looping over arguments.
:argstart
if -%1-==-- goto :argend

rem move argument into a string and then procede to parse.




rem From StackOverflow.
:strlen <resultVar> <stringVar>
(   
    setlocal EnableDelayedExpansion
    set "s=!%~2!#"
    set "len=0"
    for %%P in (4096 2048 1024 512 256 128 64 32 16 8 4 2 1) do (
        if "!s:~%%P,1!" NEQ "" ( 
            set /a "len+=%%P"
            set "s=!s:~%%P!"
        )
    )
)
( 
    endlocal
    set "%~1=%len%"
    exit /b
)