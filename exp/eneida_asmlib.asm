format MS64 COFF

extrn '__imp_LoadLibraryA' as LoadLibraryA:qword
extrn '__imp_GetProcAddress' as GetProcAddres:qword
extrn '__imp_RtlFillMemory' as RtlFillMemory:qword

public memset

section '.text' code readable executable

memset:
    sub rsp, 40             ; align stack to 16 bytes and reserve shadow space
    mov r9d, edx
    mov rdx, r8
    mov r8d, r9d
    call [RtlFillMemory]
    add rsp, 40
    ret

; vim: ft=fasm :
