@echo off
rem Build TesPassThruSocketCAN.dll (x86). Requires VS2019 Build Tools (or edit the path below).
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x86
if ERRORLEVEL 1 (echo vcvarsall failed - check your Visual Studio Build Tools path & exit /b 1)
cd /d "%~dp0"
cl /nologo /W3 /O2 /LD TesPassThruSocketCAN.c /Fe:TesPassThruSocketCAN.dll ^
   /link /DEF:TesPassThruSocketCAN.def /MACHINE:X86 ws2_32.lib advapi32.lib kernel32.lib
if %ERRORLEVEL% EQU 0 (
    echo.
    echo BUILD OK  -^>  %~dp0TesPassThruSocketCAN.dll
) else (
    echo.
    echo BUILD FAILED
    exit /b 1
)
