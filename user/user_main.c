#include "user_config.h"
#include "driver/fonts.h"

void ICACHE_FLASH_ATTR user_init()
{
  // init gpio subsytem
  gpio_init();
  spi_init(HSPI);
  edp_init();
  edp_clearFrameMemory(0xFF); // bit set = white, bit reset = black
  edp_displayFrame();
  edp_clearFrameMemory(0xFF); // bit set = white, bit reset = black
  edp_displayFrame();

  draw_string(100,100, "HELLO", Font8_Table);
  edp_displayFrame();
}
