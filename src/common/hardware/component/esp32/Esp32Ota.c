//License: Apache (https://github.com/classycodeoss/esp32-ota)

#ifdef ESP32
#include "esp_ota_ops.h"
#include "Esp32Ota.h"
#include "Debug.h"

static const esp_partition_t *sOtaPartition;

static uint32_t sFlashCurrentAddress;
static esp_ota_handle_t sOtaHandle;

static const esp_partition_t *Esp32Ota_findNextBootPartition();

#define PART_LEN 20
typedef struct sEsp32Ota
{
  char currentPartition[PART_LEN];
  unsigned int addr;
} sEsp32Ota_t;

static sEsp32Ota_t sEsp32Ota;
#define this (&sEsp32Ota)

/*!*****************************************************************************
 * \fn         void Esp32Ota_init()
 * \param[in]  none
 * \return     none
 *******************************************************************************/
void Esp32Ota_init()
{
  const esp_partition_t * currentBootPartition = esp_ota_get_boot_partition();
  memset(this->currentPartition, '\0', PART_LEN);
  memcpy(this->currentPartition, currentBootPartition->label, strlen(currentBootPartition->label));
  this->addr = currentBootPartition->address;
  BarDebug_info("Part: %s, addr:%08x \n", Esp32Ota_getCurrentPartition(), Esp32Ota_getCurrentBootAddr());
}

/*!*****************************************************************************
 * \fn         char * Esp32Ota_getCurrentPartition()
 * \param[in]  none
 * \return     char *
 *******************************************************************************/
char * Esp32Ota_getCurrentPartition()
{
  return this->currentPartition;
}

/*!*****************************************************************************
 * \fn         unsigned int Esp32Ota_getCurrentAddr()
 * \param[in]  none
 * \return     unsigned int
 *******************************************************************************/
unsigned int Esp32Ota_getCurrentBootAddr()
{
  return this->addr;
}
/*!*****************************************************************************
 * \fn         int Esp32Ota_inProgress()
 * \param[in]  none
 * \return     int
 *******************************************************************************/
int Esp32Ota_inProgress()
{
  return sOtaPartition ? 1 : 0;
}

/*!*****************************************************************************
 * \fn         TOtaResult Esp32Ota_begin()
 * \param[in]  none
 * \return     TOtaResult
 *******************************************************************************/
TOtaResult Esp32Ota_begin()
{
  sOtaPartition = Esp32Ota_findNextBootPartition();

  if(!sOtaPartition)
  {
    return OTA_ERR_PARTITION_NOT_FOUND;
  }

  sFlashCurrentAddress = sOtaPartition->address;
  BarDebug_info("Set start address for flash writes to 0x%08x\n", sFlashCurrentAddress);
  esp_err_t result = esp_ota_begin(sOtaPartition, HW_BLOCK_SIZE, &sOtaHandle);
  BarDebug_info("Result from esp_ota_begin: %d %d\n", result, sOtaHandle);

  if(result == ESP_OK)
  {
    return OTA_OK;
  }

  return OTA_ERR_BEGIN_FAILED;
}

/*!*****************************************************************************
 * \fn         TOtaResult Esp32Ota_end()
 * \param[in]  none
 * \return     TOtaResult
 *******************************************************************************/
TOtaResult Esp32Ota_end()
{
  if(!sOtaPartition)
  {
    return OTA_ERR_PARTITION_NOT_FOUND;
  }

  esp_err_t result = esp_ota_end(sOtaHandle);

  if(result != ESP_OK)
  {
    return OTA_ERR_END_FAILED;
  }

  result = esp_ota_set_boot_partition(sOtaPartition);

  if(result == ESP_OK)
  {
    BarDebug_info("Boot partition activated: %s\n", sOtaPartition->label);
    return OTA_OK;
  }

  BarDebug_err("Failed to activate boot partition %s, error %d\n", sOtaPartition->label, result);
  sOtaPartition = NULL;
  return OTA_ERR_PARTITION_NOT_ACTIVATED;
}

/*!*****************************************************************************
 * \fn         TOtaResult Esp32Ota_writeHexData(const char *hexData, int len)
 * \param[in]  int
 * \param[in]  const char *
 * \return     TOtaResult
 *******************************************************************************/
