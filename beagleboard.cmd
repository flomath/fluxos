/****************************************************************************/
/*  beagleboard.cmd                                                         */
/****************************************************************************/

-stack           0x00002000
-heap            0x00002000

MEMORY
{
	SRAM:           o = 0x40200000  l = 0x0000FC00  /* 64kB Internal SRAM */
	DDR0:           o = 0x80000000  l = 0x40000000	/* 1GB external DDR Bank 0 */
	EXCEPTIONS:		o = 0x4020FF00	l = 0x000000C4	/* RAM exception vectors */
}

stackSize = 0x20000;

SECTIONS
{
   .intvecs    > EXCEPTIONS

   .const      > DDR0
   .bss        > DDR0
   .far        > DDR0

   .stack      > DDR0
   .data       > DDR0
   .cinit      > SRAM
   .cio        > DDR0

   .text       > DDR0
   .sysmem     > DDR0
   .switch     > DDR0

   .stackArea > DDR0 {
       . = align(4);
       . = . + stackSize;
       __stackIrq = .;
       . = . + stackSize;
       __stackSvc = .;
       . = . + stackSize;
   }
}
