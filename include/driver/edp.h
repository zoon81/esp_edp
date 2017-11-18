#ifndef INC_EDP_H
#define INC_EDP_H
#include "user_config.h"

#define EDP_RST_PIN     BIT4    // D2
#define EDP_BUSY_PIN    BIT5    // D1
#define EDP_DATA_FLAG   0x100

// Display resolution
#define EDP_WIDTH       200
#define EDP_HEIGHT      200

// EPD1IN54 commands
#define DRIVER_OUTPUT_CONTROL                       0x01
#define BOOSTER_SOFT_START_CONTROL                  0x0C
#define GATE_SCAN_START_POSITION                    0x0F
#define DEEP_SLEEP_MODE                             0x10
#define DATA_ENTRY_MODE_SETTING                     0x11
#define SW_RESET                                    0x12
#define TEMPERATURE_SENSOR_CONTROL                  0x1A
#define MASTER_ACTIVATION                           0x20
#define DISPLAY_UPDATE_CONTROL_1                    0x21
#define DISPLAY_UPDATE_CONTROL_2                    0x22
#define WRITE_RAM                                   0x24
#define WRITE_VCOM_REGISTER                         0x2C
#define WRITE_LUT_REGISTER                          0x32
#define SET_DUMMY_LINE_PERIOD                       0x3A
#define SET_GATE_TIME                               0x3B
#define BORDER_WAVEFORM_CONTROL                     0x3C
#define SET_RAM_X_ADDRESS_START_END_POSITION        0x44
#define SET_RAM_Y_ADDRESS_START_END_POSITION        0x45
#define SET_RAM_X_ADDRESS_COUNTER                   0x4E
#define SET_RAM_Y_ADDRESS_COUNTER                   0x4F
#define TERMINATE_FRAME_READ_WRITE                  0xFF

#define EDP_DATA(data) ((data) | EDP_DATA_FLAG)
#define EDP_DATA_PART1(data) (EDP_DATA( (uint8_t ) ((data) & 0xff) ))
#define EDP_DATA_PART2(data) (EDP_DATA( (uint8_t ) (((data) >> 8) & 0xff) ))
#define EDP_DATA_PART3(data) (EDP_DATA( (uint8_t ) (((data) >> 16) & 0xff) ))
#define EDP_DATA_PART4(data) (EDP_DATA( (uint8_t ) (((data) >> 24) & 0xff) ))
#define EDP_DATA_BUILD18_LOWER(data) ( ((EDP_DATA_PART2((data))) << 9) | EDP_DATA_PART1((data)) )
#define EDP_DATA_BUILD18_UPPER(data) ( ((EDP_DATA_PART4((data))) << 9) | EDP_DATA_PART3((data)) )

void edp_init();
void edp_gpio_init();
void edp_reset();
void edp_sendCommand(uint8_t command);
void edp_sendData(uint8_t data);
void edp_clearFrameMemory(unsigned char color);
void edp_displayFrame(void);
void edp_setMemoryArea(int x_start, int y_start, int x_end, int y_end);
void edp_setMemoryPointer(int x, int y);
void edp_setFrameMemory(uint8_t x, uint8_t y, uint8_t x_end, uint8_t y_end, const uint8_t *image_data, uint8_t isInverted);
void edp_setFrameMemory32(uint8_t x, uint8_t y, uint8_t x_end, uint8_t y_end, const uint32_t *image_data, uint8_t isInverted);
void edp_waitUntilIdle();


#endif