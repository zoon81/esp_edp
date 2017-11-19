#include "user_config.h"

struct Entry
{
  uint32_t id;
  uint32_t value;
  const uint32_t *table[];
};
const uint32_t RODATA_ATTR demotable[] = {0x00,0x98765432};

struct Entry myArray = {
    1, 200, {demotable}
  };

struct Entry *myArray_p = &myArray;

void ICACHE_FLASH_ATTR user_init()
{
  uint32_t tmp = 0x98765432;
  /* os_printf("PARTS:%x %x %x %x", EDP_DATA_PART1(tmp), EDP_DATA_PART2(tmp), EDP_DATA_PART3(tmp), EDP_DATA_PART4(tmp));
  os_printf("UPPER: %x LOWER: %x", EDP_DATA_BUILD18_UPPER(tmp), EDP_DATA_BUILD18_LOWER(tmp));
  os_printf("%d%d", myArray.id, myArray.value);
  os_printf("%d", ((*myArray_p).table[0])[1]); */
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
  draw_icon(0, 0, &weather_icons, WEATHER_ICON_POS_MOSTLYCLOUDY);
  draw_icon(100, 0, &weather_icons, WEATHER_ICON_POS_SUNNY);
  draw_icon(0, 100, &weather_icons, WEATHER_ICON_POS_RAINY);
  draw_icon(100, 100, &weather_icons, WEATHER_ICON_POS_SNOWY);

  edp_displayFrame();
}
