#ifndef USB_CODES_H
#define USB_CODES_H

#include <unordered_map>
#include <string>
#include <vector>
#include "usb_hid_keys.h"
// The key definitions is in usb_hid_keys.h, see the file to add more macros.

/// This is the translation layer. The left hand side is the PS/2 keycode, the right hand side can be changed to program the macro.
/// By Tinyusb (or USB?) limitations only 6 keys can be pressed at the same time.
std::unordered_map<std::string, std::vector<uint8_t>> ps2_to_macro = {
    {"ESC", {KEY_ESC}},
    {"F1", {KEY_LEFTCTRL, KEY_LEFTALT, KEY_T}}, // macro
    {"F2", {KEY_MUTE}}, // macro
    {"F3", {KEY_F3}},
    {"F4", {KEY_F4}},
    {"F5", {KEY_F5}},
    {"F6", {KEY_F6}},
    {"F7", {KEY_F7}},
    {"F8", {KEY_F8}},
    {"F9", {KEY_F9}},
    {"F10", {KEY_F10}},
    {"F11", {KEY_F11}},
    {"F12", {KEY_F12}},
    {"Scroll Lock", {KEY_SCROLLLOCK}},
    {"`", {KEY_GRAVE}},
    {"1", {KEY_1}},
    {"2", {KEY_2}},
    {"3", {KEY_3}},
    {"4", {KEY_4}},
    {"5", {KEY_5}},
    {"6", {KEY_6}},
    {"7", {KEY_7}},
    {"8", {KEY_8}},
    {"9", {KEY_9}},
    {"0", {KEY_0}},
    {"-", {KEY_MINUS}},
    {"=", {KEY_EQUAL}},
    {"Backspace", {KEY_BACKSPACE}},
    {"Tab", {KEY_TAB}},
    {"Q", {KEY_Q}},
    {"W", {KEY_W}},
    {"E", {KEY_E}},
    {"R", {KEY_R}},
    {"T", {KEY_T}},
    {"Y", {KEY_Y}},
    {"U", {KEY_U}},
    {"I", {KEY_I}},
    {"O", {KEY_O}},
    {"P", {KEY_P}},
    {"[", {KEY_LEFTBRACE}},
    {"]", {KEY_RIGHTBRACE}},
    {"\\", {KEY_BACKSLASH}},
    {"Caps Lock", {KEY_CAPSLOCK}},
    {"A", {KEY_A}},
    {"S", {KEY_S}},
    {"D", {KEY_D}},
    {"F", {KEY_F}},
    {"G", {KEY_G}},
    {"H", {KEY_H}},
    {"J", {KEY_J}},
    {"K", {KEY_K}},
    {"L", {KEY_L}},
    {";", {KEY_SEMICOLON}},
    {"'", {KEY_APOSTROPHE}},
    {"Enter", {KEY_ENTER}},
    {"Shift (left)", {KEY_LEFTSHIFT}},
    {"Z", {KEY_Z}},
    {"X", {KEY_X}},
    {"C", {KEY_C}},
    {"V", {KEY_V}},
    {"B", {KEY_B}},
    {"N", {KEY_N}},
    {"M", {KEY_M}},
    {",", {KEY_COMMA}},
    {".", {KEY_DOT}},
    {"/", {KEY_SLASH}},
    {"Shift (right)", {KEY_RIGHTSHIFT}},
    {"Ctrl (left)", {KEY_LEFTCTRL}},
    {"Windows (left)", {KEY_LEFTMETA}},
    {"Alt (left)", {KEY_LEFTALT}},
    {"Spacebar", {KEY_SPACE}},
    {"Alt (right)", {KEY_RIGHTALT}},
    {"Windows (right)", {KEY_RIGHTMETA}},
    {"Ctrl (right)", {KEY_RIGHTCTRL}},
    {"Insert", {KEY_INSERT}},
    {"Home", {KEY_HOME}},
    {"Page Up", {KEY_PAGEUP}},
    {"Delete", {KEY_DELETE}},
    {"End", {KEY_END}},
    {"Page Down", {KEY_PAGEDOWN}},
    {"Up Arrow", {KEY_UP}},
    {"Left Arrow", {KEY_LEFT}},
    {"Down Arrow", {KEY_DOWN}},
    {"Right Arrow", {KEY_RIGHT}},
    {"Num Lock", {KEY_NUMLOCK}},
    {"Keypad *", {KEY_KPASTERISK}},
    {"Keypad -", {KEY_KPMINUS}},
    {"Keypad 7", {KEY_KP7}},
    {"Keypad 8", {KEY_KP8}},
    {"Keypad 9", {KEY_KP9}},
    {"Keypad +", {KEY_KPPLUS}},
    {"Keypad 4", {KEY_KP4}},
    {"Keypad 5", {KEY_KP5}},
    {"Keypad 6", {KEY_KP6}},
    {"Keypad 1", {KEY_KP1}},
    {"Keypad 2", {KEY_KP2}},
    {"Keypad 3", {KEY_KP3}},
    {"Keypad 0", {KEY_KP0}},
    {"Keypad Period", {KEY_KPDOT}}
};


