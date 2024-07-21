/*
 * FPMovingAverage.cpp
 *
 *  Created on: Jul 4, 2024
 *      Author: admin
 */

#include "FPMovingAverage.h"
#include <cmath>

uint32_t FP_MovingAverage::proceed(const uint32_t x)
{
	baseProceed(x, m_y, m_y_state);
	return m_y;
}
