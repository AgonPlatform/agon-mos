		; mos_api_setresetvector example usage
		.assume adl=1
		.org $40000
		jp start
		.align $40

		.db "MOS"
		.db 0 ; version
		.db 1 ; ADL enabled (24-bit addressing)

mos_api_setresetvector:	EQU	0x61

start:
		push iy

		; Override the rst 0x10 handler with our custom one
		ld a,mos_api_setresetvector
		ld e,0x10
		ld hl,custom_rst10_handler
		rst.lil 8

		; save the old vector
		ld (self_modify_jmp + 1),hl

		pop iy
		ld hl,0
		ret

custom_rst10_handler:
		inc a
	self_modify_jmp:
		jp 0
