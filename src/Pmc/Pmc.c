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

#include "Pmc.h"

#include <assert.h>
#include <stdbool.h>

#include <Utils/Bits.h>
#include <Utils/ErrorCode.h>
#include <Utils/Utils.h>

#include "PmcPeripheralId.h"
#include "PmcRegisters.h"

#define MEASUREMENT_ACCURACY 0.1

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define PLLB_CFG_REG_MASK 0x0000FFFFu
#define PLLA_CFG_REG_MASK 0xFFFF0000u
#endif

static bool
setMasterClockSource(Pmc *const pmc, const Pmc_MasterckSrc source,
		const uint32_t timeout, ErrorCode *const errCode)
{
	if ((source != Pmc_MasterckSrc_Slck)
			&& (source != Pmc_MasterckSrc_Mainck)
			&& (source != Pmc_MasterckSrc_Pllack))
		return returnError(errCode, Pmc_ErrorCode_InvalidClockSource);

	uint32_t mckr = pmc->registers->mckr;
	mckr &= ~PMC_MCKR_CSS_MASK;
	mckr |= source;

	pmc->registers->mckr = mckr;

	if (!waitForRegisterWithTimeout(
			    &(pmc->registers->sr), PMC_SR_MCKRDY_MASK, timeout))
		return returnError(
				errCode, Pmc_ErrorCode_MasterClkReadyTimeout);

	return true;
}

static bool
enableRC(Pmc *const pmc, const Pmc_RcOscFreq freq, const uint32_t timeout)
{
	uint32_t ckgrMor = pmc->registers->ckgrMor;

	// NOTE: RC frequency (MOSCRCF) can only be changed when MOSCRCS is set in PMC_SR
	// MOSCRCEN and MOSCRCF cannot be changed at the same time
	// Enable RC oscillator first, then set desired frequency
	ckgrMor &= ~CKGR_MOR_KEY_MASK;
	ckgrMor |= BIT_FIELD_VALUE(CKGR_MOR_KEY, CKGR_MOR_KEY_VALUE)
			| CKGR_MOR_MOSCRCEN_MASK;
	pmc->registers->ckgrMor = ckgrMor;

	if (!waitForRegisterWithTimeout(&(pmc->registers->sr),
			    PMC_SR_MOSCRCS_MASK, timeout))
		return false;

	ckgrMor &= ~CKGR_MOR_MOSCRCF_MASK;
	ckgrMor |= BIT_FIELD_VALUE(CKGR_MOR_MOSCRCF, freq);
	pmc->registers->ckgrMor = ckgrMor;

	return waitForRegisterWithTimeout(
			&(pmc->registers->sr), PMC_SR_MOSCRCS_MASK, timeout);
}

static bool
switchMainClockToRC(Pmc *const pmc, const uint32_t timeout,
		ErrorCode *const errCode)
{
	uint32_t ckgrMor = pmc->registers->ckgrMor;
	ckgrMor &= ~CKGR_MOR_KEY_MASK;
	ckgrMor |= BIT_FIELD_VALUE(CKGR_MOR_KEY, CKGR_MOR_KEY_VALUE);
	ckgrMor &= ~CKGR_MOR_MOSCSEL_MASK;

	pmc->registers->ckgrMor = ckgrMor;

	if (!waitForRegisterWithTimeout(&(pmc->registers->sr),
			    PMC_SR_MOSCSELS_MASK, timeout))
		return returnError(errCode, Pmc_ErrorCode_MainClkSelectTimeout);

	if (!waitForRegisterWithTimeout(
			    &pmc->registers->sr, PMC_SR_MCKRDY_MASK, timeout))
		return returnError(
				errCode, Pmc_ErrorCode_MasterClkReadyTimeout);

	return true;
}

static bool
setMasterClockPrescalerAndDivider(Pmc *const pmc, const Pmc_MasterckPresc presc,
		const Pmc_MasterckDiv divider, const uint32_t timeout,
		ErrorCode *const errCode)
{
	if ((divider != Pmc_MasterckDiv_1) && (divider != Pmc_MasterckDiv_2))
		return returnError(errCode, Pmc_ErrorCode_InvalidMasterckDiv);

	uint32_t mckr = pmc->registers->mckr;
	mckr &= ~PMC_MCKR_PRES_MASK;
	mckr |= BIT_FIELD_VALUE(PMC_MCKR_PRES, presc);

	pmc->registers->mckr = mckr;

	if (!waitForRegisterWithTimeout(
			    &(pmc->registers->sr), PMC_SR_MCKRDY_MASK, timeout))
		return returnError(
				errCode, Pmc_ErrorCode_MasterClkReadyTimeout);

	mckr &= ~PMC_MCKR_MDIV_MASK;
	mckr |= BIT_FIELD_VALUE(PMC_MCKR_MDIV, divider);

	pmc->registers->mckr = mckr;

	if (!waitForRegisterWithTimeout(
			    &(pmc->registers->sr), PMC_SR_MCKRDY_MASK, timeout))
		return returnError(
				errCode, Pmc_ErrorCode_MasterClkReadyTimeout);

	return true;
}

static bool
resetMainClockGenerator(Pmc *const pmc, const uint32_t timeout)
{
	pmc->registers->ckgrMor =
			BIT_FIELD_VALUE(CKGR_MOR_KEY, CKGR_MOR_KEY_VALUE)
			| CKGR_MOR_MOSCRCEN_MASK;

	return waitForRegisterWithTimeout(
			&(pmc->registers->sr), PMC_SR_MOSCRCS_MASK, timeout);
}

static void
disablePll(Pmc *const pmc)
{
	pmc->registers->ckgrPllar = CKGR_PLLAR_ONE_MASK;

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	pmc->registers->ckgrPllbr = 0u;
#endif
}

