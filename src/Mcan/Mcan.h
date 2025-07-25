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

/// \file Mcan.h
/// \addtogroup Bsp
/// \brief Mcan hardware driver function prototypes and datatypes.

#ifndef BSP_MCAN_H
#define BSP_MCAN_H

#include <stdbool.h>
#include <stdint.h>

#include <Utils/ErrorCode.h>

#include "McanRegisters.h"

/// @addtogroup Mcan
/// @ingroup Bsp
/// @{

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Mcan error codes.
typedef enum {
	/// \brief Invalid RX FIFO id was given.
	Mcan_ErrorCode_InvalidRxFifoId = ERROR_CODE_DEFINE('C', 'A', 'N', 1),
	/// \brief RX FIFO is empty.
	Mcan_ErrorCode_RxFifoEmpty = ERROR_CODE_DEFINE('C', 'A', 'N', 2),
	/// \brief TX FIFO is full.
	Mcan_ErrorCode_TxFifoFull = ERROR_CODE_DEFINE('C', 'A', 'N', 3),
	/// \brief TX Event FIFO is empty.
	Mcan_ErrorCode_TxEventFifoEmpty = ERROR_CODE_DEFINE('C', 'A', 'N', 4),
	/// \brief Clock domains synchronization timeout.
	Mcan_ErrorCode_InitializationStartTimeout =
			ERROR_CODE_DEFINE('C', 'A', 'N', 5),
	/// \brief Clock stop request timeout.
	Mcan_ErrorCode_ClockStopRequestTimeout =
			ERROR_CODE_DEFINE('C', 'A', 'N', 6),
	/// \brief Requested index out of range.
	Mcan_ErrorCode_IndexOutOfRange = ERROR_CODE_DEFINE('C', 'A', 'N', 7),
	/// \brief Invalid element size was given.
	Mcan_ErrorCode_ElementSizeInvalid = ERROR_CODE_DEFINE('C', 'A', 'N', 8),
	/// \brief Invalid operation mode was requested.
	Mcan_ErrorCode_ModeInvalid = ERROR_CODE_DEFINE('C', 'A', 'N', 9),
} Mcan_ErrorCode;

/// \brief Mcan device identifiers.
typedef enum {
	Mcan_Id_0 = 0, ///< MCAN0 Id.
	Mcan_Id_1 = 1, ///< MCAN1 Id.
} Mcan_Id;

/// \brief Mcan Rx FIFO identifier.
typedef enum {
	Mcan_RxFifoId_0 = 0, ///< Rx FIFO0 Id.
	Mcan_RxFifoId_1 = 1, ///< Rx FIFO1 Id.
} Mcan_RxFifoId;

/// \brief The status of Mcan Rx FIFO.
typedef struct {
	uint8_t count; ///< Number of elements in Rx FIFO.
	bool isFull; ///< FIFO full flag.
	bool isMessageLost; ///< Message lost flag.
} Mcan_RxFifoStatus;

/// \brief The status of Mcan Tx Queue.
typedef struct {
	bool isFull; ///< Queue full flag.
} Mcan_TxQueueStatus;

/// \brief The status of Mcan Tx Event FIFO.
typedef struct {
	uint8_t count; ///< Number of elements in Tx Event FIFO.
	bool isFull; ///< FIFO full flag.
	bool isMessageLost; ///< Message lost flag.
} Mcan_TxEventFifoStatus;

/// \brief Mcan device operation mode.
typedef enum {
	/// \brief Normal operation mode.
	Mcan_Mode_Normal,
	/// \brief Normal operation mode with automatic retransmission disabled.
	Mcan_Mode_AutomaticRetransmissionDisabled,
	/// \brief Restricted operation mode.
	Mcan_Mode_Restricted,
	/// \brief Bus monitoring operation mode.
	Mcan_Mode_BusMonitoring,
	/// \brief Power down operation mode.
	Mcan_Mode_PowerDown,
	/// \brief Internal loopback test operation mode.
	Mcan_Mode_InternalLoopBackTest,
	/// \brief Invalid mode.
	Mcan_Mode_Invalid,
} Mcan_Mode;

/// \brief Nominal and data bit timing configuration.
typedef struct {
	/// \brief Bit Rate Prescaler tq (time quantum) = t_periphclk * (bitRatePrescaler + 1)
	uint16_t bitRatePrescaler;
	/// \brief (Re)Synchronization Jump Width = tq * (synchronizationJump + 1)
	uint8_t synchronizationJump;
	/// \brief Time Segment After Sample Point = tq * (timeSegmentAfterSamplePoint + 1)
	uint8_t timeSegmentAfterSamplePoint;
	/// \brief Time Segment Before Sample Point = tq * (timeSegmentBeforeSamplePoint + 1)
	uint8_t timeSegmentBeforeSamplePoint;
} Mcan_BitTiming;

