/*
 * app_core.cpp
 *
 *  Created on: Jun 30, 2024
 *      Author: admin
 */
#include "app_core.h"
#include "main.h"
#include <RelayController.h>

RelayController* global;

void app_main(void)
{
	RelayController* const inst = global = RelayController::get_instance();
	inst->proceed();
}
