
section .text

global ActivateGDT
global ActivateIDT
global EnableInterrupts
global DisableInterrupts

; void ActivateGDT(GDT* gdt)
ActivateGDT:
    push edx
    mov edx, [esp + 8]
    lgdt [edx]
    pop edx
    ret

; void ActivateIDT(IDT* idt)
ActivateIDT:
    push edx
    mov edx, [esp + 8]
    lidt [edx]
    pop edx
    ret

; void EnableInterrupts()
EnableInterrupts:
    sti
    ret

; void DisableInterrupts()
DisableInterrupts:
    cli
    ret