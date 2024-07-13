/*
 * rawp_class.h
 *
 *  Created on: Jul 13, 2024
 *      Author: admin
 */

#ifndef CHAOS_SDK_RAWP_DYNAMIC_CLASS_H_
#define CHAOS_SDK_RAWP_DYNAMIC_CLASS_H_

#include "buffers/ringbuf.h"
#include "crc/crc_fabric.h"

/**
 * @brief RawParser_dynamic_DescriptorTypeDef structure definition
 */

typedef void (*RawPdynamicCallback)(ringbuf_t* const rxStream, void* const ctx);

typedef struct {
	u8	startByte;          // Specifies the value of start-byte.

	// packet form -----------------------------------------
	struct {
		bool	triggerSB;      // trigger for read start byte
		reg	tmpReceiveLen;		// receive length temp register
		u8 receiveState;		// receive state

		ringbuf_t input_ring;
		ringbuf_t frame_stream;
		// crc---------------
		crc_obj_t* crc;
		crc_base_t* crc_received;

		// callback
		RawPdynamicCallback rx_callback;
		void* rx_ctx;
	} RX;

	struct {
		ringbuf_t frame_stream;
		reg length_write_control;
		crc_obj_t* crc;
	} TX;

} RawParser_dynamic_t;


#endif /* CHAOS_SDK_RAWP_DYNAMIC_CLASS_H_ */
