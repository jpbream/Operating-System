    section .text

global memcpy_sse
memcpy_sse:
        push    ebp
        mov     ebp, esp

        mov eax, [ebp + 12]
        mov ecx, [ebp + 8]

        cmp dword [ebp + 16], 0
        jle memcpy_sse_end

memcpy_sse_top:
        movdqu xmm0, [eax]
        movdqu [ecx], xmm0

        add eax, 16
        add ecx, 16

        sub dword [ebp + 16], 16
        jg memcpy_sse_top
memcpy_sse_end:
        leave
        ret


global memcpy
memcpy:
        push    ebp
        mov     ebp, esp

        mov eax, [ebp + 12]
        mov ecx, [ebp + 8]

        cmp dword [ebp + 16], 0
        jle memcpy_end

memcpy_top:
        vmovdqu ymm0, [eax]
        vmovdqu [ecx], ymm0

        add eax, 32
        add ecx, 32

        sub dword [ebp + 16], 32
        jg memcpy_top
memcpy_end:
        leave
        ret


global memsetd
memsetd:
        push    ebp
        mov     ebp, esp

        mov eax, [ebp + 8]

        cmp dword [ebp + 16], 0
        jle memsetd_end

        vbroadcastss ymm0, dword [ebp + 12]

memsetd_top:
        vmovdqu [eax], ymm0
        add eax, 32
        sub dword [ebp + 16], 8
        jg memsetd_top
memsetd_end:
        leave
        ret