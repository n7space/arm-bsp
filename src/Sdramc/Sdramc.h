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

/// \brief Header for the SDRAM Controller (SDRAMC) driver.

/**
 * @defgroup Sdramc Sdramc
 * @ingroup Bsp
 * @{
 */

#ifndef BSP_SDRAMC_H
#define BSP_SDRAMC_H

#include <stdbool.h>
#include <stdint.h>

#include "SdramcRegisters.h"

/// \brief Enumeration listing SDRAMC command modes.
typedef enum {
	Sdramc_CommandMode_Normal = 0, ///< Accesses are decoded normally.
	Sdramc_CommandMode_Nop = 1, ///< Accesses are prepended with a NOP.
	Sdramc_CommandMode_AllBanksPrecharge =
			2, ///< Accesses are prepended with "All Banks Precharge"
	Sdramc_CommandMode_LoadModeReg =
			3, ///< Accesses are prepended with "Load Mode Register"
	Sdramc_CommandMode_AutoRefresh =
			4, ///< Accesses are prepended with "Auto Refresh"
	/// \brief Accesses are prepended with "Extended Load Mode Register"
	Sdramc_CommandMode_ExtLoadModeReg = 5,
	Sdramc_CommandMode_DeepPowerdown = 6, ///< Enters Deep Powerdown mode.
} Sdramc_CommandMode;

/// \brief Enumeration listing possible numbers of column bits.
typedef enum {
	Sdramc_NumberOfColumnBits_Col8 = 0, ///< 8 bits, up to 256 columns.
	Sdramc_NumberOfColumnBits_Col9 = 1, ///< 9 bits, up to 512 columns.
	Sdramc_NumberOfColumnBits_Col10 = 2, ///< 10 bits, up to 1024 columns.
	Sdramc_NumberOfColumnBits_Col11 = 3, ///< 11 bits, up to 2048 columns.
} Sdramc_NumberOfColumnBits;

/// \brief Enumeration listing possible numbers of row bits.
typedef enum {
	Sdramc_NumberOfRowBits_Row11 = 0, ///< 11 bits, up to 2048 rows.
	Sdramc_NumberOfRowBits_Row12 = 1, ///< 12 bits, up to 4096 rows.
	Sdramc_NumberOfRowBits_Row13 = 2, ///< 13 bits, up to 8192 rows.
	Sdramc_NumberOfRowBits_Reserved = 3, ///< Reserved.
} Sdramc_NumberOfRowBits;

/// \brief Enumeration listing possible numbers of banks.
typedef enum {
	Sdramc_NumberOfBanks_Bank2 = 0, ///< 2 banks.
	Sdramc_NumberOfBanks_Bank4 = 1, ///< 4 banks.
} Sdramc_NumberOfBanks;

/// \brief Enumeration listing possible CAS latencies.
typedef enum {
	Sdramc_CasLatency_Reserved = 0, ///< Reserved.
	Sdramc_CasLatency_Latency1 = 1, ///< 1 cycle latency.
	Sdramc_CasLatency_Latency2 = 2, ///< 2 cycle latency.
	Sdramc_CasLatency_Latency3 = 3, ///< 3 cycle latency.
} Sdramc_CasLatency;

/// \brief Enumeration listing possible bus widths.
typedef enum {
	Sdramc_DataBusWidth_16bits = 1, ///< 16 bit data bus.
} Sdramc_DataBusWidth;

/// \brief Enumeration listing possible low-power configuration.
typedef enum {
	Sdramc_LowPowerConfiguration_Disabled =
			0, ///< Low-power feature is inhibited.
	Sdramc_LowPowerConfiguration_SelfRefresh =
			1, ///< Self-refresh command is issued.
	Sdramc_LowPowerConfiguration_PowerDown =
			2, ///< Power-down commands are issued.
	Sdramc_LowPowerConfiguration_DeepPowerDown =
			3, ///< Deep power-down command is issued.
} Sdramc_LowPowerConfiguration;

/// \brief Enumeration listing possible Times to Define When Low-power Mode is
/// Enabled.
typedef enum {
	Sdramc_LowPowerEnableTimeout_LpLastXfer =
			0, ///< Immediately after the last transfer.
	Sdramc_LowPowerEnableTimeout_LpLastXfer64 =
			1, ///< 64 cycles after the last transfer.
	Sdramc_LowPowerEnableTimeout_LpLastXfer128 =
			2, ///< 128 cycles after the last transfer.
	Sdramc_LowPowerEnableTimeout_Reserved = 3, ///< Reserved.
} Sdramc_LowPowerEnableTimeout;

/// \brief Enumeration listing memory device types.
typedef enum {
	Sdramc_MemoryDeviceType_Sdram = 0, ///< SDRAM.
	Sdramc_MemoryDeviceType_LPSDRAM = 1, ///< Low-power SDRAM.
	Sdramc_MemoryDeviceType_Reserved1 = 2, ///< Reserved.
	Sdramc_MemoryDeviceType_Reserved2 = 3, ///< Reserved.
} Sdramc_MemoryDeviceType;

/// \brief Enumeration listring possible values of unaligned access support.
typedef enum {
	Sdramc_SupportUnalignedAccess_Unsupported =
			0, ///< Unaligned access is not supported.
	Sdramc_SupportUnalignedAccess_Supported =
			1, ///< Unaligned access is supported.
} Sdramc_SupportUnalignedAccess;

