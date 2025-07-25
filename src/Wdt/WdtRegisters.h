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

#ifndef BSP_WDT_REGISTERS_H
#define BSP_WDT_REGISTERS_H

#include <stdint.h>

/// \brief Structure representing WDT registers.
typedef struct {
	uint32_t cr; ///< 0x00 WDT_CR Watchdog Timer Control Register.
	uint32_t mr; ///< 0x04 WDT_MR Watchdog Timer Mode Register.
	uint32_t sr; ///< 0x08 WDT_SR Watchdog Timer Status Register.
} Wdt_Registers;

#if defined(N7S_TARGET_SAMV71Q21)

/// \brief Address base of the WDT0 peripheral.
#define WDT0_ADDRESS_BASE 0x400E1850

#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)

/// \brief Address base of the WDT0 peripheral.
#define WDT0_ADDRESS_BASE 0x40100250u

#else

#error "No target platform specified (missing N7S_TARGET_* macro)"

#endif

/// \brief Watchdog Restart mask.
#define WDT_CR_WDRSTT_MASK 0x00000001u
/// \brief Watchdog Restart offset.
#define WDT_CR_WDRSTT_OFFSET 0u

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
/// \brief Watchdog Reset Processor mask.
#define WDT_CR_LOCKMR_MASK 0x00000010u
/// \brief Watchdog Reset Processor offset.
#define WDT_CR_LOCKMR_OFFSET 4u

#endif

/// \brief Watchdog Restart Key mask.
#define WDT_CR_KEY_MASK 0xFF000000u
/// \brief Watchdog Restart Key offset.
#define WDT_CR_KEY_OFFSET 24u

/// \brief Watchdog Restart Key.
#define WDT_CR_KEY_VALUE 0xA5u

/// \brief Watchdog Counter Value mask.
#define WDT_MR_WDV_MASK 0x00000FFFu
/// \brief Watchdog Counter Value offset.
#define WDT_MR_WDV_OFFSET 0u
/// \brief Watchdog Fault Interrupt Enable mask.
#define WDT_MR_WDFIEN_MASK 0x00001000u
/// \brief Watchdog Fault Interrupt Enable offset.
#define WDT_MR_WDFIEN_OFFSET 12u
/// \brief Watchdog Reset Enable mask.
#define WDT_MR_WDRSTEN_MASK 0x00002000u
/// \brief Watchdog Reset Enable offset.
#define WDT_MR_WDRSTEN_OFFSET 13u

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
/// \brief Watchdog Reset Processor mask.
#define WDT_MR_WDRPROC_MASK 0x00004000u
/// \brief Watchdog Reset Processor offset.
#define WDT_MR_WDRPROC_OFFSET 14u
#endif

/// \brief Watchdog Disable mask.
#define WDT_MR_WDDIS_MASK 0x00008000u
/// \brief Watchdog Disable offset.
#define WDT_MR_WDDIS_OFFSET 15u
/// \brief Watchdog Delta Value mask.
#define WDT_MR_WDD_MASK 0x0FFF0000u
/// \brief Watchdog Delta Value offset.
#define WDT_MR_WDD_OFFSET 16u
/// \brief Watchdog Debug Halt mask.
#define WDT_MR_WDDBGHLT_MASK 0x10000000u
/// \brief Watchdog Debug Halt offset.
#define WDT_MR_WDDBGHLT_OFFSET 28u
/// \brief Watchdog Idle Halt mask.
#define WDT_MR_WDIDLEHLT_MASK 0x20000000u
/// \brief Watchdog Idle Halt offset.
#define WDT_MR_WDIDLEHLT_OFFSET 29u

/// \brief Watchdog Underflow mask.
#define WDT_SR_WDUNF_MASK 0x00000001u
/// \brief Watchdog Underflow offset.
#define WDT_SR_WDUNF_OFFSET 0u
/// \brief Watchdog Error mask.
#define WDT_SR_WDERR_MASK 0x00000002u
/// \brief Watchdog Error offset.
#define WDT_SR_WDERR_OFFSET 1u

#endif // BSP_WDT_REGISTERS_H
