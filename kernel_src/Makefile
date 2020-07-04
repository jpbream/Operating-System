
OUTPUT = ..

C_OPTIONS = -m32 -std=c99 -I include-freestanding-c99 -ffreestanding -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore
CPP_OPTIONS = -m32 -std=c++17 -I include-freestanding-c++17 -ffreestanding -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASM_OPTIONS = -f elf
LINK_OPTIONS = -melf_i386

OBJECTS = boot.o kernel.o cdtors.o

%.o: %.cpp
	g++ -c -o $@ $< $(CPP_OPTIONS)

%.o: %.c
	gcc -c -o $@ $< $(C_OPTIONS)

%.o: %.asm
	nasm -o $@ $< $(ASM_OPTIONS)

os.bin: link.ld $(OBJECTS)
	ld -T link.ld -o $(OUTPUT)/os.bin $(OBJECTS) $(LINK_OPTIONS)

clean:
	rm -f *.o