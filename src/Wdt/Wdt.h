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

/// \file Wdt.h
/// \addtogroup Bsp
/// \brief Header for the Watchdog Timer (WDT) driver.

#ifndef BSP_WDT_H
#define BSP_WDT_H

#include <Utils/Utils.h>

#include "WdtRegisters.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Wdt
/// @ingroup Bsp
/// @{

/// \brief Structure holding a WDT instance configuration.
typedef struct {
	/// \brief The value loaded in the 12-bit watchdog counter.
	uint32_t counterValue;
	/// \brief The permitted range for reloading the watchdog timer.
	uint32_t deltaValue;
	/// \brief Does a watchdog fault (underflow or error) trigger a watchdog reset.
	bool isResetEnabled;

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	/// \brief Flag indicating whether a watchdog fault (underflow or error) triggers all resets or
	///        just the processor reset.
	bool doesFaultActivateProcessorReset;
#endif

	/// \brief Does a watchdog fault (underflow or error) assert an interrupt.
	bool isFaultInterruptEnabled;
	/// \brief Is the watchdog disabled.
	bool isDisabled;
	/// \brief Is the watchdog halted in idle state.
	bool isHaltedOnIdle;
	/// \brief Is the watchdog halted in debug state.
	bool isHaltedOnDebug;
} Wdt_Config;

/// \brief Structure representing a WDT instance.
typedef struct {
	volatile Wdt_Registers
			*registers; ///< Pointer to watchdog instance registers.
} Wdt;

/// \brief Initializes the structure representing a WDT instance.
/// \param [in,out] wdt Pointer to a structure representing a WDT instance.
void Wdt_init(Wdt *const wdt);

/// \brief Sets the WDT configuration. Can be called only once for the given WDT instance.
/// \param [in,out] wdt Pointer to a structure representing a WDT instance.
/// \param [in] config WDT instance configuration.
void Wdt_setConfig(Wdt *const wdt, const Wdt_Config *const config);

/// \brief Gets the WDT configuration.
/// \param [in] wdt Pointer to a structure representing a WDT instance.
/// \param [out] config WDT instance configuration.
void Wdt_getConfig(const Wdt *const wdt, Wdt_Config *const config);

/// \brief Resets the counter of the given WDT instance.
/// \param [in] wdt Pointer to a structure representing a WDT instance.
void Wdt_reset(Wdt *const wdt);

/// \brief Returns whether an underflow occurred for the given WDT instance.
/// \param [in] wdt Pointer to a structure representing a WDT instance.
/// \retval true Underflow occurred.
/// \retval false Underflow did not occur.
bool Wdt_isUnderflow(const Wdt *const wdt);

/// \brief Returns whether an error occurred for the given WDT instance.
/// \param [in] wdt Pointer to a structure representing a WDT instance.
/// \retval true Error occurred.
/// \retval false Error did not occur.
bool Wdt_isError(const Wdt *const wdt);

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // BSP_WDT_H
