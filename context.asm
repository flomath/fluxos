		.global __context_load
		.global __context_save
		.global __switch_mode

__context_save:
	ADD		R13, R13, #4				; Point to CPSR/SPSR
	MRS		R0, SPSR					; Get SPSR and save it to R0
	STMFD	R13!, {R0}					; Put SPSR on the stack
	SUB		R13, R13, #4				; Point to R14

	STMFD 	R13, {R0-R14}^				; Save R0-R14 (User Registers)
	SUB		R13, R13, #60				; Point to begin

	MOV		R0, R13						; Return the stack pointer
	MOV 	PC, R14						; Leave method

; Load the context
; R1-R14, CPSR (SPSR), PC
; Param: 	R0 ... context_address
__context_load:
	; Stack contains R0-14^, PC, CSPR
	LDMFD	R13, {R0-R14}^				; Load Registers R0-R14 from stack
	ADD		R13, R13, #60				; Point to PC
	LDMFD	R13!, {R14}					; Load PC to R14

	STMFD	R13, {R0}					; Copy R0 to Stack
	LDMFD	R13, {R0}					; Load SPSR from Stack in R0
	MSR		SPSR_cxsf, R0				; Load mode
	SUB		R13, R13, #4				; Go to R0
	LDMFD	R13!, {R0}					; Load R0
	ADD		R13, R13, #4				; Reset SP

	SUBS 	PC, R14, #4					; Leave method to saved LR

; Switch to Mode
; Param: 	R0 ... system mode
__switch_mode:
	MOV		R1, R0					; Save R0 to R1
	MRS		R0, CPSR				; Save mode to R0
	AND		R2,R1,#0x0				; Clear R2
	ORR		R2,R14,#0x0				; R2 = R14
	BIC		R0, R0, #0x1F			; Clear last 5 bits of R0
	ORR		R0, R0, R1				; Set mode
	MSR		CPSR_c, R0				; Save altered mode to CPSR
	AND		R14, R14, #0x0			; Clear R14
	ORR		R14, R2, #0x0			; R14 = R2
	MOV		PC, R14					; Leave method
