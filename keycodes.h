#ifndef KEYCODES_H
#define KEYCODES_H

#include <unordered_map>
#include <string>

// Define a mapping from scan code to key name
std::unordered_map<std::string, std::string> keycodes = {
    {"F076", "ESC"},
    {"F005", "F1"},
    {"F006", "F2"},
    {"F004", "F3"},
    {"F00C", "F4"},
    {"F003", "F5"},
    {"F00B", "F6"},
    {"F083", "F7"},
    {"F00A", "F8"},
    {"F001", "F9"},
    {"F009", "F10"},
    {"F078", "F11"},
    {"F007", "F12"},
    {"E0F07CE0F012", "Prt Scr"},
    {"F07E", "Scroll Lock"},
    {"None", "Pause/Break"},
    {"F00E", "`"},
    {"F016", "1"},
    {"F01E", "2"},
    {"F026", "3"},
    {"F025", "4"},
    {"F02E", "5"},
    {"F036", "6"},
    {"F03D", "7"},
    {"F03E", "8"},
    {"F046", "9"},
    {"F045", "0"},
    {"F04E", "-"},
    {"F055", "="},
    {"F066", "Backspace"},
    {"F00D", "Tab"},
    {"F015", "Q"},
    {"F01D", "W"},
    {"F024", "E"},
    {"F02D", "R"},
    {"F02C", "T"},
    {"F035", "Y"},
    {"F03C", "U"},
    {"F043", "I"},
    {"F044", "O"},
    {"F04D", "P"},
    {"F054", "["},
    {"F05B", "]"},
    {"F05D", "\\"},
    {"F058", "Caps Lock"},
    {"F01C", "A"},
    {"F01B", "S"},
    {"F023", "D"},
    {"F02B", "F"},
    {"F034", "G"},
    {"F033", "H"},
    {"F03B", "J"},
    {"F042", "K"},
    {"F04B", "L"},
    {"F04C", ";"},
    {"F052", "'"},
    {"F05A", "Enter"},
    {"F012", "Shift (Left)"},
    {"F01A", "Z"},
    {"F022", "X"},
    {"F021", "C"},
    {"F02A", "V"},
    {"F032", "B"},
    {"F031", "N"},
    {"F03A", "M"},
    {"F041", ","},
    {"F049", "."},
    {"F04A", "/"},
    {"F059", "Shift (Right)"},
    {"F014", "Ctrl (left)"},
    {"E0F01F", "Windows (left)"},
    {"F011", "Alt (left)"},
    {"F029", "Spacebar"},
    {"E0F011", "Alt (right)"},
    {"E0F027", "Windows (right)"},
    {"E0F02F", "Menus"},
    {"E0F014", "Ctrl (right)"},
    {"E0F070", "Insert"},
    {"E0F06C", "Home"},
    {"E0F07D", "Page Up"},
    {"E0F071", "Delete"},
    {"E0F069", "End"},
    {"E0F07A", "Page Down"},
    {"E0F075", "Up Arrow"},
    {"E0F06B", "Left Arrow"},
    {"E0F072", "Down Arrow"},
    {"E0F074", "Right Arrow"},
    {"F077", "Num Lock"},
    {"E0F04A", "/"},
    {"F07C", "*"},
    {"F07B", "-"},
    {"F06C", "7"},
    {"F075", "8"},
    {"F07D", "9"},
    {"F079", "+"},
    {"F06B", "4"},
    {"F073", "5"},
    {"F074", "6"},
    {"F069", "1"},
    {"F072", "2"},
    {"F07A", "3"},
    {"F070", "0"},
    {"F071", "."},
    {"E0F05A", "Enter"},
    {"F0F0", "Released: "}
};

#endif // KEYCODES_H
