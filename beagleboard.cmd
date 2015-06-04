/****************************************************************************/
/*  beagleboard.cmd                                                         */
/****************************************************************************/

-stack           0x00002000
-heap            0x00002000

MEMORY
{
	SRAM:           o = 0x40200000  l = 0x0000FFC8  /* 64kB Internal SRAM */
	DDR0:           o = 0x80000000  l = 0x40000000	/* 1GB external DDR Bank 0 */
	EXCEPTIONS:		o = 0x4020FFC8	l = 0x00000037	/* RAM exception vectors */
}

stackSize = 0x20000;

SECTIONS
{
   .intvecs    > EXCEPTIONS
   .intvecs_impl > SRAM

   .const      > SRAM
   .bss        > SRAM
   .far        > SRAM

   .stack      > SRAM
   .data       > SRAM
   .cinit      > SRAM
   .cio        > SRAM

   .text       > SRAM
   .sysmem     > SRAM
   .switch     > SRAM

   .stackArea > DDR0 {
       . = align(4);
       . = . + stackSize;
       __stackIrq = .;
       . = . + stackSize;
       __stackSvc = .;
       . = . + stackSize;
   }
}
