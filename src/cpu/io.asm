
; functions callable from C++ for i/o on ports

            section .text

            global outb
            global inb

            global outw
            global inw

            global outd
            global ind

            global io_wait

outb:                                   ; void outb(uint16_t port, uint8_t byte);
    push        ebp
    mov         ebp, esp

    mov         dx, [ebp + 8]           ; move port parameter into dx (skip ebp and return address)
    mov         al, [ebp + 12]          ; move byte parameter into al
    out         dx, al                  ; output the byte

    mov         esp, ebp
    pop         ebp
    ret


outw:                                   ; void outw(uint16_t port, uint16_t word)
    push        ebp
    mov         ebp, esp

    mov         dx, [ebp + 8]
    mov         ax, [ebp + 12]
    out         dx, ax

    mov         esp, ebp
    pop         ebp
    ret

outd:                                   ; void outd(uint16_t port, uint32_t dword)
    push        ebp
    mov         ebp, esp

    mov         dx, [ebp + 8]
    mov         eax, [ebp + 12]
    out         dx, eax

    mov         esp, ebp
    pop         ebp
    ret

inb:                                    ; uint8_t inb(uint16_t port);
    push        ebp
    mov         ebp, esp

    mov         dx, [ebp + 8]           ; move port parameter into dx
    in          al, dx                  ; input the byte

    mov         esp, ebp
    pop         ebp
    ret

inw:                                    ; uint16_t inw(uint16_t port);
    push        ebp
    mov         ebp, esp

    mov         dx, [ebp + 8]
    in          ax, dx

    mov         esp, ebp
    pop         ebp
    ret

ind:                                    ; uint32_t ind(uint16_t port);
    push        ebp
    mov         ebp, esp

    mov         dx, [ebp + 8]           ; move port parameter into dx
    in          eax, dx                 ; input the byte

    mov         esp, ebp
    pop         ebp
    ret

io_wait:                                ; waits for a little while
    jmp         _1
_1:
    jmp         _2
 _2:
    ret