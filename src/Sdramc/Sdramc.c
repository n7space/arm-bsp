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

#include "Sdramc.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

#define SDRAMC_MICROSECONDS_PER_SECODND 1000000u
#define SDRAMC_CYCLES_PER_ITERATION 6u
#define SDRAMC_PREINITIALIZATION_PAUSE_DELAY_US 200u
#define SDRAMC_AUTOREFRESH_PREINITIALIZATION_STEPS 9u
#define BYTE_ADDRESS_BIT_COUNT 1u

void
Sdramc_init(Sdramc *const sdramc)
{
	memset(&(sdramc->configuration), 0, sizeof(Sdramc_Config));
	sdramc->registers = (Sdramc_Registers *)SDRAMC_REGISTERS_ADDRESS_BASE;
	sdramc->matrixRegisters =
			(Sdramc_MatrixRegisters *)SDRAMC_MATRIX_REGISTER_BASE;
	sdramc->cacheAndBranchPredictorRegisters =
			(Sdramc_CacheAndBranchPredictorRegisters *)
					SDRAMC_DCCISW_REGISTER_ADDRESS;
	sdramc->cacheControlIdentificationRegisters =
			(Sdramc_CacheControlIdentificationRegisters *)
					SDRAMC_CCSIDR_REGISTER_ADDRESS;
}

void
Sdramc_startup(Sdramc *const sdramc)
{
	sdramc->matrixRegisters->ccfg_smcnfcs |=
			SDRAMC_MATRIX_CCFG_SMCNFCS_SDRAMEN_MASK;
}

void
Sdramc_shutdown(Sdramc *const sdramc)
{
	sdramc->matrixRegisters->ccfg_smcnfcs &=
			~SDRAMC_MATRIX_CCFG_SMCNFCS_SDRAMEN_MASK;
}

static inline void
setLpsdramConfig(Sdramc *const sdramc, const Sdramc_Config *const config)
{
	const uint32_t lprMask = ~(SDRAMC_LPR_PASR_MASK | SDRAMC_LPR_TCSR_MASK
			| SDRAMC_LPR_DS_MASK);
	sdramc->registers->lpr = (sdramc->registers->lpr & lprMask)
			| (((uint32_t)config->partialArraySelfRefresh
					   << SDRAMC_LPR_PASR_OFFSET)
					& SDRAMC_LPR_PASR_MASK)
			| (((uint32_t)config->temperatureCompensatedSelfRefresh
					   << SDRAMC_LPR_TCSR_OFFSET)
					& SDRAMC_LPR_TCSR_MASK)
			| (((uint32_t)config->driveStrength
					   << SDRAMC_LPR_DS_OFFSET)
					& SDRAMC_LPR_DS_MASK);
}

static uint32_t
getCrValue(const Sdramc_Config *const config)
{
	return (((uint32_t)config->columnBits << SDRAMC_CR_NC_OFFSET)
			       & SDRAMC_CR_NC_MASK)
			| (((uint32_t)config->rowBits << SDRAMC_CR_NR_OFFSET)
					& SDRAMC_CR_NR_MASK)
			| (((uint32_t)config->banks << SDRAMC_CR_NB_OFFSET)
					& SDRAMC_CR_NB_MASK)
			| (((uint32_t)config->casLatency
					   << SDRAMC_CR_CAS_OFFSET)
					& SDRAMC_CR_CAS_MASK)
			| (((uint32_t)config->dataBusWidth
					   << SDRAMC_CR_DBW_OFFSET)
					& SDRAMC_CR_DBW_MASK)
			| (((uint32_t)config->writeRecoveryDelay
					   << SDRAMC_CR_TWR_OFFSET)
					& SDRAMC_CR_TWR_MASK)
			| (((uint32_t)config->rowCycleDelayAndRowRefreshCycle
					   << SDRAMC_CR_TRC_TRFC_OFFSET)
					& SDRAMC_CR_TRC_TRFC_MASK)
			| (((uint32_t)config->rowPrechargeDelay
					   << SDRAMC_CR_TRP_OFFSET)
					& SDRAMC_CR_TRP_MASK)
			| (((uint32_t)config->rowToColumnDelay
					   << SDRAMC_CR_TRCD_OFFSET)
					& SDRAMC_CR_TRCD_MASK)
			| (((uint32_t)config->activeToPrechargeDelay
					   << SDRAMC_CR_TRAS_OFFSET)
					& SDRAMC_CR_TRAS_MASK)
			| (((uint32_t)config->exitSelfRefreshToActiveDelay
					   << SDRAMC_CR_TXSR_OFFSET)
					& SDRAMC_CR_TXSR_MASK);
}

