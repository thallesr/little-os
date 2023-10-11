ORG 0x7c00
;origin on this address
BITS 16
;16 bits mode

start:
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
.done
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