static bool
resetMainAndMasterClockConfiguration(
		Pmc *const pmc, const uint32_t timeout, ErrorCode *errCode)
{
#if defined(N7S_TARGET_SAMV71Q21) || defined(N7S_TARGET_SAMRH707F18)
	const Pmc_RcOscFreq defaultRCFrequency = Pmc_RcOscFreq_4M;
#elif defined(N7S_TARGET_SAMRH71F20)
	const Pmc_RcOscFreq defaultRCFrequency = Pmc_RcOscFreq_10M;
#endif
	if (!enableRC(pmc, defaultRCFrequency, timeout))
		return returnError(
				errCode, Pmc_ErrorCode_RCStabilizationTimeout);

	if (!switchMainClockToRC(pmc, timeout, errCode))
		return false;

	if (!setMasterClockSource(
			    pmc, Pmc_MasterckSrc_Mainck, timeout, errCode))
		return false;

	disablePll(pmc);

	if (!resetMainClockGenerator(pmc, timeout))
		return returnError(
				errCode, Pmc_ErrorCode_RCStabilizationTimeout);

	if (!setMasterClockPrescalerAndDivider(pmc, Pmc_MasterckPresc_1,
			    Pmc_MasterckDiv_1, timeout, errCode))
		return returnError(
				errCode, Pmc_ErrorCode_MasterClkReadyTimeout);

	return true;
}

static void
disableXtal(Pmc *const pmc)
{
	uint32_t ckgrMor = pmc->registers->ckgrMor;
	ckgrMor &= ~CKGR_MOR_KEY_MASK;
	ckgrMor |= BIT_FIELD_VALUE(CKGR_MOR_KEY, CKGR_MOR_KEY_VALUE);
	ckgrMor &= ~(uint32_t)CKGR_MOR_MOSCXTEN_MASK;

	pmc->registers->ckgrMor = ckgrMor;
}

static bool
enableXtal(Pmc *const pmc, const uint16_t startupTime, const uint32_t timeout)
{
	uint32_t ckgrMor = pmc->registers->ckgrMor;
	ckgrMor &= ~(CKGR_MOR_KEY_MASK | CKGR_MOR_MOSCXTST_MASK);
	ckgrMor |= BIT_FIELD_VALUE(CKGR_MOR_KEY, CKGR_MOR_KEY_VALUE)
			| CKGR_MOR_MOSCXTEN_MASK;
	ckgrMor |= BIT_FIELD_VALUE(CKGR_MOR_MOSCXTST, startupTime >> 3u);

	pmc->registers->ckgrMor = ckgrMor;

	return waitForRegisterWithTimeout(
			&(pmc->registers->sr), PMC_SR_MOSCXTS_MASK, timeout);
}

static bool
switchMainClockToXtal(Pmc *const pmc, const uint32_t timeout)
{
	uint32_t ckgrMor = pmc->registers->ckgrMor;
	ckgrMor &= ~CKGR_MOR_KEY_MASK;
	ckgrMor |= BIT_FIELD_VALUE(CKGR_MOR_KEY, CKGR_MOR_KEY_VALUE)
			| CKGR_MOR_MOSCSEL_MASK;

	pmc->registers->ckgrMor = ckgrMor;

	return waitForRegisterWithTimeout(
			&(pmc->registers->sr), PMC_SR_MOSCSELS_MASK, timeout);
}

static bool
disableRC(Pmc *const pmc, const uint32_t timeout)
{
	uint32_t ckgrMor = pmc->registers->ckgrMor;

	// There is no need to disable RC if it is already disabled
	if ((ckgrMor & CKGR_MOR_MOSCRCEN_MASK) == 0u)
		return true;

	// NOTE: RC frequency (MOSCRCF) can only be changed when MOSCRCS is set in PMC_SR
	// MOSCRCEN and MOSCRCF cannot be changed at the same time
	// Reset frequency to default value first and then disable RC.
	ckgrMor &= ~CKGR_MOR_KEY_MASK;
	ckgrMor |= BIT_FIELD_VALUE(CKGR_MOR_KEY, CKGR_MOR_KEY_VALUE);
	ckgrMor &= ~CKGR_MOR_MOSCRCF_MASK;
	pmc->registers->ckgrMor = ckgrMor;

	if (!waitForRegisterWithTimeout(&(pmc->registers->sr),
			    PMC_SR_MOSCRCS_MASK, timeout))
		return false;

	ckgrMor &= ~CKGR_MOR_MOSCRCEN_MASK;
	pmc->registers->ckgrMor = ckgrMor;

	// Do not wait for MOSCRCS in SR here, it will not be set if RC is disabled
	return true;
}

static bool
configureMainckRcOsc(Pmc *pmc, const Pmc_MainckConfig *const config,
		const uint32_t timeout, ErrorCode *errCode)
{
	if (!enableRC(pmc, config->rcOscFreq, timeout))
		return returnError(
				errCode, Pmc_ErrorCode_RCStabilizationTimeout);

	if (!switchMainClockToRC(pmc, timeout, errCode))
		return false;

	disableXtal(pmc);

	return true;
}

static inline bool
preconfigureXtal(Pmc *pmc, const Pmc_MainckConfig *const config,
		const uint32_t timeout, ErrorCode *const errCode)
{
	if (!enableXtal(pmc, config->xoscStartupTime, timeout))
		return returnError(errCode,
				Pmc_ErrorCode_XtalStabilizationTimeout);

	if (!waitForRegisterWithTimeout(
			    &(pmc->registers->sr), PMC_SR_MCKRDY_MASK, timeout))
		return returnError(
				errCode, Pmc_ErrorCode_MasterClkReadyTimeout);

	if (!switchMainClockToXtal(pmc, timeout))
		return returnError(errCode, Pmc_ErrorCode_MainClkSelectTimeout);

	if (!waitForRegisterWithTimeout(
			    &(pmc->registers->sr), PMC_SR_MCKRDY_MASK, timeout))
		return returnError(
				errCode, Pmc_ErrorCode_MasterClkReadyTimeout);

	return true;
}

