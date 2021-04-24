
; multiboot header, recognized by the GRUB bootloader
; and launches the kernel main function

; the magic number, must be here so GRUB
; knows this is a kernel file
MAGIC equ 0x1BADB002

; set some GRUB settings
ALIGN_AS equ 0x00
MEMINFO equ 0x02 ; ask for memory info in the multiboot structure
VIDEOINFO equ 0x04 ; ask for video in the multiboot structure

FLAGS equ ALIGN_AS | MEMINFO | VIDEOINFO
CHECKSUM equ -(MAGIC + FLAGS)

; set the multiboot header data
; this is what is found by GRUB
section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

dd 0
dd 0
dd 0
dd 0
dd 0

dd 0 ; make this zero to get a larger frame buffer 
dd 0 ; desired width (0 to default)
dd 0 ; desired height (0 to default)
dd 0 ; desired color depth (0 to default)

; set up the stack pointer before
; we can run C code
section .bss
align 16    ; stack pointer must be aligned on 16 byte values

stack_bottom:
    resb 1048576    ; ~1MB of stack space
stack_top:

section .text
global _start

extern init
extern fini
extern kernelMain

_start:

    mov esp, stack_top  ; create the stack pointer

    call init           ; call global constructors

    push eax
    push ebx            ; multiboot pointers provided by the bootloader
    call kernelMain     ; start the kernel
    call fini           ; call global destructors (if the kernel ever leaves)

    ; if the system somehow gets here, put it in an infinite loop
    loop:
    cli
    hlt
    jmp loop