#ifdef ESP32
#include "Bsp.h"

#define   ESP32_NOP()   asm("NOP");

/******************************************************************************
 * FunctionName : eBarError_t Esp32Delay_init(void)
 * Parameters   : none
 * Returns      : eBarError_t
 *******************************************************************************/
eBarError_t Esp32Delay_init(void)
{
    return BAR_NO_ERROR;
}

/******************************************************************************
 * FunctionName : eBarError_t Esp32Delay_destroy(void)
 * Parameters   : none
 * Returns      : eBarError_t
 *******************************************************************************/
eBarError_t Esp32Delay_destroy(void)
{
    return BAR_NO_ERROR;
}

/******************************************************************************
 * FunctionName : void  __attribute__((optimize(0))) Esp32Delay_ms(short delay)
 * Parameters   : none
 * Returns      : void
 *******************************************************************************/
void __attribute__((optimize(0))) Esp32Delay_ms(short delay)
{
    vTaskDelay(delay / portTICK_RATE_MS);
}
/******************************************************************************
 * FunctionName : void  __attribute__((optimize(0))) Esp32Delay_us(short delay)
 * Parameters   : none
 * Returns      : void
 *******************************************************************************/
void __attribute__((optimize(0))) Esp32Delay_us(short delay)
{
    while(delay > 0) {
        /* In order to ensure that the loop will be not optimized by compiler */
        ets_delay_us(1);
        delay--;
    }
}
#endif // ESP32
