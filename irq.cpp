// TODO
// 1. make a timer to reset data and counter every 0.1 secs, with irq priority 2
// 2. set handler irq priority to 1
// 3. mask the input
// 4. check start, stop and parity

#include <iostream>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

static char event_str[128];
const uint LED_PIN = PICO_DEFAULT_LED_PIN;
const uint CLK_PIN = 2;
const uint DATA_PIN = 3;
unsigned int DATA = 0x00;
int d;
//int DATA_PARITY_MASK = 0x00;
unsigned DATA_MASK = 0x1ef;
int count = 0;

void gpio_event_string(char *buf, uint32_t events);

void gpio_callback(uint gpio, uint32_t events) {
    gpio_event_string(event_str, events);
    d = gpio_get(DATA_PIN) == true ? 1 : 0;
    DATA = (DATA << 1) | (d << 0);
    std::cout << count++ << ":GPIO " << gpio << ": [" << d << "]" << event_str << std::endl;
    gpio_put(LED_PIN, gpio_get(DATA_PIN));
    if (count == 12){
        std::cout << std::endl << "[" << DATA << "] received" << std::endl;
    	count = 0;
    	DATA = 0x00;
    }
    //printf("GPIO %d %x\n", gpio, DATA);
}

int main(void) {
    stdio_init_all();
    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);    
    
    gpio_init(DATA_PIN);
    gpio_set_dir(DATA_PIN, GPIO_IN);

    gpio_init(15);
    gpio_set_dir(15, GPIO_OUT); 
    
    gpio_put(15, 1);
    
    sleep_ms(2000);
    std::cout << "Hello GPIO IRQ" << std::endl;
    gpio_set_irq_enabled_with_callback(CLK_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Wait forever
    while (1);
}


static const char *gpio_irq_str[] = {
        "LEVEL_LOW",  // 0x1
        "LEVEL_HIGH", // 0x2
        "EDGE_FALL",  // 0x4
        "EDGE_RISE"   // 0x8
};

void gpio_event_string(char *buf, uint32_t events) {
    for (uint i = 0; i < 4; i++) {
        uint mask = (1 << i);
        if (events & mask) {
            // Copy this event string into the user string
            const char *event_str = gpio_irq_str[i];
            while (*event_str != '\0') {
                *buf++ = *event_str++;
            }
            events &= ~mask;

            // If more events add ", "
            if (events) {
                *buf++ = ',';
                *buf++ = ' ';
            }
        }
    }
    *buf++ = '\0';
}
