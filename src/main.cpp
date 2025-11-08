#include <iostream>
#include <vector>
#include <string.h>
#include "pico/stdlib.h"
#include "quaternary.h"
#include "ternary.h"
#include "binary.h"
#include "unary.h"
#include "hardware/uart.h"
using namespace std;

/* Definitions */
#define BUFSIZE 32
char serbuf[BUFSIZE];
int seridx = 0;
int newline_seen = 0;
/* Functions used in main.cpp */
void config_outputs(vector<uint32_t> outputs);
void config_inputs(vector<uint32_t> inputs);
void init_uart();
void init_uart_irq();
void uart_rx_handler();
bool cmd_logic(int argc, char **argv);

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


/* UART stuff :) */
void init_uart() { 
    uart_init(uart0, 115200); 
    gpio_set_function(0, GPIO_FUNC_UART); // TX
    gpio_set_function(1, GPIO_FUNC_UART); // RX

}

// set up interrupt
void init_uart_irq() {
    // disable FIFO
    uart_set_fifo_enabled(uart0, false);

    // mask the interrupt
    uart0_hw->imsc = (1 << 4); 
    irq_set_exclusive_handler(UART0_IRQ, uart_rx_handler);
    
    irq_set_enabled(UART0_IRQ, true);
}

// set up interrupt handler
void uart_rx_handler() {
    // Acknowledge Interrupt
    uart0_hw->icr = 1 << 4;

    if (seridx == BUFSIZE) { return; }
    
    char c = uart0_hw->dr;      // get character

    // check for what the character is
    if (c == 0x0A) { newline_seen = 1; }
    if ((c == 0x8) && (seridx > 0)) {
        putchar(c);     // backspace
        putchar(0x20);  // space
        putchar(c);     // backspace
        seridx--;   
        serbuf[seridx] = '\0';
        return;
    }

    // Otherwise
    serbuf[seridx] = c;
    uint8_t *serbuffer = (uint8_t *)serbuf;
    uart_write_blocking(uart0, &serbuffer[seridx], 1);
    seridx++;
}

// edit read and write to be able to use command line 
extern "C" int _write(int handle, const char *buffer, int length) {
    (void)handle;
    uart_write_blocking(uart0,
                        reinterpret_cast<const uint8_t*>(buffer),
                        length);
    return length;
}

extern "C" int _read(int handle, char *buffer, int length) {
    (void)handle;
    while (newline_seen == 0) { sleep_ms(5); }
    newline_seen = 0;
    int n = (seridx < length) ? seridx : length;
    memcpy(buffer, serbuf, n);   // optional: buffer[n] = '\0';
    seridx = 0;
    return n;
}


