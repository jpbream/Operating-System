mov ah, 0x0e
mov al, 'X'
int 0x10

jmp $

times 510 - ($ - $$) db 0
dw 0xaa55


; https://stackoverflow.com/questions/34268518/creating-a-bootable-iso-image-with-custom-bootloader