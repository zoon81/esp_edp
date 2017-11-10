#include "driver/edp.h"

void edp_init(){
    edp_gpio_init();
    edp_reset();

    edp_sendCommand(DRIVER_OUTPUT_CONTROL);
    edp_sendData((EPD_HEIGHT - 1) & 0xFF);
    edp_sendData(((EPD_HEIGHT - 1) >> 8) & 0xFF);
    edp_sendData(0x00);                     // GD = 0; SM = 0; TB = 0;
    edp_sendCommand(BOOSTER_SOFT_START_CONTROL);
    edp_sendData(0xD7);
    edp_sendData(0xD6);
    edp_sendData(0x9D);
    edp_sendCommand(WRITE_VCOM_REGISTER);
    edp_sendData(0xA8);                     // VCOM 7C
    edp_sendCommand(SET_DUMMY_LINE_PERIOD);
    edp_sendData(0x1A);                     // 4 dummy lines per gate
    edp_sendCommand(SET_GATE_TIME);
    edp_sendData(0x08);                     // 2us per line
    edp_sendCommand(DATA_ENTRY_MODE_SETTING);
    edp_sendData(0x03);                     // X increment; Y increment


}
void edp_gpio_init(){
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);    // RST
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);    // BUSY
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);    // DC
    gpio_output_set(0, 0, EDP_DC_PIN|EDP_RST_PIN, EDP_BUSY_PIN);
}
void edp_reset(){
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

void edp_sendCommand(uint8_t command){
    GPIO_OUTPUT_SET(EDP_DC_PIN, EDP_COMMAND);
    spi_tx8(HSPI, command);
}

void edp_sendData(uint8_t data){
    GPIO_OUTPUT_SET(EDP_DC_PIN, EDP_DATA);
    spi_tx8(HSPI, data);
}