/* logic commands:
*   > logic {gate} {inputs}
*/
bool cmd_logic(int argc, char **argv) {
    bool result = false;
    string cmd = argv[0];
    if (cmd != "logic") {
        cout << "You must start with 'logic'. Try again." << endl; 
        return result;
    }
    if (argc != 3) {
        cout << "argc needs to be 3." << endl;
        cout << "Logic {Gate} {Inputs}." << endl;
        return result;
    }

    // let's start! 
    string gateType = argv[1];
    string inputNum = argv[2];
    LogicGate *gate;
    // NOT Gate
    if (gateType == "NOT") {
        if (inputNum != "1") {
            cout << "NOT gate can only have 1 input." << endl;
            return result;
        }
        gate = new NOT_Gate();
        result = gate->check();
        delete gate;

        return result;
    }

    // AND Gate
    if (gateType == "AND") {
        if ((inputNum != "2") && (inputNum != "3") && (inputNum != "4")) {
            cout << "AND gate can only have 2,3, or 4 inputs." << endl;
            return result;
        }
        int inputs = stoi(inputNum);
        switch (inputs) {
            case (2): 
                gate = new AND_Gate();
                result = gate->check();
                delete gate;
                break;
            case (3):
                gate = new Ternary_AND();
                result = gate->check();
                delete gate;
                break;
            case (4):
                gate = new Quaternary_AND();
                result = gate->check();
                delete gate;
                break;
        }
    }

    // OR Gate
    else if (gateType == "OR") {
        if ((!(inputNum == "2") && !(inputNum == "3") && !(inputNum == "4"))) {
            cout << "OR gate can only have 2,3, or 4 inputs." << endl;
            return result;
        }
        int inputs = stoi(inputNum);
        switch (inputs) {
            case (2): 
                gate = new OR_Gate();
                result = gate->check();
                delete gate;
                break;
            case (3):
                gate = new Ternary_OR();
                result = gate->check();
                delete gate;
                break;
            case (4):
                gate = new Quaternary_OR();
                result = gate->check();
                delete gate;
                break;
        }
    }

    // NAND Gate
    else if (gateType == "NAND") {
        if ((inputNum != "2") && (inputNum != "3") && (inputNum != "4")) {
            cout << "NAND gate can only have 2,3, or 4 inputs." << endl;
            return result;
        }
        int inputs = stoi(inputNum);
        switch (inputs) { 
            case (2): 
                gate = new NAND_Gate();
                result = gate->check();
                delete gate;
                break;
            case (3):
                gate = new Ternary_NAND();
                result = gate->check();
                delete gate;
                break;
            case (4):
                gate = new Quaternary_NAND();
                result = gate->check();
                delete gate;
                break;
        }
    }

    // NOR Gate
    else if (gateType == "NOR") {
        if ((!(inputNum == "2") && !(inputNum == "3") && !(inputNum == "4"))) {
            cout << "NOR gate can only have 2,3, or 4 inputs." << endl;
            return result;
        }
        int inputs = stoi(inputNum);
        switch (inputs) {
            case (2): 
                gate = new NOR_Gate();
                result = gate->check();
                delete gate;
                break;
            case (3):
                gate = new Ternary_NOR();
                result = gate->check();
                delete gate;
                break;
            case (4):
                gate = new Quaternary_NOR();
                result = gate->check();
                delete gate;
                break;
        }
    }

    // XOR Gate
    else if (gateType == "XOR") {
        if ((!(inputNum == "2") && !(inputNum == "3") && !(inputNum == "4"))) {
            cout << "XOR gate can only have 2,3, or 4 inputs." << endl;
            return result;
        }
        int inputs = stoi(inputNum);
        switch (inputs) {
            case (2): 
                gate = new XOR_Gate();
                result = gate->check();
                delete gate;
                break;
            case (3):
                gate = new Ternary_XOR();
                result = gate->check();
                delete gate;
                break;
            case (4):
                gate = new Quaternary_XOR();
                result = gate->check();
                delete gate;
                break;
        }
    }

    return result;
}
int main(void) {

    // configure GPIO pins
    vector<uint32_t> outputs = {22, 25, OUTPUT_PIN1, OUTPUT_PIN2, OUTPUT_PIN3, OUTPUT_PIN4};    // use these pins as outputs
    vector<uint32_t> inputs = {INPUT_PIN};
    config_outputs(outputs);
    config_inputs(inputs);
    init_uart();
    init_uart_irq();
    ios::sync_with_stdio(true);

    /* Now for the driver code */
    setbuf(stdout, NULL);   // disable buffering for stdout

    
    // Welcome message
    cout << "Welcome to the Logic Tester!" << endl;
    cout << "Enter a command bellow in the form... >logic {GATE} {numInputs}" << endl << endl;
    // Inputs
    int argc;
    char *argv[10];
    char input[100];
    bool result = false;

    for (;;) {
        
        cout << "\r\n>";
        fgets(input, sizeof(input), stdin);
        fflush(stdin);
        // replace new line with null
        size_t j = strcspn(input, "\r\n");
        input[j] = '\0';    // look into this
        
        argc = 0;
        char *token = strtok(input, " ");
        while (token != NULL && argc < 10) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
        }
        argv[argc] = NULL;  // terminate the argv list


        // handle command
        result = cmd_logic(argc, argv);
        if (result) {
            /* Blink Blue LED on RP2350 (#25)*/
            gpio_put(22, false);
            sleep_ms(10);

            gpio_put(25, true);
            sleep_ms(500);
            
            cout << "Nice! Your chip works :). Feel free to try again..." << endl;
        }
           
        else {
            string cmd = argv[0];
            if (cmd != "logic") {}
            else {
            /* Blink Red LED on RP2350 (#22) */
            gpio_put(25, false);
            sleep_ms(10);

            gpio_put(22, true);
            sleep_ms(500); 
            cout << "Chip doesn't work :(. Feel free to try again..." << endl;
            }
        }
        // Welcome message
    cout << "Enter a command bellow in the form... > Logic {GATE} {numInputs}" << endl << endl;
    }
}