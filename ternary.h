#pragma once

#include "unary_binary.h"
#include "stm32f3xx_hal_conf.h"


class TernaryGate : public LogicGate
{
protected:
	TernaryGate(GPIO_TypeDef *p_port, int16_t p_input, int16_t p_output1, int16_t p_output2, int16_t p_output3)
		: m_port(p_port), m_input(p_input), m_output1(p_output1), m_output2(p_output2), m_output3(p_output3)
		{ }

	// function to test against
	virtual bool expected(bool b1, bool b2, bool b3) = 0;

	// check function
	bool check() override {
		bool result = true;

		for (int16_t m_pos1 : m_values) {
			for (int16_t m_pos2 : m_values) {
				for (int16_t m_pos3 : m_values) {
					bool actualResult = test(m_pos1 == 1, m_pos2 == 1, m_pos3 == 1);
					bool expectedResult = expected(m_pos1 == 1, m_pos2 == 1, m_pos3 == 1);

					result &= (actualResult == expectedResult);
				}
			}
		}
		return result;
	}

	virtual bool expected(bool b1, bool b2, bool b3) = 0;

private:
	GPIO_TypeDef *m_port;	// port being used (a, b, or C)
	int16_t m_input;	// the pin receiving data

	// the three pins outputting data
	int16_t m_output1;
	int16_t m_output2;
	int16_t m_output3;

	// test function
	bool test(bool p_pin1, bool p_pin2, bool p_pin3) {
		// create pin states
		GPIO_PinState m_pin1 = (p_pin1 ? GPIO_PIN_SET : GPIO_PIN_RESET);
		GPIO_PinState m_pin2 = (p_pin2 ? GPIO_PIN_SET : GPIO_PIN_RESET);
		GPIO_PinState m_pin3 = (p_pin3 ? GPIO_PIN_SET : GPIO_PIN_RESET);

		/* run the general test */
		HAL_GPIO_WritePin(m_port, m_output1, m_pin1);
		HAL_Delay(10);	// minimum delay to avoid serial write issues

		HAL_GPIO_WritePin(m_port, m_output2, m_pin2);
		HAL_Delay(10);

		HAL_GPIO_WritePin(m_port, m_output3, m_pin3);
		HAL_Delay(10);

		// read the output from the chip
		return (HAL_GPIO_ReadPin(m_port, m_input) == GPIO_PIN_SET);
	}
};

class Ternary_AND : public TernaryGate {
public:
	Ternary_AND() { } // needs to be filled in

	// set up the expected function
	virtual bool expected(bool b1, bool b2, bool b3) override { return (b1 && b2 && b3); }	// override original definition of = 0.

};

class Ternary_OR : public TernaryGate {
public:
	Ternary_OR() { } // needs to be filled in

	// set up the expected function
	virtual bool expected(bool b1, bool b2, bool b3) override { return (b1 || b2 || b3); }
};

class Ternary_NAND : public TernaryGate {
public:
	Ternary_NAND() { } // needs to be filled in

	// set up expected function
	virtual bool expected(bool b1, bool b2, bool b3) override { return !(b1 && b2 && b3); }
};

class Ternary_NOR : public TernaryGate {
public:
	Ternary_NOR() { } // needs to be filled in

	// set up expected function
	virtual bool expected(bool b1, bool b2, bool b3) override { return !(b1 || b2 || b3); }
};

class Ternary_XOR : public TernaryGate {
public:
	Ternary_XOR() { } // needs to be filled in

	// set up expected function
	virtual bool expected(bool b1, bool b2, bool b3) override { return (b1 ^ b2 ^ b3); }
};
