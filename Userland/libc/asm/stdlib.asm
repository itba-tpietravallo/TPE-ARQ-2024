GLOBAL sys_write

section .text

sys_write:
    push rbp
    mov rbp, rsp
    mov rax, 0x01
    int 0x80
    mov rsp, rbp
    pop rbp
    ret
