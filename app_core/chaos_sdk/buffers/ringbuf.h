/*
ringbuf.h

Copyright (c) 2024 Shpegun60

 */
#ifndef __RINGBUF_H__
#define __RINGBUF_H__

#include "fifo_base.h"

typedef struct ring_buffer {
	fifo_base_t base;

	void* buffer;   //buffer
} ringbuf_t;


// Initialize buffer
ringbuf_t* const ringbuf_new(void* const buffer, const reg size);
bool ringbuf_init(ringbuf_t* const ring_buf, void* const buffer, const reg size);
bool ringbuf_install_buf(ringbuf_t* const ring_buf, void* const buffer);

// Get data from buffer ===============================================================================
reg ringbuf_get(ringbuf_t* const ring_buf, void* const buffer, const reg size);
// Get byte from buffer
u8 ringbuf_getc(ringbuf_t* const ring_buf);
u16 ringbuf_get_u16(ringbuf_t* const ring_buf);
u32 ringbuf_get_u32(ringbuf_t* const ring_buf);
u64 ringbuf_get_u64(ringbuf_t* const ring_buf);

i16 ringbuf_get_i16(ringbuf_t* const ring_buf);
i32 ringbuf_get_i32(ringbuf_t* const ring_buf);
i64 ringbuf_get_i64(ringbuf_t* const ring_buf);

f32 ringbuf_get_f32(ringbuf_t* const ring_buf);
f64 ringbuf_get_f64(ringbuf_t* const ring_buf);

// PEEK data from buffer================================================================================================
reg ringbuf_peek(ringbuf_t* const ring_buf, void* const buffer, const reg size);
u8 ringbuf_peekc(ringbuf_t* const ring_buf);
u16 ringbuf_peek_u16(ringbuf_t* const ring_buf);
u32 ringbuf_peek_u32(ringbuf_t* const ring_buf);

i16 ringbuf_peek_i16(ringbuf_t* const ring_buf);
i32 ringbuf_peek_i32(ringbuf_t* const ring_buf);
i64 ringbuf_peek_i64(ringbuf_t* const ring_buf);

f32 ringbuf_peek_f32(ringbuf_t* const ring_buf);
f64 ringbuf_peek_f64(ringbuf_t* const ring_buf);


// Store data into buffer =============================================================================
reg ringbuf_put(ringbuf_t* const ring_buf, const void *buffer, const reg size);
// Store byte data in buffer
bool ringbuf_putc(ringbuf_t* const ring_buf, const u8 c);
bool ringbuf_put_u16(ringbuf_t* const ring_buf, u16 c);
bool ringbuf_put_u32(ringbuf_t* const ring_buf, u32 c);
bool ringbuf_put_u64(ringbuf_t* const ring_buf, u64 c);

bool ringbuf_put_i16(ringbuf_t* const ring_buf, i16 c);
bool ringbuf_put_i32(ringbuf_t* const ring_buf, i32 c);
bool ringbuf_put_i64(ringbuf_t* const ring_buf, i64 c);

bool ringbuf_put_f32(ringbuf_t* const ring_buf, f32 c);
bool ringbuf_put_f64(ringbuf_t* const ring_buf, f64 c);

/*
 * static uint8_t s_send_packet_buff[SEND_PACKET_SIZE];
 * static ringbuf_t s_send_ring_buff;
 *
 * ringbuf_init(&s_send_ring_buff, s_send_buffer, sizeof(s_send_buffer));
 *
 */

#endif /*__RINGBUF_H__ */
