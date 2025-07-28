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

#include "Rstc.h"

#include <assert.h>
#include <stddef.h>

#include <Utils/Bits.h>

// cppcheck-suppress misra-c2012-11.4
static volatile Rstc_Registers *const Rstc_registers =
		(volatile Rstc_Registers *)RSTC_BASE_ADDRESS;

void
Rstc_getStatus(Rstc_Status *const status)
{
	assert(status != NULL);
	const uint32_t regValue = Rstc_registers->sr;
	status->wasUserResetTriggered = (regValue & RSTC_SR_URSTS_MASK) != 0u;
	status->resetType = (Rstc_ResetType)((regValue & RSTC_SR_RSTTYP_MASK)
			>> RSTC_SR_RSTTYP_OFFSET);
	status->isNrstHigh = (regValue & RSTC_SR_NRSTL_MASK) != 0u;
	status->isCommandInProgress = (regValue & RSTC_SR_SRCMP_MASK) != 0u;
}

bool
Rstc_isBusy(void)
{
	Rstc_Status status;
	Rstc_getStatus(&status);
	return status.isCommandInProgress;
}

Rstc_ResetType
Rstc_getLastResetType(void)
{
	Rstc_Status status;
	Rstc_getStatus(&status);
	return status.resetType;
}

// LCOV_EXCL_START

// Rstc_resetSystem() function is used to perform MCU reset. As a result it is not possible to
// retrieve its coverage using current tools (the coverage data gets destroyed on the reset of the
// system). The function coverage was acquired via analysis of the RstcTest::test_RstcResetsSystem
// integration tests, which uses it to perform actual reset of the system. Function does not contain
// any conditions etc., hence a single test is enough to check its functional coverage.
void
Rstc_resetSystem(void)
{
	Rstc_registers->cr = BIT_FIELD_VALUE(RSTC_CR_KEY, RSTC_CR_KEY_PASSWD)
			| BIT_FIELD_VALUE(RSTC_CR_PROCRST, 1u);

	while (true)
		asm volatile("nop" ::: "memory");
}

// LCOV_EXCL_STOP

bool
Rstc_wasNrstTransitionDetected(void)
{
	Rstc_Status status;
	Rstc_getStatus(&status);
	return status.wasUserResetTriggered;
}

#if defined(N7S_TARGET_SAMV71Q21)
void
Rstc_triggerExternalReset(void)
{
	Rstc_registers->cr = BIT_FIELD_VALUE(RSTC_CR_KEY, RSTC_CR_KEY_PASSWD)
			| BIT_FIELD_VALUE(RSTC_CR_EXTRST, 1u);
}

void
Rstc_setUserResetEnabled(bool enabled)
{
	uint32_t mr = Rstc_registers->mr;
	mr &= ~RSTC_MR_URSTEN_MASK;
	mr |= BIT_FIELD_VALUE(RSTC_MR_KEY, RSTC_MR_KEY_PASSWD);
	mr |= BIT_VALUE(RSTC_MR_URSTEN, enabled);
	Rstc_registers->mr = mr;
}

bool
Rstc_isUserResetEnabled(void)
{
	return (Rstc_registers->mr & RSTC_MR_URSTEN_MASK) != 0u;
}

void
Rstc_setUserResetInterruptEnabled(bool enabled)
{
	uint32_t mr = Rstc_registers->mr;
	mr &= ~RSTC_MR_URSTIEN_MASK;
	mr |= BIT_FIELD_VALUE(RSTC_MR_KEY, RSTC_MR_KEY_PASSWD);
	mr |= BIT_VALUE(RSTC_MR_URSTIEN, enabled);
	Rstc_registers->mr = mr;
}

bool
Rstc_isUserResetInterruptEnabled(void)
{
	return (Rstc_registers->mr & RSTC_MR_URSTIEN_MASK) != 0u;
}

void
Rstc_setExternalResetLength(uint8_t length)
{
	uint32_t mr = Rstc_registers->mr;
	mr &= ~RSTC_MR_ERSTL_MASK;
	mr |= BIT_FIELD_VALUE(RSTC_MR_KEY, RSTC_MR_KEY_PASSWD);
	mr |= BIT_FIELD_VALUE(RSTC_MR_ERSTL, length);
	Rstc_registers->mr = mr;
}

uint8_t
Rstc_getExternalResetLength(void)
{
	return (uint8_t)((Rstc_registers->mr & RSTC_MR_ERSTL_MASK)
			>> RSTC_MR_ERSTL_OFFSET);
}

#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
void
Rstc_triggerPeripheralReset(const uint8_t peripheralId)
{
	Rstc_registers->cr = BIT_FIELD_VALUE(RSTC_CR_KEY, RSTC_CR_KEY_PASSWD)
			| BIT_FIELD_VALUE(RSTC_CR_PERID, peripheralId)
			| RSTC_CR_PERIDON_MASK;
}

void
Rstc_setCpuClockFailureResetEnabled(const bool enabled)
{
	uint32_t mr = Rstc_registers->mr;
	mr &= ~RSTC_MR_CPUFEN_MASK;
	mr |= BIT_FIELD_VALUE(RSTC_MR_KEY, RSTC_MR_KEY_PASSWD);
	mr |= BIT_FIELD_VALUE(RSTC_MR_CPUFEN, enabled);
	Rstc_registers->mr = mr;
}

void
Rstc_setSlowClockCrystalFailureResetEnabled(const bool enabled)
{
	uint32_t mr = Rstc_registers->mr;
	mr &= ~RSTC_MR_SCKSW_MASK;
	mr |= BIT_FIELD_VALUE(RSTC_MR_KEY, RSTC_MR_KEY_PASSWD);
	mr |= BIT_FIELD_VALUE(RSTC_MR_SCKSW, enabled);
	Rstc_registers->mr = mr;
}

#endif
