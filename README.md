# PicoS2
## RPi Pico macro-keyboard project which uses a PS/2 keyboard's input to send keystrokes to the computer.

## To build, you need Pico SDK in your path, arm compilers (see CMakeLists.txt) and copy pico_sdk_import.cmake in the folder, next to ps2_macro_keyboard.cpp!

Currently working on migrating from the "little" TinyUsb library (an Arduino HID port) to the official TinyUsb library. That library has better support for keys, control and so on, but has a different system. The main branch is kept at the last working "little" TinyUsb library state, and the tinyusb branch is working on implementing the TinyUsb library.

### IMPORTANT NOTES
The software is currently in heavy development, which means:
- the program can only read the PS/2 keys pressed and display them via UART
- I am working on USB HID with the help of the Git project [guruthree/pico-superkey-board](https://github.com/guruthree/pico-superkey-board)

Some info about PS/2 protocol: [hungarian](http://www.vfx.hu/info/atkeyboard.html), [english 1](https://www.avrfreaks.net/sites/default/files/PS2%20Keyboard.pdf), [english 2](https://karooza.net/how-to-interface-a-ps2-keyboard)

**I accept any help and suggestions.**
[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/Z8Z51YOFC)
