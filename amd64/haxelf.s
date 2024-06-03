; vim: filetype=nasm
; Copyright (c) 2023 by Test_User/hax

[bits 64]

[section elf_header start=0]

ELF_HEADER_START:
db 0x7F, "E", "L", "F" ;        first 4 bytes
db 0x01 ;                       64-bit
db 0x01 ;                       little-endian
db 0x01 ;                       version indicator, currently must be 1
db 0x00 ;                       generic unix thing
db 0x00 ;                       idk what this is
times 16 - ($ - $$) db 0x00     ; padding

dw 0x0002 ;                     executable thing
dw 0x0003 ;                     x86_64
dd 0x00000001 ;                 version again
dd ram_start ;                  starting address
dd PROGRAM_HEADER_START ;       where the program header is in the file
dd 0x00000000 ;                 no section header
dd 0x00000000 ;                 no flags
dw ELF_HEADER_SIZE ;            needed
dw PROGRAM_HEADER_SIZE ;        how large is each program header
dw 0x0001 ;                     number of program headers
dw 0x0000 ;                     no section size because no sections
dw 0x0000 ;                     no sections
dw 0x0000 ;                     no string header thing because no sections

ELF_HEADER_SIZE equ $ - ELF_HEADER_START

PROGRAM_HEADER_START:
dd 0x00000001 ;                 load this into memory
dd file_start ;                 offset in file
dd ram_start ;                  offset in virtual address
dd 0x00000000 ;                 physical location, doesn't really matter
dd EXECUTABLE_SECTION_SIZE ;    size in file
dd EXECUTABLE_SECTION_SIZE ;    size in ram
dd 0x00000007 ;                 read | execute | write, apparently possible
dd 0x00000000 ;                 no alignment required

PROGRAM_HEADER_SIZE equ $ - PROGRAM_HEADER_START

PROGRAM_HEADER_END:

file_start EQU $-$$
[section executable vstart=0x0000000000400000+file_start align=1]
ram_start:

mov eax, 4
mov ebx, 1
mov ecx, msg
mov edx, msg_len
int 0x80

mov eax, 1
xor ebx, ebx
int 0x80

msg: db "Hello, world!", 0x0A ; buffer to be written to
msg_len equ $ - msg

EXECUTABLE_SECTION_SIZE equ $ - ram_start
