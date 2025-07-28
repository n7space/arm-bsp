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

#include "Mcan.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include <Utils/Bits.h>
#include <Utils/ErrorCode.h>
#include <Utils/Utils.h>

#include <bsp/arm/Scb/Scb.h>

Mcan_Registers
Mcan_getDeviceRegisters(const Mcan_Id id)
{
	Mcan_Registers registers = { .base = NULL, .canDmaBase = NULL };
	switch (id) {
	case Mcan_Id_0:
		// cppcheck-suppress misra-c2012-11.4
		registers.base = (Mcan_BaseRegisters *)MCAN0_ADDRESS_BASE;
		// cppcheck-suppress misra-c2012-11.4
		registers.canDmaBase =
				(uint32_t *)MCAN0_CHIPCFG_CAN_DMA_ADDRESS_BASE;
		break;

	case Mcan_Id_1:
		// cppcheck-suppress misra-c2012-11.4
		registers.base = (Mcan_BaseRegisters *)MCAN1_ADDRESS_BASE;
		// cppcheck-suppress misra-c2012-11.4
		registers.canDmaBase =
				(uint32_t *)MCAN1_CHIPCFG_CAN_DMA_ADDRESS_BASE;
		break;
	}

	return registers;
}

void
Mcan_init(Mcan *const mcan, const Mcan_Registers regs)
{
	assert(mcan != NULL);

	(void)memset(mcan, 0, sizeof(Mcan));
	mcan->reg = regs;
}

static Mcan_DataLengthRaw
decodeElementSizeInBytes(const Mcan_ElementSize size)
{
	switch (size) {
	case Mcan_ElementSize_8: return Mcan_DataLengthRaw_8;
	case Mcan_ElementSize_12: return Mcan_DataLengthRaw_12;
	case Mcan_ElementSize_16: return Mcan_DataLengthRaw_16;
	case Mcan_ElementSize_20: return Mcan_DataLengthRaw_20;
	case Mcan_ElementSize_24: return Mcan_DataLengthRaw_24;
	case Mcan_ElementSize_32: return Mcan_DataLengthRaw_32;
	case Mcan_ElementSize_48: return Mcan_DataLengthRaw_48;
	case Mcan_ElementSize_64: return Mcan_DataLengthRaw_64;
	case Mcan_ElementSize_Invalid: break;
	}

	return Mcan_DataLengthRaw_Invalid;
}

static uint8_t
decodeTxElementSizeInBytes(const Mcan_ElementSize size)
{
	const Mcan_DataLengthRaw elementSize = decodeElementSizeInBytes(size);
	return (uint8_t)((uint32_t)elementSize
			+ (MCAN_TXELEMENT_DATA_WORD * sizeof(uint32_t)));
}

static uint8_t
decodeRxElementSizeInBytes(const Mcan_ElementSize size)
{
	const Mcan_DataLengthRaw elementSize = decodeElementSizeInBytes(size);
	return (uint8_t)((uint32_t)elementSize
			+ (MCAN_RXELEMENT_DATA_WORD * sizeof(uint32_t)));
}

static void
setMsgRamBaseAddress(Mcan *const mcan, const Mcan_Config *const config)
{
	mcan->msgRamBaseAddress = config->msgRamBaseAddress;
	// cppcheck-suppress misra-c2012-11.4
	const uint32_t addressMsb =
			(uint32_t)(config->msgRamBaseAddress) >> 16u;
	SET_FIELD_VALUE(MCAN_CHIPCFG_CANXDMABA, mcan->reg.canDmaBase,
			addressMsb);
}

static bool
setPowerDownMode(Mcan *const mcan, const uint32_t timeoutLimit,
		ErrorCode *const errCode)
{
	mcan->reg.base->cccr |= MCAN_CCCR_CSR_MASK;

	if (!waitForRegisterWithTimeout(&mcan->reg.base->cccr,
			    MCAN_CCCR_CSA_MASK, timeoutLimit))
		return returnError(errCode,
				Mcan_ErrorCode_ClockStopRequestTimeout);

	return true;
}

static bool
setMode(Mcan *const mcan, const Mcan_Config *const config,
		const uint32_t timeoutLimit, ErrorCode *const errCode)
{
	if (config->isFdEnabled)
		mcan->reg.base->cccr |= MCAN_CCCR_FDOE_MASK;

	switch (config->mode) {
	case Mcan_Mode_Normal: return true;
	case Mcan_Mode_AutomaticRetransmissionDisabled:
		mcan->reg.base->cccr |= MCAN_CCCR_DAR_MASK;
		return true;
	case Mcan_Mode_Restricted:
		mcan->reg.base->cccr |= MCAN_CCCR_ASM_MASK;
		return true;
	case Mcan_Mode_BusMonitoring:
		mcan->reg.base->cccr |= MCAN_CCCR_MON_MASK;
		return true;
	case Mcan_Mode_PowerDown:
		if (!setPowerDownMode(mcan, timeoutLimit, errCode))
			return false;
		return true;
	case Mcan_Mode_InternalLoopBackTest:
		mcan->reg.base->cccr |= MCAN_CCCR_TEST_MASK;
		mcan->reg.base->cccr |= MCAN_CCCR_MON_MASK;
		mcan->reg.base->test = MCAN_TEST_LBCK_MASK;
		return true;
	case Mcan_Mode_Invalid: break;
	}

	return returnError(errCode, Mcan_ErrorCode_ModeInvalid);
}

static void
setNominalTiming(Mcan *const mcan, const Mcan_Config *const config)
{
	const Mcan_BitTiming *timing = &config->nominalBitTiming;
	assert(timing->timeSegmentBeforeSamplePoint > 0u);

	mcan->reg.base->nbtp = BIT_FIELD_VALUE(MCAN_NBTP_NBRP,
					       timing->bitRatePrescaler)
			| BIT_FIELD_VALUE(MCAN_NBTP_NTSEG2,
					timing->timeSegmentAfterSamplePoint)
			| BIT_FIELD_VALUE(MCAN_NBTP_NTSEG1,
					timing->timeSegmentBeforeSamplePoint)
			| BIT_FIELD_VALUE(MCAN_NBTP_NSJW,
					timing->synchronizationJump);
}

static void
setDataTiming(Mcan *const mcan, const Mcan_Config *const config)
{
	const Mcan_BitTiming *timing = &config->dataBitTiming;
	assert(timing->timeSegmentBeforeSamplePoint > 0u);

	mcan->reg.base->dbtp = BIT_FIELD_VALUE(MCAN_DBTP_DBRP,
					       timing->bitRatePrescaler)
			| BIT_FIELD_VALUE(MCAN_DBTP_DTSEG2,
					timing->timeSegmentAfterSamplePoint)
			| BIT_FIELD_VALUE(MCAN_DBTP_DTSEG1,
					timing->timeSegmentBeforeSamplePoint)
			| BIT_FIELD_VALUE(MCAN_DBTP_DSJW,
					timing->synchronizationJump);
}

