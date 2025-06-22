# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.13)
include_guard(GLOBAL)

set(BOARD pico_sdk)
set(FAMILY rp2040)
include(tinyusb/hw/bsp/family_support.cmake)

project(ps2_macro_keyboard C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# !
#set(CMAKE_C_COMPILER /usr/bin/arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER /usr/bin/arm-none-eabi-g++)

#set(CMAKE_C_COMPILER arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

pico_sdk_init()

add_executable(${PROJECT_NAME})   
target_sources(${PROJECT_NAME} PUBLIC
    ps2_macro_keyboard.cpp
    includes/usb_descriptors.c
    #tinyusb/examples/device/hid_composite/src/usb_descriptors.c
    msc_disk.c
    macro_config.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR} tinyusb/src)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    pico_unique_id
    tinyusb_common
    tinyusb_device_base
    tinyusb_bsp
    tinyusb_device
    tinyusb_board
    hardware_i2c
    pico_ssd1306
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})

add_subdirectory(pico-ssd1306 tinyusb/src)

#add_definitions(-DUSB_PRODUCT="Pico Keyboard")
add_definitions(-DCFG_TUSB_CONFIG_FILE="includes/tusb_config.h")
add_definitions(-DCFG_TUD_HID=1)
add_definitions(-DCFG_TUD_MSC=1)
#add_definitions(-DUSE_TINYUSB)

# enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)