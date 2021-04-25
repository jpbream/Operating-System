
; multiboot header, recognized by the GRUB bootloader
; and launches the kernel main function


MAGIC           equ 0x1BADB002          ; the magic number, must be here so GRUB
                                        ; knows this is a multiboot kernel

; set some GRUB settings
ALIGN_AS        equ 0x00
MEMINFO         equ 0x02                ; ask for memory info in the multiboot structure
VIDEOINFO       equ 0x04                ; ask for video in the multiboot structure

FLAGS           equ ALIGN_AS | MEMINFO | VIDEOINFO
CHECKSUM        equ -(MAGIC + FLAGS)    ; MAGIC + FLAGS + CHECKSUM = 0


        section .multiboot              ; set the multiboot header data
        align 4                         ; this is what is found by GRUB
            
        dd      MAGIC
        dd      FLAGS
        dd      CHECKSUM

        dd      0                       ; misc mutiboot settings,
        dd      0                       ; don't really care about them
        dd      0                       ; at the moment
        dd      0
        dd      0

        dd      1                       ; make this zero to get a larger frame buffer 
        dd      0                       ; desired width (0 to default)
        dd      0                       ; desired height (0 to default)
        dd      0                       ; desired color depth (0 to default)

        section .bss
        align 16                        ; stack pointer must be aligned on 16 byte values

stack_bottom:
        resb    1048576                 ; ~1MB of stack space
stack_top:

        section .text
        global _start                   ; make _start known to the linker

        extern init                     ; init is defined in C++
        extern fini                     ; fini is defined in C++
        extern kernelMain               ; kernelMain is defined in C++

_start:

    mov         esp, stack_top          ; create the stack pointer

    call        init                    ; call global constructors

    push        eax                     ; magic number, so the kernel knows this was a multiboot
    push        ebx                     ; multiboot pointers provided by the bootloader
    call        kernelMain              ; start the kernel
    call        fini                    ; call global destructors (if the kernel ever leaves)

    
loop:                                   ; put system in an infinite loop
    cli
    hlt
    jmp         loop