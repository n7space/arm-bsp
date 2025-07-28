/**@file
 * This file is part of the ARM BSP for the Test Environment.
 *
 * @copyright 2018-2024 N7 Space Sp. z o.o.
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

#include "Wdt.h"

#include <Utils/Bits.h>

#if defined(N7S_TARGET_SAMV71Q21)
#include <bsp/arm/Rstc/Rstc.h>
#include <bsp/arm/Rstc/RstcRegisters.h>
#endif // N7S_TARGET_SAMV71Q21

void
Wdt_init(Wdt *const wdt)
{
	// cppcheck-suppress misra-c2012-11.4
	wdt->registers = (Wdt_Registers *)WDT0_ADDRESS_BASE;
}

void
Wdt_setConfig(Wdt *const wdt, const Wdt_Config *const config)
{
	wdt->registers->mr = BIT_FIELD_VALUE(WDT_MR_WDV, config->counterValue)
			| BIT_FIELD_VALUE(WDT_MR_WDD, config->deltaValue)
			| BIT_VALUE(WDT_MR_WDDIS, config->isDisabled)
			| BIT_VALUE(WDT_MR_WDFIEN,
					config->isFaultInterruptEnabled)
#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
			| BIT_VALUE(WDT_MR_WDRPROC,
					config->doesFaultActivateProcessorReset)
#endif
			| BIT_VALUE(WDT_MR_WDRSTEN, config->isResetEnabled)
			| BIT_VALUE(WDT_MR_WDDBGHLT, config->isHaltedOnDebug)
			| BIT_VALUE(WDT_MR_WDIDLEHLT, config->isHaltedOnIdle);

#if defined(N7S_TARGET_SAMV71Q21)
	// Wdt infinity loop protection. According to SAMV71Q21 errata, MR.ERSTL register
	// have to be greater than 0 or else wdt may fall into infinite loop.
	volatile Rstc_Registers *rstcRegisters = (volatile Rstc_Registers *)
			RSTC_BASE_ADDRESS; // cppcheck-suppress[misra-c2012-11.4]

	const uint8_t resetLengthSize = 0x1u;

	uint32_t mr = rstcRegisters->mr;
	mr &= ~RSTC_MR_ERSTL_MASK;
	mr |= BIT_FIELD_VALUE(RSTC_MR_KEY, RSTC_MR_KEY_PASSWD);
	mr |= BIT_FIELD_VALUE(RSTC_MR_ERSTL, resetLengthSize);
	rstcRegisters->mr = mr;
#endif // N7S_TARGET_SAMV71Q21
}

void
Wdt_getConfig(const Wdt *const wdt, Wdt_Config *const config)
{
	config->counterValue = (wdt->registers->mr & WDT_MR_WDV_MASK)
			>> WDT_MR_WDV_OFFSET;
	config->deltaValue = (wdt->registers->mr & WDT_MR_WDD_MASK)
			>> WDT_MR_WDD_OFFSET;
	config->isDisabled = (wdt->registers->mr & WDT_MR_WDDIS_MASK) != 0u;
	config->isFaultInterruptEnabled =
			(wdt->registers->mr & WDT_MR_WDFIEN_MASK) != 0u;
#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	config->doesFaultActivateProcessorReset =
			(wdt->registers->mr & WDT_MR_WDRPROC_MASK) != 0u;
#endif
	config->isResetEnabled =
			(wdt->registers->mr & WDT_MR_WDRSTEN_MASK) != 0u;
	config->isHaltedOnDebug =
			(wdt->registers->mr & WDT_MR_WDDBGHLT_MASK) != 0u;
	config->isHaltedOnIdle =
			(wdt->registers->mr & WDT_MR_WDIDLEHLT_MASK) != 0u;
}

void
Wdt_reset(Wdt *const wdt)
{
	wdt->registers->cr = BIT_FIELD_VALUE(WDT_CR_KEY, WDT_CR_KEY_VALUE)
			| BIT_FIELD_VALUE(WDT_CR_WDRSTT, 1u);
}

bool
Wdt_isUnderflow(const Wdt *const wdt)
{
	return (wdt->registers->sr & WDT_SR_WDUNF_MASK) != 0u;
}

bool
Wdt_isError(const Wdt *const wdt)
{
	return (wdt->registers->sr & WDT_SR_WDERR_MASK) != 0u;
}
