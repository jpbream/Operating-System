
            section .text
            global ActivateGDT
            global ActivateIDT
            global EnableInterrupts
            global DisableInterrupts
                                    
ActivateGDT:                            ; void ActivateGDT(GDT* gdt)
    mov         edx, [esp + 4]
    lgdt        [edx]
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