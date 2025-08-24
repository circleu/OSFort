CURDIR = boot/efi
CFLAGS = -I$(CURDIR)/gnu-efi/inc -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args -I$(CURDIR)/inc -Iinc
LFLAGS = -shared -Bsymbolic -L$(CURDIR)/gnu-efi/x86_64/lib -L$(CURDIR)/gnu-efi/x86_64/gnuefi -T$(CURDIR)/gnu-efi/gnuefi/elf_x86_64_efi.lds $(CURDIR)/gnu-efi/x86_64/gnuefi/crt0-efi-x86_64.o
OFLAGS = -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10
TARGET = build/boot/EFI/BOOT/BOOTX64.EFI


all: $(TARGET)

$(TARGET) : tmp/$(CURDIR)/bootloader.so
	objcopy $(OFLAGS) $< $@

tmp/$(CURDIR)/%.so : tmp/$(CURDIR)/%.o
	ld $(LFLAGS) $< -o $@ -lgnuefi -lefi

tmp/$(CURDIR)/%.o : $(CURDIR)/%.c
	gcc $(CFLAGS) -c $< -o $@