static bool
trySwitchingToTC(Pmc *pmc, const uint32_t timeout, ErrorCode *const errCode)
{
	// Main crystal has wrong frequency, fall back to internal RC oscillator
	if (!switchMainClockToRC(pmc, timeout, errCode))
		return false;

	return returnError(errCode, Pmc_ErrorCode_MeasuredFrequencyIsIncorrect);
}

static bool
configureMainckXOsc(Pmc *pmc, const Pmc_MainckConfig *const config,
		const uint32_t timeout, ErrorCode *const errCode)
{
	if (!preconfigureXtal(pmc, config, timeout, errCode))
		return false;

	Pmc_MainckMeasurement measurement;

	measurement.measuredClk = Pmc_MeasuredClk_XOsc;
	measurement.refFreq = PMC_SLOW_CLOCK_FREQ;
	measurement.measuredFreq = 0;
	if (!Pmc_measureMainck(pmc, &measurement, timeout, errCode))
		return false;

	if (!isBetweenUint32(measurement.measuredFreq,
			    (uint32_t)(PMC_MAIN_CRYSTAL_FREQ
					    * (1.0 - MEASUREMENT_ACCURACY)),
			    (uint32_t)(PMC_MAIN_CRYSTAL_FREQ
					    * (1.0 + MEASUREMENT_ACCURACY)))) {
		return trySwitchingToTC(pmc, timeout, errCode);
	}

	if (!disableRC(pmc, timeout))
		return returnError(
				errCode, Pmc_ErrorCode_RCStabilizationTimeout);

	if (!waitForRegisterWithTimeout(
			    &(pmc->registers->sr), PMC_SR_MCKRDY_MASK, timeout))
		return returnError(
				errCode, Pmc_ErrorCode_MasterClkReadyTimeout);

	return true;
}

#if defined(N7S_TARGET_SAMV71Q21) || defined(N7S_TARGET_SAMRH71F20)
static bool
configureMainckBypass(Pmc *pmc, const uint32_t timeout, ErrorCode *errCode)
{
	uint32_t ckgrMor = pmc->registers->ckgrMor;
	// Set the access key.
	ckgrMor &= ~(uint32_t)CKGR_MOR_KEY_MASK;
	ckgrMor |= CKGR_MOR_KEY_VALUE << CKGR_MOR_KEY_OFFSET;

	// Switch to external clock; crystal oscillator must be disabled.
	ckgrMor &= ~(uint32_t)CKGR_MOR_MOSCXTEN_MASK;
	ckgrMor |= CKGR_MOR_MOSCXTBY_MASK;
	pmc->registers->ckgrMor = ckgrMor;

	// Wait for completion.
	if (!waitForRegisterWithTimeout(
			    &(pmc->registers->sr), PMC_SR_MCKRDY_MASK, timeout))
		return returnError(
				errCode, Pmc_ErrorCode_MasterClkReadyTimeout);

	if (!switchMainClockToXtal(pmc, timeout))
		return returnError(errCode, Pmc_ErrorCode_MainClkSelectTimeout);

	if (!waitForRegisterWithTimeout(
			    &(pmc->registers->sr), PMC_SR_MCKRDY_MASK, timeout))
		return returnError(
				errCode, Pmc_ErrorCode_MasterClkReadyTimeout);

	if (!disableRC(pmc, timeout))
		return returnError(
				errCode, Pmc_ErrorCode_RCStabilizationTimeout);

	if (!waitForRegisterWithTimeout(
			    &(pmc->registers->sr), PMC_SR_MCKRDY_MASK, timeout))
		return returnError(
				errCode, Pmc_ErrorCode_MasterClkReadyTimeout);

	return true;
}
#endif

static bool
configurePlla(Pmc *pmc, const Pmc_PllConfig *const config,
		const uint32_t timeout, ErrorCode *errCode)
{
	if ((config->pllaDiv == 0u) || (config->pllaMul == 0u)) {
		// 0 in MULA or DIVA fields disables PLLA
		pmc->registers->ckgrPllar = CKGR_PLLAR_ONE_MASK;
		return true;
	}

	uint32_t pllar = CKGR_PLLAR_ONE_MASK
			| BIT_FIELD_VALUE(CKGR_PLLAR_MULA, config->pllaMul)
			| BIT_FIELD_VALUE(CKGR_PLLAR_PLLACOUNT,
					config->pllaStartupTime)
			| BIT_FIELD_VALUE(CKGR_PLLAR_DIVA, config->pllaDiv);
#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	pllar |= BIT_FIELD_VALUE(CKGR_PLLAR_FREQVCO, config->pllaVco);
#endif
	pmc->registers->ckgrPllar = pllar;

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	uint32_t pllCfg = pmc->registers->pllCfg & PLLA_CFG_REG_MASK;
	pllCfg |= BIT_FIELD_VALUE(PMC_PLLCFG_SCA, config->pllaFilterCapacitor);
	pllCfg |= BIT_FIELD_VALUE(PMC_PLLCFG_SRA, config->pllaFilterResistor);
	pllCfg |= BIT_FIELD_VALUE(PMC_PLLCFG_OUTCUR_PLLA, config->pllaCurrent);
	pmc->registers->pllCfg = pllCfg;
#endif

	if (!waitForRegisterWithTimeout(
			    &(pmc->registers->sr), PMC_SR_LOCKA_MASK, timeout))
		return returnError(errCode, Pmc_ErrorCode_PllaNotLocked);

	return true;
}

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
static bool
configurePllb(Pmc *pmc, const Pmc_PllConfig *const config,
		const uint32_t timeout, ErrorCode *const errCode)
{
	if ((config->pllbDiv == 0u) || (config->pllbMul == 0u)) {
		// 0 in MULB or DIVB fields disables PLLB
		pmc->registers->ckgrPllbr = 0u;
		return true;
	}

	pmc->registers->ckgrPllbr =
			BIT_FIELD_VALUE(CKGR_PLLBR_SRCB, config->pllbSrc)
			| BIT_FIELD_VALUE(CKGR_PLLBR_MULB, config->pllbMul)
			| BIT_FIELD_VALUE(CKGR_PLLBR_PLLBCOUNT,
					config->pllbStartupTime)
			| BIT_FIELD_VALUE(CKGR_PLLBR_DIVB, config->pllbDiv)
			| BIT_FIELD_VALUE(CKGR_PLLBR_FREQVCO, config->pllbVco);

	uint32_t pllCfg = pmc->registers->pllCfg & PLLB_CFG_REG_MASK;
	pllCfg |= BIT_FIELD_VALUE(PMC_PLLCFG_SCB, config->pllbFilterCapacitor);
	pllCfg |= BIT_FIELD_VALUE(PMC_PLLCFG_SRB, config->pllbFilterResistor);
	pllCfg |= BIT_FIELD_VALUE(PMC_PLLCFG_OUTCUR_PLLB, config->pllbCurrent);
	pmc->registers->pllCfg = pllCfg;

	if (!waitForRegisterWithTimeout(
			    &(pmc->registers->sr), PMC_SR_LOCKB_MASK, timeout))
		return returnError(errCode, Pmc_ErrorCode_PllbNotLocked);

	return true;
}
#endif

