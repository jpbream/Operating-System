qemu-system-i386                                 ^
  -accel tcg,thread=single                       ^
  -cpu kvm32,+avx,+pdpe1gb,+sse,+sse2,+xsave,+fpu     ^
  -m 128                                         ^
  -no-reboot                                     ^
  -drive format=raw,media=cdrom,file=os.iso      ^
  -serial stdio                                  ^
  -smp 1                                         ^
  -usb                                           ^
  -vga std