/*
 * rawp_dynamic_stream.c
 *
 *  Created on: Jul 13, 2024
 *      Author: admin
 */

#include "rawp_dynamic_stream.h"
#include "byte_order/byte_order.h"

// include assert engine
#include "assertsEngine/assert_engine.h"

void rawp_stream_add_u8(RawParser_dynamic_t* const self, u8 c)
{
	ringbuf_t* const tx_ring = &self->TX.frame_stream;
	crc_obj_t* const tx_crc = self->TX.crc;

	const reg remaining_len = self->TX.length_write_control;
	M_Assert_BreakSaveCheck(remaining_len == 0, M_EMPTY, return, "Overflow!!!!!!");

	M_Assert_SafeFunctionCall(tx_crc, CRC_BYTE(tx_crc, c));
	rawp_stream_addByte(tx_ring, c, self->startByte);

	self->TX.length_write_control = remaining_len - 1;
}

#define RAW_P_STREAM_INST(TYPE) 																			\
    void rawp_stream_add_##TYPE(RawParser_dynamic_t* const self, const TYPE c) {							\
																											\
		const reg remaining_len = self->TX.length_write_control;											\
		M_Assert_BreakSaveCheck(remaining_len < sizeof(TYPE), M_EMPTY, return, "Overflow!!!!!!");			\
																											\
		const u8 SB = self->startByte;																		\
		ringbuf_t* const tx_ring = &self->TX.frame_stream;													\
		crc_obj_t* const tx_crc = self->TX.crc;																\
																											\
		union {TYPE from; u8 to[sizeof(TYPE)];} u;															\
		u.from = c;																							\
		LittleEndianGeneric(&u.from, sizeof(TYPE));															\
																											\
		M_Assert_SafeFunctionCall(tx_crc, {																	\
				CRC_ARRAY(tx_crc, u.to, sizeof(TYPE));														\
		});																									\
																											\
		for(u8 i = 0; i != sizeof(TYPE); ++i) {																\
			rawp_stream_addByte(tx_ring, u.to[i], SB);														\
		}																									\
																											\
		self->TX.length_write_control = remaining_len - sizeof(TYPE);										\
    }

RAW_P_STREAM_INST(u16)
RAW_P_STREAM_INST(u32)
RAW_P_STREAM_INST(u64)
RAW_P_STREAM_INST(i16)
RAW_P_STREAM_INST(i32)
RAW_P_STREAM_INST(i64)
RAW_P_STREAM_INST(f32)
RAW_P_STREAM_INST(f64)

