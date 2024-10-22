GLOBAL sys_beep

GLOBAL sys_fonts_text_color
GLOBAL sys_fonts_background_color
GLOBAL sys_fonts_decrease_size
GLOBAL sys_fonts_increase_size
GLOBAL sys_clear_screen

GLOBAL sys_hour
GLOBAL sys_minute
GLOBAL sys_second

section .text

%macro sys_int80 1
    push rbp
    mov rbp, rsp
    mov rax, %1
    int 0x80
    mov rsp, rbp
    pop rbp
    ret
%endmacro

sys_beep: sys_int80 0x80000000

sys_fonts_text_color: sys_int80 0x80000002
sys_fonts_background_color: sys_int80 0x80000003



sys_fonts_decrease_size: sys_int80 0x80000007
sys_fonts_increase_size: sys_int80 0x80000008

sys_clear_screen: sys_int80 0x8000000A


sys_hour: sys_int80 0x80000010
sys_minute: sys_int80 0x80000011
sys_second: sys_int80 0x80000012
