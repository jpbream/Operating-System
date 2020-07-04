
INPUT = kernel_src
OUTPUT = kernel_bin

C_OPTIONS = -m32 -std=c99 -I include-freestanding-c99 -ffreestanding -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASM_OPTIONS = -f elf
LINK_OPTIONS = -melf_i386

$(OUTPUT)/kernel.o: $(INPUT)/kernel.c
	gcc -o $(OUTPUT)/kernel.o $(INPUT)/kernel.c -c $(C_OPTIONS)

$(OUTPUT)/boot.o: $(INPUT)/boot.asm
	nasm -o $(OUTPUT)/boot.o $(INPUT)/boot.asm $(ASM_OPTIONS)

os.bin: link.ld $(OUTPUT)/boot.o $(OUTPUT)/kernel.o
	ld -T link.ld -o os.bin $(OUTPUT)/boot.o $(OUTPUT)/kernel.o $(LINK_OPTIONS)
