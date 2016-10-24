GLOBAL key_pressed
GLOBAL get_key

key_pressed:
	in al, 64h
	and rax, 1h
	ret

get_key:
    in al, 60h
    movzx rax, al
    ret