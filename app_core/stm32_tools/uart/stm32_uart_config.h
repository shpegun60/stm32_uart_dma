/*
 * stm32_uart_config.h
 *
 *  Created on: Jun 30, 2024
 *      Author: admin
 */

#ifndef TOOLS_UART_STM32_UART_CONFIG_H_
#define TOOLS_UART_STM32_UART_CONFIG_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#if defined(HAL_DMA_MODULE_ENABLED) && (defined(HAL_UART_MODULE_ENABLED) || defined(HAL_USART_MODULE_ENABLED))

#include "status.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define UART_CONTAINER_ENA

#ifdef UART_CONTAINER_ENA
#	define UART_CONTAINER_COUNT 1
#endif /* UART_CONTAINER_ENA */



/*! @brief Return status for the UART driver.
 *  @ingroup status */
enum StatusUART
{
    /*! Baud rate not supported by system. */
    ERROR_UART_BAUDRATE_NOT_SUPPORTED = -71,

    /*! Receiver buffer hasen't been read before receiving new data.
     *  Data loss! */
    ERROR_UART_RX_OVERRUN = -72,

    /*! Noise detected in the received character. */
    ERROR_UART_RX_NOISE = -73,

    /*! Framing error occurs when the receiver detects a logic 0 where a stop
     *  bit was expected. This suggests the receiver was not properly aligned
     *  to a character frame. */
    ERROR_UART_FRAMING_ERR = -74,

    /*! Transmitting error stemming from the DMA module. */
    ERROR_UART_TX_DMA_ERR = -75,

    /*! Receiving error stemming from the DMA module. */
    ERROR_UART_RX_DMA_ERR = -75,
};

/*!***************************************************************************
 * @brief   SCI physical layer received byte callback function type.
 * @details Callback that is invoked whenever data has been received via the
 *          physical layer.
 * @param   data The received data as byte (uint8_t) array.
 * @param   size The size of the received data.
 * @return  -
 *****************************************************************************/
typedef void (*uart_rx_callback_t)(uint8_t* const data, uint32_t const size, void* const captured);

/*!***************************************************************************
 * @brief   SCI physical layer transmit done callback function type.
 * @details Callback that is invoked whenever the physical layer has finished
 *          transmitting the current data buffer.
 * @param   status The \link #status_t status\endlink of the transmitter;
 *                   (#STATUS_OK on success).
 * @param   state A pointer to the state that was passed to the Tx function.
 * @return  -
 *****************************************************************************/
typedef void (*uart_tx_callback_t)(const status_t status, void* const captured);

/*!***************************************************************************
 * @brief   SCI error callback function type.
 * @detail  Callback that is invoked whenever a error occurs.
 * @param   status The error \link #status_t status\endlink that invoked the
 *                 callback.
 * @return  -
 *****************************************************************************/
typedef void (*uart_error_callback_t)(const status_t status, void* const captured);

typedef struct stm32_DMA_uart {
	/*UART handle Structure pointer */
	UART_HandleTypeDef *huart;

	/*! The busy indication for the uart */
	volatile bool isTxBusy_;

	struct {
		/*! The TX callback for the uart */
		uart_tx_callback_t txCallback_;
		/*! The TX callback data for the uart */
		void* txCallbackData_;
	} TX;

	struct {
		//*! The RX callback for the uart */
		uart_rx_callback_t rxCallback_;
		/*! The RX callback data for the uart */
		void* rxCallbackData_;
		/*! The RX data buffer size. */
		uint32_t rxBufferSize;
		uint8_t* rxBuffer1;
		uint8_t* rxBuffer2;
	} RX;

	struct {
		/*! The callback for the uart */
		uart_error_callback_t errorCallback;
		/*! The RX callback data for the uart */
		void* errorCallbackData_;
	} Error;

} stm32_DMA_uart_t;
#endif /* defined(HAL_DMA_MODULE_ENABLED) && (defined(HAL_UART_MODULE_ENABLED) || defined(HAL_USART_MODULE_ENABLED)) */

#ifdef __cplusplus
} // extern "C"
#endif
#endif /* TOOLS_UART_STM32_UART_CONFIG_H_ */
