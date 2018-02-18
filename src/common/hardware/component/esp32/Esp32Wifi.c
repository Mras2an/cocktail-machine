#ifdef ESP32
#include "esp_wifi.h"
#include "FileSystem.h"
#include "Bsp.h"
#include "Esp32Wifi.h"

#define SSID_WIFI_FILE      "ssidWifi"
#define PASSWORD_WIFI_FILE  "passwordWifi"

static wifi_config_t Esp32Wifi_getSSIDAndPass(void);

/******************************************************************************
 * FunctionName : void Esp32Wifi_init()
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void Esp32Wifi_init()
{
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  wifi_config_t sta_config = Esp32Wifi_getSSIDAndPass();
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
  ESP_ERROR_CHECK(esp_wifi_set_country(WIFI_COUNTRY_EU));
  ESP_ERROR_CHECK(esp_wifi_start());
  BarDebug_info("Free Heap at %d = %d\n", __LINE__, OsGetFreeHeapSize());
}

/******************************************************************************
 * FunctionName : void Esp32Wifi_saveSSIDAndPass(const char * ssid, const char * password)
 * Parameters   : const char *
 * Parameters   : const char *
 * Returns      : void
 *******************************************************************************/
void Esp32Wifi_saveSSIDAndPass(const char * ssid, const char * password)
{
  if(Fs_write(SSID_WIFI_FILE, ssid, SSID_WIFI_FILE) != ESP_OK)
  {
    BarDebug_err("Error to save SSID\n");
  }

  if(Fs_write(PASSWORD_WIFI_FILE, password, PASSWORD_WIFI_FILE) != ESP_OK)
  {
    BarDebug_err("Error to save password\n");
  }
}

/******************************************************************************
 * FunctionName : static void Esp32Wifi_saveSSIDAndPass(const char * ssid, const char * password)
 * Parameters   : const char *
 * Parameters   : const char *
 * Returns      : void
 *******************************************************************************/
static wifi_config_t Esp32Wifi_getSSIDAndPass(void)
{
  wifi_config_t wifi_config;
  char * ssid = (char *)Esp32Wifi_getSSID();
  char * password = (char *)Esp32Wifi_getPassword();

  if((password != NULL) && (ssid != NULL))
  {
    wifi_config.sta.bssid_set = false;
    memcpy(wifi_config.sta.ssid, ssid, strlen(ssid) + 1);
    memcpy(wifi_config.sta.password, password, strlen(password) + 1);
    BarDebug_info("-- %s, %s\n", wifi_config.sta.ssid, wifi_config.sta.password);
  }

  if(ssid != NULL)
    OsFree(ssid);

  if(password != NULL)
    OsFree(password);

  return wifi_config;
}

/******************************************************************************
 * FunctionName : static const char * Esp32Wifi_getSSID(void)
 * Parameters   : void
 * Returns      : const char *
 *******************************************************************************/
const char * Esp32Wifi_getSSID(void)
{
  return Fs_read(SSID_WIFI_FILE, SSID_WIFI_FILE);
}

/******************************************************************************
 * FunctionName : static const char * Esp32Wifi_getPassword(void)
 * Parameters   : void
 * Returns      : const char *
 *******************************************************************************/
const char * Esp32Wifi_getPassword(void)
{
  return Fs_read(PASSWORD_WIFI_FILE, PASSWORD_WIFI_FILE);
}


/******************************************************************************
 * FunctionName : void Esp32Wifi_removeSsidAndPass(void)
 * Parameters   : void
 * Returns      : void
 *******************************************************************************/
void Esp32Wifi_removeSsidAndPass(void)
{
  Fs_delete(SSID_WIFI_FILE, SSID_WIFI_FILE);
  Fs_delete(PASSWORD_WIFI_FILE, PASSWORD_WIFI_FILE);
}
#endif // ESP32
