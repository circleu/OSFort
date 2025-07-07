#!/bin/sh


loopback=`losetup --partscan --show --find disk.img`
mount $loopback"p1" build/p1
mount $loopback"p2" build/p2


make clean
make efiloader

mkdir -p "build/p1/EFI/BOOT"
cp tmp/BOOTX64.EFI "build/p1/EFI/BOOT"


umount build/p1
umount build/p2
losetup -d $loopback