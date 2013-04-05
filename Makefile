GCC = nspire-gcc
LD = nspire-ld
GCCFLAGS = -Os -nostdlib -Wall -W -marm -Werror
# Feel free to comment the following lines out if you're
# not using git or you don't want the build date included
BUILDFLAGS = -DBUILD_DATE="\"$(shell date --rfc-2822)\""
BUILDFLAGS+= -DGIT_COMMIT="\"$(shell git rev-parse --short HEAD)\""
LDFLAGS = -lnspireio
OBJCOPY := "$(shell which arm-elf-objcopy 2>/dev/null)"
ifeq (${OBJCOPY},"")
	OBJCOPY := arm-none-eabi-objcopy
endif
EXE = linuxloader2.tns
OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
DISTDIR = .
vpath %.tns $(DISTDIR)

all: $(EXE)

%.o: %.c
	$(GCC) $(BUILDFLAGS) $(GCCFLAGS) -c $<

$(EXE): $(OBJS)
	$(LD) $^ -o $(@:.tns=.elf) $(LDFLAGS)
	mkdir -p $(DISTDIR)
	$(OBJCOPY) -O binary $(@:.tns=.elf) $(DISTDIR)/$@

clean:
	rm -f *.o *.elf
	rm -f $(DISTDIR)/$(EXE)