/// \brief Transmitter Delay Compensation settings.
typedef struct {
	bool isEnabled; ///< Is Transmitter Delay Compensation enabled flag.
	uint8_t filter; ///< Transmitter Delay Compensation Filter.
	uint8_t offset; ///< Transmitter Delay Compensation Offset.
} Mcan_TransmitterDelayCompensation;

/// \brief Mcan timestamp clock source.
typedef enum {
	Mcan_TimestampClk_None = 0, ///< No clock, timestamp is always 0.
	Mcan_TimestampClk_Internal =
			1, ///< Internal Mcan clock with prescaler is used.
	Mcan_TimestampClk_External = 2, ///< Selected PCK is used.
} Mcan_TimestampClk;

/// \brief Mcan timeout type.
typedef enum {
	Mcan_TimeoutType_Continuous = 0,
	Mcan_TimeoutType_TxEventFifo = 1,
	Mcan_TimeoutType_RxFifo0 = 2,
	Mcan_TimeoutType_RxFifo1 = 3,
} Mcan_TimeoutType;

/// \brief The policy of handling non-matching frames.
typedef enum {
	Mcan_NonMatchingPolicy_RxFifo0 =
			0, ///< Put non-matching frames in RX FIFO 0.
	Mcan_NonMatchingPolicy_RxFifo1 =
			1, ///< Put non-matching frames in RX FIFO 1.
	Mcan_NonMatchingPolicy_Rejected = 2, ///< Reject non-matching frames.
} Mcan_NonMatchingPolicy;

/// \brief Id filtering policy used for standard and extended IDs filtering.
typedef struct {
	bool isIdRejected; ///< Reject all frames flag.
	Mcan_NonMatchingPolicy
			nonMatchingPolicy; ///< Non-matching frames handling policy.
	uint32_t *filterListAddress; ///< Filter list address within message RAM.
	uint8_t filterListSize; ///< Filter list elements count.
} Mcan_IdFilter;

/// \brief Rx FIFO or Rx Buffer element size.
typedef enum {
	Mcan_ElementSize_8 = 0, ///< 8-byte data field.
	Mcan_ElementSize_12 = 1, ///< 12-byte data field.
	Mcan_ElementSize_16 = 2, ///< 16-byte data field.
	Mcan_ElementSize_20 = 3, ///< 20-byte data field.
	Mcan_ElementSize_24 = 4, ///< 24-byte data field.
	Mcan_ElementSize_32 = 5, ///< 32-byte data field.
	Mcan_ElementSize_48 = 6, ///< 48-byte data field.
	Mcan_ElementSize_64 = 7, ///< 64-byte data field.
	Mcan_ElementSize_Invalid = 8, ///< invalid size of data field.
} Mcan_ElementSize;

/// \brief Data length code in CAN frame.
typedef enum {
	Mcan_DataLengthCode_1 = 1u, ///< 1-byte data field.
	Mcan_DataLengthCode_2 = 2u, ///< 2-byte data field.
	Mcan_DataLengthCode_3 = 3u, ///< 3-byte data field.
	Mcan_DataLengthCode_4 = 4u, ///< 4-byte data field.
	Mcan_DataLengthCode_5 = 5u, ///< 5-byte data field.
	Mcan_DataLengthCode_6 = 6u, ///< 6-byte data field.
	Mcan_DataLengthCode_7 = 7u, ///< 7-byte data field.
	Mcan_DataLengthCode_8 = 8u, ///< 8-byte data field.
	Mcan_DataLengthCode_12 = 9u, ///< 12-byte data field.
	Mcan_DataLengthCode_16 = 10u, ///< 16-byte data field.
	Mcan_DataLengthCode_20 = 11u, ///< 20-byte data field.
	Mcan_DataLengthCode_24 = 12u, ///< 24-byte data field.
	Mcan_DataLengthCode_32 = 13u, ///< 32-byte data field.
	Mcan_DataLengthCode_48 = 14u, ///< 48-byte data field.
	Mcan_DataLengthCode_64 = 15u, ///< 64-byte data field.
	Mcan_DataLengthCode_Invalid = 0xf7u, ///< invalid size of data field.
} Mcan_DataLengthCode;

