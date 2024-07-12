#ifndef __RAW_PARSER_DYNAMIC_FUSION_H__
#define __RAW_PARSER_DYNAMIC_FUSION_H__ 1
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "raw_parser/rawparser_port.h"
#include "buffers/ringbuf.h"
#include "crc/crc_fabric.h"

typedef struct {
    u8* data;
    u16 size;
    u16 cap;
    u16 rdPos;
    u16 wrPos;
} RawParser_stream_t;

/**
 * @brief RawParser_dynamic_DescriptorTypeDef structure definition
 */
typedef struct {
	u8	m_startByte;          // Specifies the value of start-byte.

	// packet form -----------------------------------------
	struct {
		u8	triggerSB;          // trigger for read start byte
		reg	tmpReceiveLen;		// receive length temp register
		u8 receiveState;		// receive state

		ringbuf_t ring;
		RawParser_stream_t stream;
		crc_base_t* crc;
	} RX;

	struct {
		RawParser_stream_t stream;
		reg length_write_control;
		crc_base_t* crc;
	} TX;

} RawParser_dynamic_t;

/* CREATE/DELETE FUNCTIONS *************************************************************************************************
 *
 */
RawParser_dynamic_t* const rawParser_dynamic_new(const u8 packStart, const crc_type crc_type);
bool rawParser_dynamic_init(RawParser_dynamic_t * const self, const u8 packStart, const crc_type crc_type);
bool rawParser_dynamic_delete(RawParser_dynamic_t** data);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAW_PARSER_DYNAMIC_FUSION_H__ */
