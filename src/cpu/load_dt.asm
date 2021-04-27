
            section .text
            global ActivateGDT
            global ActivateIDT
            global EnableInterrupts
            global DisableInterrupts
                                    
ActivateGDT:                            ; void ActivateGDT(GDT* gdt)
    mov         edx, [esp + 4]
    lgdt        [edx]
    jmp         0x10:reload_CS          ; 0x10 points at the new code selector
reload_CS:
    mov         ax, 0x18                ; 0x18 points at the new data selector
    mov         ds, ax
    mov         es, ax
    mov         fs, ax
    mov         gs, ax
    mov         ss, ax
    ret

ActivateIDT:                            ; void ActivateIDT(IDT* idt)
    mov         edx, [esp + 4]
    lidt        [edx]
    ret

EnableInterrupts:                       ; void EnableInterrupts()
    sti
    ret

DisableInterrupts:                      ; void DisableInterrupts()
    cli
    ret