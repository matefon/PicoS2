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

// for reading, io, basic Pico functions
#include <iostream>
#include <deque>
#include <set>
#include <sstream>
#include <iomanip>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
// for keycodes
#include "keycodes.h"
// for USB HID
#include "pico/binary_info.h"
#include "includes/usb.h"
#include "includes/Adafruit_USBD_CDC-stub.h"
#include "Adafruit_TinyUSB_Arduino/src/Adafruit_TinyUSB.h"
#include "TinyUSB_Mouse_and_Keyboard/TinyUSB_Mouse_and_Keyboard.h"

//#define DEBUG
#define DISPLAY
#define PRINT

#ifdef DISPLAY
// for OLED 128x32 display
    #include "pico-ssd1306/ssd1306.h"
    #include "pico-ssd1306/textRenderer/TextRenderer.h"
    #include "hardware/i2c.h"
#endif

const uint LED_PIN = PICO_DEFAULT_LED_PIN;
const uint CLK_PIN = 16;
const uint DATA_PIN = 17;

class PS2 {
private:
    std::set<std::string> keys;
    bool rel;
    bool ext;
    #ifdef DISPLAY
        pico_ssd1306::SSD1306* display;
    #endif
public:
    PS2() : rel(false), ext(false) {keys.clear();}

    // Return value? It should return if insertion is successful, but insert() returns std::pair.
    void press(std::string hex_key) {
        #ifdef DEBUG
            std::cout << "[PS2 hex] " << hex_key << std::endl;
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
            #ifdef DEBUG
                std::cout << "[PS2 key] " << key << std::endl;
            #endif
            if (rel) { // remove key
                release(key);
                rel = false;
                #ifdef DISPLAY
                    display->clear();
                    display->sendBuffer();
                #endif
            } else {
                keys.insert(key); // if no release code received, add key
            }
        }
    } 

    int release(std::string key) {return keys.erase(key);}

    bool empty() {return keys.empty();}

    std::string list() const {
        std::string keys_string;
        for (auto it = keys.begin(); it != keys.end(); ++it) {
            keys_string += *it;
        }
        return keys_string;
    }

    std::set<std::string> getkeys() const {return keys;}

    #ifdef DISPLAY
        void setdisplay(pico_ssd1306::SSD1306* d) {
            display = d;
        }
    #endif
    friend std::ostream& operator<<(std::ostream& os, const PS2& k);

    ~PS2() {}

};

std::ostream& operator<<(std::ostream& os, const PS2& k) {
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
PS2 ps2;

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
                #endif
                if (keycodes.find(hex_key) != keycodes.end()) {
                    //std::cout << "Received key: " << keycodes[hex_key] << std::endl;
                    ps2.press(hex_key);
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

// **************** from pico-superkey-board.cpp:
// This function is called by a timer to change the on-board LED to flash
// differently depending on USB state
static bool loopTask(repeating_timer_t *rt){
    led_blinking_task();
    return true;
}

// Adafruit TinyUSB instance
extern Adafruit_USBD_Device TinyUSBDevice;

// *********************

int main() {
    stdio_init_all();
    stdio_uart_init_full(uart1, 115200,0,1);

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
        display.setOrientation(0);
        pico_ssd1306::drawText(&display, font_8x8, "PS2 macro", 0 ,0);
        pico_ssd1306::drawText(&display, font_8x8, "keyboard", 0 ,10);
        display.sendBuffer();
        ps2.setdisplay(&display);
    #endif
    // *** from pico_superkey_board.cpp
    //bi_decl(bi_program_description("A PS/2 to macro keyboard software"));
    //bi_decl(bi_program_feature("USB HID Device"));

    board_init(); // Sets up the onboard LED as an output
    TinyUSBDevice.begin(); // Initialise Adafruit TinyUSB

    // Timer for regularly processing USB events
    struct repeating_timer timer;
    add_repeating_timer_ms(10, loopTask, NULL, &timer);

    // Initialise a keyboard (code will wait here to be plugged in)
    Keyboard.begin();
                
    //Keyboard.release(KEY_LEFT_GUI); // and true is released
    //Keyboard.press(KEY_LEFT_GUI);
    // ***

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);    
    
    gpio_init(DATA_PIN);
    gpio_set_dir(DATA_PIN, GPIO_IN);

    gpio_init(CLK_PIN);
    gpio_set_dir(CLK_PIN, GPIO_IN);
    
    gpio_pull_up(CLK_PIN); // Pull-up resistor on the clock line
    
    sleep_ms(500);
    std::cout << "Hello GPIO IRQ" << std::endl;

    #ifdef DISPLAY
        display.clear();
        display.sendBuffer();
    #endif

    gpio_set_irq_enabled_with_callback(CLK_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_callback);

    // Wait forever
    while (1) {
        if (!ps2.empty()) {
            #ifdef PRINT
                std::cout << ps2 << std::endl;
            #endif
            //Keyboard.write('a');

            Keyboard.print(ps2.list());
            #ifdef DISPLAY
                //display.clear();
                pico_ssd1306::drawText(&display, font_12x16, ps2.list().c_str(), 0 ,0);
                display.sendBuffer();
            #endif
        }
        sleep_ms(100);
    }
}