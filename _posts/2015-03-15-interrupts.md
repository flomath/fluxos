---
layout: page
title: "Interrupts"
category: doc
date: 2015-09-15 11:10:09
---

## Interrupts

The interrupt vectors must be defined in the [linker script](https://github.com/flomath/fluxos/blob/master/beagleboard.cmd). They will need their own memory section. 

```
/** beagleboard.cmd **/
MEMORY
{
  SRAM:           o = 0x40200000  l = 0x0000FFC8  /* 64kB Internal SRAM */
 	DDR0:           o = 0x80000000  l = 0x40000000	/* 1GB external DDR Bank 0 */
  EXCEPTIONS:		o = 0x4020FFC8	l = 0x00000037	/* RAM exception vectors */
}

SECTIONS
{
  .intvecs    > EXCEPTIONS
...
```

The interrupt handlers are than defined in the [intvecs.asm](https://github.com/flomath/fluxos/blob/master/intvecs.asm) file. Ensure that the memory addresses are the same as referenced in the manual, table 25-10.

The interrupt handlers can now be written in C. Use the `#pragma INTERRUPT(method_name, IRQ)` to ensure that the methods are correctly linked to the interrupts.

Because we will enter the Interrupt System Mode whenever an interrupt occurs we also need to define a [stack](https://github.com/flomath/fluxos/blob/interrupts/boot.asm) for this mode. If this is not done, we will receive Memory Abort Exceptions whenever we are trying to call a method in the ISR.

### Activate an IRQ

To activate an interrupt for a device, you will have to enable a bit in the MPU_INTC_INTCPS_MIR register. See table 10-4 in the OMAP reference and use our method `void interrupt_add_listener(uint32_t irq, interrupt_callback* listener)`.

### System call
TODO