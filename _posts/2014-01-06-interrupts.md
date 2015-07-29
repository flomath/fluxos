---
layout: page
title: "Interrupts"
category: doc
date: 2014-01-06 11:10:09
---

## Interrupts

The interrupt vectors must be defined in the [linker script](https://github.com/flomath/fluxos/blob/master/beagleboard.cmd). They need their own memory section:

```
/** beagleboard.cmd **/
MEMORY
{
	BOOTROM:	    o = 0x40000000	l = 0x0001BFFF	/* BOOT ROM */
	SRAM:           o = 0x40200000  l = 0x0000FFC8  /* 64kB Internal SRAM */
	EXCEPTIONS:		o = 0x4020FFC8	l = 0x00000037	/* RAM exception vectors */
	DDR0:           o = 0x80000000  l = 0x40000000	/* 1GB external DDR Bank 0 */
}

SECTIONS
{
    .intvecs    > EXCEPTIONS
    .intvecs_impl > SRAM
...
```

The interrupt handlers are than defined in the [intvecs.asm](https://github.com/flomath/fluxos/blob/master/intvecs.asm) file. Ensure that the memory addresses are the same as referenced in the technial reference manual, table 25-10.

The defined handlers can be written in C with the help of `#pragma INTERRUPT(method_name, IRQ)` to ensure that the methods are correctly linked. Those handlers need to be extracted to another memory section, because the allocated space for the RAM exception vectors is too small. Therefore the pragma `SET_CODE_SECTION` was used to set the code section of the handlers to "[intvecs_impl](https://github.com/flomath/fluxos/blob/master/src/system/hal/omap3530/interrupt/interrupt.c#L65)", which is a section located in the SRAM.  

```c
#pragma SET_CODE_SECTION(".intvecs_impl")
#pragma INTERRUPT(irq_handler, IRQ)
void irq_handler(void) {
    ...
}

#pragma INTERRUPT(dabt_handler, DABT)
interrupt void dabt_handler(void) {
    ...
}

...
```

Because we will enter the Interrupt System Mode whenever an interrupt occurs, we also need to define a [stack](https://github.com/flomath/fluxos/blob/interrupts/boot.asm) for this mode. If this is not done, we will receive Memory Abort Exceptions whenever we are trying to call a method in the ISR.

### Activate an IRQ
To activate an interrupt for a device, a bit in the `MPU_INTC_INTCPS_MIR` register has to be enabled (table 10-4 OMAP35x reference guide) and the implemented function `void interrupt_add_listener(uint32_t irq, interrupt_callback* listener)` has to be called.

### System call
The seperation of proccess and OS space usually means that the process cannot access the OS space, because of the user/system mode. That means that an interface/API is needed to access OS space, a so called system call.

A system call is defined by the interrupt vectors. If such a system call is called, it does not run in the interrupt mode as usual, but does run in the system mode. The declaration of a function in our case `syscall` is needed. This function is annotated with the pragma `SWI_ALIAS`.

```c
#pragma SWI_ALIAS(syscall, 0)
void syscall(uint32_t swiID, uint32_t params[], uint32_t paramLength);
```

The "swiID" is needed, to distinguish between different system calls.

```c
#define SYS_DEBUG	    10
#define SYS_EXIT        20
#define SYS_PRINT       30
#define SYS_READ        31
```