static void
setTransmitterDelayCompensation(Mcan *const mcan,
		const Mcan_TransmitterDelayCompensation *const config)
{
	changeBitAtOffset(&mcan->reg.base->dbtp, MCAN_DBTP_TDC_OFFSET,
			config->isEnabled);

	mcan->reg.base->tdcr = BIT_FIELD_VALUE(MCAN_TDCR_TDCF, config->filter)
			| BIT_FIELD_VALUE(MCAN_TDCR_TDCO, config->offset);
}

static void
setTimestamp(Mcan *const mcan, const Mcan_Config *const config)
{
	mcan->reg.base->tscc =
			BIT_FIELD_VALUE(MCAN_TSCC_TSS, config->timestampClk)
			| BIT_FIELD_VALUE(MCAN_TSCC_TCP,
					config->timestampTimeoutPrescaler);

	// Clear the timestamp counter.
	mcan->reg.base->tscv = 0u;
}

static void
setTimeout(Mcan *const mcan, const Mcan_TimeoutConfig *const config)
{
	if (config->isEnabled) {
		mcan->reg.base->tocc = MCAN_TOCC_ETOC_MASK
				| BIT_FIELD_VALUE(MCAN_TOCC_TOS, config->type)
				| BIT_FIELD_VALUE(
						MCAN_TOCC_TOP, config->period);
	} else {
		mcan->reg.base->tocc = 0u;
	}
}

static void
setStandardIdFiltering(Mcan *const mcan, const Mcan_Config *const config)
{
	if (config->standardIdFilter.isIdRejected) {
		mcan->reg.base->gfc |= MCAN_GFC_RRFS_MASK;
		mcan->reg.base->sidfc = 0u;
		mcan->rxStdFilterAddress = NULL;
		mcan->rxStdFilterSize = 0u;
	} else {
		mcan->reg.base->gfc |= BIT_FIELD_VALUE(MCAN_GFC_ANFS,
				config->standardIdFilter.nonMatchingPolicy);
		// cppcheck-suppress misra-c2012-11.4
		const uint32_t filterListAddress =
				(uint32_t)config->standardIdFilter
						.filterListAddress;
		mcan->reg.base->sidfc =
				(filterListAddress & MCAN_SIDFC_FLSSA_MASK)
				| BIT_FIELD_VALUE(MCAN_SIDFC_LSS,
						config->standardIdFilter
								.filterListSize);
		mcan->rxStdFilterAddress =
				config->standardIdFilter.filterListAddress;
		mcan->rxStdFilterSize = config->standardIdFilter.filterListSize;
	}
}

static void
setExtendedIdFiltering(Mcan *const mcan, const Mcan_Config *const config)
{
	if (config->extendedIdFilter.isIdRejected) {
		mcan->reg.base->gfc |= MCAN_GFC_RRFE_MASK;
		mcan->reg.base->xidfc = 0u;
		mcan->rxExtFilterAddress = NULL;
		mcan->rxExtFilterSize = 0u;
	} else {
		mcan->reg.base->gfc |= BIT_FIELD_VALUE(MCAN_GFC_ANFE,
				config->extendedIdFilter.nonMatchingPolicy);
		// cppcheck-suppress misra-c2012-11.4
		const uint32_t filterListAddress =
				(uint32_t)config->extendedIdFilter
						.filterListAddress;
		mcan->reg.base->xidfc =
				(filterListAddress & MCAN_XIDFC_FLESA_MASK)
				| BIT_FIELD_VALUE(MCAN_XIDFC_LSE,
						config->extendedIdFilter
								.filterListSize);
		mcan->rxExtFilterAddress =
				config->extendedIdFilter.filterListAddress;
		mcan->rxExtFilterSize = config->extendedIdFilter.filterListSize;
	}
}

static void
setRxFifo0(Mcan *const mcan, const Mcan_Config *const config)
{
	mcan->reg.base->rxesc &= ~MCAN_RXESC_F0DS_MASK;

	if (config->rxFifo0.isEnabled) {
		// cppcheck-suppress misra-c2012-11.4
		const uint32_t startAddress =
				(uint32_t)config->rxFifo0.startAddress;
		mcan->reg.base->rxf0c = (startAddress & MCAN_RXF0C_F0SA_MASK)
				| BIT_FIELD_VALUE(MCAN_RXF0C_F0S,
						config->rxFifo0.size)
				| BIT_FIELD_VALUE(MCAN_RXF0C_F0WM,
						config->rxFifo0.watermark)
				| BIT_FIELD_VALUE(MCAN_RXF0C_F0OM,
						config->rxFifo0.mode);
		mcan->reg.base->rxesc |= BIT_FIELD_VALUE(
				MCAN_RXESC_F0DS, config->rxFifo0.elementSize);
		mcan->rxFifo0.address = config->rxFifo0.startAddress;
		mcan->rxFifo0.size = config->rxFifo0.size;
		mcan->rxFifo0.elementSize = decodeRxElementSizeInBytes(
				config->rxFifo0.elementSize);
	} else {
		mcan->reg.base->rxf0c = 0u;
		mcan->rxFifo0.address = NULL;
		mcan->rxFifo0.size = 0u;
		mcan->rxFifo0.elementSize = 0u;
	}
}

static void
setRxFifo1(Mcan *const mcan, const Mcan_Config *const config)
{
	mcan->reg.base->rxesc &= ~MCAN_RXESC_F1DS_MASK;

	if (config->rxFifo1.isEnabled) {
		// cppcheck-suppress misra-c2012-11.4
		const uint32_t startAddress =
				(uint32_t)config->rxFifo1.startAddress;
		mcan->reg.base->rxf1c = (startAddress & MCAN_RXF1C_F1SA_MASK)
				| BIT_FIELD_VALUE(MCAN_RXF1C_F1S,
						config->rxFifo1.size)
				| BIT_FIELD_VALUE(MCAN_RXF1C_F1WM,
						config->rxFifo1.watermark)
				| BIT_FIELD_VALUE(MCAN_RXF1C_F1OM,
						config->rxFifo1.mode);
		mcan->reg.base->rxesc |= BIT_FIELD_VALUE(
				MCAN_RXESC_F1DS, config->rxFifo1.elementSize);
		mcan->rxFifo1.address = config->rxFifo1.startAddress;
		mcan->rxFifo1.size = config->rxFifo1.size;
		mcan->rxFifo1.elementSize = decodeRxElementSizeInBytes(
				config->rxFifo1.elementSize);
	} else {
		mcan->reg.base->rxf1c = 0u;
		mcan->rxFifo1.address = NULL;
		mcan->rxFifo1.size = 0u;
		mcan->rxFifo1.elementSize = 0u;
	}
}

