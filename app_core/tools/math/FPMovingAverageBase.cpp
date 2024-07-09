/*
 * FPMovingAverageBase.cpp
 *
 *  Created on: Jul 4, 2024
 *      Author: admin
 */

#include "FPMovingAverageBase.h"
#include <cmath>


bool FP_MovingAverageBase::init(const float alpha, const uint8_t Q)
{
	const int N = -std::log2(alpha);

	if((Q + N) > 32 || N < 0) {
		return false;
	}

	m_N = N;
	m_Q = Q;
	m_diff_b = (m_Q - m_N);
	m_alpha = alpha;
	m_alpha_scaled = static_cast<uint32_t>(static_cast<float>(1 << Q) * alpha);

	return true;
}

