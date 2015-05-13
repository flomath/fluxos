
#include "clock.h"

void clock_enable_gpio(uint32_t port)
{
  // see p.3366
  switch(port)
  {
    // GPIO1
    case GPIO1:
    {
      hal_bitmask_set(WKUP_CM, CM_FCLKEN_WKUP, BV(1));
      hal_bitmask_set(WKUP_CM, CM_ICLKEN_WKUP, BV(1));
    }
    break;

    // GPIO2 to 6
    case GPIO2:
    case GPIO3:
    case GPIO4:
    case GPIO5:
    case GPIO6:
    {
      hal_bitmask_set(PER_CM, CM_FCLKEN_PER, BV(1));
      hal_bitmask_set(PER_CM, CM_ICLKEN_PER, BV(1));
    }
    break;
  }
}