static void
setRxBuffer(Mcan *const mcan, const Mcan_Config *const config)
{
	// cppcheck-suppress misra-c2012-11.4
	mcan->reg.base->rxbc = (uint32_t)config->rxBuffer.startAddress
			& MCAN_RXBC_RBSA_MASK;
	SET_FIELD_VALUE(MCAN_RXESC_RBDS, &mcan->reg.base->rxesc,
			config->rxBuffer.elementSize);
	mcan->rxBufferAddress = config->rxBuffer.startAddress;
	mcan->rxBufferElementSize = decodeRxElementSizeInBytes(
			config->rxBuffer.elementSize);
}

static bool
setTxBuffer(Mcan *const mcan, const Mcan_Config *const config)
{
	if (config->txBuffer.isEnabled) {
		const uint8_t txElementSize = decodeTxElementSizeInBytes(
				config->txBuffer.elementSize);
		if (txElementSize == MCAN_TXELEMENT_SIZE_INVALID)
			return false;

		assert((config->txBuffer.bufferSize
				       + config->txBuffer.queueSize)
				<= 32u);

		// cppcheck-suppress misra-c2012-11.4
		const uint32_t startAddress =
				(uint32_t)config->txBuffer.startAddress;

		mcan->reg.base->txbc = (startAddress & MCAN_TXBC_TBSA_MASK)
				| BIT_FIELD_VALUE(MCAN_TXBC_NDTB,
						config->txBuffer.bufferSize)
				| BIT_FIELD_VALUE(MCAN_TXBC_TFQS,
						config->txBuffer.queueSize)
				| BIT_FIELD_VALUE(MCAN_TXBC_TFQM,
						config->txBuffer.queueType);
		mcan->reg.base->txesc = BIT_FIELD_VALUE(
				MCAN_TXESC_TBDS, config->txBuffer.elementSize);
		mcan->tx.bufferAddress = config->txBuffer.startAddress;
		mcan->tx.bufferSize = config->txBuffer.bufferSize;
		mcan->tx.queueAddress =
				&config->txBuffer.startAddress
						 [(txElementSize
								  * (uint32_t)config
										  ->txBuffer
										  .bufferSize)
								 / sizeof(uint32_t)];
		mcan->tx.queueSize = config->txBuffer.queueSize;
		mcan->tx.elementSize = txElementSize;
	} else {
		mcan->reg.base->txbc = 0u;
		mcan->reg.base->txesc = 0u;
		mcan->tx.bufferAddress = NULL;
		mcan->tx.bufferSize = 0u;
		mcan->tx.queueAddress = NULL;
		mcan->tx.queueSize = 0u;
		mcan->tx.elementSize = 0u;
	}

	return true;
}

static void
setTxEventFifo(Mcan *const mcan, const Mcan_Config *const config)
{
	if (config->txEventFifo.isEnabled) {
		// cppcheck-suppress misra-c2012-11.4
		const uint32_t startAddress =
				(uint32_t)config->txEventFifo.startAddress;
		mcan->reg.base->txefc = (startAddress & MCAN_TXEFC_EFSA_MASK)
				| BIT_FIELD_VALUE(MCAN_TXEFC_EFS,
						config->txEventFifo.size)
				| BIT_FIELD_VALUE(MCAN_TXEFC_EFWM,
						config->txEventFifo.watermark);
		mcan->txEventFifoAddress = config->txEventFifo.startAddress;
		mcan->txEventFifoSize = config->txEventFifo.size;
	} else {
		mcan->reg.base->txefc = 0u;
		mcan->txEventFifoAddress = NULL;
		mcan->txEventFifoSize = 0u;
	}
}

static void
setInterrupts(Mcan *const mcan, const Mcan_Config *const config)
{
	for (uint32_t i = 0u; i < (uint32_t)Mcan_Interrupt_Count; i++) {
		if ((i != (uint32_t)Mcan_Interrupt_Reserved1)
				&& (i != (uint32_t)Mcan_Interrupt_Reserved2)) {
			mcan->reg.base->ir = shiftBitLeft(true, i);

			changeBitAtOffset(&mcan->reg.base->ie, i,
					config->interrupts[i].isEnabled);
			changeBitAtOffset(&mcan->reg.base->ils, i,
					config->interrupts[i].line
							!= Mcan_InterruptLine_0);
		}
	}

	mcan->reg.base->ile = BIT_VALUE(MCAN_ILE_EINT0,
					      config->isLine0InterruptEnabled)
			| BIT_VALUE(MCAN_ILE_EINT1,
					config->isLine1InterruptEnabled);

	mcan->reg.base->txbtie = 0u;
	mcan->reg.base->txbcie = 0u;
}

bool
Mcan_setConfig(Mcan *const mcan, const Mcan_Config *const config,
		const uint32_t timeoutLimit, ErrorCode *const errCode)
{
	setMsgRamBaseAddress(mcan, config);

	mcan->reg.base->cccr = MCAN_CCCR_INIT_MASK;

	if (!waitForRegisterWithTimeout(&mcan->reg.base->cccr,
			    MCAN_CCCR_INIT_MASK, timeoutLimit))
		return returnError(errCode,
				Mcan_ErrorCode_InitializationStartTimeout);

	// Enable write-protected registers.
	mcan->reg.base->cccr |= MCAN_CCCR_CCE_MASK;
	// Clear the CCCR register.
	mcan->reg.base->cccr =
			(uint32_t)(MCAN_CCCR_CCE_MASK | MCAN_CCCR_INIT_MASK);
	mcan->reg.base->gfc = 0u;

	if (!setMode(mcan, config, timeoutLimit, errCode))
		return false;

	setNominalTiming(mcan, config);
	if (config->isFdEnabled) {
		setDataTiming(mcan, config);
		setTransmitterDelayCompensation(
				mcan, &config->transmitterDelayCompensation);
	}
	setTimestamp(mcan, config);
	setTimeout(mcan, &config->timeout);
	setStandardIdFiltering(mcan, config);
	setExtendedIdFiltering(mcan, config);
	setRxFifo0(mcan, config);
	setRxFifo1(mcan, config);
	setRxBuffer(mcan, config);
	if (!setTxBuffer(mcan, config))
		return returnError(errCode, Mcan_ErrorCode_ElementSizeInvalid);
	setTxEventFifo(mcan, config);
	setInterrupts(mcan, config);

	mcan->reg.base->rwd = BIT_FIELD_VALUE(MCAN_RWD_WDC, config->wdtCounter);

	// SAE J1939 masking is not supported
	// Set all bits of extended ID mask to ones
	mcan->reg.base->xidam = MCAN_XIDAM_EIDM_MASK;

	mcan->reg.base->cccr &= ~MCAN_CCCR_INIT_MASK;

	return true;
}

static void
getMsgRamBaseAddress(const Mcan *const mcan, Mcan_Config *const config)
{
	config->msgRamBaseAddress = mcan->msgRamBaseAddress;
}

