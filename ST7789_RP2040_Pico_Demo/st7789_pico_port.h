#ifndef __ST7789_PICO_PORT_H
#define __ST7789_PICO_PORT_H

#include <stddef.h>
#include <stdint.h>

#include <hardware/dma.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <pico/time.h>

#define ST7789_USE_CS 1

#ifndef ST7789_SPI_PORT
#define ST7789_SPI_PORT spi0
#endif

#ifndef ST7789_SPI_BAUDRATE
#define ST7789_SPI_BAUDRATE 40000000u
#endif

#ifndef ST7789_SPI_SCK_PIN
#define ST7789_SPI_SCK_PIN 18
#endif

#ifndef ST7789_SPI_TX_PIN
#define ST7789_SPI_TX_PIN 19
#endif

#ifndef ST7789_CS_PIN
#define ST7789_CS_PIN 17
#endif

#ifndef ST7789_DC_PIN
#define ST7789_DC_PIN 20
#endif

#ifndef ST7789_RST_PIN
#define ST7789_RST_PIN 21
#endif

#ifndef ST7789_BL_PIN
#define ST7789_BL_PIN 22
#endif

#ifndef ST7789_USE_DMA
#define ST7789_USE_DMA 1
#endif

void st7789_pico_port_init(void);
void st7789_pico_spi_write_dma(const uint8_t *data, size_t size);
void st7789_pico_spi_wait_dma(void);

#define ST7789_DELAY(ms) sleep_ms(ms)
#define ST7789_SPI_WRITE(data, size) spi_write_blocking(ST7789_SPI_PORT, (const uint8_t *)(data), (size_t)(size))

#if ST7789_USE_DMA
#define ST7789_SPI_WRITE_DMA(data, size) st7789_pico_spi_write_dma((const uint8_t *)(data), (size_t)(size))
#define ST7789_SPI_WAIT_DMA() st7789_pico_spi_wait_dma()
#endif

#define ST7789_RST_Clr() gpio_put(ST7789_RST_PIN, 0)
#define ST7789_RST_Set() gpio_put(ST7789_RST_PIN, 1)

#define ST7789_DC_Clr() gpio_put(ST7789_DC_PIN, 0)
#define ST7789_DC_Set() gpio_put(ST7789_DC_PIN, 1)

#define ST7789_Select() gpio_put(ST7789_CS_PIN, 0)
#define ST7789_UnSelect() gpio_put(ST7789_CS_PIN, 1)

#endif