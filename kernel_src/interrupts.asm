global irq0
extern irq0_handler

global ignore
extern irq_ignore

irq0:
    pusha
    call irq0_handler
    popa
    iret

ignore:
    pusha
    call irq_ignore
    popa
    iret
