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

/// \file Mpu.h
/// \addtogroup Bsp
/// \brief Header containing interface for MPU (Memory Protection Unit) driver.

#ifndef BSP_MPU_H
#define BSP_MPU_H

#include <stdbool.h>
#include <stdint.h>

#include "MpuRegisters.h"

/// @addtogroup Mpu
/// @ingroup Bsp
/// @{

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Access type to the memory region.
typedef enum {
	Mpu_RegionAccess_NoAccess, ///< No access is allowed.
	Mpu_RegionAccess_ReadWrite, ///< Read and write access is allowed.
	Mpu_RegionAccess_ReadOnly, ///< Only read access is allowed.
} Mpu_RegionAccess;

/// \brief Memory region cache policy.
typedef enum {
	Mpu_RegionCachePolicy_NonCacheable = 0, ///< Region is non-cacheable.
	Mpu_RegionCachePolicy_WriteBackReadWriteAllocate =
			1, ///< Write back; read/write allocation.
	Mpu_RegionCachePolicy_WriteThroughNoWriteAllocate =
			2, ///< Write through; no write allocation.
	Mpu_RegionCachePolicy_WriteBackNoWriteAllocate =
			3, ///< Write back; no write allocation.
} Mpu_RegionCachePolicy;

/// \brief Memory region types.
typedef enum {
	Mpu_RegionMemoryType_StronglyOrdered, ///< Strongly ordered memory.
	Mpu_RegionMemoryType_Device, ///< Device memory.
	Mpu_RegionMemoryType_Normal, ///< Normal memory.
} Mpu_RegionMemoryType;

/// \brief Memory region configuration structure.
typedef struct {
	/// \brief The 32-bit base address of the configured region.
	uint32_t address;
	/// \brief Sector enable flag.
	bool isEnabled;
	/// \brief Sector size defined as 2^(size+1).
	uint8_t size;
	/// \brief The mask of 8 subregions disable flags.
	/// \details Each subregion has the size equal to regionSize/8.
	uint8_t subregionDisableMask;
	/// \brief Region shareable flag (ignored for StronglyOrdered memory type).
	bool isShareable;
	/// \brief Is the region data executable flag.
	bool isExecutable;
	/// \brief The type of the memory within the defined region.
	Mpu_RegionMemoryType memoryType;
	/// \brief Inner cache level policy.
	Mpu_RegionCachePolicy innerCachePolicy;
	/// \brief Outer cache level policy.
	Mpu_RegionCachePolicy outerCachePolicy;
	/// \brief Access permissions in privileged mode.
	Mpu_RegionAccess privilegedAccess;
	/// Access permissions in unprivileged mode.
	Mpu_RegionAccess unprivilegedAccess;
} Mpu_RegionConfig;

/// \brief MPU peripheral configuration structure.
typedef struct {
	bool isEnabled; ///< MPU enabled flag.
	bool isDefaultMemoryMapEnabled; ///< Default memory map enable flag.
	bool isMpuEnabledInHandlers; ///< MPU enabled in exception handlers flag.
} Mpu_Config;

/// \brief MPU peripheral descriptor.
typedef struct {
	volatile Mpu_Registers *reg; ///< MPU registers pointer.
} Mpu;

/// \brief Initializes a device descriptor for Mpu.
/// \param [out] mpu Mpu device descriptor.
void Mpu_init(Mpu *const mpu);

/// \brief Configures the Mpu device.
/// \param [in] mpu Mpu device descriptor.
/// \param [in] config Mpu device configuration.
void Mpu_setConfig(Mpu *const mpu, const Mpu_Config *const config);

/// \brief Reads the Mpu device configuration.
/// \param [in] mpu Mpu device descriptor.
/// \param [out] config Mpu device configuration.
void Mpu_getConfig(const Mpu *const mpu, Mpu_Config *const config);

/// \brief Configures the memory region in Mpu device.
/// \param [in] mpu Mpu device descriptor.
/// \param [in] region Memory region number.
/// \param [in] config Memory region configuration.
void Mpu_setRegionConfig(Mpu *const mpu, const uint8_t region,
		const Mpu_RegionConfig *const config);

/// \brief Generates region attribute mask for the given inputs.
/// \param [in] type Memory region type.
/// \param [in] isShareable Is shareable?
/// \param [in] innerCachePolicy Inner cache policy.
/// \param [in] outerCachePolicy Outer cache policy.
/// \param [in] privilegedAccess Access type to the memory region for privileged code.
/// \param [in] unprivilegedAccess Access type to the memory region for unprivileged code.
/// \returns Generated region attribute mask.
uint32_t Mpu_generateAttributes(const Mpu_RegionMemoryType type,
		const bool isShareable,
		const Mpu_RegionCachePolicy innerCachePolicy,
		const Mpu_RegionCachePolicy outerCachePolicy,
		const Mpu_RegionAccess privilegedAccess,
		const Mpu_RegionAccess unprivilegedAccess);

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // BSP_MPU_H
