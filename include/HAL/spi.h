/*
* The MIT License (MIT)
* 
* Copyright (c) 2015 David Ogilvy (MetalPhreak)
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef SPI_APP_H
#define SPI_APP_H
#include "user_config.h"
#include "HAL/spi_registers.h"

//Define SPI hardware modules
#define SPI 0
#define HSPI 1

#define SPI_CLK_USE_DIV 0
#define SPI_CLK_80MHZ_NODIV 1

#define SPI_BYTE_ORDER_HIGH_TO_LOW 1
#define SPI_BYTE_ORDER_LOW_TO_HIGH 0

#ifndef CPU_CLK_FREQ //Should already be defined in eagle_soc.h
#define CPU_CLK_FREQ 80*1000000
#endif

//Define some default SPI clock settings
#define SPI_CLK_PREDIV 10
#define SPI_CLK_CNTDIV 32
#define SPI_CLK_FREQ CPU_CLK_FREQ/(SPI_CLK_PREDIV*SPI_CLK_CNTDIV) // 80 / 320 = 250 KHz





void spi_init(uint8_t spi_no);
void spi_mode(uint8_t spi_no, uint8_t spi_cpha,uint8_t spi_cpol);
void spi_init_gpio(uint8_t spi_no, uint8_t sysclk_as_spiclk);
void spi_clock(uint8_t spi_no, uint16 prediv, uint8_t cntdiv);
void spi_tx_byte_order(uint8_t spi_no, uint8_t byte_order);
void spi_rx_byte_order(uint8_t spi_no, uint8_t byte_order);
uint32_t spi_transaction(uint8_t spi_no, uint8_t cmd_bits, uint16 cmd_data, uint32 addr_bits, uint32 addr_data, uint32 dout_bits, uint32 dout_data, uint32 din_bits, uint32 dummy_bits);
void hspi_autocs_mode(uint8_t spi_cs_state);

//Expansion Macros
#define spi_busy(spi_no) READ_PERI_REG(SPI_CMD(spi_no))&SPI_USR

#define spi_txd(spi_no, bits, data) spi_transaction(spi_no, 0, 0, 0, 0, bits, (uint32) data, 0, 0)
#define spi_tx8(spi_no, data)       spi_transaction(spi_no, 0, 0, 0, 0, 8,    (uint32) data, 0, 0)
#define spi_tx16(spi_no, data)      spi_transaction(spi_no, 0, 0, 0, 0, 16,   (uint32) data, 0, 0)
#define spi_tx32(spi_no, data)      spi_transaction(spi_no, 0, 0, 0, 0, 32,   (uint32) data, 0, 0)

#define spi_rxd(spi_no, bits) spi_transaction(spi_no, 0, 0, 0, 0, 0, 0, bits, 0)
#define spi_rx8(spi_no)       spi_transaction(spi_no, 0, 0, 0, 0, 0, 0, 8,    0)
#define spi_rx16(spi_no)      spi_transaction(spi_no, 0, 0, 0, 0, 0, 0, 16,   0)
#define spi_rx32(spi_no)      spi_transaction(spi_no, 0, 0, 0, 0, 0, 0, 32,   0)

#define HSPI_CSMODE_MANUAL   0
#define HSPI_CSMODE_AUTO     1
#define HSPI_CS_PIN          BIT15
#define HSPI_CS_HI           while (spi_busy(HSPI)); gpio_output_set(HSPI_CS_PIN, 0, 0, 0)
#define HSPI_CS_LOW          while (spi_busy(HSPI)); gpio_output_set(0, HSPI_CS_PIN, 0, 0); os_delay_us(20)

#endif
