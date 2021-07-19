# PicoS2
## RPi Pico macro-keyboard project which uses a PS/2 keyboard's input to send keystrokes to the computer.

### IMPORTANT NOTES
The software is currently in heavy development, which means:
- the program can only read the PS/2 keys pressed and display them via UART
- I am trying to get the HID working with the help of the Git project [buildcomics/AHK_keyboard](https://github.com/buildcomics/AHK_keyboard), but I am facing compile errors

Read HIDINFO.txt for information about which file is used for the HID support.

Some info about PS/2 protocol: [hungarian](http://www.vfx.hu/info/atkeyboard.html), [english](https://www.avrfreaks.net/sites/default/files/PS2%20Keyboard.pdf)

**Also, this is my first C/C++ project, not just on the Pico, therefore I accept any help and suggestions.**
