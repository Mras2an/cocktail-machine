#ifdef ESP32
#include "esp_wifi.h"
#include "esp_event_loop.h"

#include "rom/rtc.h"
#include "LedRGBHandling.h"
#include "Board.h"
#include "System.h"
#include "esp_flash_encrypt.h"

#include "Cocktail.h"
#include "QueueCocktail.h"
#include "FileSystem.h"
#include "Ota.h"

#ifdef SMART_CONFIG
  #include "SmartConfig.h"
#endif

OsTimerHandle_t esp32Timer;
#define  WIFI_PERIOD      (20000 / portTICK_PERIOD_MS)
#define  MD5_LEN_TO_READ  1040384

static esp_err_t Esp32_eventHandler(void *ctx, system_event_t *event);
static void Esp32_Callback(OsTimerHandle_t esp32Timer);

static void Esp32_Callback(OsTimerHandle_t esp32Timer)
{
  System_cpuReset();
}

typedef struct sEsp32
{
  char * ip;
  char * mask;
  char * gw;
  char * mac;
  char * uniqueId;
  char * md5Read;
  char * md5Cal;
} sEsp32_t;

static sEsp32_t sEsp32;
#define this (&sEsp32)

/******************************************************************************
 * FunctionName : static esp_err_t Esp32_eventHandler(void *ctx, system_event_t *event)
 * Parameters   : void *, system_event_t *
 * Returns      : esp_err_t
 *******************************************************************************/
static esp_err_t Esp32_eventHandler(void *ctx, system_event_t *event)
{
  switch(event->event_id)
  {
    case SYSTEM_EVENT_STA_START:
    {
      BarDebug_info("started\n");
      #ifdef SMART_CONFIG
      SmartConfig_createTask();
      #endif
      esp_wifi_connect();

      if(!SmartConfig_isSmartconfigEnable())
      {
        esp32Timer = OsTimerCreate("Timer", WIFI_PERIOD, pdTRUE, 0, Esp32_Callback);

        if(esp32Timer == NULL)
        {
          BarDebug_err("Create timer\n");
        }

        if(OsTimerReset(esp32Timer, 0) != pdPASS)
        {
          BarDebug_err("Reset timer\n");
        }

        uint32_t bp1[1];
        Gpio_get(BUTTON_GPIO_MODE, bp1);

        if(bp1[0] == 0)
        {
          if(OsTimerStop(esp32Timer, 0) != pdPASS)
          {
            BarDebug_err("Stop timer\n");
          }
        }
      }
    }
    break;

    case SYSTEM_EVENT_STA_DISCONNECTED:
    {
      BarDebug_info("disconnected\n");
      #ifdef SMART_CONFIG
      SmartConfig_groupClearBits();
      #endif
    }
    break;

    case SYSTEM_EVENT_SCAN_DONE:
    {
      BarDebug_info("Scan done\n");
    }
    break;

    case SYSTEM_EVENT_STA_CONNECTED:
    {
      BarDebug_info("connected\n");
    }
    break;

    case SYSTEM_EVENT_STA_GOT_IP:
    {
      #ifdef SMART_CONFIG
      SmartConfig_groupSetBits();
      #endif
      BarDebug_info("dhcp => event_handler:SYSTEM_EVENT_STA_GOT_IP!\n");
      Esp32_saveIp((const char *)ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
      Esp32_saveMask((const char *)ip4addr_ntoa(&event->event_info.got_ip.ip_info.netmask));
      Esp32_saveGw((const char *)ip4addr_ntoa(&event->event_info.got_ip.ip_info.gw));
      Esp32_getMac();

      char * update = Fs_read("Update", "Update");
      if (update != NULL)
      {
          BarDebug_info("LED GREEN\n");
          LedRGBHandling_ExecuteLedTaskFromISR(GREEN_LED);
    	  Fs_delete("Update", "Update");
    	  OsFree(update);
    	  Ota_InitTask();
      }
      else
      {
          BarDebug_info("LED BLUE\n");
          LedRGBHandling_ExecuteLedTaskFromISR(BLUE_LED);
          Cocktail_init();
          QueueCocktail_init();
      }


      if(!SmartConfig_isSmartconfigEnable())
      {
    	  BarDebug_info("stop smartconfig timer\n");
        if(OsTimerStop(esp32Timer, 0) != pdPASS)
        {
          BarDebug_err("Stop timer\n");
        }
      }
    }
    break;

    case SYSTEM_EVENT_AP_STACONNECTED:
    {
      BarDebug_info("station:"MACSTR" join,AID=%d\n", MAC2STR(event->event_info.sta_connected.mac), event->event_info.sta_connected.aid);
}
break;

    case SYSTEM_EVENT_AP_STADISCONNECTED:
{
BarDebug_info("station:"MACSTR"leave,AID=%d\n", MAC2STR(event->event_info.sta_disconnected.mac), event->event_info.sta_disconnected.aid);
}
break;

    case SYSTEM_EVENT_STA_STOP:
{
BarDebug_info("stopped\n");
}
break;

    default:
{
BarDebug_info("Unhandled event (%d)\n", event->event_id);
}
break;
  }
  return ESP_OK;
}



/******************************************************************************
 * FunctionName : uint32_t  Esp32_getRandom(void)
 * Parameters   : void
 * Returns      : uint32_t
 *******************************************************************************/
uint32_t Esp32_getRandom(void)
{
  return esp_random();
}

/******************************************************************************
 * FunctionName : void Esp32_getRssi(void)
 * Parameters   : void
 * Returns      : int
 *******************************************************************************/
int Esp32_getRssi(void)
{
  wifi_ap_record_t wifidata;
  if (esp_wifi_sta_get_ap_info(&wifidata) == 0)
  {
    return wifidata.rssi;
  }
  return 0;
}

/******************************************************************************
 * FunctionName : char * Esp32_getMac(void)
 * Parameters   : void
 * Returns      : char *
 *******************************************************************************/
char * Esp32_getMac(void)
{
  uint8_t data[6];
  esp_wifi_get_mac(WIFI_IF_STA, data);
  sprintf(this->mac, "%02x:%02x:%02x:%02x:%02x:%02x", data[0], data[1], data[2], data[3], data[4], data[5]);

  return this->mac;
}

/******************************************************************************
 * FunctionName : char * Esp32_getIp(void)
 * Parameters   : void
 * Returns      : char *
 *******************************************************************************/
char * Esp32_getIp(void)
{
  return this->ip;
}

/******************************************************************************
 * FunctionName : void Esp32_saveIp(char * ip)
 * Parameters   : char *
 * Returns      : void
 *******************************************************************************/
void Esp32_saveIp(const char * ip)
{
  size_t ipLen = strlen(ip) + 1;
  this->ip = OsMalloc(ipLen);
  memset(this->ip, '\0', ipLen);
  memcpy(this->ip, ip, ipLen);
  BarDebug_info("%s\n", this->ip);
}

/******************************************************************************
 * FunctionName : char * Esp32_getGw(void)
 * Parameters   : void
 * Returns      : char *
 *******************************************************************************/
char * Esp32_getGw(void)
{
  return this->gw;
}

/******************************************************************************
 * FunctionName : void Esp32_saveGw(char * ip)
 * Parameters   : char *
 * Returns      : void
 *******************************************************************************/
void Esp32_saveGw(const char * gw)
{
  size_t gwLen = strlen(gw) + 1;
  this->gw = OsMalloc(gwLen);
  memset(this->gw, '\0', gwLen);
  memcpy(this->gw, gw, gwLen);
}

/******************************************************************************
 * FunctionName : char * Esp32_getMask(void)
 * Parameters   : void
 * Returns      : char *
 *******************************************************************************/
char * Esp32_getMask(void)
{
  return this->mask;
}

/******************************************************************************
 * FunctionName : void Esp32_saveMask(char * ip)
 * Parameters   : char *
 * Returns      : void
 *******************************************************************************/
void Esp32_saveMask(const char * mask)
{
  size_t maskLen = strlen(mask) + 1;
  this->mask = OsMalloc(maskLen);
  memset(this->mask, '\0', maskLen);
  memcpy(this->mask, mask, maskLen);
}

/******************************************************************************
 * FunctionName : void Esp32_init(void)
 * Parameters   : void
 * Returns      : void
 *******************************************************************************/
void Esp32_init(void)
{
  this->ip = NULL;
  this->gw = NULL;
  this->mask = NULL;
  this->mac = OsMalloc(sizeof(char) * 17);
  this->uniqueId = NULL;
  this->uniqueId = OsMalloc(sizeof(char) * 16);
  this->md5Read = NULL;
  this->md5Read = OsMalloc(sizeof(char) * 33);
  this->md5Cal = NULL;
  this->md5Cal = OsMalloc(sizeof(char) * 33);

  tcpip_adapter_init();

  ESP_ERROR_CHECK(tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA));
  tcpip_adapter_ip_info_t info = { 0,};
  IP4_ADDR(&info.ip, 192, 168, 1, 51);
  IP4_ADDR(&info.gw, 192, 168, 1, 1);
  IP4_ADDR(&info.netmask, 255, 255, 255, 0);

  ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &info));

  BarDebug_info("Free Heap at %d = %d\n", __LINE__, OsGetFreeHeapSize());
  ESP_ERROR_CHECK(esp_event_loop_init(Esp32_eventHandler, NULL));
}