/// \brief Raw data length in CAN frame.
typedef enum {
	Mcan_DataLengthRaw_1 = 1u, ///< 1-byte data field.
	Mcan_DataLengthRaw_2 = 2u, ///< 2-byte data field.
	Mcan_DataLengthRaw_3 = 3u, ///< 3-byte data field.
	Mcan_DataLengthRaw_4 = 4u, ///< 4-byte data field.
	Mcan_DataLengthRaw_5 = 5u, ///< 5-byte data field.
	Mcan_DataLengthRaw_6 = 6u, ///< 6-byte data field.
	Mcan_DataLengthRaw_7 = 7u, ///< 7-byte data field.
	Mcan_DataLengthRaw_8 = 8u, ///< 8-byte data field.
	Mcan_DataLengthRaw_12 = 12u, ///< 12-byte data field.
	Mcan_DataLengthRaw_16 = 16u, ///< 16-byte data field.
	Mcan_DataLengthRaw_20 = 20u, ///< 20-byte data field.
	Mcan_DataLengthRaw_24 = 24u, ///< 24-byte data field.
	Mcan_DataLengthRaw_32 = 32u, ///< 32-byte data field.
	Mcan_DataLengthRaw_48 = 48u, ///< 48-byte data field.
	Mcan_DataLengthRaw_64 = 64u, ///< 64-byte data field.
	Mcan_DataLengthRaw_Invalid = 0xf7u, ///< invalid size of data field.
} Mcan_DataLengthRaw;

/// \brief Rx FIFO operation mode.
typedef enum {
	Mcan_RxFifoOperationMode_Blocking =
			0, ///< No new message is added, when FIFO is full.
	Mcan_RxFifoOperationMode_Overwrite =
			1, ///< Oldest message is overwritten when FIFO is full.
} Mcan_RxFifoOperationMode;

/// \brief Rx FIFO settings.
typedef struct {
	bool isEnabled; ///< Rx FIFO enable flag.
	uint32_t *startAddress; ///< Rx FIFO start address within message RAM.
	uint8_t size; ///< Rx FIFO max elements count.
	uint8_t watermark; ///< Rx FIFO watermark level for interrupt; 0 - disabled.
	Mcan_RxFifoOperationMode mode; ///< Operation mode.
	Mcan_ElementSize elementSize; ///< The size of Rx FIFO element.
} Mcan_RxFifoConfig;

/// \brief Rx Buffer settings.
typedef struct {
	uint32_t *startAddress; ///< Rx Buffer start address within message RAM.
	Mcan_ElementSize elementSize; ///< The size of Rx Buffer element.
} Mcan_RxBufferConfig;

/// \brief Tx Queue type.
typedef enum {
	Mcan_TxQueueType_Fifo = 0, ///< Tx FIFO.
	Mcan_TxQueueType_Id =
			1, ///< Tx Id queue (lower Id means higher priority).
} Mcan_TxQueueType;

/// \brief Tx Buffer/FIFO/Queue settings.
typedef struct {
	bool isEnabled; ///< Tx Buffer/FIFO/Queue enable flag.
	uint32_t *startAddress; ///< Tx Buffer start address within message RAM.
	uint8_t bufferSize; ///< Number of Tx Buffer elements.
	uint8_t queueSize; ///< Number of Tx FIFO/Queue elements.
	Mcan_TxQueueType queueType; ///< Type of Tx Queue (FIFO or Id queue).
	Mcan_ElementSize
			elementSize; ///< The size of Tx Buffer/FIFO/Queue element.
} Mcan_TxBufferConfig;

/// \brief Tx Event FIFO settings.
typedef struct {
	bool isEnabled; ///< Tx Event FIFO enable flag.
	uint32_t *startAddress; ///< Tx Event FIFO start address within message RAM.
	uint8_t size; ///< Tx Event FIFO max elements count.
	uint8_t watermark; ///< Tx Event FIFO watermark level for interrupt; 0 - disabled.
} Mcan_TxEventFifoConfig;

