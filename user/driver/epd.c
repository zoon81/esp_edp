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
    /* edp_sendCommand(DRIVER_OUTPUT_CONTROL);
    edp_sendData((EPD_HEIGHT - 1) & 0xFF);
    edp_sendData(((EPD_HEIGHT - 1) >> 8) & 0xFF);
    edp_sendData(0x00);                     // GD = 0; SM = 0; TB = 0; */
    spi_transaction(HSPI, 0, 0, 9, DRIVER_OUTPUT_CONTROL, 27, ( (EDP_DATA((EDP_HEIGHT - 1) & 0xFF) << 18) | (EDP_DATA(((EDP_HEIGHT - 1) >> 8) & 0xFF) << 9) | EDP_DATA(0x00)), 0, 0);
    /* edp_sendCommand(BOOSTER_SOFT_START_CONTROL);
    edp_sendData(0xD7);
    edp_sendData(0xD6);
    edp_sendData(0x9D); */
    spi_transaction(HSPI, 0, 0, 9, BOOSTER_SOFT_START_CONTROL, 27, ( (EDP_DATA(0xD7) << 18) | (EDP_DATA(0xD6) << 9) | EDP_DATA(0x9D) ), 0, 0);
    /* edp_sendCommand(WRITE_VCOM_REGISTER);
    edp_sendData(0xA8);                     // VCOM 7C */
    spi_transaction(HSPI, 0, 0, 9, WRITE_VCOM_REGISTER, 9, EDP_DATA(0xA8), 0, 0);
    /* edp_sendCommand(SET_DUMMY_LINE_PERIOD);
    edp_sendData(0x1A);                     // 4 dummy lines per gate */
    spi_transaction(HSPI, 0, 0, 9, SET_DUMMY_LINE_PERIOD, 9, EDP_DATA(0x1A), 0, 0);
    /* edp_sendCommand(SET_GATE_TIME);
    edp_sendData(0x08);    */ // 2us per line
    spi_transaction(HSPI, 0, 0, 9, SET_GATE_TIME, 9, EDP_DATA(0x08), 0, 0);
    /* edp_sendCommand(DATA_ENTRY_MODE_SETTING);
    edp_sendData(0x03);  */ // X increment; Y increment
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
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2); // DC
    gpio_output_set(0, 0, EDP_DC_PIN | EDP_RST_PIN, 0);
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
    /* send the color data */
    uint16_t i;
    for (i = 0; i < EDP_WIDTH / 8 * EDP_HEIGHT; i++) {
        spi_transaction(HSPI, 0, 0, 0, 0, 9,EDP_DATA(color), 0, 0);
    }
    HSPI_CS_HI;
    hspi_autocs_mode(HSPI_CSMODE_AUTO);
}


void edp_displayFrame(void) {
    /* SendCommand(DISPLAY_UPDATE_CONTROL_2);
    SendData(0xC4); */
    spi_transaction(HSPI,0,0,9, DISPLAY_UPDATE_CONTROL_2, 9, EDP_DATA(0xC4),0,0);
    //SendCommand(MASTER_ACTIVATION);
    spi_transaction(HSPI,0,0,0,0, 9, MASTER_ACTIVATION, 0, 0);
    //SendCommand(TERMINATE_FRAME_READ_WRITE);
    spi_transaction(HSPI,0,0,0,0, 9, TERMINATE_FRAME_READ_WRITE,0,0);
    //WaitUntilIdle();
}

void edp_setMemoryArea(int x_start, int y_start, int x_end, int y_end) {
    /* SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);
    // x point must be the multiple of 8 or the last 3 bits will be ignored
    SendData((x_start >> 3) & 0xFF);
    SendData((x_end >> 3) & 0xFF); */
    spi_transaction(HSPI, 0,0, 9, SET_RAM_X_ADDRESS_START_END_POSITION, 18, ( (EDP_DATA((x_start >> 3) & 0xFF) << 9) |  EDP_DATA((x_end >> 3) & 0xFF)), 0, 0);
    /* SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);
    SendData(y_start & 0xFF);
    SendData((y_start >> 8) & 0xFF);
    SendData(y_end & 0xFF);
    SendData((y_end >> 8) & 0xFF); */
    spi_transaction(HSPI, 9, SET_RAM_Y_ADDRESS_START_END_POSITION, 18, ((EDP_DATA(y_start & 0xFF) << 9) | EDP_DATA((y_start >> 8) & 0xFF)), 18, ((EDP_DATA(y_end & 0xFF) << 9) | EDP_DATA((y_end >> 8) & 0xFF)),0,0);
}

void edp_setMemoryPointer(int x, int y) {
    spi_transaction(HSPI, 0,0, 9, SET_RAM_X_ADDRESS_COUNTER, 9, EDP_DATA((x >> 3) & 0xFF), 0, 0);
   // SendCommand(SET_RAM_X_ADDRESS_COUNTER);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
   // SendData((x >> 3) & 0xFF);
    spi_transaction(HSPI, 0,0, 9, SET_RAM_Y_ADDRESS_COUNTER, 18, ((EDP_DATA(y & 0xFF) << 9) | EDP_DATA((y >> 8) & 0xFF)), 0, 0);
    //SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    //SendData(y & 0xFF);
    //SendData((y >> 8) & 0xFF);
    //WaitUntilIdle();
}   