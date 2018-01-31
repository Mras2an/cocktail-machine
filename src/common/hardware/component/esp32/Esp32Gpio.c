#ifdef ESP32

#include "driver/gpio.h"
#include "Esp32Gpio.h"
#include "Debug.h"

/******************************************************************************
 * FunctionName : eBarError_t Esp32Gpio_init(void)
 * Parameters   : none
 * Returns      : eBarError_t
 *******************************************************************************/
eBarError_t Esp32Gpio_init(void)
{
  return BAR_NO_ERROR;
}

/******************************************************************************
 * FunctionName : eBarError_t Esp32Gpio_destroy(void)
 * Parameters   : none
 * Returns      : eBarError_t
 *******************************************************************************/
eBarError_t Esp32Gpio_destroy(void)
{
  return BAR_NO_ERROR;
}

/******************************************************************************
 * FunctionName : eBarError_t Esp32Gpio_configure(eGpioNum_t pin, eGpioMode_t mode, eGpioPull_t pull)
 * Parameters   : uint32_t pin
 * Parameters   : eGpioMode mode
 * Parameters   : eGpioPull_t pull
 * Returns      : eBarError_t
 *******************************************************************************/
eBarError_t Esp32Gpio_configure(eGpioNum_t pin, eGpioMode_t mode, eGpioPull_t pull)
{
  if(pin >= BAR_GPIO_NUM_MAX)
  {
    return BAR_ERROR(BAR_INVALID_PARAMETER);
  }

  gpio_mode_t m;

  switch(mode)
  {
    case BAR_GPIO_MODE_INPUT:
      m = GPIO_MODE_INPUT;
      break;

    case BAR_GPIO_MODE_OUTPUT:
      m=GPIO_MODE_OUTPUT;
      break;

    case BAR_GPIO_MODE_OUTPUT_OPEN_DRAIN:
      m= GPIO_MODE_OUTPUT_OD;
      break;

    case BAR_GPIO_MODE_INPUT_OUTPUT:
      m= GPIO_MODE_INPUT_OUTPUT;
      break;

    case BAR_GPIO_MODE_INPUT_OUTPUT_OPEN_DRAIN:
      m= GPIO_MODE_INPUT_OUTPUT_OD;
      break;

    default:
      return BAR_ERROR(BAR_INVALID_PARAMETER);
  }

  gpio_pullup_t up;
  gpio_pulldown_t down;

  switch(pull)
  {
    case BAR_GPIO_PULL_NONE:
      up = GPIO_PULLUP_DISABLE;
      down = GPIO_PULLDOWN_DISABLE;
      break;

    case BAR_GPIO_PULL_UP:
      up = GPIO_PULLUP_ENABLE;
      down = GPIO_PULLDOWN_ENABLE;
      break;

    case BAR_GPIO_PULL_DOWN:
      up = GPIO_PULLUP_DISABLE;
      down = GPIO_PULLDOWN_ENABLE;
      break;

    default:
      return BAR_ERROR(BAR_INVALID_PARAMETER);
  }

  gpio_config_t io_conf;
  io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
  io_conf.mode = m;
  io_conf.pin_bit_mask = (((uint64_t) 1) << pin);
  io_conf.pull_down_en = down;
  io_conf.pull_up_en = up;

  if(gpio_config(&io_conf) != ESP_OK)
  {
    return BAR_ERROR(BAR_INVALID_PARAMETER);
  }

  return BAR_NO_ERROR;
}

/******************************************************************************
 * FunctionName : eBarError_t Esp32Gpio_set(eGpioNum_t pin, uint32_t value)
 * Parameters   : eGpioNum_t, uint32_t
 * Returns      : none
 *******************************************************************************/
eBarError_t Esp32Gpio_set(eGpioNum_t pin, uint32_t value)
{
  if(gpio_set_level(pin, value) != ESP_OK)
  {
    return BAR_ERROR(BAR_INVALID_PARAMETER);
  }

  return BAR_NO_ERROR;
}

/******************************************************************************
 * FunctionName : eBarError_t Esp32Gpio_get(eGpioNum_t pin, uint32_t *value)
 * Parameters   : eGpioNum_t, *uint32_t
 * Returns      : eBarError_t
 *******************************************************************************/
eBarError_t Esp32Gpio_get(eGpioNum_t pin, uint32_t *value)
{
  *value = gpio_get_level(pin);
  return BAR_NO_ERROR;
}

/******************************************************************************
 * FunctionName : eBarError_t Esp32Gpio_toggle(eGpioNum_t pin)
 * Parameters   : eGpioNum_t pin
 * Returns      : eBarError_t
 *******************************************************************************/
eBarError_t Esp32Gpio_toggle(eGpioNum_t pin)
{
  if(gpio_get_level(pin))
  {
    gpio_set_level(pin, 0);
  }
  else
  {
    gpio_set_level(pin, 1);
  }

  return BAR_NO_ERROR;
}

#endif // ESP32

