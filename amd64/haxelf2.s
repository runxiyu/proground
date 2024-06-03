; elf thing in assembly

; Written by: Test_User <hax@andrewyu.org>
; 
; This is free and unencumbered software released into the public
; domain.
; 
; Anyone is free to copy, modify, publish, use, compile, sell, or
; distribute this software, either in source code form or as a compiled
; binary, for any purpose, commercial or non-commercial, and by any
; means.
; 
; In jurisdictions that recognize copyright laws, the author or authors
; of this software dedicate any and all copyright interest in the
; software to the public domain. We make this dedication for the benefit
; of the public at large and to the detriment of our heirs and
; successors. We intend this dedication to be an overt act of
; relinquishment in perpetuity of all present and future rights to this
; software under copyright law.
; 
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
; EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
; MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
; IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
; OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
; ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
; OTHER DEALINGS IN THE SOFTWARE.

[bits 64]

[section elf_header start=0]

ELF_HEADER_START:
db 0x7F, "E", "L", "F" ;	first 4 bytes
db 0x02 ;			64-bit
db 0x01 ;			little-endian
db 0x01 ;			version indicator, currently must be 1
db 0x00 ;			generic unix thing
db 0x00 ;			yes
times 16 - ($ - $$) db 0x00	; padding

dw 0x0002 ;			executable thing
dw 0x003E ;			x86_64
dd 0x00000001 ;			version again
dq entry ;			starting address
dq PROGRAM_HEADER_START ;	where the program header is in the file
dq 0x0000000000000000 ;		no section header
dd 0x00000000 ;			no flags
dw ELF_HEADER_SIZE ;		needed
dw PROGRAM_HEADER_SIZE ;	how large is each program header
dw 0x0001 ;			number of program headers
dw 0x0000 ;			no section size because no sections
dw 0x0000 ;			no sections
dw 0x0000 ;			no string header thing because no sections

ELF_HEADER_SIZE equ $ - ELF_HEADER_START

PROGRAM_HEADER_START:
dd 0x00000001 ;			load this into memory
dd 0x00000007 ;			read | execute | write, apparently possible
dq FILE_START ;			offset in file
dq RAM_START ;			offset in virtual address
dq 0x0000000000000000 ;		physical location, doesn't really matter
dq EXECUTABLE_SECTION_SIZE ;	size in file
dq EXECUTABLE_SECTION_RAM ;	size in ram
dq 0x0000000000000000 ;		no alignment required

PROGRAM_HEADER_SIZE equ $ - PROGRAM_HEADER_START

PROGRAM_HEADER_END:

FILE_START EQU $-$$
MAP_START EQU 0x400000
[section executable vstart=0x0000000000400000+FILE_START align=1]
CLONE_VM EQU 0x00000100

RAM_START:

other_thread:
mov rax, 0x3B
mov rdi, subprocess_file
xor rsi, rsi
xor rdx, rdx
syscall

; if exec fails for some reason...
mov BYTE [exec_fail], 1

exit:
mov rax, 0x3C
mov rdi, 0x01
syscall

entry:

unmap_junk:
; MAP_END: first byte outside of our mapping
; MAP_START: first byte inside out our mapping
xor rdi, rdi
mov rsi, MAP_START
mov rax, 0x0B
syscall

mov rdi, MAP_END
mov rsi, 0x7FFFFFFFF000 ; top of address space
sub rsi, rdi
mov rax, 0x0B
syscall

spawn:
mov rax, 0x38
mov rdi, CLONE_VM ; flags
; xor rsi, rsi ; stack pointer... who needs a stack anyways :D
syscall
test rax, rax
jz other_thread

.wait_loop:
mov rax, 0xF7
xor rdi, rdi ; P_ALL
; rsi ignored because P_ALL
xor rdx, rdx ; no infop
mov r10, 0x40000004 ; __WALL | WEXITED
syscall

cmp rax, -10 ; -ECHILD
jne .wait_loop

cmp BYTE [exec_fail], 0
je spawn ; if there was no *critical* failure, like executable missing or whatever

jmp exit

subprocess_file:
db "/bin/bash", 0
EXECUTABLE_SECTION_SIZE equ $ - RAM_START
[absolute 0x0000000000400000+FILE_START+EXECUTABLE_SECTION_SIZE]
UNINITIALIZED_START:
exec_fail:
resb 1

RAM_END:
EXECUTABLE_SECTION_RAM equ EXECUTABLE_SECTION_SIZE + $ - UNINITIALIZED_START
resb (0-($-$$)) % 4096
MAP_END:
