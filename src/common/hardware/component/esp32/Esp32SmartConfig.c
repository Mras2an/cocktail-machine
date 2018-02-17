#ifdef ESP32
#ifdef SMART_CONFIG

#include "esp_wifi.h"
#include "esp_smartconfig.h"
#include "Wifi.h"
#include "LedRGBHandling.h"
#include "System.h"
#include "CpuDelay.h"
typedef struct sSmartConfig
{
  char * ssid;
  char * pass;
  int smartConfigEnable;
} sSmartConfig_t;

static sSmartConfig_t sSmartConfig;
#define this (&sSmartConfig)
#define SMARTCONFIG_PERIOD (40000 / portTICK_PERIOD_MS)

static OsEventGroupHandle_t wifi_event_group;
static OsTimerHandle_t smartconfigTimer;

static const int CONNECTED_BIT = BIT0;
static const int ESPTOUCH_DONE_BIT = BIT1;

void Esp32SmartConfig_task(void * parm);
static void Esp32SmartConfig_Callback(OsTimerHandle_t smartconfigTimer);

/*!*****************************************************************************
 * \fn         void Esp32SmartConfig_init(void)
 * \param[in]  none
 * \return     void
 *******************************************************************************/
void Esp32SmartConfig_init(void)
{
  this->smartConfigEnable = 0;
  char * ssid = (char *)Wifi_getSsid();
  char * pass = (char *)Wifi_getPassword();

  if((ssid == NULL) || (pass == NULL))
  {
    this->smartConfigEnable = 1;
    wifi_event_group = xEventGroupCreate();
    this->ssid = NULL;
    this->pass = NULL;
  }

  if(ssid != NULL)
    OsFree(ssid);

  if(pass != NULL)
    OsFree(pass);
}

/*!*****************************************************************************
 * \fn         int Esp32SmartConfig_getSmartconfigEnable(void)
 * \param[in]  void
 * \return     int
 *******************************************************************************/
int Esp32SmartConfig_getSmartconfigEnable(void)
{
  return this->smartConfigEnable;
}

/*!*****************************************************************************
 * \fn         void Esp32SmartConfig_groupSetBits()
 * \param[in]  none
 * \return     void
 *******************************************************************************/
void Esp32SmartConfig_groupSetBits()
{
  if(this->smartConfigEnable)
    OsEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
}

/*!*****************************************************************************
 * \fn         void Esp32SmartConfig_groupClearBits()
 * \param[in]  none
 * \return     void
 *******************************************************************************/
void Esp32SmartConfig_groupClearBits()
{
  if(this->smartConfigEnable)
    OsEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
}

/*!*****************************************************************************
 * \fn         void Esp32SmartConfig_createTask()
 * \param[in]  none
 * \return     void
 *******************************************************************************/
void Esp32SmartConfig_createTask()
{
  if(this->smartConfigEnable)
    OsTaskCreate(Esp32SmartConfig_task, "Esp32SmartConfig_task", 4096, NULL, 3, NULL);
}

/*!*****************************************************************************
 * \fn         static void Esp32SmartConfig_callback(smartconfig_status_t status, void *pdata)
 * \param[in]  smartconfig_status_t
 * \param[in]  void *
 * \return     void
 *******************************************************************************/
