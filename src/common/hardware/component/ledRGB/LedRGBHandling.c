#include "LedRGBGpioDriver.h"

static OsQueueHandle_t tsQueueForLed = NULL;

static eLedMode_t eAsyncCurrent;
static eLedMode_t enableLed;

#define BLINK_SLOW 600
#define BLINK_FAST 100

/*!************************************************************************************************
 * \fn         void LedRGBHandling_ExecuteLedTaskFromISR(eLedMode_t eEvent)
 * \brief      execute event in the LED manager.
 * \param[in]  eLedMode_t
 * \return     void
 ***************************************************************************************************/
void LedRGBHandling_ExecuteLedTaskFromISR(eLedMode_t eEvent)
{
  OsQueueSendFromISR(tsQueueForLed, &eEvent, NULL);
}

/*!*****************************************************************************
 * \fn         int LedRGBHandling_GetSpeed(void)
 * \brief      Get speed for blinking LED
 * \param[in]  none
 * \return     int
 *******************************************************************************/
int LedRGBHandling_GetSpeed(void)
{
  return 0;
}

/*!*****************************************************************************
 * \fn         void LedRGBHandling_SetSpeed(int speed)
 * \brief      Set speed for blinking LED
 * \param[in]  int
 * \return     none
 *******************************************************************************/
void LedRGBHandling_SetSpeed(int speed)
{
}

/*!*****************************************************************************
 * \fn         static void LedRGBHandling_Task(void *pvParameters)
 * \brief      Task for LED color
 * \param[in]  void *
 * \return     none
 *******************************************************************************/
