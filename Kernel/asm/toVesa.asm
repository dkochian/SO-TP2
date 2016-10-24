GLOBAL change

change:
	;Setup es for geting VESA BIOS video infomation...
	mov ax, 0x1000
	mov es, ax
	;Try getting VESA BIOS video infomation...
	;mov di, 0x0000000000005C00
	;mov ax, 4F00h
	;int 10h
	;Try getting VESA BIOS video mode 105h infomation...
	mov di, 0xC000
	mov cx, 105h
	mov ax, 4F01h
	int 10h
	;Try to change video to 1024x768 graphic mode...
	mov ax, 04F02h
	mov bx, 0105h
	int 10h