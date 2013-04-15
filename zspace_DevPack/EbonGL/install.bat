set rootdir=c:\tools\
set installdir=c:\tools\hcilibs_64bit_vs2010\

@echo off

REM ############### Copy DLLs ###############
REM #xcopy x64\Debug\libebongl_d.dll %installdir%bin\ /Y
REM #IF %errorlevel% NEQ 0 GOTO copy_failed

REM #xcopy x64\Release\libebongl.dll %installdir%bin\ /Y
REM #IF %errorlevel% NEQ 0 GOTO copy_failed

REM #xcopy x64\Debug\libebongl_d.dll %rootdir%allbin\ /Y
REM #IF %errorlevel% NEQ 0 GOTO copy_failed

REM #xcopy x64\Release\libebongl.dll %rootdir%allbin\ /Y
REM #IF %errorlevel% NEQ 0 GOTO copy_failed

REM ############### Copy Libs ###############
xcopy x64\Debug\libebongl_d.lib %installdir%lib\ /Y
IF %errorlevel% NEQ 0 GOTO copy_failed

xcopy x64\Release\libebongl.lib %installdir%lib\ /Y
IF %errorlevel% NEQ 0 GOTO copy_failed

REM ############### Copy Header Files ###############
echo Deleting any pre-existing header files...
del %installdir%include\libebongl\*.hpp
echo Complete

for %%I in (libebongl\*.hpp) do xcopy %%I %installdir%include\libebongl\ /Y
IF %errorlevel% NEQ 0 GOTO copy_failed

xcopy EbonGL.hpp %installdir%include\ /Y
IF %errorlevel% NEQ 0 GOTO copy_failed

echo Installation complete!
exit /B

REM ############### Show an Error Message ###############
:copy_failed
echo Aborting due to errors!
