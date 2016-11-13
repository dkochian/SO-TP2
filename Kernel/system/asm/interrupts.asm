GLOBAL _timerTickHandler
GLOBAL _keyboardHandler
GLOBAL _sysCallHandler
GLOBAL _write_port
GLOBAL _cli
GLOBAL _sti
GLOBAL _enter_region
GLOBAL _leave_region

EXTERN timerTickHandler
EXTERN keyboardHandler
EXTERN sysCallHandler

SECTION .text


;http://f.osdev.org/viewtopic.php?f=1&t=23617
;**************************************************************************************
;PUSHAQ:    Emulates the 'pushaq instruction' under long mode.
;**************************************************************************************
align 8
%macro pushaq 0
    
  ;Save registers to the stack.
  ;--------------------------------

  push rax      ;save current rax
  push rbx      ;save current rbx
  push rcx      ;save current rcx
  push rdx      ;save current rdx
  push rbp      ;save current rbp
  push rdi      ;save current rdi
  push rsi      ;save current rsi
  push r8       ;save current r8
  push r9       ;save current r9
  push r10      ;save current r10
  push r11      ;save current r11
  push r12      ;save current r12
  push r13      ;save current r13
  push r14      ;save current r14
  push r15      ;save current r15
  push gs
  push fs
  
  %endmacro ;end of macro definition

;**************************************************************************************
;POPAQ:    Emulates the 'popaq instruction' under long mode.;
;**************************************************************************************
align 8

%macro popaq 0
  ;Restore registers from the stack.
  ;--------------------------------

  pop fs
  pop gs
  pop r15         ;restore current r15
  pop r14         ;restore current r14
  pop r13         ;restore current r13
  pop r12         ;restore current r12
  pop r11         ;restore current r11
  pop r10         ;restore current r10
  pop r9          ;restore current r9
  pop r8          ;restore current r8
  pop rsi         ;restore current rsi
  pop rdi         ;restore current rdi
  pop rbp         ;restore current rbp
  pop rdx         ;restore current rdx
  pop rcx         ;restore current rcx
  pop rbx         ;restore current rbx
  pop rax         ;restore current rax

%endmacro;end of macro definition

;------------------------------------------------------------
; sti wrapper for C
;------------------------------------------------------------
_sti:
  sti
  ret

;------------------------------------------------------------
; cli wrapper for C
;------------------------------------------------------------
_cli:
  cli
  ret;

_write_port:
    push rbp
    mov rbp, rsp

    mov edx, edi    
    mov rax, rsi
    and rax, 0xFFFF  
    out dx, al
    
    mov rsp,rbp
    pop rbp  
    ret

;------------------------------------------------------------
; Timer tick idt handler -> processed in C
;------------------------------------------------------------
_timerTickHandler:
  cli
  pushaq

  mov rdi, rsp
  call timerTickHandler
  mov rbx, rax            ;backup rax and use rbx instead

  mov al,20h
  out 20h,al

  cmp rbx, 0
  je skip
  mov rsp, rbx

skip:
  popaq
  
  sti
  iretq

;------------------------------------------------------------
; keyboard idt handler -> processed in C
;------------------------------------------------------------
_keyboardHandler:
  cli
  push rdi
  push rax

  xor rax, rax
  in al, 60h
  and rax,0xFF
  xor rdi,rdi
  mov rdi,rax
  call keyboardHandler

  mov al,20h
  out 20h,al

  pop rax
  pop rdi

  sti
  iretq

;------------------------------------------------------------
; syscalls idt handler -> processed in C
;------------------------------------------------------------

_sysCallHandler:
  cli
  pushaq  
  call sysCallHandler
  popaq
  sti
  iretq


;_lock:
;  ts      rax, rcx   // load word into R0
;  bnz     rax, 0          // if 0, lock obtained

;_unlock:
;  st      rcx, 0   // store 0 to address

_enter_region:        ; A "jump to" tag; function entry point.
  push rdi
  push rax
  mov rdi, rax
  mov rax,1
  xchg rax,rdi      ; Test and Set Lock; flag is the
                     ; shared variable; it is copied
                     ; into the register rax and flag
                     ; then atomically set to 1.

  cmp rax,0        ; Was flag zero on entry_region?

  jne _enter_region   ; Jump to enter_region if
                     ; rax is non-zero; i.e.,
                     ; flag was non-zero on entry.
  pop rax
  pop rdi

  ret                ; Exit; i.e., flag was zero on
                     ; entry. If we get here, tsl
                     ; will have set it non-zero; thus,
                     ; we have claimed the resource
                     ; associated with flag.

_leave_region:
  mov rcx,0      ; store 0 in flag
  ret                ; return to caller