void
Pmc_init(Pmc *const pmc, Pmc_Registers *pmcRegistersAddress)
{
	pmc->registers = pmcRegistersAddress;
}

bool
Pmc_setMainckConfig(Pmc *pmc, const Pmc_MainckConfig *const config,
		const uint32_t timeout, ErrorCode *const errCode)
{
	switch (config->src) {
	case Pmc_MainckSrc_RcOsc:
		if (!configureMainckRcOsc(pmc, config, timeout, errCode))
			return false;
		break;
	case Pmc_MainckSrc_XOsc:
		if (!configureMainckXOsc(pmc, config, timeout, errCode))
			return false;
		break;
#if defined(N7S_TARGET_SAMV71Q21) || defined(N7S_TARGET_SAMRH71F20)
	case Pmc_MainckSrc_XOscBypassed:
		if (!configureMainckBypass(pmc, timeout, errCode))
			return false;
		break;
#endif
	default: return returnError(errCode, Pmc_ErrorCode_InvalidClockSource);
	}

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	uint32_t ckgrMor = pmc->registers->ckgrMor;

	ckgrMor &= ~CKGR_MOR_KEY_MASK;
	ckgrMor |= BIT_FIELD_VALUE(CKGR_MOR_KEY, CKGR_MOR_KEY_VALUE);
	ckgrMor &= ~(CKGR_MOR_CFDEN_MASK | CKGR_MOR_XT32KFME_MASK
			| CKGR_MOR_BCPURST_MASK | CKGR_MOR_BCPUNMIC_MASK);
	ckgrMor |= BIT_VALUE(CKGR_MOR_CFDEN, config->detectClockFail)
			| BIT_VALUE(CKGR_MOR_XT32KFME,
					config->xt32kMonitorEnabled)
			| BIT_VALUE(CKGR_MOR_BCPURST,
					config->badCpuResetEnabled)
			| BIT_VALUE(CKGR_MOR_BCPUNMIC,
					config->badCpuNmicIrqEnabled);

	pmc->registers->ckgrMor = ckgrMor;
#endif

	return true;
}

void
Pmc_getMainckConfig(const Pmc *const pmc, Pmc_MainckConfig *const config)
{
	const uint32_t ckgrMor = pmc->registers->ckgrMor;
	config->rcOscFreq = (ckgrMor & CKGR_MOR_MOSCRCF_MASK)
			>> CKGR_MOR_MOSCRCF_OFFSET;

	// Subtracking 3u from offset value got register value is multiplied by 8
	const uint32_t multiplierOffsetValue = 3u;

	config->xoscStartupTime = ((ckgrMor & CKGR_MOR_MOSCXTST_MASK)
			>> (CKGR_MOR_MOSCXTST_OFFSET - multiplierOffsetValue));

	if ((ckgrMor & CKGR_MOR_MOSCSEL_MASK) != 0u) {
		// Main crystal oscillator.
#if defined(N7S_TARGET_SAMV71Q21) || defined(N7S_TARGET_SAMRH71F20)
		if ((ckgrMor & CKGR_MOR_MOSCXTBY_MASK) != 0u)
			config->src = Pmc_MainckSrc_XOscBypassed;
		else
			config->src = Pmc_MainckSrc_XOsc;
#elif defined(N7S_TARGET_SAMRH707F18)
		config->src = Pmc_MainckSrc_XOsc;
#endif
	} else
		// Main RC oscillator.
		config->src = Pmc_MainckSrc_RcOsc;

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	config->detectClockFail = ((ckgrMor & CKGR_MOR_CFDEN_MASK) != 0u);
	config->xt32kMonitorEnabled =
			((ckgrMor & CKGR_MOR_XT32KFME_MASK) != 0u);
	config->badCpuResetEnabled = ((ckgrMor & CKGR_MOR_BCPURST_MASK) != 0u);
	config->badCpuNmicIrqEnabled =
			((ckgrMor & CKGR_MOR_BCPUNMIC_MASK) != 0u);
#endif
}

