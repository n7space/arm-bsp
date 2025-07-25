/**@file
 * This file is part of the N7-Core library used in the Test Environment.
 *
 * @copyright 2023-2024 N7 Space Sp. z o.o.
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

#ifndef UTILS_CONCURRENTACCESSFLAG_H
#define UTILS_CONCURRENTACCESSFLAG_H

/// \file  ConcurrentAccessFlag.h
/// \addtogroup Utils
/// \brief Module representing flag for monitoring concurrent access to data.

#include <stdbool.h>

/// @addtogroup  ConcurrentAccessFlag
/// @ingroup Utils
/// @{

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Structure representing concurrent access flag.
/// \details Flag can be used to monitor e.g. changes made by the interrupt handlers.
typedef struct {
	bool flag;
} ConcurrentAccessFlag;

/// \brief Resets the flag, should be called before entering the monitored section.
/// \details Usually should be called by user code before entering section that uses data which
///          could be modified concurrently (for example by some interrupt handler).
/// \param [in,out] self The flag.
void ConcurrentAccessFlag_reset(ConcurrentAccessFlag *const self);

/// \brief Checks the flag, should be called after leaving the monitored section.
/// \details Usually should be called by user code after leaving section that uses data which
///          could be modified concurrently (for example by some interrupt handler). If the check
///          returns true it means that the operation might got corrupted results.
/// \param [in] self The flag.
/// \retval true when the flag was modified since last reset call.
/// \retval false otherwise.
bool ConcurrentAccessFlag_check(ConcurrentAccessFlag *const self);

/// \brief Sets the flag, should be called by procedure that modifies the data.
/// \details Usually should be called by user code that modifies the shared data, for example by
///          interrupt handler.
/// \param [in,out] self The flag.
void ConcurrentAccessFlag_set(ConcurrentAccessFlag *const self);

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // UTILS_CONCURRENTACCESSFLAG_H
