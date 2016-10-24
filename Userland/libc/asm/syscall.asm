GLOBAL _syscall

section .text

_syscall:
	push rbp
	mov rbp, rsp

	push rax
	push rbx

	mov rax, rdi
	mov rbx, rsi
	;rcx and rdx have the correct information

	int 80h

	pop rbx
	pop rax

	mov rsp, rbp
	pop rbp
	ret