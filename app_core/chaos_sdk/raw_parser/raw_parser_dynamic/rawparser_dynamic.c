#include "rawparser_dynamic.h"
#include "byte_order/byte_order.h"
#include <stdlib.h>

#include"buffers/crc_from.h"

// include assert engine
#include "assertsEngine/assert_engine.h"
#define _INT_SWITCH(...) __VA_ARGS__


/* CREATE/DELETE FUNCTIONS *************************************************************************************************
 *
 */

RawParser_dynamic_t* const rawParser_dynamic_new(const u8 packStart,
		u16 rxBufferSize, u16 rxFrameSize, u16 txFrameSize, const crc_type crc_type)
{
	M_Assert_BreakSaveCheck(packStart == RECEIVE_EXTENDED_LEN_CMD, M_EMPTY,
			return NULL,
					"start byte: %d must be not equal RECEIVE_EXTENDED_LEN_CMD: %d", packStart, RECEIVE_EXTENDED_LEN_CMD);

	RawParser_dynamic_t* self = (RawParser_dynamic_t *)calloc(1, sizeof(RawParser_dynamic_t));
	M_Assert_BreakSaveCheck(self == NULL, M_EMPTY, return self, "No memory for allocation ");
	return self;
}

bool rawParser_dynamic_init_default(RawParser_dynamic_t * const self, const u8 packStart,
		u16 rxBufferSize, u16 rxFrameSize, u16 txFrameSize, const crc_type crc_type)
{
	M_Assert_BreakSaveCheck(packStart == RECEIVE_EXTENDED_LEN_CMD, M_EMPTY,
			return NULL,
					"start byte: %d must be not equal RECEIVE_EXTENDED_LEN_CMD: %d", packStart, RECEIVE_EXTENDED_LEN_CMD);

	M_Assert_BreakSaveCheck(self == NULL, M_EMPTY, return true, "No input data valid ");

	memset(self, 0, sizeof(RawParser_dynamic_t));
	self->startByte = packStart;

	// RX init -----------------------------
	self->RX.receiveState = 0;
	ringbuf_init(&self->RX.input_ring, NULL, rxBufferSize);

	ringbuf_init(&self->RX.frame_stream, NULL, rxFrameSize);
	self->RX.crc = createCRC(crc_type);
	self->RX.crc_received = CRC_BASE_DUPLICATE(self->RX.crc);

	// TX init -----------------------------
	ringbuf_init(&self->TX.frame_stream, NULL, txFrameSize);
	self->TX.crc = createCRC(crc_type);
	return false;
}


/* PROCEED FUNCTIONS *************************************************************************************************
 *
 */

