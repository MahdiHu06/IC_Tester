#pragma once
#include "pico/stdlib.h"
#include "unary.h"

class QuaternaryGate : public LogicGate {
protected:
    QuaternaryGate(uint16_t p_input, uint16_t p_output1, uint16_t p_output2, uint16_t p_output3, uint16_t p_output4) 
    : m_input(p_input), m_output1(p_output1), m_output2(p_output2) , m_output3(p_output3), m_output4(p_output4)
    {}

        // check expected vs. actual
        virtual bool expected(bool b1, bool b2, bool b3, bool b4) = 0;
        bool check() override {
            bool result = true;

            // all possible results
            for (int16_t m_pos1 : m_values) {
                for (int16_t m_pos2 : m_values) {
                    for (int16_t m_pos3 : m_values) {
                        for (int16_t m_pos4 : m_values) {    
                            bool actualRslt = test(m_pos1, m_pos2, m_pos3, m_pos4);
                            bool expectedRslt = expected(m_pos1 == 1, m_pos2 == 1, m_pos3 == 1, m_pos4 == 1);

                            result &= (actualRslt == expectedRslt);
                        }
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
    uint16_t m_output4;

    bool test(uint16_t p_pin1, uint16_t p_pin2, uint16_t p_pin3, uint16_t p_pin4) {
        bool m_pinState1 = p_pin1 ? true : false;
        bool m_pinState2 = p_pin2 ? true : false;
        bool m_pinState3 = p_pin3 ? true : false;
        bool m_pinState4 = p_pin4 ? true : false;

        gpio_put(m_output1, m_pinState1);
        sleep_ms(10);

        gpio_put(m_output2, m_pinState2);
        sleep_ms(10);

        gpio_put(m_output3, m_pinState3);
        sleep_ms(10);

        gpio_put(m_output4, m_pinState4);
        sleep_ms(10);

        // check the input pin
        return (gpio_get(m_input) != 0);
    }
};

class Quaternary_AND : public QuaternaryGate {
public:
    Quaternary_AND() :
    QuaternaryGate(INPUT_PIN, OUTPUT_PIN1, OUTPUT_PIN2, OUTPUT_PIN3, OUTPUT_PIN4)
    {}
    virtual bool expected(bool b1, bool b2, bool b3, bool b4) override { return (b1 && b2 && b3 && b4); }
};

class Quaternary_OR : public QuaternaryGate {
public:
    Quaternary_OR() :
    QuaternaryGate(INPUT_PIN, OUTPUT_PIN1, OUTPUT_PIN2, OUTPUT_PIN3, OUTPUT_PIN4)
    {}
    virtual bool expected(bool b1, bool b2, bool b3, bool b4) override { return (b1 || b2 || b3 || b4); }
};

class Quaternary_NAND : public QuaternaryGate {
public:
    Quaternary_NAND() :
    QuaternaryGate(INPUT_PIN, OUTPUT_PIN1, OUTPUT_PIN2, OUTPUT_PIN3, OUTPUT_PIN4)
    {}
    virtual bool expected(bool b1, bool b2, bool b3, bool b4) override { return (!(b1 && b2 && b3 && b4)); }
};

class Quaternary_NOR : public QuaternaryGate {
public:
    Quaternary_NOR() :
    QuaternaryGate(INPUT_PIN, OUTPUT_PIN1, OUTPUT_PIN2, OUTPUT_PIN3, OUTPUT_PIN4)
    {}
    virtual bool expected(bool b1, bool b2, bool b3, bool b4) override { return (!(b1 || b2 || b3 || b4)); }
};

class Quaternary_XOR : public QuaternaryGate {
public:
    Quaternary_XOR() :
    QuaternaryGate(INPUT_PIN, OUTPUT_PIN1, OUTPUT_PIN2, OUTPUT_PIN3, OUTPUT_PIN4)
    {}
    virtual bool expected(bool b1, bool b2, bool b3, bool b4) override { return (b1 ^ b2 ^ b3 ^ b4); }
};