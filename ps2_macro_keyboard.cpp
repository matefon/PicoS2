/**
 * \file ps2_macro_keyboard.cpp
 * 
 * PicoS2 - The PS/2 macro keyboard by matefon
 * https://github.com/matefon/PicoS2
 * 
 * The main code for reading input from PS/2
 *
 * \par Usage:
 * -1. Wiring. You need a soldering iron, wire cutter and preferably a PS/2 jack to connect the keyboard. 
 * 0. Install and set up Pico SDK (add to path!)
 * 1. clone the repo
 * 2. change the keyboard macro definitions by editing usb_codes.h (you can remove unnecessary keys by actually removing them from that list) - see example at F1 and F2
 * 3. edit the CMakeLists.txt (if necessary)
 * 4. enable/disable modules by editing the macros below
 * 5. mkdir build, cd build
 * 6. cmake ..
 * 7. make
 * 8. copy the .uf2 file to the Pico (hold bootsel button and plug in)
 * 9. enjoy.
 */

// *** MACROS *** //
#define DEBUG // additional prints on the terminal, for debugging use
#define USB // comment this to use screen alone, as the code waits for USB connection to be established
#define DISPLAY // enable OLED screen
#define PRINT // enable printing to terminal (less info, than debug, but enough)
#define SLEEP_DELAY 20 // amount of ms to sleep in the main while loop; too high values result in slow response rate (e.g. key released but not immediately), low values can be too fast (maybe crash?)
//#define EMULATE // Emulate having a keyboard by sending some keys to the keylist buffer. Useful for testing when no keyboard is connected (when I'm on a train)
//#define INFOKEY "F1" // for future use
// ************* //

#include <iostream> // for reading, io, basic Pico functions
#include <deque> // for storing data bits
#include <set> // for storing pressed keys efficiently
#include <sstream> // for reading, io, basic Pico functions
#include <iomanip> // for reading, io, basic Pico functions
#include "pico/stdlib.h" // for reading, io, basic Pico functions
#include "hardware/gpio.h" // for reading, io, basic Pico functions
#include "keycodes.h" // for keycodes
#include "usb_codes.h" // for keycodes
#include "tinyusb/src/common/tusb_types.h"

#ifndef USB
    #ifndef DISPLAY
        #warning Not enabling DISPLAY when USB is disabled is for testing, no data is transmitted via USB and nothing is displayed.
    #endif // DISPLAY
#endif // USB

#ifdef DISPLAY
    #include "pico-ssd1306/ssd1306.h" // for OLED 128x32 display
    #include "pico-ssd1306/textRenderer/TextRenderer.h" // for OLED 128x32 display
    #include "hardware/i2c.h" // for OLED 128x32 display
#endif

#include "pico/binary_info.h" // for USB HID
//#include "includes/tusb_config.h"
#include "tinyusb/examples/device/hid_composite/src/tusb_config.h"
#include "tinyusb/hw/bsp/board_api.h"
//#include "tusb.h"
//#include "includes/usb_descriptors.h"
#include "tinyusb/examples/device/hid_composite/src/usb_descriptors.h"
#include "includes/tinyusb.hpp"
#include "usb_hid_keys.h"


const uint LED_PIN = PICO_DEFAULT_LED_PIN;
const uint CLK_PIN = 16;
const uint DATA_PIN = 17;

class PS2 {
private:
    std::set<std::string> keys;
    bool rel;
    bool ext;
public:
    PS2() : rel(false), ext(false) {keys.clear();}

    // No return for minimizing memory usage
    void press(std::string hex_key) {
        #ifdef DEBUG
            std::cout << "[PS2 hex] " << hex_key << std::endl;
        #endif // DEBUG
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
            #ifdef DEBUG
                std::cout << "[PS2 key] " << key << std::endl;
            #endif // DEBUG
            if (rel) { // remove key
                release(key);
                rel = false;
            } else {
                keys.insert(key); // if no release code received, add key
            }
        }
    } 

    // No return for minimizing memory usage
    void release(std::string key) {keys.erase(key);}

    bool empty() {return keys.empty();}

    std::string list() const {
        std::string keys_string;
        for (auto it : keys) {
            keys_string += it;
        }
        return keys_string;
    }

    std::set<std::string> getkeys() const {return keys;}

    // Used by the callback when an unknown key is detected. Some long keycodes (print screen and pause/break) can break the whole code, sometimes crashing the Pico
    void depress() {
        keys.clear();
        rel = false;
        ext = false;
    }

    friend std::ostream& operator<<(std::ostream& os, const PS2& k);

    ~PS2() {}

};

