#include "user_config.h"

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

  //draw_string(0,10, "HELLO FONT16", &font16);
  //draw_string(0,27, "HELLO FONT8", &font8);
  draw_icon(0, 37, &weather_icons, WEATHER_ICON_POS_MOSTLYCLOUDY);

  edp_displayFrame();
}
