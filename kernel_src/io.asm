
; functions callable from C++ for i/o on ports

section .text

global outb
global inb

global io_wait

; void outb(uint16_t port, uint8_t byte)
outb:

    ; standard calling convention entry
    push ebp
    mov ebp, esp

    ; this function uses eax and edx
    push eax
    push edx

    mov dx, [ebp + 8] ; move port parameter into dx (skip ebp and return address)
    mov al, [ebp + 12] ; move byte parameter into al
    out dx, al     ; output the byte

    ; restore registers
    pop edx
    pop eax

    ; standard calling convention exit
    mov esp, ebp
    pop ebp
    ret


; uint8_t inb(uint16_t port);
inb:
    push ebp
    mov ebp, esp
    push edx

    mov dx, [ebp + 8] ; move port parameter into dx
    in al, dx ; input the byte

    pop edx
    mov esp, ebp
    pop ebp
    ret

; waits for a little while
io_wait:
    jmp _1
    _1:
    jmp _2
    _2:
    ret