/**@file
 * This file is part of the ARM BSP for the Test Environment.
 *
 * @copyright 2018-2025 N7 Space Sp. z o.o.
 *
 * Test Environment was developed under a programme of,
 * and funded by, the European Space Agency (the "ESA").
 *
 *
 * Licensed under the ESA Public License (ESA-PL) Permissive (Type 3),
 * Version 2.4 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://essr.esa.int/license/list
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/// \brief Uart hardware driver function prototypes and datatypes.

/**
 * @defgroup Uart Uart
 * @ingroup Bsp
 * @{
 */

#ifndef BSP_UART_H
#define BSP_UART_H

#include <Utils/ByteFifo.h>
#include <Utils/Utils.h>

#include "UartRegisters.h"

/// \brief Uart device identifiers.
typedef enum {
	Uart_Id_0 = 0, ///< Uart instance 0.
	Uart_Id_1 = 1, ///< Uart instance 1.
	Uart_Id_2 = 2, ///< Uart instance 2.
	Uart_Id_3 = 3, ///< Uart instance 3.
	Uart_Id_4 = 4, ///< Uart instance 4.
} Uart_Id;

/// \brief Uart baud rate clock sources.
typedef enum {
	Uart_BaudRateClk_PeripheralCk =
			0, ///< Use peripheral clock for baud rate generation.
	Uart_BaudRateClk_Pck = 1, ///< Use PCK for baud rate generation.
} Uart_BaudRateClk;

/// \brief Uart parity.
typedef enum {
	Uart_Parity_Even = 0, ///< Assume even parity bit.
	Uart_Parity_Odd = 1, ///< Assume odd parity bit.
	Uart_Parity_None = 4, ///< Assume no parity bit.
} Uart_Parity;

/// \brief Uart configuration descriptor.
typedef struct {
	bool isTxEnabled; ///< Flag indicating whether the transmitter should be enabled.
	bool isRxEnabled; ///< Flag indicating whether the receiver should be enabled.
	bool isTestModeEnabled; ///< Flag indicating whether to enable local loopback mode.
	Uart_Parity parity; ///< Indicator of used parity bit.
	uint32_t baudRate; ///< Target baud rate.
	Uart_BaudRateClk
			baudRateClkSrc; ///< Indicator of the baud rate clock source.
	uint32_t baudRateClkFreq; ///< Baud rate clock source frequency.
} Uart_Config;

/// \brief A function serving as a callback called at the end of transmission.
/// \returns ByteFifo from which data transmission should be continued.
typedef ByteFifo *(*UartTxEndCallback)(void *arg);

/// \brief A descriptor of an end-of-transmission event handler.
typedef struct {
	UartTxEndCallback callback; ///< Callback function.
	void *arg; ///< Argument to the callback function.
} Uart_TxHandler;

/// \brief A function serving as a callback called upon a reception of a byte
///        if the reception queue contains at least a number of bytes specified
///        in the handler descriptor.
typedef void (*UartRxEndLengthCallback)(void *arg);
/// \brief A function serving as a callback called upon a reception of a byte if
/// byte matches
///        a target specified in the handler descriptor.
typedef void (*UartRxEndCharacterCallback)(void *arg);

/// \brief A descriptor of a byte reception event handler.
typedef struct {
	/// \brief Callback called when reception queue data count is greater
	/// than or equal targetLength.
	UartRxEndLengthCallback lengthCallback;
	/// \brief Callback called when a targetCharacter is received.
	UartRxEndCharacterCallback characterCallback;
	/// \brief Argument for the length callback.
	void *lengthArg;
	/// \brief Argument for the character callback.
	void *characterArg;
	/// \brief Target character, upon reception of which character callback
	/// is called.
	uint8_t targetCharacter;
	/// \brief Target length of reception queue, upon reaching of which
	/// length callback is called.
	uint32_t targetLength;
} Uart_RxHandler;

/// \brief Uart error flags.
typedef struct {
	bool hasOverrunOccurred; // Hardware FIFO overrun detected.
	bool hasFramingErrorOccurred; // Framing error detected.
	bool hasParityErrorOccurred; // Parity error detected.
	bool hasRxFifoFullErrorOccurred; // Rx FIFO full error detected.
} Uart_ErrorFlags;

/// \brief A function serving as a callback called upon detection of an error by
/// hardware.
typedef void (*UartErrorCallback)(Uart_ErrorFlags errorFlags, void *arg);

/// \brief A descriptor of an error handler.
typedef struct {
	UartErrorCallback callback; ///< Callback function.
	void *arg; ///< Argument to the callback function.
} Uart_ErrorHandler;

/// \brief Uart error codes.
typedef enum
{
    Uart_ErrorCodes_Timeout = 1,      ///< Timeout has occurred during a write/read operation.
    Uart_ErrorCodes_Rx_Fifo_Full = 2, ///< Rx fifo was full during new byte reception
} Uart_ErrorCodes;

