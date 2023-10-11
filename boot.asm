ORG 0x7c00
BITS 16

start:
	mov ah, 0eh
	mov al, 'A'
	mov bx, 0
	int 0x10

	jmp $
; jump to the same line

times 510- ($ - $$) db 0
; filling 510 bytes of data at least
dw 0xAA55
; little endian 55AA on the last 2 bytes of the 512 boot sector