// Function to convert byte to hexadecimal string with leading zeros
std::string byte_to_hex(unsigned char byte) {
    std::ostringstream oss;
    oss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    return oss.str();
}

std::deque<int> data_bits;
PS2 ps2;

#ifdef USB
    /// @brief  :(
    /// Clears key set and USB codes
    void depress() {
        ps2.depress();
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        hid_task();
        std::cout << "[Depress] Done" << std::endl;
    }
#endif // USB

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
                #ifdef DEBUG
                    std::cout << "[CALLBACK hex] " << hex_key << std::endl;  
                #endif // DEBUG
                if (keycodes.find(hex_key) != keycodes.end()) {
                    ps2.press(hex_key);
                } else {
                    std::cout << "Unknown key" << std::endl;
                    #ifdef USB
                        depress(); // :(
                    #else
                        ps2.depress();
                    #endif // USB
                }
            } else {
                std::cout << "Invalid byte received" << std::endl;
                #ifdef USB
                    depress();
                #else
                    ps2.depress();
                #endif // USB
            }

            // Reset bit count for the next byte
            bit_count = 0;
            return;
        }
    }
}

// *********************

#ifdef EMULATE
    bool emulate(struct repeating_timer *t) {
        static std::string test[12] = {"1C", "F0", "1C", "32", "F0", "32", "06", "F0", "06", "05", "F0", "05"}; // A, B, F2, F1 (PS/2 keycodes)
        static int test_i = 0;
        if (test_i == 12) test_i = 0;
        ps2.press(test[test_i++]);
        return true;
    }
#endif // EMULATE

#ifdef USB
    /**
     * @brief Using the keyboard_report function from tinyusb to press macros. Releasing is handled by send_macro()
     * The function takes keys as std::vector. Up to 6 keys can be added, this is a limitation of tinyusb (or USB?)
     * @param keycodes the vector of the keycodes, translated by send_macro
     */
    void press(const std::vector<uint8_t> keycodes) {
        uint8_t keycode[6] = { 0 };
        int keycodes_size = keycodes.size();
        for (int i = 0; i < keycodes_size; i++) {
            keycode[i] = static_cast<uint8_t>(keycodes[i]);
            #ifdef DEBUG
                std::cout << "press: " << i << ": " << keycode[i] << std::endl;
            #endif // DEBUG
        }
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
        hid_task();
    }

    bool send_macro(const std::set<std::string> keys) {
        if (keys.size() > 6 || keys.empty()) {
            tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
            return false;
        } else {
            for (auto it : keys) {
                #ifdef DEBUG
                    std::cout << "send macro: " << it << std::endl;
                #endif // DEBUG
                press(ps2_to_macro[it]);
            }
        }
        return true;
    }
#endif // USB

