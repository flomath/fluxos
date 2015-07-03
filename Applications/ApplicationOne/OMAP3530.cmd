/****************************************************************************/
/*  OMAP3530.cmd                                                            */
/*  Copyright (c) 2010  Texas Instruments Incorporated                      */
/*  Author: Rafael de Souza                                                 */
/*                                                                          */
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on an OMAP3530.          */
/*                 Use it as a guideline.  You will want to                 */
/*                 change the memory layout to match your specific          */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/****************************************************************************/

MEMORY
{
    CODE:           o = 0x00115000  l = 0x00010000  /* 64kB Internal CODE */
    STACK: 			o = 0x10000000	l = 0x00020000 /* 8kB Stack */
}

SECTIONS
{
    .text          >  CODE
    .stack         >  CODE
    .bss           >  CODE
    .cio           >  CODE
    .const         >  CODE
    .data          >  CODE
    .switch        >  CODE
    .sysmem        >  CODE
    .far           >  CODE
    .args          >  CODE
    .ppinfo        >  CODE
    .ppdata        >  CODE
  
    /* TI-ABI or COFF sections */
    .pinit         >  CODE
    .cinit         >  CODE
  
    /* EABI sections */
    .binit         >  CODE
    .init_array    >  CODE
    .neardata      >  CODE
    .fardata       >  CODE
    .rodata        >  CODE
    .c6xabi.exidx  >  CODE
    .c6xabi.extab  >  CODE

	.STACK			> STACK  {
		__stackUsr = .;
	}


}
