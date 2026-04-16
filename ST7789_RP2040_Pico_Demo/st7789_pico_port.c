#include "st7789_pico_port.h"

#include "pico/stdlib.h"

static int st7789_pico_dma_channel = -1;
static dma_channel_config st7789_pico_dma_config;
static bool st7789_pico_dma_active = false;

static void st7789_pico_gpio_init(uint gpio)
{
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_OUT);
}

static void st7789_pico_dma_init(void)
{
#if ST7789_USE_DMA
    if (st7789_pico_dma_channel >= 0) {
        return;
    }

    st7789_pico_dma_channel = dma_claim_unused_channel(true);
    st7789_pico_dma_config = dma_channel_get_default_config(st7789_pico_dma_channel);
    channel_config_set_transfer_data_size(&st7789_pico_dma_config, DMA_SIZE_8);
    channel_config_set_read_increment(&st7789_pico_dma_config, true);
    channel_config_set_write_increment(&st7789_pico_dma_config, false);
    channel_config_set_dreq(&st7789_pico_dma_config, spi_get_dreq(ST7789_SPI_PORT, true));
#endif
}

void st7789_pico_port_init(void)
{
    stdio_init_all();

    spi_init(ST7789_SPI_PORT, ST7789_SPI_BAUDRATE);
    spi_set_format(ST7789_SPI_PORT, 8, SPI_CPOL_1, SPI_CPHA_0, SPI_MSB_FIRST);

    gpio_set_function(ST7789_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(ST7789_SPI_TX_PIN, GPIO_FUNC_SPI);

    st7789_pico_gpio_init(ST7789_DC_PIN);
    st7789_pico_gpio_init(ST7789_RST_PIN);
    st7789_pico_gpio_init(ST7789_CS_PIN);
    st7789_pico_gpio_init(ST7789_BL_PIN);

    gpio_put(ST7789_CS_PIN, 1);
    gpio_put(ST7789_BL_PIN, 1);

    st7789_pico_dma_init();
}

void st7789_pico_spi_write_dma(const uint8_t *data, size_t size)
{
#if ST7789_USE_DMA
    if (size == 0) {
        return;
    }

    st7789_pico_dma_init();
    st7789_pico_spi_wait_dma();

    dma_channel_configure(
        st7789_pico_dma_channel,
        &st7789_pico_dma_config,
        &spi_get_hw(ST7789_SPI_PORT)->dr,
        data,
        size,
        true);

    st7789_pico_dma_active = true;
#else
    spi_write_blocking(ST7789_SPI_PORT, data, size);
#endif
}

void st7789_pico_spi_wait_dma(void)
{
#if ST7789_USE_DMA
    if (!st7789_pico_dma_active) {
        return;
    }

    dma_channel_wait_for_finish_blocking(st7789_pico_dma_channel);
    while (spi_is_busy(ST7789_SPI_PORT)) {
        tight_loop_contents();
    }

    st7789_pico_dma_active = false;
#endif
}