[BITS 32]
global _start ; exporting the symbol
global problem
global enable_int
extern kernel_main
CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

        in al,0x92
        ; read cpu bus
        or al,2
        ; set bits
        out 0x92, al
        ;write to cpu bus?


        ; remap master pic
        mov al, 00010001b
        out 0x20, al  ;initialization mode

        mov al, 0x20 ; interrupt 0x20 is where the master ISR will start;
        out 0x21, al

        mov al, 00000001b ;; put pic in x86 mode
        out 0x21, al
        ; end remap of master pic

        call kernel_main

        jmp $

problem:
    ;mov eax, 0
    ;div eax
    int 0
times 512-($ - $$) db 0

enable_int:
            sti; reenable interrupts
ret
