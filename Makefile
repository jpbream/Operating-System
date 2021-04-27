OUTPUT = .

PROJDIR := $(realpath $(CURDIR))

SOURCEDIR := $(PROJDIR)/src
BUILDDIR := $(PROJDIR)/bin

SOURCEDIRS := $(shell find $(SOURCEDIR) -not -path '*/\.*' -not -path '../builder' -type d)
SOURCES := $(shell find $(SOURCEDIR) -type f \( -name '*.c' -o -name '*.cpp' -o -name '*.asm' \))

INCLUDES = $(foreach dir, $(SOURCEDIRS), $(addprefix -I, $(dir)))

OBJDIRS := $(subst $(SOURCEDIR),$(BUILDDIR),$(SOURCEDIRS))
OBJS := $(subst $(SOURCEDIR),$(BUILDDIR),$(patsubst %.cpp,%.o,$(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(SOURCES)))))

C_OPTIONS = -g -MD -m32 -std=c99 -I include-freestanding-c99 -ffreestanding -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -ffloat-store $(INCLUDES)
CPP_OPTIONS = -g -MD -m32 -std=c++17 -I include-freestanding-c++17 -ffreestanding -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -ffloat-store $(INCLUDES)
ASM_OPTIONS = -f elf
LINK_OPTIONS = -melf_i386 -L $(OUTPUT)/lib/ -lgcc

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	@g++ -c -o $@ $< $(CPP_OPTIONS) -D $(ENV)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@gcc -c -o $@ $< $(C_OPTIONS) -D $(ENV)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.asm
	@nasm -o $@ $< $(ASM_OPTIONS) -D $(ENV)

dirs:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(OBJDIRS)

os.bin: clean dirs link.ld $(OBJS)
	@ld -T link.ld -o $(OUTPUT)/os.bin $(OBJS) $(LINK_OPTIONS)

clean:
	@rm -rf $(BUILDDIR)/*


-include $(OBJS:.o=.d)
