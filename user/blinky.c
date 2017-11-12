#include "user_config.h"


void ICACHE_FLASH_ATTR user_init()
{
  // init gpio subsytem
  gpio_init();
  spi_init(HSPI);
  edp_init();
}
