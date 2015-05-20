		.global __context_load
		.global __context_tmp_save
		.global __context_save
		.global __switch_mode

; Save the context of the interrupted process into the temporary __context_current variable
; Param:	R0 ... context_current
; Requirements: Stack must contain old value of R12 and R0
__context_tmp_save:
	MOV 	R12, R0						; Move address of variable to R12
	LDMFD 	R13!, {R0}				; Reload old R0 value
	STMEA 	R12!, {R0-R11}			; Save Registers R0-R11 in context variable
	MOV 	R0, R12						; Move address of context variable to register R0
	LDMFD 	R13!, {R12}				; Get the original R12 value from the stack
	STMEA 	R0!, {R12}				; Store R12 to the context variable
									; PC will not need to be saved, because we are copying the LR to the PC
	MOV 	PC, R14						; Leave method

; Load the context
; R1-R14, CPSR (SPSR), PC
; Param: 	R0 ... context_address
__context_load:
	ADD 	R0, R0, #76					; Skip size, PID and State and Registers R0-14
	MOV		R4, R0
	LDMFD 	R4, {R4}					; Get mode value
	MSR		SPSR_c, R4					; Save altered mode to SPSR_irq

	SUB 	R0, R0, #60					; Point to R1
	LDMFD 	R0, {R1-R14}^				; Load Registers from context to registers

	ADD 	R14, R0, #52				; Save saved LR Move to R14
	SUB 	R0, R0, #4					; Move back to R0
	LDMFD 	R0, {R0}					; R0 = saved R0
	LDMFD 	R14, {R14}					; R14 = saved R14

	SUBS 	PC, R14, #4					; Leave method to saved LR

; Save R13, R14 and CSPR (SPSR)
; Param: 	R0 ... context_address
__context_save:
	ADD 	R0, R0, #68					; Skip size, PID and State and Registers R0-R12
	MRS		R1, SPSR					; Save mode to R1
	STMEA 	R0, {R13-R14}^				; Save shadowed registers R13 and R14
	ADD 	R0, R0, #12					; Skip Register R13, R14 and PC (Point to CPSR)
	STMEA 	R0, {R1}					; Save mode

	MOV 	R0, #0						; Set return value to 0
	MOV 	PC, R14						; Leave method

; Switch to Mode
; Param: 	R0 ... system mode
__switch_mode:
	MOV		R1, R0					; Save mode to R1
	MRS		R0, CPSR				; Save mode to R0
	AND		R2,R1,#0x0				; Clear R2
	ORR		R2,R14,#0x0				; R2 = R14
	BIC		R0, R0, #0x1F			; Clear last 5 bits of R0
	ORR		R0, R0, R1				; Set mode
	MSR		CPSR_c, R0				; Save altered mode to CPSR
	AND		R14, R14, #0x0			; Clear R14
	ORR		R14, R2, #0x0			; R14 = R2
	MOV		PC, R14					; Leave method
