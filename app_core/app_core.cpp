/*
 * app_core.cpp
 *
 *  Created on: Jul 17, 2024
 *      Author: admin
 */
#include "app_core.h"
#include "bc_power.h"

void app_core()
{
	auto& app = BC_Power::instance();
	app.loop();
}
