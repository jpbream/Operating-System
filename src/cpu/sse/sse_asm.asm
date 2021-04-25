            section .text
            global InitSSE
                                      
InitSSE:                                ; void InitSSE();
    mov         eax, cr0
    and         ax, 0FFFBh		        ; clear coprocessor emulation
    or          ax, 2			        ; set coprocessor monitoring
    mov         cr0, eax

	mov         eax, cr4
    or          eax, 600h               ; enable sse and sse exceptions
    or          eax, 1<<18              ; enable xgetbv instruction
    mov         cr4, eax

    ; will also need a way to save/restore these registers
    ; using the XSAVE interface

    mov         ecx, 0
    xgetbv                              ; Load XCR0 register
    or          eax, 7                  ; Set AVX, SSE, X87 bits
    xsetbv                              ; Save back to XCR0

	ret

            segment .data
            align 16
SavedFloats:            times 512 db 0