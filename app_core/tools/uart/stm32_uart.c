/*************************************************************************//**
 * @file
 * @brief       This file is part of the STM32F401RE platform layer.
 * @details     This file provides UART driver functionality.
 *
 * @copyright
 *
 * Copyright (c) 2021, Broadcom Inc
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "stm32_uart.h"

#if defined(HAL_DMA_MODULE_ENABLED) && (defined(HAL_UART_MODULE_ENABLED) || defined(HAL_USART_MODULE_ENABLED))

#include "tools/debug/printf/printf.h"
#include "tools/irq/stm32_irq_block.h"

#ifdef UART_CONTAINER_ENA
#	include "stm32_uart_container.h"
#endif /* UART_CONTAINER_ENA */

#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>


/*!***************************************************************************
 * @brief   Initialize the Universal Asynchronous Receiver/Transmitter
 *          (UART or LPSCI) bus and DMA module
 * @param   -
 * @return  Returns the \link #status_t status\endlink (#STATUS_OK on success).
 *****************************************************************************/

status_t UART_Init(stm32_DMA_uart_t* const self, UART_HandleTypeDef * const huart, const uint32_t rxBufferSize)
{
	if((self == NULL) || (huart == NULL) || (rxBufferSize > 2048U)) {
		return ERROR_INVALID_ARGUMENT;
	}

	memset(self, 0, sizeof(stm32_DMA_uart_t));
	self->huart = huart;
	self->isTxBusy_ = false;

#ifdef UART_CONTAINER_ENA

	if(pushContainerUartInstance(self) != STATUS_OK) {
		self->isTxBusy_ = true;
		self->RX.rxBufferSize = 0;
		return ERROR_FAIL;
	}

#endif /* UART_CONTAINER_ENA */

	void* const buf1 = malloc(rxBufferSize);
	if(buf1 == NULL) {
		return ERROR_FAIL;
	}
	void* const buf2 = malloc(rxBufferSize);
	if(buf2 == NULL) {
		free(buf1);
		return ERROR_FAIL;
	}

	self->RX.rxBuffer1 = buf1;
	self->RX.rxBuffer2 = buf2;
	self->RX.rxBufferSize = rxBufferSize;

	//HAL_UART_Receive_DMA(huart, self->RX.rxBuffer1, rxBufferSize);
	huart->pRxBuffPtr = self->RX.rxBuffer1;
	UART_SetRxCallback(self, 0, 0);

	return STATUS_OK;
}

stm32_DMA_uart_t* const UART_new(UART_HandleTypeDef * const huart, const uint32_t rxBufferSize)
{
	stm32_DMA_uart_t* const m = malloc(sizeof(stm32_DMA_uart_t));
	if(m == NULL) {
		return NULL;
	}

	if(UART_Init(m, huart, rxBufferSize) != STATUS_OK) {
		free(m);
		return NULL;
	}

	return m;
}

void UART_delete(stm32_DMA_uart_t** self)
{
	if(self == NULL) {
		return;
	}

	stm32_DMA_uart_t* const self_reg = (*self);
	free(self_reg->RX.rxBuffer1);
	free(self_reg->RX.rxBuffer2);
	free(self_reg);

	(*self) = NULL;
}


uint32_t UART_GetBaudRate(stm32_DMA_uart_t* const self)
{
	return self->huart->Init.BaudRate;
}

status_t UART_SetBaudRate(stm32_DMA_uart_t* const self, const uint32_t baudRate)
{
	/* Check module state; TX line must be idle to rest baud rate... */
	IRQ_LOCK();
	if (self->isTxBusy_) {
		IRQ_UNLOCK();
		return STATUS_BUSY;
	}

	self->isTxBusy_ = true;
	IRQ_UNLOCK();

	/* remove callback and disable RX line. */
	const uart_rx_callback_t callback = self->RX.rxCallback_;
	void* const captured = self->RX.rxCallbackData_;
	UART_SetRxCallback(self, 0, 0);

	/* Obtain correct baud rate setting value. */
	self->huart->Init.BaudRate = baudRate;
	HAL_UART_Init(self->huart);

	/* Add callback and enable RX line again. */
	UART_SetRxCallback(self, callback, captured);

	self->isTxBusy_ = false;

	return STATUS_OK;
}


