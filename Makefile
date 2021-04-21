OUTPUT = .

ENV := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
$(eval $(ENV):;@:)

PROJDIR := $(realpath $(CURDIR))
SOURCEDIR := $(PROJDIR)/src
BUILDDIR := $(PROJDIR)/bin

SOURCEDIRS := $(shell find $(SOURCEDIR) -not -path '*/\.*' -not -path '../builder' -type d)
SOURCES := $(shell find $(SOURCEDIR) -type f \( -name '*.c' -o -name '*.cpp' -o -name '*.asm' \))

INCLUDES = $(foreach dir, $(SOURCEDIRS), $(addprefix -I, $(dir)))

OBJDIRS := $(subst $(SOURCEDIR),$(BUILDDIR),$(SOURCEDIRS))
OBJS := $(subst $(SOURCEDIR),$(BUILDDIR),$(patsubst %.cpp,%.o,$(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(SOURCES)))))

C_OPTIONS = -MD -m32 -std=c99 -I include-freestanding-c99 -ffreestanding -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -ffloat-store -O2 $(INCLUDES)
CPP_OPTIONS = -MD -m32 -std=c++17 -I include-freestanding-c++17 -ffreestanding -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffloat-store -O2 $(INCLUDES)
ASM_OPTIONS = -f elf
LINK_OPTIONS = -melf_i386 -L $(OUTPUT)/lib/ -lgcc

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	g++ -c -o $@ $< $(CPP_OPTIONS) -D $(ENV)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	gcc -c -o $@ $< $(C_OPTIONS) -D $(ENV)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.asm
	nasm -o $@ $< $(ASM_OPTIONS)

build:
	mkdir -p $(BUILDDIR)
	mkdir -p $(OBJDIRS)

os.bin: link.ld $(OBJS)
	ld -T link.ld -o $(OUTPUT)/os.bin $(OBJS) $(LINK_OPTIONS)

clean:
	rm -rf $(BUILDDIR)/*

-include $(OBJS:.o=.d)
