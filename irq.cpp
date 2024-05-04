/**
 * \file irq.cpp
 * The main code for reading input from PS/2
 *
 * \par Usage:
 * 1. 
 *
 *
 *
 * made by matefon (assisted by ChatGPT)
 */

#include <iostream>
#include <deque>
#include <set>
#include <sstream>
#include <iomanip>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "keycodes.h"

#include "usb_keyboard.h"
#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "tusb_config.h"

const uint LED_PIN = PICO_DEFAULT_LED_PIN;
const uint CLK_PIN = 16;
const uint DATA_PIN = 17;

class Keyboard {
private:
    std::set<std::string> keys;
    bool rel;
    bool ext;
public:
    Keyboard() : rel(false), ext(false) {keys.clear();}

    // Return value? It should return if insertion is successful, but insert() returns std::pair.
    void press(std::string hex_key) {
        #ifdef DEBUG
            std::cout << hex_key << std::endl;
        #endif
        if (hex_key == "F0") { // if release code is received, the next key should be removed
            rel = true;
        } else if (hex_key == "E0") {
            ext = true;
        } else {
            if (ext) {
                hex_key = "E0" + hex_key;
                ext = false;
            }
            std::string key = keycodes[hex_key];
            if (rel) { // remove key
                release(key);
                rel = false;
            } else {
                keys.insert(key); // if no release code received, add key
            }
        }
    } 

    int release(std::string key) {return keys.erase(key);}

    bool empty() {return keys.empty();}

    void list() const {
        for (auto it = keys.begin(); it != keys.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }

    std::set<std::string> getkeys() const {return keys;}

    friend std::ostream& operator<<(std::ostream& os, const Keyboard& k);

    ~Keyboard() {}

};

std::ostream& operator<<(std::ostream& os, const Keyboard& k) {
    std::set<std::string> temp = k.getkeys();
    for (auto it = temp.begin(); it != temp.end(); ++it) {
            os << *it << ", ";
    }
    return os;
}

// Function to convert byte to hexadecimal string with leading zeros
std::string byte_to_hex(unsigned char byte) {
    std::ostringstream oss;
    oss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    return oss.str();
}

std::deque<int> data_bits;
Keyboard keyboard;

void gpio_callback(uint gpio, uint32_t events) {
    static int bit_count = 0;
    static unsigned char received_byte = 0;

    if (gpio == CLK_PIN && events & GPIO_IRQ_EDGE_FALL) {
        int data_bit = gpio_get(DATA_PIN);
        
        // Check for start bit
        if (bit_count == 0 && data_bit == 0) {
            bit_count = 1;
            received_byte = 0;
            data_bits.clear();
            return;
        }

        // Store data bits
        if (bit_count >= 1 && bit_count <= 8) {
            data_bits.push_back(data_bit);
            bit_count++;
            return;
        }

        // Store parity bit
        if (bit_count == 9) {
            bit_count++;
            return;
        }

        // Store stop bit
        if (bit_count == 10 && data_bit == 1) {
            // Combine the received bits to form a byte
            if (data_bits.size() == 8) {
                for (int i = 0; i < 8; ++i) {
                    received_byte |= (data_bits[i] << i);
                }

                // Print the received key              
                std::string hex_key = byte_to_hex(received_byte);
                //std::cout << hex_key << std::endl;  
                if (keycodes.find(hex_key) != keycodes.end()) {
                    //std::cout << "Received key: " << keycodes[hex_key] << std::endl;
                    keyboard.press(hex_key);
                } else {
                    std::cout << "Unknown key" << std::endl;
                }
            } else {
                std::cout << "Invalid byte received" << std::endl;
            }

            // Reset bit count for the next byte
            bit_count = 0;
            return;
        }
    }
}

int main() {
    stdio_init_all();

    board_init();
// init device stack on configured roothub port
    tud_init(BOARD_TUD_RHPORT);
    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);    
    
    gpio_init(DATA_PIN);
    gpio_set_dir(DATA_PIN, GPIO_IN);

    gpio_init(CLK_PIN);
    gpio_set_dir(CLK_PIN, GPIO_IN);
    
    gpio_pull_up(CLK_PIN); // Pull-up resistor on the clock line
    
    sleep_ms(2000);
    std::cout << "Hello GPIO IRQ" << std::endl;

    gpio_set_irq_enabled_with_callback(CLK_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_callback);

    // Wait forever
    while (1) {

        tud_task(); // tinyusb device task
        led_blinking_task();

        if (!keyboard.empty()) {
            std::cout << keyboard << std::endl;
        }
        sleep_ms(100);
    }
}
