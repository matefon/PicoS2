cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(ps2_macro_keyboard C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# !
set(CMAKE_C_COMPILER /usr/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-none-eabi-g++)

pico_sdk_init()

add_executable(${PROJECT_NAME}
        ps2_macro_keyboard.cpp
)   

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    pico_unique_id
    tinyusb_device
    tinyusb_board
    hardware_i2c
    pico_ssd1306
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})

add_subdirectory(pico-ssd1306)

add_definitions(-DUSB_PRODUCT="Pico Keyboard")
add_definitions(-DCFG_TUSB_CONFIG_FILE="includes/tusb_config.h")

# enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
