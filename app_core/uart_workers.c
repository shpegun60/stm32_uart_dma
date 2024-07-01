/*
 * uart_workers.c
 *
 *  Created on: Jul 1, 2024
 *      Author: admin
 */
#include "uart_workers.h"
#include "uart_communicator.h"

void pingWorker(u8* const inputData, u8* const outputData, reg* const size, const reg maxOutBufferSize, PREPROCESSOR_CTX_TYPE(ctx))
{
    stm32_uart_communicator_t* const parrent = PREPROCESSOR_CTX_TYPE_CAST(ctx);
    RawParser_dma_t* const parser = &parrent->raw_p;

    u8 t1 = 0x01;
    u8 t2 = 0x11;
    u8 t3 = 0x22;
    u8 t4 = 0x33;
    u16 t5 = 0xAA55;

    RawParser_dma_startTransmittPacket(parser, 10);
    RawParser_dma_universalWrite(parser, sizeof(u8), sizeof(u8), &t1);
    RawParser_dma_universalWrite(parser, sizeof(u8), sizeof(u8), &t2);
    RawParser_dma_universalWrite(parser, sizeof(u8), sizeof(u8), &t3);
    RawParser_dma_universalWrite(parser, sizeof(u8), sizeof(u8), &t4);
    RawParser_dma_universalWrite(parser, sizeof(u16), sizeof(u16), (u8*)&t5);

    RawParser_Frame_t*const frame = RawParser_dma_finishTransmittPacket(parser);
    UART_SendBuffer(&parrent->stm32_uart, frame->data, frame->size);
}



