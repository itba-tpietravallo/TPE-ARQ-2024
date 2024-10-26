GLOBAL cpuVendor
GLOBAL getKeyboardBuffer

GLOBAL getSecond
GLOBAL getMinute
GLOBAL getHour

GLOBAL setPITMode
GLOBAL setPITFrequency
GLOBAL setSpeaker

GLOBAL takeRegisterSnapshot
GLOBAL getRegisterSnapshot

section .text

getKeyboardBuffer:
	push rbp
	mov rbp, rsp

	in al, 60h

	mov rsp, rbp
	pop rbp

	ret


getSecond:
	push rbp
	mov rbp, rsp

	mov al, 0
	out 70h, al
	in al, 71h

	mov rsp, rbp
	pop rbp

	ret


getMinute:
	push rbp
	mov rbp, rsp

	mov al, 2
	out 70h, al
	in al, 71h

	mov rsp, rbp
	pop rbp

	ret


getHour:
	push rbp
	mov rbp, rsp

	mov al, 4
	out 70h, al
	in al, 71h

	mov rsp, rbp
	pop rbp

	ret


setPITMode:
	push rbp
	mov rbp, rsp
	
	mov rax, rdi
	out 0x43, al

	mov rsp, rbp
	pop rbp

	ret


setPITFrequency:
	push rbp
	mov rbp, rsp

	mov rax, rdi
	out 0x42, al
	mov al, ah
	out 0x42, al

	mov rsp, rbp
	pop rbp
	ret


setSpeaker:
	push rbp
	mov rbp, rsp

	cmp rdi, 0
	je .off

	.on:
	in al, 0x61
	or al, 0x03
	out 0x61, al
	jmp .end

	.off:
	in al, 0x61
	and al, 0xFC
	out 0x61, al

	.end:
	mov rsp, rbp
	pop rbp
	ret

takeRegisterSnapshot:
	cli

	mov [register_snapshot + 0x00], rax ; 1
	mov [register_snapshot + 0x08], rbx ; 2
	mov [register_snapshot + 0x10], rcx ; 3
	mov [register_snapshot + 0x18], rdx ; 4
	mov [register_snapshot + 0x20], rbp ; 5
	mov [register_snapshot + 0x28], rdi ; 6
	mov [register_snapshot + 0x30], rsi ; 7
	mov [register_snapshot + 0x38], r8 	; 8
	mov [register_snapshot + 0x40], r9 	; 9
	mov [register_snapshot + 0x48], r10 ; 10
	mov [register_snapshot + 0x50], r11 ; 11
	mov [register_snapshot + 0x58], r12 ; 12
	mov [register_snapshot + 0x60], r13 ; 13
	mov [register_snapshot + 0x68], r14 ; 14
	mov [register_snapshot + 0x70], r15 ; 15
	lea rax, [rel $+0x0]	; https://stackoverflow.com/a/12397650, https://stackoverflow.com/questions/69854542/force-rip-relative-access-to-fixed-offset-in-nasm
	mov [register_snapshot + 0x78], rax ; 16
	pushfq ; https://www.felixcloutier.com/x86/pushf:pushfd:pushfq
	pop rax
	mov [register_snapshot + 0x80], rax ; 17

	mov byte [register_snapshot_taken], 0x01

	sti
	ret

getRegisterSnapshot:
	push rbp
	mov rbp, rsp
	cmp byte [register_snapshot_taken], 0x01
	jne .no_snapshot
	mov EAX, register_snapshot
	jmp .end
	.no_snapshot:
	mov EAX, 0
	.end:
	mov rsp, rbp
	pop rbp
	ret


section .bss
	register_snapshot_taken resb 1
	register_snapshot resq 17