bool
Pmc_setPllConfig(Pmc *pmc, const Pmc_PllConfig *const config,
		const uint32_t timeout, ErrorCode *const errCode)
{
	assert(config != NULL);
	if (!configurePlla(pmc, config, timeout, errCode))
		return false;

#if defined(N7S_TARGET_SAMV71Q21)
	return true;
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	return configurePllb(pmc, config, timeout, errCode);
#endif
}

void
Pmc_getPllConfig(const Pmc *const pmc, Pmc_PllConfig *const config)
{
	const uint32_t ckgrPllar = pmc->registers->ckgrPllar;
	config->pllaMul = (uint8_t)((ckgrPllar & CKGR_PLLAR_MULA_MASK)
			>> CKGR_PLLAR_MULA_OFFSET);
	config->pllaDiv = (ckgrPllar & CKGR_PLLAR_DIVA_MASK)
			>> CKGR_PLLAR_DIVA_OFFSET;
	config->pllaStartupTime = (ckgrPllar & CKGR_PLLAR_PLLACOUNT_MASK)
			>> CKGR_PLLAR_PLLACOUNT_OFFSET;

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	const uint32_t pllCfg = pmc->registers->pllCfg;

	config->pllaVco = (ckgrPllar & CKGR_PLLAR_FREQVCO_MASK)
			>> CKGR_PLLAR_FREQVCO_OFFSET;
	config->pllaCurrent =
			(Pmc_PllCurrent)((pllCfg & PMC_PLLCFG_OUTCUR_PLLA_MASK)
					>> PMC_PLLCFG_OUTCUR_PLLA_OFFSET);
	config->pllaFilterCapacitor =
			(Pmc_FilterCapacitor)((pllCfg & PMC_PLLCFG_SCA_MASK)
					>> PMC_PLLCFG_SCA_OFFSET);
	config->pllaFilterResistor =
			(Pmc_FilterResistor)((pllCfg & PMC_PLLCFG_SRA_MASK)
					>> PMC_PLLCFG_SRA_OFFSET);

	const uint32_t ckgrPllbr = pmc->registers->ckgrPllbr;
	config->pllbSrc = (Pmc_PllbSrc)((ckgrPllbr & CKGR_PLLBR_SRCB_MASK)
			>> CKGR_PLLBR_SRCB_OFFSET);
	config->pllbMul = (uint8_t)((ckgrPllbr & CKGR_PLLBR_MULB_MASK)
			>> CKGR_PLLBR_MULB_OFFSET);
	config->pllbDiv = (ckgrPllbr & CKGR_PLLBR_DIVB_MASK)
			>> CKGR_PLLBR_DIVB_OFFSET;
	config->pllbStartupTime = (ckgrPllbr & CKGR_PLLBR_PLLBCOUNT_MASK)
			>> CKGR_PLLBR_PLLBCOUNT_OFFSET;
	config->pllbVco = (ckgrPllbr & CKGR_PLLBR_FREQVCO_MASK)
			>> CKGR_PLLBR_FREQVCO_OFFSET;
	config->pllbCurrent =
			(Pmc_PllCurrent)((pllCfg & PMC_PLLCFG_OUTCUR_PLLB_MASK)
					>> PMC_PLLCFG_OUTCUR_PLLB_OFFSET);
	config->pllbFilterCapacitor =
			(Pmc_FilterCapacitor)((pllCfg & PMC_PLLCFG_SCB_MASK)
					>> PMC_PLLCFG_SCB_OFFSET);
	config->pllbFilterResistor =
			(Pmc_FilterResistor)((pllCfg & PMC_PLLCFG_SRB_MASK)
					>> PMC_PLLCFG_SRB_OFFSET);
#endif
}

bool
Pmc_setMasterckConfig(Pmc *const pmc, const Pmc_MasterckConfig *const config,
		const uint32_t timeout, ErrorCode *const errCode)
{
	if ((config->src != Pmc_MasterckSrc_Pllack)
			&& (!setMasterClockSource(
					pmc, config->src, timeout, errCode)))
		return false;

	if (!setMasterClockPrescalerAndDivider(pmc, config->presc,
			    config->divider, timeout, errCode))
		return false;

	if ((config->src == Pmc_MasterckSrc_Pllack)
			&& (!setMasterClockSource(
					pmc, config->src, timeout, errCode)))
		return false;

	return true;
}

void
Pmc_getMasterckConfig(const Pmc *const pmc, Pmc_MasterckConfig *const config)
{
	const uint32_t mckr = pmc->registers->mckr;
	config->src = (mckr & PMC_MCKR_CSS_MASK) >> PMC_MCKR_CSS_OFFSET;
	config->presc = (mckr & PMC_MCKR_PRES_MASK) >> PMC_MCKR_PRES_OFFSET;
	config->divider = (mckr & PMC_MCKR_MDIV_MASK) >> PMC_MCKR_MDIV_OFFSET;
}

bool
Pmc_setConfig(Pmc *const pmc, const Pmc_Config *const config,
		const uint32_t timeout, ErrorCode *const errCode)
{
	if (!resetMainAndMasterClockConfiguration(pmc, timeout, errCode))
		return false;

	if (!Pmc_setMainckConfig(pmc, &config->mainck, timeout, errCode))
		return false;

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	Pmc_setRc2OscillatorConfig(pmc, &config->rc2Osc);
#endif

	if (!Pmc_setPllConfig(pmc, &config->pll, timeout, errCode))
		return false;

	if (!Pmc_setMasterckConfig(pmc, &config->masterck, timeout, errCode))
		return false;

	for (uint32_t i = 0; i < (uint32_t)Pmc_PckId_Count; i++) {
		if (!Pmc_setPckConfig(pmc, (Pmc_PckId)i, &config->pck[i],
				    timeout, errCode))
			return false;
	}

	return true;
}

