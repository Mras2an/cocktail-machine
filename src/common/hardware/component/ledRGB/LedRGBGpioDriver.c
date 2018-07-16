#include "LedRGBGpioDriver.h"

static sLedRgbGpioConfig_t sLedRgbGpioConfig;
#define this (&sLedRgbGpioConfig)

/*!*****************************************************************************
 * \fn         void LedRGBGpioDriver_Init(void)
 * \brief      Init LED
 * \param[in]  none
 * \return     none
 *******************************************************************************/
eBarError_t LedRGBGpioDriver_Init(const sLedRgbGpioConfig_t *config)
{
    *this = *config;
    /* Init */
    BAR_ERROR_CHECK(Gpio_configure(this->blue, BAR_GPIO_MODE_INPUT_OUTPUT, BAR_GPIO_PULL_UP));
    BAR_ERROR_CHECK(Gpio_configure(this->red, BAR_GPIO_MODE_INPUT_OUTPUT, BAR_GPIO_PULL_UP));
    BAR_ERROR_CHECK(Gpio_configure(this->green, BAR_GPIO_MODE_INPUT_OUTPUT, BAR_GPIO_PULL_UP));
    /* Set low */
    BAR_ERROR_CHECK(Gpio_set(this->red, BAR_LEVEL_LOW));
    BAR_ERROR_CHECK(Gpio_set(this->blue, BAR_LEVEL_LOW));
    BAR_ERROR_CHECK(Gpio_set(this->green, BAR_LEVEL_LOW));
    return BAR_NO_ERROR;
}

/*!*****************************************************************************
 * \fn         int LedRGBGpioDriver_SetColor(int color)
 * \brief      Set LED color
 * \param[in]  int
 * \return     int
 *******************************************************************************/
eLedColor_t LedRGBGpioDriver_SetColor(eLedColor_t color)
{
    eLedColor_t c = LED_NOT_DEFINED;
    /* Set low */
    BAR_ERROR_CHECK(Gpio_set(this->red, BAR_LEVEL_LOW));
    BAR_ERROR_CHECK(Gpio_set(this->blue, BAR_LEVEL_LOW));
    BAR_ERROR_CHECK(Gpio_set(this->green, BAR_LEVEL_LOW));

    if((color & LED_GREEN) ==  LED_GREEN) {
        BAR_ERROR_CHECK(Gpio_set(this->green, BAR_LEVEL_HIGH));
        c |= LED_GREEN;
    }

    if((color & LED_RED) == LED_RED) {
        BAR_ERROR_CHECK(Gpio_set(this->red, BAR_LEVEL_HIGH));
        c |= LED_RED;
    }

    if((color & LED_BLUE) == LED_BLUE) {
        BAR_ERROR_CHECK(Gpio_set(this->blue, BAR_LEVEL_HIGH));
        c |= LED_BLUE;
    }

    return c;
}

/*!*****************************************************************************
 * \fn         int LedRGBGpioDriver_ToggleColor(int color)
 * \brief      toggle LED color
 * \param[in]  int
 * \return     int
 *******************************************************************************/
eLedColor_t LedRGBGpioDriver_ToggleColor(eLedColor_t color)
{
    int c = LED_NOT_DEFINED;

    if((color & LED_GREEN) != LED_GREEN) {
        BAR_ERROR_CHECK(Gpio_set(this->green, BAR_LEVEL_LOW));
    }

    if((color & LED_RED) != LED_RED) {
        BAR_ERROR_CHECK(Gpio_set(this->red, BAR_LEVEL_LOW));
    }

    if((color & LED_BLUE) != LED_BLUE) {
        BAR_ERROR_CHECK(Gpio_set(this->blue, BAR_LEVEL_LOW));
    }

    if((color & LED_GREEN) ==  LED_GREEN) {
        BAR_ERROR_CHECK(Gpio_toggle(this->green));
        c |= LED_GREEN;
    }

    if((color & LED_RED) == LED_RED) {
        BAR_ERROR_CHECK(Gpio_toggle(this->red));
        c |= LED_RED;
    }

    if((color & LED_BLUE) == LED_BLUE) {
        BAR_ERROR_CHECK(Gpio_toggle(this->blue));
        c |= LED_BLUE;
    }

    return c;
}

/*!*****************************************************************************
 * \fn         int LedRGBGpioDriver_SetColor(int color)
 * \brief      Get LED color
 * \param[in]  none
 * \return     int
 *******************************************************************************/
eLedColor_t LedRGBGpioDriver_GetColor(void)
{
    eLedColor_t led = 0;
    uint32_t value1[1], value2[1], value3[1];
    Gpio_get(this->green, value1);
    Gpio_get(this->red, value2);
    Gpio_get(this->blue, value3);

    if(value1[0]) {
        led |= LED_GREEN;
    }

    if(value2[0]) {
        led |= LED_RED;
    }

    if(value3[0]) {
        led |= LED_BLUE;
    }

    return led;
}
