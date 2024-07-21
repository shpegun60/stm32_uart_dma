/*
 * rawp_stream.h
 *
 *  Created on: Jul 13, 2024
 *      Author: admin
 */

#ifndef CHAOS_SDK_RAWP_DYNAMIC_STREAM_H_
#define CHAOS_SDK_RAWP_DYNAMIC_STREAM_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "rawp_dynamic_class.h"


// elementary byte adding functions ----------------------------------------------------------------------------
STATIC_FORCEINLINE void rawp_stream_addByte(ringbuf_t* const ring, const u8 c, const u8 SB)
{
	if(c == SB) {
		const u8 arr [2] = {SB, SB};
		ringbuf_put(ring, arr, sizeof(arr));
	} else {
		ringbuf_putc(ring, c);
	}
}


void rawp_stream_add_u8(RawParser_dynamic_t* const self, const u8 c);
void rawp_stream_add_u16(RawParser_dynamic_t* const self, const u16 c);
void rawp_stream_add_u32(RawParser_dynamic_t* const self, const u32 c);
void rawp_stream_add_u64(RawParser_dynamic_t* const self, const u64 c);

void rawp_stream_add_i16(RawParser_dynamic_t* const self, const i16 c);
void rawp_stream_add_i32(RawParser_dynamic_t* const self, const i32 c);
void rawp_stream_add_i64(RawParser_dynamic_t* const self, const i64 c);
void rawp_stream_add_f32(RawParser_dynamic_t* const self, const f32 c);
void rawp_stream_add_f64(RawParser_dynamic_t* const self, const f64 c);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CHAOS_SDK_RAWP_DYNAMIC_STREAM_H_ */
