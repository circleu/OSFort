all:
	mkdir -p build/boot/EFI/BOOT
	make -f boot/efi/Makefile
	make -f dev/acpi/Makefile
	make -f dev/pci/Makefile
	make -f inc/Makefile
	make -f sys/Makefile
	make -f boot/kernel/Makefile

clean:
	rm -r tmp/*
	mkdir tmp/boot
	mkdir tmp/boot/efi
	mkdir tmp/boot/kernel
	mkdir tmp/dev
	mkdir tmp/dev/acpi
	mkdir tmp/dev/ahci
	mkdir tmp/dev/pci
	mkdir tmp/dev/pic
	mkdir tmp/fs
	mkdir tmp/inc
	mkdir tmp/sys
	cp tmp/.dummy tmp/boot/efi
	cp tmp/.dummy tmp/boot/kernel
	cp tmp/.dummy tmp/dev/acpi
	cp tmp/.dummy tmp/dev/ahci
	cp tmp/.dummy tmp/dev/pci
	cp tmp/.dummy tmp/dev/pic
	cp tmp/.dummy tmp/fs
	cp tmp/.dummy tmp/inc
	cp tmp/.dummy tmp/sys
