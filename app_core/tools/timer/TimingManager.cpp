/*
 * TimingManager.cpp
 *
 *  Created on: Jun 5, 2024
 *      Author: admin
 */

#include "TimingManager.h"
#include <algorithm>
#include "main.h"

std::vector<TimingManager*> TimingManager::m_timers;

TimingManager::~TimingManager() {

	auto it = std::find(m_timers.begin(), m_timers.end(), this);
	if (it != m_timers.end()) {
		m_timers.erase(it);
	}
}

TimingManager::TimingManager()
{
	m_counter = 0;
	m_timers.push_back(this);
}

TimingManager::TimingManager(const uint32_t delay)
{
	m_counter = delay;
	m_timers.push_back(this);
}



//---------------------------- PUT INVOKING THIS FUNCTION TO SysTick() Interrupt!!!------------------------------------------------------------------------------
//---------------------------- file: stm32f1xx_it.c ------------------------------------------------------------------------------
void HAL_SYSTICK_Callback(void)
{
	TimingManager::proceed();
}
