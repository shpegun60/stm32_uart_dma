/*
 * FPMovingAverage.h
 *
 *  Created on: Jul 4, 2024
 *      Author: admin
 */

#ifndef TOOLS_MATH_FPMOVINGAVERAGE_H_
#define TOOLS_MATH_FPMOVINGAVERAGE_H_

#include "FPMovingAverageBase.h"
#include <cstdint>

class FP_MovingAverage : public FP_MovingAverageBase
{
public:
	FP_MovingAverage() = default;
	~FP_MovingAverage() override final = default;

	uint32_t proceed(const uint32_t value);
private:
	uint32_t m_y_state = 0;
	uint32_t m_y = 0;
};

#endif /* TOOLS_MATH_FPMOVINGAVERAGE_H_ */