static void
getMode(const Mcan *const mcan, Mcan_Config *const config)
{
	const uint32_t cccr = mcan->reg.base->cccr;
	config->isFdEnabled = IS_BIT_SET(MCAN_CCCR_FDOE, cccr);

	if (IS_BIT_SET(MCAN_CCCR_INIT, cccr)) {
		if (IS_BIT_SET(MCAN_CCCR_CSR, cccr)
				&& IS_BIT_SET(MCAN_CCCR_CSA, cccr))
			config->mode = Mcan_Mode_PowerDown;
		else
			config->mode = Mcan_Mode_Invalid;
	} else {
		if (IS_BIT_SET(MCAN_CCCR_CSR, cccr)
				|| IS_BIT_SET(MCAN_CCCR_CSA, cccr)) {
			config->mode = Mcan_Mode_Invalid;
			return;
		}

		if (IS_BIT_SET(MCAN_CCCR_TEST, cccr)
				&& IS_BIT_SET(MCAN_CCCR_MON, cccr)) {
			if (IS_BIT_SET(MCAN_TEST_LBCK, mcan->reg.base->test))
				config->mode = Mcan_Mode_InternalLoopBackTest;
			else
				config->mode = Mcan_Mode_Invalid;
			return;
		}

		if (IS_BIT_SET(MCAN_CCCR_MON, cccr)) {
			config->mode = Mcan_Mode_BusMonitoring;
			return;
		}

		if (IS_BIT_SET(MCAN_CCCR_TEST, cccr)) {
			config->mode = Mcan_Mode_Invalid;
			return;
		}

		if (IS_BIT_SET(MCAN_CCCR_ASM, cccr)) {
			config->mode = Mcan_Mode_Restricted;
			return;
		}

		if (IS_BIT_SET(MCAN_CCCR_DAR, cccr)) {
			config->mode = Mcan_Mode_AutomaticRetransmissionDisabled;
			return;
		}

		config->mode = Mcan_Mode_Normal;
	}
}

static void
getNominalTiming(const Mcan *const mcan, Mcan_Config *const config)
{
	const uint32_t nbtp = mcan->reg.base->nbtp;
	config->nominalBitTiming.bitRatePrescaler =
			(uint16_t)GET_FIELD_VALUE(MCAN_NBTP_NBRP, nbtp);
	config->nominalBitTiming.synchronizationJump =
			(uint8_t)GET_FIELD_VALUE(MCAN_NBTP_NSJW, nbtp);
	config->nominalBitTiming.timeSegmentAfterSamplePoint =
			(uint8_t)GET_FIELD_VALUE(MCAN_NBTP_NTSEG2, nbtp);
	config->nominalBitTiming.timeSegmentBeforeSamplePoint =
			(uint8_t)GET_FIELD_VALUE(MCAN_NBTP_NTSEG1, nbtp);
}

static void
getDataTiming(const Mcan *const mcan, Mcan_Config *const config)
{
	const uint32_t dbtp = mcan->reg.base->dbtp;
	config->dataBitTiming.bitRatePrescaler =
			(uint16_t)GET_FIELD_VALUE(MCAN_DBTP_DBRP, dbtp);
	config->dataBitTiming.synchronizationJump =
			(uint8_t)GET_FIELD_VALUE(MCAN_DBTP_DSJW, dbtp);
	config->dataBitTiming.timeSegmentAfterSamplePoint =
			(uint8_t)GET_FIELD_VALUE(MCAN_DBTP_DTSEG2, dbtp);
	config->dataBitTiming.timeSegmentBeforeSamplePoint =
			(uint8_t)GET_FIELD_VALUE(MCAN_DBTP_DTSEG1, dbtp);
}

static void
getTransmitterDelayCompensation(
		const Mcan *const mcan, Mcan_Config *const config)
{
	config->transmitterDelayCompensation.isEnabled =
			IS_BIT_SET(MCAN_DBTP_TDC, mcan->reg.base->dbtp);

	const uint32_t tdcr = mcan->reg.base->tdcr;
	config->transmitterDelayCompensation.filter =
			(uint8_t)GET_FIELD_VALUE(MCAN_TDCR_TDCF, tdcr);
	config->transmitterDelayCompensation.offset =
			(uint8_t)GET_FIELD_VALUE(MCAN_TDCR_TDCO, tdcr);
}

static void
getTimestamp(const Mcan *const mcan, Mcan_Config *const config)
{
	const uint32_t tscc = mcan->reg.base->tscc;
	config->timestampClk = GET_FIELD_VALUE(MCAN_TSCC_TSS, tscc);
	config->timestampTimeoutPrescaler =
			(uint8_t)GET_FIELD_VALUE(MCAN_TSCC_TCP, tscc);
}

static void
getTimeout(const Mcan *const mcan, Mcan_TimeoutConfig *const config)
{
	const uint32_t tocc = mcan->reg.base->tocc;
	config->type = GET_FIELD_VALUE(MCAN_TOCC_TOS, tocc);
	config->period = (uint16_t)GET_FIELD_VALUE(MCAN_TOCC_TOP, tocc);
	config->isEnabled = IS_BIT_SET(MCAN_TOCC_ETOC, tocc);
}

static void
getStandardIdFiltering(const Mcan *const mcan, Mcan_Config *const config)
{
	const uint32_t gfc = mcan->reg.base->gfc;

	config->standardIdFilter.isIdRejected = IS_BIT_SET(MCAN_GFC_RRFS, gfc);
	config->standardIdFilter.nonMatchingPolicy =
			GET_FIELD_VALUE(MCAN_GFC_ANFS, gfc);
	config->standardIdFilter.filterListAddress = mcan->rxStdFilterAddress;
	config->standardIdFilter.filterListSize = mcan->rxStdFilterSize;
}

static void
getExtendedIdFiltering(const Mcan *const mcan, Mcan_Config *const config)
{
	const uint32_t gfc = mcan->reg.base->gfc;

	config->extendedIdFilter.isIdRejected = IS_BIT_SET(MCAN_GFC_RRFE, gfc);
	config->extendedIdFilter.nonMatchingPolicy =
			GET_FIELD_VALUE(MCAN_GFC_ANFE, gfc);
	config->extendedIdFilter.filterListAddress = mcan->rxExtFilterAddress;
	config->extendedIdFilter.filterListSize = mcan->rxExtFilterSize;
}

static Mcan_ElementSize
encodeElementSize(const uint8_t size)
{
	switch (size) {
	case 8u: return Mcan_ElementSize_8;
	case 12u: return Mcan_ElementSize_12;
	case 16u: return Mcan_ElementSize_16;
	case 20u: return Mcan_ElementSize_20;
	case 24u: return Mcan_ElementSize_24;
	case 32u: return Mcan_ElementSize_32;
	case 48u: return Mcan_ElementSize_48;
	case 64u: return Mcan_ElementSize_64;
	default: return Mcan_ElementSize_Invalid;
	}
}

static Mcan_ElementSize
encodeTxElementSizeInBytes(const uint8_t size)
{
	return encodeElementSize((uint8_t)(size
			- (MCAN_TXELEMENT_DATA_WORD * sizeof(uint32_t))));
}