void
Pmc_getConfig(const Pmc *const pmc, Pmc_Config *const config)
{
	Pmc_getMainckConfig(pmc, &config->mainck);
	Pmc_getPllConfig(pmc, &config->pll);
	Pmc_getMasterckConfig(pmc, &config->masterck);
#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	Pmc_getRc2OscillatorConfig(pmc, &config->rc2Osc);
#endif
	for (uint32_t i = 0u; i < (uint32_t)Pmc_PckId_Count; ++i) {
		Pmc_getPckConfig(pmc, (Pmc_PckId)i, &config->pck[i]);
	}
}

void
Pmc_enablePeripheralClk(Pmc *const pmc, const Pmc_PeripheralId peripheralId)
{
	pmc->registers->pcr = BIT_FIELD_VALUE(PMC_PCR_PID, peripheralId);

	const uint32_t pcr = pmc->registers->pcr | PMC_PCR_CMD_WRITE
			| BIT_VALUE(PMC_PCR_EN, true);

	pmc->registers->pcr = pcr;
}

void
Pmc_disablePeripheralClk(Pmc *const pmc, const Pmc_PeripheralId peripheralId)
{
	pmc->registers->pcr = BIT_FIELD_VALUE(PMC_PCR_PID, peripheralId);

	const uint32_t pcr = pmc->registers->pcr | PMC_PCR_CMD_WRITE;

	pmc->registers->pcr = (pcr & (~PMC_PCR_EN_MASK));
}

bool
Pmc_isPeripheralClkEnabled(
		const Pmc *const pmc, const Pmc_PeripheralId peripheralId)
{
	const uint32_t numberOfBitsInRegister = 32u;

#if defined(N7S_TARGET_SAMV71Q21)
	assert(((uint32_t)peripheralId < 64u) && "Invalid PeripheralId");
	if ((uint32_t)peripheralId < numberOfBitsInRegister)
		return ((pmc->registers->pcsr0
					& (UINT32_C(1) << (uint32_t)peripheralId))
				!= 0u);

	return ((pmc->registers->pcsr1
				& (1u << ((uint32_t)peripheralId
						   - numberOfBitsInRegister)))
			!= 0u);
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	assert(((uint32_t)peripheralId < 128u) && "Invalid PeripheralId");
	return ((pmc->registers->csr[(uint32_t)peripheralId
				 / numberOfBitsInRegister]
				& (1u << ((uint32_t)peripheralId
						   % numberOfBitsInRegister)))
			!= 0u);
#else
#error "No target platform specified (missing N7S_TARGET_* macro)"
#endif
}

bool
Pmc_setPckConfig(Pmc *const pmc, const Pmc_PckId id,
		const Pmc_PckConfig *const config, const uint32_t timeout,
		ErrorCode *const errCode)
{
	pmc->registers->scdr = 1u << (PMC_SCDR_PCK0_OFFSET + (uint32_t)id);
	pmc->registers->pck[id] = BIT_FIELD_VALUE(PMC_PCK_CSS, config->src)
			| BIT_FIELD_VALUE(PMC_PCK_PRES, config->presc);
	if (config->isEnabled) {
		pmc->registers->scer = 1u
				<< (PMC_SCER_PCK0_OFFSET + (uint32_t)id);
		if (!waitForRegisterWithTimeout(&(pmc->registers->sr),
				    (1u << (PMC_SR_PCKRDY0_OFFSET
						     + (uint32_t)id)),
				    timeout))
			return returnError(
					errCode, Pmc_ErrorCode_PckReadyTimeout);
	}

	return true;
}

void
Pmc_getPckConfig(const Pmc *const pmc, const Pmc_PckId id,
		Pmc_PckConfig *const config)
{
	const uint32_t scsr = pmc->registers->scsr;
	const uint32_t pck = pmc->registers->pck[id];
	config->isEnabled =
			(scsr & (1u << (PMC_SCSR_PCK0_OFFSET + (uint32_t)id)))
			!= 0u;
	config->src = (pck & PMC_PCK_CSS_MASK) >> PMC_PCK_CSS_OFFSET;
	config->presc = (pck & PMC_PCK_PRES_MASK) >> PMC_PCK_PRES_OFFSET;
}

bool
Pmc_measureMainck(Pmc *const pmc, Pmc_MainckMeasurement *data,
		const uint32_t timeout, ErrorCode *const errCode)
{
	pmc->registers->ckgrMcfr =
			BIT_FIELD_VALUE(CKGR_MCFR_CCSS, data->measuredClk);
	pmc->registers->ckgrMcfr |= BIT_VALUE(CKGR_MCFR_RCMEAS, true);

	if (!waitForRegisterWithTimeout(&(pmc->registers->ckgrMcfr),
			    CKGR_MCFR_MAINFRDY_MASK, timeout))
		return returnError(errCode,
				Pmc_ErrorCode_FrequencyMeasurementTimeout);

	// According to DS60001593G page 320 MAINF field gives number of cycles of the meaured clock
	// within 16 MD_SCLK periods, so f_measured = (MAINF * f_MD_SLCK) >> 4.
	const uint32_t mainfShift = 4u;
	data->measuredFreq = (((pmc->registers->ckgrMcfr & CKGR_MCFR_MAINF_MASK)
					      >> CKGR_MCFR_MAINF_OFFSET)
					     * data->refFreq)
			>> mainfShift;

	return true;
}

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)

