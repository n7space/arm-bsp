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

#include "Uart.h"

#include <assert.h>
#include <string.h>

#define UART_BAUDRATE_BASE_SCALER 16u

static inline void
enableTxIrq(Uart *const uart)
{
	uart->reg->ier = UART_IER_TXEMPTY_MASK;
}

static inline void
disableTxIrq(Uart *const uart)
{
	uart->reg->idr = UART_IDR_TXEMPTY_MASK;
}

static inline void
enableRxIrq(Uart *const uart)
{
	uart->reg->ier = UART_IER_RXRDY_MASK;
}

static inline void
disableRxIrq(Uart *const uart)
{
	uart->reg->idr = UART_IDR_RXRDY_MASK;
}

void
Uart_startup(Uart *const uart)
{
	// Disable all interrupt sources.
	uart->reg->idr = (UART_IDR_RXRDY_MASK | UART_IDR_TXRDY_MASK
			| UART_IDR_OVRE_MASK | UART_IDR_FRAME_MASK
			| UART_IDR_PARE_MASK | UART_IDR_TXEMPTY_MASK);
}

void
Uart_shutdown(Uart *const uart)
{
	(void)uart;
}

static uint32_t
addressBase(const Uart_Id id)
{
	switch (id) {
	case Uart_Id_0: return UART0_ADDRESS_BASE;
	case Uart_Id_1: return UART1_ADDRESS_BASE;
	case Uart_Id_2: return UART2_ADDRESS_BASE;
	case Uart_Id_3: return UART3_ADDRESS_BASE;
	case Uart_Id_4: return UART4_ADDRESS_BASE;
	}
	assert(0 && "Incorrect UART id");
	return 0;
}

void
Uart_init(const Uart_Id id, Uart *const uart)
{
	assert(uart != NULL);
	memset(uart, 0, sizeof(Uart));

	uart->id = id;

	const uint32_t registersAddress = addressBase(id);
	uart->reg = (Uart_Registers *)registersAddress;
}

void
Uart_setConfig(Uart *const uart, const Uart_Config *const config)
{
	if (config->isTxEnabled)
		uart->reg->cr = UART_CR_TXEN_MASK;
	else
		uart->reg->cr = UART_CR_TXDIS_MASK;

	if (config->isRxEnabled)
		uart->reg->cr = UART_CR_RXEN_MASK;
	else
		uart->reg->cr = UART_CR_RXDIS_MASK;

	uint32_t mr = (((uint32_t)config->parity << UART_MR_PAR_OFFSET)
				      & UART_MR_PAR_MASK)
			| (((uint32_t)config->baudRateClkSrc
					   << UART_MR_BSRCCK_OFFSET)
					& UART_MR_BSRCCK_MASK);

	if (config->isTestModeEnabled)
		mr |= (UART_MR_CHMODE_LOCAL_LOOPBACK_VALUE
				<< UART_MR_CHMODE_OFFSET);

	uart->reg->mr = mr;

	uart->reg->brgr = (config->baudRateClkFreq
			/ (UART_BAUDRATE_BASE_SCALER * config->baudRate));

	uart->config = *config;
}

void
Uart_getConfig(const Uart *const uart, Uart_Config *const config)
{
	// There is no way to get the status of TX/RX from the device registers,
	// so since this will require storing some status data in the
	// descriptor, might as well store and retrieve it in its entirety.
	*config = uart->config;

	uint32_t mr = uart->reg->mr;

	config->isTestModeEnabled =
			(((mr & UART_MR_CHMODE_MASK) >> UART_MR_CHMODE_OFFSET)
					== UART_MR_CHMODE_LOCAL_LOOPBACK_VALUE);

	config->parity = ((mr & UART_MR_PAR_MASK) >> UART_MR_PAR_OFFSET);
	config->baudRateClkSrc =
			((mr & UART_MR_BSRCCK_MASK) >> UART_MR_BSRCCK_OFFSET);
}

bool
Uart_write(Uart *const uart, const uint8_t data, uint32_t const timeoutLimit,
		int *const errCode)
{
	uint32_t timeout = timeoutLimit;
	while (((uart->reg->sr & UART_SR_TXRDY_MASK) == 0u) && (timeout > 0u))
		timeout--;

	if (timeout == 0u)
		return returnError(errCode, Uart_ErrorCodes_Timeout);

	uart->reg->thr = data;

	return true;
}

bool
Uart_read(Uart *const uart, uint8_t *const data, uint32_t timeoutLimit,
		int *const errCode)
{
	uint32_t timeout = timeoutLimit;
	while (((uart->reg->sr & UART_SR_RXRDY_MASK) == 0u) && (timeout > 0u))
		timeout--;

	if (timeout == 0u)
		return returnError(errCode, Uart_ErrorCodes_Timeout);

	*data = (uint8_t)uart->reg->rhr;

	return true;
}

void
Uart_writeAsync(Uart *const uart, ByteFifo *const fifo,
		const Uart_TxHandler handler)
{
	disableTxIrq(uart);

	uart->txFifo = fifo;
	uart->txHandler = handler;

	uint8_t data;
	if ((uart->txFifo != NULL) && ByteFifo_pull(uart->txFifo, &data)) {
		uart->reg->thr = data;
		enableTxIrq(uart);
	}
}

void
Uart_readAsync(Uart *const uart, ByteFifo *const fifo,
		const Uart_RxHandler handler)
{
	disableRxIrq(uart);

	uart->rxFifo = fifo;
	uart->rxHandler = handler;

	if (uart->rxFifo != NULL)
		enableRxIrq(uart);
}