static Mcan_ElementSize
encodeRxElementSizeInBytes(const uint8_t size)
{
	return encodeElementSize((uint8_t)(size
			- (MCAN_RXELEMENT_DATA_WORD * sizeof(uint32_t))));
}

static void
getRxFifo0(const Mcan *const mcan, Mcan_Config *const config)
{
	const uint32_t rxf0c = mcan->reg.base->rxf0c;

	config->rxFifo0.isEnabled = (rxf0c != 0u);

	config->rxFifo0.mode = GET_FIELD_VALUE(MCAN_RXF0C_F0OM, rxf0c);
	config->rxFifo0.watermark =
			(uint8_t)GET_FIELD_VALUE(MCAN_RXF0C_F0WM, rxf0c);
	config->rxFifo0.startAddress = mcan->rxFifo0.address;
	config->rxFifo0.size = mcan->rxFifo0.size;
	config->rxFifo0.elementSize =
			encodeRxElementSizeInBytes(mcan->rxFifo0.elementSize);
}

static void
getRxFifo1(const Mcan *const mcan, Mcan_Config *const config)
{
	const uint32_t rxf1c = mcan->reg.base->rxf1c;

	config->rxFifo1.isEnabled = (rxf1c != 0u);

	config->rxFifo1.mode = (rxf1c & MCAN_RXF1C_F1OM_MASK)
			>> MCAN_RXF1C_F1OM_OFFSET;
	config->rxFifo1.watermark = (rxf1c & MCAN_RXF1C_F1WM_MASK)
			>> MCAN_RXF1C_F1WM_OFFSET;
	config->rxFifo1.startAddress = mcan->rxFifo1.address;
	config->rxFifo1.size = mcan->rxFifo1.size;
	config->rxFifo1.elementSize =
			encodeRxElementSizeInBytes(mcan->rxFifo1.elementSize);
}

static void
getRxBuffer(const Mcan *const mcan, Mcan_Config *const config)
{
	config->rxBuffer.startAddress = mcan->rxBufferAddress;
	config->rxBuffer.elementSize =
			encodeRxElementSizeInBytes(mcan->rxBufferElementSize);
}

static void
getTxBuffer(const Mcan *const mcan, Mcan_Config *const config)
{
	const uint32_t txbc = mcan->reg.base->txbc;

	config->txBuffer.isEnabled = (txbc != 0u);

	config->txBuffer.startAddress = mcan->tx.bufferAddress;
	config->txBuffer.bufferSize = mcan->tx.bufferSize;
	config->txBuffer.queueSize = mcan->tx.queueSize;
	config->txBuffer.queueType = GET_FIELD_VALUE(MCAN_TXBC_TFQM, txbc);
	config->txBuffer.elementSize =
			encodeTxElementSizeInBytes(mcan->tx.elementSize);
}

static void
getTxEventFifo(const Mcan *const mcan, Mcan_Config *const config)
{
	const uint32_t txefc = mcan->reg.base->txefc;

	config->txEventFifo.isEnabled = (txefc != 0u);

	config->txEventFifo.startAddress = mcan->txEventFifoAddress;
	config->txEventFifo.size = mcan->txEventFifoSize;
	config->txEventFifo.watermark =
			(uint8_t)GET_FIELD_VALUE(MCAN_TXEFC_EFWM, txefc);
}

static void
getInterrupts(const Mcan *const mcan, Mcan_Config *const config)
{
	const uint32_t ie = mcan->reg.base->ie;
	const uint32_t ile = mcan->reg.base->ile;
	const uint32_t ils = mcan->reg.base->ils;

	for (uint8_t i = 0u; i < (uint8_t)Mcan_Interrupt_Count; i++) {
		if ((i != (uint8_t)Mcan_Interrupt_Reserved1)
				&& (i != (uint8_t)Mcan_Interrupt_Reserved2)) {
			config->interrupts[i].isEnabled = isBitSet(ie, i);

			if (isBitSet(ils, i))
				config->interrupts[i].line =
						Mcan_InterruptLine_1;
			else
				config->interrupts[i].line =
						Mcan_InterruptLine_0;
		} else
			config->interrupts[i].isEnabled = false;
	}

	config->isLine0InterruptEnabled = IS_BIT_SET(MCAN_ILE_EINT0, ile);
	config->isLine1InterruptEnabled = IS_BIT_SET(MCAN_ILE_EINT1, ile);
}

void
Mcan_getConfig(const Mcan *const mcan, Mcan_Config *const config)
{
	getMsgRamBaseAddress(mcan, config);
	getMode(mcan, config);
	getNominalTiming(mcan, config);
	getDataTiming(mcan, config);
	getTransmitterDelayCompensation(mcan, config);
	getTimestamp(mcan, config);
	getTimeout(mcan, &config->timeout);
	getStandardIdFiltering(mcan, config);
	getExtendedIdFiltering(mcan, config);
	getRxFifo0(mcan, config);
	getRxFifo1(mcan, config);
	getRxBuffer(mcan, config);
	getTxBuffer(mcan, config);
	getTxEventFifo(mcan, config);
	getInterrupts(mcan, config);

	config->wdtCounter = (uint8_t)GET_FIELD_VALUE(
			MCAN_RWD_WDC, mcan->reg.base->rwd);
}

static Mcan_DataLengthCode
encodeDataLengthCode(const Mcan_DataLengthRaw size)
{
	if ((uint8_t)size <= (uint8_t)Mcan_DataLengthRaw_8)
		return (Mcan_DataLengthCode)size;
	switch (size) {
	case Mcan_DataLengthRaw_12: return Mcan_DataLengthCode_12;
	case Mcan_DataLengthRaw_16: return Mcan_DataLengthCode_16;
	case Mcan_DataLengthRaw_20: return Mcan_DataLengthCode_20;
	case Mcan_DataLengthRaw_24: return Mcan_DataLengthCode_24;
	case Mcan_DataLengthRaw_32: return Mcan_DataLengthCode_32;
	case Mcan_DataLengthRaw_48: return Mcan_DataLengthCode_48;
	case Mcan_DataLengthRaw_64: return Mcan_DataLengthCode_64;
	default: return Mcan_DataLengthCode_Invalid;
	}
}

static Mcan_DataLengthRaw
decodeDataLengthCode(const Mcan_DataLengthCode dlc, const bool isCanFdFrame)
{
	if ((uint8_t)dlc <= (uint8_t)Mcan_DataLengthRaw_8)
		return (Mcan_DataLengthRaw)dlc;
	if (!isCanFdFrame)
		return Mcan_DataLengthRaw_8;

	switch (dlc) {
	case Mcan_DataLengthCode_12: return Mcan_DataLengthRaw_12;
	case Mcan_DataLengthCode_16: return Mcan_DataLengthRaw_16;
	case Mcan_DataLengthCode_20: return Mcan_DataLengthRaw_20;
	case Mcan_DataLengthCode_24: return Mcan_DataLengthRaw_24;
	case Mcan_DataLengthCode_32: return Mcan_DataLengthRaw_32;
	case Mcan_DataLengthCode_48: return Mcan_DataLengthRaw_48;
	default: return Mcan_DataLengthRaw_64;
	}
}

