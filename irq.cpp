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
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "keycodes.h"

const uint LED_PIN = PICO_DEFAULT_LED_PIN;
const uint CLK_PIN = 2;
const uint DATA_PIN = 3;

std::deque<int> data_bits;

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
                std::string hex_key = "F" + std::to_string(received_byte, std::hex);
                if (keycodes.find(hex_key) != keycodes.end()) {
                    std::cout << "Received key: " << keycodes[hex_key] << std::endl;
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
    while (1);
}
