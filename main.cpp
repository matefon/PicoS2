#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"
#include "pico/util/queue.h"
#include <queue>
#include <string>
#include "codes.h"
//#include "tinyusb/device/hid.h"

using namespace std;

const uint pin_led = 25;
const uint pin_data = 2;
const uint pin_clock = 3;
const uint count_to = 10;
uint counter = 0;
uint led_value = 0;
uint data_bin = 0;
uint keypress_counter = 0;
queue<int> dataq;
int dataarr[8];
string hex = "";
bool rel = false, spec = false;

void falling_callback(uint gpio, uint32_t events){
    if (counter < count_to){
        counter ++;
        if (gpio_get(pin_data) != 0){
            data_bin = 1;
        } else {
            data_bin = 0;
        }
        dataq.push(data_bin);
        //printf("Interrupt occured at pin %d with event %d\n", gpio, events);
    } else {
        //int len = sizeof(arr_data)/sizeof(arr_data[0]);
        //printf("%d,\n", counter);
        
        // Deleting start bits (end bit is not read, but it is always 1 so who cares)
        dataq.pop();
        for (int i = 0; i < 8; i++){
            dataq.push(dataq.front());
            dataq.pop();
        }
        dataq.pop();

        // Adding the queue to an array in reverse, then deleting the queue
        //while (!dataq.empty()) {
        for (int i = 7; i >= 0; i--){
            dataarr[i] = dataq.front();
            dataq.pop();
        }
        hex = bintohex(dataarr);
        printf("HEX: %s\n", hex.c_str());
        counter = 0;

        if (hex == "F0"){rel = true;} //else {rel = false;}
        if (hex == "E0"){spec = true;} //else {spec = false;}

        if ((hex == "22") && !rel){
            gpio_put(pin_led, true);
        } else if ((hex == "22") && rel){
            gpio_put(pin_led, false);
            rel = false;
        }
    }
}

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
    while (gpio_get(pin_clock) != 0);
    gpio_put(pin_led, false);
    sleep_ms(500);
    printf("Starting done.\n");

    stdio_init_all();

    gpio_init(pin_data);
    gpio_set_dir(pin_data, GPIO_IN);
    gpio_pull_down(pin_data);

    gpio_init(pin_clock);
    gpio_set_dir(pin_clock, GPIO_IN);
    gpio_pull_down(pin_clock);

    //struct repeating_timer timer;
    //add_repeating_timer_ms(500, repeating_timer_callback, NULL, &timer);

    gpio_set_irq_enabled_with_callback(pin_clock, 0x04, 1, falling_callback);
    //gpio_set_irq_enabled_with_callback(pin_clock, 0x08, 1, rising_callback);

    while(1){
        tight_loop_contents();
    }
}