/// \brief Structure representing SDRAMC configuration.
typedef struct {
	uint32_t refreshTimerCount; ///< SDRAMC Refresh Timer Count (12-bit).
	Sdramc_NumberOfColumnBits columnBits; ///< Number of Column Bits.
	Sdramc_NumberOfRowBits rowBits; ///< Number of Row Bits.
	Sdramc_NumberOfBanks banks; ///< Number of Banks.
	Sdramc_CasLatency casLatency; ///< CAS Latency.
	Sdramc_DataBusWidth dataBusWidth; ///< Data Bus Width.
	uint8_t writeRecoveryDelay; ///< Write Recovery Delay (TWR).
	uint8_t rowCycleDelayAndRowRefreshCycle; ///< Row Cycle Delay and Row Refresh Cycle (TRC_TRFC).
	uint8_t rowPrechargeDelay; ///< Row Precharge Delay (TRP).
	uint8_t rowToColumnDelay; ///< Row to Column Delay (TRCD).
	uint8_t activeToPrechargeDelay; ///< Active to Precharge Delay (TRAS).
	uint8_t exitSelfRefreshToActiveDelay; ///< Exit Self-Refresh to Active Delay (TXSR).
	/// \brief Low-power configuration.
	Sdramc_LowPowerConfiguration lowPowerConfiguration;
	uint8_t partialArraySelfRefresh; ///< Partial Array Self-refresh (PASR); device specific.
	/// \brief Temperature Compensated Self-Refrsh (TCSR); device specific.
	uint8_t temperatureCompensatedSelfRefresh;
	uint8_t driveStrength; ///< Drive Strength (DS); device specific.
	/// \brief Timeout: Time to Define When Low-power Mode is Enabled.
	Sdramc_LowPowerEnableTimeout lowPowerEnableTimeout;
	Sdramc_MemoryDeviceType deviceType; ///< Memory device type.
	/// \brief Load Mode Register Command to Active or Refresh Command
	/// (TMRD).
	uint8_t loadModeRegisterCommandToActiveOrRefreshCommand;
	/// \brief Support Unaligned Access.
	Sdramc_SupportUnalignedAccess supportUnalignedAccess;
	/// \brief Is Refresh Error Status interrupt enabled.
	bool isRefreshErrorStatusInterruptEnabled;
} Sdramc_Config;

/// \brief Structure representing SDRAMC.
typedef struct {
	Sdramc_Registers *registers; ///< Pointer to SDRAMC registers.
	Sdramc_MatrixRegisters
			*matrixRegisters; ///< Pointer to MATRIX registers.
	/// \brief Pointer to Cache Control Identification Registers.
	Sdramc_CacheControlIdentificationRegisters
			*cacheControlIdentificationRegisters;
	/// \brief Pointer to Cache and Branch Predictor Registers.
	Sdramc_CacheAndBranchPredictorRegisters
			*cacheAndBranchPredictorRegisters;
	Sdramc_Config configuration; ///< Copy of the user submitted configuration.
} Sdramc;

/// \brief Structure representing Off-chip Memory Scramble configuration.
typedef struct {
	bool isEnabled; ///< Is off-chip memory scramble (OCMS) enabled.
	uint64_t key; ///< OCMS Key.
} Sdramc_OcmsConfig;

/// \brief Initializes the structure representing SDRAMC.
/// \param [in,out] sdramc Pointer to a structure representing SDRAMC.
void Sdramc_init(Sdramc *const sdramc);

/// \brief Enables SDRAM hardware.
/// \param [in] sdramc Pointer to a structure representing SDRAMC.
void Sdramc_startup(Sdramc *const sdramc);

/// \brief Disables SDRAM hardware.
/// \param [in] sdramc Pointer to a structure representing SDRAMC.
void Sdramc_shutdown(Sdramc *const sdramc);

/// \brief Sets the SDRAMC configuration. Must be called before performing
/// initialization sequece. \param [in,out] sdramc Pointer to a structure
/// representing SDRAMC. \param [in] config SDRAMC configuration.
void Sdramc_setConfig(Sdramc *const sdramc, const Sdramc_Config *const config);

/// \brief Gets the SDRAMC configuration.
/// \param [in] sdramc Pointer to a structure representing SDRAMC.
/// \param [out] config SDRAMC configuration.
void Sdramc_getConfig(Sdramc *const sdramc, Sdramc_Config *const config);

/// \brief Sets the SDRAMC OCMS configuration.
///        Must be called when the external memory device is not powered.
/// \param [in,out] sdramc Pointer to a structure representing SDRAMC.
/// \param [in] config SDRAMC OCMS configuration.
void Sdramc_setOcmsConfig(
		Sdramc *const sdramc, const Sdramc_OcmsConfig *const config);

/// \brief Gets the SDRAMC OCMS configuration. Only enable status is actually
/// read. \param [in] sdramc Pointer to a structure representing SDRAMC. \param
/// [out] config SDRAMC OCMS configuration.
void Sdramc_getOcmsConfig(
		const Sdramc *const sdramc, Sdramc_OcmsConfig *const config);

/// \brief Perform SDRAMC initialization sequence. Must be called after
/// configuration has been set. \param [in] sdramc Pointer to a structure
/// representing SDRAMC. \param [in] clockFrequency CPU core clock frequency,
/// used to properly calculate a delay.
void Sdramc_performInitializationSequence(
		Sdramc *const sdramc, uint32_t clockFrequency);

/// \brief Returns whether Refresh Error has been detected since the last time
/// its status was read. \param [in] sdramc Pointer to a structure representing
/// SDRAMC. \retval true Refresh Error has been detected since the last status
/// read. \retval false Refresh Error has not been detected.
bool Sdramc_hasRefreshErrorBeenDetected(const Sdramc *const sdramc);

#endif // BSP_SDRAMC_H

/** @} */