static void LedRGBHandling_Task(void * pvParameters)
{
  eLedMode_t eAsyncMsg = IDEL_LED;
  eAsyncCurrent = eAsyncMsg;
  portTickType queueReceiveDelay = OsPortTimingPeriod;

  for(;;)
  {
    OsQueueReceive(tsQueueForLed, &eAsyncMsg, queueReceiveDelay);
    eAsyncCurrent = eAsyncMsg;

    if(IDEL_LED == eAsyncMsg)
    {
      LedRGBGpioDriver_SetColor(LED_NOT_DEFINED);
    }
    else if(GREEN_LED == eAsyncMsg)
    {
      queueReceiveDelay = OsPortTimingPeriod;

      if(!LedRGBGpioDriver_SetColor(LED_GREEN))
      {
        BarDebug_info("LED NOT DEFINED");
      }
    }
    else if((ORANGE_LED | enableLed) == eAsyncMsg)
    {
      queueReceiveDelay = OsPortTimingPeriod;

      if(!LedRGBGpioDriver_SetColor(LED_ORANGE))
      {
        BarDebug_info("LED NOT DEFINED");
      }
    }
    else if(RED_LED == eAsyncMsg)
    {
      queueReceiveDelay = OsPortTimingPeriod;

      if(!LedRGBGpioDriver_SetColor(LED_RED))
      {
        BarDebug_info("LED NOT DEFINED");
      }
    }
    else if((BLUE_LED | enableLed) == eAsyncMsg)
    {
      queueReceiveDelay = OsPortTimingPeriod;

      if(!LedRGBGpioDriver_SetColor(LED_BLUE))
      {
        BarDebug_info("LED NOT DEFINED");
      }
    }
    else if(GREEN_LED_SLOW_BLINKING == eAsyncMsg)
    {
      queueReceiveDelay = BLINK_SLOW;

      if(!LedRGBGpioDriver_ToggleColor(LED_GREEN))
      {
        BarDebug_info("LED NOT DEFINED");
      }
    }
    else if((ORANGE_LED_SLOW_BLINKING | enableLed) == eAsyncMsg)
    {
      queueReceiveDelay = BLINK_SLOW;

      if(!LedRGBGpioDriver_ToggleColor(LED_ORANGE))
      {
        BarDebug_info("LED NOT DEFINED");
      }
    }
    else if(RED_LED_SLOW_BLINKING == eAsyncMsg)
    {
      queueReceiveDelay = BLINK_SLOW;

      if(!LedRGBGpioDriver_ToggleColor(LED_RED))
      {
        BarDebug_info("LED NOT DEFINED");
      }
    }
    else if((BLUE_LED_SLOW_BLINKING | enableLed) == eAsyncMsg)
    {
      queueReceiveDelay = BLINK_SLOW;

      if(!LedRGBGpioDriver_ToggleColor(LED_BLUE))
      {
        BarDebug_info("LED NOT DEFINED");
      }
    }
    else if(GREEN_LED_FAST_BLINKING == eAsyncMsg)
    {
      queueReceiveDelay = BLINK_FAST;

      if(!LedRGBGpioDriver_ToggleColor(LED_GREEN))
      {
        BarDebug_info("LED NOT DEFINED");
      }
    }
    else if((ORANGE_LED_FAST_BLINKING | enableLed) == eAsyncMsg)
    {
      queueReceiveDelay = BLINK_FAST;

      if(!LedRGBGpioDriver_ToggleColor(LED_ORANGE))
      {
        BarDebug_info("LED NOT DEFINED");
      }
    }
    else if(RED_LED_FAST_BLINKING == eAsyncMsg)
    {
      queueReceiveDelay = BLINK_FAST;

      if(!LedRGBGpioDriver_ToggleColor(LED_RED))
      {
        BarDebug_info("LED NOT DEFINED");
      }
    }
    else if((BLUE_LED_FAST_BLINKING | enableLed) == eAsyncMsg)
    {
      queueReceiveDelay = BLINK_FAST;

      if(!LedRGBGpioDriver_ToggleColor(LED_BLUE))
      {
        BarDebug_info("LED NOT DEFINED");
      }
    }
    else if(MULTI_2_LED_FAST_BLINKING == eAsyncMsg)
    {
      queueReceiveDelay = BLINK_SLOW;

      if(LedRGBGpioDriver_GetColor() == LED_GREEN)
      {
        LedRGBGpioDriver_SetColor(LED_BLUE);
      }
      else
      {
        LedRGBGpioDriver_SetColor(LED_GREEN);
      }
    }
    else if(MULTI_3_LED_FAST_BLINKING == eAsyncMsg)
    {
      queueReceiveDelay = 250;

      if(LedRGBGpioDriver_GetColor() == LED_GREEN)
      {
        LedRGBGpioDriver_SetColor(LED_BLUE);
      }
      else if(LedRGBGpioDriver_GetColor() == LED_BLUE)
      {
        LedRGBGpioDriver_SetColor(LED_RED);
      }
      else if(LedRGBGpioDriver_GetColor() == LED_RED)
      {
        LedRGBGpioDriver_SetColor(LED_GREEN);
      }
    }
    else
    {
      LedRGBGpioDriver_SetColor(LED_NOT_DEFINED);
      queueReceiveDelay = OsPortTimingPeriod;
    }
  }

  OsQueueDelete(tsQueueForLed);
  tsQueueForLed = NULL;
}

/*!*****************************************************************************
 * \fn         void LedRGBHandling_enableLed(int status)
 * \brief      0x00 = enable; 0xff = disable;
 * \param[in]  none
 * \return     none
 *******************************************************************************/
void LedRGBHandling_enableLed(int status)
{
  if(status)
  {
    enableLed = DISABLE_LED;
  }
  else
  {
    enableLed = IDEL_LED;
  }

  LedRGBHandling_ExecuteLedTaskFromISR(eAsyncCurrent);
}

/*!*****************************************************************************
 * \fn         void LedRGBHandling_Init(void)
 * \brief      init LED manager
 * \param[in]  none
 * \return     none
 *******************************************************************************/
void LedRGBHandling_Init(const sLedRgbGpioConfig_t *config)
{
  enableLed = IDEL_LED;
  BAR_ERROR_CHECK(LedRGBGpioDriver_Init(config));

  if(tsQueueForLed == NULL)
    tsQueueForLed = OsQueueCreate(10, 1);

  if(tsQueueForLed != NULL)
    OsTaskCreate(LedRGBHandling_Task, "LED task", 2048, NULL, 4, NULL);
}
