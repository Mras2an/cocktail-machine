#ifndef _LED_RGB_HANDLING_H_
#define _LED_RGB_HANDLING_H_

#include "LedRGBGpioDriver.h"

void LedRGBHandling_Init(const sLedRgbGpioConfig_t *config);

void LedRGBHandling_enableLed(int status);

void LedRGBHandling_ExecuteLedTaskFromISR(eLedMode_t eEvent);

#endif /* _LED_RGB_HANDLING_H_ */
