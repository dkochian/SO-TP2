GLOBAL getXResolution
GLOBAL getYResolution
GLOBAL setXResolution
GLOBAL setYResolution
GLOBAL getRAM

section .text

getXResolution:
	mov rax, 0
	mov al, [0x5084]
	ret

getYResolution:
	mov rax, 0
	mov al, [0x5086]
	ret

getRAM:
	mov rax, [0x5020]
	ret

setXResolution:
	ret

setYResolution:
	ret
