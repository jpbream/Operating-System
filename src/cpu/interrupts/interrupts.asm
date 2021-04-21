section .text

extern HARDWARE_OFFSET
extern InterruptCallback

%macro AddExceptionHandler 1

    global  HandleException%1
    HandleException%1:
        push eax
        mov al, %1
        mov [interrupt_number], al
        pop eax

        mov dword [INTERRUPT_ERROR], 0
        jmp interrupt_handler

%endmacro

%macro AddExceptionHandlerWithErrorCode 1

    global  HandleException%1
    HandleException%1:
        push eax
        mov al, %1
        mov [interrupt_number], al
        pop eax

        pop dword [INTERRUPT_ERROR]
        jmp interrupt_handler

%endmacro

%macro AddInterruptHandler 1

    global  HandleInterrupt%1
    HandleInterrupt%1:
        push eax
        mov al, [HARDWARE_OFFSET]
        add al, %1
        mov [interrupt_number], al
        pop eax

        mov dword [INTERRUPT_ERROR], 0
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

    ; if the interrupt is an exception
    ; this will save the address
    ; and segment that caused it
    push eax
    mov eax, [esp + 4]
    mov dword [FAULT_ADDRESS], eax
    mov ax, [esp + 8]
    mov word [FAULT_SEGMENT], ax
    pop eax

    pusha
    push ds
    push es
    push fs
    push gs
    
    push dword [interrupt_number]
    call InterruptCallback
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds
    popa

global IgnoreInterrupt
IgnoreInterrupt:

    iret



section .data

interrupt_number dd 0

global INTERRUPT_ERROR
INTERRUPT_ERROR dd 0

global FAULT_ADDRESS
FAULT_ADDRESS dd 0

global FAULT_SEGMENT
FAULT_SEGMENT dw 0