/// \brief MCAN interrupt sources.
typedef enum {
	Mcan_Interrupt_Rf0n = 0, ///< Receive FIFO 0 New Message.
	Mcan_Interrupt_Rf0w = 1, ///< Receive FIFO 0 Watermark Reached.
	Mcan_Interrupt_Rf0f = 2, ///< Receive FIFO 0 Full.
	Mcan_Interrupt_Rf0l = 3, ///< Receive FIFO 0 Message Lost.
	Mcan_Interrupt_Rf1n = 4, ///< Receive FIFO 1 New Message.
	Mcan_Interrupt_Rf1w = 5, ///< Receive FIFO 1 Watermark Reached.
	Mcan_Interrupt_Rf1f = 6, ///< Receive FIFO 1 Full.
	Mcan_Interrupt_Rf1l = 7, ///< Receive FIFO 1 Message Lost.
	Mcan_Interrupt_Hpm = 8, ///< High Priority Message.
	Mcan_Interrupt_Tc = 9, ///< Transmission Completed.
	Mcan_Interrupt_Tcf = 10, ///< Transmission Cancellation Finished.
	Mcan_Interrupt_Tfe = 11, ///< Tx FIFO Empty.
	Mcan_Interrupt_Tefn = 12, ///< Tx Event FIFO New Entry.
	Mcan_Interrupt_Tefw = 13, ///< Tx Event FIFO Watermark Reached.
	Mcan_Interrupt_Teff = 14, ///< Tx Event FIFO Full.
	Mcan_Interrupt_Tefl = 15, ///< Tx Event FIFO Element Lost.
	Mcan_Interrupt_Tsw = 16, ///< Timestamp Wraparound.
	Mcan_Interrupt_Mraf = 17, ///< Message RAM Access Failure.
	Mcan_Interrupt_Too = 18, ///< Timeout Occurred.
	Mcan_Interrupt_Drx =
			19, ///< Message stored to Dedicated Receive Buffer.
	Mcan_Interrupt_Reserved1 = 20, ///< Reserved, shall not be used.
	Mcan_Interrupt_Reserved2 = 21, ///< Reserved, shall not be used.
	Mcan_Interrupt_Elo = 22, ///< Error Logging Overflow.
	Mcan_Interrupt_Ep = 23, ///< Error Passive.
	Mcan_Interrupt_Ew = 24, ///< Warning Status.
	Mcan_Interrupt_Bo = 25, ///< Bus_Off Status.
	Mcan_Interrupt_Wdi = 26, ///< Watchdog Interrupt.
	Mcan_Interrupt_Pea = 27, ///< Protocol Error in Arbitration Phase.
	Mcan_Interrupt_Ped = 28, ///< Protocol Error in Data Phase.
	Mcan_Interrupt_Ara = 29, ///< Access to Reserved Address.
	Mcan_Interrupt_Count =
			30, ///< Number of interrupts including reserved indices.
} Mcan_Interrupt;

/// \brief Line connected to given MCAN interrupt.
typedef enum {
	Mcan_InterruptLine_0 = 0, ///< Interrupt Line 0.
	Mcan_InterruptLine_1 = 1, ///< Interrupt Line 1.
} Mcan_InterruptLine;

/// \brief MCAN interrupt configuration.
typedef struct {
	bool isEnabled; ///< Interrupt enable flag.
	Mcan_InterruptLine line; ///< Line connected to the interrupt.
} Mcan_InterruptConfig;

/// \brief MCAN interrupt status structure.
typedef struct {
	bool hasRf0nOccurred; ///< Receive FIFO 0 New Message interrupt occurred.
	bool hasRf0wOccurred; ///< Receive FIFO 0 Watermark Reached interrupt occurred.
	bool hasRf0fOccurred; ///< Receive FIFO 0 Full interrupt occurred.
	bool hasRf0lOccurred; ///< Receive FIFO 0 Message Lost interrupt occurred.
	bool hasRf1nOccurred; ///< Receive FIFO 1 New Message interrupt occurred.
	bool hasRf1wOccurred; ///< Receive FIFO 1 Watermark Reached interrupt occurred.
	bool hasRf1fOccurred; ///< Receive FIFO 1 Full interrupt occurred.
	bool hasRf1lOccurred; ///< Receive FIFO 1 Message Lost interrupt occurred.
	bool hasHpmOccurred; ///< High Priority Message interrupt occurred.
	bool hasTcOccurred; ///< Transmission Completed interrupt occurred.
	bool hasTcfOccurred; ///< Transmission Cancellation Finished interrupt occurred.
	bool hasTfeOccurred; ///< Tx FIFO Empty interrupt occurred.
	bool hasTefnOccurred; ///< Tx Event FIFO New Entry interrupt occurred.
	bool hasTefwOccurred; ///< Tx Event FIFO Watermark Reached interrupt occurred.
	bool hasTeffOccurred; ///< Tx Event FIFO Full interrupt occurred.
	bool hasTeflOccurred; ///< Tx Event FIFO Element Lost interrupt occurred.
	bool hasTswOccurred; ///< Timestamp Wraparound interrupt occurred.
	bool hasMrafOccurred; ///< Message RAM Access Failure interrupt occurred.
	bool hasTooOccurred; ///< Timeout Occurred interrupt occurred.
	bool hasDrxOccurred; ///< Message stored to Dedicated Receive Buffer interrupt occurred.
	bool hasEloOccurred; ///< Error Logging Overflow interrupt occurred.
	bool hasEpOccurred; ///< Error Passive interrupt occurred.
	bool hasEwOccurred; ///< Warning Status interrupt occurred.
	bool hasBoOccurred; ///< Bus_Off Status interrupt occurred.
	bool hasWdiOccurred; ///< Watchdog Interrupt interrupt occurred.
	bool hasPeaOccurred; ///< Protocol Error in Arbitration Phase interrupt occurred.
	bool hasPedOccurred; ///< Protocol Error in Data Phase interrupt occurred.
	bool hasAraOccurred; ///< Access to Reserved Address interrupt occurred.
} Mcan_InterruptStatus;

