OUTPUT = .

PROJDIR := $(realpath $(CURDIR))

SOURCEDIR := $(PROJDIR)/src
BUILDDIR := $(PROJDIR)/bin

SOURCEDIRS := $(shell find $(SOURCEDIR) -not -path '*/\.*' -not -path '../builder' -type d)
SOURCES := $(shell find $(SOURCEDIR) -type f \( -name '*.c' -o -name '*.cpp' -o -name '*.asm' \))

INCLUDES = $(foreach dir, $(SOURCEDIRS), $(addprefix -I, $(dir)))

OBJDIRS := $(subst $(SOURCEDIR),$(BUILDDIR),$(SOURCEDIRS))
OBJS := $(subst $(SOURCEDIR),$(BUILDDIR),$(patsubst %.cpp,%.o,$(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(SOURCES)))))

C_OPTIONS = @c_options $(INCLUDES) -D $(ENV)
CPP_OPTIONS = @cpp_options $(INCLUDES) -D $(ENV)
ASM_OPTIONS = -f elf -D $(ENV)
LINK_OPTIONS = -melf_i386 -L $(OUTPUT)/lib/ -lgcc

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	@g++ -c -o $@ $< $(CPP_OPTIONS)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@gcc -c -o $@ $< $(C_OPTIONS)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.asm
	@nasm -o $@ $< $(ASM_OPTIONS)

os.bin: dirs link.ld $(OBJS)
	@ld -T link.ld -o $(OUTPUT)/os.bin $(OBJS) $(LINK_OPTIONS)

dirs:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(OBJDIRS)

clean:
	@rm -rf $(BUILDDIR)/*

-include $(OBJS:.o=.d)