static void Esp32SmartConfig_callback(smartconfig_status_t status, void * pdata)
{
  if(this->smartConfigEnable)
  {
    switch(status)
    {
      case SC_STATUS_WAIT:
        BarDebug_info("SC_STATUS_WAIT\n");
        break;

      case SC_STATUS_FIND_CHANNEL:
        BarDebug_info("SC_STATUS_FINDING_CHANNEL\n");
        break;

      case SC_STATUS_GETTING_SSID_PSWD:
        BarDebug_info("SC_STATUS_GETTING_SSID_PSWD\n");
        LedRGBHandling_ExecuteLedTaskFromISR(RED_LED_FAST_BLINKING);
        smartconfigTimer = OsTimerCreate("Timer", SMARTCONFIG_PERIOD, pdTRUE, 0, Esp32SmartConfig_Callback);

        if(smartconfigTimer == NULL)
        {
          BarDebug_err("Create timer");
        }

        if(OsTimerReset(smartconfigTimer, 0) != pdPASS)
        {
          BarDebug_err("Reset timer");
        }

        break;

      case SC_STATUS_LINK:
        BarDebug_info("SC_STATUS_LINK\n");
        wifi_config_t *wifi_config = pdata;
        BarDebug_info("SSID:%s\n", wifi_config->sta.ssid);
        BarDebug_info("PASSWORD:%s\n", wifi_config->sta.password);
        {
          int ssidLen = strlen((const char *)wifi_config->sta.ssid);
          int passLen = strlen((const char *)wifi_config->sta.password);
          this->ssid = OsMalloc(ssidLen + 1);
          this->pass = OsMalloc(passLen + 1);
          memset(this->ssid, '\0', (ssidLen + 1));
          memset(this->pass, '\0', (passLen + 1));
          memcpy(this->ssid, wifi_config->sta.ssid, ssidLen);
          memcpy(this->pass, wifi_config->sta.password, passLen);
        }

        if(OsTimerStop(smartconfigTimer, 0) != pdPASS)
        {
          BarDebug_err("Stop timer");
        }
        // save if you have error: "smartconfig: send failed, errno 118"
        Wifi_saveSSIDAndPass(this->ssid, this->pass);

        ESP_ERROR_CHECK(esp_wifi_disconnect());
        ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, wifi_config));
        ESP_ERROR_CHECK(esp_wifi_connect());
        break;

      case SC_STATUS_LINK_OVER:
        BarDebug_info("SC_STATUS_LINK_OVER\n");

        if(pdata != NULL)
        {
          uint8_t phone_ip[4] =
          { 0};
          memcpy(phone_ip, (uint8_t*) pdata, 4);
          BarDebug_info("Phone ip: %d.%d.%d.%d\n", phone_ip[0], phone_ip[1], phone_ip[2], phone_ip[3]);
        }

        OsEventGroupSetBits(wifi_event_group, ESPTOUCH_DONE_BIT);
        break;

      default:
        break;
    }
  }
}

/*!*****************************************************************************
 * \fn         static void Esp32SmartConfig_Callback( OsTimerHandle_t smartconfigTimer )
 * \param[in]  none
 * \return     void
 *******************************************************************************/
static void Esp32SmartConfig_Callback(OsTimerHandle_t smartconfigTimer)
{
  System_cpuReset();
}

/*!*****************************************************************************
 * \fn         void Esp32SmartConfig_factoryReset()
 * \param[in]  none
 * \return     void
 *******************************************************************************/
void Esp32SmartConfig_factoryReset()
{
  Wifi_removeSsidAndPass();
}

/*!*****************************************************************************
 * \fn         void Esp32SmartConfig_task(void * parm)
 * \param[in]  none *
 * \return     void
 *******************************************************************************/
void Esp32SmartConfig_task(void * parm)
{
  if(this->smartConfigEnable)
  {
    EventBits_t uxBits;
    ESP_ERROR_CHECK(esp_smartconfig_set_type(SC_TYPE_ESPTOUCH));
    ESP_ERROR_CHECK(esp_smartconfig_start(Esp32SmartConfig_callback));

    while(1)
    {
      uxBits = OsEventGroupWaitBits(wifi_event_group, CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY);

      if(uxBits & CONNECTED_BIT)
      {
        BarDebug_info("WiFi Connected to ap\n");
      }

      if(uxBits & ESPTOUCH_DONE_BIT)
      {
        BarDebug_info("smartconfig over\n");

        if((this->pass != NULL) && (this->ssid != NULL))
        {
          if(OsTimerStop(smartconfigTimer, 0) != pdPASS)
          {
            BarDebug_err("Stop timer");
          }

          Wifi_saveSSIDAndPass(this->ssid, this->pass);
          BarDebug_info("-- %s, %s\n", this->ssid, this->pass);
          OsFree(this->ssid);
          OsFree(this->pass);
        }

        esp_smartconfig_stop();
        OsTaskDelete(NULL);
      }
    }
  }
}

#endif //SMART_CONFIG
#endif // ESP32
