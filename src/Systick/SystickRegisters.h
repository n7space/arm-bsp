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

#ifndef BSP_SYSTICK_REGISTERS_H
#define BSP_SYSTICK_REGISTERS_H

#include <stdint.h>

/// \brief Structure representing SysTick registers.
typedef struct {
	uint32_t csr; ///< Control and Status register.
	uint32_t rvr; ///< Reload Value register.
	uint32_t cvr; ///< Current Value register.
	uint32_t calib; ///< Calibration Value register.
	uint32_t reserved[56]; ///< Reserved.
} Systick_Registers;

/// \brief Address base of the SysTick registers.
#define SYSTICK_ADDRESS_BASE 0xE000E010u

/// \brief Count Flag register offset.
#define SYSTICK_CSR_COUNTFLAG_OFFSET 16u
/// \brief Count Flag register mask.
#define SYSTICK_CSR_COUNTFLAG_MASK 0x00010000u

/// \brief SysTick Clock Source register offset.
#define SYSTICK_CSR_CLKSOURCE_OFFSET 2U
/// \brief SysTick Clock Source register mask.
#define SYSTICK_CSR_CLKSOURCE_MASK 0x00000004u

/// \brief SysTick Interrupt Status register offset.
#define SYSTICK_CSR_TICKINT_OFFSET 1u
/// \brief SysTick Interrupt Status register mask.
#define SYSTICK_CSR_TICKINT_MASK 0x00000002u

/// \brief SysTick Enable register offset.
#define SYSTICK_CSR_ENABLE_OFFSET 0u
/// \brief SysTick Enable register mask.
#define SYSTICK_CSR_ENABLE_MASK 0x00000001u

/// \brief SysTick Reload Value register offset.
#define SYSTICK_RVR_RELOAD_OFFSET 0u
/// \brief SysTick Reload Value register mask.
#define SYSTICK_RVR_RELOAD_MASK 0x00FFFFFFu

/// \brief SysTick Current Value register offset.
#define SYSTICK_CVR_CURRENT_OFFSET 0u
/// \brief SysTick Current Value register mask.
#define SYSTICK_CVR_CURRENT_MASK 0xFFFFFFFFu

/// \brief 10ms Reload Value register offset.
#define SYSTICK_CALIB_TENMS_OFFSET 0u
/// \brief 10ms Reload Value register mask.
#define SYSTICK_CALIB_TENMS_MASK 0x00FFFFFFu

/// \brief Is the 10ms Reload Value Exact register offset.
#define SYSTICK_CALIB_SKEW_OFFSET 30u
/// \brief Is the 10ms Reload Value Exact register mask.
#define SYSTICK_CALIB_SKEW_MASK 0x40000000u

/// \brief Is the Implementation Defined Reference Clock Implemented register offset.
#define SYSTICK_CALIB_NOREF_OFFSET 31u
/// \brief Is the Implementation Defined Reference Clock Implemented register mask.
#define SYSTICK_CALIB_NOREF_MASK 0x80000000u

#endif // BSP_SYSTICK_REGISTERS_H
