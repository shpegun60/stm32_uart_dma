/*
 * FPMovingAverageArray.h
 *
 *  Created on: Jul 4, 2024
 *      Author: admin
 */

#ifndef TOOLS_MATH_FPMOVINGAVERAGEARRAY_H_
#define TOOLS_MATH_FPMOVINGAVERAGEARRAY_H_

#include "FPMovingAverageBase.h"

class FP_MovingAverageArray: public FP_MovingAverageBase
{
public:
	FP_MovingAverageArray() = default;
	~FP_MovingAverageArray();

	void allocArrays(const uint8_t n);
	void proceed(uint16_t* const values);

	inline uint32_t getOut(const uint8_t n) const
	{
		if(n < m_n) {
			return m_y[n];
		}
		return 0;
	}
private:
	uint32_t* m_y_states = nullptr;
	uint32_t* m_y = nullptr;
	uint8_t m_n = 0;
};

#endif /* TOOLS_MATH_FPMOVINGAVERAGEARRAY_H_ */