static bool
txAddElement(Mcan *const mcan, const Mcan_TxElement element,
		uint32_t *const baseAddress, const uint8_t index)
{
	const uint32_t dataLengthCode = encodeDataLengthCode(element.dataSize);
	if (dataLengthCode == MCAN_DLC_INVALID)
		return false;

	(void)memset(baseAddress, 0, mcan->tx.elementSize);

	// cppcheck-suppress [objectIndex]
	baseAddress[MCAN_TXELEMENT_ESI_WORD] |=
			BIT_FIELD_VALUE(MCAN_TXELEMENT_ESI, element.esiFlag);
	// cppcheck-suppress [objectIndex]
	baseAddress[MCAN_TXELEMENT_XTD_WORD] |=
			BIT_FIELD_VALUE(MCAN_TXELEMENT_XTD, element.idType);
	// cppcheck-suppress [objectIndex]
	baseAddress[MCAN_TXELEMENT_RTR_WORD] |=
			BIT_FIELD_VALUE(MCAN_TXELEMENT_RTR, element.frameType);
	// cppcheck-suppress [objectIndex]
	baseAddress[MCAN_TXELEMENT_MM_WORD] |=
			BIT_FIELD_VALUE(MCAN_TXELEMENT_MM, element.marker);

	if (element.idType == Mcan_IdType_Standard)
		baseAddress[MCAN_TXELEMENT_STDID_WORD] |= BIT_FIELD_VALUE(
				MCAN_TXELEMENT_STDID, element.id);
	else
		baseAddress[MCAN_TXELEMENT_EXTID_WORD] |= BIT_FIELD_VALUE(
				MCAN_TXELEMENT_EXTID, element.id);

	if (element.isTxEventStored)
		baseAddress[MCAN_TXELEMENT_EFC_WORD] |= MCAN_TXELEMENT_EFC_MASK;
	if (element.isCanFdFormatEnabled)
		baseAddress[MCAN_TXELEMENT_FDF_WORD] |= MCAN_TXELEMENT_FDF_MASK;
	if (element.isBitRateSwitchingEnabled)
		baseAddress[MCAN_TXELEMENT_BRS_WORD] |= MCAN_TXELEMENT_BRS_MASK;
	baseAddress[MCAN_TXELEMENT_DLC_WORD] |=
			BIT_FIELD_VALUE(MCAN_TXELEMENT_DLC, dataLengthCode);

	uint8_t *dataPointer =
			(uint8_t *)&baseAddress[MCAN_TXELEMENT_DATA_WORD];
	(void)memcpy(dataPointer, element.data, element.dataSize);
	MEMORY_SYNC_BARRIER();

	changeBitAtOffset(&mcan->reg.base->txbtie, index,
			element.isInterruptEnabled);

	return true;
}

bool
Mcan_txBufferAdd(Mcan *const mcan, const Mcan_TxElement element,
		const uint8_t index, ErrorCode *const errCode)
{
	if (index >= mcan->tx.bufferSize)
		return returnError(errCode, Mcan_ErrorCode_IndexOutOfRange);

	uint32_t *bufferPointer = &(mcan->tx.bufferAddress
					[(mcan->tx.elementSize
							 * (uint32_t)index)
							/ sizeof(uint32_t)]);

	if (!txAddElement(mcan, element, bufferPointer, index))
		return returnError(errCode, Mcan_ErrorCode_ElementSizeInvalid);

	mcan->reg.base->txbar = 1u << index;

	return true;
}

bool
Mcan_txQueuePush(Mcan *const mcan, const Mcan_TxElement element,
		uint8_t *const index, ErrorCode *const errCode)
{
	const uint32_t txfqs = mcan->reg.base->txfqs;
	const bool full = IS_BIT_SET(MCAN_TXFQS_TFQF, txfqs);
	if (full)
		return returnError(errCode, Mcan_ErrorCode_TxFifoFull);
	*index = (uint8_t)GET_FIELD_VALUE(MCAN_TXFQS_TFQPI, txfqs);
	uint32_t *baseAddr =
			&mcan->tx.bufferAddress
					 [(mcan->tx.elementSize
							  * (uint32_t)(*index))
							 / sizeof(uint32_t)];
	if (!txAddElement(mcan, element, baseAddr, *index))
		return returnError(errCode, Mcan_ErrorCode_ElementSizeInvalid);

	mcan->reg.base->txbar = 1u << *index;

	return true;
}

bool
Mcan_txBufferIsTransmissionFinished(const Mcan *const mcan, const uint8_t index)
{
	return isBitSet(mcan->reg.base->txbto, index);
}

bool
Mcan_txEventFifoPull(const Mcan *const mcan, Mcan_TxEventElement *const element,
		ErrorCode *const errCode)
{
	uint32_t count =
			GET_FIELD_VALUE(MCAN_TXEFS_EFFL, mcan->reg.base->txefs);
	if (count == 0u)
		return returnError(errCode, Mcan_ErrorCode_TxEventFifoEmpty);

	uint8_t getIndex = (uint8_t)GET_FIELD_VALUE(
			MCAN_TXEFS_EFGI, mcan->reg.base->txefs);
	const uint32_t *const baseAddr =
			&mcan->txEventFifoAddress[(MCAN_TXEVENTELEMENT_SIZE
								  * getIndex)
					/ sizeof(uint32_t)];

	element->esiFlag = GET_FIELD_VALUE(MCAN_TXEVENTELEMENT_ESI,
			baseAddr[MCAN_TXEVENTELEMENT_ESI_WORD]);
	element->idType = GET_FIELD_VALUE(MCAN_TXEVENTELEMENT_XTD,
			baseAddr[MCAN_TXEVENTELEMENT_XTD_WORD]);
	element->frameType = GET_FIELD_VALUE(MCAN_TXEVENTELEMENT_RTR,
			baseAddr[MCAN_TXEVENTELEMENT_RTR_WORD]);

	if (element->idType == Mcan_IdType_Standard)
		element->id = GET_FIELD_VALUE(MCAN_TXEVENTELEMENT_STDID,
				baseAddr[MCAN_TXEVENTELEMENT_STDID_WORD]);
	else
		element->id = GET_FIELD_VALUE(MCAN_TXEVENTELEMENT_EXTID,
				baseAddr[MCAN_TXEVENTELEMENT_EXTID_WORD]);
	element->marker = (uint8_t)GET_FIELD_VALUE(MCAN_TXEVENTELEMENT_MM,
			baseAddr[MCAN_TXEVENTELEMENT_MM_WORD]);
	element->eventType = GET_FIELD_VALUE(MCAN_TXEVENTELEMENT_ET,
			baseAddr[MCAN_TXEVENTELEMENT_ET_WORD]);
	element->isCanFdFormatEnabled = IS_BIT_SET(MCAN_TXEVENTELEMENT_FDF,
			baseAddr[MCAN_TXEVENTELEMENT_FDF_WORD]);
	element->isBitRateSwitchingEnabled = IS_BIT_SET(MCAN_TXEVENTELEMENT_BRS,
			baseAddr[MCAN_TXEVENTELEMENT_BRS_WORD]);
	element->timestamp = (uint16_t)GET_FIELD_VALUE(MCAN_TXEVENTELEMENT_TXTS,
			baseAddr[MCAN_TXEVENTELEMENT_TXTS_WORD]);
	const Mcan_DataLengthRaw dataSize = decodeDataLengthCode(
			(uint8_t)GET_FIELD_VALUE(MCAN_TXEVENTELEMENT_DLC,
					baseAddr[MCAN_TXEVENTELEMENT_DLC_WORD]),
			element->isCanFdFormatEnabled);
	element->dataSize = (uint8_t)dataSize;

	MEMORY_SYNC_BARRIER();

	mcan->reg.base->txefa = BIT_FIELD_VALUE(MCAN_TXEFA_EFAI, getIndex);
	return true;
}

