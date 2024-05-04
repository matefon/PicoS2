cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/lib/tinyusb/hw/bsp/family_support.cmake)

project(irq C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

#add_executable(${PROJECT_NAME}
#        irq.cpp
#        tinyusb/src/tusb.c
#)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/irq.cpp
        #$ENV{PICO_SDK_PATH}/lib/tinyusb/src/tusb.c
        $ENV{PICO_SDK_PATH}/lib/tinyusb/hw/bsp/rp2040/family.c
        )
target_include_directories(${PROJECT_NAME} PUBLIC
        $ENV{PICO_SDK_PATH}/lib/
)       


# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})

# pull in common dependencies
target_link_libraries(${PROJECT_NAME} pico_stdlib)

# enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# add url via pico_set_program_url
#example_auto_set_url(${PROJECT_NAME})
