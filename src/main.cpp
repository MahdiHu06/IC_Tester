#include <iostream>
#include <vector>
#include "pico/stdlib.h"
#include "quaternary.h"
#include "ternary.h"
#include "binary.h"
#include "unary.h"

using namespace std;

/* Functions used in main.cpp */
void config_outputs(vector<uint32_t> outputs);
void config_inputs(vector<uint32_t> inputs);


/* Start Code */
void config_outputs(vector<uint32_t> pins) {
    for (auto currPin : pins) {
        gpio_init(currPin); // initialize GPIO
        gpio_set_dir(currPin, GPIO_OUT);  // set to output
        sleep_ms(10);

        gpio_put(currPin, false);
    }
}

void config_inputs(vector<uint32_t> pins) {
    for (auto currPin : pins) {
        gpio_init(currPin);
        gpio_set_dir(currPin, GPIO_IN);

        gpio_put(currPin, false); // turn pin off
    }
}
int main(void) {

    // configure GPIO pins
    vector<uint32_t> outputs = {22, 23, 24, 25, OUTPUT_PIN1, OUTPUT_PIN2, OUTPUT_PIN3, OUTPUT_PIN4};    // use these pins as outputs
    vector<uint32_t> inputs = {INPUT_PIN};
    config_outputs(outputs);
    config_inputs(inputs);
}