static void
getRxElement(const uint32_t *const baseAddr, Mcan_RxElement *const element)
{
	element->esiFlag = GET_FIELD_VALUE(
			MCAN_RXELEMENT_ESI, baseAddr[MCAN_RXELEMENT_ESI_WORD]);
	element->idType = GET_FIELD_VALUE(
			MCAN_RXELEMENT_XTD, baseAddr[MCAN_RXELEMENT_XTD_WORD]);
	element->frameType = GET_FIELD_VALUE(
			MCAN_RXELEMENT_RTR, baseAddr[MCAN_RXELEMENT_RTR_WORD]);
	if (element->idType == Mcan_IdType_Standard)
		element->id = GET_FIELD_VALUE(MCAN_RXELEMENT_STDID,
				baseAddr[MCAN_RXELEMENT_STDID_WORD]);
	else
		element->id = GET_FIELD_VALUE(MCAN_RXELEMENT_EXTID,
				baseAddr[MCAN_RXELEMENT_EXTID_WORD]);
	element->isNonMatchingFrame = GET_FIELD_VALUE(MCAN_RXELEMENT_ANMF,
			baseAddr[MCAN_RXELEMENT_ANMF_WORD]);
	element->filterIndex = (uint8_t)GET_FIELD_VALUE(MCAN_RXELEMENT_FIDX,
			baseAddr[MCAN_RXELEMENT_FIDX_WORD]);
	element->isCanFdFormatEnabled = IS_BIT_SET(
			MCAN_RXELEMENT_FDF, baseAddr[MCAN_RXELEMENT_FDF_WORD]);
	element->isBitRateSwitchingEnabled = IS_BIT_SET(
			MCAN_RXELEMENT_BRS, baseAddr[MCAN_RXELEMENT_BRS_WORD]);
	element->timestamp = (uint16_t)GET_FIELD_VALUE(MCAN_RXELEMENT_RXTS,
			baseAddr[MCAN_RXELEMENT_RXTS_WORD]);
	const Mcan_DataLengthRaw dataSize = decodeDataLengthCode(
			(uint8_t)GET_FIELD_VALUE(MCAN_RXELEMENT_DLC,
					baseAddr[MCAN_RXELEMENT_DLC_WORD]),
			element->isCanFdFormatEnabled);
	element->dataSize = (uint8_t)dataSize;
	const uint8_t *const dataPointer =
			(const uint8_t *)&baseAddr[MCAN_RXELEMENT_DATA_WORD];
	(void)memcpy(element->data, dataPointer, element->dataSize);

	MEMORY_SYNC_BARRIER();
}

void
Mcan_rxBufferGet(const Mcan *const mcan, const uint8_t index,
		Mcan_RxElement *const element)
{
	const uint32_t *const bufferPointer =
			&mcan->rxBufferAddress
					 [(mcan->rxBufferElementSize
							  * (uint32_t)index)
							 / sizeof(uint32_t)];

	getRxElement(bufferPointer, element);
}

bool
Mcan_rxFifoPull(Mcan *const mcan, const Mcan_RxFifoId id,
		Mcan_RxElement *const element, ErrorCode *const errCode)
{
	assert(mcan != NULL);
	uint32_t rxfs = 0u;
	const uint32_t *fifoAddress = NULL;
	uint32_t elementSize = 0u;
	volatile uint32_t *ackReg = NULL;

	switch (id) {
	case Mcan_RxFifoId_0:
		rxfs = mcan->reg.base->rxf0s;
		fifoAddress = mcan->rxFifo0.address;
		elementSize = mcan->rxFifo0.elementSize;
		ackReg = &mcan->reg.base->rxf0a;
		break;
	case Mcan_RxFifoId_1:
		rxfs = mcan->reg.base->rxf1s;
		fifoAddress = mcan->rxFifo1.address;
		elementSize = mcan->rxFifo1.elementSize;
		ackReg = &mcan->reg.base->rxf1a;
		break;
	}

	if (fifoAddress == NULL)
		return returnError(errCode, Mcan_ErrorCode_InvalidRxFifoId);

	const uint8_t count = (uint8_t)GET_FIELD_VALUE(MCAN_RXF0S_F0FL, rxfs);
	if (count == 0u)
		return returnError(errCode, Mcan_ErrorCode_RxFifoEmpty);
	const uint8_t getIndex =
			(uint8_t)GET_FIELD_VALUE(MCAN_RXF0S_F0GI, rxfs);
	const uint32_t *const baseAddress =
			&fifoAddress[(elementSize * getIndex)
					/ sizeof(uint32_t)];
	getRxElement(baseAddress, element);
	*ackReg = getIndex;

	return true;
}

bool
Mcan_getRxFifoStatus(const Mcan *const mcan, const Mcan_RxFifoId id,
		Mcan_RxFifoStatus *const status, ErrorCode *const errCode)
{
	switch (id) {
	case Mcan_RxFifoId_0: {
		const uint32_t rxf0s = mcan->reg.base->rxf0s;
		status->count = (uint8_t)GET_FIELD_VALUE(
				MCAN_RXF0S_F0FL, rxf0s);
		status->isFull = IS_BIT_SET(MCAN_RXF0S_F0F, rxf0s);
		status->isMessageLost = IS_BIT_SET(MCAN_RXF0S_RF0L, rxf0s);
		return true;
	}
	case Mcan_RxFifoId_1: {
		const uint32_t rxf1s = mcan->reg.base->rxf1s;
		status->count = (uint8_t)GET_FIELD_VALUE(
				MCAN_RXF1S_F1FL, rxf1s);
		status->isFull = IS_BIT_SET(MCAN_RXF1S_F1F, rxf1s);
		status->isMessageLost = IS_BIT_SET(MCAN_RXF1S_RF1L, rxf1s);
		return true;
	}
	}

	return returnError(errCode, Mcan_ErrorCode_InvalidRxFifoId);
}

