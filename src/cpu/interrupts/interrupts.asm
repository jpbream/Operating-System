section .text

HARDWARE_OFFSET equ 0x20
extern InterruptCallback

%macro AddExceptionHandler 1

    global  HandleException%1
    HandleException%1:
        mov dword [interrupt_number], %1
        push dword [ZERO] ; push a dummy error code onto the stack
        jmp interrupt_handler

%endmacro

%macro AddExceptionHandlerWithErrorCode 1

    global  HandleException%1
    HandleException%1:
        mov dword [interrupt_number], %1

        jmp interrupt_handler

%endmacro

%macro AddInterruptHandler 1

    global  HandleInterrupt%1
    HandleInterrupt%1:
        mov dword [interrupt_number], %1 + HARDWARE_OFFSET
        push dword [ZERO] ; push a dummy error code onto the stack
        jmp interrupt_handler

%endmacro

; exceptions 8, and 10-14 post error messages
; on the stack

AddExceptionHandler 0x00
AddExceptionHandler 0x01
AddExceptionHandler 0x02
AddExceptionHandler 0x03
AddExceptionHandler 0x04
AddExceptionHandler 0x05
AddExceptionHandler 0x06
AddExceptionHandler 0x07
AddExceptionHandlerWithErrorCode 0x08
AddExceptionHandler 0x09
AddExceptionHandlerWithErrorCode 0x0A
AddExceptionHandlerWithErrorCode 0x0B
AddExceptionHandlerWithErrorCode 0x0C
AddExceptionHandlerWithErrorCode 0x0D
AddExceptionHandlerWithErrorCode 0x0E
AddExceptionHandler 0x0F
AddExceptionHandler 0x10
AddExceptionHandler 0x11
AddExceptionHandler 0x12
AddExceptionHandler 0x13
AddExceptionHandler 0x14
AddExceptionHandler 0x15
AddExceptionHandler 0x16
AddExceptionHandler 0x17
AddExceptionHandler 0x18
AddExceptionHandler 0x19
AddExceptionHandler 0x1A
AddExceptionHandler 0x1B
AddExceptionHandler 0x1C
AddExceptionHandler 0x1D
AddExceptionHandler 0x1E
AddExceptionHandler 0x1F

AddInterruptHandler 0x00
AddInterruptHandler 0x01
AddInterruptHandler 0x02
AddInterruptHandler 0x03
AddInterruptHandler 0x04
AddInterruptHandler 0x05
AddInterruptHandler 0x06
AddInterruptHandler 0x07
AddInterruptHandler 0x08
AddInterruptHandler 0x09
AddInterruptHandler 0x0A
AddInterruptHandler 0x0B
AddInterruptHandler 0x0C
AddInterruptHandler 0x0D
AddInterruptHandler 0x0E
AddInterruptHandler 0x0F

interrupt_handler:

    ; push registers in the order the CPUState struct expects them
    ; (reverse)
    push ebp
    push edi
    push esi 

    push edx 
    push ecx 
    push ebx 
    push eax
    
    push esp ; esp acts as a pointer to all the registers we just pushed
    push dword [interrupt_number]
    call InterruptCallback
    mov esp, eax ; switch the stack to a new task

    ; restore registers, but this time for the new task
    pop eax 
    pop ebx 
    pop ecx 
    pop edx 

    pop esi 
    pop edi 
    pop ebp

    add esp, 4 ; skip over the error code

global IgnoreInterrupt
IgnoreInterrupt:

    iret


section .data

interrupt_number dd 0

ZERO dd 0
