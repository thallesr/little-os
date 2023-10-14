ORG 0
;origin on this address
BITS 16
;16 bits mode

start:
	cli ; clear interrupts
	; taking control of initializing those
	mov ax, 0x7c0
	mov ds, ax
	mov es, ax
	mov ax, 0x00
	mov ss, ax
	mov sp, 0x7c00
	sti ; enables interrupts
	mov si, message
	call print
	jmp $
print:
	mov bx, 0
.loop:
	lodsb
;load what si is pointing to al register
	cmp al, 0
	je .done
	call print_char
	jmp .loop
.done:
	ret

print_char:
	mov ah, 0eh
	int 0x10
	ret
message: db 'Hello World!', 0

times 510- ($ - $$) db 0
; filling 510 bytes of data at least
dw 0xAA55
; little endian 55AA on the last 2 bytes of the 512 boot sector
