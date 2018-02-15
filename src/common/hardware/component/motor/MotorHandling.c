#include <stdio.h>
#include "CpuDelay.h"
#include "Gpio.h"
#include "MotorHandling.h"
#include "Board.h"

//#define MOTOR_OFFSET 200 //360°
#define MOTOR_OFFSET 495   //10cm
#define MOTOR_LIMIT 800

/******************************************************************************
 * FunctionName : void MotorHandling_init(void)
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void MotorHandling_init(void)
{
  //x
  BAR_ERROR_CHECK(
    Gpio_configure(MOTOR_AXE_X_DIR, BAR_GPIO_MODE_INPUT_OUTPUT, BAR_GPIO_PULL_NONE));
  BAR_ERROR_CHECK(
    Gpio_configure(MOTOR_AXE_X_CLK, BAR_GPIO_MODE_INPUT_OUTPUT, BAR_GPIO_PULL_NONE));
  //y
  BAR_ERROR_CHECK(
    Gpio_configure(MOTOR_AXE_Y_DIR, BAR_GPIO_MODE_INPUT_OUTPUT, BAR_GPIO_PULL_NONE));
  BAR_ERROR_CHECK(
    Gpio_configure(MOTOR_AXE_Y_CLK, BAR_GPIO_MODE_INPUT_OUTPUT, BAR_GPIO_PULL_NONE));
  // pump
  BAR_ERROR_CHECK(
    Gpio_configure(MOTOR_PUMP_1, BAR_GPIO_MODE_INPUT_OUTPUT, BAR_GPIO_PULL_NONE));
  BAR_ERROR_CHECK(
    Gpio_configure(MOTOR_PUMP_2, BAR_GPIO_MODE_INPUT_OUTPUT, BAR_GPIO_PULL_NONE));
  BAR_ERROR_CHECK(
    Gpio_configure(MOTOR_PUMP_3, BAR_GPIO_MODE_INPUT_OUTPUT, BAR_GPIO_PULL_NONE));
}

/******************************************************************************
 * FunctionName : void MotorHandling_setPositionOnX(int position)
 * Parameters   : int
 * Returns      : none
 *******************************************************************************/
void MotorHandling_setPositionOnX(int position)
{
  if(position == 0)
  {
    MotorHandling_setInitialPosition();
    return;
  }

  int end = 0;

  if(position > 0)
  {
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_X_DIR, BAR_LEVEL_LOW));
    end = position * MOTOR_OFFSET;
  }
  else
  {
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_X_DIR, BAR_LEVEL_HIGH));
    end = (position * (-1)) * MOTOR_OFFSET;
  }

  for(int i = 0; i < end; i++)
  {
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_X_CLK, BAR_LEVEL_LOW));
    CpuDelay_ms(1);
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_X_CLK, BAR_LEVEL_HIGH));
    CpuDelay_ms(1);
  }
}

/******************************************************************************
 * FunctionName : static void MotorHandling_get10ml()
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
static void MotorHandling_get10ml()
{
  int end = MOTOR_LIMIT;
  BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_DIR, BAR_LEVEL_LOW));

  for(int i = 0; i < end; i++)
  {
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, BAR_LEVEL_LOW));
    CpuDelay_ms(1);
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, BAR_LEVEL_HIGH));
    CpuDelay_ms(1);
  }

  CpuDelay_ms(800);
  uint32_t bp2[1];
  Gpio_get(DETECTION_AXE_Y, bp2);
  BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_DIR, BAR_LEVEL_HIGH));
  end = MOTOR_LIMIT;

  while(bp2[0] && (end != 0))
  {
    Gpio_get(DETECTION_AXE_Y, bp2);
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, BAR_LEVEL_LOW));
    CpuDelay_ms(1);
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, BAR_LEVEL_HIGH));
    CpuDelay_ms(1);
    end--;
  }
  CpuDelay_ms(800);
}

/******************************************************************************
 * FunctionName : static void MotorHandling_get25ml()
 * Parameters   : void
 * Returns      : none
 *******************************************************************************/