/// This map contains the basic translation layer, with corresponding keys. Not for macro use.
std::unordered_map<std::string, uint8_t> ps2_to_usb = {
    {"76", KEY_ESC},          // ESC
    {"05", KEY_F1},           // F1
    {"06", KEY_F2},           // F2
    {"04", KEY_F3},           // F3
    {"0C", KEY_F4},           // F4
    {"03", KEY_F5},           // F5
    {"0B", KEY_F6},           // F6
    {"83", KEY_F7},           // F7
    {"0A", KEY_F8},           // F8
    {"01", KEY_F9},           // F9
    {"09", KEY_F10},          // F10
    {"78", KEY_F11},          // F11
    {"07", KEY_F12},          // F12
    {"7E", KEY_SCROLLLOCK},   // Scroll Lock
    {"0E", KEY_GRAVE},        // `
    {"16", KEY_1},            // 1
    {"1E", KEY_2},            // 2
    {"26", KEY_3},            // 3
    {"25", KEY_4},            // 4
    {"2E", KEY_5},            // 5
    {"36", KEY_6},            // 6
    {"3D", KEY_7},            // 7
    {"3E", KEY_8},            // 8
    {"46", KEY_9},            // 9
    {"45", KEY_0},            // 0
    {"4E", KEY_MINUS},        // -
    {"55", KEY_EQUAL},        // =
    {"66", KEY_BACKSPACE},    // Backspace
    {"0D", KEY_TAB},          // Tab
    {"15", KEY_Q},            // Q
    {"1D", KEY_W},            // W
    {"24", KEY_E},            // E
    {"2D", KEY_R},            // R
    {"2C", KEY_T},            // T
    {"35", KEY_Y},            // Y
    {"3C", KEY_U},            // U
    {"43", KEY_I},            // I
    {"44", KEY_O},            // O
    {"4D", KEY_P},            // P
    {"54", KEY_LEFTBRACE},    // [
    {"5B", KEY_RIGHTBRACE},   // ]
    {"5D", KEY_BACKSLASH},    // Backslash
    {"58", KEY_CAPSLOCK},     // Caps Lock
    {"1C", KEY_A},            // A
    {"1B", KEY_S},            // S
    {"23", KEY_D},            // D
    {"2B", KEY_F},            // F
    {"34", KEY_G},            // G
    {"33", KEY_H},            // H
    {"3B", KEY_J},            // J
    {"42", KEY_K},            // K
    {"4B", KEY_L},            // L
    {"4C", KEY_SEMICOLON},    // ;
    {"52", KEY_APOSTROPHE},   // '
    {"5A", KEY_ENTER},        // Enter
    {"12", KEY_LEFTSHIFT},    // Left Shift
    {"1A", KEY_Z},            // Z
    {"22", KEY_X},            // X
    {"21", KEY_C},            // C
    {"2A", KEY_V},            // V
    {"32", KEY_B},            // B
    {"31", KEY_N},            // N
    {"3A", KEY_M},            // M
    {"41", KEY_COMMA},        // ,
    {"49", KEY_DOT},          // .
    {"4A", KEY_SLASH},        // /
    {"59", KEY_RIGHTSHIFT},   // Right Shift
    {"14", KEY_LEFTCTRL},     // Left Ctrl
    {"E01F", KEY_LEFTMETA},   // Left Windows key
    {"11", KEY_LEFTALT},      // Left Alt
    {"29", KEY_SPACE},        // Spacebar
    {"E011", KEY_RIGHTALT},   // Right Alt
    {"E027", KEY_RIGHTMETA},  // Right Windows key
    {"E014", KEY_RIGHTCTRL},  // Right Ctrl
    {"E070", KEY_INSERT},     // Insert
    {"E06C", KEY_HOME},       // Home
    {"E07D", KEY_PAGEUP},     // Page Up
    {"E071", KEY_DELETE},     // Delete
    {"E069", KEY_END},        // End
    {"E07A", KEY_PAGEDOWN},   // Page Down
    {"E075", KEY_UP},         // Up Arrow
    {"E06B", KEY_LEFT},       // Left Arrow
    {"E072", KEY_DOWN},       // Down Arrow
    {"E074", KEY_RIGHT},      // Right Arrow
    {"77", KEY_NUMLOCK},      // Num Lock
    {"E04A", KEY_SLASH},      // /
    {"7C", KEY_KPASTERISK},   // Keypad *
    {"7B", KEY_KPMINUS},      // Keypad -
    {"6C", KEY_KP7},          // Keypad 7
    {"75", KEY_KP8},          // Keypad 8
    {"7D", KEY_KP9},          // Keypad 9
    {"79", KEY_KPPLUS},       // Keypad +
    {"6B", KEY_KP4},          // Keypad 4
    {"73", KEY_KP5},          // Keypad 5
    {"74", KEY_KP6},          // Keypad 6
    {"69", KEY_KP1},          // Keypad 1
    {"72", KEY_KP2},          // Keypad 2
    {"7A", KEY_KP3},          // Keypad 3
    {"70", KEY_KP0},          // Keypad 0
    {"71", KEY_KPDOT}         // Keypad Period
};

