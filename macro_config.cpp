#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include "usb_codes.h"

// --- Helper: key name to keycode map ---
std::unordered_map<std::string, uint8_t> keyname_to_code = {
    {"KEY_ESC", KEY_ESC},
    {"KEY_F1", KEY_F1},
    {"KEY_F2", KEY_F2},
    {"KEY_F3", KEY_F3},
    {"KEY_F4", KEY_F4},
    {"KEY_F5", KEY_F5},
    {"KEY_F6", KEY_F6},
    {"KEY_F7", KEY_F7},
    {"KEY_F8", KEY_F8},
    {"KEY_F9", KEY_F9},
    {"KEY_F10", KEY_F10},
    {"KEY_F11", KEY_F11},
    {"KEY_F12", KEY_F12},
    {"KEY_SCROLLLOCK", KEY_SCROLLLOCK},
    {"KEY_GRAVE", KEY_GRAVE},
    {"KEY_1", KEY_1},
    {"KEY_2", KEY_2},
    {"KEY_3", KEY_3},
    {"KEY_4", KEY_4},
    {"KEY_5", KEY_5},
    {"KEY_6", KEY_6},
    {"KEY_7", KEY_7},
    {"KEY_8", KEY_8},
    {"KEY_9", KEY_9},
    {"KEY_0", KEY_0},
    {"KEY_MINUS", KEY_MINUS},
    {"KEY_EQUAL", KEY_EQUAL},
    {"KEY_BACKSPACE", KEY_BACKSPACE},
    {"KEY_TAB", KEY_TAB},
    {"KEY_Q", KEY_Q},
    {"KEY_W", KEY_W},
    {"KEY_E", KEY_E},
    {"KEY_R", KEY_R},
    {"KEY_T", KEY_T},
    {"KEY_Y", KEY_Y},
    {"KEY_U", KEY_U},
    {"KEY_I", KEY_I},
    {"KEY_O", KEY_O},
    {"KEY_P", KEY_P},
    {"KEY_LEFTBRACE", KEY_LEFTBRACE},
    {"KEY_RIGHTBRACE", KEY_RIGHTBRACE},
    {"KEY_BACKSLASH", KEY_BACKSLASH},
    {"KEY_CAPSLOCK", KEY_CAPSLOCK},
    {"KEY_A", KEY_A},
    {"KEY_S", KEY_S},
    {"KEY_D", KEY_D},
    {"KEY_F", KEY_F},
    {"KEY_G", KEY_G},
    {"KEY_H", KEY_H},
    {"KEY_J", KEY_J},
    {"KEY_K", KEY_K},
    {"KEY_L", KEY_L},
    {"KEY_SEMICOLON", KEY_SEMICOLON},
    {"KEY_APOSTROPHE", KEY_APOSTROPHE},
    {"KEY_ENTER", KEY_ENTER},
    {"KEY_LEFTSHIFT", KEY_LEFTSHIFT},
    {"KEY_Z", KEY_Z},
    {"KEY_X", KEY_X},
    {"KEY_C", KEY_C},
    {"KEY_V", KEY_V},
    {"KEY_B", KEY_B},
    {"KEY_N", KEY_N},
    {"KEY_M", KEY_M},
    {"KEY_COMMA", KEY_COMMA},
    {"KEY_DOT", KEY_DOT},
    {"KEY_SLASH", KEY_SLASH},
    {"KEY_RIGHTSHIFT", KEY_RIGHTSHIFT},
    {"KEY_LEFTCTRL", KEY_LEFTCTRL},
    {"KEY_LEFTMETA", KEY_LEFTMETA},
    {"KEY_LEFTALT", KEY_LEFTALT},
    {"KEY_SPACE", KEY_SPACE},
    {"KEY_RIGHTALT", KEY_RIGHTALT},
    {"KEY_RIGHTMETA", KEY_RIGHTMETA},
    {"KEY_RIGHTCTRL", KEY_RIGHTCTRL},
    {"KEY_INSERT", KEY_INSERT},
    {"KEY_HOME", KEY_HOME},
    {"KEY_PAGEUP", KEY_PAGEUP},
    {"KEY_DELETE", KEY_DELETE},
    {"KEY_END", KEY_END},
    {"KEY_PAGEDOWN", KEY_PAGEDOWN},
    {"KEY_UP", KEY_UP},
    {"KEY_LEFT", KEY_LEFT},
    {"KEY_DOWN", KEY_DOWN},
    {"KEY_RIGHT", KEY_RIGHT},
    {"KEY_NUMLOCK", KEY_NUMLOCK},
    {"KEY_KPSLASH", KEY_KPSLASH},
    {"KEY_KPASTERISK", KEY_KPASTERISK},
    {"KEY_KPMINUS", KEY_KPMINUS},
    {"KEY_KP7", KEY_KP7},
    {"KEY_KP8", KEY_KP8},
    {"KEY_KP9", KEY_KP9},
    {"KEY_KPPLUS", KEY_KPPLUS},
    {"KEY_KP4", KEY_KP4},
    {"KEY_KP5", KEY_KP5},
    {"KEY_KP6", KEY_KP6},
    {"KEY_KP1", KEY_KP1},
    {"KEY_KP2", KEY_KP2},
    {"KEY_KP3", KEY_KP3},
    {"KEY_KP0", KEY_KP0},
    {"KEY_KPDOT", KEY_KPDOT}
};

