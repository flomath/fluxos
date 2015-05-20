		.global __mmu_enable_write_buffer
		.global __mmu_set_domain
		.global __mmu_set_ttbcr
		.global __mmu_set_ttbr0
		.global __mmu_set_ttbr1
		.global __mmu_enable
		.global __mmu_disable
		.global __mmu_load_dabt

;* REGISTERS Table G4-42 Secure registers

;*-------------------------------------------------------
;* Enable Write Buffer MMU
;*-------------------------------------------------------
__mmu_enable_write_buffer:
		STMFD R13!, {R0, R1} ; backup r0, r1 on stack

		MRC p15, #0, r0, c1, c0, #0	; Read SCTLR - G6.2.117
	    ORR r0, r0, #(0x1 << 3)		; Table 4-52 - Bit 3 RES1 on
	    MCR p15, #0, r0, c1, c0, #0	; Write SCTLR

		LDMFD R13!, {R0, R1} ; restore r0,r1 and jump back
		MOV PC, R14


;*-------------------------------------------------------
;* SET Domain DACR - G6.2.155
;*-------------------------------------------------------
__mmu_set_domain:
		STMFD R13!, {R0, R1, R14} ; backup r0, r1, r14 on stack

		MCR p15, #0, r0, c3, c0, #0

		LDMFD R13!, {R0, R1, PC} ; restore r0, r1 and jump back
		;MOV PC, R14

;*-------------------------------------------------------
;* SET TTBCR bits
;*-------------------------------------------------------
__mmu_set_ttbcr:
		MCR p15, #0, r0, c2, c0, #2 ; Write TTBCR

		MOV PC, R14

;*-------------------------------------------------------
;* SET TTBR0 - G6.2.156
;*-------------------------------------------------------
__mmu_set_ttbr0:
		MCR p15, #0, R0, c2, c0, #0	; set LTable of process
		MCR p15, #0, R1, c13, c0, #1; set process id and ASID

		MOV PC, R14

;*-------------------------------------------------------
;* SET TTBR1 - G6.2.157
;*-------------------------------------------------------
__mmu_set_ttbr1:
		MCR p15, #0, R0, c2, c0, #1 ; set LTable of OS

		MOV PC, R14

;*-------------------------------------------------------
;* Enable MMU
;*-------------------------------------------------------
__mmu_enable:
		STMFD R13!, {R0, R1, R14} ; backup r0, r1 on stack

		MRC p15, #0, r1, c1, c0, #0	; Read System Control Register (SCTLR)
	    ORR r1, r1, #0x1			; Table 4-52 - Bit 0 MMU on
	    DSB
	    MCR p15, #0, r1, c1, c0, #0	; Write System Control Register (SCTLR)
		ISB
		LDMFD R13!, {R0, R1, PC} ; restore r0,r1 and jump back
		; MOV PC, R14

;*-------------------------------------------------------
;* Disable MMU
;*-------------------------------------------------------
__mmu_disable:
		STMFD R13!, {R0, R1} ; backup r0, r1 on stack

		MRC p15, #0, r0, c1, c0, #0	; Read System Control Register (SCTLR)
    	BIC r0, r0, #0x1			; Table 4-52 - Bit 0 MMU off
    	MCR p15, #0, r0, c1, c0, #0	; Write System Control Register (SCTLR)

		LDMFD R13!, {R0, R1} ; restore r0,r1 and jump back
		MOV PC, R14

;*-------------------------------------------------------
;* Load DFAR (B4.1.51) and DFSR (B4.1.52) for dabt
;*-------------------------------------------------------
__mmu_load_dabt:
		MRC	p15, #0, R0, c6, c0, #0 ; Read DFAR
		MRC p15, #0, R1, c5, c0, #0 ; Read DFSR

		MOV PC, R14
