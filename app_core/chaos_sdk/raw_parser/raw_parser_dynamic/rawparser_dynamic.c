#include "raw_parser/rawparser_port.h"
#include "rawparser_dynamic.h"
#include "buffers/crc_from.h"

#include "byte_order/byte_order.h"
#include <stdlib.h>



// include assert engine
#include "assertsEngine/assert_engine.h"
#define _INT_SWITCH(...) __VA_ARGS__


/* HELP FUNCTIONS *************************************************************************************************
 *
 */

static bool _rawp_start(RawParser_dynamic_t * const self, const u8 packStart)
{
	M_Assert_BreakSaveCheck(packStart == RECEIVE_EXTENDED_LEN_CMD || packStart == 0x00, M_EMPTY,
			return false, "SB not valid");

	memset(self, 0, sizeof(RawParser_dynamic_t));
	self->startByte = packStart;

	return true;
}

static void _rx_new(RawParser_dynamic_t * const self,
		const u16 rxBuffSize, const u16 rxFrameSize, void* const frameBuff, const crc_type crc_type)
{
	// RX init -----------------------------
	self->RX.receiveState = 0;
	ringbuf_init(&self->RX.input_ring, NULL, rxBuffSize);

	ringbuf_init(&self->RX.frame_stream, frameBuff, rxFrameSize);
	self->RX.crc = createCRC(crc_type);
	self->RX.crc_received = CRC_BASE_DUPLICATE(self->RX.crc);
}

static void _tx_new(RawParser_dynamic_t * const self,
		const u16 txFrameSize, void* const txBuff, const crc_type crc_type)
{
	ringbuf_init(&self->TX.frame_stream, txBuff, txFrameSize);
	self->TX.crc = createCRC(crc_type);
}

/* CREATE/DELETE FUNCTIONS *************************************************************************************************
 *
 */

RawParser_dynamic_t* const rawP_dynamic_new()
{
	RawParser_dynamic_t* const self = (RawParser_dynamic_t *)calloc(1, sizeof(RawParser_dynamic_t));
	M_Assert_BreakSaveCheck(self == NULL, M_EMPTY, return NULL, "No memory for allocation ");
	return self;
}

bool rawP_dynamic_init(RawParser_dynamic_t* const self, const RawP_dynamic_init_t* const settings)
{
	M_Assert_BreakSaveCheck(self == NULL || settings == NULL, M_EMPTY, return NULL, "No valid parameters");

	if(!_rawp_start(self, settings->packStart)) {
		return false;
	}

	// RX init -----------------------------
	_rx_new(self, settings->rxBufferSize, settings->rxFrameSize, settings->rxFrameBuff, settings->crc_type);

	// TX init -----------------------------
	_tx_new(self, settings->txFrameSize, settings->txFrameBuff, settings->crc_type);

	return true;
}


/* PROCEED FUNCTIONS *************************************************************************************************
 *
 */

