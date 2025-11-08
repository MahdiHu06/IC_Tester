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

/* NOTE 
*   NOT if modulo 10 == 1
*   AND if modulo 10 == 2
*   OR if modulo 10 == 3
*   NAND if modulo 10 == 4
*   NOR if modulo 10 == 5
*   XOR if modulo 10 == 6
*/
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
    vector<uint32_t> outputs = {22, 25, OUTPUT_PIN1, OUTPUT_PIN2, OUTPUT_PIN3, OUTPUT_PIN4};    // use these pins as outputs
    vector<uint32_t> inputs = {INPUT_PIN};
    config_outputs(outputs);
    config_inputs(inputs);

    // First let's hard-code some tests...
    uint16_t input = 14;
    auto gateIndex = input % 10;
    bool result = false;

    /* One and Two input gates */   
    if (input / 10 == 1) {
        LogicGate *gates[uint16_t(GateType::LAST) + 1] = {  new NOT_Gate(), 
                                                            new AND_Gate(), 
                                                            new OR_Gate(), 
                                                            new NAND_Gate(), 
                                                            new NOR_Gate(), 
                                                            new XOR_Gate()
        };

        // now for the check
        auto *gate = gates[gateIndex - 1];
        result = gate->check();        // call the checker function

        // delete created variables
        for (LogicGate *g : gates) { delete g; }
    } 
    /* Three input gates */
    else if (input / 10 == 2) {
        LogicGate *gates[uint16_t(GateType::LAST) + 1] = {  new Ternary_AND(), 
                                                            new Ternary_OR(), 
                                                            new Ternary_NAND(), 
                                                            new Ternary_NOR(), 
                                                            new Ternary_XOR()
        };

        // now for the check
        auto *gate = gates[gateIndex - 2];  // minus two because no more not gate
        result = gate->check();        // call the checker function

        // delete created variables
        for (LogicGate *g : gates) { delete g; }
    }

    /* Four input gates */
    else if (input / 10 == 3) {
        LogicGate *gates[uint16_t(GateType::LAST) + 1] = {  new Quaternary_AND(), 
                                                            new Quaternary_OR(), 
                                                            new Quaternary_NAND(), 
                                                            new Quaternary_NOR(), 
                                                            new Quaternary_XOR()
        };

        // now for the check
        auto *gate = gates[gateIndex - 2];  // minus two because no more not gate
        result = gate->check();        // call the checker function

        // delete created variables
        for (LogicGate *g : gates) { delete g; }
    }    

    /* Now for the driver code */
    for (;;) {
        if (result) {
            /* Blink Blue LED on RP2350 (#25)*/
            gpio_put(25, true);
            sleep_ms(500);

            gpio_put(25, false);
            sleep_ms(500);
        }   
        else {
            /* Blink Red LED on RP2350 (#22) */
            gpio_put(22, true);
            sleep_ms(500);

            gpio_put(22, false);
            sleep_ms(500);
        }
    }
}