/**@file
 * This file is part of the ARM BSP for the Test Environment.
 *
 * @copyright 2018-2026 N7 Space Sp. z o.o.
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

#include <Utils/Bits.h>

#include <Scb/Scb.h>

#define UART_BAUDRATE_BASE_SCALER 16u

static uint32_t enterCriticalSection(Uart* const uart)
{
  const uint32_t mask = uart->registers->imr;
  uart->registers->idr = mask;
  MEMORY_SYNC_BARRIER();
  return mask;
}

static void exitCriticalSection(Uart* const uart, const uint32_t state)
{
  MEMORY_SYNC_BARRIER();
  uart->registers->ier = state;
}

static void disableTxIrq(Uart* const uart)
{
  uart->registers->idr = UART_IDR_TXRDY_MASK;
  MEMORY_SYNC_BARRIER();
}

static void disableRxIrq(Uart* const uart)
{
  uart->registers->idr = UART_IDR_RXRDY_MASK;
  MEMORY_SYNC_BARRIER();
}

static uintptr_t addressBase(const Uart_Id id)
{
  return UART0_ADDRESS_BASE + ((uint32_t)id * UART_ADDRESS_OFFSET);
}

void Uart_init(const Uart_Id id, Uart* const uart)
{
  assert(uart != NULL);
  memset(uart, 0, sizeof(Uart));

  uart->id = id;

  const uintptr_t registersAddress = addressBase(id);
  uart->registers = (Uart_Registers*)registersAddress;
}

void Uart_reset(Uart* const uart)
{
  // Disable all interrupt sources.
  uart->registers->idr = (UART_IDR_RXRDY_MASK | UART_IDR_TXRDY_MASK | UART_IDR_OVRE_MASK
                          | UART_IDR_FRAME_MASK | UART_IDR_PARE_MASK | UART_IDR_TXEMPTY_MASK);

  // Reset device
  uart->registers->cr = (UART_CR_RSTRX_MASK | UART_CR_RSTTX_MASK | UART_CR_RSTSTA_MASK);

  // Clear leftover values in other registers
#if defined(N7S_TARGET_SAMV71Q21)
  uart->registers->mr &= ~(UART_MR_PAR_MASK | UART_MR_CHMODE_MASK);
  uart->registers->brgr &= ~UART_BRGR_CD_MASK;
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
  uart->registers->mr &= ~(UART_MR_PAR_MASK | UART_MR_USCLKS_MASK | UART_MR_NBSTOP_MASK
                           | UART_MR_CHRL_MASK | UART_MR_CHMODE_MASK | UART_MR_SYNC_MASK);
  uart->registers->brgr &= ~(UART_BRGR_CD_MASK | UART_BRGR_FP_MASK);
#endif
}

void Uart_setConfig(Uart* const uart, const Uart_Config* const config)
{
  // According to the documentation separate enable and disable operations need
  // to be performed in separate register writes.

  uart->registers->cr
    = BIT_VALUE(UART_CR_TXEN, config->isTxEnabled) | BIT_VALUE(UART_CR_RXEN, config->isRxEnabled);

  uart->registers->cr = BIT_VALUE(UART_CR_TXDIS, !config->isTxEnabled)
                      | BIT_VALUE(UART_CR_RXDIS, !config->isRxEnabled);

  uart->registers->mr = BIT_FIELD_VALUE(UART_MR_PAR, config->parity)
#if defined(N7S_TARGET_SAMV71Q21)
                      | BIT_FIELD_VALUE(UART_MR_BSRCCK, config->baudRateClkSrc);
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
                      | BIT_FIELD_VALUE(UART_MR_USCLKS, config->baudRateClkSrc)
                      | BIT_FIELD_VALUE(UART_MR_NBSTOP, config->stopBits)
                      | BIT_FIELD_VALUE(UART_MR_CHRL, config->charLength);
#endif

  if (config->isTestModeEnabled)
    uart->registers->mr |= ((uint32_t)UART_MR_CHMODE_LOCAL_LOOPBACK_VALUE << UART_MR_CHMODE_OFFSET);

  const uint32_t coarseDiv
    = config->baudRateClkFreq / (UART_BAUDRATE_BASE_SCALER * config->baudRate);

#if defined(N7S_TARGET_SAMV71Q21)
  // Set-up Clock Divider
  uart->registers->brgr = BIT_FIELD_VALUE(UART_BRGR_CD, coarseDiv);
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)

  const uint32_t fineDivCoeff = 8u;
  const uint32_t fineDiv
    = (config->baudRateClkFreq / ((UART_BAUDRATE_BASE_SCALER / fineDivCoeff) * config->baudRate))
    - (fineDivCoeff * coarseDiv);

  // Set-up Clock Divider and Fractional Part
  uart->registers->brgr
    = BIT_FIELD_VALUE(UART_BRGR_CD, coarseDiv) | BIT_FIELD_VALUE(UART_BRGR_FP, fineDiv);
#endif

  uart->config = *config;
}

void Uart_getConfig(const Uart* const uart, Uart_Config* const config)
{
  // There is no way to get the status of TX/RX from the device registers,
  // so since this will require storing some status data in the descriptor,
  // might as well store and retrieve it in its entirety.
  *config = uart->config;

  uint32_t mr = uart->registers->mr;

  config->isTestModeEnabled = (((mr & UART_MR_CHMODE_MASK) >> UART_MR_CHMODE_OFFSET)
                               == UART_MR_CHMODE_LOCAL_LOOPBACK_VALUE);

  config->parity = ((mr & UART_MR_PAR_MASK) >> UART_MR_PAR_OFFSET);

#if defined(N7S_TARGET_SAMV71Q21)
  config->baudRateClkSrc = ((mr & UART_MR_BSRCCK_MASK) >> UART_MR_BSRCCK_OFFSET);
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
  config->baudRateClkSrc = ((mr & UART_MR_USCLKS_MASK) >> UART_MR_USCLKS_OFFSET);
#endif
}

bool Uart_write(Uart* const uart,
  const uint8_t data,
  const uint32_t timeoutLimit,
  ErrorCode* const errCode)
{
  uint32_t timeout = timeoutLimit;
  if (!waitForRegisterWithTimeout(&(uart->registers->sr), UART_SR_TXRDY_MASK, timeout))
    return returnError(errCode, Uart_ErrorCode_Timeout);

  uart->registers->thr = data;

  return true;
}

bool Uart_read(const Uart* const uart,
  uint8_t* const data,
  uint32_t timeoutLimit,
  ErrorCode* const errCode)
{
  uint32_t timeout = timeoutLimit;
  if (!waitForRegisterWithTimeout(&(uart->registers->sr), UART_SR_RXRDY_MASK, timeout))
    return returnError(errCode, Uart_ErrorCode_Timeout);

  *data = (uint8_t)uart->registers->rhr;

  return true;
}

void Uart_writeAsync(Uart* const uart, ByteFifo* const fifo, const Uart_TxHandler handler)
{
  uint32_t state = enterCriticalSection(uart);

  uart->txFifo = fifo;
  uart->txHandler = handler;

  uint8_t data;
  if ((uart->txFifo != NULL) && ByteFifo_pull(uart->txFifo, &data))
  {
    uart->registers->thr = data;
    state |= UART_IER_TXRDY_MASK;
  }

  exitCriticalSection(uart, state);
}

void Uart_readAsync(Uart* const uart, ByteFifo* const fifo, const Uart_RxHandler handler)
{
  uint32_t state = enterCriticalSection(uart);

  uart->rxFifo = fifo;
  uart->rxHandler = handler;

  if (uart->rxFifo != NULL)
    state |= UART_IER_RXRDY_MASK;

  exitCriticalSection(uart, state);
}

void Uart_readRxFifo(Uart* const uart, ByteFifo* const fifo)
{
  if (uart->rxFifo == NULL)
    return;

  while (!ByteFifo_isFull(fifo))
  {
    const uint32_t state = enterCriticalSection(uart);

    uint8_t data;
    if (!ByteFifo_pull(uart->rxFifo, &data))
    {
      exitCriticalSection(uart, state);
      return;
    }
    exitCriticalSection(uart, state);

    ByteFifo_push(fifo, data);
  }
}

void Uart_registerErrorHandler(Uart* const uart, const Uart_ErrorHandler handler)
{
  uart->registers->idr = UART_IDR_OVRE_MASK | UART_IDR_FRAME_MASK | UART_IDR_PARE_MASK;

  uart->errorHandler = handler;

  if (uart->errorHandler.callback != NULL)
    uart->registers->ier = UART_IER_OVRE_MASK | UART_IER_FRAME_MASK | UART_IER_PARE_MASK;
}

uint32_t Uart_getTxFifoCount(Uart* const uart)
{
  const uint32_t state = enterCriticalSection(uart);

  const uint32_t count = (uart->txFifo == NULL) ? 0u : (uint32_t)ByteFifo_getCount(uart->txFifo);

  exitCriticalSection(uart, state);

  return count;
}

uint32_t Uart_getRxFifoCount(Uart* const uart)
{
  const uint32_t state = enterCriticalSection(uart);

  const uint32_t count = (uart->rxFifo == NULL) ? 0u : (uint32_t)ByteFifo_getCount(uart->rxFifo);

  exitCriticalSection(uart, state);

  return count;
}

static inline bool handleRxInterrupt(Uart* const uart, ErrorCode* const errCode)
{
  if (uart->rxFifo == NULL)
  {
    disableRxIrq(uart);
    return true;
  }

  const uint8_t data = (uint8_t)uart->registers->rhr;

  // WARNING: If FIFO is full, byte will be lost.
  const bool retValue = ByteFifo_push(uart->rxFifo, data);

  if (!retValue)
    setErrorCode(errCode, Uart_ErrorCode_Rx_Fifo_Full);

  if ((uart->rxHandler.characterCallback != NULL) && (data == uart->rxHandler.targetCharacter))
    uart->rxHandler.characterCallback(uart->rxHandler.characterArg);
  if ((uart->rxHandler.lengthCallback != NULL)
      && (ByteFifo_getCount(uart->rxFifo) >= uart->rxHandler.targetLength))
    uart->rxHandler.lengthCallback(uart->rxHandler.lengthArg);

  return retValue;
}

static ByteFifo* getNextTxFifo(Uart* const uart)
{
  return (uart->txHandler.callback != NULL) ? uart->txHandler.callback(uart->txHandler.arg) : NULL;
}

static inline void handleTxInterrupt(Uart* const uart)
{
  uint8_t data = 0;
  if (uart->txFifo == NULL)
  {
    disableTxIrq(uart);
  }
  else if (ByteFifo_pull(uart->txFifo, &data))
  {
    uart->registers->thr = data;
    if (ByteFifo_isEmpty(uart->txFifo))
      uart->txFifo = getNextTxFifo(uart);
  }
  else
  {
    do
    {
      uart->txFifo = getNextTxFifo(uart);

      if (uart->txFifo == NULL)
      {
        disableTxIrq(uart);
        return;
      }
      // While next fifo is empty
    } while (!ByteFifo_pull(uart->txFifo, &data));

    uart->registers->thr = data;
  }
}

bool Uart_handleInterrupt(Uart* const uart, ErrorCode* const errCode)
{
  bool retValue = true;
  const uint32_t status = Uart_getStatusRegister(uart);

  if ((status & UART_SR_RXRDY_MASK) != 0u)
    retValue = handleRxInterrupt(uart, errCode);
  if ((status & UART_SR_TXRDY_MASK) != 0u)
    handleTxInterrupt(uart);

  if (uart->errorHandler.callback == NULL)
    return retValue;

  Uart_ErrorFlags errFlags = { 0 };
  Uart_getLinkErrors(status, &errFlags);

  if (errFlags.hasOverrunOccurred || errFlags.hasFramingErrorOccurred
      || errFlags.hasParityErrorOccurred)
    uart->errorHandler.callback(&errFlags, uart->errorHandler.arg);

  return retValue;
}

void* Uart_getDmaWriteAddress(Uart* const uart)
{
  return (void*)(uintptr_t)&(uart->registers->thr);
}

void* Uart_getDmaReadAddress(Uart* const uart)
{
  return (void*)(uintptr_t)&(uart->registers->rhr);
}

bool Uart_isTxEmpty(const Uart* const uart)
{
  return ((uart->registers->sr & UART_SR_TXEMPTY_MASK) != 0u)
      && ((uart->registers->sr & UART_SR_TXRDY_MASK) != 0u);
}

bool Uart_isDataAvailable(const Uart* const uart)
{
  if ((uart->registers->sr & UART_SR_RXRDY_MASK) == 0u)
    return false;
  return true;
}

void Uart_getLinkErrors(const uint32_t statusRegister, Uart_ErrorFlags* const errFlags)
{
  errFlags->hasFramingErrorOccurred = (statusRegister & UART_SR_FRAME_MASK) != 0u;
  errFlags->hasOverrunOccurred = (statusRegister & UART_SR_OVRE_MASK) != 0u;
  errFlags->hasParityErrorOccurred = (statusRegister & UART_SR_PARE_MASK) != 0u;
}

uint32_t Uart_getStatusRegister(const Uart* const uart)
{
  const uint32_t status = uart->registers->sr;
  uart->registers->cr = UART_CR_RSTSTA_MASK;
  return status;
}
