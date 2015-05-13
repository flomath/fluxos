		.global __context_save
		.global __context_load
		.global __context_save_tmp

		IMPORT __context_current


; Save the context of the interrupted process into the temporary __context_current variable
__context_save_tmp:
	STMFD R13!, {R12}				; Temporarly save R12 in Stack
	LDR R12, __context_current		; Load the address of the context variable in R12
	STMEA R12!, {R0-R11}			; Save Registers R0-R11 in context variable
	MOV R5, R12						; Move address of context variable to register R5
	LDMFD R13!, {R12}				; Get the original R12 value from the stack
	STMEA R5!, {R12}				; Store R12 to the context variable
									; Registers R13 and R14 are shadowed
									; PC will not need to be saved, because we are copying the LR to the PC


; Save the context
__context_save:
	; Backup all registers

; Load the context
__context_load:
	;
