/**@file
 * This file is part of the ARM BSP for the Test Environment.
 *
 * @copyright 2022-2026 N7 Space Sp. z o.o.
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

#ifndef N7S_BSP_UART_UART_H
#define N7S_BSP_UART_UART_H

/// \file Uart.h
/// \addtogroup Bsp
/// \brief Header containing interface for UART driver.

#include <Utils/ByteFifo.h>
#include <Utils/ErrorCode.h>
#include <Utils/Utils.h>

#include "UartRegisters.h"

/// @addtogroup Uart
/// @ingroup Bsp
/// @{

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Uart device identifiers.
typedef enum {
#if defined(N7S_TARGET_SAMV71Q21)
  // Bigger hex values are necessary to get correct Uart address value
  Uart_Id_0 = 0x00,  ///< Uart instance 0.
  Uart_Id_1 = 0x01,  ///< Uart instance 1.
  Uart_Id_2 = 0x09,  ///< Uart instance 2.
  Uart_Id_3 = 0x0a,  ///< Uart instance 3.
  Uart_Id_4 = 0x0b,  ///< Uart instance 4.
  Uart_Id_Count = 5, ///< Number of available instances of UART.
#elif defined(N7S_TARGET_SAMRH71F20)
  Uart_Id_0 = 0,      ///< Uart instance 0.
  Uart_Id_1 = 1,      ///< Uart instance 1.
  Uart_Id_2 = 2,      ///< Uart instance 2.
  Uart_Id_3 = 3,      ///< Uart instance 3.
  Uart_Id_4 = 4,      ///< Uart instance 4.
  Uart_Id_5 = 5,      ///< Uart instance 5.
  Uart_Id_6 = 6,      ///< Uart instance 6.
  Uart_Id_7 = 7,      ///< Uart instance 7.
  Uart_Id_8 = 8,      ///< Uart instance 8.
  Uart_Id_9 = 9,      ///< Uart instance 9.
  Uart_Id_Count = 10, ///< Number of available instances of UART.
#elif defined(N7S_TARGET_SAMRH707F18)
  Uart_Id_0 = 0,     ///< Uart instance 0.
  Uart_Id_1 = 1,     ///< Uart instance 1.
  Uart_Id_2 = 2,     ///< Uart instance 2.
  Uart_Id_3 = 3,     ///< Uart instance 3.
  Uart_Id_Count = 4, ///< Number of available instances of UART.
#endif
} Uart_Id;

/// \brief Uart baud rate clock sources.
typedef enum {
  Uart_BaudRateClk_PeripheralCk = 0, ///< Use peripheral clock for baud rate generation.
#if defined(N7S_TARGET_SAMV71Q21)
  Uart_BaudRateClk_Pck = 1, ///< Use PCK for baud rate generation.
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
  Uart_BaudRateClk_PeripheralCkBy8 = 1, ///< Use peripheral clock divided by 8.
  Uart_BaudRateClk_GenericClock = 2,    ///< Use the Pmc generic clock.
  Uart_BaudRateClk_ExternalSck = 3,     ///< Use the clock given on an external SCK pin.
#endif
} Uart_BaudRateClk;

/// \brief Uart parity.
typedef enum {
  Uart_Parity_Even = 0, ///< Assume even parity bit.
  Uart_Parity_Odd = 1,  ///< Assume odd parity bit.
  Uart_Parity_None = 4, ///< Assume no parity bit.
} Uart_Parity;

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
/// \brief Uart stop bits.
typedef enum {
  Uart_StopBits_OneBit = 0,          ///< Use one stop bit.
  Uart_StopBits_OneAndAHalfBits = 1, ///< Use one and a half stop bits.
  Uart_StopBits_TwoBits = 2,         ///< Use two stop bits.
} Uart_StopBits;

/// \brief Uart character length.
typedef enum {
  Uart_CharacterLength_5Bits = 0, ///< Use 5 bits for a character.
  Uart_CharacterLength_6Bits = 1, ///< Use 6 bits for a character.
  Uart_CharacterLength_7Bits = 2, ///< Use 7 bits for a character.
  Uart_CharacterLength_8Bits = 3, ///< Use 8 bits for a character.
} Uart_CharacterLength;
#endif

/// \brief Uart configuration descriptor.
typedef struct {
  bool isTxEnabled;       ///< Flag indicating whether the transmitter should be enabled.
  bool isRxEnabled;       ///< Flag indicating whether the receiver should be enabled.
  bool isTestModeEnabled; ///< Flag indicating whether to enable local loopback mode.
  Uart_Parity parity;     ///< Indicator of used parity bit.
#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
  Uart_StopBits stopBits;          ///< Number of stop bits used.
  Uart_CharacterLength charLength; ///< Character length.
#endif
  uint32_t baudRate;               ///< Target baud rate.
  Uart_BaudRateClk baudRateClkSrc; ///< Indicator of the baud rate clock source.
  uint32_t baudRateClkFreq;        ///< Baud rate clock source frequency.
} Uart_Config;

/// \brief A function serving as a callback called at the end of transmission.
/// \returns ByteFifo from which data transmission should be continued.
typedef ByteFifo* (*UartTxEndCallback)(void* arg);

/// \brief A descriptor of an end-of-transmission event handler.
typedef struct {
  UartTxEndCallback callback; ///< Callback function.
  void* arg;                  ///< Argument to the callback function.
} Uart_TxHandler;

/// \brief A function serving as a callback called upon a reception of a byte
///        if the reception queue contains at least a number of bytes specified in the handler
///        descriptor.
typedef void (*UartRxEndLengthCallback)(void* arg);
/// \brief A function serving as a callback called upon a reception of a byte if byte matches
///        a target specified in the handler descriptor.
typedef void (*UartRxEndCharacterCallback)(void* arg);

/// \brief A descriptor of a byte reception event handler.
typedef struct {
  /// \brief Callback called when reception queue data count is greater than or equal targetLength.
  UartRxEndLengthCallback lengthCallback;
  /// \brief Callback called when a targetCharacter is received.
  UartRxEndCharacterCallback characterCallback;
  /// \brief Argument for the length callback.
  void* lengthArg;
  /// \brief Argument for the character callback.
  void* characterArg;
  /// \brief Target character, upon reception of which character callback is called.
  uint8_t targetCharacter;
  /// \brief Target length of reception queue, upon reaching of which length callback is called.
  uint32_t targetLength;
} Uart_RxHandler;

/// \brief Uart error flags.
typedef struct {
  bool hasOverrunOccurred;      // Hardware FIFO overrun detected.
  bool hasFramingErrorOccurred; // Framing error detected.
  bool hasParityErrorOccurred;  // Parity error detected.
} Uart_ErrorFlags;

/// \brief A function serving as a callback called upon detection of an error by hardware.
typedef void (*UartErrorCallback)(const Uart_ErrorFlags* errorFlags, void* arg);

/// \brief A descriptor of an error handler.
typedef struct {
  UartErrorCallback callback; ///< Callback function.
  void* arg;                  ///< Argument to the callback function.
} Uart_ErrorHandler;

/// \brief Uart error codes.
typedef enum {
  /// \brief Timeout has occurred during a write/read operation.
  Uart_ErrorCode_Timeout = ERROR_CODE_DEFINE('U', 'A', 'R', 1),
  /// \brief Rx fifo was full during new byte reception
  Uart_ErrorCode_Rx_Fifo_Full = ERROR_CODE_DEFINE('U', 'A', 'R', 2),
} Uart_ErrorCode;

/// \brief Uart device descriptor.
typedef struct {
  Uart_Id id;                         ///< Device identifier.
  Uart_TxHandler txHandler;           ///< End-of-transmission handler descriptor.
  Uart_RxHandler rxHandler;           ///< Reception handler descriptor.
  Uart_ErrorHandler errorHandler;     ///< Error handler descriptor.
  ByteFifo* txFifo;                   ///< Pointer to a transmission byte queue.
  ByteFifo* rxFifo;                   ///< Pointer to a reception byte queue.
  volatile Uart_Registers* registers; ///< Pointer to memory-mapped device registers.
  Uart_Config config;                 ///< Configuration descriptor.
} Uart;

/// \brief Initializes a device descriptor for Uart.
/// \param [in] id Uart device identifier.
/// \param [out] uart Uart device descriptor.
void Uart_init(const Uart_Id id, Uart* const uart);

/// \brief Configures an Uart device based on a configuration descriptor.
/// \param [in] uart Uart device descriptor.
/// \param [in] config A configuration descriptor.
void Uart_setConfig(Uart* const uart, const Uart_Config* const config);

/// \brief Retrieves configuration of an Uart device.
/// \param [in] uart Uart device descriptor.
/// \param [out] config A configuration descriptor.
void Uart_getConfig(const Uart* const uart, Uart_Config* const config);

/// \brief Synchronously sends a byte over Uart.
/// \param [in] uart Uart device descriptor.
/// \param [in] data Byte to send.
/// \param [in] timeoutLimit An arbitrary timeout value.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Sending was successful.
/// \retval false Sending timed out.
bool Uart_write(Uart* const uart,
  const uint8_t data,
  uint32_t timeoutLimit,
  ErrorCode* const errCode);

/// \brief Synchronously receives a byte over Uart.
/// \param [in] uart Uart device descriptor.
/// \param [in] data Received byte pointer.
/// \param [in] timeoutLimit An arbitrary timeout value.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Reception was successful.
/// \retval false Reception timed out.
bool Uart_read(const Uart* const uart,
  uint8_t* const data,
  uint32_t timeoutLimit,
  ErrorCode* const errCode);

/// \brief Asynchronously sends a series of bytes over Uart.
/// \param [in] uart Uart device descriptor.
/// \param [in] fifo Pointer to the output byte queue.
/// \param [in] handler Descriptor of the transmission handler.
void Uart_writeAsync(Uart* const uart, ByteFifo* const fifo, const Uart_TxHandler handler);

/// \brief Asynchronously receives a series of bytes over Uart.
/// \param [in] uart Uart device descriptor.
/// \param [in] fifo Pointer to the input byte queue.
/// \param [in] handler Descriptor of the reception handler.
void Uart_readAsync(Uart* const uart, ByteFifo* const fifo, const Uart_RxHandler handler);

/// \brief Checks if all bytes were sent.
/// \param [in] uart Uart device descriptor.
/// \retval true Tx queue is empty.
/// \retval false Tx is busy.
bool Uart_isTxEmpty(const Uart* const uart);

/// \brief Pulls bytes stored in the reception queue.
/// \param [in] uart Uart device descriptor.
/// \param [out] fifo Byte queue into which bytes from the reception queue will be moved.
void Uart_readRxFifo(Uart* const uart, ByteFifo* const fifo);

/// \brief Gets transmission queue byte count.
/// \param [in] uart Uart device descriptor.
/// \returns The number of bytes in the sending queue, yet to be sent out.
uint32_t Uart_getTxFifoCount(Uart* const uart);

/// \brief Gets reception queue byte count.
/// \param [in] uart Uart device descriptor.
/// \returns The number of bytes in the reception queue, waiting to be pulled.
uint32_t Uart_getRxFifoCount(Uart* const uart);

/// \brief Registers a handler called upon detection of a hardware error.
/// \param [in] uart Uart device descriptor.
/// \param [in] handler Error handler descriptor.
void Uart_registerErrorHandler(Uart* const uart, const Uart_ErrorHandler handler);

/// \brief Default interrupt handler for Uart devices.
/// \param [in] uart Uart device descriptor.
/// \param [out] errCode An error code generated during the operation.
/// \retval true interrupt handling was successful
/// \retval false interrupt handling error occurred
bool Uart_handleInterrupt(Uart* const uart, ErrorCode* const errCode);

/// \brief Checks status register for hardware errors.
/// \param [in] statusRegister Twihs status register value.
/// \param [out] errFlags Pointer to error flag structure.
void Uart_getLinkErrors(uint32_t statusRegister, Uart_ErrorFlags* const errFlags);

/// \brief Reads Uart device status register. Register flags are cleared upon read.
/// \param [in] uart Uart device descriptor.
/// \returns The status register value.
uint32_t Uart_getStatusRegister(const Uart* const uart);

/// \brief Checks whenever RX has pending data.
/// \param [in] uart Uart device descriptor.
/// \retval true Data is available for reading.
/// \retval false Data is not available for reading.
bool Uart_isDataAvailable(const Uart* const uart);

/// \brief Performs a hardware reset procedure of an Uart device.
///        Reset is performed in the scope of supported features.
/// \param [in] uart Uart device descriptor.
void Uart_reset(Uart* const uart);

/// \brief Obtain address for DMA writes.
/// \details Returns Uart Transmit Data Register address for use with Xdmac. For proper
/// configuration and usage see TEST(XdmacTests, Xdmac_triggerChannel_triggersATransfer) example.
/// \param [in] uart Uart device descriptor.
/// \returns Address for DMA writes.
void* Uart_getDmaWriteAddress(Uart* const uart);

/// \brief Obtain address for DMA reads.
/// \details Returns Uart Received Data Register address for use with Xdmac. For proper
/// configuration and usage see TEST(XdmacTests, Xdmac_triggerChannel_triggersATransfer) example.
/// \param [in] uart Uart device descriptor.
/// \returns Address for DMA reads.
void* Uart_getDmaReadAddress(Uart* const uart);

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // N7S_BSP_UART_UART_H
