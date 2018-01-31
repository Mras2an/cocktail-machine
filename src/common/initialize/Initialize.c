#include "FileSystem.h"
#include "CpuDelay.h"
#include "LedRGBHandling.h"
#include "Board.h"
#include "Cocktail.h"
#include "QueueCocktail.h"
#include "MotorHandling.h"
#include "Wifi.h"

/*!************************************************************************************************
 * \fn         void Initialize_init(void)
 * \param[in]  empty
 * \return     void
 ***************************************************************************************************/
void Initialize_init(void)
{
  Fs_init();
  CpuDelay_init();
  //Wifi_saveSSIDAndPass("xxxx", "xxxxx");
  sLedRgbGpioConfig_t ledConfig;
  ledConfig.red = LED_GPIO_RED;
  ledConfig.green = LED_GPIO_GREEN;
  ledConfig.blue = LED_GPIO_BLUE;
  LedRGBHandling_Init(&ledConfig);
  BarDebug_info("LED RED\n");
  LedRGBHandling_ExecuteLedTaskFromISR(RED_LED);
  MotorHandling_init();
  MotorHandling_setInitialPosition();
}
