		.assume adl = 1	
		.global _on_crash
		.text
_on_crash:
		di
		push iy
		push ix
		push hl
		push de
		push bc
		push af

		ld ix,0
		add ix,sp

		; push SPL
		pea ix-18
		; push SPS
		ld hl,0
		add.s hl,sp
		push hl
		; mb
		ld hl,0
		ld a,mb
		ld l,a
		push hl
		; PC before rst 0x38
		ld hl,(ix+18)
		push hl

		ld hl,panic_msg
		push hl
		call _printf
		pop af
		pop af
		pop af
		pop af
		pop af

		ei 		; enable interrupts so user can make keypresses
	2:	xor a 		; wait for 'r'esume
		rst.lil 08h
		cp 'r'
		jr z, 3f
		cp 'R'
		jr z, 3f
		jr 2b

		; restore everything and return to userspace
	3:	pop af
		pop bc
		pop de
		pop hl
		pop ix
		pop iy
		; ret, not ret.lil, because we assume accidental entry to
		; rst38 from ADL code. so rst.lil was not used, just rst
		ret

panic_msg:
		.ascii "\x11\x81\x11\x10"
		.ascii "!! RST $38 panic. Guru meditation:   !!\r\n"
		.ascii "PC:%06x MB:%02x SPS:%04x SPL:%06x\r\n"
		.ascii "AF:%06x BC:%06x DE:%06x HL:%06x\r\n"
		.ascii "IX:%06x IY:%06x\r\n"
		.ascii "!! [r] resume, [ctrl-alt-del] reboot !!\r\n"
		.ascii "\x11\x80\x11\x0f"
		.db 0
