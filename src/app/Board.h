#ifndef _BOARD_H_
#define _BOARD_H_

#include "Gpio.h"

#ifdef ESP32
    #define LED_GPIO_RED            BAR_GPIO_NUM_32
    #define LED_GPIO_GREEN          BAR_GPIO_NUM_33
    #define LED_GPIO_BLUE           BAR_GPIO_NUM_25

    #define MOTOR_AXE_X_DIR         BAR_GPIO_NUM_12
    #define MOTOR_AXE_X_CLK         BAR_GPIO_NUM_13
    #define MOTOR_AXE_Y_DIR         BAR_GPIO_NUM_14
    #define MOTOR_AXE_Y_CLK         BAR_GPIO_NUM_15

    #define MOTOR_PUMP_1            BAR_GPIO_NUM_5
    #define MOTOR_PUMP_2            BAR_GPIO_NUM_18
    #define MOTOR_PUMP_3            BAR_GPIO_NUM_19

    #define DETECTION_AXE_Y         BAR_GPIO_NUM_34
    #define DETECTION_AXE_X         BAR_GPIO_NUM_35

    #define BUTTON_GPIO_MODE        BAR_GPIO_NUM_39
#else
    #error "board/hardware is not defined"
#endif


#endif /* _BOARD_H_ */
