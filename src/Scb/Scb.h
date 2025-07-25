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

/// \file Scb.h
/// \addtogroup Bsp
/// \brief Header containing interface for SCB (System Control Block) driver.

#ifndef BSP_SCB_H
#define BSP_SCB_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "ScbRegisters.h"

#ifdef CLANG_TIDY
// Clang-tidy assumes x86 architecture.
#define ASM_R0 "eax"
#define ASM_R1 "ebx"
#define ASM_R2 "ecx"
#define ASM_R3 "edx"
#else
#define ASM_R0 "r0"
#define ASM_R1 "r1"
#define ASM_R2 "r2"
#define ASM_R3 "r3"
#endif

/// @addtogroup Scb
/// @ingroup Bsp
/// @{

#ifdef __cplusplus
extern "C" {
#endif

/// \brief A memory barrier macro.
#define MEMORY_SYNC_BARRIER() \
	do { \
		asm volatile("dsb" ::: "memory"); \
		asm volatile("isb" ::: "memory"); \
	} while (0)

/// \brief Returns whether the instruction cache is enabled.
/// \returns Whether the instruction cache is enabled.
static inline bool
Scb_isICacheEnabled(void)
{
	// cppcheck-suppress misra-c2012-11.4
	const volatile Scb_Registers *const scb =
			(volatile Scb_Registers *)SCB_BASE_ADDRESS;
	return (scb->ccr & SCB_CCR_IC_MASK) != 0u;
}

/// \brief Returns whether the data cache is enabled.
/// \returns Whether the data cache is enabled.
static inline bool
Scb_isDCacheEnabled(void)
{
	// cppcheck-suppress misra-c2012-11.4
	const volatile Scb_Registers *const scb =
			(volatile Scb_Registers *)SCB_BASE_ADDRESS;
	return (scb->ccr & SCB_CCR_DC_MASK) != 0u;
}

/// \brief Returns whether the memory management exception is enabled.
/// \returns Whether the memory management exception is enabled.
static inline bool
Scb_isMemoryManagementExceptionEnabled(void)
{
	// cppcheck-suppress misra-c2012-11.4
	const volatile Scb_Registers *const scb =
			(volatile Scb_Registers *)SCB_BASE_ADDRESS;
	return (scb->shcsr & SCB_SHCSR_MEMFAULTENA_MASK) != 0u;
}

/// \brief Disables the instruction cache.
/// \returns Whether the cache disable procedure was executed.
static inline bool
Scb_disableICache(void)
{
	// cppcheck-suppress misra-c2012-11.4
	const volatile Scb_Registers *const scb =
			(volatile Scb_Registers *)SCB_BASE_ADDRESS;
	// If the cache is already disabled, don't disable it, as cache clean and invalidation may lead
	// to artifacts.
	if (!Scb_isICacheEnabled())
		return false;

	// Assembly below should be the equivalent of this code.
	// This is rewritten to assembly to avoid any instrumentation (e.g. gcov) interference.
	//   asm volatile ("dsb");
	//   asm volatile ("isb");
	//   scb->ccr &= ~SCB_CCR_IC_MASK;  <- Disable I-Cache.
	//   scb->iciallu = 0;              <- Invalidate I-cache.
	//   asm volatile ("dsb");
	//   asm volatile ("isb");
	const uint32_t newCcr = scb->ccr & ~(uint32_t)SCB_CCR_IC_MASK;
	asm volatile("mov r1, #0\n" // r1 = 0;
		     "dsb\n"
		     "isb\n"
		     "str %2, [%1]\n" // *CCR = newCcr;
		     "str r1, [%0]\n" // *ICIALLU = r1 (0);
		     "nop\n"
		     "nop\n"
		     "nop\n"
		     "nop\n"
		     "nop\n"
		     "nop\n"
		     "dsb\n"
		     "isb\n"
			:
			: "r"(&(scb->iciallu)), "r"(&(scb->ccr)), "r"(newCcr)
			: ASM_R1);
	return true;
}

/// \brief Enables the instruction cache.
/// \returns Whether the cache enable procedure was executed.
static inline bool
Scb_enableICache(void)
{
	// cppcheck-suppress misra-c2012-11.4
	const volatile Scb_Registers *const scb =
			(volatile Scb_Registers *)SCB_BASE_ADDRESS;

	if (Scb_isICacheEnabled())
		return false;

	// Assembly below should be the equivalent of this code.
	// This is rewritten to assembly to avoid any instrumentation (e.g. gcov) interference.
	//   asm volatile ("dsb");
	//   asm volatile ("isb");
	//   scb->iciallu = 0;             <- Invalidate I-cache.
	//   asm volatile ("dsb");
	//   asm volatile ("isb");
	//   scb->ccr |= SCB_CCR_IC_MASK;  <- Enable I-Cache.
	//   asm volatile ("dsb");
	//   asm volatile ("isb");
	const uint32_t newCcr = scb->ccr | SCB_CCR_IC_MASK;
	asm volatile("mov r1, #0\n" // r1 = 0;
		     "dsb\n" //
		     "isb\n" //
		     "str r1, [%0]\n" // *ICIALLU = r1 (0);
		     "dsb\n" //
		     "isb\n" //
		     "str %2, [%1]\n" // *CCR = newCcr;
		     "nop\n"
		     "nop\n"
		     "nop\n"
		     "nop\n"
		     "nop\n"
		     "nop\n"
		     "dsb\n"
		     "isb\n"
			:
			: "r"(&(scb->iciallu)), "r"(&(scb->ccr)), "r"(newCcr)
			: ASM_R1);

	return true;
}

/// \brief Cleans the data cache.
/// \returns Whether the cache clean procedure was executed.
static inline bool
Scb_cleanDCache(void)
{
	if (!Scb_isDCacheEnabled())
		return false;
	// cppcheck-suppress misra-c2012-11.4
	static const volatile Scb_Registers *const scb =
			(volatile Scb_Registers *)SCB_BASE_ADDRESS;
	const uint32_t ccsidr = scb->ccsidr;
	const uint32_t sets = (ccsidr & SCB_CCSIDR_NUMSETS_MASK)
			>> SCB_CCSIDR_NUMSETS_OFFSET;
	const uint32_t ways = (ccsidr & SCB_CCSIDR_ASSOCIATIVITY_MASK)
			>> SCB_CCSIDR_ASSOCIATIVITY_OFFSET;

	asm volatile("dsb\n"
		     "ESETLOOP%=:\n"
		     "mov r0, %2\n" // r0(way) = ways;
		     "EWAYLOOP%=:\n" //
		     "mov r1, %1\n" // r1 = sets;
		     "lsl r1, %3\n" // r1 = r1 << SCB_DCCISW_SET_OFFSET;
		     "mov r2, r0\n" // r2 = way;
		     "lsl r2, %4\n" // r2 = r2 << SCB_DCCISW_WAY_OFFSET;
		     "orr r1, r2\n" // r1 |= r2;
		     "str r1, [%0]\n" // DCISW = r1;
		     "sub r0, r0, 1\n" // r1(ways)--;
		     "cmp r0, 0\n" //
		     "bge EWAYLOOP%=\n" // while(way >= 0);
		     "sub %1, %1, 1\n" // r0(sets)--;
		     "cmp %1, 0\n" //
		     "bge ESETLOOP%=\n" // while(set >= 0);
		     "dsb\n"
			:
			: "r"(&(scb->dccsw)), "r"(sets), "r"(ways),
			"n"(SCB_DCCISW_SET_OFFSET), "n"(SCB_DCCISW_WAY_OFFSET)
			: ASM_R0, ASM_R1, ASM_R2);

	return true;
}

/// \brief Invalidates the data cache.
/// \returns Whether the cache invalidation procedure was executed.
static inline bool
Scb_invalidateDCache(void)
{
	if (!Scb_isDCacheEnabled())
		return false;
	// cppcheck-suppress misra-c2012-11.4
	static const volatile Scb_Registers *const scb =
			(volatile Scb_Registers *)SCB_BASE_ADDRESS;
	const uint32_t ccsidr = scb->ccsidr;
	const uint32_t sets = (ccsidr & SCB_CCSIDR_NUMSETS_MASK)
			>> SCB_CCSIDR_NUMSETS_OFFSET;
	const uint32_t ways = (ccsidr & SCB_CCSIDR_ASSOCIATIVITY_MASK)
			>> SCB_CCSIDR_ASSOCIATIVITY_OFFSET;

	// Assembly below should be the equivalent of this code.
	// This is rewritten to assembly to avoid any instrumentation (e.g. gcov) interference.
	//   asm volatile("dsb");
	//   for (int32_t set = sets; set >= 0; set--)
	//     for (int32_t way = ways; way >= 0; way--)
	//       scb->dcisw = (((set << SCB_DCCISW_SET_OFFSET) & SCB_DCCISW_SET_MASK)
	//                    | ((way << SCB_DCCISW_WAY_OFFSET) & SCB_DCCISW_WAY_MASK));
	//   asm volatile ("dsb");
	//   scb->ccr |= SCB_CCR_DC_MASK; <- Enable D-Cache.
	//   asm volatile (
	//     "dsb\n"
	//     "isb\n"
	//   );

	asm volatile("dsb\n"
		     "ESETLOOP%=:\n"
		     "mov r0, %2\n" // r0(way) = ways;
		     "EWAYLOOP%=:\n" //
		     "mov r1, %1\n" // r1 = sets;
		     "lsl r1, %3\n" // r1 = r1 << SCB_DCCISW_SET_OFFSET;
		     "mov r2, r0\n" // r2 = way;
		     "lsl r2, %4\n" // r2 = r2 << SCB_DCCISW_WAY_OFFSET;
		     "orr r1, r2\n" // r1 |= r2;
		     "str r1, [%0]\n" // DCISW = r1;
		     "sub r0, r0, 1\n" // r1(ways)--;
		     "cmp r0, 0\n" //
		     "bge EWAYLOOP%=\n" // while(way >= 0);
		     "sub %1, %1, 1\n" // r0(sets)--;
		     "cmp %1, 0\n" //
		     "bge ESETLOOP%=\n" // while(set >= 0);
		     "dsb\n"
			:
			: "r"(&(scb->dcisw)), "r"(sets), "r"(ways),
			"n"(SCB_DCCISW_SET_OFFSET), "n"(SCB_DCCISW_WAY_OFFSET)
			: ASM_R0, ASM_R1, ASM_R2);

	return true;
}

/// \brief Invalidates instruction cache.
/// \returns Whether the cache invalidation procedure was executed.
static inline bool
Scb_invalidateICache(void)
{
	if (!Scb_isICacheEnabled())
		return false;
	// cppcheck-suppress misra-c2012-11.4
	static volatile Scb_Registers *const scb =
			(volatile Scb_Registers *)SCB_BASE_ADDRESS;

	asm volatile("dsb");
	asm volatile("isb");
	scb->iciallu = 0u;
	asm volatile("dsb");
	asm volatile("isb");

	return true;
}

/// \brief Invalidates given address in cache if address is found in cache.
/// \param [in] addr Pointer to the data to invalidate.
/// \param [in] size of block of memory to invalidate (must be multiple of cache line size).
/// \returns Whether the cache invalidation procedure was executed.
static inline bool
Scb_invalidateDCacheByAddr(const void *const addr, const uint32_t size)
{
	if (!Scb_isDCacheEnabled())
		return false;

	// cppcheck-suppress misra-c2012-11.4
	volatile Scb_Registers *const scb =
			(volatile Scb_Registers *)SCB_BASE_ADDRESS;

	assert((size % SCB_CACHE_LINE_SIZE) == 0u);

	// cppcheck-suppress misra-c2012-11.6
	uint32_t addrValue = (uint32_t)addr;
	asm volatile("dsb");

	uint32_t bytesLeft = size;
	while (bytesLeft != 0u) {
		scb->dcimvac = addrValue;
		addrValue += SCB_CACHE_LINE_SIZE;
		bytesLeft -= SCB_CACHE_LINE_SIZE;
	}
	asm volatile("dsb");
	asm volatile("isb");

	return true;
}

/// \brief Invalidates given address in instruction cache if address is found in cache.
/// \param [in] addr Pointer to the data to invalidate.
/// \param [in] size of block of memory to invalidate (must be multiple of cache line size).
/// \returns Whether the cache invalidation procedure was executed.
static inline bool
Scb_invalidateICacheByAddr(const void *const addr, const uint32_t size)
{
	if (!Scb_isICacheEnabled())
		return false;

	// cppcheck-suppress misra-c2012-11.4
	volatile Scb_Registers *const scb =
			(volatile Scb_Registers *)SCB_BASE_ADDRESS;

	assert((size % SCB_CACHE_LINE_SIZE) == 0u);

	// cppcheck-suppress misra-c2012-11.6
	uint32_t addrValue = (uint32_t)addr;
	asm volatile("dsb");

	uint32_t bytesLeft = size;
	while (bytesLeft != 0u) {
		scb->icimvau = addrValue;
		addrValue += SCB_CACHE_LINE_SIZE;
		bytesLeft -= SCB_CACHE_LINE_SIZE;
	}
	asm volatile("dsb");
	asm volatile("isb");

	return true;
}

/// \brief Cleans and invalidates given address in cache if address is found in cache.
/// \param [in] addr Pointer to the data to clean and invalidate.
/// \param [in] size of block of memory to process (must be multiple of cache line size).
/// \returns Whether the cache clean-invalidation procedure was executed.
static inline bool
Scb_cleanInvalidateDCacheByAddr(const void *const addr, const uint32_t size)
{
	if (!Scb_isDCacheEnabled())
		return false;

	// cppcheck-suppress misra-c2012-11.4
	volatile Scb_Registers *const scb =
			(volatile Scb_Registers *)SCB_BASE_ADDRESS;

	assert((size % SCB_CACHE_LINE_SIZE) == 0u);

	// cppcheck-suppress misra-c2012-11.6
	uint32_t addrValue = (uint32_t)addr;
	asm volatile("dsb");

	uint32_t bytesLeft = size;
	while (bytesLeft != 0u) {
		scb->dccimvac = addrValue;
		addrValue += SCB_CACHE_LINE_SIZE;
		bytesLeft -= SCB_CACHE_LINE_SIZE;
	}
	asm volatile("dsb");
	asm volatile("isb");

	return true;
}

/// \brief Cleans given address in cache if address is found in cache.
/// \param [in] addr Pointer to the data to clean.
/// \param [in] size of block of memory to clean (must be multiple of cache line size).
/// \returns Whether the cache clean procedure was executed.
static inline bool
Scb_cleanDCacheByAddr(const void *const addr, const uint32_t size)
{
	if (!Scb_isDCacheEnabled())
		return false;

	// cppcheck-suppress misra-c2012-11.4
	volatile Scb_Registers *const scb =
			(volatile Scb_Registers *)SCB_BASE_ADDRESS;

	assert((size % SCB_CACHE_LINE_SIZE) == 0u);

	// cppcheck-suppress misra-c2012-11.6
	uint32_t addrValue = (uint32_t)addr;
	asm volatile("dsb");

	uint32_t bytesLeft = size;
	while (bytesLeft != 0u) {
		scb->dccmvac = addrValue;
		addrValue += SCB_CACHE_LINE_SIZE;
		bytesLeft -= SCB_CACHE_LINE_SIZE;
	}
	asm volatile("dsb");
	asm volatile("isb");

	return true;
}

/// \brief Enables the data cache.
/// \returns Whether the cache enable procedure was executed.
static inline bool
Scb_enableDCache(void)
{
	// cppcheck-suppress misra-c2012-11.4
	static volatile Scb_Registers *const scb =
			(volatile Scb_Registers *)SCB_BASE_ADDRESS;

	if (Scb_isDCacheEnabled())
		return false;

	const uint32_t ccsidr = scb->ccsidr;
	const uint32_t sets = (ccsidr & SCB_CCSIDR_NUMSETS_MASK)
			>> SCB_CCSIDR_NUMSETS_OFFSET;
	const uint32_t ways = (ccsidr & SCB_CCSIDR_ASSOCIATIVITY_MASK)
			>> SCB_CCSIDR_ASSOCIATIVITY_OFFSET;

	// Assembly below should be the equivalent of this code.
	// This is rewritten to assembly to avoid any instrumentation (e.g. gcov) interference.
	//   asm volatile("dsb");
	//   for (uint32_t set = sets; set >= 0; set--)
	//     for (uint32_t way = ways; way >= 0; way--)
	//       scb->dcisw = (((set << SCB_DCCISW_SET_OFFSET) & SCB_DCCISW_SET_MASK)
	//                    | ((way << SCB_DCCISW_WAY_OFFSET) & SCB_DCCISW_WAY_MASK));
	//   asm volatile ("dsb");
	//   scb->ccr |= SCB_CCR_DC_MASK; <- Enable D-Cache.
	//   asm volatile (
	//     "dsb\n"
	//     "isb\n"
	//   );

	const uint32_t newCcr = scb->ccr | SCB_CCR_DC_MASK;
	asm volatile("dsb\n"
		     "ESETLOOP%=:\n"
		     "mov r0, %[ways]\n" // r0(way) = ways;
		     "EWAYLOOP%=:\n" //
		     "mov r1, %[sets]\n" // r1 = sets;
		     "lsl r1, %[setOffset]\n" // r1 = r1 << SCB_DCCISW_SET_OFFSET;
		     "mov r2, r0\n" // r2 = way;
		     "lsl r2, %[wayOffset]\n" // r2 = r2 << SCB_DCCISW_WAY_OFFSET;
		     "orr r1, r2\n" // r1 |= r2;
		     "str r1, [%[dciswAddr]]\n" // DCISW = r1;
		     "sub r0, r0, 1\n" // r1(ways)--;
		     "cmp r0, 0\n" //
		     "bge EWAYLOOP%=\n" // while(way >= 0);
		     "sub %[sets], %[sets], 1\n" // r0(sets)--;
		     "cmp %[sets], 0\n" //
		     "bge ESETLOOP%=\n" // while(set >= 0);
		     "dsb\n" //
		     "str %[newCcr], [%[ccrAddr]]\n" // CCR = newCcr;
		     "dsb\n"
		     "isb\n"
			:
			: [dciswAddr] "r"(&(scb->dcisw)),
			[ccrAddr] "r"(&(scb->ccr)), [newCcr] "r"(newCcr),
			[sets] "r"(sets), [ways] "r"(ways),
			[setOffset] "n"(SCB_DCCISW_SET_OFFSET),
			[wayOffset] "n"(SCB_DCCISW_WAY_OFFSET)
			: ASM_R0, ASM_R1, ASM_R2);

	return true;
}

/// \brief Disables the data cache.
/// \returns Whether the cache invalidation procedure was executed.
static inline bool
Scb_disableDCache(void)
{
	// cppcheck-suppress misra-c2012-11.4
	static const volatile Scb_Registers *const scb =
			(volatile Scb_Registers *)SCB_BASE_ADDRESS;
	// If the cache is already disabled, don't disable it, as cache clean and invalidation may lead
	// to artefacts.
	if (!Scb_isDCacheEnabled())
		return false;

	const uint32_t ccsidr = scb->ccsidr;
	const uint32_t sets = (ccsidr & SCB_CCSIDR_NUMSETS_MASK)
			>> SCB_CCSIDR_NUMSETS_OFFSET;
	const uint32_t ways = (ccsidr & SCB_CCSIDR_ASSOCIATIVITY_MASK)
			>> SCB_CCSIDR_ASSOCIATIVITY_OFFSET;

	// Assembly below should be the equivalent of this code.
	// This is rewritten to assembly to avoid any instrumentation (e.g. gcov) interference.
	//   asm volatile("dsb");
	//   scb->ccr &= ~SCB_CCR_DC_MASK; <- Disable D-Cache.
	//   for (uint32_t set = sets; set >= 0; set--)
	//     for (uint32_t way = ways; way >= 0; way--)
	//       scb->dccisw = (((set << SCB_DCCISW_SET_OFFSET) & SCB_DCCISW_SET_MASK)
	//                     | ((way << SCB_DCCISW_WAY_OFFSET) & SCB_DCCISW_WAY_MASK));
	//   asm volatile ("dsb");
	//   asm volatile (
	//     "dsb\n"
	//     "isb\n"
	//   );
	const uint32_t newCcr = scb->ccr & ~(uint32_t)SCB_CCR_DC_MASK;
	asm volatile("dsb\n"
		     "str %2, [%1]\n" // CCR = newCcr;
		     "DSETLOOP%=:\n" //
		     "mov r0, %4\n" // r0(way) = ways;
		     "DWAYLOOP%=:\n" //
		     "mov r1, %3\n" // r1 = sets;
		     "lsl r1, %5\n" // r1 = r1 << SCB_DCCISW_SET_OFFSET;
		     "mov r2, r0\n" // r2 = way;
		     "lsl r2, %6\n" // r2 = r2 << SCB_DCCISW_WAY_OFFSET;
		     "orr r1, r2\n" // r1 |= r2;
		     "str r1, [%0]\n" // DCCSW = r1;
		     "sub r0, r0, 1\n" // r1(ways)--;
		     "cmp r0, 0\n" //
		     "bge DWAYLOOP%=\n" // while(way >= 0);
		     "sub %3, %3, 1\n" // r0(sets)--;
		     "cmp %3, 0\n" //
		     "bge DSETLOOP%=\n" // while(set >= 0);
		     "dsb\n"
		     "isb\n"
			:
			: "r"(&(scb->dccisw)), "r"(&(scb->ccr)), "r"(newCcr),
			"r"(sets), "r"(ways), "n"(SCB_DCCISW_SET_OFFSET),
			"n"(SCB_DCCISW_WAY_OFFSET)
			: ASM_R0, ASM_R1, ASM_R2);
	return true;
}

/// \brief Enables or disables the MemoryManagement exception.
/// \param [in] enabled Enable/disable flag.
static inline void
Scb_setMemoryManagementExceptionEnabled(const bool enabled)
{
	// cppcheck-suppress misra-c2012-11.4
	volatile Scb_Registers *const scb =
			(volatile Scb_Registers *)SCB_BASE_ADDRESS;

	if (enabled)
		scb->shcsr = scb->shcsr | SCB_SHCSR_MEMFAULTENA_MASK;
	else
		scb->shcsr = scb->shcsr & ~SCB_SHCSR_MEMFAULTENA_MASK;
}

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // BSP_SCB_H
