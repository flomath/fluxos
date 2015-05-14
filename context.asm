		.global __context_load
		.global __context_tmp_save


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
	STMEA R0, {R13-R14}^			; Registers R13 and R14 are shadowed
									; PC will not need to be saved, because we are copying the LR to the PC

; Load the context
; Param: 	R0 ... context_address
__context_load:
	ADD R0, R0, #12					; Skip size, PID and State
									; Load Registers from context to stack
	LDMFD R13, {R0-R14}^			; Load Registers from stack to registers
