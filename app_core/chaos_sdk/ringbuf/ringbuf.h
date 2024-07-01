/*
ringbuf.h

MIT License

Copyright (c) 2016 Yongqian Tang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef __RINGBUF_H__
#define __RINGBUF_H__

#include "my_ctype/my_ctypes_def.h"
#include "inline.h"

// Determine whether the buffer is full
#define ringbuf_is_full(handle)\
		(handle)->wrFull
#define ringbuf_is_empty(handle)\
		(handle)->rdEmpty

typedef struct ring_buffer {
    void * buffer;   //buffer
    reg size;     	//size
    reg msk;		// mask

    reg tail;			//Exit location
    reg head;			//Entrance location
    u8 wrFull;
    u8 rdEmpty;
} ringbuf_t;


// Initialize buffer
reg ringbuf_init(ringbuf_t* const ring_buf, void *buffer, const reg size);
// Get data from buffer
reg ringbuf_get(ringbuf_t* const ring_buf, void* const buffer, const reg size);
// Store data into buffer
reg ringbuf_put(ringbuf_t* const ring_buf, const void *buffer, const reg size);
// Store byte data in buffer
reg ringbuf_putc(ringbuf_t* const ring_buf, const u8 c);

// The length of the buffer data
STATIC_FORCEINLINE reg ringbuf_use_len(const ringbuf_t * const ring_buf)
{
    return (ring_buf->head - ring_buf->tail);
}



/*
 * static uint8_t s_send_packet_buff[SEND_PACKET_SIZE];
 * static ringbuf_t s_send_ring_buff;
 *
 * ringbuf_init(&s_send_ring_buff, s_send_buffer, sizeof(s_send_buffer));
 *
 */

#endif /*__RINGBUF_H__ */
