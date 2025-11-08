#pragma once

/* Includes */
#include "pico/stdlib.h"
//////////////////////////////////////////

/* PINS USED FOR INPUT AND OUTPUT */
#define INPUT_PIN 16
#define OUTPUT_PIN1 17
#define OUTPUT_PIN2 18
#define OUTPUT_PIN3 19
#define OUTPUT_PIN4 20
//////////////////////////////////////////
/* Gates that can be chosen */
enum class GateType {
    NOT = 0,
    AND = 1,
    OR = 2,
    NAND = 3,
    NOR = 4,
    XOR = 5,
    LAST = XOR
};

/* Original Class */
class LogicGate {
public:
    // keep each check unique to respective gate
    virtual bool check() = 0;
    virtual ~LogicGate() {} // destructor
protected:
    int16_t m_values[2];    // possible values (0, 1)
    LogicGate() : m_values{ 0, 1 } {}
};

/* NOT gate is Unary */
class UnaryGate : public LogicGate {
protected:
    UnaryGate(uint16_t p_input, uint16_t p_outPin)
    : LogicGate(), m_input(p_input), m_output(p_outPin)
    {}

    // define the 'check' function for NOT
    virtual bool expected(bool b1) = 0; // virtual abstract so each function has own unique expected cases

    bool check() override {
        bool result = true;

        // possible results
        for (uint16_t m_pos : m_values) {
            bool actualRslt = test(m_pos);
            bool expectedRslt = expected(m_pos == 1);

            // if result goes false it stays false
            result &= (expectedRslt == actualRslt);
        }
        return result;
    }
private:
    uint16_t m_input;   // input pin
    uint16_t m_output;  // output pin

    /* Test function */
    bool test(uint16_t p_pin) { // pin is expected pin
        bool m_pinState = p_pin ? true : false;
        gpio_put(m_output, m_pinState);
        
        return (gpio_get(m_input) != 0);  // 0 for low and non-zero for high
    }
};

class NOT_Gate : public UnaryGate {
public:
	// Constructor
	NOT_Gate() : UnaryGate(INPUT_PIN, OUTPUT_PIN1) {}

	// set up expected to be used in check
	virtual bool expected(bool b1) override { return !b1; }
};