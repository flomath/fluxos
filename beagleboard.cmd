/****************************************************************************/
/*  beagleboard.cmd                                                         */
/****************************************************************************/

MEMORY
{
	BOOTROM:	    o = 0x40000000	l = 0x0001BFFF	/* BOOT ROM */
	SRAM:           o = 0x40200000  l = 0x0000FFC8  /* 64kB Internal SRAM */
	EXCEPTIONS:		o = 0x4020FFC8	l = 0x00000037	/* RAM exception vectors */
	DDR0:           o = 0x80000000  l = 0x40000000	/* 1GB external DDR Bank 0 */
}

stackSize = 0x20000;

SECTIONS
{

   .intvecs    > EXCEPTIONS
   .intvecs_impl > SRAM

   .const      > DDR0
   .bss        > DDR0
   .far        > DDR0
   .stack      > DDR0
   .data       > DDR0
   .cio        > DDR0
   .text       > DDR0
   .sysmem     > DDR0
   .switch     > DDR0

   .cinit      > SRAM

   .stackArea > DDR0 {
       . = align(4);
       . = . + stackSize;
       __stackIrq = .;
       . = . + stackSize;
       __stackSvc = .;
       . = . + stackSize;
   }
}
