;*-------------------------------------------------------
;* Interrupt vectors for fluxos
;*
;* Author: Philip Heimböck
;* Date: 26.03.2015
;*-------------------------------------------------------


; Define interrupt handler

		.global _c_int00
		.global udef_handler
		.global swi_handler
		.global pabt_handler
		.global dabt_handler
		.global irq_handler
		.global fiq_handler


; Define interrupt memory sections [25-10]

		.sect ".intvecs"
			B udef_handler 	; Undefined 		(0x4020FFC8)
			B swi_handler	; SWI				(0x4020FFCC)
			B pabt_handler	; Prefetch Abort 	(0x4020FFD0)
			B dabt_handler	; Data Abort 		(0x4020FFD4)
			.word 0			; Unused			(0x4020FFD8)
			B irq_handler	; Interrupt Request	(0x4020FFDC)
			B fiq_handler	; Fast IRQ			(0x4020FFE0)
