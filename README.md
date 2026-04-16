# Floyd-Fish/ST7789-STM32 fork with fixed DMA colors
Origianl Floyd-Fish lib has a problem with color output when using DMA - colors are mixed up. The code may not compile at all with an error, or produce firmware with mixed up colors during DMA

# List of changes:
-Correct display of colors with DMA

-Removed interference on the screen when turning on

-Added functions for drawing using DMA


# ST7789-STM32-DMA
Using Hardware SPI to drive an ST7789 based LCD display.

The library still defaults to STM32 HAL, but it now has a small platform port layer so the same driver source can also be built for Raspberry Pi Pico with the Pico SDK.

## Supported targets

- STM32 HAL: default target, with the existing DMA path preserved
- Raspberry Pi Pico / RP2040: optional target via Pico SDK, with SPI TX DMA support

## How to use ?

1. Copy the `ST7789` dir into your project and add it to the include path.
2. Include `st7789.h` where you want to use the driver.
3. Provide a port header for your platform.
4. Select your display geometry and rotation with compile definitions or by editing `st7789.h`.
5. Call `ST7789_Init();` during startup.
6. Turn the backlight on in your board code.

The driver includes `st7789_port.h` by default. To use a different platform binding, compile with:

```c
-DST7789_PORT_HEADER=\"your_port_header.h\"
```

The port header must define these macros:

- `ST7789_DELAY(ms)`
- `ST7789_SPI_WRITE(data, size)`
- `ST7789_RST_Clr()` and `ST7789_RST_Set()`
- `ST7789_DC_Clr()` and `ST7789_DC_Set()`
- `ST7789_Select()` and `ST7789_UnSelect()`

If `ST7789_USE_DMA` is enabled, the port header must also define:

- `ST7789_SPI_WRITE_DMA(data, size)`
- `ST7789_SPI_WAIT_DMA()`

The built-in STM32 HAL port keeps DMA enabled by default. Targets that do not provide DMA should define `ST7789_USE_DMA` as `0`.

## Configuration

Display selection now supports compile-time overrides. If no display macro is set, the library falls back to `USING_240X320` and `ST7789_ROTATION=1`.

Example:

```c
-DUSING_240X240 -DST7789_ROTATION=2
```

This code has been tested on 240x240 & 170x320 LCD screens.
  
> Most MCUs don't have a large enough RAM, so a framebuffer is "cut" into pieces, e.g: a 240x5 pixel buffer for a 240x240 screen.  

## SPI Interface

If you are using **Dupont Line(or jumper wire)**, please notice that your CLK frequency should not exceed 40MHz (may vary, depends on the length of your wire), **otherwise data transfer will collapse!**  
For higher speed applications, it's recommended to **use PCB** rather than jumper wires.  

In STM32CubeMX/CubeIDE, config the SPI params as follow:

![spi](fig/spi.jpg)

I've had a simple test, connect the screen and mcu via 20cm dupont line, and it works normally on **21.25MB/s**. And if I connect a logic analyzer to the clk and data lines(15cm probe), **21.25MB/s doesn't work anymore**, I have to lower its datarate to 10.625MB/s. Using PCB to connect the display, it works up to **40MB/s** and still looks nice.

## Supported Displays

- 135*240   
- 240*240   
- 170*320 (new)
- 240*320 (new)  

If you like, you could customize it's resolution to drive different displays you prefer. 
> For example, a 240x320 display is perfectly suited for st7789.  
> Just set all X_SHIFT and Y_SHIFT to 0, and set resolution to 240|320.  

For more details, please refer to ST7789's datasheet.  

## STM32 demo

`ST7789_STM32F103C8_Demo` now builds directly against the shared files in `ST7789/`, so the STM32 example and the reusable library stay in sync.

## Raspberry Pi Pico demo

`ST7789_RP2040_Pico_Demo` is a Pico SDK example that builds the shared driver with `st7789_pico_port.h` and `st7789_pico_port.c`.

Default wiring in that demo:

- SPI0 SCK: GP18
- SPI0 MOSI: GP19
- CS: GP17
- DC: GP20
- RST: GP21
- BL: GP22

Default display settings in that demo:

- `USING_240X240`
- `ST7789_ROTATION=2`
- `ST7789_USE_DMA=1`

Build steps:

```sh
cd ST7789_RP2040_Pico_Demo
mkdir -p build
cd build
cmake .. -DPICO_SDK_PATH=/path/to/pico-sdk
cmake --build .
```

The Pico port now uses an RP2040 DMA channel for larger SPI transfers and falls back to blocking writes for small transfers, matching the shared driver's existing DMA threshold logic. The demo's `st7789_pico_port_init()` helper configures SPI, GPIO, backlight, and the DMA channel setup used by the transport layer.

# Powerful fork

https://github.com/deividAlfa/ST7789-STM32-uGUI

# Special thanks to

#### Helped fix the library
- [drunkbatya](https://github.com/drunkbatya)

#### Reference
- [ananevilya's Arduino-ST7789-Lib](https://github.com/ananevilya/Arduino-ST7789-Library)  
- [afiskon's stm32-st7735 lib](https://github.com/afiskon/stm32-st7735)

#### Contributor
- [JasonLrh](https://github.com/JasonLrh)  
- [ZiangCheng](https://github.com/ZiangCheng)  
