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
handle_zero:
	mov ah, 0eh
	mov al, 'A'
	mov bx, 0x00
	int 0x10
	iret
handle_one:
	mov ah, 0eh
	mov al, 'B'
	mov bx, 0x00
	int 0x10
	iret

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

	;force using ss as segment
	;default is using ax
	mov word[ss:0x00], handle_zero
	mov word[ss:0x02], 0x7c0

	mov word[ss:0x04], handle_one
	mov word[ss:0x06], 0x7c0
	int 0; define interupt 0 , called also when we divide by 0
	mov ax,0
	div ax
	int 1
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