/// \brief Mcan timeout configuration structure.
typedef struct {
	/// \brief Timeout counter enable flag.
	bool isEnabled;
	/// \brief The type of timeout to be used (valid only if isTimeoutEnabled == true).
	Mcan_TimeoutType type;
	/// \brief Timeout period.
	uint16_t period;
} Mcan_TimeoutConfig;

/// \brief Mcan configuration structure.
typedef struct {
	/// \brief Base address of the message ram; only the upper 16 bits are used
	///        which shall be constant for the whole message RAM area.
	uint32_t *msgRamBaseAddress;
	/// \brief Operation mode.
	Mcan_Mode mode;
	/// \brief CAN FD (without bit rate switching) enable flag.
	bool isFdEnabled;
	/// \brief Nominal bit timing settings.
	Mcan_BitTiming nominalBitTiming;
	/// \brief Data bit timing for CAN FD settings.
	Mcan_BitTiming dataBitTiming;
	/// \brief Transmitter Delay Compensation settings.
	Mcan_TransmitterDelayCompensation transmitterDelayCompensation;
	/// \brief The clock source used for timestamp generation.
	Mcan_TimestampClk timestampClk;
	/// \brief Timestamp and timeout clock prescaler.
	/// \details clock = selected clock / (timestampPrescaler + 1).
	uint8_t timestampTimeoutPrescaler;
	/// \brief Timeout config.
	Mcan_TimeoutConfig timeout;
	/// \brief ID filtering settings for standard CAN IDs.
	Mcan_IdFilter standardIdFilter;
	/// \brief ID filtering settings for extended CAN IDs.
	Mcan_IdFilter extendedIdFilter;
	/// \brief Rx FIFO 0 settings.
	Mcan_RxFifoConfig rxFifo0;
	/// \brief Rx FIFO 1 settings.
	Mcan_RxFifoConfig rxFifo1;
	/// \brief Rx Buffer settings.
	Mcan_RxBufferConfig rxBuffer;
	/// \brief Tx Buffer settings.
	Mcan_TxBufferConfig txBuffer;
	/// \brief Tx Event FIFO settings.
	Mcan_TxEventFifoConfig txEventFifo;
	/// \brief MCAN interrupts configuration.
	Mcan_InterruptConfig interrupts[Mcan_Interrupt_Count];
	/// \brief MCAN Line 0 interrupt enabled flag.
	bool isLine0InterruptEnabled;
	/// \brief MCAN Line 1 interrupt enabled flag.
	bool isLine1InterruptEnabled;
	/// \brief RAM watchdog counter value.
	uint8_t wdtCounter;
} Mcan_Config;

/// \brief CAN FD ESI flag value.
typedef enum {
	Mcan_ElementEsi_Dominant =
			0, ///< ESI depends only on error passive flag.
	Mcan_ElementEsi_Recessive = 1, ///< ESI is transmitted recessive.
} Mcan_ElementEsi;

/// \brief The type of CAN Id used in the element.
typedef enum {
	Mcan_IdType_Standard = 0, ///< Element contains CAN Standard Id.
	Mcan_IdType_Extended = 1, ///< Element contains CAN Extended Id.
} Mcan_IdType;

/// \brief The type of CAN Id used in the element.
typedef enum {
	Mcan_FrameType_Data = 0, ///< Frame contains data.
	Mcan_FrameType_Remote = 1, ///< Frame is a transmission request (RTR).
} Mcan_FrameType;

/// \brief The type of detected event.
typedef enum {
	Mcan_TxEventType_Tx = 1, ///< Element was sent.
	Mcan_TxEventType_Cancellation = 2, ///< Transmission was cancelled.
} Mcan_TxEventType;

