#ifndef __RAW_PARSER_DYNAMIC_FUSION_H__
#define __RAW_PARSER_DYNAMIC_FUSION_H__ 1
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

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





#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAW_PARSER_DYNAMIC_FUSION_H__ */
