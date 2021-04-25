
		section .text							
		global CheckCPUID
		global GetBasicMax
		global GetExtendedMax
		global GetVendorID
		global GetProcessorSignature
		global GetMiscInfo
		global GetFeatures
		global GetNewFeatures
		global GetExtendedFeatures
		global GetDescriptors
		global GetMidPSN
		global GetLowPSN
		global GetBrandString

CheckCPUID: 							; bool CheckCPUID();			
	push 		ebp
	mov 		ebp, esp
	push 		ebx

	pushfd 								; save eflags
	pushfd 								; save a copy we will modify
	mov 		ebx, 21					; we need to check the 21st bit of eflags
	xor 		ecx, ecx				; clear ecx
	bts 		ecx, ebx 				; now, ecx contains the mask we need
	xor 		dword [esp], ecx 		; toggle the 21st bit in the copy of eflags
	popfd 								; save the changed copy to eflags

	pushfd								; get a copy of eflags again
	pop 		eax 					; load the changed eflags into eax
	xor 		eax, [esp] 				; eax holds any bits that were changed
	popfd 								; restore original eflags

	and 		eax, ecx 				; check if the cpuid bit was changed
	bt 			eax, ebx				; if the 21st bit was changed
	setc 		al						; then the cpuid instruction is supported

	pop 		ebx
	mov 		esp, ebp
    pop 		ebp
    ret


GetBasicMax:							; unsigned int GetBasicMax();
	push 		ebx
	mov 		eax, 0
	cpuid
	pop 		ebx
	ret


GetExtendedMax:							; unsigned int GetExtendedMax();
	push 		ebx
	mov 		eax, 80000000H
	cpuid
	pop 		ebx
	ret

GetVendorID:							; void GetVendorID(char* id);
	push 		ebp
	mov 		ebp, esp
	push 		ebx

	mov 		eax, 0
	cpuid

	mov 		eax, [ebp + 8]			; get the pointer to where to store the vendor id

	mov 		[eax], ebx				; the vendor id is split between ebx, edx, and ecx
	mov 		[eax + 4], edx
	mov 		[eax + 8], ecx

	pop 		ebx
	mov 		esp, ebp
	pop 		ebp
	ret

GetProcessorSignature:					; uint32_t GetProcessorSignature();
	push 		ebx
	mov 		eax, 1
	cpuid
	pop 		ebx
	ret

GetMiscInfo:							; uint32_t GetMiscInfo();
	push 		ebx
	mov 		eax, 1
	cpuid
	mov 		eax, ebx
	pop 		ebx
	ret


GetFeatures:							; uint32_t GetFeatures();
	push 		ebx
	mov 		eax, 1
	cpuid
	mov 		eax, edx
	pop 		ebx
	ret


GetNewFeatures:							; uint32_t GetNewFeatures();
	push 		ebx
	mov 		eax, 1
	cpuid
	mov 		eax, ecx
	pop 		ebx
	ret


GetExtendedFeatures:					; uint32_t GetExtendedFeatures();
	push 		ebx
	mov 		eax, 7h
	mov 		ecx, 0
	cpuid
	mov 		eax, ebx				; there are also features in ebx and edx,
	pop 		ebx						; but i am ignoring these
	ret


GetDescriptors:							; int GetDescriptors(uint8_t* codes);
	push 		ebp
	mov 		ebp, esp
	push 		ebx
	push 		edi

	sub 		esp, 4
	mov 		dword [esp], 0 			; counter for the number of descriptors

	mov 		eax, 2
	cpuid

	; I am choosing to ignore the lower 8 bits of eax
	; which describe how many times cpuid must be executed
	; it is 1 most of the time

	bt 			eax, 31					; a register has descriptors in it if bit
	mov 		al, 0					; 31 is off
	jc 			do_ebx

	mov 		edi, [ebp + 8]
	mov 		[edi], eax				; add this descriptor
	add 		dword [esp], 4 			; add 4 to the counter

do_ebx:
	mov 		eax, [ebp + 8] 			; eax is now freed up, move the code pointer into it
	add 		eax, [esp] 				; add the counter in case eax had valid descriptors

	bt 			ebx, 31
	jc 			do_ecx

	mov 		[eax], ebx				; load descriptors from ebx
	add 		eax, 4
	add 		dword [esp], 4

do_ecx:
	bt 			ecx, 31
	jc 			do_edx

	mov 		[eax], ecx 				; load descriptors from ecx
	add 		eax, 4
	add 		dword [esp], 4

do_edx:
	bt 			edx, 31
	jc 			done

	mov 		[eax], edx				; load descriptors from edx
	add 		eax, 4
	add 		dword [esp], 4
	
done:
	mov 		eax, [esp] 				; return the counter

	add 		esp, 4
	pop 		edi
	pop 		ebx
	mov 		esp, ebp
	pop 		ebp
	ret

GetMidPSN:								; uint32_t GetMidPSN();
	push 		ebx
	mov 		eax, 3
	cpuid
	mov 		eax, edx
	pop 		ebx
	ret

GetLowPSN:								; uint32_t GetLowPSN();
	push 		ebx
	mov 		eax, 3
	cpuid
	mov 		eax, ecx
	pop 		ebx
	ret

GetBrandString:							; char* GetBrandString();
	push 		ebp
	mov 		ebp, esp
	push 		ebx

	mov 		eax, 80000002h
	cpuid
	mov 		[brandstr], eax
	mov 		[brandstr + 4], ebx
	mov 		[brandstr + 8], ecx
	mov 		[brandstr + 12], edx

	mov 		eax, 80000003h
	cpuid
	mov 		[brandstr + 16], eax
	mov 		[brandstr + 20], ebx
	mov 		[brandstr + 24], ecx
	mov 		[brandstr + 28], edx

	mov 		eax, 80000004h
	cpuid
	mov 		[brandstr + 32], eax
	mov 		[brandstr + 36], ebx
	mov 		[brandstr + 40], ecx
	mov 		[brandstr + 44], edx

	mov 		eax, brandstr

	pop 		ebx
	mov 		esp, ebp
	pop 		ebp
	ret

			section .data
brandstr 		times 48 db 0