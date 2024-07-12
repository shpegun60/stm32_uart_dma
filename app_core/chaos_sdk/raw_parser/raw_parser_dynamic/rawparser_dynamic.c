#include "rawparser_dynamic.h"
#include "assertsEngine/assert_engine.h"

#include <stdlib.h>

/* CREATE/DELETE FUNCTIONS *************************************************************************************************
 *
 */

//RawParser_dynamic_t* const rawParser_dynamic_new(const u8 packStart,
//		u16 rxRingSize, const crc_type crc_type, )
//{
//	M_Assert_BreakSaveCheck(packStart == RECEIVE_EXTENDED_LEN_CMD, M_EMPTY,
//			return NULL,
//					"start byte: %d must be not equal RECEIVE_EXTENDED_LEN_CMD: %d", packStart, RECEIVE_EXTENDED_LEN_CMD);
//
//	RawParser_dynamic_t* self = (RawParser_dynamic_t *)calloc(1, sizeof(RawParser_dynamic_t));
//	M_Assert_BreakSaveCheck(self == NULL, M_EMPTY, return self, "No memory for allocation ");
//
//	if(rawParser_dynamic_init(self, packStart, crc_type)) {
//		free(self);
//		self = NULL;
//	}
//	return self;
//}


bool rawParser_dynamic_init(RawParser_dynamic_t * const self, const u8 packStart, const crc_type crc_type)
{
	M_Assert_BreakSaveCheck(packStart == RECEIVE_EXTENDED_LEN_CMD, M_EMPTY,
				return NULL,
						"start byte: %d must be not equal RECEIVE_EXTENDED_LEN_CMD: %d", packStart, RECEIVE_EXTENDED_LEN_CMD);

	M_Assert_BreakSaveCheck(self == NULL, M_EMPTY, return true, "No input data valid ");

	memset(self, 0, sizeof(RawParser_dynamic_t));
	self->m_startByte = packStart;

	return false;

}


bool rawParser_dynamic_delete(RawParser_dynamic_t** data);
