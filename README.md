To create the floppy.img file and write a bootloader:
dd if=/dev/zero of=floppy.img bs=1024 count=1440
dd if=bootloader.bin of=floppy.img seek=0 count=1 conv=notrunc

Packages to install when using grub:
grub-common
xorriso
grub-pc-bin

Run build_os.sh to build the OS bin file and iso file.
Burn the iso to a disk to launch on bare metal, or mount in a VM
to test.

The grub source folder is included to satisfy the GPL.