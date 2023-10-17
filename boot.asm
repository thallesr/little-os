ORG 0
;origin on this address
BITS 16
;16 bits mode
_start:
	jmp short start
	nop
times 33 db 0
;Bios parameter block filling, avoid issues with bios overwriting those
start:
	jmp 0x7c0:nextstep

nextstep:
	cli ; clear interrupts
	; taking control of initializing those
	mov ax, 0x7c0
	mov ds, ax
	mov es, ax
	mov ax, 0x00
	mov ss, ax
	mov sp, 0x7c00
	sti ; enables interrupts
	
	mov ah, 2 ;read sector command
	mov al, 1 ; one sector to read
	mov ch, 0; cylinder low eight bits
	mov cl, 2; read sector two
	mov dh, 0; head number 
	;dl is already set by bios for the current disk
	mov bx, buffer
	int 0x13
	jc error
	
	mov si, buffer
	call print

	
	jmp $
	
error:
	mov si, error_message
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
error_message: db 'Failed to load sector'
times 510- ($ - $$) db 0
; filling 510 bytes of data at least
dw 0xAA55
; little endian 55AA on the last 2 bytes of the 512 boot sector
buffer: ; anything here will not be loaded , out of the 512 bytes, but can be referenced