static void _proceedByte(RawParser_dynamic_t* const self, const u8 ch, const bool newFrame)
{
#define NEXT_STATE(state) self->RX.receiveState = state

	if (newFrame) {
		M_Assert_SafeFunctionCall(self->RX.crc, {
				CRC_INIT(self->RX.crc);
				self->RX.crc_received->crc_size = 0;
		});

		FIFO_CLEAN(&self->RX.frame_stream);
		NEXT_STATE(0);
	}

	// FSM data fusion -----------------------------
#define RECEIVE_LEN_0                     0x00U
#define RECEIVE_LEN_LOW                0x01U
#define RECEIVE_LEN_HIGH               0x02U
#define RECEIVE_DATA                   	0x03U
#define RECEIVE_CRC                   	0x04U

#define RECEIVE_ERR                       0x0CU
#define RECEIVE_OK                        0x0DU

	switch(self->RX.receiveState) {

	case RECEIVE_LEN_0: {
		M_Assert_SafeFunctionCall(self->RX.crc, {
				CRC_BYTE(self->RX.crc, ch);
		});

		if(RECEIVE_EXTENDED_LEN_CMD == ch) {
			NEXT_STATE(RECEIVE_LEN_LOW);
		} else {
			const reg rx_len = self->RX.tmpReceiveLen = ((ch > self->startByte) ? (ch - 1U) : ch);
			FIFO_CLEAN(&self->RX.frame_stream);

			M_Assert_WarningSaveCheck((rx_len > FIFO_CAPACITY(&self->RX.frame_stream) || rx_len == 0), M_EMPTY, {
					NEXT_STATE(RECEIVE_ERR);
			}, "No valid receive length");

			NEXT_STATE(RECEIVE_DATA);
		}

		break;}

	// ----------------- TWO BYTES LEN -------------------------------------------------------------------------
	case RECEIVE_LEN_LOW: {
		M_Assert_SafeFunctionCall(self->RX.crc, {
				CRC_BYTE(self->RX.crc, ch);
		});

		self->RX.tmpReceiveLen = ch;    // read low byte
		NEXT_STATE(RECEIVE_LEN_HIGH);
		break;}

	case RECEIVE_LEN_HIGH: {
		M_Assert_SafeFunctionCall(self->RX.crc, {
				CRC_BYTE(self->RX.crc, ch);
		});

		reg rx_len = ((reg)(ch << 8U) | self->RX.tmpReceiveLen) & 0x0000FFFFUL ; // read high byte
		self->RX.tmpReceiveLen = rx_len = LittleEndianU16(rx_len);
		FIFO_CLEAN(&self->RX.frame_stream);

		M_Assert_WarningSaveCheck((rx_len > FIFO_CAPACITY(&self->RX.frame_stream) || rx_len == 0), M_EMPTY, {
				NEXT_STATE(RECEIVE_ERR);
		}, "No valid receive length");

		NEXT_STATE(RECEIVE_DATA);
		break;}

	// ----------------- TWO BYTES LEN -------------------------------------------------------------------------

	case RECEIVE_DATA: {

		ringbuf_putc(&self->RX.frame_stream, ch);

		if (/*FIFO_LEN(&self->RX.frame_stream)*/self->RX.frame_stream.base.head == self->RX.tmpReceiveLen) {
			if(self->RX.crc) {
				NEXT_STATE(RECEIVE_CRC);
			} else {
				const RawPdynamicCallback _callback = self->RX.rx_callback;
				void* const _ctx = self->RX.rx_ctx;

				if(_callback) {
					_callback(&self->RX.frame_stream, _ctx);
				}

				NEXT_STATE(RECEIVE_OK);
			}
		}
		break;}

	case RECEIVE_CRC: {
		u8* const crc_pointer = self->RX.crc_received->crc;
		u8 crc_counter = self->RX.crc_received->crc_size;
		*(crc_pointer + crc_counter) = ch;
		++crc_counter;

		if(crc_counter == self->RX.crc->base.crc_size) {
			LittleEndianGeneric(crc_pointer, crc_counter);
			crc_from_ringbuf(self->RX.crc, &self->RX.frame_stream);

			if(crc_compleate(&self->RX.crc->base, self->RX.crc_received)) {
				const RawPdynamicCallback _callback = self->RX.rx_callback;
				void* const _ctx = self->RX.rx_ctx;

				if(_callback) {
					_callback(&self->RX.frame_stream, _ctx);
				}

				NEXT_STATE(RECEIVE_OK);
			}

			NEXT_STATE(RECEIVE_ERR);
		}
		break;}

	case RECEIVE_ERR: {
		M_Assert_WarningSaveCheck(M_ALWAYS, M_EMPTY, M_EMPTY, "ERROR PACKET");
		break;}

	case RECEIVE_OK: {
		M_Assert_WarningSaveCheck(M_ALWAYS, M_EMPTY, M_EMPTY, "RECEIVE OK");
		break;}
	}
}

void rawParser_dynamic_proceed(RawParser_dynamic_t* const self)
{
	_INT_SWITCH(M_Assert_Break((self == NULL), M_EMPTY, return NULL, "No valid input"));

	u8 buffer[10];
	const u8 SB = self->startByte;
	bool triggerSB = self->RX.triggerSB;

	while(FIFO_NOT_EMPTY(&self->RX.input_ring)) {
		const reg len = ringbuf_get(&self->RX.input_ring, buffer, sizeof(buffer));

		for(reg i = 0; i != len; ++i) {
			const u8 ch = buffer[i];

			if (triggerSB) {
				if(SB == ch) { 		//{SB}{SB} -> {SB}
					_proceedByte(self, ch, false);
				} else { 			//{SB}{!SB} -> {SB} and newframe
					_proceedByte(self, ch, true);
				}
				triggerSB = false;
			} else if (SB == ch) { 	//{!SB}{SB} -> set flag and skip step
				triggerSB = true;
			} else { 				//{!SB}{!SB} -> {!SB}
				_proceedByte(self, ch, false);
			}
		}
	}

	self->RX.triggerSB = triggerSB;
}


#undef _INT_SWITCH
