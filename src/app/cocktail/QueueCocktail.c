#include "QueueCocktail.h"
#include "Cocktail.h"
#include "CpuDelay.h"
#include "Bsp.h"
#include "MotorHandling.h"
#include "LedRGBHandling.h"

typedef struct sCtxQueueCocktail
{
  OsTaskHandle_t taskId;
  OsQueueHandle_t xQueueCocktailEventQueue;
} sCtxQueueCocktail_t;

static sCtxQueueCocktail_t ctx, *pCtx = &ctx;

typedef struct
{
  int bottle[MAX_BOTTLE];
  int position[MAX_BOTTLE];
  int measure[MAX_BOTTLE];
} sBottleList;
sBottleList bottleList;

static void QueueCocktail_receivedTask(void* pvParameters);

/*!*****************************************************************************
 * \fn         int QueueCocktail_init()
 * \brief      Queue init
 * \param[in]  none
 * \return     int
 *******************************************************************************/
int QueueCocktail_init(void)
{
  memset(pCtx, 0x00, sizeof(sCtxQueueCocktail_t));
  pCtx->xQueueCocktailEventQueue = OsQueueCreate(5, sizeof(sCtxQueueCocktail_t));

  if(pCtx->xQueueCocktailEventQueue == NULL)
  {
    return 1;
  }

  if(OsTaskCreate(QueueCocktail_receivedTask, "queueReceivedTask", 4096, NULL, 4, &(pCtx->taskId)) != pdPASS)
  {
    return 1;
  }

  return 0;
}

/*!*****************************************************************************
 * \fn         static void QueueCocktail_receivedTask(void* pvParameters)
 * \brief      received task
 * \param[in]  void* pvParameters
 * \return     void
 *******************************************************************************/
static void QueueCocktail_receivedTask(void* pvParameters)
{
  while(1)
  {
    int QueueCocktail;
    /* Wait until there is something to do. */
    OsQueueReceive(pCtx->xQueueCocktailEventQueue, &QueueCocktail, OsPortTimingPeriod);
    LedRGBHandling_ExecuteLedTaskFromISR(BLUE_LED_FAST_BLINKING);
    MotorHandling_setInitialPosition();
    memset(bottleList.bottle, 255, MAX_BOTTLE);
    memset(bottleList.position, 255, MAX_BOTTLE);
    memset(bottleList.measure, 255, MAX_BOTTLE);
    int nbIngredients = Cocktail_getDispoIngredients(bottleList.bottle, bottleList.position, bottleList.measure, QueueCocktail);
    BarDebug_info("   nb ingredients: %i\n\n", nbIngredients);
    int goToPosition = 0;
    int currentPosition = 0;

    for(int i = 0; i < nbIngredients; i++)
    {
      BarDebug_info("\t bottle: %i\n", bottleList.bottle[i]);
      BarDebug_info("\t position: %i\n", bottleList.position[i]);
      BarDebug_info("\t measure: %i\n\n", bottleList.measure[i]);

      if(currentPosition != bottleList.position[i])
      {
        goToPosition = bottleList.position[i] - currentPosition;
        MotorHandling_setPositionOnX(goToPosition);
        currentPosition += goToPosition;
        BarDebug_info("\t\t currentPosition %i, goToPosition %i\n",currentPosition ,goToPosition);
        CpuDelay_ms(500);
      }

      {
        if(currentPosition != 0)
        {
          MotorHandling_getAMeasureOnY(bottleList.measure[i]);
        }
        else
        {
          MotorHandling_getAMeasureOnPump(bottleList.measure[i]);
        }
      }
    }

    MotorHandling_setInitialPosition();
    LedRGBHandling_ExecuteLedTaskFromISR(BLUE_LED);
  }

  OsTaskDelete(NULL);
}

/*!*****************************************************************************
 * \fn         int QueueCocktail_received(int **newQueueCocktail)
 * \brief      Queue received
 * \param[in]  sQueueCocktail_t* newQueueCocktail
 * \return     int
 *******************************************************************************/
int QueueCocktail_received(int newQueueCocktail)
{
  if(OsQueueSend(pCtx->xQueueCocktailEventQueue, &newQueueCocktail, 0) != pdPASS)
  {
    return 1;
  }

  return 0;
}
