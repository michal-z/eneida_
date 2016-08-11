format MS64 COFF

extrn '__imp_LoadLibraryA' as LoadLibraryA:qword
extrn '__imp_GetProcAddress' as GetProcAddres:qword
extrn '__imp_RtlFillMemory' as RtlFillMemory:qword

public memset

section '.text' code readable executable

memset:
            push        r12
            sub         rsp, 32                 ; reserve shadow space
            mov         r12, rcx                ; save dest pointer
            mov         r9d, edx
            mov         rdx, r8
            mov         r8d, r9d
            call        [RtlFillMemory]
            mov         rax, r12
            add         rsp, 32
            pop         r12
            ret

; vim: ft=fasm autoindent st=12 sts=12 sw=12 :