/*!***************************************************************************
 * @brief   Writes several bytes to the UART connection.
 * @param   txBuff Data array to write to the uart connection
 * @param   txSize The size of the data array
 * @param   f Callback function after tx is done, set 0 if not needed;
 * @param   state Optional user state that will be passed to callback
 *                  function; set 0 if not needed.
 * @return  Returns the \link #status_t status\endlink:
 *           - #STATUS_OK (0) on success.
 *           - #STATUS_BUSY on Tx line busy
 *           - #ERROR_NOT_INITIALIZED
 *           - #ERROR_INVALID_ARGUMENT
 *****************************************************************************/
status_t UART_SendBuffer_callback(stm32_DMA_uart_t* const self, uint8_t const *txBuff, const size_t txSize, const uart_tx_callback_t f, void* const captured)
{
	/* Verify arguments. */
	if (!txBuff || txSize == 0) {
		return ERROR_INVALID_ARGUMENT;
	}

	/* Lock interrupts to prevent completion interrupt before setup is complete */
	IRQ_LOCK();
	if (self->isTxBusy_) {
		IRQ_UNLOCK();
		return STATUS_BUSY;
	}

	/* Set Tx Busy Status. */
	self->isTxBusy_ = true;
	self->TX.txCallback_ = f;
	self->TX.txCallbackData_ = captured;

	const HAL_StatusTypeDef hal_error = HAL_UART_Transmit_DMA(self->huart, (uint8_t*) txBuff, txSize);
	IRQ_UNLOCK(); // this must come after HAL_UART_Transmit_DMA to avoid race conditions w/ IRQs

	if (hal_error != HAL_OK) {
		return ERROR_FAIL;
	}

	return STATUS_OK;
}

status_t UART_SendBuffer(stm32_DMA_uart_t* const self, uint8_t const *txBuff, const size_t txSize)
{
	/* Verify arguments. */
	if (!txBuff || txSize == 0) {
		return ERROR_INVALID_ARGUMENT;
	}

	/* Lock interrupts to prevent completion interrupt before setup is complete */
	IRQ_LOCK();
	if (self->isTxBusy_) {
		IRQ_UNLOCK();
		return STATUS_BUSY;
	}

	/* Set Tx Busy Status. */
	self->isTxBusy_ = true;

	const HAL_StatusTypeDef hal_error = HAL_UART_Transmit_DMA(self->huart, (uint8_t*) txBuff, txSize);
	IRQ_UNLOCK(); // this must come after HAL_UART_Transmit_DMA to avoid race conditions w/ IRQs

	if (hal_error != HAL_OK) {
		return ERROR_FAIL;
	}

	return STATUS_OK;
}

status_t UART_SetTxcallback(stm32_DMA_uart_t* const self, const uart_tx_callback_t f, void* const captured)
{
	if (self->isTxBusy_) {
		return STATUS_BUSY;
	}

	self->TX.txCallback_ = f;
	self->TX.txCallbackData_ = captured;

	return STATUS_OK;
}


void UART_SetRxCallback(stm32_DMA_uart_t* const self, const uart_rx_callback_t f, void* const captured)
{
	UART_HandleTypeDef* const huart = self->huart;

	self->RX.rxCallback_ = f;
	self->RX.rxCallbackData_ = captured;

	/* Start receiving */
	if (f) {
		uint8_t* const r_rxBuffer = self->RX.rxBuffer1;
		const uint32_t r_buff_size = self->RX.rxBufferSize;

		HAL_UART_Receive_DMA(huart, r_rxBuffer, r_buff_size); // Start receiving via DMA
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);  // Enable serial port idle interrupt
	} else {
		HAL_UART_AbortReceive(huart);
		UART_RXCallback(self);
		__HAL_UART_DISABLE_IT(huart, UART_IT_IDLE);  // Disable serial port idle interrupt
	}
}

void UART_SetErrorCallback(stm32_DMA_uart_t* const self, const uart_error_callback_t f, void* const captured)
{
	self->Error.errorCallback = f;
	self->Error.errorCallbackData_ = captured;
}


