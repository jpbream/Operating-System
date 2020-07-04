
; multiboot header, recognized by the GRUB bootloader
; and launches the kernel main function

; the magic number, must be here so GRUB
; knows this is a kernel file
MAGIC equ 0x1BADB002

; set some GRUB settings
ALIGN_AS equ 1<<0
MEMINFO equ 1<<1
FLAGS equ ALIGN_AS | MEMINFO
CHECKSUM equ -(MAGIC + FLAGS)

; set the multiboot header data
; this is what is found by GRUB
section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

; set up the stack pointer before
; we can run C code
section .bss
align 16    ; stack pointer must be aligned on 16 byte values

stack_bottom:
    resb 16384    ; ~16kB of stack space
stack_top:

section .text
global start
extern kernelMain

start:

    mov esp, stack_top  ; create the stack pointer

    call kernelMain     ; start the kernel

    ; if the system somehow gets here, put it in an infinite loop
    loop:
    cli
    hlt
    jmp loop