/* obselete: kept for now if needed
std::unordered_map<std::string, uint8_t> usb_codes = {
    {"ESC", 0x29},
    {"F1", 0x3a},
    {"F2", 0x3b},
    {"F3", 0x3c},
    {"F4", 0x3d},
    {"F5", 0x3e},
    {"F6", 0x3f},
    {"F7", 0x40},
    {"F8", 0x41},
    {"F9", 0x42},
    {"F10", 0x43},
    {"F11", 0x44},
    {"F12", 0x45},
    {"Scroll Lock", 0x47},
    //{"`", '`'},
    {"1", 0x1e},
    {"2", 0x1f},
    {"3", 0x20},
    {"4", 0x21},
    {"5", 0x22},
    {"6", 0x23},
    {"7", 0x24},
    {"8", 0x25},
    {"9", 0x26},
    {"0", 0x27},
    {"-", 0x2d},
    {"=", 0x2e},
    {"Backspace", 0x2a},
    {"Tab", 0x2b},
    {"Q", 0x14},
    {"W", 0x1a},
    {"E", 0x08},
    {"R", 0x15},
    {"T", 0x17},
    {"Y", 0x1c},
    {"U", 0x18},
    {"I", 0x0c},
    {"O", 0x12},
    {"P", 0x13},
    {"[", 0x2f},
    {"]", 0x30},
    {"\\", 0x31},
    {"Caps Lock", 0x39},
    {"A", 0x04},
    {"S", 0x16},
    {"D", 0x07},
    {"F", 0x09},
    {"G", 0x0a},
    {"H", 0x0b},
    {"J", 0x0d},
    {"K", 0x0e},
    {"L", 0x0f},
    {";", 0x33},
    {"\'", 0x34},
    {"Enter", 0x28},
    {"Shift (Left)", 0xe1},
    {"Z", 0x1d},
    {"X", 0x1b},
    {"C", 0x06},
    {"V", 0x19},
    {"B", 0x05},
    {"N", 0x11},
    {"M", 0x10},
    {",", 0x36},
    {".", 0x37},
    {"/", 0x38},
    {"Shift (Right)", 0xe5},
    {"Ctrl (left)", 0xe0},
    {"Windows (left)", 0xe3},
    {"Alt (left)", 0xe2},
    {"Spacebar", 0x2c},
    {"Alt (right)", 0xe6},
    {"Windows (right)", 0xe7},
    {"Ctrl (right)", 0xe4},
    {"Insert", 0x49},
    {"Home", 0x4a},
    {"Page Up", 0x4b},
    {"Delete", 0x4c},
    {"End", 0x4d},
    {"Page Down", 0x4e},
    {"Up Arrow", 0x52},
    {"Left Arrow", 0x50},
    {"Down Arrow", 0x51},
    {"Right Arrow", 0x4f},
    {"*", '*'},
    {"+", 0x57},
};
*/
#endif
