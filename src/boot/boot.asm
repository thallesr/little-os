ORG 0x7c00
;origin on this address

;The ORG instruction is used to provide a "hint" to the assembler and the linker for the resulting object file.
; It allows you to specify the base address of the section of the file.
;When creating "functions" (things that you might CALL), if those are sufficiently distant from where you are calling from,
; the assembler will need to use an absolute address for the 
;call rather than an offset from the current instruction. The ORG instruction effectively locks the code in place,
;making it non-position independent code.
;In practice, unless you are writing long code blocks, the majority of the JMP and CALL instructions will be SHORT or NEAR,
;allowing for simple offsets to be used

BITS 16
;16 bits mode

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

jmp short start
nop

;FAT16 header
OEMIdentifier     db 'LITTLEOS'
BytesPerSector    dw 0x200 ; we cannot change the way disk works
SectorsPerCluster db 0x80
ReservedSectors   dw 200
FATCopies		  db 0x02
RootDirEntries    dw 0x40
NumSectors		  dw 0x00
MediaType         db 0xF8
SectorsPerFat     dw 0x100
SectorsPerTrack   dw 0x20
NumberOfHeads     dw 0x40
HiddenSectors     dd 0x00
SectorsBig        dd 0x773594

;extended BPB - Dos4.0
DriveNumber 	  db 0x80
WinNTBit		  db 0x00
Signature         dd 0x29
VolumeID		  dd 0xD105
VolumeIDString    db 'LITTLEOS BO'
SystemIDString    db 'FAT16   '


start:
    jmp 0:step2

step2:
	cli ; clear interrupts
	; taking control of initializing those
	mov ax, 0x00
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7c00
	sti ; enables interrupts
	
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
	mov eax, 1   ; start from sector 1, 0 is bootloader
	mov ecx, 100 ; read 100 sec
	mov edi, 0x0100000 ; put on this address
	call ata_lba_read
	jmp CODE_SEG:0x0100000

ata_lba_read:
	mov ebx, eax ; Backup the LBA
	; send the highest 8 bits of the lba to hard disk controller
	shr eax, 24
	or eax, 0xE0 ; select the master drive
	mov dx, 0x1F6
	out dx, al
	; Finished sending the highest 9 bits of the lba

	mov eax, ecx
	mov dx, 0x1F2
	out dx, al
	; Finished sending the total sectors to read	

	; send more bits of the lba
	mov eax, ebx
	mov dx, 0x1F3
	out dx, al
	; finished sending more bits of the LBA

	; sending more bits of the lba
	mov dx, 0x1F4
	mov eax, ebx ; Restore the backup of the LBA
	shr eax, 8
	out dx, al
	; finished sending more bits of the lba

	;send upper 16 bits of the lba
	mov dx, 0x1F5
	mov eax, ebx ; restore backup LBA
	shr eax, 16
	out dx, al
	; Finished sending upper 16bits of the LBA

	mov dx, 0x1F7
	mov al, 0x20
	out dx, al

	;REad all sectors into memory
.next_sector:
	push ecx

.try_again:
	mov dx, 0x1F7
	in al, dx
	test al, 8
	jz .try_again

; We need to read 256 words at a time
	mov ecx, 256
	mov dx, 0x1F0
	rep insw ; read input work from io port specified in DX to memory location specified in ES:(E)DI
	pop ecx
	loop .next_sector
;end of reading sectors into the memory
	ret
	
	
times 510- ($ - $$) db 0
; filling 510 bytes of data at least
dw 0xAA55
; little endian 55AA on the last 2 bytes of the 512 boot sector
buffer: ; anything here will not be loaded , out of the 512 bytes, but can be referenced
