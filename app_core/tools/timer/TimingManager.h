/*
 * TimingManager.h
 *
 *  Created on: Jun 5, 2024
 *      Author: admin
 */

#ifndef TOOLS_TIMINGMANAGER_H_
#define TOOLS_TIMINGMANAGER_H_

#include <vector>
#include <cstdint>

class TimingManager {
public:
	TimingManager();
	TimingManager(const uint32_t delay);
	~TimingManager();

    static inline void proceed() {
        for (TimingManager* const timer : m_timers) {
            if (timer->m_counter) {
                --timer->m_counter;
            }
        }
    }

    inline bool isExpired() const {
        return m_counter == 0;
    }

    inline void start(const uint32_t delay) { m_counter = delay; }

private:
    volatile uint32_t m_counter = 0;
    static std::vector<TimingManager*> m_timers;
};


#endif /* TOOLS_TIMINGMANAGER_H_ */