/// \brief Mcan Tx element for Tx Buffer/FIFO/Queue.
typedef struct {
	Mcan_ElementEsi esiFlag; ///< CAN FD ESI flag value.
	Mcan_IdType idType; ///< The type of CAN Id.
	Mcan_FrameType frameType; ///< The type of frame.
	uint32_t id; ///< CAN Id - 11 or 29 bit (for Extended Frame type).
	uint8_t marker; ///< Message marker to be placed in the Tx Event FIFO.
	bool isTxEventStored; ///< Tx Event storage enable flag.
	bool isCanFdFormatEnabled; ///< CAN FD format enable flag.
	bool isBitRateSwitchingEnabled; ///< Bit rate switching enable flag.
	/// \brief Number of data bytes set in DLC field.
	///        Allowed values:
	///        Standard Id CAN: 0-8,
	///        Extended Id CAN: 0-8, 12, 16, 20, 24, 32, 48, 64.
	uint8_t dataSize;
	const uint8_t *data; ///< Data pointer;
	bool isInterruptEnabled; ///< Enable interrupt after transmission complete.
} Mcan_TxElement;

/// \brief Mcan Tx element for Tx Event FIFO.
typedef struct {
	Mcan_ElementEsi esiFlag; ///< CAN FD ESI flag value.
	Mcan_IdType idType; ///< The type of CAN Id.
	Mcan_FrameType frameType; ///< The type of frame.
	uint32_t id; ///< CAN Id - 11 or 29 bit (for Extended Frame type).
	uint8_t marker; ///< Message marker to be placed in the Tx Event FIFO.
	Mcan_TxEventType eventType; ///< Tx Event type;
	bool isCanFdFormatEnabled; ///< CAN FD format enable flag.
	bool isBitRateSwitchingEnabled; ///< Bit rate switching enable flag.
	/// \brief Number of data bytes set in DLC field.
	///        Allowed values:
	///        Standard Id CAN: 0-8,
	///        Extended Id CAN: 0-8, 12, 16, 20, 24, 32, 48, 64.
	uint8_t dataSize;
	uint16_t timestamp; ///< Frame timestamp;
} Mcan_TxEventElement;

/// \brief Mcan Rx element for Rx Buffer/FIFO.
typedef struct {
	Mcan_ElementEsi esiFlag; ///< CAN FD ESI flag value.
	Mcan_IdType idType; ///< The type of CAN Id.
	Mcan_FrameType frameType; ///< The type of frame.
	uint32_t id; ///< CAN Id - 11 or 29 bit (for Extended Frame type).
	bool isNonMatchingFrame; ///< Rx frame did not match any filter.
	uint8_t filterIndex; ///< Matching filter index (valid if nonMatchingFrame == false).
	bool isCanFdFormatEnabled; ///< CAN FD format enable flag.
	bool isBitRateSwitchingEnabled; ///< Bit rate switching enable flag.
	uint16_t timestamp; ///< Frame timestamp;
	/// \brief Number of data bytes set in DLC field.
	///        Allowed values:
	///        Standard Id CAN: 0-8,
	///        Extended Id CAN: 0-8, 12, 16, 20, 24, 32, 48, 64.
	uint8_t dataSize;
	uint8_t *data; ///< Data pointer;
} Mcan_RxElement;

/// \brief The type of Rx filter.
typedef enum {
	Mcan_RxFilterType_Range = 0, ///< Range filter; id1 <= id <= id2.
	Mcan_RxFilterType_Dual = 1, ///< Dual filter; id == id1 || id == id2.
	Mcan_RxFilterType_Mask = 2, ///< Mask filter; id & id2 == id1.
} Mcan_RxFilterType;

/// \brief The type of Rx filter.
typedef enum {
	Mcan_RxFilterConfig_Disabled = 0, ///< Disable filter element.
	Mcan_RxFilterConfig_RxFifo0 =
			1, ///< Store in Rx FIFO 0 if filter matches.
	Mcan_RxFilterConfig_RxFifo1 =
			2, ///< Store in Rx FIFO 1 if filter matches.
	Mcan_RxFilterConfig_Reject = 3, ///< Reject ID if filter matches.
	Mcan_RxFilterConfig_Priority = 4, ///< Set priority if filter matches.
	Mcan_RxFilterConfig_PriorityRxFifo0 =
			5, ///< Set priority and store in FIFO 0 if filter matches.
	Mcan_RxFilterConfig_PriorityRxFifo1 =
			6, ///< Set priority and store in FIFO 1 if filter matches,
	/// \brief Store into Rx Buffer or as debug message, filter type (SFT[1:0]) ignored, id == id1.
	Mcan_RxFilterConfig_RxBuffer = 7,
} Mcan_RxFilterConfig;

