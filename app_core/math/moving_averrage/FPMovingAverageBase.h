/*
 * FPMovingAverageBase.h
 *
 *  Created on: Jul 4, 2024
 *      Author: admin
 */

#ifndef TOOLS_MATH_FPMOVINGAVERAGEBASE_H_
#define TOOLS_MATH_FPMOVINGAVERAGEBASE_H_

#include "my_cpp_types.h"

class FP_MovingAverageBase
{
public:
	FP_MovingAverageBase() = default;
	virtual ~FP_MovingAverageBase() = default;

	bool init(const float alpha, const u8 Q);
	bool inline setAlpha(const float alpha) {return init(alpha, m_Q);}

	inline void proceed(const u32 x, u32& last, u32& last_shifted)
	{
		const int32_t val = (x - last) * m_alpha_scaled;
		last_shifted += val >> m_diff_b;
		last = last_shifted >> m_N;
	}

private:
	u8 m_Q = 0;
	u8 m_N = 0;
	u8 m_diff_b = 0;
	f32 m_alpha = 0.f;
	u32 m_alpha_scaled = 0;
};

#endif /* TOOLS_MATH_FPMOVINGAVERAGEBASE_H_ */