// --- Helper: parse a line like "PS2_F1=KEY_LEFTCTRL,KEY_LEFTALT,KEY_T" ---
std::vector<uint8_t> parse_macro_keys(const std::string& keys) {
    std::vector<uint8_t> result;
    std::stringstream ss(keys);
    std::string key;
    while (std::getline(ss, key, ',')) {
        key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
        auto it = keyname_to_code.find(key);
        if (it != keyname_to_code.end())
            result.push_back(it->second);
    }
    return result;
}

// --- Parse the config file into a map ---
std::unordered_map<std::string, std::vector<uint8_t>> parse_macro_file(const std::string& file_contents) {
    std::unordered_map<std::string, std::vector<uint8_t>> macros;
    std::stringstream ss(file_contents);
    std::string line;
    while (std::getline(ss, line)) {
        auto eq = line.find('=');
        if (eq != std::string::npos) {
            std::string ps2key = line.substr(0, eq);
            std::string keys = line.substr(eq + 1);
            macros[ps2key] = parse_macro_keys(keys);
        }
    }
    return macros;
}

// Helper: Convert "MACRO.TXT" to FAT12 8.3 format
void filename_to_fat(const char* name, char fatname[11]) {
    memset(fatname, ' ', 11);
    int i = 0, j = 0;
    while (name[i] && j < 8 && name[i] != '.') fatname[j++] = toupper(name[i++]);
    if (name[i] == '.') {
        i++;
        j = 8;
        int k = 0;
        while (name[i] && k < 3) fatname[j++] = toupper(name[i++]), k++;
    }
}

// Read MACRO.TXT from FAT12 RAM disk
std::string read_macro_txt_from_fat12() {
    extern uint8_t msc_disk[][512];

    // FAT12 layout (assuming standard 1.44MB floppy params, adjust if needed)
    const int BYTES_PER_SECTOR = 512;
    const int RESERVED_SECTORS = 1;
    const int NUM_FATS = 2;
    const int SECTORS_PER_FAT = 1; // For 64K disk, likely 1 sector per FAT
    const int ROOT_ENTRIES = 224;
    const int ROOT_DIR_SECTORS = ((ROOT_ENTRIES * 32) + (BYTES_PER_SECTOR - 1)) / BYTES_PER_SECTOR;
    const int FAT_START = RESERVED_SECTORS;
    const int ROOT_START = FAT_START + NUM_FATS * SECTORS_PER_FAT;
    const int DATA_START = ROOT_START + ROOT_DIR_SECTORS;

    // 1. Search root directory for MACRO.TXT
    char fatname[11];
    filename_to_fat("MACRO.TXT", fatname);

    for (int entry = 0; entry < ROOT_ENTRIES; ++entry) {
        int sector = ROOT_START + (entry * 32) / BYTES_PER_SECTOR;
        int offset = (entry * 32) % BYTES_PER_SECTOR;
        uint8_t* dir = &msc_disk[sector][offset];
        if (dir[0] == 0x00) break; // No more entries
        if ((dir[11] & 0x08) || (dir[11] & 0x10)) continue; // Skip volume labels/dirs
        if (memcmp(dir, fatname, 11) == 0) {
            // Found!
            uint16_t start_cluster = dir[26] | (dir[27] << 8);
            uint32_t file_size = dir[28] | (dir[29] << 8) | (dir[30] << 16) | (dir[31] << 24);

            // 2. Read file data by following FAT chain
            std::vector<uint8_t> file_data;
            uint16_t cluster = start_cluster;
            while (cluster >= 2 && cluster < 0xFF8 && file_data.size() < file_size) {
                int sector_num = DATA_START + (cluster - 2);
                uint8_t* data = &msc_disk[sector_num][0];
                size_t to_copy = std::min((size_t)BYTES_PER_SECTOR, (size_t)(file_size - file_data.size()));
                file_data.insert(file_data.end(), data, data + to_copy);

                // Read next cluster from FAT
                int fat_offset = (cluster * 3) / 2;
                uint16_t next;
                if (cluster & 1)
                    next = ((msc_disk[FAT_START][fat_offset] >> 4) | (msc_disk[FAT_START][fat_offset + 1] << 4)) & 0xFFF;
                else
                    next = (msc_disk[FAT_START][fat_offset] | ((msc_disk[FAT_START][fat_offset + 1] & 0x0F) << 8)) & 0xFFF;
                cluster = next;
            }
            return std::string(file_data.begin(), file_data.end());
        }
    }
    return ""; // Not found
}