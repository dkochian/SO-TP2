section .text
GLOBAL sleepasm

sleepasm:
	push rsp
	mov rbp,rsp

	mov rax,162
	mov rbx, secs
	int 80h

	pop rbp
	ret

section .data
secs dq 10,0
delay: 
	sec dd 5
	nsec dd 100000000