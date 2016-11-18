GLOBAL _syscall
GLOBAL _int81

section .text

_syscall:
	push rbp
	mov rbp, rsp

	int 80h

	mov rsp, rbp
	pop rbp
	ret

_int81:
	push rbp
	mov rbp, rsp

	int 81h

	mov rsp, rbp
	pop rbp
	ret