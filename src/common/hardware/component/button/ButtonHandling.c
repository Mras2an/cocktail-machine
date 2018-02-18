#include "ButtonHandling.h"
#include "System.h"
#include "Board.h"
#include "Utils.h"
#include "SmartConfig.h"

static OsQueueHandle_t tsQueueForButton = NULL;

static void ButtonHandling_task(void * pvParameters);

#define TIME_TO_REBOOT 10

/*!*****************************************************************************
 * \fn         void buttonHandling_init(void)
 * \param[in]  none
 * \return     none
 *******************************************************************************/
void ButtonHandling_init(void)
{
  if(tsQueueForButton == NULL)
    tsQueueForButton = xQueueCreate(10, 1);

  if(tsQueueForButton != NULL)
    xTaskCreate(ButtonHandling_task, "ButtonTask", 1500, NULL, 4, NULL);
}

/*!*****************************************************************************
 * \fn         void ButtonHandling_executeTask(eButtonMode_t eEvent)
 * \param[in]  eButtonMode_t
 * \return     none
 *******************************************************************************/
void ButtonHandling_executeTask(eButtonMode_t eEvent)
{
  xQueueSend(tsQueueForButton, &eEvent, 0);
}

/*!*****************************************************************************
 * \fn         static void ButtonHandling_execution(unsigned int time)
 * \param[in]  unsigned int
 * \return     none
 *******************************************************************************/
//static void ButtonHandling_execution(unsigned int time)
//{
//  /* 3 sec = 6 * 500ms */
//  if ((time * 500) < 3000)
//  {
//    //remove wi-i ssid and passwd
//    SmartConfig_factoryReset();
//  }
//}

/*!*****************************************************************************
 * \fn         static void ButtonHandling_task(void * pvParameters)
 * \param[in]  void *
 * \return     none
 *******************************************************************************/
static void ButtonHandling_task(void * pvParameters)
{
  eButtonMode_t eAsyncMsg = IDLE_BUTTON;
  unsigned int time = 0;

  for(;;)
  {
    xQueueReceive(tsQueueForButton, &eAsyncMsg, 500);
    uint32_t value[1];
    Gpio_get(BUTTON_GPIO_MODE, value);

    switch(eAsyncMsg)
    {
      case IDLE_BUTTON:
      {
        if(value[0] == FALSE)
        {
          time = 0;
          ButtonHandling_executeTask(RISING_BUTTON);
        }
      }
      break;

      case RISING_BUTTON:
      {
        if(value[0] == TRUE)
        {
          ButtonHandling_executeTask(FALLING_BUTTON);
        }
        else
        {
          time++;

          if(time == TIME_TO_REBOOT)
          {
            System_cpuReset();
          }
        }
      }
      break;

      case FALLING_BUTTON:
      {
        //ButtonHandling_execution(time);
        time = 0;
        ButtonHandling_executeTask(IDLE_BUTTON);
      }
      break;

      default:
        ;
    }
  }
}