/// \brief Uart device descriptor.
typedef struct {
	Uart_Id id; ///< Device identifier.
	Uart_TxHandler txHandler; ///< End-of-transmission handler descriptor.
	Uart_RxHandler rxHandler; ///< Reception handler descriptor.
	Uart_ErrorHandler errorHandler; ///< Error handler descriptor.
	ByteFifo *txFifo; ///< Pointer to a transmission byte queue.
	ByteFifo *rxFifo; ///< Pointer to a reception byte queue.
	volatile Uart_Registers
			*reg; ///< Pointer to memory-mapped device registers.
	Uart_Config config; ///< Configuration descriptor.
} Uart;

/// \brief Performs a hardware startup procedure of an Uart device.
/// \param [in] uart Uart device descriptor.
void Uart_startup(Uart *const uart);

/// \brief Performs a hardware shutdown procedure of an Uart device.
/// \param [in] uart Uart device descriptor.
void Uart_shutdown(Uart *const uart);

/// \brief Intiializes a device descriptor for Uart.
/// \param [in] id Uart device identifier.
/// \param [out] uart Uart device descriptor.
void Uart_init(const Uart_Id id, Uart *const uart);

/// \brief Configures an Uart device based on a configuration descriptor.
/// \param [in] uart Uart device descriptor.
/// \param [in] config A configuration descriptor.
void Uart_setConfig(Uart *const uart, const Uart_Config *const config);

/// \brief Retrieves configuration of an Uart device.
/// \param [in] uart Uart device descriptor.
/// \param [out] config A configuration descriptor.
void Uart_getConfig(const Uart *const uart, Uart_Config *const config);

/// \brief Checks whenever RX has pending data.
/// \param [in] uart Uart device descriptor.
/// \retval true Data is available for reading.
/// \retval false Data is not available for reading.
bool Uart_isDataAvailable(const Uart *const uart);

/// \brief Synchronously sends a byte over Uart.
/// \param [in] uart Uart device descriptor.
/// \param [in] data Byte to send.
/// \param [in] timeoutLimit An arbitrary timeout value.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Sending was successful.
/// \retval false Sending timed out.
bool Uart_write(Uart *const uart, const uint8_t data, uint32_t timeoutLimit,
		int *const errCode);

/// \brief Synchronously receives a byte over Uart.
/// \param [in] uart Uart device descriptor.
/// \param [in] data Received byte pointer.
/// \param [in] timeoutLimit An arbitrary timeout value.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Reception was successful.
/// \retval false Reception timed out.
bool Uart_read(Uart *const uart, uint8_t *const data, uint32_t timeoutLimit,
		int *const errCode);

/// \brief Asynchronously sends a series of bytes over Uart.
/// \param [in] uart Uart device descriptor.
/// \param [in] fifo Pointer to the output byte queue.
/// \param [in] handler Descriptor of the transmission handler.
void Uart_writeAsync(Uart *const uart, ByteFifo *const fifo,
		const Uart_TxHandler handler);

/// \brief Asynchronously receives a series of bytes over Uart.
/// \param [in] uart Uart device descriptor.
/// \param [in] fifo Pointer to the input byte queue.
/// \param [in] handler Descriptor of the reception handler.
void Uart_readAsync(Uart *const uart, ByteFifo *const fifo,
		const Uart_RxHandler handler);

/// \brief Checks if all bytes were sent.
/// \param [in] uart Uart device descriptor.
/// \retval true Tx queue is empty.
/// \retval false Tx is busy.
bool Uart_isTxEmpty(const Uart *const uart);

/// \brief Pulls bytes stored in the reception queue.
/// \param [in] uart Uart device descriptor.
/// \param [out] fifo Byte queue into which bytes from the reception queue will
/// be moved.
void Uart_readRxFifo(Uart *const uart, ByteFifo *const fifo);

/// \brief Gets transmission queue byte count.
/// \param [in] uart Uart device descriptor.
/// \returns The number of bytes in the sending queue, yet to be sent out.
uint32_t Uart_getTxFifoCount(Uart *const uart);

/// \brief Gets reception queue byte count.
/// \param [in] uart Uart device descriptor.
/// \returns The number of bytes in the reception queue, waiting to be pulled.
uint32_t Uart_getRxFifoCount(Uart *const uart);

/// \brief Registers a handler called upon detection of a hardware error.
/// \param [in] uart Uart device descriptor.
/// \param [in] handler Error handler descriptor.
void Uart_registerErrorHandler(
		Uart *const uart, const Uart_ErrorHandler handler);

/// \brief Default interrupt handler for Uart devices.
/// \param [in] uart Uart device descriptor.
void Uart_handleInterrupt(Uart* const uart);

/// \brief Checks status register for hardware errors.
/// \param [in] statusRegister Twihs statrus register value.
/// \param [out] errFlags Pointer to error flag structure.
void Uart_getLinkErrors(
		uint32_t statusRegister, Uart_ErrorFlags *const errFlags);

/// \brief Reads Uart device status register. Register flags are cleared upon
/// read. \param [in] uart Uart device descriptor. \returns The status register
/// value.
uint32_t Uart_getStatusRegister(const Uart *const uart);

#endif // BSP_UART_H

/** @} */
