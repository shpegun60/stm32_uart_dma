/*
 * FPMovingAverageBase.h
 *
 *  Created on: Jul 4, 2024
 *      Author: admin
 */

#ifndef TOOLS_MATH_FPMOVINGAVERAGEBASE_H_
#define TOOLS_MATH_FPMOVINGAVERAGEBASE_H_

#include <cstdint>

class FP_MovingAverageBase
{
public:
	FP_MovingAverageBase() = default;
	virtual ~FP_MovingAverageBase() = default;

	bool init(const float alpha, const uint8_t Q);
	bool inline setAlpha(const float alpha) {return init(alpha, m_Q);}

	void inline baseProceed(const uint32_t x, uint32_t& last, uint32_t& last_shifted)
	{
		const int32_t val = (x - last) * m_alpha_scaled;
		last_shifted += val >> m_diff_b;
		last = last_shifted >> m_N;
	}

private:
	uint8_t m_Q = 0;
	uint8_t m_N = 0;
	uint8_t m_diff_b = 0;
	float m_alpha = 0.f;
	uint32_t m_alpha_scaled = 0;
};

#endif /* TOOLS_MATH_FPMOVINGAVERAGEBASE_H_ */
