GLOBAL _outpw
GLOBAL _inpw

_outpw:
	push rbp
	mov rbp, rsp

	push rbx

	mov	dx,di
	mov	ax,si
	out	dx,ax

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

_inpw:
	push rbp
	mov rbp, rsp

	push rbx

	mov	dx,di
	in	ax,dx

	pop rbx

	mov rsp, rbp
	pop rbp
	ret