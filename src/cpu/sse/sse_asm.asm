section .text

; CR4.OSXMMEXCPT[bit 10]=1
; CR4.OSFXSR[bit 9] = 1

global InitSSE
InitSSE:
	push eax
    push ecx

    mov eax, cr0
    and ax, 0FFFBh		; clear coprocessor emulation
    or ax, 2			; set coprocessor monitoring
    mov cr0, eax


	mov eax, cr4

    or eax, 600h ; enable sse and sse exceptions
    or eax, 1<<18 ; enable xgetbv instruction

    ; will also need to provide support for something called XSAVE
    ; but that can come later

    mov cr4, eax

    ;fxsave [SavedFloats]

    mov ecx, 0
    xgetbv ;Load XCR0 register
    or eax, 7 ;Set AVX, SSE, X87 bits
    xsetbv ;Save back to XCR0

    pop ecx
	pop eax
	ret

segment .data
align 16
SavedFloats: TIMES 512 db 0