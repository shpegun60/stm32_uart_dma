/*
 * Timerms.cpp
 *
 *  Created on: May 21, 2024
 *      Author: admin
 */

#include "ProgramTimer.h"


ProgramTimer::~ProgramTimer() {
	// TODO Auto-generated destructor stub
}

void ProgramTimer::proceed(const uint32_t current_time)
{
	const uint32_t dt = current_time - m_last_time;

	if(dt < m_dt) {
		return;
	}

	if(m_event) {
		m_event(current_time, dt);
	}

	m_last_time = current_time;
}
