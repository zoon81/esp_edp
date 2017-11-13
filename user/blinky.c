#include "user_config.h"


void ICACHE_FLASH_ATTR user_init()
{
  // init gpio subsytem
  gpio_init();
  spi_init(HSPI);
  edp_init();
  edp_clearFrameMemory(0xFF);   // bit set = white, bit reset = black
  edp_displayFrame();
  edp_clearFrameMemory(0xFF);   // bit set = white, bit reset = black
  edp_displayFrame();

}