/// \brief Rx filter element.
typedef struct {
	Mcan_RxFilterType type; ///< The type of configured filter.
	Mcan_RxFilterConfig config; ///< Filter behaviour configuration.
	uint32_t id1; ///< Filter Id1; bits [10:0] for standard Id, [28:0] for extended Id.
	uint32_t id2; ///< Filter Id2; bits [10:0] for standard Id, [28:0] for extended Id.
} Mcan_RxFilterElement;

/// \brief Mcan Tx descriptor
typedef struct {
	uint32_t *bufferAddress; ///< Address (32-bit) of the Tx Buffer within message RAM.
	uint8_t bufferSize; ///< Size (number of 32-bit words) of the Tx Buffer.
	uint32_t *queueAddress; ///< Address (32-bit) of the Tx Queue within message RAM.
	uint8_t queueSize; ///< Size (number of 32-bit words) of the Tx Queue.
	uint8_t elementSize; ///< Size of the data field (in bytes) of the Tx Element.
} Mcan_Tx;

/// \brief Mcan Rx Fifo descriptor.
typedef struct {
	uint32_t *address; ///< Address (32-bit) of the Rx FIFO within message RAM.
	uint8_t size; ///< Size (number of 32-bit words) of the Rx FIFO.
	uint8_t elementSize; ///< Size of the data field (in bytes) of the Rx Element in FIFO.
} Mcan_RxFifo;

/// \brief Mcan device descriptor.
typedef struct {
	uint32_t *msgRamBaseAddress; ///< Base address of the message ram;
	Mcan_Registers reg; ///< MCAN registers.
	Mcan_Tx tx; ///< Tx buffer/queue descriptor.
	uint32_t *rxBufferAddress; ///< Address (32-bit) of the Rx Buffer within message RAM.
	uint8_t rxBufferElementSize; ///< Size of the data field (in bytes) of the Tx Element.
	Mcan_RxFifo rxFifo0; ///< Rx FIFO0 descriptor.
	Mcan_RxFifo rxFifo1; ///< Rx FIFO1 descriptor.
	uint32_t *txEventFifoAddress; ///< Address (32-bit) of the Tx Event Fifo within message RAM.
	uint8_t txEventFifoSize; ///< Size (number of 32-bit words) of the Tx Event Fifo.
	uint32_t *rxStdFilterAddress; ///< Address (32-bit) of the Standard Id filter within message RAM.
	uint8_t rxStdFilterSize; ///< Size (number of 32-bit words) of the Standard Id filter.
	uint32_t *rxExtFilterAddress; ///< Address (32-bit) of the Extended Id filter within message RAM.
	uint8_t rxExtFilterSize; ///< Size (number of 32-bit words) of the Extended Id filter.
} Mcan;

/// \brief Returns Mcan registers base address.
/// \param [in] id Identifier of the device.
/// \returns Structure containing start addresses of Mcan registers.
Mcan_Registers Mcan_getDeviceRegisters(const Mcan_Id id);

/// \brief Initializes a device descriptor for Mcan.
/// \param [out] mcan Mcan device descriptor.
/// \param [in] regs Pointers to Mcan register descriptors.
void Mcan_init(Mcan *const mcan, const Mcan_Registers regs);

/// \brief Configures an Mcan device based on a configuration descriptor.
/// \param [in] mcan Mcan device descriptor.
/// \param [in] config A configuration descriptor.
/// \param [in] timeoutLimit Timeout counter value for the configuration process.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Configuration was successful.
/// \retval false Configuration failed.
bool Mcan_setConfig(Mcan *const mcan, const Mcan_Config *const config,
		const uint32_t timeoutLimit, ErrorCode *const errCode);

/// \brief Reads the current configuration of the Mcan device.
/// \param [in] mcan Mcan device descriptor.
/// \param [out] config A configuration descriptor.
void Mcan_getConfig(const Mcan *const mcan, Mcan_Config *const config);

/// \brief Adds a new element to the Tx Buffer and initializes its transmission.
/// \param [in] mcan Mcan device descriptor.
/// \param [in] element Tx element to send.
/// \param [in] index Tx Buffer index.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Adding element was successful.
/// \retval false Adding element failed.
bool Mcan_txBufferAdd(Mcan *const mcan, const Mcan_TxElement element,
		const uint8_t index, ErrorCode *const errCode);

