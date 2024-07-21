/*
 * PID.cpp
 *
 *  Created on: May 21, 2024
 *      Author: admin
 */

#include "PID.h"
#include "math.h"


float PID::update(const float input, const float dt_s)
{
	// The error is the difference between the reference (setpoint) and the
	// actual position (input)
	const float error = m_task - input;

	// The integral or sum of current and previous errors
	const float newIntegral = m_integral + (error * dt_s * m_ki);

	// Compute the difference between the current and the previous input,
	// but compute a weighted average using a factor α ∊ (0,1]
	const float diff = m_emaAlpha * (m_prevInput - input);
	const float derivative = m_kd * (diff / dt_s);

	// Update the average
	m_prevInput -= diff;

	// Standard PID rule
	float output = (m_kp * error) + m_integral + derivative;

	// Clamp and anti-windup
	if (output > m_maxOutput) {
		output = m_maxOutput;
	} else if (output < m_minOutput) {
		output = m_minOutput;
	} else {
		m_integral = newIntegral;
	}

	m_output = output;
	return output;
}



float PID::calcAlphaEMA(float fn)
{
    // α(fₙ) = cos(2πfₙ) - 1 + √( cos(2πfₙ)² - 4 cos(2πfₙ) + 3 )
    const float c = std::cos(2.0 * float(M_PI) * fn);
    return c - 1.0 + std::sqrt(c * c - 4.0 * c + 3.0);
}

