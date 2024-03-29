#!/bin/sh

ISODIR=image

export ENV=$1

make clean
make os.bin
make clean

# ensure that the multiboot header was set
# correctly in the binary
if grub-file --is-x86-multiboot os.bin; then
    echo Multiboot Confirmed
else
    echo Multiboot Failed
    exit
fi

cp os.bin $ISODIR/boot/os.bin
cp grub.cfg $ISODIR/boot/grub/grub.cfg
grub-mkrescue -o os.iso $ISODIR