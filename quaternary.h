#pragma once

#include "unary_binary.h"
#include "stm32f3xx_hal_conf.h"

class quaternaryGate : public LogicGate
{
protected:
	quaternaryGate(GPIO_TypeDef *p_port, int16_t p_input, int16_t p_output1, int16_t p_output2, int16_t p_output3, int16_t p_output4)
	: m_port(p_port), m_input(p_input), m_output1(p_output1), m_output2(p_output2), m_output3(p_output3), m_output4(p_output4)
	{ }

	// the gate's check function
	bool check() override {
		bool result = true;

		// start checking
		for (int16_t m_pos1 : m_values) {
			for (int16_t m_pos2 : m_values) {
				for (int16_t m_pos3 : m_values) {
					for (int16_t m_pos4 : m_values) {
						bool expectedResult = expected(m_pos1 == 1, m_pos2 == 1, m_pos3 == 1, m_pos4 == 1);
						bool actualResult = test(m_pos1 == 1, m_pos2 == 1, m_pos3 == 1, m_pos4 == 1);

						result &= (expectedResult == actualResult);
					}
				}
			}
		}
		return result;
	}
	//expected function
	virtual bool expected(bool p_b1, bool p_b2, bool p_b3, bool p_b4) = 0;	// must be redefined later

private:
	GPIO_TypeDef *m_port;	// port being used (A, B, or C)
	int16_t m_input;	// pin to read from

	// pins to output from during test
	int16_t m_output1;
	int16_t m_output2;
	int16_t m_output3;
	int16_t m_output4;

	// testing function
	bool test(bool p_b1, bool p_b2, bool p_b3, bool p_b4) {
		// create GPIO states
		GPIO_PinState m_pin1 = (p_b1 ? GPIO_PIN_SET : GPIO_PIN_RESET);
		GPIO_PinState m_pin2 = (p_b2 ? GPIO_PIN_SET : GPIO_PIN_RESET);
		GPIO_PinState m_pin3 = (p_b3 ? GPIO_PIN_SET : GPIO_PIN_RESET);
		GPIO_PinState m_pin4 = (p_b4 ? GPIO_PIN_SET : GPIO_PIN_RESET);

		// testing logic
		HAL_GPIO_WritePin(m_port, m_output1, m_pin1);
		HAL_Delay(10); // minimum delay

		HAL_GPIO_WritePin(m_port, m_output2, m_pin2);
		HAL_Delay(10);

		HAL_GPIO_WritePin(m_port, m_output3, m_pin3);
		HAL_Delay(10);

		HAL_GPIO_WritePin(m_port, m_output4, m_pin4);
		HAL_Delay(10);

		return (HAL_GPIO_ReadPin(m_port, m_input) == GPIO_PIN_SET);
	}
};

// AND gate
class quaternary_AND : public quaternaryGate {
public:
	quaternary_AND() : quaternaryGate(GPIOB, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8)
	{ }

	virtual bool expected(bool p_b1, bool p_b2, bool p_b3, bool p_b4) override { return (p_b1 && p_b2 && p_b3 && p_b4); }
};

// OR Gate
class quaternary_OR : public quaternaryGate {
public:
	quaternary_OR() : quaternaryGate(GPIOB, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8)
	{ }

	virtual bool expected(bool p_b1, bool p_b2, bool p_b3, bool p_b4) override { return (p_b1 || p_b2 || p_b3 || p_b4); }
};

// NAND gate
class quaternary_NAND : public quaternaryGate {
public:
	quaternary_NAND() : quaternaryGate(GPIOB, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8)
	{ }

	virtual bool expected(bool p_b1, bool p_b2, bool p_b3, bool p_b4) override { return (!(p_b1 && p_b2 && p_b3 && p_b4)); }
};

// NOR gate
class quaternary_NOR : public quaternaryGate {
public:
	quaternary_NOR() : quaternaryGate(GPIOB, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8)
	{ }

	virtual bool expected(bool p_b1, bool p_b2, bool p_b3, bool p_b4) override { return (!(p_b1 || p_b2 || p_b3 || p_b4)); }
};

// XOR gate
class quaternary_XOR : public quaternaryGate {
public:
	quaternary_XOR() : quaternaryGate(GPIOB, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8)
	{ }

	virtual bool expected(bool p_b1, bool p_b2, bool p_b3, bool p_b4) override { return (p_b1 ^ p_b2 ^ p_b3 ^ p_b4); }
};