static uint32_t
getCfr1Value(const Sdramc_Config *const config)
{
	return (((uint32_t)config->loadModeRegisterCommandToActiveOrRefreshCommand
				<< SDRAMC_CFR1_TMRD_OFFSET)
			       & SDRAMC_CFR1_TMRD_MASK)
			| (((uint32_t)config->supportUnalignedAccess
					   << SDRAMC_CFR1_UNAL_OFFSET)
					& SDRAMC_CFR1_UNAL_MASK);
}

static uint32_t
getLprValue(const Sdramc *const sdramc, const Sdramc_Config *const config)
{
	return (sdramc->registers->lpr
			       & (~(SDRAMC_LPR_LPCB_MASK
					       | SDRAMC_LPR_TIMEOUT_MASK)))
			| (((uint32_t)config->lowPowerConfiguration
					   << SDRAMC_LPR_LPCB_OFFSET)
					& SDRAMC_LPR_LPCB_MASK)
			| (((uint32_t)config->lowPowerEnableTimeout
					   << SDRAMC_LPR_TIMEOUT_OFFSET)
					& SDRAMC_LPR_TIMEOUT_MASK);
}

void
Sdramc_setConfig(Sdramc *const sdramc, const Sdramc_Config *const config)
{
	sdramc->configuration = *config;

	sdramc->registers->cr = getCrValue(config);
	sdramc->registers->cfr1 = getCfr1Value(config);
	sdramc->registers->lpr = getLprValue(sdramc, config);

	if (config->deviceType == Sdramc_MemoryDeviceType_LPSDRAM)
		setLpsdramConfig(sdramc, config);

	sdramc->registers->mdr =
			((uint32_t)config->deviceType << SDRAMC_MDR_MD_OFFSET)
			& SDRAMC_MDR_MD_MASK;

	if (config->isRefreshErrorStatusInterruptEnabled)
		sdramc->registers->ier = (1u << SDRAMC_IER_RES_OFFSET)
				& SDRAMC_IER_RES_MASK;
	else
		sdramc->registers->idr = (1u << SDRAMC_IDR_RES_OFFSET)
				& SDRAMC_IDR_RES_MASK;
}

static void
readCrRegistry(const Sdramc *const sdramc, Sdramc_Config *const config)
{
	const uint32_t cr = sdramc->registers->cr;
	config->columnBits = (Sdramc_NumberOfColumnBits)(
			(cr & SDRAMC_CR_NC_MASK) >> SDRAMC_CR_NC_OFFSET);
	config->rowBits = (Sdramc_NumberOfRowBits)(
			(cr & SDRAMC_CR_NR_MASK) >> SDRAMC_CR_NR_OFFSET);
	config->banks = (Sdramc_NumberOfBanks)(
			(cr & SDRAMC_CR_NB_MASK) >> SDRAMC_CR_NB_OFFSET);
	config->casLatency = (Sdramc_CasLatency)(
			(cr & SDRAMC_CR_CAS_MASK) >> SDRAMC_CR_CAS_OFFSET);
	config->dataBusWidth = (Sdramc_DataBusWidth)(
			(cr & SDRAMC_CR_DBW_MASK) >> SDRAMC_CR_DBW_OFFSET);
	config->writeRecoveryDelay = (uint8_t)(
			(cr & SDRAMC_CR_TWR_MASK) >> SDRAMC_CR_TWR_OFFSET);
	config->rowCycleDelayAndRowRefreshCycle =
			(uint8_t)((cr & SDRAMC_CR_TRC_TRFC_MASK)
					>> SDRAMC_CR_TRC_TRFC_OFFSET);
	config->rowPrechargeDelay = (uint8_t)(
			(cr & SDRAMC_CR_TRP_MASK) >> SDRAMC_CR_TRP_OFFSET);
	config->rowToColumnDelay = (uint8_t)(
			(cr & SDRAMC_CR_TRCD_MASK) >> SDRAMC_CR_TRCD_OFFSET);
	config->activeToPrechargeDelay = (uint8_t)(
			(cr & SDRAMC_CR_TRAS_MASK) >> SDRAMC_CR_TRAS_OFFSET);
	config->exitSelfRefreshToActiveDelay = (uint8_t)(
			(cr & SDRAMC_CR_TXSR_MASK) >> SDRAMC_CR_TXSR_OFFSET);
}

