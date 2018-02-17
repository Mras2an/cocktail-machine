#include "Wifi.h"
#include "System.h"
#include "Initialize.h"
#include "Board.h"
#include "ButtonHandling.h"
#include "LedRGBHandling.h"
#include "Html.h"
#include "CpuDelay.h"
#include "Gpio.h"
#include "MotorHandling.h"

#ifdef SMART_CONFIG
  #include "SmartConfig.h"
#endif

/******************************************************************************
 * FunctionName : void app_main(void)
 * Parameters   : void
 * Returns      : void
 *******************************************************************************/
void app_main(void)
{
  /* gpio switch stop detection*/
  BAR_ERROR_CHECK(Gpio_configure(DETECTION_AXE_X, BAR_GPIO_MODE_INPUT, BAR_GPIO_PULL_NONE));
  /* gpio switch stop detection*/
  BAR_ERROR_CHECK(Gpio_configure(DETECTION_AXE_Y, BAR_GPIO_MODE_INPUT, BAR_GPIO_PULL_NONE));

  Initialize_init();
  BarDebug_info("AB Free Heap at %d = %d\n", __LINE__, OsGetFreeHeapSize());
  System_init();

//  BAR_ERROR_CHECK(Gpio_configure(BUTTON_GPIO_MODE, BAR_GPIO_MODE_INPUT, BAR_GPIO_PULL_NONE));
//  uint32_t bp[1];
//  Gpio_get(BUTTON_GPIO_MODE, bp);
//  if(bp[0] == 0)
//  {
//    SmartConfig_factoryReset();
//  }

  #ifdef SMART_CONFIG
  SmartConfig_init();
  #endif

  Wifi_init();
}