/// \brief Adds a new element to the Tx Queue and initializes its transmission.
/// \param [in] mcan Mcan device descriptor.
/// \param [in] element Tx element to send.
/// \param [out] index Memory buffer index at which the element was added.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Adding element was successful.
/// \retval false Adding element failed.
bool Mcan_txQueuePush(Mcan *const mcan, const Mcan_TxElement element,
		uint8_t *const index, ErrorCode *const errCode);

/// \brief Checks whether the specified Tx Buffer or Queue element was sent.
/// \param [in] mcan Mcan device descriptor.
/// \param [in] index Queried element index.
/// \retval true Element was sent.
/// \retval false Element was not sent.
bool Mcan_txBufferIsTransmissionFinished(
		const Mcan *const mcan, const uint8_t index);

/// \brief Pulls element the Tx Event Queue.
/// \param [in] mcan Mcan device descriptor.
/// \param [out] element Tx Event element pointer.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Pulling element was successful.
/// \retval false Pulling element failed.
bool Mcan_txEventFifoPull(const Mcan *const mcan,
		Mcan_TxEventElement *const element, ErrorCode *const errCode);

/// \brief Receives element from the Rx Buffer.
/// \param [in] mcan Mcan device descriptor.
/// \param [in] index Index of the Rx element to obtain.
/// \param [out] element Rx element pointer.
void Mcan_rxBufferGet(const Mcan *const mcan, const uint8_t index,
		Mcan_RxElement *const element);

/// \brief Pulls element the Rx Fifo.
/// \param [in] mcan Mcan device descriptor.
/// \param [in] id The id of the Rx Fifo.
/// \param [out] element Rx element pointer.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Pulling element was successful.
/// \retval false Pulling element failed.
bool Mcan_rxFifoPull(Mcan *const mcan, const Mcan_RxFifoId id,
		Mcan_RxElement *const element, ErrorCode *const errCode);

/// \brief Reads the status of the Rx Fifo.
/// \param [in] mcan Mcan device descriptor.
/// \param [in] id The id of the Rx Fifo.
/// \param [out] status Rx Fifo status pointer.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Obtaining RxFifo status was successful.
/// \retval false Obtaining RxFifo status failed.
bool Mcan_getRxFifoStatus(const Mcan *const mcan, const Mcan_RxFifoId id,
		Mcan_RxFifoStatus *const status, ErrorCode *const errCode);

/// \brief Reads the status of the Tx Queue.
/// \param [in] mcan Mcan device descriptor.
/// \param [out] status Tx Queue status pointer.
void Mcan_getTxQueueStatus(
		const Mcan *const mcan, Mcan_TxQueueStatus *const status);

/// \brief Reads the status of the Tx Event Fifo.
/// \param [in] mcan Mcan device descriptor.
/// \param [out] status Tx Event Fifo status pointer.
void Mcan_getTxEventFifoStatus(
		const Mcan *const mcan, Mcan_TxEventFifoStatus *const status);

/// \brief Set the Rx filter for standard CAN Id.
/// \param [in] mcan Mcan device descriptor.
/// \param [in] element Rx filter element.
/// \param [in] index Rx filter list index.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Setting standard ID filter was successful.
/// \retval false Setting standard ID filter failed.
bool Mcan_setStandardIdFilter(Mcan *const mcan,
		const Mcan_RxFilterElement element, const uint8_t index,
		ErrorCode *const errCode);

/// \brief Set the Rx filter for extended CAN Id.
/// \param [in] mcan Mcan device descriptor.
/// \param [in] element Rx filter element.
/// \param [in] index Rx filter list index.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Setting extended ID filter was successful.
/// \retval false Setting extended ID filter failed.
bool Mcan_setExtendedIdFilter(Mcan *const mcan,
		const Mcan_RxFilterElement element, const uint8_t index,
		ErrorCode *const errCode);

/// \brief Reads the information about active interrupts.
/// \param [in] mcan Mcan device descriptor.
/// \param [in] status Interrupt status pointer.
void Mcan_getInterruptStatus(
		const Mcan *const mcan, Mcan_InterruptStatus *const status);

/// \brief Resets the timeout counter value when in Continuous mode.
/// \param [in] mcan Mcan device descriptor.
static inline void
Mcan_resetTimeoutCounter(Mcan *const mcan)
{
	mcan->reg.base->tocv = 0;
}

/// \brief Returns true if the Tx Queue is empty
/// \param [in] mcan Mcan device descriptor.
/// \retval true Hardware transmission FIFO is empty.
/// \retval false Hardware transmission FIFO is not empty.
bool Mcan_isTxFifoEmpty(const Mcan *const mcan);

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // BSP_MCAN_H
