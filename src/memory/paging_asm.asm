            section .text
            global EnablePaging

EnablePaging:                           ; void EnablePaging(uint32_t* dirAddress)
    mov     eax, [esp + 4]
    mov     cr3, eax                    ; load the page directory address

    mov     eax, cr0
    or      eax, 0x80000001             ; enable paging and protection flags
    mov     cr0, eax
    ret   