;*-------------------------------------------------------
;* initial boot routine for fluxos
;*-------------------------------------------------------
	.armfunc _c_int00
	.global	_c_int00

;*-------------------------------------------------------
;* Set main_func and args
;*-------------------------------------------------------
    .asg	_args_main,   	ARGS_MAIN_RTN
    .asg	exit,         	EXIT_RTN
    .asg    main_func_sp,	MAIN_FUNC_SP

;*-------------------------------------------------------
;* See p.37/38 on arm arch ref manual
;*-------------------------------------------------------

		.global __stackSvc

stackSvc	.long	__stackSvc

;*-----------------------	--------------------------------
;* FunctionDef for c_int00
;*-------------------------------------------------------
_c_int00: .asmfunc

	;*-------------------------------------------------------
	;* Save current stack pointer for SDP analysis
	;*-------------------------------------------------------
	;	r0, c_mf_sp
	;STR	sp, [r0]

	;*-------------------------------------------------------
	;* Set KernelStack
	;*-------------------------------------------------------
	CPS      0x13 ; Supervisor mode
	LDR      sp, stackSvc

    ;*------------------------------------------------------
    ;* Perform all the required initilizations:
    ;*   - Process BINIT Table
    ;*   - Perform C auto initialization
    ;*   - Call global constructors
    ;*------------------------------------------------------
    BL      __TI_auto_init

	;*-------------------------------------------------------
	;* Call Application
	;*-------------------------------------------------------
    BL      ARGS_MAIN_RTN

	;*-------------------------------------------------------
	;* Call exit(1) if application didn't call it before
	;*-------------------------------------------------------
    ;MOV     R0, #1
    ;BL      EXIT_RTN

.end

;******************************************************
;* UNDEFINED REFERENCES                               *
;******************************************************
	.global ARGS_MAIN_RTN
	.global MAIN_FUNC_SP
	.global	EXIT_RTN
	.global __TI_auto_init

