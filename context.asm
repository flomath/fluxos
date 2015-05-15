		.global __context_load
		.global __context_tmp_save
		.global __context_save
		.global __start_process

; Save the context of the interrupted process into the temporary __context_current variable
; Param:	R0 ... context_current
; Requirements: Stack must contain old value of R12 and R0
__context_tmp_save:
	MOV R12, R0						; Move address of variable to R12
	LDMFD R13!, {R0}				; Reload old R0 value
	STMEA R12!, {R0-R11}			; Save Registers R0-R11 in context variable
	MOV R0, R12						; Move address of context variable to register R0
	LDMFD R13!, {R12}				; Get the original R12 value from the stack
	STMEA R0!, {R12}				; Store R12 to the context variable
									; PC will not need to be saved, because we are copying the LR to the PC
	MOV PC, R14						; Leave method

; Load the context
; Param: 	R0 ... context_address
__context_load:
	ADD R0, R0, #76					; Skip size, PID and State and Registers R0-14
	MOV R4, R0						; Copy R0
	LDMFD R0, {R0}					; Get user mode value
	BL __switch_mode				; Switch mode
	MOV R0, R4						; Restore R0

	SUB R0, R0, #60					; Point to R1
	LDMFD R0, {R1-R14}^				; Load Registers from context to registers

	SUB R0, R0, #4					; Move back to R0
	ADD R14, R0, #52				; Save saved LR Move to R14
	LDMFD R14, {R14}				; R14 = saved R14
	LDMFD R0, {R0}					; R0 = saved R0

	MOVS PC, R14					; Leave method to saved LR

; Save R13 and R14
; Param: 	R0 ... context_address
__context_save:
	ADD R0, R0, #68					; Skip size, PID and State and Registers R0-R12
	STMEA R0, {R13-R14}^			; Registers R13 and R14 are shadowed
	MOV R0, #0						; Set return value to 0
	MOV PC, R14						; Leave method

; Switch to Mode
; Param: 	R0 ... user_mode
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