/*!***************************************************************************
* IT Callbacks
 *****************************************************************************/

//general in stm32f1xx_it.c
void USER_UART_IRQHandler(stm32_DMA_uart_t* const self)
{
	UART_HandleTypeDef* const huart = self->huart;

	if (RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE)) {
		// On idle interruption
		__HAL_UART_CLEAR_IDLEFLAG(huart); // Clear idle interrupt sign
		HAL_UART_AbortReceive(huart);
		UART_RXCallback(self);
	}
}

// rx callback in void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
void UART_RXCallback(stm32_DMA_uart_t* const self)
{
	UART_HandleTypeDef* const huart = self->huart;
	uint8_t* const r_rxBuffer1 = self->RX.rxBuffer1;
	uint8_t* const r_rxBuffer2 = self->RX.rxBuffer2;
	const uint32_t r_buff_size = self->RX.rxBufferSize;

	assert(huart->pRxBuffPtr == r_rxBuffer1 || huart->pRxBuffPtr == r_rxBuffer2);

	const uint32_t size = r_buff_size - __HAL_DMA_GET_COUNTER(huart->hdmarx);
	uint8_t* const curr = huart->pRxBuffPtr;
	uint8_t* const next = (huart->pRxBuffPtr == r_rxBuffer1) ? r_rxBuffer2 : r_rxBuffer1;

	const HAL_StatusTypeDef rtn = HAL_UART_Receive_DMA(huart, next, r_buff_size);
	if (rtn != HAL_OK || huart->gState == HAL_UART_STATE_ERROR) {
		UART_ErrorCallback(self);
	}


	void* const captured = self->RX.rxCallbackData_;
	const uart_rx_callback_t callback = self->RX.rxCallback_;
	if (callback) {
		callback(curr, size, captured);
	}
}

/**
 * @brief  Tx Transfer completed callbacks.
 * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
 *                the configuration information for the specified UART module.
 * @retval None
 */
//tx callback in void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
void UART_TXCallback(stm32_DMA_uart_t* const self)
{
	void* const captured = self->TX.txCallbackData_;
	const uart_tx_callback_t callback = self->TX.txCallback_;
	const status_t status = (self->huart->gState == HAL_UART_STATE_ERROR) ? ERROR_FAIL : STATUS_OK;
	self->isTxBusy_ = false;

	if (callback) {
		callback(status, captured);
	}
}

// error callback in void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
void UART_ErrorCallback(stm32_DMA_uart_t* const self)
{
	UART_HandleTypeDef* const huart = self->huart;
	status_t status = ERROR_FAIL;

	if (huart->ErrorCode & HAL_UART_ERROR_ORE) {
		status = ERROR_UART_RX_OVERRUN;
	} else if (huart->ErrorCode & HAL_UART_ERROR_FE) {
		status = ERROR_UART_FRAMING_ERR;
	} else if (huart->ErrorCode & HAL_UART_ERROR_NE) {
		status = ERROR_UART_RX_NOISE;
	} else if (huart->ErrorCode & HAL_UART_ERROR_DMA) {
		self->isTxBusy_ = false;
		status = ERROR_UART_TX_DMA_ERR;
	} else if (huart->gState == HAL_UART_STATE_BUSY) {
		status = STATUS_BUSY;
	} else if (huart->gState == HAL_UART_STATE_BUSY_RX) {
		status = STATUS_BUSY;
	} else if (huart->gState == HAL_UART_STATE_BUSY_TX) {
		status = STATUS_BUSY;
	} else if (huart->gState == HAL_UART_STATE_BUSY_TX_RX) {
		status = STATUS_BUSY;
	} else if (huart->gState == HAL_UART_STATE_ERROR) {
		self->isTxBusy_ = false;
		status = ERROR_FAIL;
	} else if (huart->gState == HAL_UART_STATE_TIMEOUT) {
		self->isTxBusy_ = false;
		status = ERROR_TIMEOUT;
	}

	void* const captured = self->Error.errorCallbackData_;
	const uart_error_callback_t callback = self->Error.errorCallback;
	if (callback) {
		callback(status, captured);
	}
}



#endif /* if defined DMA and USART or UART  */