void
Uart_readRxFifo(Uart *const uart, ByteFifo *const fifo)
{
	if (uart->rxFifo == NULL)
		return;

	while (!ByteFifo_isFull(fifo)) {
		disableRxIrq(uart);

		uint8_t data;
		if (!ByteFifo_pull(uart->rxFifo, &data)) {
			enableRxIrq(uart);
			break;
		}
		enableRxIrq(uart);
		ByteFifo_push(fifo, data);
	}
}

void
Uart_registerErrorHandler(Uart *const uart, const Uart_ErrorHandler handler)
{
	uart->reg->idr = UART_IDR_OVRE_MASK | UART_IDR_FRAME_MASK
			| UART_IDR_PARE_MASK;

	uart->errorHandler = handler;

	if (uart->errorHandler.callback != NULL)
		uart->reg->ier = UART_IER_OVRE_MASK | UART_IER_FRAME_MASK
				| UART_IER_PARE_MASK;
}

uint32_t
Uart_getTxFifoCount(Uart *const uart)
{
	disableTxIrq(uart);

	uint32_t count;
	if (uart->txFifo == NULL)
		count = 0;
	else
		count = (uint32_t)ByteFifo_getCount(uart->txFifo);

	enableTxIrq(uart);

	return count;
}

uint32_t
Uart_getRxFifoCount(Uart *const uart)
{
	disableRxIrq(uart);

	uint32_t count;
	if (uart->rxFifo == NULL)
		count = 0;
	else
		count = (uint32_t)ByteFifo_getCount(uart->rxFifo);

	enableRxIrq(uart);

	return count;
}

static inline bool
handleRxInterrupt(Uart *const uart, int* const errCode)
{
	uint8_t data = (uint8_t)uart->reg->rhr;

	if (uart->rxFifo == NULL) {
		disableRxIrq(uart);
		return true;
	}

	if(!ByteFifo_push(uart->rxFifo, data)) {
		return returnError(errCode, Uart_ErrorCodes_Rx_Fifo_Full);
	}

	if ((uart->rxHandler.characterCallback != NULL)
			&& (data == uart->rxHandler.targetCharacter))
		uart->rxHandler.characterCallback(uart->rxHandler.characterArg);
	if ((uart->rxHandler.lengthCallback != NULL)
			&& (ByteFifo_getCount(uart->rxFifo)
					>= uart->rxHandler.targetLength))
		uart->rxHandler.lengthCallback(uart->rxHandler.lengthArg);

	return true;
}

static inline void
handleTxInterrupt(Uart *const uart)
{
	uint8_t data = 0;
	if (uart->txFifo == NULL) {
		disableTxIrq(uart);
	} else if (ByteFifo_pull(uart->txFifo, &data)) {
		uart->reg->thr = data;
	} else {
		do {
			if (uart->txHandler.callback != NULL)
				uart->txFifo = uart->txHandler.callback(
						uart->txHandler.arg);
			else
				uart->txFifo = NULL;

			if (uart->txFifo == NULL) {
				disableTxIrq(uart);
				return;
			}
		} while (!ByteFifo_pull(uart->txFifo, &data));

		uart->reg->thr = data;
	}
}

static inline bool
Uart_hasAnyErrorOccured(Uart_ErrorFlags* const errFlags)
{
	return (errFlags->hasFramingErrorOccurred || errFlags->hasOverrunOccurred || errFlags->hasParityErrorOccurred
            || errFlags->hasRxFifoFullErrorOccurred);
}

void
Uart_handleInterrupt(Uart *const uart)
{
	int errorCode = 0;
	Uart_ErrorFlags errorFlags = { false, false, false, false };

	uint32_t status = uart->reg->sr & uart->reg->imr;
	uart->reg->cr = UART_CR_RSTSTA_MASK;
	if ((status & UART_SR_RXRDY_MASK) != 0u)
	{
		handleRxInterrupt(uart, &errorCode);
		if(errorCode  == Uart_ErrorCodes_Rx_Fifo_Full)
			errorFlags.hasRxFifoFullErrorOccurred = true;
	}
	if ((status & UART_SR_TXEMPTY_MASK) != 0u)
		handleTxInterrupt(uart);

	if (uart->errorHandler.callback == NULL)
		return;

	Uart_getLinkErrors(status, &errorFlags);
	if(Uart_hasAnyErrorOccured(&errorFlags))
   		uart->errorHandler.callback(errorFlags, uart->errorHandler.arg);
}

inline bool
Uart_isTxEmpty(const Uart *const uart)
{
	return ((uart->reg->sr & UART_SR_TXEMPTY_MASK) != 0u)
			&& ((uart->reg->sr & UART_SR_TXRDY_MASK) != 0u);
}

inline bool
Uart_isDataAvailable(const Uart *const uart)
{
	if ((uart->reg->sr & UART_SR_RXRDY_MASK) == 0u)
		return false;
	return true;
}

void
Uart_getLinkErrors(uint32_t statusRegister, Uart_ErrorFlags *const errFlags)
{
	errFlags->hasFramingErrorOccurred =
			(statusRegister & UART_SR_FRAME_MASK) != 0u;
	errFlags->hasOverrunOccurred =
			(statusRegister & UART_SR_OVRE_MASK) != 0u;
	errFlags->hasParityErrorOccurred =
			(statusRegister & UART_SR_PARE_MASK) != 0u;
}

uint32_t
Uart_getStatusRegister(const Uart *const uart)
{
	const uint32_t status = uart->reg->sr;
	uart->reg->cr = UART_CR_RSTSTA_MASK;
	return status;
}
