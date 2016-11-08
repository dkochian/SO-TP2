GLOBAL cpuVendor
GLOBAL _accelPIT

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

_accelPIT:
  push rax
  mov al,00110100b      ;channel 0, lobyte/hibyte, rate generator
  out 0x43, al
 
  mov ax,1193         ;ax = 16 bit reload value
  out 0x40,al         ;Set low byte of PIT reload value
  mov al,ah         ;ax = high 8 bits of reload value
  out 0x40,al         ;Set high byte of PIT reload value
  pop rax
  ret
