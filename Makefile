
INPUT = kernel_src
OUTPUT = kernel_bin

C_OPTIONS = -m32 -std=c99 -I include-freestanding-c99 -ffreestanding -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore
CPP_OPTIONS = -m32 -std=c++17 -I include-freestanding-c++17 -ffreestanding -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASM_OPTIONS = -f elf
LINK_OPTIONS = -melf_i386

$(OUTPUT)/kernel.o: $(INPUT)/kernel.cpp
	g++ -o $(OUTPUT)/kernel.o $(INPUT)/kernel.cpp -c $(CPP_OPTIONS)

$(OUTPUT)/cdtors.o: $(INPUT)/cdtors.cpp
	g++ -o $(OUTPUT)/cdtors.o $(INPUT)/cdtors.cpp -c $(CPP_OPTIONS)

$(OUTPUT)/boot.o: $(INPUT)/boot.asm
	nasm -o $(OUTPUT)/boot.o $(INPUT)/boot.asm $(ASM_OPTIONS)

os.bin: link.ld $(OUTPUT)/boot.o $(OUTPUT)/kernel.o $(OUTPUT)/cdtors.o
	ld -T link.ld -o os.bin $(OUTPUT)/boot.o $(OUTPUT)/kernel.o $(OUTPUT)/cdtors.o $(LINK_OPTIONS)
