
section .text

global DetectFPU
DetectFPU:

	; change some bits in the fpu status
	mov eax, cr0
	and al, ~6
	mov cr0, eax

	mov ax, 3333h

	; store and load the edited status
	fninit
	fnstsw ax

	; check if the status was actually changed
	cmp ax, 0
	je found_fpu
	ret

	found_fpu:
	mov eax, 1
	ret

global InitFPU
InitFPU:
	push eax

	mov eax, cr0

	; bit 2 must be off to use the FPU
	and eax, ~4

	; if this bit is on, internal cpu exceptions will be generated
	; if off, external interrupts will be used to signal exceptions
	and eax, ~32

	mov cr0, eax

	fninit

	; write this number to enable exceptions for
	; division by zero and invalid operands
	fldcw [all_exceptions]

	pop eax
	ret

section .data

; bit 2, division by zero
; bit 0, invalid operands and stack overflows
all_exceptions dw 37Bh
	