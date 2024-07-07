global _start

section .text

_start:
	mov rax, 1        ; write
	mov rdi, 1        ; stdout
	mov rsi, msg
	mov rdx, msglen
	syscall

	mov rax, 60       ; exit
	mov rdi, 0
	syscall

section .rodata
	msg: db "Hello, world!", 10
	msglen: equ $ - msg
