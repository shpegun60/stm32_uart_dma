/*
 * Timerms.h
 *
 *  Created on: May 21, 2024
 *      Author: admin
 */

#ifndef KLS_TIMERMS_H_
#define KLS_TIMERMS_H_

#include <cstdint>
#include <functional>

class ProgramTimer
{
public:
	ProgramTimer() = default;
	ProgramTimer(std::function<void(const uint32_t current_time, const uint32_t dt)> event): m_event(event) {}
	~ProgramTimer();

	typedef std::function<void(const uint32_t current_time, const uint32_t dt)> event_t;

	inline void set_event_and_dt(const uint32_t dt, const event_t event) { m_dt = dt; m_event = event; }
	inline void set_event(const event_t event) { m_event = event; }
	inline void set_dt(const uint32_t dt) { m_dt = dt; }

	void proceed(const uint32_t current_time);

private:
	uint32_t m_dt;
	uint32_t m_last_time;
	event_t m_event = nullptr;
};

#endif /* KLS_TIMERMS_H_ */
