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

/// \file Pmc.h
/// \addtogroup Bsp
/// \brief Main header file for the Power Management Controller (PMC) driver.

#ifndef BSP_PMC_H
#define BSP_PMC_H

#include <stdbool.h>
#include <stdint.h>

#include <Utils/ErrorCode.h>

#include "PmcPeripheralId.h"
#include "PmcRegisters.h"

/// @addtogroup Pmc
/// @ingroup Bsp
/// @{

#ifdef __cplusplus
extern "C" {
#endif

/// \brief The built-in slow clock frequency in Hz.
#define PMC_SLOW_CLOCK_FREQ 32768u

/// \brief Default timeout for PMC api, value in iterations of busy-wait loop
#define PMC_DEFAULT_TIMEOUT 120000u

#ifndef PMC_MAIN_CRYSTAL_FREQ
/// \brief The main crystal oscillator frequency in Hz (can be overriden by an external define).
#if defined(N7S_TARGET_SAMV71Q21)
#define PMC_MAIN_CRYSTAL_FREQ 12000000u
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define PMC_MAIN_CRYSTAL_FREQ 10000000u
#else
#error "No target platform specified (missing N7S_TARGET_* macro)"
#endif
#endif // PMC_MAIN_CRYSTAL_FREQ

/// \brief Enumeration listing possible error codes.
typedef enum {
	/// \brief The measured clock frequency is incorrect.
	Pmc_ErrorCode_MeasuredFrequencyIsIncorrect =
			ERROR_CODE_DEFINE('P', 'M', 'C', 1),
	/// \brief PLL A not locked in a specified time.
	Pmc_ErrorCode_PllaNotLocked = ERROR_CODE_DEFINE('P', 'M', 'C', 2),
	/// \brief Master clock ready flag not raised in specified time.
	Pmc_ErrorCode_MasterClkReadyTimeout =
			ERROR_CODE_DEFINE('P', 'M', 'C', 3),
	/// \brief Main clock selection flag not raised in specified time.
	Pmc_ErrorCode_MainClkSelectTimeout =
			ERROR_CODE_DEFINE('P', 'M', 'C', 4),
	/// \brief Main RC oscillator failed to stabilize in specified time.
	Pmc_ErrorCode_RCStabilizationTimeout =
			ERROR_CODE_DEFINE('P', 'M', 'C', 5),
	/// \brief Crystal oscillator failed to stabilize in specified time.
	Pmc_ErrorCode_XtalStabilizationTimeout =
			ERROR_CODE_DEFINE('P', 'M', 'C', 6),
	/// \brief Failed to measure main clock frequency within specified time
	Pmc_ErrorCode_FrequencyMeasurementTimeout =
			ERROR_CODE_DEFINE('P', 'M', 'C', 7),
	/// \brief Programmable clock failed to raise ready flag within specified time
	Pmc_ErrorCode_PckReadyTimeout = ERROR_CODE_DEFINE('P', 'M', 'C', 8),
	/// \brief Invalid clock source selected
	Pmc_ErrorCode_InvalidClockSource = ERROR_CODE_DEFINE('P', 'M', 'C', 9),
	/// \brief Invalid Master clock divider selected
	Pmc_ErrorCode_InvalidMasterckDiv = ERROR_CODE_DEFINE('P', 'M', 'C', 10),
#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	/// \brief PLL B not locked in a specified time.
	Pmc_ErrorCode_PllbNotLocked = ERROR_CODE_DEFINE('P', 'M', 'C', 11),
#endif
} Pmc_ErrorCode;

/// \brief Main clock source selection enumeration.
typedef enum {
	Pmc_MainckSrc_RcOsc, ///< Use the main RC oscillator.
	Pmc_MainckSrc_XOsc, ///< Use the main crystal oscillator.
#if defined(N7S_TARGET_SAMV71Q21) || defined(N7S_TARGET_SAMRH71F20)
	Pmc_MainckSrc_XOscBypassed, ///< Use an external clock (bypass the main crystal oscillator).
#endif
} Pmc_MainckSrc;

/// \brief Main RC oscillator frequency selection enumeration.
typedef enum {
	Pmc_RcOscFreq_4M = 0, ///< Set the frequency at 4MHz.
	Pmc_RcOscFreq_8M = 1, ///< Set the frequency at 8MHz.
#if defined(N7S_TARGET_SAMV71Q21)
	Pmc_RcOscFreq_12M = 2, ///< Set the frequency at 12MHz.
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	Pmc_RcOscFreq_10M = 2, ///< Set the frequency at 10MHz.
	Pmc_RcOscFreq_12M = 3, ///< Set the frequency at 12MHz.
#endif
} Pmc_RcOscFreq;

/// \brief Master clock source selection enumeration.
typedef enum {
	Pmc_MasterckSrc_Slck = 0, ///< Use the slow clock.
	Pmc_MasterckSrc_Mainck = 1, ///< Use the main clock.
	Pmc_MasterckSrc_Pllack = 2, ///< Use the PLLA clock.
} Pmc_MasterckSrc;

/// \brief Master clock (processor clock) prescaler enumeration.
typedef enum {
	Pmc_MasterckPresc_1 = 0, ///< Don't divide the selected clock.
	Pmc_MasterckPresc_2 = 1, ///< Divide the selected clock by 2.
	Pmc_MasterckPresc_4 = 2, ///< Divide the selected clock by 4.
	Pmc_MasterckPresc_8 = 3, ///< Divide the selected clock by 8.
	Pmc_MasterckPresc_16 = 4, ///< Divide the selected clock by 16.
	Pmc_MasterckPresc_32 = 5, ///< Divide the selected clock by 32.
	Pmc_MasterckPresc_64 = 6, ///< Divide the selected clock by 64.
#if defined(N7S_TARGET_SAMV71Q21)
	Pmc_MasterckPresc_3 = 7, ///< Divide the selected clock by 3.
#endif
} Pmc_MasterckPresc;

/// \brief Master clock division selection enumeration. Only divider values 1 and 2 are allowed
///        according to the SAMV71 family errata DS80000767A, 5.1
typedef enum {
	Pmc_MasterckDiv_1 = 0, ///< Divide the clock further by 1.
	Pmc_MasterckDiv_2 = 1, ///< Divide the clock further by 2.
} Pmc_MasterckDiv;

/// \brief Programmable clock source selection enumeration.
typedef enum {
	Pmc_PckSrc_Slck = 0, ///< Use the slow clock.
	Pmc_PckSrc_Mainck = 1, ///< Use the main clock.
	Pmc_PckSrc_Pllack = 2, ///< Use the PLLA clock.
#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	Pmc_PckSrc_Pllbck = 3, ///< Use the PLLB clock.
#endif
	Pmc_PckSrc_Masterck = 4, ///< Use the master clock.
} Pmc_PckSrc;

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
/// \brief Enumeration listing possible Pllb clock sources
typedef enum {
	Pmc_PllbSrc_Mainck = 0, ///< Main clock
	Pmc_PllbSrc_Rc2ck = 1, ///< Rc2 clock
} Pmc_PllbSrc;

/// \brief Possible PLL filter resistance values.
typedef enum {
	Pmc_FilterResistor_24K = 0, ///< 24 kOhm resistor
	Pmc_FilterResistor_6K = 1, ///< 6 kOhm resistor
	Pmc_FilterResistor_3K = 2, ///< 3 kOhm resistor
	Pmc_FilterResistor_12K = 3 ///< 12 kOhm resistor
} Pmc_FilterResistor;

/// \brief Possible PLL filter capacitance values.
typedef enum {
	Pmc_FilterCapacitor_20p = 0, ///< 20 pF capacitor
	Pmc_FilterCapacitor_40p = 1, ///< 40 pF capacitor
	Pmc_FilterCapacitor_30p = 2, ///< 30 pF capacitor
	Pmc_FilterCapacitor_60p = 3 ///< 60 pF capacitor
} Pmc_FilterCapacitor;

/// \brief Possible PLL output currents.
typedef enum {
	Pmc_PllCurrent_500u = 0, ///< 0,5 mA current
	Pmc_PllCurrent_750u = 1, ///< 0,75 mA current
	Pmc_PllCurrent_1000u = 2, ///< 1 mA current
	Pmc_PllCurrent_1250u = 3 ///< 1,250 mA current
} Pmc_PllCurrent;

/// \brief RC oscillator selector.
typedef enum {
	Pmc_RcOsc_MainRc = 0,
	Pmc_RcOsc_SecondRc = 1,
} Pmc_RcOsc;

typedef struct {
	bool isEnabled; ///< Second RC oscillator activation flag.
	Pmc_RcOscFreq frequency; ///< Second RC oscillator frequency selector.
} Pmc_Rc2OscConfig;
#endif

/// \brief Possible Pck clock identifiers.
typedef enum {
	Pmc_PckId_0 = 0,
	Pmc_PckId_1 = 1,
	Pmc_PckId_2 = 2,
	Pmc_PckId_3 = 3,
#if defined(N7S_TARGET_SAMV71Q21)
	Pmc_PckId_4 = 4,
	Pmc_PckId_5 = 5,
	Pmc_PckId_6 = 6,
	Pmc_PckId_7 = 7,
	Pmc_PckId_Count = 8
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	Pmc_PckId_Count = 4
#endif
} Pmc_PckId;

/// \brief Enumeration used to select the oscillator to measure.
typedef enum {
	Pmc_MeasuredClk_RcOsc = 0, ///< Select the main RC oscillator.
	Pmc_MeasuredClk_XOsc = 1, ///< Select the main crystal oscillator.
} Pmc_MeasuredClk;

#if defined(N7S_TARGET_SAMV71Q21)
/// \brief Enumeration listing possible clock sources for Generic clock.
typedef enum {
	Pmc_GclkSrc_Slck = 0, ///< Slow clock
	Pmc_GclkSrc_Mainck = 1, ///< Main clock
	Pmc_GclkSrc_Pllack = 2, ///< Plla clock
	Pmc_GclkSrc_Upllck = 3, ///< USB Pll clock
	Pmc_GclkSrc_Masterck = 4, ///< Master clock
} Pmc_GclkSrc;
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
/// \brief Enumeration listing possible clock sources for Generic clock.
typedef enum {
	Pmc_GclkSrc_Slck = 0, ///< Slow clock
	Pmc_GclkSrc_Mainck = 1, ///< Main clock
	Pmc_GclkSrc_Pllack = 2, ///< Plla clock
	Pmc_GclkSrc_Pllbck = 3, ///< Pllb clock
	Pmc_GclkSrc_Masterck = 4, ///< Master clock
	Pmc_GclkSrc_Rc2ck = 5, ///< Rc2 clock
} Pmc_GclkSrc;

/// \brief Peripheral clocks configuration structure.
typedef struct {
	bool isPeripheralClkEnabled; ///< Enable the peripheral clock.
	bool isGclkEnabled; ///< Enable the generic clock for peripheral.
	Pmc_GclkSrc gclkSrc; ///< Generic clock source clock
	uint8_t gclkPresc; ///< Generic clock divider
} Pmc_PeripheralClkConfig;

#endif

/// \brief A programmable clock configuration descriptor.
typedef struct {
	bool isEnabled; ///< Flag indicating whether to enable the programmable clock.
	Pmc_PckSrc src; ///< Clock source selector.
	uint8_t presc; ///< Clock prescaler value.
} Pmc_PckConfig;

typedef struct {
	Pmc_MasterckSrc src; ///< Master clock source selector.
	Pmc_MasterckPresc presc; ///< Processor clock prescaler selector.
	Pmc_MasterckDiv divider; ///< MCK clock divider selector.
} Pmc_MasterckConfig;

typedef struct {
	Pmc_MainckSrc src; ///< Main clock source selector.
	Pmc_RcOscFreq rcOscFreq; ///< Main RC oscillator frequency selector.
	/// \brief Main crystal oscillator startup time in SCLK cycles.
	/// \details Must be multiple of 8. 2040 max value.
	uint16_t xoscStartupTime;
#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	bool detectClockFail; ///< Clock failure detection is enabled.
	bool xt32kMonitorEnabled; ///< External 32.768 kHz clock frequency monitoring is enabled.
	bool badCpuResetEnabled; ///< Clock failure triggers a system reset.
	bool badCpuNmicIrqEnabled; ///< Clock failure triggers NMIC interrupt.
#endif
} Pmc_MainckConfig;

typedef struct {
	uint8_t pllaMul; ///< PLLA clock frequency multiplier value. Set 0 to disable.
	uint8_t pllaDiv; ///< PLLA clock frequency divider value. Set 0 to disable.
	uint8_t pllaStartupTime; ///< PLLA clock startup time value.
#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	uint8_t pllaVco; ///< PLLA clock VCO frequency value.
	Pmc_FilterCapacitor
			pllaFilterCapacitor; ///< PLLA Filter Capacitance value
	Pmc_FilterResistor pllaFilterResistor; ///< PLLA Filter Resistance value
	Pmc_PllCurrent pllaCurrent; ///< PLLA output current

	Pmc_PllbSrc pllbSrc; ///< PLLB clock source clock selection value.
	uint8_t pllbMul; ///< PLLB clock frequency multiplier value. Set 0 to disable.
	uint8_t pllbDiv; ///< PLLB clock frequency divider value. Set 0 to disable.
	uint8_t pllbStartupTime; ///< PLLB clock startup time value.
	uint8_t pllbVco; ///< PLLB clock VCO frequency value.
	Pmc_FilterCapacitor
			pllbFilterCapacitor; ///< PLLB Filter Capacitance value
	Pmc_FilterResistor pllbFilterResistor; ///< PLLB Filter Resistance value
	Pmc_PllCurrent pllbCurrent; ///< PLLB output current
#endif
} Pmc_PllConfig;

/// \brief Power Management Controller configuration descriptor.
typedef struct {
	Pmc_MainckConfig mainck; ///< Main clock configuration structure.
	Pmc_PllConfig pll; ///< Pll configuration structure.
	Pmc_MasterckConfig masterck; ///< Master clock configuration structure.
	Pmc_PckConfig pck
			[Pmc_PckId_Count]; ///< Programmable clocks' configuration descriptors.
#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	Pmc_Rc2OscConfig
			rc2Osc; ///< Second RC oscillator configuration structure.
#endif
} Pmc_Config;

/// \brief Main clock frequency measurement descriptor.
typedef struct {
	Pmc_MeasuredClk measuredClk; ///< Oscillator to measure.
	uint32_t refFreq; ///< Reference frequency.
	uint32_t measuredFreq; ///< Measured frequency.
} Pmc_MainckMeasurement;

/// \brief Structure representing a PMC instance.
typedef struct {
	volatile Pmc_Registers
			*registers; ///< Pointer to PMC instance registers.
} Pmc;

/// \brief Returns PMC registers base address.
/// \returns Start address of PMC registers.
static inline Pmc_Registers *
Pmc_getDeviceRegisterStartAddress(void)
{
	// cppcheck-suppress misra-c2012-11.4
	return ((Pmc_Registers *)(PMC_BASE_ADDRESS));
}

/// \brief Function used to initialize PMC instance
/// \param [in,out] pmc PMC instance pointer
/// \param [in] pmcRegistersAddress Base address of PMC registers
void Pmc_init(Pmc *const pmc, Pmc_Registers *pmcRegistersAddress);

/// \brief Function used to set a Main clock configuration.
/// \param [in] pmc PMC instance pointer
/// \param [in] config Configuration structure
/// \param [in] timeout Timeout for busy-wait operations on registers
/// (actual duration depends on processor clock)
/// \param [out] errCode Possible error code in case of a failure (may be NULL).
/// \returns Whether the configuration was successfully set.
bool Pmc_setMainckConfig(Pmc *const pmc, const Pmc_MainckConfig *const config,
		const uint32_t timeout, ErrorCode *const errCode);

/// \brief Function used to read a Main clock configuration.
/// \param [in] pmc PMC instance pointer
/// \param [in] config Configuration structure
void Pmc_getMainckConfig(const Pmc *const pmc, Pmc_MainckConfig *const config);

/// \brief Function used to set a Pll clocks configuration.
/// \param [in] pmc PMC instance pointer
/// \param [in] config Configuration structure
/// \param [in] timeout Timeout for busy-wait operations on registers
/// \param [out] errCode Possible error code in case of a failure (may be NULL).
/// \returns Whether the configuration was successfully set.
bool Pmc_setPllConfig(Pmc *const pmc, const Pmc_PllConfig *const config,
		const uint32_t timeout, ErrorCode *const errCode);

/// \brief Function used to read a Pll clocks configuration.
/// \param [in] pmc PMC instance pointer
/// \param [in] config Configuration structure
void Pmc_getPllConfig(const Pmc *const pmc, Pmc_PllConfig *const config);

/// \brief Function used to set a Master clock configuration.
/// \param [in] pmc PMC instance pointer
/// \param [in] config Configuration structure
/// \param [in] timeout Timeout for busy-wait operations on registers
/// \param [out] errCode Possible error code in case of a failure (may be NULL).
/// \returns Whether the configuration was successfully set.
bool Pmc_setMasterckConfig(Pmc *const pmc,
		const Pmc_MasterckConfig *const config, const uint32_t timeout,
		ErrorCode *const errCode);

/// \brief Function used to read a Master clock configuration.
/// \param [in] pmc PMC instance pointer
/// \param [in] config Configuration structure
void Pmc_getMasterckConfig(
		const Pmc *const pmc, Pmc_MasterckConfig *const config);

/// \brief Function used to configure the PMC.
/// \param [in] pmc PMC instance pointer
/// \param [in] config PMC configuration descriptor.
/// \param [in] timeout Timeout for busy-wait operations on registers
/// \param [out] errCode Possible error code in case of a failure (may be NULL).
/// \returns Whether the configuration was successfully set.
bool Pmc_setConfig(Pmc *const pmc, const Pmc_Config *const config,
		const uint32_t timeout, ErrorCode *const errCode);

/// \brief Function used to retrieve current configuration of the PMC.
/// \param [in] pmc PMC instance pointer
/// \param [out] config Pointer to a configuration descriptor used to store current configuration.
void Pmc_getConfig(const Pmc *const pmc, Pmc_Config *const config);

/// \brief Function used to enable a peripheral clock.
/// \param [in] pmc PMC instance pointer
/// \param [in] peripheralId Identifier of the peripheral clock.
void Pmc_enablePeripheralClk(
		Pmc *const pmc, const Pmc_PeripheralId peripheralId);

/// \brief Function used to disable a peripheral clock.
/// \param [in] pmc PMC instance pointer
/// \param [in] peripheralId Identifier of the peripheral clock.
void Pmc_disablePeripheralClk(
		Pmc *const pmc, const Pmc_PeripheralId peripheralId);

/// \brief Function used to query the state of a peripheral clock.
/// \param [in] pmc PMC instance pointer
/// \param [in] peripheralId Identifier of the peripheral clock.
/// \returns Whether the clock is enabled.
bool Pmc_isPeripheralClkEnabled(
		const Pmc *const pmc, const Pmc_PeripheralId peripheralId);

/// \brief Function used to set a PCK clock configuration.
/// \param [in] pmc PMC instance pointer
/// \param [in] id Identifier of the PCK clock.
/// \param [in] config Configuration structure
/// \param [in] timeout Timeout for busy-wait operations on registers
/// \param [out] errCode Possible error code in case of a failure (may be NULL).
/// \returns Whether the configuration was successfully set.
bool Pmc_setPckConfig(Pmc *const pmc, const Pmc_PckId id,
		const Pmc_PckConfig *const config, const uint32_t timeout,
		ErrorCode *const errCode);

/// \brief Function used to read a PCK clock configuration.
/// \param [in] pmc PMC instance pointer
/// \param [in] id Identifier of the PCK clock.
/// \param [out] config Configuration structure
void Pmc_getPckConfig(const Pmc *const pmc, const Pmc_PckId id,
		Pmc_PckConfig *const config);

/// \brief Function used to measure the frequency of a particular oscillator in the system.
/// \param [in] pmc PMC instance pointer
/// \param [in,out] data Pointer to a measurement descriptor.
/// \param [in] timeout Timeout for busy-wait operations on registers
/// \param [out] errCode Possible error code in case of a failure (may be NULL).
/// \returns Whether the clock frequency was successfully measured
bool Pmc_measureMainck(Pmc *const pmc, Pmc_MainckMeasurement *data,
		const uint32_t timeout, ErrorCode *const errCode);

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)

