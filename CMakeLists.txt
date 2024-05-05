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
        #pico-superkey-board.cpp
        TinyUSB_Mouse_and_Keyboard/TinyUSB_Mouse_and_Keyboard.cpp
        Adafruit_TinyUSB_Arduino/src/arduino/hid/Adafruit_USBD_HID.cpp
        Adafruit_TinyUSB_Arduino/src/arduino/Adafruit_USBD_Device.cpp
        Adafruit_TinyUSB_Arduino/src/arduino/ports/rp2040/Adafruit_TinyUSB_rp2040.cpp
        Print.cpp
)   

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    pico_unique_id
    tinyusb_device
    tinyusb_board
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})

add_definitions(-DUSE_TINYUSB)
add_definitions(-DUSB_PRODUCT="Pico Keyboard")
add_definitions(-DARDUINO_ARCH_RP2040)
add_definitions(-DCFG_TUSB_CONFIG_FILE="includes/tusb_config.h")

include_directories(includes
    TinyUSB_Mouse_and_Keyboard
    Adafruit_TinyUSB_Arduino/src
)

# enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

# remove Adafruit tinyusb library code so that only the pico-sdk tinyusb code is used
# to restore, cd Adafruit_TinyUSB_Arduino/ && git checkout HEAD .
file(REMOVE Adafruit_TinyUSB_Arduino/src/tusb.c)
file(REMOVE Adafruit_TinyUSB_Arduino/src/tusb.h)
file(REMOVE Adafruit_TinyUSB_Arduino/src/tusb_option.h)
file(REMOVE_RECURSE Adafruit_TinyUSB_Arduino/src/osal/)
file(REMOVE_RECURSE Adafruit_TinyUSB_Arduino/src/portable/)
file(REMOVE_RECURSE Adafruit_TinyUSB_Arduino/src/device/)
file(REMOVE_RECURSE Adafruit_TinyUSB_Arduino/src/common/)
file(REMOVE_RECURSE Adafruit_TinyUSB_Arduino/src/class/)
