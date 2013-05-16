include libfdt/Makefile.libfdt
GCC = nspire-gcc
LD = nspire-ld
GCCFLAGS = -mcpu=arm926ej-s -Os -nostdlib -Wall -W -marm -Ilibfdt/

# Feel free to comment the following lines out if you're
# not using git or you don't want the build date included
BUILDFLAGS := -DBUILD_DATE="\"$(shell date --rfc-2822)\""
BUILDFLAGS += -DGIT_COMMIT="\"$(shell git rev-parse --short HEAD)\""

LDFLAGS = -lnspireio
OBJCOPY := "$(shell which arm-elf-objcopy 2>/dev/null)"
ifeq (${OBJCOPY},"")
	OBJCOPY := arm-none-eabi-objcopy
endif
EXE = linuxloader2.tns

OBJS  = $(patsubst %.c,%.o,$(wildcard *.c))
OBJS += $(patsubst %.S,%.o,$(wildcard *.S))
OBJS += $(addprefix libfdt/, $(LIBFDT_OBJS))

vpath %.tns $(DISTDIR)

all: $(EXE)

%.o: %.c
	$(GCC) $(BUILDFLAGS) $(GCCFLAGS) -c $< -o $@
%.o: %.S
	$(GCC) $(BUILDFLAGS) $(GCCFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	$(LD) $^ -o $(@:.tns=.elf) $(LDFLAGS)
	$(OBJCOPY) -O binary $(@:.tns=.elf) $@

clean:
	rm -f $(OBJS) *.elf
	rm -f $(EXE)

