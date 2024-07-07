%: %.o
	x86_64-linux-gnu-ld -o $@ $<

%.o: %.s
	nasm -f elf64 -o $@ $<
