#ifndef USB_CODES_H
#define USB_CODES_H

#include <unordered_map>
#include <string>

std::unordered_map<std::string, uint8_t> usb_codes = {
    {"76", 0xB1},
    {"1C", 0x04}
};



#endif
