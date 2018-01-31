#ifndef _LED_RGB_GPIO_DRIVER_H_
#define _LED_RGB_GPIO_DRIVER_H_

#include "Gpio.h"

typedef enum eLedMode_t
{
  IDEL_LED = 0x00,
  GREEN_LED = 0x01,
  RED_LED = 0x02,
  ORANGE_LED = 0x03,
  BLUE_LED = 0x04,
  GREEN_LED_SLOW_BLINKING = 0x05,
  ORANGE_LED_SLOW_BLINKING = 0x06,
  RED_LED_SLOW_BLINKING = 0x07,
  BLUE_LED_SLOW_BLINKING = 0x08,
  GREEN_LED_FAST_BLINKING = 0x09,
  ORANGE_LED_FAST_BLINKING = 0x0a,
  RED_LED_FAST_BLINKING = 0x0b,
  BLUE_LED_FAST_BLINKING = 0x0c,
  MULTI_2_LED_FAST_BLINKING = 0xd,
  MULTI_3_LED_FAST_BLINKING = 0xe,
  DISABLE_LED = 0xff,
} eLedMode_t;


typedef enum eLedColor_t
{
  LED_NOT_DEFINED = 0b000,
  LED_GREEN       = 0b001,
  LED_RED         = 0b010,
  LED_BLUE        = 0b100,
  LED_ORANGE      = (LED_GREEN | LED_RED),
  LED_VIOLET      = (LED_BLUE  | LED_RED),
  LED_BLUE_GREEN  = (LED_BLUE  | LED_GREEN)
} eLedColor_t;

typedef struct sLedRgbConfig
{
  eGpioNum_t red;
  eGpioNum_t green;
  eGpioNum_t blue;
} sLedRgbGpioConfig_t;


eBarError_t LedRGBGpioDriver_Init(const sLedRgbGpioConfig_t *config);

eLedColor_t LedRGBGpioDriver_SetColor(const eLedColor_t color);

eLedColor_t LedRGBGpioDriver_ToggleColor(const eLedColor_t color);

eLedColor_t LedRGBGpioDriver_GetColor(void);

#endif /* _LED_RGB_GPIO_DRIVER_H_ */
