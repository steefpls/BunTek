@echo off
REM : This is a handy script to show how to build in command line without 
REM : all the hassle of going through Visual Studio. At the end of the day,
REM : Visual Studio runs something like this when you build your project.

REM : We need to find where 'vcvarsall' is installed and run it.
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64

REM : Compile!!
cl -I CProcessing\inc main.c /link CProcessing\lib\x64\CProcessing.lib LIBCMT.lib /OUT:gam100-gol.exe

REM : Copy the required dlls to current directory
xcopy /Y CProcessing\lib\x64\CProcessing.dll .
xcopy /Y CProcessing\lib\x64\fmod.dll .
