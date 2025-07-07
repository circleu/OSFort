#!/bin/sh


# dd if=/dev/zero of=disk.img bs=512 count=16777216
sgdisk disk.img -o
sgdisk disk.img -n 1:2048:+1046528 -t 1:C12A7328-F81F-11D2-BA4B-00A0C93EC93B
sgdisk disk.img -n 2:1048576:+2097152 -t 2:0FC63DAF-8483-4772-8E79-3D69D8477DE4

loopback=`losetup --partscan --show --find disk.img`
mkfs.fat -F 32 $loopback"p1"
mkfs.ext2 $loopback"p2"
losetup -d $loopback
