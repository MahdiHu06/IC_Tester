#include <iostream>
#include <vector>
#include "pico/stdlib.h"
using namespace std;

/* Functions used in main.cpp */
void config_outputs(vector<uint32_t> outputs);
void config_inputs(vector<uint32_t> inputs);


/* Start Code */
void config_outputs(vector<uint32_t> pins) {
    for (auto currPin : pins) {
        gpio_init(currPin); // initialize GPIO
        gpio_set_dir(currPin, GPIO_OUT);  // set to output
        
        sio_hw->gpio_clr = currPin; // turn pin off !!
    }
}

void config_inputs(vector<uint32_t> pins) {
    for (auto currPin : pins) {
        gpio_init(currPin);
        gpio_set_dir(currPin, GPIO_IN);

        sio_hw->gpio_clr = currPin; // turn pin off
    }
}
int main(void) {

    // configure GPIO pins
    vector<uint32_t> outputs = {22, 23, 24, 25};    // use these pins as outputs
    vector<uint32_t> inputs = {};
    config_outputs(outputs);
    config_inputs(inputs);
}