static void _proceedByte(RawParser_dynamic_t* const self, const u8 ch, const bool newFrame)
{
	// FSM data fusion -----------------------------
#define RECEIVE_LEN_0					0x00U
#define RECEIVE_LEN_LOW                	0x01U
#define RECEIVE_LEN_HIGH               	0x02U
#define RECEIVE_DATA                   	0x03U
#define RECEIVE_CRC                   	0x04U
	//--------------------------------------------
#define RECEIVE_COMPLEATE              	0x0CU

	u8 _state = self->RX.receiveState;
	ringbuf_t* const rx_frame_ring = &self->RX.frame_stream;
	crc_obj_t* const rx_crc_check = self->RX.crc;

	if (newFrame) {
		M_Assert_SafeFunctionCall(rx_crc_check, {
				CRC_INIT(rx_crc_check);
				self->RX.crc_received->crc_size = 0;
		});

		FIFO_CLEAN(rx_frame_ring);
		_state = RECEIVE_LEN_0;
	}

	switch(_state) {

	case RECEIVE_LEN_0: {
		M_Assert_SafeFunctionCall(rx_crc_check, CRC_BYTE(rx_crc_check, ch));

		if(RECEIVE_EXTENDED_LEN_CMD == ch) {
			_state = RECEIVE_LEN_LOW;
		} else {
			const reg rx_len = self->RX.tmpReceiveLen = ((ch > self->startByte) ? (ch - 1U) : ch);
			FIFO_CLEAN(rx_frame_ring);

			M_Assert_WarningSaveCheck((rx_len > FIFO_CAPACITY(rx_frame_ring) || rx_len == 0), M_EMPTY, {
					self->RX.receiveState = RECEIVE_COMPLEATE;
					return;
			}, "No valid receive length");

			_state = RECEIVE_DATA;

		}

		break;}

	// ----------------- TWO BYTES LEN -------------------------------------------------------------------------
	case RECEIVE_LEN_LOW: {
		M_Assert_SafeFunctionCall(rx_crc_check, CRC_BYTE(rx_crc_check, ch));

		self->RX.tmpReceiveLen = ch;    // read low byte
		_state = RECEIVE_LEN_HIGH;
		break;}

	case RECEIVE_LEN_HIGH: {
		M_Assert_SafeFunctionCall(rx_crc_check, CRC_BYTE(rx_crc_check, ch));

		reg rx_len = ((reg)(ch << 8U) | self->RX.tmpReceiveLen) & 0x0000FFFFUL ; // read high byte
		LittleEndianGeneric(&rx_len, sizeof(u16));
		self->RX.tmpReceiveLen = rx_len;
		FIFO_CLEAN(rx_frame_ring);

		M_Assert_WarningSaveCheck((rx_len > FIFO_CAPACITY(rx_frame_ring) || rx_len == 0), M_EMPTY, {
				self->RX.receiveState = RECEIVE_COMPLEATE;
				return;
		}, "No valid receive length");

		_state = RECEIVE_DATA;
		break;}

	// ----------------- TWO BYTES LEN -------------------------------------------------------------------------

	case RECEIVE_DATA: {

		ringbuf_putc(rx_frame_ring, ch);

		if (/*FIFO_LEN(rx_frame_ring)*/rx_frame_ring->base.head == self->RX.tmpReceiveLen) {
			if(rx_crc_check) {
				_state = RECEIVE_CRC;
			} else {
				const RawPdynamicCallback _callback = self->RX.rx_callback;
				void* const _ctx = self->RX.rx_ctx;

				if(_callback) {
					_callback(rx_frame_ring, _ctx);
				}

				_state = RECEIVE_COMPLEATE;
			}
		}
		break;}

	case RECEIVE_CRC: {
		crc_base_t* const rx_crc = self->RX.crc_received;

		u8* const crc_pointer = rx_crc->crc;
		u8 crc_counter = rx_crc->crc_size;


		*(crc_pointer + crc_counter) = ch;
		++crc_counter;
		rx_crc->crc_size = crc_counter;

		if(crc_counter == rx_crc_check->base.crc_size) {
			LittleEndianGeneric(crc_pointer, crc_counter);
			crc_from_ringbuf(rx_crc_check, rx_frame_ring);

			if(crc_compleate(&rx_crc_check->base, rx_crc)) {
				const RawPdynamicCallback _callback = self->RX.rx_callback;
				void* const _ctx = self->RX.rx_ctx;

				if(_callback) {
					_callback(rx_frame_ring, _ctx);
				}
			}

			_state = RECEIVE_COMPLEATE;
		}
		break;}

	case RECEIVE_COMPLEATE: {
		M_Assert_WarningSaveCheck(M_ALWAYS, M_EMPTY, M_EMPTY, "ERROR PACKET");
		break;}
	}

	// save to memory
	self->RX.receiveState = _state;


#undef RECEIVE_LEN_0
#undef RECEIVE_LEN_LOW
#undef RECEIVE_LEN_HIGH
#undef RECEIVE_DATA
#undef RECEIVE_CRC
	//--------------------------------------------
#undef RECEIVE_COMPLEATE

#undef NEXT_STATE
}

void rawP_dynamic_subscribe(RawParser_dynamic_t* const self, const RawPdynamicCallback rx_callback, void* const rx_ctx)
{
	M_Assert_BreakSaveCheck(self == NULL, M_EMPTY, return, "No valid input");
	self->RX.rx_callback = rx_callback;
	self->RX.rx_ctx = rx_ctx;
}

