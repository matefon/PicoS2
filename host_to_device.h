#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h" // used to access gpio
#include <string> // enable strings
#include "codes.h" // bintohex and hextobin functions

using namespace std;

// VARIABLES
const uint pin_data = 2; // PS/2 keyboard data pin
const uint pin_clock = 3; // PS/2 clock pin, data sending is still using the device clock
const uint data_pulldown = 4; // data pulldown transistor pin, send data
const uint clock_pulldown = 5; // clock pulldown transistor pin, pull clock down to begin sending data

uint counter = 0; // bit counter

void start_com(){
    printf("Starting communication...\n");
    gpio_put(clock_pulldown, true);
    sleep_ms(0.1);
    gpio_put(data_pulldown, true);
    sleep_ms(0.1);
    gpio_put(clock_pulldown, false);
}

void send_bit(string input){

}

// unlike my first attempt, this code doesn't use an irq callback, so it can be called by the main code while running
void send_to_device(string hex1, string hex2 = ""){ // hex2 default empty
// get string containing the binary number converted from hex
    string bin1 = hextobin(hex1);
// get string length
    int len1 = bin1.length();
// do the same for hex2 if not empty
    if (hex2 != ""){
        string bin2 = hextobin(hex2);
        int len2 = bin2.length();
        /*char bin2_array[len2 + 1];
        strcpy(bin2_array, bin2.c_str());*/
    }
// send code
    start_com();
    for (int i = 0; i < len1; i++){
        send_bit(bin1[i]);
    }
    if (hex2 != ""){
        start_com();
        for (int i = 0; i < len2; i++){
            send_bit(bin2[i]);
        }
    }
}