void
Pmc_setPeripheralClkConfig(Pmc *const pmc, const Pmc_PeripheralId peripheralId,
		const Pmc_PeripheralClkConfig *const config)
{
	pmc->registers->pcr = BIT_FIELD_VALUE(PMC_PCR_PID, peripheralId);
	(void)pmc->registers->pcr;

	uint32_t pcr = pmc->registers->pcr;
	pcr |= PMC_PCR_CMD_WRITE;
	pcr &= ~(PMC_PCR_GCLKEN_MASK | PMC_PCR_EN_MASK);
	pmc->registers->pcr = pcr;
	(void)pmc->registers->pcr;

	pmc->registers->pcr = BIT_FIELD_VALUE(PMC_PCR_PID, peripheralId);
	pcr = pmc->registers->pcr;
	pcr |= PMC_PCR_CMD_WRITE;
	pcr &= ~(PMC_PCR_GCLKCSS_MASK | PMC_PCR_GCLKDIV_MASK);
	pcr |= BIT_FIELD_VALUE(PMC_PCR_GCLKCSS, config->gclkSrc);
	pcr |= BIT_FIELD_VALUE(PMC_PCR_GCLKDIV, config->gclkPresc);
	pcr |= BIT_VALUE(PMC_PCR_GCLKEN, config->isGclkEnabled);
	pcr |= BIT_VALUE(PMC_PCR_EN, config->isPeripheralClkEnabled);
	pmc->registers->pcr = pcr;
}

void
Pmc_getPeripheralClkConfig(const Pmc *const pmc,
		const Pmc_PeripheralId peripheralId,
		Pmc_PeripheralClkConfig *const config)
{
	pmc->registers->pcr = BIT_FIELD_VALUE(PMC_PCR_PID, peripheralId);
	const uint32_t pcr = pmc->registers->pcr;
	config->isGclkEnabled = (pcr & PMC_PCR_GCLKEN_MASK) != 0u;
	config->isPeripheralClkEnabled = (pcr & PMC_PCR_EN_MASK) != 0u;
	config->gclkPresc =
			(pcr & PMC_PCR_GCLKDIV_MASK) >> PMC_PCR_GCLKDIV_OFFSET;
	config->gclkSrc = (Pmc_GclkSrc)((pcr & PMC_PCR_GCLKCSS_MASK)
			>> PMC_PCR_GCLKCSS_OFFSET);
}

void
Pmc_enableGenericClk(Pmc *const pmc, const Pmc_PeripheralId peripheralId,
		const Pmc_PckSrc clkSrc, const uint32_t prescaler)
{
	pmc->registers->pcr = BIT_FIELD_VALUE(PMC_PCR_PID, peripheralId);

	uint32_t pcr = pmc->registers->pcr | PMC_PCR_CMD_WRITE;

	pcr &= ~(PMC_PCR_GCLKCSS_MASK | PMC_PCR_GCLKDIV_MASK);
	pcr |= BIT_FIELD_VALUE(PMC_PCR_GCLKCSS, clkSrc)
			| BIT_FIELD_VALUE(PMC_PCR_GCLKDIV, prescaler)
			| PMC_PCR_GCLKEN_MASK;

	pmc->registers->pcr = pcr;
}

void
Pmc_disableGenericClk(Pmc *const pmc, const Pmc_PeripheralId peripheralId)
{
	pmc->registers->pcr = BIT_FIELD_VALUE(PMC_PCR_PID, peripheralId);

	uint32_t pcr = pmc->registers->pcr | PMC_PCR_CMD_WRITE;
	pcr &= ~PMC_PCR_GCLKEN_MASK;

	pmc->registers->pcr = pcr;
}

bool
Pmc_isGenericClockEnabled(
		const Pmc *const pmc, const Pmc_PeripheralId peripheralId)
{
	const uint32_t numberOfBitsInRegister = 32u;

	return ((pmc->registers->gcsr[(uint32_t)peripheralId
				 / numberOfBitsInRegister]
				& (1u << ((uint32_t)peripheralId
						   % numberOfBitsInRegister)))
			!= 0u);
}

void
Pmc_setRc2OscillatorConfig(Pmc *const pmc, const Pmc_Rc2OscConfig *const config)
{
	assert(config != NULL);

	uint32_t osc2 = BIT_FIELD_VALUE(PMC_OSC2_KEY, PMC_OSC2_KEY_VALUE)
			| BIT_FIELD_VALUE(PMC_OSC2_OSCRCF, config->frequency)
			| PMC_OSC2_EN_WR_CALIB_MASK | PMC_OSC2_EN_MASK;
	pmc->registers->osc2 = BIT_FIELD_VALUE(PMC_OSC2_KEY, PMC_OSC2_KEY_VALUE)
			| PMC_OSC2_EN_WR_CALIB_MASK;
	(void)pmc->registers->osc2;
	asm volatile("nop");
	pmc->registers->osc2 = osc2;
	(void)pmc->registers->osc2;
	asm volatile("nop");
	pmc->registers->osc2 &= (uint32_t)(~PMC_OSC2_EN_WR_CALIB_MASK);
}

void
Pmc_getRc2OscillatorConfig(const Pmc *const pmc, Pmc_Rc2OscConfig *const config)
{
	assert(config != NULL);

	config->isEnabled = ((pmc->registers->osc2 & PMC_OSC2_EN_MASK) != 0u);
	config->frequency = ((pmc->registers->osc2 & PMC_OSC2_OSCRCF_MASK)
			>> PMC_OSC2_OSCRCF_OFFSET);
}

typedef struct {
	uint32_t selMask;
	uint32_t calFreqMask;
	uint32_t calFreqOffset;
	uint32_t calTempMask;
	uint32_t calTempOffset;
} CalibrationConfiguration;

