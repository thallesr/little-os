all: boot.asm
	nasm -f bin ./boot.asm -o ./boot.bin
