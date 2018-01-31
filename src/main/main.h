#ifndef MAIN_H_
#define MAIN_H_

typedef enum eWifiGateEvent
{
  ESP_GATE_RESET,
  ESP_GATE_IAP_START,
  ESP_GATE_IAP_BOX_CONNECTED,
  ESP_GATE_IAP_BOX_DISCONNECTED,
  ESP_GATE_IAP_BAR_START,
  ESP_GATE_IAP_BAR_CONNECTED,
  ESP_GATE_IAP_BAR_DISCONNECTED,
  ESP_GATE_IAP_BAR_RECEIVED_REQ,
  ESP_GATE_AP_START,
  ESP_GATE_AP_CONNECTED,
  ESP_GATE_SW_UPATE_REQ,
  ESP_GATE_SW_UPATE_DONE,
  ESP_GATE_AP_TIMEOUT,
} eWifiGateEvent_t;

#endif /* MAIN_H_ */

