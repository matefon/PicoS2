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

// *** MACROS *** //
//#define DEBUG // additional prints on the terminal, for debugging use
#define USB // comment this to use screen alone, as the code waits for USB connection to be established
#define DISPLAY // enable OLED screen
#define PRINT // enable printing to terminal (less info, than debug, but enough)
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
#include "includes/usb.h" // for USB HID
#include "includes/Adafruit_USBD_CDC-stub.h" // for USB HID
#include "Adafruit_TinyUSB_Arduino/src/Adafruit_TinyUSB.h" // for USB HID
#include "TinyUSB_Mouse_and_Keyboard/TinyUSB_Mouse_and_Keyboard.h" // for USB HID

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
        for (auto it = keys.begin(); it != keys.end(); ++it) {
            keys_string += *it;
        }
        return keys_string;
    }

    std::set<std::string> getkeys() const {return keys;}

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
                #endif // DEBUG
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
    #endif // DISPLAY

    #ifdef USB
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
        // ***
    #endif // USB

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);    
    gpio_init(DATA_PIN);
    gpio_set_dir(DATA_PIN, GPIO_IN);
    gpio_init(CLK_PIN);
    gpio_set_dir(CLK_PIN, GPIO_IN);
    gpio_pull_up(CLK_PIN); // Pull-up resistor on the clock line
    
    sleep_ms(100); // wait a little to get terminal output

    std::cout << "PS/2 macro keyboard" << std::endl;
    #ifdef USB
        std::cout << "[DEFINED: USB]" << std::endl;
    #endif // USB
    #ifdef DISPLAY
    std::cout << "[DEFINED: DISPLAY]" << std::endl;
    #endif // DISPLAY
    #ifdef DEBUG
        std::cout << "[DEFINED: DEBUG]" << std::endl;
    #endif // DEBUG
    #ifdef PRINT
        std::cout << "[DEFINED: PRINT]" << std::endl;
    #endif // PRINT


    #ifdef DISPLAY
        display.clear();
        display.sendBuffer();
    #endif // DISPLAY

    gpio_set_irq_enabled_with_callback(CLK_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_callback);

    // Main loop
    while (1) {
        if (!ps2.empty()) {
            #ifdef PRINT
                std::cout << ps2 << std::endl;
            #endif // PRINT
            std::string keylist = ps2.list();
            #ifdef USB
                if (keylist == "F1") {
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.press(KEY_LEFT_ALT);
                    Keyboard.write('t');
                    Keyboard.releaseAll();
                    Keyboard.println("whoami");
                } else {
                    Keyboard.print(ps2.list());
                }
            #endif // USB
            #ifdef DISPLAY
                display.clear();
                pico_ssd1306::drawText(&display, font_12x16, keylist.c_str(), 0 ,0);
                display.sendBuffer();
            #endif // DISPLAY
        } 
        #ifdef DISPLAY
            else {
                display.clear();
                display.sendBuffer();
            }
        #endif // DISPLAY
        sleep_ms(100);
    }
}