@echo off
setlocal
set "lock=%temp%\wait%random%.lock"

set ASM=.\vc2015-toolchain\fasm.exe
set CPP=.\vc2015-toolchain\cl.exe
set HLSL=.\vc2015-toolchain\fxc /Ges /O3 /WX /nologo /Qstrip_reflect /Qstrip_debug /Qstrip_priv

if exist *.exe del *.exe
if exist *.pdb del *.pdb
if exist *.obj del *.obj
if exist *.h del *.h

%ASM% eneida_asmlib.asm
if errorlevel 1 goto :fail



:: Launch processes asynchronously, with stream 9 redirected to a lock file.
:: The lock file will remain locked until the script ends.
start "" /B 9>"%lock%1" %HLSL% /D_s00 /Vn s_s00 /E main /Fh s00.h /T vs_5_1 eneida.hlsl
start "" /B 9>"%lock%2" %HLSL% /D_s01 /Vn s_s01 /E main /Fh s01.h /T ps_5_1 eneida.hlsl
start "" /B 9>"%lock%3" %HLSL% /D_s02 /Vn s_s02 /E main /Fh s02.h /T cs_5_1 eneida.hlsl

:Wait for all processes to finish (wait until lock files are no longer locked)
1>nul 2>nul ping /n 2 ::1
for %%F in ("%lock%*") do (
  (call ) 9>"%%F" || goto :Wait
) 2>nul

::delete the lock files
del "%lock%*"



:: /Fm generates linker map file
:: /Fa generates assembly listing
%CPP% ^
/Zi /Od /D_DEBUG /Gm- /nologo /WX /W3 /GS- /Gs999999 /Gy /Gw /EHa- eneida.cpp ^
/link eneida_asmlib.obj vc2015-toolchain\kernel32.lib ^
/OPT:REF /INCREMENTAL:NO /SUBSYSTEM:WINDOWS /ENTRY:Start /NODEFAULTLIB
if errorlevel 1 goto :fail

set STATUS=0
goto :cleanup

:fail
set STATUS=1

:cleanup
if exist vc140.pdb del vc140.pdb
if exist *.obj del *.obj
if exist *.h del *.h
exit /B %STATUS%
