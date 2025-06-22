#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

std::unordered_map<std::string, std::vector<uint8_t>> parse_macro_file(const std::string& file_contents);
std::string read_macro_txt_from_fat12();
std::vector<uint8_t> parse_macro_keys(const std::string& keys);