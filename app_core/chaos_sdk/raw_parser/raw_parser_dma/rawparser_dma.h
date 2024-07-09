#ifndef __RAW_PARSER_DMA_FUSION_H__
#define __RAW_PARSER_DMA_FUSION_H__ 1
#ifdef __cplusplus
extern "C" {
#endif


#include "raw_parser/rawparser_port.h"

/**
  * @brief RawParser_dma_DescriptorTypeDef structure definition
  */
typedef struct {
    // packet form -----------------------------------------
    u8           m_startByte;          // Specifies the value of start-byte.
    reg          m_receivePackLen;
    u8           m_receiveBuffer[D_RAW_P_RX_BUF_SIZE];        // received raw byte array

#ifndef D_RAW_P_DISABLE_INTERNAL_RX_BUFFER
    u8           m_receiveFrameBuffer[D_RAW_P_RX_BUF_SIZE];   // frame buffer to proceed into user logic`s
#endif /* D_RAW_P_DISABLE_INTERNAL_RX_BUFFER */

#ifndef D_RAW_P_DISABLE_INTERNAL_TX_BUFFER
    u8           m_sendBuffer[D_RAW_P_TX_BUF_SIZE];    // array for save tx buffer
#endif /* D_RAW_P_DISABLE_INTERNAL_TX_BUFFER */

    // if crc enable -> crc data variable
#ifdef D_RAW_P_CRC_ENA
    rawP_crc_t   m_receiveCalcCRC;            // rx crc calc data
    rawP_crc_t   m_transmittCalcCRC;          // tx crc calc data

#   if defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) || defined(D_RAW_P_USE_CRC64)
    rawP_crc_t   m_receiveCRCBuf;            // buffer for rx crc
#   endif /* defined(D_RAW_P_USE_CRC16) || defined(D_RAW_P_USE_CRC32) */

#endif /* D_RAW_P_CRC_ENA */
    // ----------------------------------------------------

    u8              m_triggerSB;            // trigger for read start byte
    reg             m_receivePos;           // receive raw position
    reg             m_receiveReadPos;       // receive read position
    reg             m_receiveHandlePos;     // receive handler position

    u8 receiveState;

    RawParser_Frame_t TX;
    RawParser_Frame_t RX;
    reg uniRXPosition; // value for universal read position function
    reg length_write_control;

} RawParser_dma_t;

/* CREATE/DELETE FUNCTIONS *************************************************************************************************
 *
 */
RawParser_dma_t* const rawParser_dma_new(const u8 packStart);
u8 rawParser_dma_init(RawParser_dma_t * const self, const u8 packStart);
u8 rawParser_dma_delete(RawParser_dma_t** data);

// set user buffers
#ifdef D_RAW_P_DISABLE_INTERNAL_TX_BUFFER
void rawParser_dma_setUserBufferTX(RawParser_dma_t * const self, u8 * const txBuffer);
#endif /* D_RAW_P_DISABLE_INTERNAL_TX_BUFFER */

#ifdef D_RAW_P_DISABLE_INTERNAL_RX_BUFFER
void rawParser_dma_setUserBufferRX(RawParser_dma_t * const self, u8 * const rxBuffer);
#endif /* D_RAW_P_DISABLE_INTERNAL_RX_BUFFER */

#if defined(D_RAW_P_DISABLE_INTERNAL_TX_BUFFER) && defined(D_RAW_P_DISABLE_INTERNAL_RX_BUFFER)
void rawParser_dma_setUserBuffers(RawParser_dma_t * const self, u8 * const rxBuffer, u8 * const txBuffer);
#endif /* defined(D_RAW_P_DISABLE_INTERNAL_TX_BUFFER) || defined(D_RAW_P_DISABLE_INTERNAL_RX_BUFFER) */


/* PROCEED FUNCTIONS *************************************************************************************************
 *
 */

// receive functions-----------------------------------------------------------------------------------------
STATIC_FORCEINLINE void RawParser_dma_receiveByte(RawParser_dma_t* const self, const u8 byte)
{
    // save to register ------------------------------------------------------------------------------
    u8* const m_receiveBuffer   = self->m_receiveBuffer;
    const reg m_receivePos      = self->m_receivePos;
    // do logic --------------------------------------------------------------------------------------
    m_receiveBuffer[m_receivePos & (D_RAW_P_RX_BUF_SIZE - 1U)] = byte;
    // save position to memory ------------------------------------------------------------------------
    self->m_receivePos = m_receivePos + 1U;
}

STATIC_FORCEINLINE void RawParser_dma_receiveArray(RawParser_dma_t* const self, const u8* const arr, const reg size)
{
    if(size > D_RAW_P_RX_BUF_SIZE) {
    	return;
    }

    // save to register ------------------------------------------------------------------------------
    u8* const m_receiveBuffer		= self->m_receiveBuffer;						// store pointer to register

    const reg m_receivePosReg		= self->m_receivePos;							// store head
    const reg m_receivePosReg_msk	= m_receivePosReg & (D_RAW_P_RX_BUF_SIZE - 1U); // get head position
    const reg remaining_to_end 		= D_RAW_P_RX_BUF_SIZE - m_receivePosReg_msk; 	// get remaining from head to end

    // do logic --------------------------------------------------------------------------------------
    if(size > remaining_to_end) {
    	const reg remaining_n		= size - remaining_to_end;
    	memcpy(m_receiveBuffer + m_receivePosReg_msk, arr, remaining_to_end);
    	memcpy(m_receiveBuffer, arr + remaining_to_end, remaining_n);
    } else {
    	memcpy(m_receiveBuffer + m_receivePosReg_msk, arr, size);
    }

    // save position to memory ------------------------------------------------------------------------
    self->m_receivePos = (m_receivePosReg + size);
}


RawParser_Frame_t* RawParser_dma_proceed(RawParser_dma_t* const self);

// slow shield functions (slow & more copy)-----------------------------------------------------------------------------------------
RawParser_Frame_t* RawParser_dma_shieldFrame(RawParser_dma_t* const self, const u8* data, reg len); // shield data array before transmitting
// fast shield functions (no copy)-----------------------------------------------------------------------------------------
u8 RawParser_dma_startTransmittPacket(RawParser_dma_t* const self, reg predictedLen);
RawParser_Frame_t* const RawParser_dma_finishTransmittPacket(RawParser_dma_t* const self);


// function for use universal macro ---------------------------------------------------------------------------------------------------------
u8 RawParser_dma_universalWrite(RawParser_dma_t* const self, reg totalLenInByte, const reg typelenInByte, const u8 *data);
u8 RawParser_dma_universalRead(RawParser_dma_t* const self, reg totalLenInByte, const reg typelenInByte, u8 *data);

#ifdef __cplusplus
}
#endif
#endif /* __RAW_PARSER_DMA_FUSION_H__ */