static void
readCfr1(const Sdramc *const sdramc, Sdramc_Config *const config)
{
	const uint32_t cfr1 = sdramc->registers->cfr1;
	config->supportUnalignedAccess = (Sdramc_SupportUnalignedAccess)(
			(cfr1 & SDRAMC_CFR1_UNAL_MASK)
			>> SDRAMC_CFR1_UNAL_OFFSET);
	config->loadModeRegisterCommandToActiveOrRefreshCommand =
			((cfr1 & SDRAMC_CFR1_TMRD_MASK)
					>> SDRAMC_CFR1_TMRD_OFFSET);
}

static void
readLpr(const Sdramc *const sdramc, Sdramc_Config *const config)
{
	const uint32_t lpr = sdramc->registers->lpr;
	config->lowPowerConfiguration = (Sdramc_LowPowerConfiguration)(
			(lpr & SDRAMC_LPR_LPCB_MASK) >> SDRAMC_LPR_LPCB_OFFSET);
	config->partialArraySelfRefresh =
			(lpr & SDRAMC_LPR_PASR_MASK) >> SDRAMC_LPR_PASR_OFFSET;
	config->temperatureCompensatedSelfRefresh =
			(lpr & SDRAMC_LPR_TCSR_MASK) >> SDRAMC_LPR_TCSR_OFFSET;
	config->driveStrength =
			(lpr & SDRAMC_LPR_DS_MASK) >> SDRAMC_LPR_DS_OFFSET;
	config->lowPowerEnableTimeout = (Sdramc_LowPowerEnableTimeout)(
			(lpr & SDRAMC_LPR_TIMEOUT_MASK)
			>> SDRAMC_LPR_TIMEOUT_OFFSET);
}

void
Sdramc_getConfig(Sdramc *const sdramc, Sdramc_Config *const config)
{
	readCrRegistry(sdramc, config);

	config->refreshTimerCount =
			(sdramc->registers->tr & SDRAMC_TR_COUNT_MASK)
			>> SDRAMC_TR_COUNT_OFFSET;

	config->deviceType = (Sdramc_MemoryDeviceType)(
			(sdramc->registers->mdr & SDRAMC_MDR_MD_MASK)
			>> SDRAMC_MDR_MD_OFFSET);

	readCfr1(sdramc, config);
	readLpr(sdramc, config);

	config->isRefreshErrorStatusInterruptEnabled =
			(sdramc->registers->imr & SDRAMC_IMR_RES_MASK) != 0u;

	sdramc->configuration = *config;
}

void
Sdramc_setOcmsConfig(
		Sdramc *const sdramc, const Sdramc_OcmsConfig *const config)
{
	sdramc->registers->ocmsKey1 = (uint32_t)(config->key & 0xFFFFFFFFu);
	sdramc->registers->ocmsKey2 =
			(uint32_t)((config->key >> 32u) & 0xFFFFFFFFu);
	sdramc->registers->ocms =
			(sdramc->registers->ocms & (~SDRAMC_OCMS_SDR_SE_MASK))
			| (((config->isEnabled ? 1U : 0U)
					   << SDRAMC_OCMS_SDR_SE_OFFSET)
					& SDRAMC_OCMS_SDR_SE_MASK);
}

void
Sdramc_getOcmsConfig(
		const Sdramc *const sdramc, Sdramc_OcmsConfig *const config)
{
	config->isEnabled = (sdramc->registers->ocms & SDRAMC_OCMS_SDR_SE_MASK)
			!= 0u;
	// Keys are write only, so they cannot be read back.
}

static inline void
delay(uint32_t clockFrequency, uint32_t us)
{
	const uint32_t cycles =
			(clockFrequency * us) / SDRAMC_MICROSECONDS_PER_SECODND;
	const uint32_t iterations = cycles / SDRAMC_CYCLES_PER_ITERATION;
	for (uint32_t i = 0; i < iterations; i++)
		asm volatile("nop");
}

static inline void
memoryBarrier(void)
{
	asm("dmb"); ///< Data Memory Barrier.
	asm("dsb"); ///< Data Synchronization Barrier.
}

static inline void
performWriteToBank00MemoryAddress(uint16_t data)
{
	volatile uint16_t *dummy = (uint16_t *)SDRAMC_SDRAM_ADRESS_BASE;
	*dummy = data;
}