/******************************************************************************
 * FunctionName : int Esp32_getCpu(void)
 * Parameters   : void
 * Returns      : int
 *******************************************************************************/
int Esp32_getCpu(void)
{
  return CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ;
}

/******************************************************************************
 * FunctionName : int Esp32_getRam(void)
 * Parameters   : void
 * Returns      : int
 *******************************************************************************/
int Esp32_getRam(void)
{
  return OsGetFreeHeapSize();
}

/******************************************************************************
 * FunctionName : void Esp32_cpuReset()
 * Parameters   : void
 * Returns      : void
 *******************************************************************************/
void Esp32_cpuReset()
{
  BarDebug_err("\n");
  esp_restart();
}

/******************************************************************************
 * FunctionName : int Esp32_cpuResetReason(int reason)
 * Parameters   : int
 * Returns      : int
 *******************************************************************************/
int Esp32_cpuResetReason(int reason)
{
  return rtc_get_reset_reason(reason);
}

/******************************************************************************
 * FunctionName : void Esp32_flashIsProtected(char * tmp)
 * Parameters   : char *
 * Returns      : void
 *******************************************************************************/
void Esp32_flashIsProtected(char * tmp)
{
  if (esp_flash_encryption_enabled())
  {
    memcpy(tmp, "true", 4);
  }
  else
  {
    memcpy(tmp, "false", 5);
  }
}
/******************************************************************************
 * FunctionName : void Esp32_delete(void)
 * Parameters   : void
 * Returns      : void
 *******************************************************************************/
void Esp32_delete(void)
{
  if(this->ip)
  {
    OsFree(this->ip);
  }
  if(this->gw)
  {
    OsFree(this->gw);
  }
  if(this->mask)
  {
    OsFree(this->mask);
  }
  if(this->mac)
  {
    OsFree(this->mac);
  }
  if(this->uniqueId)
  {
    OsFree(this->uniqueId);
  }
  if(this->md5Read)
  {
    OsFree(this->md5Read);
  }
  if(this->md5Cal)
  {
    OsFree(this->md5Cal);
  }
}
#endif // ESP32