static void MotorHandling_get25ml()
{
  int end = MOTOR_LIMIT;
  BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_DIR, BAR_LEVEL_LOW));

  for(int i = 0; i < end; i++)
  {
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, BAR_LEVEL_LOW));
    CpuDelay_ms(1);
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, BAR_LEVEL_HIGH));
    CpuDelay_ms(1);
  }

  CpuDelay_ms(2000);
  uint32_t bp2[1];
  Gpio_get(DETECTION_AXE_Y, bp2);
  BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_DIR, BAR_LEVEL_HIGH));
  end = MOTOR_LIMIT;

  while(bp2[0] && (end != 0))
  {
    Gpio_get(DETECTION_AXE_Y, bp2);
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, BAR_LEVEL_LOW));
    CpuDelay_ms(1);
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, BAR_LEVEL_HIGH));
    CpuDelay_ms(1);
    end--;
  }

  CpuDelay_ms(800);
}

/******************************************************************************
 * FunctionName : void MotorHandling_getAMeasureOnY(int measure)
 * Parameters   : int
 * Returns      : none
 *******************************************************************************/
void MotorHandling_getAMeasureOnY(int measure)
{
  BarDebug_info("\t\t MotorHandling_getAMeasureOnY \n");

  if(measure == 1)
  {
    MotorHandling_get10ml();
  }

  if(measure == 2)
  {
    MotorHandling_get25ml();
  }

  if(measure == 3)
  {
    MotorHandling_get25ml();
    MotorHandling_get10ml();
  }

  if(measure == 4)
  {
    MotorHandling_get25ml();
    MotorHandling_get25ml();
  }

  if(measure == 5)
  {
    MotorHandling_get25ml();
    MotorHandling_get25ml();
    MotorHandling_get10ml();
  }

}

/******************************************************************************
 * FunctionName : void MotorHandling_getAMeasureOnPump(int measure)
 * Parameters   : int
 * Returns      : none
 *******************************************************************************/
void MotorHandling_getAMeasureOnPump(int measure)
{
  BarDebug_info("\t\t MotorHandling_getAMeasureOnPump \n");
  BAR_ERROR_CHECK(Gpio_set(MOTOR_PUMP_3, BAR_LEVEL_HIGH));

  CpuDelay_ms(500);

  for(int j = 0; j < measure; j++)
  {
    CpuDelay_ms(550);
  }

  BAR_ERROR_CHECK(Gpio_set(MOTOR_PUMP_3, BAR_LEVEL_LOW));
}

/******************************************************************************
 * FunctionName : void MotorHandling_setInitialPosition()
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void MotorHandling_setInitialPosition()
{
  uint32_t bp1[1], bp2[1];
  BarDebug_info("Set motor at the initial position on y...\n");
  Gpio_get(DETECTION_AXE_Y, bp2);
  BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_DIR, BAR_LEVEL_HIGH));
  int end = MOTOR_LIMIT;

  while(bp2[0] && (end != 0))
  {
    Gpio_get(DETECTION_AXE_Y, bp2);
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, BAR_LEVEL_LOW));
    CpuDelay_ms(1);
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_Y_CLK, BAR_LEVEL_HIGH));
    CpuDelay_ms(1);
    end--;
  }

  BarDebug_info("Motor is now at the initial position on y.\n");
  BarDebug_info("Set motor at the initial position on x...\n");
  Gpio_get(DETECTION_AXE_X, bp1);
  BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_X_DIR, BAR_LEVEL_HIGH));

  while(bp1[0])
  {
    Gpio_get(DETECTION_AXE_X, bp1);
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_X_CLK, BAR_LEVEL_LOW));
    CpuDelay_ms(1);
    BAR_ERROR_CHECK(Gpio_set(MOTOR_AXE_X_CLK, BAR_LEVEL_HIGH));
    CpuDelay_ms(1);
  }

  BarDebug_info("Motor is now at the initial position on x.\n");
}
