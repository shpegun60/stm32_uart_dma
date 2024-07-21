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

bool FP_MovingAverageArray::init(const u8 n, const f32 alpha, const u8 Q)
{
	if(!FP_MovingAverageBase::init(alpha, Q)) {
		return false;
	}

	m_y_states = new u32[n];
	m_y = new u32[n];
	m_n = n;

	return true;
}

void FP_MovingAverageArray::proceed(const u16* const values)
{
	for(u8 i = 0; i != m_n; ++i) {
		FP_MovingAverageBase::proceed(values[i], m_y[i], m_y_states[i]);
	}
}
