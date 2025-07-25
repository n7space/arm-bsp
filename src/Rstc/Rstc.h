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

/// \file Rstc.h
/// \addtogroup Bsp
/// \brief Header for the Reset Controller (RSTC) driver.

#ifndef BSP_RSTC_H
#define BSP_RSTC_H

#include <stdbool.h>
#include <stdint.h>

#include "RstcRegisters.h"

/// @addtogroup Rstc
/// @ingroup Bsp
/// @{

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Enumeration listing possible reset types.
typedef enum {
	Rstc_ResetType_Powerup = 0, ///< First power-up reset.
	Rstc_ResetType_Backup = 1, ///< Return from Backup mode.
	Rstc_ResetType_Watchdog = 2, ///< Watchdog fault occurred.
	Rstc_ResetType_Software = 3, ///< Processor reset required by software.
	Rstc_ResetType_User = 4, ///< NRST pin detected low.
} Rstc_ResetType;

/// \brief Structure describing Reset Controller (Rstc) status.
typedef struct {
	bool wasUserResetTriggered; ///< Was user reset triggered.
	Rstc_ResetType resetType; ///< Last processor reset type.
	bool isNrstHigh; ///< Is NRST pin high.
	bool isCommandInProgress; ///< Is software reset command in progress.
} Rstc_Status;

/// \brief Reads combined Rstc status.
/// \param [in,out] status Pointer to a structure which will hold the current status.
void Rstc_getStatus(Rstc_Status *const status);

/// \brief Reads Rstc status and returns whether Rstc is busy.
/// \returns Whether a software reset command is in progress.
bool Rstc_isBusy(void);

/// \brief Reads Rstc status and returns the last reset type.
/// \returns The last reset type.
Rstc_ResetType Rstc_getLastResetType(void);

/// \brief Causes a system reset.
/// \details ARM recommends to execute memory barrier (DSB) before resetting the system.
///          Optionally IRQ disable should also be executed. User of this function should take
///          those recommendation into account when designing reset procedure.
void Rstc_resetSystem(void);

/// \brief Reads Rstc status and returns whether Nrst transition was detected.
/// \returns Whether Nrst transition was detected.
bool Rstc_wasNrstTransitionDetected(void);

#if defined(N7S_TARGET_SAMV71Q21)
/// \brief Triggers an external reset.
void Rstc_triggerExternalReset(void);

/// \brief Enables and disables the user reset.
/// \param [in] enabled Whether the user reset shall be enabled.
void Rstc_setUserResetEnabled(const bool enabled);

/// \brief Returns whether the user reset is enabled.
/// \returns Whether the user reset is enabled.
bool Rstc_isUserResetEnabled(void);

/// \brief Enables and disables the user reset interrupt.
/// \param [in] enabled Whether the user reset interrupt shall be enabled.
void Rstc_setUserResetInterruptEnabled(bool enabled);

/// \brief Returns whether the user reset interrupt is enabled.
/// \returns Whether the user reset interrupt is enabled.
bool Rstc_isUserResetInterruptEnabled(void);

/// \brief Sets the external reset length.
/// \param [in] length The external reset length.
void Rstc_setExternalResetLength(uint8_t length);

/// \brief Returns the external reset length.
/// \returns The external reset length.
uint8_t Rstc_getExternalResetLength(void);

#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
/// \brief Triggers a peripheral reset.
/// \param [in] peripheralId Identifier of the peripheral to be reset.
void Rstc_triggerPeripheralReset(const uint8_t peripheralId);

/// \brief Sets whether a CPU clock failure will trigger a system reset.
/// \param [in] enabled Flag indicating whether the failure should trigger a reset.
void Rstc_setCpuClockFailureResetEnabled(const bool enabled);

/// \brief Sets whether a slow clock crystal failure will trigger a system reset.
/// \param [in] enabled Flag indicating whether the failure should trigger a reset.
void Rstc_setSlowClockCrystalFailureResetEnabled(const bool enabled);
#endif

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // BSP_RSTC_H
