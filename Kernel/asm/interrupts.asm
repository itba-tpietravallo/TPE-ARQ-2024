
GLOBAL _cli
GLOBAL _sti
GLOBAL _hlt

GLOBAL picMasterMask
GLOBAL picSlaveMask

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq80Handler

GLOBAL _exceptionHandler00

EXTERN irqDispatcher
EXTERN syscallDispatcher
EXTERN exceptionDispatcher

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pass argument to irqDispatcher
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pass argument to exceptionDispatcher
	call exceptionDispatcher

	popState
	iretq
%endmacro

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret

_sti:
	sti
	ret

picMasterMask:
	push rbp     ; Stack frame
	mov rbp, rsp
	mov rax, rdi

	out 0x21, al ; https://wiki.osdev.org/8259_PIC#Masking

	mov rsp, rbp
	pop rbp
	ret

picSlaveMask:
	push rbp
	mov rbp, rsp
	mov rax, rdi

	out 0xA1, al ; https://wiki.osdev.org/8259_PIC#Masking

	mov rsp, rbp
	pop rbp
	ret

; 8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

; Keyboard
_irq01Handler:
	irqHandlerMaster 1

; System Call
; Not using the %irqHandlerMaster macro because it needs to pass the stack pointer to the syscall
_irq80Handler:
	pushState

	mov rdi, rsp ; pass REGISTERS (stack) to irqDispatcher, see: `pushState` two lines above
	call syscallDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq

; Zero Division Exception
_exceptionHandler00:
	exceptionHandler 0
