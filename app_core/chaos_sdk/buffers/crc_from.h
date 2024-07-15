/*
 * crc_from_ringbuf.h
 *
 *  Created on: Jul 12, 2024
 *      Author: admin
 */

#ifndef CHAOS_SDK_BUFFERS_CRC_FROM_H_
#define CHAOS_SDK_BUFFERS_CRC_FROM_H_

#include "crc/crc_factory.h"
#include "ringbuf.h"

void crc_from_ringbuf(crc_strategy_t* const crc_base, const ringbuf_t* const ring_buf);

#endif /* CHAOS_SDK_BUFFERS_CRC_FROM_H_ */
