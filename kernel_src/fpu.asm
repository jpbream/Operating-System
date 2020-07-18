
section .text

global InitFPU

InitFPU:

	mov eax, cr0
	and al, ~6
	mov cr0, eax
	fninit
	fnstsw [status]
	cmp word [status], 0
	jne no_fpu
	fnstcw [status]
	mov ax, [status]
	and ax, 0x103F
	cmp ax, 0x003F
	jne no_fpu

	mov eax, cr0
	and eax, ~2
	or eax, 1
	or eax, 16
	mov cr0, eax
	fninit

	mov eax, 1
	ret

no_fpu:
	mov eax, 0
	ret


status dw 0x3333