
#include "clock.h"
#include "../gpio/gpio.h"

void clock_enable_gpio(uint32_t port)
{
  // see p.3366
  switch(port)
  {
    // GPIO1
    case GPIO1:
    {
      hal_bitmask_set(WKUP_CM, CM_FCLKEN_WKUP, BV(EN_GPIO1));
      hal_bitmask_set(WKUP_CM, CM_ICLKEN_WKUP, BV(EN_GPIO1));
    }
    break;

    // GPIO2
    case GPIO2:
    {
      hal_bitmask_set(PER_CM, CM_FCLKEN_PER, BV(EN_GPIO2));
      hal_bitmask_set(PER_CM, CM_ICLKEN_PER, BV(EN_GPIO2));
    }
    break;

    // GPIO3
	case GPIO3:
	{
	  hal_bitmask_set(PER_CM, CM_FCLKEN_PER, BV(EN_GPIO3));
	  hal_bitmask_set(PER_CM, CM_ICLKEN_PER, BV(EN_GPIO3));
	}
	break;

	// GPIO4
	case GPIO4:
	{
	  hal_bitmask_set(PER_CM, CM_FCLKEN_PER, BV(EN_GPIO4));
	  hal_bitmask_set(PER_CM, CM_ICLKEN_PER, BV(EN_GPIO4));
	}
	break;

	// GPIO5
	case GPIO5:
	{
	  hal_bitmask_set(PER_CM, CM_FCLKEN_PER, BV(EN_GPIO5));
	  hal_bitmask_set(PER_CM, CM_ICLKEN_PER, BV(EN_GPIO5));
	}
	break;

	// GPIO6
	case GPIO6:
	{
	  hal_bitmask_set(PER_CM, CM_FCLKEN_PER, BV(EN_GPIO6));
	  hal_bitmask_set(PER_CM, CM_ICLKEN_PER, BV(EN_GPIO6));
	}
	break;
  }
}
