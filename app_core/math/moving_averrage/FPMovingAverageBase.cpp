/*
 * FPMovingAverageBase.cpp
 *
 *  Created on: Jul 4, 2024
 *      Author: admin
 */

#include "FPMovingAverageBase.h"
#include <cmath>


bool FP_MovingAverageBase::init(const float alpha, const u8 Q)
{
	const int N = -std::log2(alpha);

	if(N < 0 || static_cast<u32>(Q + N) > sizeof(u32)) {
		return false;
	}

	m_N = N;
	m_Q = Q;
	m_diff_b = (m_Q - m_N);
	m_alpha = alpha;
	m_alpha_scaled = static_cast<u32>(static_cast<float>(1 << Q) * alpha);

	return true;
}