static inline uint32_t
getNumberOfRowBitsValue(Sdramc_NumberOfRowBits enumeration)
{
	switch (enumeration) {
	case Sdramc_NumberOfRowBits_Row11: return 11;
	case Sdramc_NumberOfRowBits_Row12: return 12;
	case Sdramc_NumberOfRowBits_Row13: return 13;
	case Sdramc_NumberOfRowBits_Reserved: break;
	}
	assert(false && "getRowBits received incorrect enumeration value");
	return 0;
}

static inline uint32_t
getNumberOfColumnBitsValue(Sdramc_NumberOfColumnBits enumeration)
{
	switch (enumeration) {
	case Sdramc_NumberOfColumnBits_Col8: return 8;
	case Sdramc_NumberOfColumnBits_Col9: return 9;
	case Sdramc_NumberOfColumnBits_Col10: return 10;
	case Sdramc_NumberOfColumnBits_Col11: return 11;
	}
	assert(false && "getColumnBits received incorrect enumeration value");
	return 0;
}

static inline uint32_t
getBankAddressBitCount(Sdramc_NumberOfBanks banks)
{
	switch (banks) {
	case Sdramc_NumberOfBanks_Bank2: return 1U;
	case Sdramc_NumberOfBanks_Bank4: return 2U;
	}
	assert(false && "getBankAddressBitCount received incorrect enumeration value");
	return 0;
}

static inline void
performWriteToBank01MemoryAddress(Sdramc_Config *const config)
{
	const uint32_t offset =
			1U << (getNumberOfColumnBitsValue(config->columnBits)
					+ getNumberOfRowBitsValue(
							config->rowBits)
					+ getBankAddressBitCount(config->banks)
					+ BYTE_ADDRESS_BIT_COUNT);
	volatile uint16_t *dummy =
			(uint16_t *)(SDRAMC_SDRAM_ADRESS_BASE + offset);
	*dummy = 0;
}

static inline void
setMode(Sdramc *const sdramc, Sdramc_CommandMode mode)
{
	volatile uint32_t mr = sdramc->registers->mr;
	mr = (mr & (~SDRAMC_MR_MODE_MASK))
			| (((uint32_t)mode << SDRAMC_MR_MODE_OFFSET)
					& SDRAMC_MR_MODE_MASK);
	sdramc->registers->mr = mr; // Write the updated mode.
	mr = sdramc->registers->mr; // Perform a required MR register read
	mr += 0u;
	(void)mr; // Satisfy CppCheck.
	memoryBarrier();
}

void
Sdramc_performInitializationSequence(
		Sdramc *const sdramc, uint32_t clockFrequency)
{
	// The assumption is that the configuration of CR, CFR1 and LPR
	// registers is already performed. Ensure after-configuration delay.
	delay(clockFrequency, SDRAMC_PREINITIALIZATION_PAUSE_DELAY_US);
	setMode(sdramc, Sdramc_CommandMode_Nop);
	performWriteToBank00MemoryAddress(0);
	setMode(sdramc, Sdramc_CommandMode_AllBanksPrecharge);
	performWriteToBank00MemoryAddress(0);
	delay(clockFrequency, SDRAMC_PREINITIALIZATION_PAUSE_DELAY_US);
	for (uint16_t i = 1U; i < SDRAMC_AUTOREFRESH_PREINITIALIZATION_STEPS;
			i++) {
		setMode(sdramc, Sdramc_CommandMode_AutoRefresh);
		performWriteToBank00MemoryAddress(i);
	}
	setMode(sdramc, Sdramc_CommandMode_LoadModeReg);
	performWriteToBank00MemoryAddress(0);
	delay(clockFrequency, SDRAMC_PREINITIALIZATION_PAUSE_DELAY_US);
	if (sdramc->configuration.deviceType
			== Sdramc_MemoryDeviceType_LPSDRAM) {
		setMode(sdramc, Sdramc_CommandMode_ExtLoadModeReg);
		performWriteToBank01MemoryAddress(&(sdramc->configuration));
	}
	setMode(sdramc, Sdramc_CommandMode_Normal);
	performWriteToBank00MemoryAddress(0);
	delay(clockFrequency, SDRAMC_PREINITIALIZATION_PAUSE_DELAY_US);
	sdramc->registers->tr =
			(sdramc->registers->tr & (~SDRAMC_TR_COUNT_MASK))
			| ((sdramc->configuration.refreshTimerCount
					   << SDRAMC_TR_COUNT_OFFSET)
					& SDRAMC_TR_COUNT_MASK);
}

bool
Sdramc_hasRefreshErrorBeenDetected(const Sdramc *const sdramc)
{
	return (sdramc->registers->isr & SDRAMC_ISR_RES_MASK) != 0u;
}
