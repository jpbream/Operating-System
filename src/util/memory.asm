                section .text
                global memcpy_sse
                global memcpy
                global memsetd

memcpy_sse:                                     ; void *memcpy_sse(void *dest, const void * src, size_t n);
        push    ebp
        mov     ebp, esp

        mov     eax, [ebp + 12]                 ; eax = src
        mov     ecx, [ebp + 8]                  ; ecx = dest

        cmp     dword [ebp + 16], 0             ; if the count is zero
        jle     memcpy_sse_end                  ; return from the function

memcpy_sse_top:
        movdqu  xmm0, [eax]                     ; move a xmm from the source
        movdqu  [ecx], xmm0                     ; into the destination

        add     eax, 16                         ; increment the source pointer
        add     ecx, 16                         ; increment the destination pointer

        sub     dword [ebp + 16], 16            ; subtract from the counter
        jg      memcpy_sse_top                  ; if the counter is zero, return from the function
memcpy_sse_end:
        leave
        ret


memcpy:                                         ; void *memcpy(void *dest, const void * src, size_t n);
        push    ebp
        mov     ebp, esp

        mov     eax, [ebp + 12]                     
        mov     ecx, [ebp + 8]

        cmp     dword [ebp + 16], 0
        jle     memcpy_end

memcpy_top:
        vmovdqu ymm0, [eax]                     ; move a ymm from the source
        vmovdqu [ecx], ymm0                     ; into the destination

        add     eax, 32
        add     ecx, 32

        sub     dword [ebp + 16], 32
        jg      memcpy_top
memcpy_end:
        leave
        ret


memsetd:                                        ; extern "C" void *memsetd(void *str, uint32_t d, size_t n);
        push    ebp
        mov     ebp, esp

        mov     eax, [ebp + 8]                  ; eax = str

        cmp     dword [ebp + 16], 0
        jle     memsetd_end

        vbroadcastss ymm0, dword [ebp + 12]     ; load 4 copies of the dword into a ymm

memsetd_top:
        vmovdqu [eax], ymm0                     ; load the ymm into the destination
        add     eax, 32
        sub     dword [ebp + 16], 8
        jg      memsetd_top
memsetd_end:
        leave
        ret