#pragma once

// includes
#include "stm32f3xx_hal_conf.h"

// Parent Class
class LogicGate {
public:
	// keep each check individual
	virtual bool check() = 0;

protected:
	uint16_t m_values[2];	// possible values (0, 1).

	// Constructor
	LogicGate() : m_values{ 0, 1 } { }
};

// for NOT Gate

class UnaryLogicGate : public LogicGate {
protected:
	UnaryLogicGate(GPIO_TypeDef *p_port, uint16_t p_input, uint16_t p_output)
	: LogicGate(), m_port(p_port), m_input(p_input), m_output(p_output)
	{ }

	// define the 'check' function (Used for NOT)
	bool check() override {
		bool result = true;

		// iterate through the possible results
		for (uint16_t m_pos : m_values) {
			auto actual = test(m_pos);
			auto expected = expected(m_pos == 1);

			result &= (expected == actual);	// if false at any point it will remain false
		}

		return result;
	}

	virtual bool expected(bool b1) = 0;		// set up as virtual abstract in order for each function to have unique expected cases. 

	// testing function
private:
	GPIO_TypeDef *m_port;
	uint16_t m_input;
	uint16_t m_output;
	
	// test
	bool test(GPIO_PinState p_pin) {
		HAL_GPIO_WritePin(m_port, m_output, p_pin);
		HAL_Delay(10);	// minimum delay
		
		return (HAL_GPIO_ReadPin(m_port, m_input) == GPIO_PIN_SET);
	}
};


class BinaryLogicGate : public LogicGate {

protected:
	BinaryLogicGate(GPIO_TypeDef *p_port, uint16_t p_input, uint16_t p_output1, uint16_t p_output2)
	: m_port(p_port), m_input(p_input), m_output1(p_output1), m_output2(p_output2)
	{ }

	// checks expected V.S. actual
	bool check() override {
		bool result = true;

		// iterate through the possible results
		for (uint16_t m_pos1 : m_values) {
			for(uint16_t m_pos2 : m_values) {
				auto actual = test(m_pos1, m_pos2);
				auto expected = expected(m_pos1 == 1, m_pos2 == 1);

				result &= (expected == actual);
			}
		}

		return result;
	}
	
	virtual bool expected(bool b1, bool b2) = 0;	// set up as virtual abstract in order to be used in each function independently
private:
	GPIO_TypeDef *m_port;
	uint16_t m_input;
	uint16_t m_output1;
	uint16_t m_output2;
	
	// tests actual chip
	bool test(GPIO_PinState p_pin1, GPIO_PinState p_pin2){
		// run the general test
		HAL_GPIO_WritePin(m_port, p_output1, p_pin1);
		HAL_Delay(10);	// minimum delay to avoid serial write issues

		HAL_GPIO_WritePin(m_port, p_output2, p_pin2);
		HAL_Delay(10);

		return (HAL_GPIO_ReadPin(m_port, m_input) == GPIO_PIN_SET);
	}


};


/* Individual Gate Classes */

class NOT_Gate : public UnaryLogicGate {
public:
	// Constructor
	NOT_Gate() : UnaryLogicGate(GPIOB, GPIO_PIN_5, GPIO_PIN_6) { }
	
	// set up expected to be used in check
	virtual bool expected(bool b1) override { return !b1; }
};

class AND_Gate : public BinaryLogicGate {
public:
	// Constructor
	AND_Gate() : BinaryLogicGate(GPIOB, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_4) { }
	
	// set up expected to be used in check
	bool expected(bool b1, bool b2) override { return b1 && b2; } // override original
};


class OR_Gate : public BinaryLogicGate {
public:
	// constructor
	OR_Gate() : BinaryLogicGate(GPIOB, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_4) { }
	
	// set up expected to be used in check
	bool expected(bool b1, bool b2) override { return b1 || b2; }
};

class NAND_Gate : public BinaryLogicGate {
public:
	// Constructor
	NAND_Gate() : BinaryLogicGate(GPIOB, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_4) { }
	
	// Set up expected to be used in check
	bool expected(bool b1, bool b2) override { return !(b1 && b2); }
};

class NOR_Gate : public BinaryLogicGate {
public:
	// Constructor
	NOR_Gate() : BinaryLogicGate(GPIOB, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_4) { }
	
	//Set up expected to be used in check
	bool expected(bool b1, bool b2) override { return !(b1 || b2); }
};

class XOR_Gate : public BinaryLogicGate {
public:
	// Constructor
	XOR_Gate() : BinaryLogicGate(GPIOB, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_4) { }
	
	// Set up expected to be used in check
	bool expected(bool b1, bool b2) override { return b1 ^ b2; }
};
