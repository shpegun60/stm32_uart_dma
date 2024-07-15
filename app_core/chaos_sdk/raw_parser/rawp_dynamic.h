#ifndef __RAW_PARSER_DYNAMIC_FUSION_H__
#define __RAW_PARSER_DYNAMIC_FUSION_H__ 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "rawp_dynamic_stream.h"

typedef struct {
	u8 packStart;
	//rx -------------
	u16 rxBufferSize;
	u16 rxFrameSize;
	void* rxFrameBuff;
	//tx--------------
	u16 txFrameSize;
	void* txFrameBuff;
	crc_type crc_strategy;
} RawP_dynamic_strategy_t;


/* CREATE/DELETE FUNCTIONS *************************************************************************************************
 *
 */
RawParser_dynamic_t* const rawP_dynamic_new();
bool rawP_dynamic_init(RawParser_dynamic_t* const self, const RawP_dynamic_strategy_t* const settings);

/* PROCEED Receive FUNCTIONS *************************************************************************************************
 *
 */

// receive functions-----------------------------------------------------------------------------------------
STATIC_FORCEINLINE void rawP_dynamic_receiveByte(RawParser_dynamic_t* const self, const u8 byte)
{
	ringbuf_putc(&self->RX.input_ring, byte);
}

STATIC_FORCEINLINE void rawP_dynamic_receiveArray(RawParser_dynamic_t* const self, const u8* const arr, const reg size)
{
	ringbuf_put(&self->RX.input_ring, arr, size);
}

void rawP_dynamic_subscribe(RawParser_dynamic_t* const self, const RawPdynamicCallback rx_callback, void* const rx_ctx);
void rawP_dynamic_proceed(RawParser_dynamic_t* const self);

/* PROCEED Transmit FUNCTIONS *************************************************************************************************
 *
 */
// fast shield functions (no copy)-----------------------------------------------------------------------------------------
bool rawP_dynamic_startTransmittPacket(RawParser_dynamic_t* const self, reg len);
ringbuf_t* const rawP_dynamic_finishTransmittPacket(RawParser_dynamic_t* const self);

STATIC_FORCEINLINE bool rawP_dynamic_install_txbuf(RawParser_dynamic_t* const self, void* const buffer)
{
	return ringbuf_install_buf(&self->TX.frame_stream, buffer);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAW_PARSER_DYNAMIC_FUSION_H__ */