void
Mcan_getTxQueueStatus(const Mcan *const mcan, Mcan_TxQueueStatus *const status)
{
	status->isFull = IS_BIT_SET(MCAN_TXFQS_TFQF, mcan->reg.base->txfqs);
}

void
Mcan_getTxEventFifoStatus(
		const Mcan *const mcan, Mcan_TxEventFifoStatus *const status)
{
	const uint32_t txefs = mcan->reg.base->txefs;
	status->count = (uint8_t)GET_FIELD_VALUE(MCAN_TXEFS_EFFL, txefs);
	status->isFull = IS_BIT_SET(MCAN_TXEFS_EFF, txefs);
	status->isMessageLost = IS_BIT_SET(MCAN_TXEFS_TEFL, txefs);
}

bool
Mcan_setStandardIdFilter(Mcan *const mcan, const Mcan_RxFilterElement element,
		const uint8_t index, ErrorCode *const errCode)
{
	if (index >= mcan->rxStdFilterSize)
		return returnError(errCode, Mcan_ErrorCode_IndexOutOfRange);

	uint32_t *const bufferPointer =
			&mcan->rxStdFilterAddress[(MCAN_STDRXFILTERELEMENT_SIZE
								  * index)
					/ sizeof(uint32_t)];
	*bufferPointer = BIT_FIELD_VALUE(MCAN_STDRXFILTERELEMENT_SFT,
					 element.type)
			| BIT_FIELD_VALUE(MCAN_STDRXFILTERELEMENT_SFEC,
					element.config)
			| BIT_FIELD_VALUE(MCAN_STDRXFILTERELEMENT_SFID1,
					element.id1)
			| BIT_FIELD_VALUE(MCAN_STDRXFILTERELEMENT_SFID2,
					element.id2);

	return true;
}

bool
Mcan_setExtendedIdFilter(Mcan *const mcan, const Mcan_RxFilterElement element,
		const uint8_t index, ErrorCode *const errCode)
{
	if (index >= mcan->rxExtFilterSize)
		return returnError(errCode, Mcan_ErrorCode_IndexOutOfRange);

	uint32_t *const bufferPointer =
			&mcan->rxExtFilterAddress[(MCAN_EXTRXFILTERELEMENT_SIZE
								  * index)
					/ sizeof(uint32_t)];

	(void)memset(bufferPointer, 0, MCAN_EXTRXFILTERELEMENT_SIZE);

	bufferPointer[MCAN_EXTRXFILTERELEMENT_EFT_WORD] |= BIT_FIELD_VALUE(
			MCAN_EXTRXFILTERELEMENT_EFT, element.type);
	bufferPointer[MCAN_EXTRXFILTERELEMENT_EFEC_WORD] |= BIT_FIELD_VALUE(
			MCAN_EXTRXFILTERELEMENT_EFEC, element.config);
	bufferPointer[MCAN_EXTRXFILTERELEMENT_EFID1_WORD] |= BIT_FIELD_VALUE(
			MCAN_EXTRXFILTERELEMENT_EFID1, element.id1);
	bufferPointer[MCAN_EXTRXFILTERELEMENT_EFID2_WORD] |= BIT_FIELD_VALUE(
			MCAN_EXTRXFILTERELEMENT_EFID2, element.id2);

	return true;
}

void
Mcan_getInterruptStatus(
		const Mcan *const mcan, Mcan_InterruptStatus *const status)
{
	const uint32_t flags = mcan->reg.base->ir;
	mcan->reg.base->ir = flags;

	status->hasRf0nOccurred = (flags & MCAN_IR_RF0N_MASK) != 0u;
	status->hasRf0wOccurred = (flags & MCAN_IR_RF0W_MASK) != 0u;
	status->hasRf0fOccurred = (flags & MCAN_IR_RF0F_MASK) != 0u;
	status->hasRf0lOccurred = (flags & MCAN_IR_RF0L_MASK) != 0u;
	status->hasRf1nOccurred = (flags & MCAN_IR_RF1N_MASK) != 0u;
	status->hasRf1wOccurred = (flags & MCAN_IR_RF1W_MASK) != 0u;
	status->hasRf1fOccurred = (flags & MCAN_IR_RF1F_MASK) != 0u;
	status->hasRf1lOccurred = (flags & MCAN_IR_RF1L_MASK) != 0u;
	status->hasHpmOccurred = (flags & MCAN_IR_HPM_MASK) != 0u;
	status->hasTcOccurred = (flags & MCAN_IR_TC_MASK) != 0u;
	status->hasTcfOccurred = (flags & MCAN_IR_TCF_MASK) != 0u;
	status->hasTfeOccurred = (flags & MCAN_IR_TFE_MASK) != 0u;
	status->hasTefnOccurred = (flags & MCAN_IR_TEFN_MASK) != 0u;
	status->hasTefwOccurred = (flags & MCAN_IR_TEFW_MASK) != 0u;
	status->hasTeffOccurred = (flags & MCAN_IR_TEFF_MASK) != 0u;
	status->hasTeflOccurred = (flags & MCAN_IR_TEFL_MASK) != 0u;
	status->hasTswOccurred = (flags & MCAN_IR_TSW_MASK) != 0u;
	status->hasMrafOccurred = (flags & MCAN_IR_MRAF_MASK) != 0u;
	status->hasTooOccurred = (flags & MCAN_IR_TOO_MASK) != 0u;
	status->hasDrxOccurred = (flags & MCAN_IR_DRX_MASK) != 0u;
	status->hasEloOccurred = (flags & MCAN_IR_ELO_MASK) != 0u;
	status->hasEpOccurred = (flags & MCAN_IR_EP_MASK) != 0u;
	status->hasEwOccurred = (flags & MCAN_IR_EW_MASK) != 0u;
	status->hasBoOccurred = (flags & MCAN_IR_BO_MASK) != 0u;
	status->hasWdiOccurred = (flags & MCAN_IR_WDI_MASK) != 0u;
	status->hasPeaOccurred = (flags & MCAN_IR_PEA_MASK) != 0u;
	status->hasPedOccurred = (flags & MCAN_IR_PED_MASK) != 0u;
	status->hasAraOccurred = (flags & MCAN_IR_ARA_MASK) != 0u;
}

bool
Mcan_isTxFifoEmpty(const Mcan *const mcan)
{
	const uint32_t freeLevel =
			GET_FIELD_VALUE(MCAN_TXFQS_TFFL, mcan->reg.base->txfqs);
	const uint32_t queueSize =
			GET_FIELD_VALUE(MCAN_TXBC_TFQS, mcan->reg.base->txbc);
	return freeLevel == queueSize;
}
