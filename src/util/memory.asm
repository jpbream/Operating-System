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
        vmovdqu xmm0, [eax]
        vmovdqu [ecx], xmm0

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

        ; copied from compiler explorer output
        ; this unfortunate code simply loads the
        ; value to copy into xmm0
        movd      xmm1, dword [ebp + 12]
        movd      xmm2, dword [ebp + 12]
        punpckldq xmm1, xmm2
        movd      xmm0, dword [ebp + 12]
        movd      xmm3, dword [ebp + 12]
        punpckldq       xmm0, xmm3
        punpcklqdq      xmm0, xmm1
memsetd_top:
        movdqu [eax], xmm0
        add eax, 16
        sub dword [ebp + 16], 16
        jg memsetd_top
memsetd_end:
        leave
        ret