void rawP_dynamic_proceed(RawParser_dynamic_t* const self)
{
	_INT_SWITCH(M_Assert_Break((self == NULL), M_EMPTY, return NULL, "No valid input"));

	u8 buffer[10];
	const u8 SB = self->startByte;
	bool triggerSB = self->RX.triggerSB;
	ringbuf_t* const rx_ring = &self->RX.input_ring;

	while(FIFO_NOT_EMPTY(rx_ring)) {
		const reg len = ringbuf_get(rx_ring, buffer, sizeof(buffer));

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



/* PROCEED Transmit FUNCTIONS *************************************************************************************************
 *
 */
// fast shield functions (no copy)-----------------------------------------------------------------------------------------
bool rawP_dynamic_startTransmittPacket(RawParser_dynamic_t* const self, reg len)
{
	_INT_SWITCH(M_Assert_Break((self == NULL), M_EMPTY, return false, "No valid input"));

	// move to cash ---------------------------------------------
	const u8 SB = self->startByte;
	crc_obj_t* const tx_crc = self->TX.crc;
	ringbuf_t* const tx_ring = &self->TX.frame_stream;
	const reg max_size = FIFO_CAPACITY(tx_ring) - 5U;

	// do logic ------------------------------------------------
	M_Assert_WarningSaveCheck(len == 0 || len > max_size,
			M_EMPTY, return false, "No valid input length");

	M_Assert_Warning(len > (max_size >> 1),
			M_EMPTY, M_EMPTY, "UNDEFINED BEHAVIOR!!!");

	M_Assert_SafeFunctionCall(tx_crc, CRC_INIT(tx_crc));

	FIFO_CLEAN(tx_ring);
	self->TX.length_write_control = len;

	ringbuf_putc(tx_ring, SB);

	if(len > D_RAW_P_LEN_SEPARATOR) {
		LittleEndianGeneric(&len, sizeof(u16));

		const u8 len_l = (u8)(len & 0xFF);
		const u8 len_h = (u8)((len >> 8) & 0xFF);

		ringbuf_putc(tx_ring, RECEIVE_EXTENDED_LEN_CMD);
		rawp_stream_addByte(tx_ring, len_l, SB);
		rawp_stream_addByte(tx_ring, len_h, SB);

		M_Assert_SafeFunctionCall(tx_crc, {
				const u8 arr[3] = {RECEIVE_EXTENDED_LEN_CMD, len_l, len_h};
				CRC_ARRAY(tx_crc, arr, sizeof(arr));
		});

	} else {
		const reg len_reg = (len >= SB) ? (len + 1U) : len;
		ringbuf_putc(tx_ring, len_reg);

		M_Assert_SafeFunctionCall(tx_crc, CRC_BYTE(tx_crc, len_reg));
	}
	return true;
}


ringbuf_t* const rawP_dynamic_finishTransmittPacket(RawParser_dynamic_t* const self)
{
	_INT_SWITCH(M_Assert_Break((self == NULL), M_EMPTY, return NULL, "No valid input"));

	crc_obj_t* const tx_crc = self->TX.crc;
	ringbuf_t* const tx_ring = &self->TX.frame_stream;
	reg ramained_len = self->TX.length_write_control;

	// fill remaining len
	if(tx_crc) {
		const u8 SB = self->startByte;

		u8* const crc_pointer = tx_crc->base.crc;
		const u8 crc_size = tx_crc->base.crc_size;
		const crc_byte_func _crc_calc = tx_crc->crc_byte;

		while(ramained_len--) {
			ringbuf_putc(tx_ring, 0x00);
			_crc_calc(crc_pointer, 0x00);
		}

		LittleEndianGeneric(crc_pointer, crc_size);

		for(u8 i = 0; i != crc_size; ++i) {
			rawp_stream_addByte(tx_ring, *(crc_pointer + i), SB);
		}

	} else {
		while(ramained_len--) {
			ringbuf_putc(tx_ring, 0x00);
		}
	}

	return tx_ring;
}


#undef _INT_SWITCH
