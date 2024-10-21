GLOBAL _divzero
GLOBAL _invalidopcode

_divzero:

    push rbp
    mov rbp, rsp

    mov eax, 10
    xor ebx, ebx
    div ebx

    mov rsp, rbp
    pop rbp
    ret

_invalidopcode:

    push rbp
    mov rbp, rsp

    mov rsp, rbp
    pop rbp
    ret



