#!/bin/sh

ISODIR=image

cd kernel_src
make os.bin
make clean
cd ..

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