int main() {
    stdio_init_all();

    #ifdef PRINT
        stdio_uart_init_full(uart1, 115200,0,1); // for terminal with USB-TTL adapter
    #endif // PRINT

    #ifdef USB
        board_init(); // Sets up the onboard LED as an output
        tusb_rhport_init_t dev_init = {
            .role = TUSB_ROLE_DEVICE,
            .speed = TUSB_SPEED_AUTO
        };
        tusb_init(BOARD_TUD_RHPORT, &dev_init);

        if (board_init_after_tusb) {
            board_init_after_tusb();
        }
    #endif // USB  

    #ifdef DISPLAY
        gpio_init(11); // display power, the vcc pin is used for the ps2 keyboard
        gpio_set_dir(11, GPIO_OUT);
        gpio_put(11, 1);

        i2c_init(i2c0, 1000000);
        gpio_set_function(12, GPIO_FUNC_I2C);
        gpio_set_function(13, GPIO_FUNC_I2C);
        gpio_pull_up(12);
        gpio_pull_up(13);
        sleep_ms(250);
        pico_ssd1306::SSD1306 display = pico_ssd1306::SSD1306(i2c0, 0x3C, pico_ssd1306::Size::W128xH32);
        display.setOrientation(1);
        pico_ssd1306::drawText(&display, font_8x8, "PicoS2", 0 ,0);
        pico_ssd1306::drawText(&display, font_8x8, "by matefon", 0 ,10);
        display.sendBuffer();
    #endif // DISPLAY

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);    
    gpio_init(DATA_PIN);
    gpio_set_dir(DATA_PIN, GPIO_IN);
    gpio_init(CLK_PIN);
    gpio_set_dir(CLK_PIN, GPIO_IN);
    gpio_pull_up(CLK_PIN); // Pull-up resistor on the clock line
    
    sleep_ms(300); // wait a little to get terminal output

    #ifdef DEBUG
        #ifdef CFG_TUD_MSC
            std::cout << "CFG_TUD_MSC" << std::endl;
        #endif
        #ifdef CFG_TUD_HID
            std::cout << "CFG_TUD_HID" << std::endl;
        #endif
    #endif //DEBUG

    std::cout << std::endl << "PicoS2 - The PS/2 macro keyboard by matefon (https://github.com/matefon/PicoS2)" << std::endl;
    std::cout << "Enabled modules:" << std::endl;
    #ifdef USB
        std::cout << "- [USB]" << std::endl;
    #endif // USB
    #ifdef DISPLAY
    std::cout << "- [DISPLAY]" << std::endl;
    #endif // DISPLAY
    #ifdef DEBUG
        std::cout << "- [DEBUG]" << std::endl;
    #endif // DEBUG
    #ifdef PRINT
        std::cout << "- [PRINT]" << std::endl;
    #endif // PRINT
    #ifdef EMULATE
        std::cout << "- [EMULATE]" << std::endl;
    #endif // EMULATE

    #ifdef DISPLAY
        sleep_ms(1000);
        display.clear();
        display.sendBuffer();
    #endif // DISPLAY

    gpio_set_irq_enabled_with_callback(CLK_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_callback);

    #ifdef USB // wait to be initialized
        std::cout << "[USB] {wait for connection}" << std::endl;
        #ifdef DISPLAY
            display.clear();
            pico_ssd1306::drawText(&display, font_8x8, "[USB]", 0 ,0);
            pico_ssd1306::drawText(&display, font_8x8, "connecting...", 0 ,10);
            display.sendBuffer();
        #endif // DISPLAY
        while (!tud_hid_ready()) {
            tud_task(); 
            led_blinking_task(); 
            hid_task(); 
            sleep_ms(100);
        }
        std::cout << "[USB] {connected}" << std::endl;
        #ifdef DISPLAY
            display.clear();
            pico_ssd1306::drawText(&display, font_8x8, "[USB]", 0 ,0);
            pico_ssd1306::drawText(&display, font_8x8, "connected", 0 ,10);
            display.sendBuffer();
            sleep_ms(500);
            display.clear();
            display.sendBuffer();
        #endif // DISPLAY
    #endif // USB

    #ifdef EMULATE
        struct repeating_timer timer;
        add_repeating_timer_ms(200, emulate, NULL, &timer);
    #endif

    // Main loop
    while (1) {
        #ifdef USB
            send_macro(ps2.getkeys());
            tud_task(); // tinyusb device task
            led_blinking_task();
            hid_task();
        #endif // USB
        #if defined(PRINT) || defined(DISPLAY) // if nothing is sent to the console or display, it is unnecessary to get the keys string
            if (!ps2.empty()) {
                std::string keylist = ps2.list();
                #ifdef PRINT
                    std::cout << keylist << std::endl;
                #endif // PRINT
                #ifdef DISPLAY
                    display.clear();
                    pico_ssd1306::drawText(&display, font_12x16, keylist.c_str(), 0 ,0);
                    display.sendBuffer();
                #endif // DISPLAY
            }
        #endif // PRINT || DISPLAY
    #ifdef DISPLAY
        else {
            display.clear();
            pico_ssd1306::drawText(&display, font_8x8, "[]", 110, 24);
            display.sendBuffer();
        }
    #endif // DISPLAY
        sleep_ms(SLEEP_DELAY);
    }
}