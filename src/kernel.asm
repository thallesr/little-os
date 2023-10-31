[BITS 32]
global _start ; exporting the symbol
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
        jmp $
times 512- ($ - $$) db 0