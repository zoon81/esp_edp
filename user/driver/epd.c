#include "driver/edp.h"

LOCAL const unsigned char lut_full_update[] =
    {
        0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22,
        0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88,
        0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51,
        0x35, 0x51, 0x51, 0x19, 0x01, 0x00};

LOCAL const unsigned char lut_partial_update[] =
    {
        0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void edp_init()
{
    edp_gpio_init();
    edp_reset();
    spi_transaction(HSPI, 0, 0, 9, DRIVER_OUTPUT_CONTROL, 27, ( (EDP_DATA((EDP_HEIGHT - 1) & 0xFF) << 18) | (EDP_DATA(((EDP_HEIGHT - 1) >> 8) & 0xFF) << 9) | EDP_DATA(0x00)), 0, 0);
    spi_transaction(HSPI, 0, 0, 9, BOOSTER_SOFT_START_CONTROL, 27, ( (EDP_DATA(0xD7) << 18) | (EDP_DATA(0xD6) << 9) | EDP_DATA(0x9D) ), 0, 0);
    spi_transaction(HSPI, 0, 0, 9, WRITE_VCOM_REGISTER, 9, EDP_DATA(0xA8), 0, 0);
    spi_transaction(HSPI, 0, 0, 9, SET_DUMMY_LINE_PERIOD, 9, EDP_DATA(0x1A), 0, 0);
    spi_transaction(HSPI, 0, 0, 9, SET_GATE_TIME, 9, EDP_DATA(0x08), 0, 0);
    spi_transaction(HSPI, 0, 0, 9, DATA_ENTRY_MODE_SETTING, 9, EDP_DATA(0x03), 0, 0);

    hspi_autocs_mode(HSPI_CSMODE_MANUAL);
    HSPI_CS_LOW;
    spi_transaction(HSPI,0,0,0,0, 9, WRITE_LUT_REGISTER, 0, 0 );
    uint8_t i;
    for(i = 0; i < 30 ; i++){
        spi_transaction(HSPI,0,0,0,0, 9, EDP_DATA( lut_full_update[i]), 0, 0);
    }
    HSPI_CS_HI;
    hspi_autocs_mode(HSPI_CSMODE_AUTO);
    
}
void edp_gpio_init()
{
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4); // RST
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5); // BUSY
    gpio_output_set(0, 0, EDP_RST_PIN, EDP_BUSY_PIN);    // Set RST as output and BUSY as input
}
void edp_reset()
{
    gpio_output_set(0, EDP_RST_PIN, 0, 0);
    os_delay_us(50000);
    system_soft_wdt_feed();
    os_delay_us(50000);
    system_soft_wdt_feed();
    gpio_output_set(EDP_RST_PIN, 0, 0, 0);
    os_delay_us(50000);
    system_soft_wdt_feed();
    os_delay_us(50000);
    system_soft_wdt_feed();
}

void edp_clearFrameMemory(unsigned char color) {
    edp_setMemoryArea(0, 0, EDP_WIDTH - 1, EDP_HEIGHT - 1);
    edp_setMemoryPointer(0, 0);
    hspi_autocs_mode(HSPI_CSMODE_MANUAL);
    HSPI_CS_LOW;
    spi_transaction(HSPI,0,0,0,0, 9, WRITE_RAM, 0, 0);
    uint16_t i;
    for (i = 0; i < EDP_WIDTH / 8 * EDP_HEIGHT; i++) {
        spi_transaction(HSPI, 0, 0, 0, 0, 9,EDP_DATA(color), 0, 0);
    }
    HSPI_CS_HI;
    hspi_autocs_mode(HSPI_CSMODE_AUTO);
}

void edp_setFrameMemory(uint8_t x, uint8_t y, uint8_t x_end, uint8_t y_end, const uint8_t *image_data, uint8_t isInverted) {
    edp_setMemoryArea(x, y, x_end, y_end);
    edp_setMemoryPointer(x, y);

    hspi_autocs_mode(HSPI_CSMODE_MANUAL);
    HSPI_CS_LOW;
    spi_transaction(HSPI,0,0,0,0, 9, WRITE_RAM, 0, 0);
    uint8_t i,j;
    for(j = y ; j < y_end ; j++){
        for (i = x; i < (x_end + 1) ; i++) {
            if(isInverted)
                spi_transaction(HSPI, 0, 0, 0, 0, 9,EDP_DATA(~(*image_data++)), 0, 0);
            else
                spi_transaction(HSPI, 0, 0, 0, 0, 9,EDP_DATA(*image_data++), 0, 0);
        }
    }
    HSPI_CS_HI;
    hspi_autocs_mode(HSPI_CSMODE_AUTO);
}


void edp_displayFrame(void) {
    spi_transaction(HSPI,0,0,9, DISPLAY_UPDATE_CONTROL_2, 9, EDP_DATA(0xC4),0,0);
    spi_transaction(HSPI,0,0,0,0, 9, MASTER_ACTIVATION, 0, 0);
    spi_transaction(HSPI,0,0,0,0, 9, TERMINATE_FRAME_READ_WRITE,0,0);
    edp_waitUntilIdle();
}
//waiting until the display processing data (Busy pin set HI)
void edp_waitUntilIdle(){
    while(GPIO_INPUT_GET(5)){
        os_delay_us(1000);
        system_soft_wdt_feed();
    }
}

void edp_setMemoryArea(int x_start, int y_start, int x_end, int y_end) {
    spi_transaction(HSPI, 0,0, 9, SET_RAM_X_ADDRESS_START_END_POSITION, 18, ( (EDP_DATA((x_start >> 3) & 0xFF) << 9) |  EDP_DATA((x_end >> 3) & 0xFF)), 0, 0);
    spi_transaction(HSPI, 9, SET_RAM_Y_ADDRESS_START_END_POSITION, 18, ((EDP_DATA(y_start & 0xFF) << 9) | EDP_DATA((y_start >> 8) & 0xFF)), 18, ((EDP_DATA(y_end & 0xFF) << 9) | EDP_DATA((y_end >> 8) & 0xFF)),0,0);
}

void edp_setMemoryPointer(int x, int y) {
    spi_transaction(HSPI, 0,0, 9, SET_RAM_X_ADDRESS_COUNTER, 9, EDP_DATA((x >> 3) & 0xFF), 0, 0);
    spi_transaction(HSPI, 0,0, 9, SET_RAM_Y_ADDRESS_COUNTER, 18, ((EDP_DATA(y & 0xFF) << 9) | EDP_DATA((y >> 8) & 0xFF)), 0, 0);
    edp_waitUntilIdle();
}   