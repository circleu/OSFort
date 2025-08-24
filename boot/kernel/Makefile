CURDIR = boot/kernel
INCLUDEOBJ = tmp/dev/acpi/acpi.o tmp/inc/asm.o tmp/sys/string.o tmp/dev/pci/pcie.o
CFLAGS = -ffreestanding -nostdlib -nolibc -fno-stack-protector -m64 -masm=att -mno-sse -mno-mmx -mno-80387 -Iinc
SOURCE = $(wildcard $(CURDIR)/*.c)
OBJECT = $(subst $(CURDIR)/,tmp/$(CURDIR)/,$(patsubst %.c,%.o,$(SOURCE)))
TARGET = build/boot/kernel


all: $(TARGET)

$(TARGET) : $(OBJECT) tmp/$(CURDIR)/load_gdt.o $(INCLUDEOBJ)
	ld -Tkrnloader.ld -static $^ -o $@

tmp/$(CURDIR)/%.o : $(CURDIR)/%.c
	gcc $(CFLAGS) -c $< -o $@

tmp/$(CURDIR)/load_gdt.o : $(CURDIR)/load_gdt.S
	as -c $^ -o $@