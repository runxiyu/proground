#!/usr/bin/env -S tcc -run

#include <elf.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// ELF is defined assuming 8-bit bytes
#include <stdint.h>

int main64(int fd, char endianness);

int main(void) {
        int fd = open("./Library.so", O_RDONLY);

        unsigned char e_ident[EI_NIDENT];
        if (read(fd, e_ident, EI_NIDENT) != EI_NIDENT)
                return 1;

        if (e_ident[EI_MAG0] != ELFMAG0 || e_ident[EI_MAG1] != ELFMAG1 || e_ident[EI_MAG2] != ELFMAG2 || e_ident[EI_MAG3] != ELFMAG3) {
                puts("This doesn't seem to be an ELF file.");
                return 1;
        }

        char endianness;
        switch (e_ident[EI_DATA]) {
                case ELFDATA2LSB:
                        endianness = 0;
                        break;
                case ELFDATA2MSB:
                        endianness = 1;
                        break;
                default:
                        puts("Your ELF file seems invalid.");
                        return 1;
        }

        switch (e_ident[EI_CLASS]) {
                case ELFCLASS32:
                        puts("32-bit ELF loading not yet implemented.");
                        return 1;
                case ELFCLASS64:
                        return main64(fd, endianness);
                default:
                        puts("Your ELF file seems invalid.");
                        return 1;
        }

        return 2;
}

int main64(int fd, char endianness) {
        Elf64_Ehdr elf_header;
        if (pread(fd, &elf_header, sizeof(elf_header), 0) != sizeof(elf_header))
                return 1;

        if (elf_header.e_type != ET_DYN) {
                puts("This is not a shared library.");
                return 1;
        }

        if (elf_header.e_version != EV_CURRENT) {
                puts("Well... at the very least, the naming here is poor.");
                return 1;
        }



        Elf64_Shdr *section_headers;
        section_headers = calloc(sizeof(*section_headers), elf_header.e_shnum);
        if (!section_headers && elf_header.e_shnum != 0) {
                puts("OOM.");
                return 1;
        }

        if (elf_header.e_shentsize != sizeof(*section_headers)) {
                puts("Your ELF file seems wrong.");
                return 1;
        }

        size_t shnum = elf_header.e_shnum;
        for (size_t i = 0; i < shnum; i++) {
                size_t remaining = elf_header.e_shentsize;
                size_t got = 0;
                ssize_t res;
                do {
                        res = pread(fd, &(section_headers[i]), remaining, elf_header.e_shoff + (elf_header.e_shentsize * i));
                        if (res < 0) {
                                puts("Failed to read ELF section headers");
                                return 1;
                        }
                        got += res;
                        remaining -= res;
                } while (remaining != 0);
        }



        Elf64_Phdr *program_headers;
        program_headers = calloc(sizeof(*program_headers), elf_header.e_phnum);
        if (!program_headers && elf_header.e_phnum != 0) {
                puts("OOM.");
                return 1;
        }

        if (elf_header.e_phentsize != sizeof(*program_headers)) {
                puts("Your ELF file seems wrong.");
                return 1;
        }

        size_t phnum = elf_header.e_phnum;
        if (phnum >= PN_XNUM) {
                if (shnum == 0) {
                        puts("Your ELF file is broken.");
                        return 1;
                }
        }

        for (size_t i = 0; i < phnum; i++) {
                size_t remaining = elf_header.e_phentsize;
                size_t got = 0;
                ssize_t res;
                do {
                        res = pread(fd, &(program_headers[i]), remaining, elf_header.e_phoff + (elf_header.e_phentsize * i));
                        if (res < 0) {
                                puts("Failed to read ELF section headers");
                                return 1;
                        }
                        got += res;
                        remaining -= res;
                } while (remaining != 0);
        }



        for (size_t i = 0; i < phnum; i++) {
                switch (program_headers[i].p_type) {
                case PT_LOAD:
                        break;
                case PT_DYNAMIC:
                        break;
                default:
                        break;
                }
        }

        return 0;
}