TOtaResult Esp32Ota_writeHexData(const char *hexData, int len)
{
  uint8_t buf[HW_BLOCK_SIZE];

  for(int i = 0; i < HW_BLOCK_SIZE; i++)
  {
    buf[i] = (i < len) ? hexData[i] : 0xff;
  }

  if(sFlashCurrentAddress % 0x1000 == 0)
  {
    int flashSectorToErase = sFlashCurrentAddress / 0x1000;
    BarDebug_info("Erasing flash sector %d\n", flashSectorToErase);
    spi_flash_erase_sector(flashSectorToErase);
  }

  BarDebug_info("Writing flash at 0x%08x...\n", sFlashCurrentAddress);
  esp_err_t result = esp_ota_write(sOtaHandle, buf, len);

  if(result != ESP_OK)
  {
    BarDebug_err("Failed to write flash at address 0x%08x, error %d\n", sFlashCurrentAddress, result);
    return OTA_ERR_WRITE_FAILED;
  }

  sFlashCurrentAddress += len;
  return OTA_OK;
}

/*!*****************************************************************************
 * \fn         static const esp_partition_t *Esp32Ota_findNextBootPartition()
 * \param[in]  none
 * \return     const esp_partition_t *
 *******************************************************************************/
static const esp_partition_t *Esp32Ota_findNextBootPartition()
{
  const esp_partition_t *currentBootPartition = esp_ota_get_boot_partition();
  const esp_partition_t *nextBootPartition = NULL;

  if(!strcmp("factory", currentBootPartition->label))
  {
    nextBootPartition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, "ota_0");
  }

  if(!strcmp("ota_0", currentBootPartition->label))
  {
    nextBootPartition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, "ota_1");
  }

  if(!strcmp("ota_1", currentBootPartition->label))
  {
    nextBootPartition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, "ota_0");
  }

  if(nextBootPartition)
  {
    BarDebug_info("Found next boot partition: %02x %02x 0x%08x %s",
                  nextBootPartition->type, nextBootPartition->subtype, nextBootPartition->address, nextBootPartition->label);
  }
  else
  {
    BarDebug_err("Failed to determine next boot partition from current boot partition: %s",
                 currentBootPartition ? currentBootPartition->label : "NULL");
  }

  return nextBootPartition;
}

/*!*****************************************************************************
 * \fn         void Esp32Ota_getNextBootPartition(char * part)
 * \param[in]  char *
 * \return     void
 *******************************************************************************/
void Esp32Ota_getNextBootPartition(char * part)
{
  const esp_partition_t *currentBootPartition = esp_ota_get_boot_partition();

  if(!strcmp("factory", currentBootPartition->label))
  {
    memcpy(part, "ota_0", strlen("ota_0"));
  }

  if(!strcmp("ota_0", currentBootPartition->label))
  {
    memcpy(part, "ota_1", strlen("ota_1"));
  }

  if(!strcmp("ota_1", currentBootPartition->label))
  {
    memcpy(part, "ota_0", strlen("ota_0"));
  }
}

/*!*****************************************************************************
 * \fn         const esp_partition_t * Esp32Ota_getPartitionStruct(int addrPart)
 * \param[in]  const esp_partition_t *
 * \return     void
 *******************************************************************************/
const esp_partition_t * Esp32Ota_getPartitionStruct(int addrPart)
{
  if(ADDR_PART_1 == addrPart)
  {
    return esp_ota_get_boot_partition();
  }

  if(ADDR_PART_2 == addrPart)
  {
    return esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, "ota_0");
  }

  if(ADDR_PART_3 == addrPart)
  {
    return esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, "ota_1");
  }

  return NULL;
}

/*!*****************************************************************************
 * \fn         int Esp32Ota_getAddrBootPartition(char * part)
 * \param[in]  none
 * \return     char *
 *******************************************************************************/
int Esp32Ota_getAddrBootPartition(char * part)
{
  if(!strcmp("factory", part))
  {
    return ADDR_PART_1;
  }

  if(!strcmp("ota_0", part))
  {
    return ADDR_PART_2;
  }

  if(!strcmp("ota_1", part))
  {
    return ADDR_PART_3;
  }

  return 0;
}

#endif // ESP32
