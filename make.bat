@echo off

set ASM=.\vc2015-toolchain\fasm.exe
set CPP=.\vc2015-toolchain\cl.exe
set HLSL=.\vc2015-toolchain\fxc /Ges /O3 /WX /nologo /Qstrip_reflect /Qstrip_debug /Qstrip_priv

if exist *.exe del *.exe
if exist *.pdb del *.pdb
if exist *.obj del *.obj
if exist hlsl\*.h del hlsl\*.h

%ASM% eneida_asmlib.asm
if errorlevel 1 goto fail

:: %HLSL% /E vs_full_triangle /Fo data\vs_full_triangle.cso /T vs_5_1 hlsl\eneida.hlsl
:: if errorlevel 1 goto fail
:: %HLSL% /E ps_sketch0 /Fo data\ps_sketch0.cso /T ps_5_1 hlsl\eneida.hlsl
:: if errorlevel 1 goto fail

:: /Fm generates linker map file
:: /Fa generates assembly listing
%CPP% ^
/Zi /Od /D_DEBUG /Gm- /nologo /WX /W3 /GS- /Gs999999 /Gy /Gw /EHa- eneida.cpp ^
/link eneida_asmlib.obj vc2015-toolchain\kernel32.lib ^
/OPT:REF /INCREMENTAL:NO /SUBSYSTEM:WINDOWS /ENTRY:Start /NODEFAULTLIB
if errorlevel 1 goto fail

set STATUS=0
goto eof

:fail
set STATUS=1

:eof
if exist vc140.pdb del vc140.pdb
if exist *.obj del *.obj
if exist hlsl\*.h del hlsl\*.h
exit %STATUS%
