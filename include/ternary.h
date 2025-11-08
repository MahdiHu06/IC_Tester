#pragma once

#include "unary.h"
#include "pico/stdlib.h"

class TernaryGate : public LogicGate {
protected:
    TernaryGate(uint16_t p_input, uint16_t p_output1, uint16_t p_output2, uint16_t p_output3) 
    : m_input(p_input), m_output1(p_output1), m_output2(p_output2) , m_output3(p_output3)
    {}

        // check expected vs. actual
        virtual bool expected(bool b1, bool b2, bool b3) = 0;
        bool check() override {
            bool result = true;

            // all possible results
            for (int16_t m_pos1 : m_values) {
                for (int16_t m_pos2 : m_values) {
                    for (int16_t m_pos3 : m_values) {
                        bool actualRslt = test(m_pos1, m_pos2, m_pos3);
                        bool expectedRslt = expected(m_pos1 == 1, m_pos2 == 1, m_pos3 == 1);

                        result &= (actualRslt == expectedRslt);
                    }
                }
            }
            return result;
        }
private:
    uint16_t m_input;
    uint16_t m_output1;
    uint16_t m_output2;
    uint16_t m_output3;

    bool test(uint16_t p_pin1, uint16_t p_pin2, uint16_t p_pin3) {
        bool m_pinState1 = p_pin1 ? true : false;
        bool m_pinState2 = p_pin2 ? true : false;
        bool m_pinState3 = p_pin3 ? true : false;

        gpio_put(m_output1, m_pinState1);
        sleep_ms(10);

        gpio_put(m_output2, m_pinState2);
        sleep_ms(10);

        gpio_put(m_output3, m_pinState3);
        sleep_ms(10);

        // check the input pin
        return (gpio_get(m_input) != 0);
    }
};

class Ternary_AND : public TernaryGate {
public:
    Ternary_AND() 
    : TernaryGate(INPUT_PIN, OUTPUT_PIN1, OUTPUT_PIN2, OUTPUT_PIN3) 
    {}
    virtual bool expected(bool b1, bool b2, bool b3) override { return (b1 && b2 && b3); }
};

class Ternary_OR : public TernaryGate {
public:
    Ternary_OR() 
    : TernaryGate(INPUT_PIN, OUTPUT_PIN1, OUTPUT_PIN2, OUTPUT_PIN3) 
    {}
    virtual bool expected(bool b1, bool b2, bool b3) override { return (b1 || b2 || b3); }
};

class Ternary_NAND : public TernaryGate {
public:
    Ternary_NAND() 
    : TernaryGate(INPUT_PIN, OUTPUT_PIN1, OUTPUT_PIN2, OUTPUT_PIN3) 
    {}
    virtual bool expected(bool b1, bool b2, bool b3) override { return (!(b1 && b2 && b3)); }
};

class Ternary_NOR : public TernaryGate {
public:
    Ternary_NOR() 
    : TernaryGate(INPUT_PIN, OUTPUT_PIN1, OUTPUT_PIN2, OUTPUT_PIN3) 
    {}
    virtual bool expected(bool b1, bool b2, bool b3) override { return (!(b1 || b2 || b3)); }
};

class Ternary_XOR : public TernaryGate {
public:
    Ternary_XOR() 
    : TernaryGate(INPUT_PIN, OUTPUT_PIN1, OUTPUT_PIN2, OUTPUT_PIN3) 
    {}
    virtual bool expected(bool b1, bool b2, bool b3) override { return (b1 ^ b2 ^ b3); }
};

