section .text
GLOBAL seconds
GLOBAL minutes
GLOBAL hour
GLOBAL day
GLOBAL month
GLOBAL year
GLOBAL dayweak

section .text

clock:
	push rax
	mov al, 0Bh
	out 70h, al
	in al, 71h
	or al, byte 00000100b
	out 71h, al 
	pop rax
	out 70h, al
	in  al, 71h
	movzx rax, al
	ret

seconds:
	mov al, 0x00
	call clock
	ret

minutes:
	mov al, 0x02
	call clock
	ret

hour:
	mov al, 0x04
	call clock
	ret

dayweak:
	mov al, 0x06
	call clock
	ret

day:
	mov al, 0x07
	call clock
	ret

month:
	mov al, 0x08
	call clock
	ret

year: 
	mov al, 0x09
	call clock
	ret
