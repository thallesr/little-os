ORG 0x7c00
;origin on this address
BITS 16
;16 bits mode

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
_start:
	jmp 0:start
	nop
times 33 db 0
;Bios parameter block filling, avoid issues with bios overwriting those
start:
	jmp 0x7c0:nextstep

nextstep:
	cli ; clear interrupts
	; taking control of initializing those
	mov ax, 0x00
	mov ds, ax
	mov es, ax
	mov ax, 0x00
	mov ss, ax
	mov sp, 0x7c00
	sti ; enables interrupts
	
	jmp $
.load_protected:
	cli
	lgdt[gdt_descriptor]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	jmp CODE_SEG:load32
	
gdt_start:
gdt_null:
	dd 0x0
	dd 0x0
;offset 0x8
gdt_code:		; CS should point to this 
	dw 0xffff    	; Segment limit first 0-15 bits
	dw 0		; base first 0-15 bit
	db 0		; base 16-24 bits
	db 0x9a		;
	db 11001111b	; hight 4 bit flags and the low 4 bit flags
	db 0		; base 24-31 bits
;offset 0x10
gdt_data: 		; DS, SS , ES, FS, GS
	dw 0xffff       ; Segment limit first 0-15 bits
        dw 0            ; base first 0-15 bit
        db 0            ; base 16-24 bits
        db 0x92         ;
        db 11001111b    ; hight 4 bit flags and the low 4 bit flags
        db 0            ; base 24-31 bits
	
gdt_end:
gdt_descriptor:
	dw gdt_end - gdt_start -1;
	dd gdt_start

[BITS 32]
load32:
	jmp $
times 510- ($ - $$) db 0
; filling 510 bytes of data at least
dw 0xAA55
; little endian 55AA on the last 2 bytes of the 512 boot sector
buffer: ; anything here will not be loaded , out of the 512 bytes, but can be referenced
