cmake_minimum_required(VERSION 3.12)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(macro C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${PROJECT_NAME}
    main.cpp
    usb_descriptors.c # added for usb hid support
)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_timer
    hardware_gpio
    pico_util
    tinyusb_device # added for usb hid support
    tinyusb_board # added for usb hid support
)

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

pico_add_extra_outputs(${PROJECT_NAME})

# added for usb hid support:
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -DCFG_TUSB_DEBUG=2")
set(CMAKE_DEFSYM "${CMAKE_DEFSYM} -DLOG=2")
