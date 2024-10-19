GLOBAL sys_beep

section .text

sys_beep:
    push rbp
    mov rbp, rsp
    mov rax, 0x80000000
    int 0x80
    mov rsp, rbp
    pop rbp
    ret