#ifndef __RAW_PARSER_DYNAMIC_FUSION_H__
#define __RAW_PARSER_DYNAMIC_FUSION_H__ 1
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "raw_parser/rawparser_port.h"
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

/* CREATE/DELETE FUNCTIONS *************************************************************************************************
 *
 */
RawParser_dynamic_t* const rawParser_dynamic_new(const u8 packStart,
		u16 rxBufferSize, u16 rxFrameSize, u16 txFrameSize, const crc_type crc_type);

bool rawParser_dynamic_init_default(RawParser_dynamic_t * const self,const u8 packStart,
		u16 rxBufferSize, u16 rxFrameSize, u16 txFrameSize, const crc_type crc_type);

/* PROCEED FUNCTIONS *************************************************************************************************
 *
 */

// receive functions-----------------------------------------------------------------------------------------
STATIC_FORCEINLINE void rawParser_dynamic_receiveByte(RawParser_dynamic_t* const self, const u8 byte)
{
	ringbuf_putc(&self->RX.input_ring, byte);
}

STATIC_FORCEINLINE void rawParser_dynamic_receiveArray(RawParser_dynamic_t* const self, const u8* const arr, const reg size)
{
	ringbuf_put(&self->RX.input_ring, arr, size);
}

void rawParser_dynamic_proceed(RawParser_dynamic_t* const self);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAW_PARSER_DYNAMIC_FUSION_H__ */
