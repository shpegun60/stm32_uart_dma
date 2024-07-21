/*
 * RelayController.h
 *
 *  Created on: Jul 2, 2024
 *      Author: admin
 */

#ifndef RELAYCONTROLLER_H_
#define RELAYCONTROLLER_H_

#include "timer/TimingManager.h"
#include "ProtocolSender.h"
#include "TPS1HTC30/Keytps1htc30Group.h"

class RelayController
{
	RelayController();
public:
	~RelayController() = default;

	void init_all();

	static RelayController* const get_instance();
	void proceed();

private:
	TimingManager led_timer;
	ProtocolSender m_comm;
	Key_tps1htc30Group* m_relayGroup = nullptr;

	uint16_t currents[10];
};

#endif /* RELAYCONTROLLER_H_ */
