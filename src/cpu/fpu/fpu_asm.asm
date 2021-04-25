
			section .text
			global DetectFPU
			global InitFPU
										
DetectFPU:								; bool DetectFPU();
	mov 		eax, cr0				; change some bits in the fpu status
	and 		al, ~6
	mov 		cr0, eax

	mov 		ax, 3333h

	fninit								; store and load the edited status
	fnstsw 		ax

	cmp 		ax, 0					; check if the status was actually changed		
	je 			found_fpu
	ret

found_fpu:
	mov 		eax, 1
	ret


InitFPU:								; void InitFPU();
	mov 		eax, cr0
	and 		eax, ~4					; bit 2 must be off to use the FPU

	and 		eax, ~32				; if this bit is on, internal cpu exceptions will be generated	
										; if off, external interrupts will be used to signal exceptions

	mov 		cr0, eax
	fninit
	fldcw 		[all_exceptions]		; write this number to enable exceptions for
	ret									; division by zero and invalid operands

			section .data
all_exceptions 		dw 37Bh				; bit 2, division by zero
										; bit 0, invalid operands and stack overflows
	