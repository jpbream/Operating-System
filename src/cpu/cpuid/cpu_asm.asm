
section .text

extern EFLAGS_BIT

global CheckCPUID
CheckCPUID:
	push ebp
	mov ebp, esp

	push ebx
	push ecx

	pushfd ; save eflags

	pushfd ; save a copy we will modify

	mov ebx, [EFLAGS_BIT]
	xor ecx, ecx
	bts ecx, ebx ; ecx contains the mask we need

	xor dword [esp], ecx ; toggle the bit in eflags

	popfd ; save the changed copy to eflags
	
	pushfd
	pop eax ; load the changed eflags into eax

	xor eax, [esp] ; eax holds any bits that were changed
	popfd ; restore original eflags

	and eax, ecx ; check if the cpuid bit was changed

	bt eax, ebx
	setc al

	pop ecx
	pop ebx

	mov esp, ebp
    pop ebp
    ret

global GetBasicMax
GetBasicMax:

	push ebx
	push ecx
	push edx

	mov eax, 0
	cpuid

	pop edx
	pop ecx
	pop ebx

	ret

global GetExtendedMax
GetExtendedMax:
	
	push ebx
	push ecx
	push edx

	mov eax, 80000000H
	cpuid

	pop edx
	pop ecx
	pop ebx

	ret

global GetVendorID
GetVendorID:

	push ebp
	mov ebp, esp

	push eax
	push ebx
	push edx
	push ecx

	mov eax, 0
	cpuid

	mov eax, [ebp + 8]

	; this works because the registers are
	; little endian
	mov [eax], ebx
	mov [eax + 4], edx
	mov [eax + 8], ecx

	pop ecx
	pop edx
	pop ebx
	pop eax

	mov esp, ebp
	pop ebp
	ret

global GetProcessorSignature
GetProcessorSignature:
	
	push ebx
	push edx
	push ecx

	mov eax, 1
	cpuid

	pop ecx
	pop edx
	pop ebx

	ret

global GetMiscInfo
GetMiscInfo:

	push ebx
	push edx
	push ecx

	mov eax, 1
	cpuid

	mov ebx, eax

	pop ecx
	pop edx
	pop ebx

	ret

global GetFeatures
GetFeatures:

	push ebx
	push edx
	push ecx

	mov eax, 1
	cpuid

	mov eax, edx

	pop ecx
	pop edx
	pop ebx

	ret

global GetNewFeatures
GetNewFeatures:

	push ebx
	push edx
	push ecx

	mov eax, 1
	cpuid

	mov eax, ecx

	pop ecx
	pop edx
	pop ebx

	ret

global GetExtendedFeatures
GetExtendedFeatures:
	push ebx
	mov eax, 7h
	mov ecx, 0
	cpuid

	; there are also features in ebx and edx,
	; but i am ignoring these
	mov eax, ebx
	pop ebx
	ret

global GetDescriptors
GetDescriptors:
	
	push ebp
	mov ebp, esp

	push ebx
	push edx
	push ecx

	sub esp, 4
	mov dword [esp], 0 ; counter for the number of descriptors

	mov eax, 2
	cpuid

	; I am choosing to ignore the lower 8 bits of eax
	; which describe how many times cpuid must be executed
	; it is 1 most of the time

	; a register has descriptors in it if bit
	; 31 is off
	bt eax, 31
	mov al, 0
	jc do_ebx

	; we are out of registers so we will need to use esp temporarily
	push ebx
	mov ebx, [ebp + 8]
	mov [ebx], eax
	pop ebx

	add dword [esp], 4 ; add 4 to the counter

do_ebx:

	; eax is now freed up, move the code pointer into it
	mov eax, [ebp + 8]
	add eax, [esp] ; add the counter in case eax had valid descriptors

	bt ebx, 31
	jc do_ecx

	; load descriptors from ebx
	mov [eax], ebx
	add eax, 4
	add dword [esp], 4

do_ecx:

	bt ecx, 31
	jc do_edx

	; load descriptors from ecx
	mov [eax], ecx
	add eax, 4
	add dword [esp], 4

do_edx:

	bt edx, 31
	jc done

	; load descriptors from edx
	mov [eax], edx
	add eax, 4
	add dword [esp], 4
	
done:
	mov eax, [esp] ; return the counter

	add esp, 4

	pop ecx
	pop edx
	pop ebx

	mov esp, ebp
	pop ebp
	ret

global GetMidPSN
GetMidPSN:
	
	push ebx
	push edx
	push ecx

	mov eax, 3
	cpuid

	mov ebx, edx

	pop ecx
	pop edx
	pop ebx

	ret


global GetLowPSN
GetLowPSN:

	push ebx
	push edx
	push ecx

	mov eax, 3
	cpuid

	mov ebx, ecx

	pop ecx
	pop edx
	pop ebx

	ret

global GetBrandString
GetBrandString:

	push ebp
	mov ebp, esp

	push ebx
	push edx
	push ecx

	mov eax, 80000002h
	cpuid
	mov [brandstr], eax
	mov [brandstr + 4], ebx
	mov [brandstr + 8], ecx
	mov [brandstr + 12], edx

	mov eax, 80000003h
	cpuid
	mov [brandstr + 16], eax
	mov [brandstr + 20], ebx
	mov [brandstr + 24], ecx
	mov [brandstr + 28], edx

	mov eax, 80000004h
	cpuid
	mov [brandstr + 32], eax
	mov [brandstr + 36], ebx
	mov [brandstr + 40], ecx
	mov [brandstr + 44], edx

	pop ecx
	pop edx
	pop ebx

	mov eax, brandstr

	mov esp, ebp
	pop ebp
	ret

section .data
brandstr times 48 db 0