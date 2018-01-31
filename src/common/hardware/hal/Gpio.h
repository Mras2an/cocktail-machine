#ifndef _GPIO_H_
#define _GPIO_H_

#include "Bsp.h"

typedef enum eGpioNum
{
  BAR_GPIO_NUM_0=0,
  BAR_GPIO_NUM_1,
  BAR_GPIO_NUM_2,
  BAR_GPIO_NUM_3,
  BAR_GPIO_NUM_4,
  BAR_GPIO_NUM_5,
  BAR_GPIO_NUM_6,
  BAR_GPIO_NUM_7,
  BAR_GPIO_NUM_8,
  BAR_GPIO_NUM_9,
  BAR_GPIO_NUM_10,
  BAR_GPIO_NUM_11,
  BAR_GPIO_NUM_12,
  BAR_GPIO_NUM_13,
  BAR_GPIO_NUM_14,
  BAR_GPIO_NUM_15,
  BAR_GPIO_NUM_16,
  BAR_GPIO_NUM_17,
  BAR_GPIO_NUM_18,
  BAR_GPIO_NUM_19,
  BAR_GPIO_NUM_20,
  BAR_GPIO_NUM_21,
  BAR_GPIO_NUM_22,
  BAR_GPIO_NUM_23,
  BAR_GPIO_NUM_24,
  BAR_GPIO_NUM_25,
  BAR_GPIO_NUM_26,
  BAR_GPIO_NUM_27,
  BAR_GPIO_NUM_28,
  BAR_GPIO_NUM_29,
  BAR_GPIO_NUM_30,
  BAR_GPIO_NUM_31,
  BAR_GPIO_NUM_32,
  BAR_GPIO_NUM_33,
  BAR_GPIO_NUM_34,
  BAR_GPIO_NUM_35,
  BAR_GPIO_NUM_36,
  BAR_GPIO_NUM_37,
  BAR_GPIO_NUM_38,
  BAR_GPIO_NUM_39,
  BAR_GPIO_NUM_MAX = 40,
} eGpioNum_t;


typedef enum eGpioMode
{
  BAR_GPIO_MODE_INPUT,
  BAR_GPIO_MODE_OUTPUT,
  BAR_GPIO_MODE_OUTPUT_OPEN_DRAIN,
  BAR_GPIO_MODE_INPUT_OUTPUT,
  BAR_GPIO_MODE_INPUT_OUTPUT_OPEN_DRAIN,
} eGpioMode_t;

typedef enum eGpioPull
{
  BAR_GPIO_PULL_NONE,
  BAR_GPIO_PULL_UP,
  BAR_GPIO_PULL_DOWN,
} eGpioPull_t;

typedef enum eGpioLevel
{
  BAR_LEVEL_LOW,
  BAR_LEVEL_HIGH,
} eGpioLevel_t;


#ifdef ESP32
  #include "Esp32Gpio.h"
  #define Gpio_init       Esp32Gpio_init
  #define Gpio_destroy    Esp32Gpio_destroy
  #define Gpio_configure  Esp32Gpio_configure
  #define Gpio_set        Esp32Gpio_set
  #define Gpio_get        Esp32Gpio_get
  #define Gpio_toggle     Esp32Gpio_toggle
#else
  #error "board/hardware is not defined"
#endif

#endif // _GPIO_H_

