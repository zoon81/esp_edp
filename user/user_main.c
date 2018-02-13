#include "user_config.h"

struct Entry
{
  uint32_t id;
  uint32_t value;
  const uint32_t *table[];
};
const uint32_t RODATA_ATTR demotable[] = {0x00, 0x98765432};
void scanCB(void *arg, STATUS status);
void initDone();
void eventHandler(System_Event_t *event);
void wifi_is_ready();
void ICACHE_FLASH_ATTR owm_http_callback(char *response_body, int http_status, char *response_headers, int body_size);

void ICACHE_FLASH_ATTR user_init()
{
  wifi_station_set_config_current(FALSE);

  system_init_done_cb(initDone);
  wifi_set_event_handler_cb(eventHandler);
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
  //draw_icon(0, 0, &clear_sky.icon_96);
  //draw_icon(0, 100, &clear_sky.icon_64);

  //draw_icon(100, 0, &weather_icons, WEATHER_ICON_POS_SUNNY);
  //draw_icon(0, 100, &weather_icons, WEATHER_ICON_POS_RAINY);
  //draw_number(100, 100, 23, &numbers24);
  //draw_string(100, 134, "BP HU", &font16);
  //edp_displayFrame();
}

void initDone()
{
  wifi_set_opmode_current(STATION_MODE);
  struct station_config stationConfig;
  strncpy(stationConfig.ssid, "THECORE_2.4", 32);
  strncpy(stationConfig.password, "sp33dlink", 64);
  wifi_station_set_config(&stationConfig);
  wifi_station_connect();
}

void eventHandler(System_Event_t *event)
{
  switch (event->event)
  {
  case EVENT_STAMODE_CONNECTED:
    os_printf("Event: EVENT_STAMODE_CONNECTED\n");
    break;
  case EVENT_STAMODE_DISCONNECTED:
    os_printf("Event: EVENT_STAMODE_DISCONNECTED\n");
    break;
  case EVENT_STAMODE_AUTHMODE_CHANGE:
    os_printf("Event: EVENT_STAMODE_AUTHMODE_CHANGE\n");
    break;
  case EVENT_STAMODE_GOT_IP:
    os_printf("Event: EVENT_STAMODE_GOT_IP\n");
    wifi_is_ready();
    break;
  case EVENT_SOFTAPMODE_STACONNECTED:
    os_printf("Event: EVENT_SOFTAPMODE_STACONNECTED\n");
    break;
  case EVENT_SOFTAPMODE_STADISCONNECTED:
    os_printf("Event: EVENT_SOFTAPMODE_STADISCONNECTED\n");
    break;
  default:
    os_printf("Unexpected event: %d\n", event->event);
    break;
  }
}

void wifi_is_ready()
{
  struct ip_info ipconfig;
  wifi_get_ip_info(STATION_IF, &ipconfig);
  os_printf("IP: %d.%d.%d.%d\n", IP2STR(&(ipconfig.ip.addr)));
  owm_getWeatherinfo(0);
}

static void example_read_file_posix()
{
  const int buf_size = 0xFF;
  uint8_t buf[buf_size];

  int fd = open("test.txt", O_RDONLY);
  if (fd < 0)
  {
    os_printf("Error opening file\n");
    return;
  }

  int read_bytes = read(fd, buf, buf_size);
  os_printf("Read %d bytes\n", read_bytes);

  buf[read_bytes] = '\0'; // zero terminate string
  os_printf("Data: %s\n", buf);

  close(fd);
}

static void example_read_file_spiffs()
{
  const int buf_size = 0xFF;
  uint8_t buf[buf_size];

  spiffs_file fd = SPIFFS_open(&fs, "other.txt", SPIFFS_RDONLY, 0);
  if (fd < 0)
  {
    os_printf("Error opening file\n");
    return;
  }

  int read_bytes = SPIFFS_read(&fs, fd, buf, buf_size);
  os_printf("Read %d bytes\n", read_bytes);

  buf[read_bytes] = '\0'; // zero terminate string
  os_printf("Data: %s\n", buf);

  SPIFFS_close(&fs, fd);
}

static void example_write_file()
{
  uint8_t buf[] = "Example data, written by ESP8266";

  int fd = open("other.txt", O_WRONLY | O_CREAT, 0);
  if (fd < 0)
  {
    os_printf("Error opening file\n");
    return;
  }

  int written = write(fd, buf, sizeof(buf));
  os_printf("Written %d bytes\n", written);

  close(fd);
}

static void example_fs_info()
{
  uint32_t total, used;
  SPIFFS_info(&fs, &total, &used);
  os_printf("Total: %d bytes, used: %d bytes", total, used);
}

void test_task()
{
#if SPIFFS_SINGLETON == 1
  esp_spiffs_init();
#else
  // for run-time configuration when SPIFFS_SINGLETON = 0
  esp_spiffs_init(0x200000, 0x10000);
#endif

  if (esp_spiffs_mount() != SPIFFS_OK)
  {
    os_printf("Error mount SPIFFS\n");
  }

  example_write_file();

  example_read_file_posix();

  example_read_file_spiffs();

  example_fs_info();

  printf("\n\n");
}
