#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h" // used for repeating timer
#include "hardware/gpio.h" // used to access gpio
#include "pico/util/queue.h" // enable queues
#include <queue>
#include <string> // enable strings
#include "codes.h" // header file containing complementary functions and the keycode map
//#include "tinyusb/device/hid.h" <-- TODO

using namespace std;

// VARIABLES
const uint pin_led = 25; // onboard led
const uint pin_data = 2; // PS/2 keyboard data pin
const uint pin_clock = 3; // PS/2 clock pin
const uint count_to = 10; // counting to 10 clock ticks (the protocol uses 11, but somewhy it didn't work)
uint counter = 0; // clock tick counter
uint led_value = 0; // internal led state
uint data_bin = 0; // converting data pin state to either 1 or 0
queue<int> dataq; // queue for the data input
int dataarr[8]; // array to reverse bits of the data
string hex = ""; // used to get keycode in hex format
string key = ""; // used to get key name
bool rel = false, spec = false; // used to tell if a key is being released or is special (e.g. left arrow)

// callback for clock pulses
void falling_callback(uint gpio, uint32_t events){
// while counter is less than count_to, it adds data to queue and move the counter forward
    if (counter < count_to){
        counter ++;
// if data pin's value is not 0, the bit is 1, else 0
        if (gpio_get(pin_data) != 0){
            data_bin = 1;
        } else {
            data_bin = 0;
        }
// adding the bit to the queue
        dataq.push(data_bin);
    } else {
// deleting start and parity bit (end bit is not read, but it is always 1 so who cares)
        dataq.pop();
        for (int i = 0; i < 8; i++){
            dataq.push(dataq.front());
            dataq.pop();
        }
        dataq.pop();
// adding the queue to an array in reverse, while deleting the queue
        for (int i = 7; i >= 0; i--){
            dataarr[i] = dataq.front();
            dataq.pop();
        }
// converting the array to the hex value
        hex = bintohex(dataarr);
// exception handling to prevent output for corrupted or unknown data (e.g. plugging in the keyboard) <-- TODO: detect plugging in and sleep for 200ms
        try {
// getting key name according to the read 8 bits
            key = getkey(dataarr, spec);
            if (key != "rel" && key != "spec" && key != "spec_rel"){ // if the read data means release or special, do not print
                printf("%s ", key.c_str());
                if (rel){printf("%s\n", "released.");} 
                else {printf("%s\n", "pressed.");}
            }   
        } catch (exception& ex) {
            printf("Data corrupted or keycode '%s' is not found.\n", hex.c_str());
        }
// resetting clock counter
        counter = 0;
// turn on internal led while x is pressed (fun to watch while the rest of main functionality don't work)
        if ((key == "X") && !rel){ // if X is pressed and not releasing, turn on internal led
            gpio_put(pin_led, true);
        } else if ((key == "X") && rel){ // if X is detected and is releasing, turn off internal led
            gpio_put(pin_led, false);
        }
// the following 2 lines check if the release, special or both codes was sent
// both are resetted if their values are true except some cases:
// - release is not resetted if special is true (only when false)
// - special is not resetted if the 'releasing special key' situation is present
        if(hex == "F0"){rel = true;} else if(rel && spec){rel = true;} else if (rel && !spec){rel = false;}
        if(hex == "E0"){spec = true;} else if(key == "spec_rel"){spec == true;} else if(spec){spec = false;}
    }
}
// repeating timer callback to blink internal led (now turned off so I can turn it on by pressing X)
bool repeating_timer_callback(struct repeating_timer *t){
    led_value = 1 - led_value;
    gpio_put(pin_led, led_value);
    return true;
}

int main(){
    gpio_init(pin_led);
    gpio_set_dir(pin_led, GPIO_OUT);
// startup sequence: wait for first key so the queue is not false
    gpio_put(pin_led, true);
    sleep_ms(300);
    gpio_put(pin_led, false);
    sleep_ms(300);
    gpio_put(pin_led, true);
// don't start until a key is pressed (first key's read was always corrupted, therefore the queue was not functional)
    while (gpio_get(pin_clock) != 0);
    gpio_put(pin_led, false);
    sleep_ms(300);
    printf("Starting done.\n");

    stdio_init_all();

    gpio_init(pin_data);
    gpio_set_dir(pin_data, GPIO_IN);
    gpio_pull_down(pin_data);

    gpio_init(pin_clock);
    gpio_set_dir(pin_clock, GPIO_IN);
    gpio_pull_down(pin_clock);

// repeating timer to blink internal led (now turned off so I can turn it on by pressing X)
    //struct repeating_timer timer;
    //add_repeating_timer_ms(500, repeating_timer_callback, NULL, &timer);

// irq to detect clock pulses
    gpio_set_irq_enabled_with_callback(pin_clock, 0x04, 1, falling_callback);

// main loop
    while(1){
        tight_loop_contents();
    }
}

/*-------------------------//
// NOTES FOR MY SILLY HEAD //
//-------------------------//

line41, interrupt handler info: printf("Interrupt occured at pin %d with event %d\n", gpio, events);
line68, key name and keycode info: printf("%s %s\n", key.c_str(), hex.c_str());
line121, rising callback: gpio_set_irq_enabled_with_callback(pin_clock, 0x08, 1, rising_callback);
size of an array: int len = sizeof(arr_data)/sizeof(arr_data[0]);

*/
