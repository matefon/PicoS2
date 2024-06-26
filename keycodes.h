#ifndef KEYCODES_H
#define KEYCODES_H

#include <unordered_map>
#include <string>
#include "names.h"

// source: https://techdocs.altium.com/display/FPGA/PS2+Keyboard+Scan+Codes
// Define a mapping from scan code to key name
std::unordered_map<std::string, std::string> keycodes = {
    {"76", PS2_ESC},
    {"05", PS2_F1},
    {"06", PS2_F2},
    {"04", PS2_F3},
    {"0C", PS2_F4},
    {"03", PS2_F5},
    {"0B", PS2_F6},
    {"83", PS2_F7},
    {"0A", PS2_F8},
    {"01", PS2_F9},
    {"09", PS2_F10},
    {"78", PS2_F11},
    {"07", PS2_F12},
    // {"E07CE0F012", "Prt Scr"}, // release code: E0F07CE0F012
    {"7E", PS2_SCROLL_LOCK},
    // {"E11477E1F014E077", "Pause/Break"}, // release code: None
    {"0E", PS2_GRAVE},
    {"16", PS2_1},
    {"1E", PS2_2},
    {"26", PS2_3},
    {"25", PS2_4},
    {"2E", PS2_5},
    {"36", PS2_6},
    {"3D", PS2_7},
    {"3E", PS2_8},
    {"46", PS2_9},
    {"45", PS2_0},
    {"4E", PS2_MINUS},
    {"55", PS2_EQUAL},
    {"66", PS2_BACKSPACE},
    {"0D", PS2_TAB},
    {"15", PS2_Q},
    {"1D", PS2_W},
    {"24", PS2_E},
    {"2D", PS2_R},
    {"2C", PS2_T},
    {"35", PS2_Y},
    {"3C", PS2_U},
    {"43", PS2_I},
    {"44", PS2_O},
    {"4D", PS2_P},
    {"54", PS2_LEFT_BRACKET},
    {"5B", PS2_RIGHT_BRACKET},
    {"5D", PS2_BACKSLASH},
    {"58", PS2_CAPS_LOCK},
    {"1C", PS2_A},
    {"1B", PS2_S},
    {"23", PS2_D},
    {"2B", PS2_F},
    {"34", PS2_G},
    {"33", PS2_H},
    {"3B", PS2_J},
    {"42", PS2_K},
    {"4B", PS2_L},
    {"4C", PS2_SEMICOLON},
    {"52", PS2_APOSTROPHE},
    {"5A", PS2_ENTER},
    {"12", PS2_LEFT_SHIFT},
    {"1A", PS2_Z},
    {"22", PS2_X},
    {"21", PS2_C},
    {"2A", PS2_V},
    {"32", PS2_B},
    {"31", PS2_N},
    {"3A", PS2_M},
    {"41", PS2_COMMA},
    {"49", PS2_DOT},
    {"4A", PS2_SLASH},
    {"59", PS2_RIGHT_SHIFT},
    {"14", PS2_LEFT_CTRL},
    {"E01F", PS2_LEFT_WINDOWS},
    {"11", PS2_LEFT_ALT},
    {"29", PS2_SPACEBAR},
    {"E011", PS2_RIGHT_ALT},
    {"E027", PS2_RIGHT_WINDOWS},
    {"E02F", PS2_MENUS},
    {"E014", PS2_RIGHT_CTRL},
    {"E070", PS2_INSERT},
    {"E06C", PS2_HOME},
    {"E07D", PS2_PAGE_UP},
    {"E071", PS2_DELETE},
    {"E069", PS2_END},
    {"E07A", PS2_PAGE_DOWN},
    {"E075", PS2_UP_ARROW},
    {"E06B", PS2_LEFT_ARROW},
    {"E072", PS2_DOWN_ARROW},
    {"E074", PS2_RIGHT_ARROW},
    {"77", PS2_NUM_LOCK},
    {"E04A", PS2_KEYPAD_SLASH},
    {"7C", PS2_KEYPAD_ASTERISK},
    {"7B", PS2_KEYPAD_MINUS},
    {"6C", PS2_KEYPAD_7},
    {"75", PS2_KEYPAD_8},
    {"7D", PS2_KEYPAD_9},
    {"79", PS2_KEYPAD_PLUS},
    {"6B", PS2_KEYPAD_4},
    {"73", PS2_KEYPAD_5},
    {"74", PS2_KEYPAD_6},
    {"69", PS2_KEYPAD_1},
    {"72", PS2_KEYPAD_2},
    {"7A", PS2_KEYPAD_3},
    {"70", PS2_KEYPAD_0},
    {"71", PS2_KEYPAD_PERIOD},
    {"E05A", PS2_KEYPAD_ENTER},
    {"F0", "Release"}, // Release code (only for PS/2)
    {"E0", "Extended"} // some keys (e.g. arrows) use E0 tags
};

#endif // KEYCODES_H
