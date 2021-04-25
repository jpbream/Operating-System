qemu-system-i386                                 ^
  -accel tcg,thread=single                       ^
  -cpu core2duo                                  ^
  -m 128                                         ^
  -no-reboot                                     ^
  -drive format=raw,media=cdrom,file=os.iso      ^
  -serial stdio                                  ^
  -smp 1                                         ^
  -usb                                           ^
  -vga std

pause