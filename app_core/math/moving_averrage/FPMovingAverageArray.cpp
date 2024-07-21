/*
 * FPMovingAverageArray.cpp
 *
 *  Created on: Jul 4, 2024
 *      Author: admin
 */

#include "FPMovingAverageArray.h"

FP_MovingAverageArray::~FP_MovingAverageArray()
{
	// TODO Auto-generated destructor stub
	delete[] m_y_states;
	delete[] m_y;
}

void FP_MovingAverageArray::allocArrays(const uint8_t n)
{
	m_y_states = new uint32_t[n];
	m_y = new uint32_t[n];
	m_n = n;
}

void FP_MovingAverageArray::proceed(uint16_t* const values)
{
	uint8_t n = m_n;
	uint16_t* x_ptr = values;
	uint32_t* y_ptr = m_y;
	uint32_t* y_state_ptr = m_y_states;

	while(n) {
		baseProceed(*x_ptr, *y_ptr, *y_state_ptr);

		++x_ptr;
		++y_ptr;
		++y_state_ptr;
		--n;
	}
}