/// \brief Function used to set a peripheral and generic clocks configuration.
/// \param [in] pmc PMC instance pointer
/// \param [in] peripheralId Identifier of the peripheral to enable the clocks for.
/// \param [in] config Configuration structure
void Pmc_setPeripheralClkConfig(Pmc *const pmc,
		const Pmc_PeripheralId peripheralId,
		const Pmc_PeripheralClkConfig *const config);

/// \brief Function used to read a peripheral and generic clocks configuration.
/// \param [in] pmc PMC instance pointer
/// \param [in] peripheralId Identifier of the peripheral to enable the clocks for.
/// \param [out] config Configuration structure
void Pmc_getPeripheralClkConfig(const Pmc *const pmc,
		const Pmc_PeripheralId peripheralId,
		Pmc_PeripheralClkConfig *const config);

/// \brief Function used to enable a generic clock.
/// \param [in] pmc PMC instance pointer
/// \param [in] peripheralId Identifier of the peripheral clock to enable the generic clock for.
/// \param [in] clkSrc Source fot the generic clock.
/// \param [in] prescaler Prescaler value for the generic clock.
void Pmc_enableGenericClk(Pmc *const pmc, const Pmc_PeripheralId peripheralId,
		const Pmc_PckSrc clkSrc, const uint32_t prescaler);

