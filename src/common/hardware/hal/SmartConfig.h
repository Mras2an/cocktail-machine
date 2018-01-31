#ifndef SMART_CONFIG_H_
#define SMART_CONFIG_H_

#ifdef ESP32
  #ifdef SMART_CONFIG
    #include "Esp32SmartConfig.h"
    #define  SmartConfig_createTask            Esp32SmartConfig_createTask
    #define  SmartConfig_groupSetBits          Esp32SmartConfig_groupSetBits
    #define  SmartConfig_groupClearBits        Esp32SmartConfig_groupClearBits
    #define  SmartConfig_init                  Esp32SmartConfig_init
    #define  SmartConfig_factoryReset          Esp32SmartConfig_factoryReset
    #define  SmartConfig_isSmartconfigEnable   Esp32SmartConfig_getSmartconfigEnable
  #else
    #error "Smart config is not defined"
  #endif
#else
  #error "board/hardware is not defined"
#endif

#endif /* SMART_CONFIG_H_ */
