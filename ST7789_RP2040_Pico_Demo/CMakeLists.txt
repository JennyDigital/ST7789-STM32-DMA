cmake_minimum_required(VERSION 3.13)

if(NOT DEFINED PICO_SDK_PATH)
  if(DEFINED ENV{PICO_SDK_PATH})
    set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
  else()
    message(FATAL_ERROR "Set PICO_SDK_PATH to your Pico SDK checkout before configuring this demo")
  endif()
endif()

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(st7789_pico_demo C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(st7789_pico_demo
  main.c
  st7789_pico_port.c
  ../ST7789/st7789.c
  ../ST7789/fonts.c
)

target_include_directories(st7789_pico_demo PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../ST7789
)

target_compile_definitions(st7789_pico_demo PRIVATE
  ST7789_PORT_HEADER="st7789_pico_port.h"
  USING_240X240
  ST7789_ROTATION=2
)

target_link_libraries(st7789_pico_demo PRIVATE
  pico_stdlib
  hardware_dma
  hardware_gpio
  hardware_spi
)

pico_enable_stdio_usb(st7789_pico_demo 0)
pico_enable_stdio_uart(st7789_pico_demo 0)

pico_add_extra_outputs(st7789_pico_demo)