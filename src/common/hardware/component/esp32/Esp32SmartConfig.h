
#ifndef ESP32_SMART_CONFIG_H_
#define ESP32_SMART_CONFIG_H_

void Esp32SmartConfig_createTask();

void Esp32SmartConfig_groupSetBits();

void Esp32SmartConfig_groupClearBits();

void Esp32SmartConfig_init(void);

void Esp32SmartConfig_factoryReset();

int Esp32SmartConfig_getSmartconfigEnable(void);

#endif /* ESP32_SMART_CONFIG_H_ */

