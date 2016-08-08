@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64

if exist *.exe del *.exe
if exist *.pdb del *.pdb
if exist *.obj del *.obj
if exist hlsl\*.h del hlsl\*.h

fasm\fasm.exe eneida_asmlib.asm
if errorlevel 1 goto fail

set FXC=fxc /Ges /O3 /WX /nologo /Qstrip_reflect /Qstrip_debug /Qstrip_priv

%FXC% /E vs_full_triangle /Fo data\vs_full_triangle.cso /T vs_5_1 hlsl\eneida.hlsl
if errorlevel 1 goto fail
%FXC% /E ps_sketch0 /Fo data\ps_sketch0.cso /T ps_5_1 hlsl\eneida.hlsl
if errorlevel 1 goto fail


:: /Fm generates linker map file
:: /Fa generates assembly listing
cl /Zi /Od /D_DEBUG /arch:AVX2 /Gm- /nologo /WX /D_CRT_SECURE_NO_WARNINGS /W3 /GS- /Gs999999 /Gy /Gw /EHa- eneida.cpp ^
   /link eneida_asmlib.obj d3d12.lib dxgi.lib dxguid.lib kernel32.lib user32.lib gdi32.lib ^
   /OPT:REF /INCREMENTAL:NO /SUBSYSTEM:WINDOWS /ENTRY:Start /NODEFAULTLIB
if errorlevel 1 goto fail

if exist vc140.pdb del vc140.pdb
if exist *.obj del *.obj
if exist hlsl\*.h del hlsl\*.h
exit 0

:fail
pause
if exist vc140.pdb del vc140.pdb
if exist *.obj del *.obj
if exist hlsl\*.h del hlsl\*.h
exit 1