/// \brief Function used to disable a generic clock.
/// \param [in] pmc PMC instance pointer
/// \param [in] peripheralId Identifier of the peripheral clock to disable the generic clock for.
void Pmc_disableGenericClk(Pmc *const pmc, const Pmc_PeripheralId peripheralId);

/// \brief Function used to query the state of a peripheral generic clock.
/// \param [in] pmc PMC instance pointer
/// \param [in] peripheralId Identifier of the peripheral generic clock.
/// \returns Whether the clock is enabled.
bool Pmc_isGenericClockEnabled(
		const Pmc *const pmc, const Pmc_PeripheralId peripheralId);

/// \brief Function used to set a RC2 oscillator configuration.
/// \param [in] pmc PMC instance pointer
/// \param [in] config Configuration structure.
void Pmc_setRc2OscillatorConfig(
		Pmc *const pmc, const Pmc_Rc2OscConfig *const config);

/// \brief Function used to read a RC2 oscillator configuration.
/// \param [out] config Configuration structure.
/// \param [in] pmc PMC instance pointer
void Pmc_getRc2OscillatorConfig(
		const Pmc *const pmc, Pmc_Rc2OscConfig *const config);

/// \brief Function used to set the calibration settings of the main RC oscillator's frequency.
/// \param [in] pmc PMC instance pointer
/// \param [in] frequency Frequency of the oscillator.
/// \param [in] temperatureTrim Temperature trim value.
/// \param [in] frequencyTrim Frequency trim value.
void Pmc_setCalibrationConfigurationForMainRc(Pmc *const pmc,
		const Pmc_RcOscFreq frequency, const uint32_t temperatureTrim,
		const uint32_t frequencyTrim);

/// \brief Function used to set the calibration settings of the second RC oscillator's frequency.
/// \param [in] pmc PMC instance pointer
/// \param [in] frequency Frequency of the oscillator.
/// \param [in] temperatureTrim Temperature trim value.
/// \param [in] frequencyTrim Frequency trim value.
void Pmc_setCalibrationConfigurationForSecondRc(Pmc *const pmc,
		const Pmc_RcOscFreq frequency, const uint32_t temperatureTrim,
		const uint32_t frequencyTrim);

#endif

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // BSP_PMC_H