void
Pmc_setCalibrationConfigurationForMainRc(Pmc *const pmc,
		const Pmc_RcOscFreq frequency, const uint32_t temperatureTrim,
		const uint32_t frequencyTrim)
{
	static const CalibrationConfiguration calibration4M = {
		.selMask = PMC_OCR_SEL4_MASK,
		.calFreqMask = PMC_OCR_CAL4_FREQ_MASK,
		.calFreqOffset = PMC_OCR_CAL4_FREQ_OFFSET,
		.calTempMask = PMC_OCR_CAL4_TEMP_MASK,
		.calTempOffset = PMC_OCR_CAL4_TEMP_OFFSET,
	};

	static const CalibrationConfiguration calibration8M = {
		.selMask = PMC_OCR_SEL8_MASK,
		.calFreqMask = PMC_OCR_CAL8_FREQ_MASK,
		.calFreqOffset = PMC_OCR_CAL8_FREQ_OFFSET,
		.calTempMask = PMC_OCR_CAL8_TEMP_MASK,
		.calTempOffset = PMC_OCR_CAL8_TEMP_OFFSET,
	};

	static const CalibrationConfiguration calibration10M = {
		.selMask = PMC_OCR_SEL10_MASK,
		.calFreqMask = PMC_OCR_CAL10_FREQ_MASK,
		.calFreqOffset = PMC_OCR_CAL10_FREQ_OFFSET,
		.calTempMask = PMC_OCR_CAL10_TEMP_MASK,
		.calTempOffset = PMC_OCR_CAL10_TEMP_OFFSET,
	};

	static const CalibrationConfiguration calibration12M = {
		.selMask = PMC_OCR_SEL12_MASK,
		.calFreqMask = PMC_OCR_CAL12_FREQ_MASK,
		.calFreqOffset = PMC_OCR_CAL12_FREQ_OFFSET,
		.calTempMask = PMC_OCR_CAL12_TEMP_MASK,
		.calTempOffset = PMC_OCR_CAL12_TEMP_OFFSET,
	};

	CalibrationConfiguration calibration;

	switch (frequency) {
	case Pmc_RcOscFreq_4M: calibration = calibration4M; break;
	case Pmc_RcOscFreq_8M: calibration = calibration8M; break;
	case Pmc_RcOscFreq_10M: calibration = calibration10M; break;
	case Pmc_RcOscFreq_12M: calibration = calibration12M; break;
	default: return;
	}

	pmc->registers->osc2 = BIT_FIELD_VALUE(PMC_OSC2_KEY, PMC_OSC2_KEY_VALUE)
			| PMC_OSC2_EN_WR_CALIB_MASK;

	pmc->registers->ocr1 = calibration.selMask
			| ((frequencyTrim << calibration.calFreqOffset)
					& calibration.calFreqMask)
			| ((temperatureTrim << calibration.calTempOffset)
					& calibration.calTempMask);

	pmc->registers->osc2 =
			BIT_FIELD_VALUE(PMC_OSC2_KEY, PMC_OSC2_KEY_VALUE);
}

void
Pmc_setCalibrationConfigurationForSecondRc(Pmc *const pmc,
		const Pmc_RcOscFreq frequency, const uint32_t temperatureTrim,
		const uint32_t frequencyTrim)
{
	static const CalibrationConfiguration calibration4M = {
		.selMask = PMC_OCR2_SEL4_MASK,
		.calFreqMask = PMC_OCR2_CAL4_FREQ_MASK,
		.calFreqOffset = PMC_OCR2_CAL4_FREQ_OFFSET,
		.calTempMask = PMC_OCR2_CAL4_TEMP_MASK,
		.calTempOffset = PMC_OCR2_CAL4_TEMP_OFFSET,
	};

	static const CalibrationConfiguration calibration8M = {
		.selMask = PMC_OCR2_SEL8_MASK,
		.calFreqMask = PMC_OCR2_CAL8_FREQ_MASK,
		.calFreqOffset = PMC_OCR2_CAL8_FREQ_OFFSET,
		.calTempMask = PMC_OCR2_CAL8_TEMP_MASK,
		.calTempOffset = PMC_OCR2_CAL8_TEMP_OFFSET,
	};

	static const CalibrationConfiguration calibration10M = {
		.selMask = PMC_OCR2_SEL10_MASK,
		.calFreqMask = PMC_OCR2_CAL10_FREQ_MASK,
		.calFreqOffset = PMC_OCR2_CAL10_FREQ_OFFSET,
		.calTempMask = PMC_OCR2_CAL10_TEMP_MASK,
		.calTempOffset = PMC_OCR2_CAL10_TEMP_OFFSET,
	};

	static const CalibrationConfiguration calibration12M = {
		.selMask = PMC_OCR2_SEL12_MASK,
		.calFreqMask = PMC_OCR2_CAL12_FREQ_MASK,
		.calFreqOffset = PMC_OCR2_CAL12_FREQ_OFFSET,
		.calTempMask = PMC_OCR2_CAL12_TEMP_MASK,
		.calTempOffset = PMC_OCR2_CAL12_TEMP_OFFSET,
	};

	CalibrationConfiguration calibration;

	switch (frequency) {
	case Pmc_RcOscFreq_4M: calibration = calibration4M; break;
	case Pmc_RcOscFreq_8M: calibration = calibration8M; break;
	case Pmc_RcOscFreq_10M: calibration = calibration10M; break;
	case Pmc_RcOscFreq_12M: calibration = calibration12M; break;
	default: return;
	}

	pmc->registers->osc2 = BIT_FIELD_VALUE(PMC_OSC2_KEY, PMC_OSC2_KEY_VALUE)
			| PMC_OSC2_EN_WR_CALIB_MASK;

	pmc->registers->ocr2 = calibration.selMask
			| ((frequencyTrim << calibration.calFreqOffset)
					& calibration.calFreqMask)
			| ((temperatureTrim << calibration.calTempOffset)
					& calibration.calTempMask);

	pmc->registers->osc2 = ((PMC_OSC2_KEY_VALUE << PMC_OSC2_KEY_OFFSET)
			& PMC_OSC2_KEY_MASK);
}
#endif
