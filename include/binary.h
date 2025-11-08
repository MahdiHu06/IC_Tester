#pragma once
#include "unary.h"

class BinaryGate : public LogicGate {
protected:
    BinaryGate(uint16_t p_input, uint16_t p_output1, uint16_t p_output2) 
    : m_input(p_input), m_output1(p_output1), m_output2(p_output2) 
    {}

        // check expected vs. actual
        virtual bool expected(bool b1, bool b2) = 0;
        bool check() override {
            bool result = true;

            // all possible results
            for (int16_t m_pos1 : m_values) {
                for (int16_t m_pos2 : m_values) {
                    bool actualRslt = test(m_pos1, m_pos2);
                    bool expectedRslt = expected(m_pos1 == 1, m_pos2 == 1);

                    result &= (actualRslt == expectedRslt);
                }
            }
            return result;
        }
private:
    uint16_t m_input;
    uint16_t m_output1;
    uint16_t m_output2;

    bool test(uint16_t p_pin1, uint16_t p_pin2) {
        bool m_pinState1 = p_pin1 ? true : false;
        bool m_pinState2 = p_pin2 ? true : false;

        gpio_put(m_output1, m_pinState1);
        sleep_ms(10);

        gpio_put(m_output2, m_pinState2);
        sleep_ms(10);

        // check the input pin
        return (gpio_get(m_input) != 0);
    }
};

class AND_Gate : public BinaryGate {
public:
    AND_Gate() : BinaryGate(INPUT_PIN, OUTPUT_PIN1, OUTPUT_PIN2) {}
    bool expected(bool b1, bool b2) override { return (b1 && b2); }
};

class OR_Gate : public BinaryGate {
public:
    OR_Gate() : BinaryGate(INPUT_PIN, OUTPUT_PIN1, OUTPUT_PIN2) {}
    bool expected(bool b1, bool b2) override { return (b1 || b2); }
};

class NAND_Gate : public BinaryGate {
public:
    NAND_Gate() : BinaryGate(INPUT_PIN, OUTPUT_PIN1, OUTPUT_PIN2) {}
    bool expected(bool b1, bool b2) override { return (!(b1 && b2)); }
};

class NOR_Gate : public BinaryGate {
public:
    NOR_Gate() : BinaryGate(INPUT_PIN, OUTPUT_PIN1, OUTPUT_PIN2) {}
    bool expected(bool b1, bool b2) override { return (!(b1 || b2)); }
};

class XOR_Gate : public BinaryGate {
public:
    XOR_Gate() : BinaryGate(INPUT_PIN, OUTPUT_PIN1, OUTPUT_PIN2) {}
    bool expected(bool b1, bool b2) override { return (b1 ^ b2); }
};