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

#include "Mpu.h"

#include <assert.h>

#include <Utils/Bits.h>

#include <bsp/arm/Scb/Scb.h>

#define MPU_REGIONS_COUNT 16u

void
Mpu_init(Mpu *const mpu)
{
	// cppcheck-suppress misra-c2012-11.4
	mpu->reg = (Mpu_Registers *)MPU_ADDRESS_BASE;
}

void
Mpu_setConfig(Mpu *const mpu, const Mpu_Config *const config)
{
	uint32_t ctrl = 0;

	if (config->isEnabled) {
		ctrl |= MPU_CTRL_ENABLE_MASK;
		if (config->isDefaultMemoryMapEnabled)
			ctrl |= MPU_CTRL_PRIVDEFENA_ENABLE_MASK;
		if (config->isMpuEnabledInHandlers)
			ctrl |= MPU_CTRL_HFNMIENA_ENABLE_MASK;
	}

	mpu->reg->ctrl = ctrl;

	MEMORY_SYNC_BARRIER();
}

void
Mpu_getConfig(const Mpu *const mpu, Mpu_Config *const config)
{
	uint32_t ctrl = mpu->reg->ctrl;

	config->isEnabled =
			(ctrl & MPU_CTRL_ENABLE_MASK) == MPU_CTRL_ENABLE_MASK;
	config->isMpuEnabledInHandlers = (ctrl & MPU_CTRL_HFNMIENA_ENABLE_MASK)
			== MPU_CTRL_HFNMIENA_ENABLE_MASK;
	config->isDefaultMemoryMapEnabled =
			(ctrl & MPU_CTRL_PRIVDEFENA_ENABLE_MASK)
			== MPU_CTRL_PRIVDEFENA_ENABLE_MASK;
}

// This function sets the attributes according to the Table B3-15 in the
// ARM®v7-M Architecture Reference Manual
static inline uint32_t
generateAccessPermissionsAttrs(const Mpu_RegionAccess privilegedAccess,
		const Mpu_RegionAccess unprivilegedAccess)
{
	if (privilegedAccess == Mpu_RegionAccess_ReadWrite) {
		if (unprivilegedAccess == Mpu_RegionAccess_ReadWrite)
			return MPU_RASR_AP_VALUE_3;
		if (unprivilegedAccess == Mpu_RegionAccess_ReadOnly)
			return MPU_RASR_AP_VALUE_2;
		assert(unprivilegedAccess == Mpu_RegionAccess_NoAccess);
		return MPU_RASR_AP_VALUE_1;
	}
	if (privilegedAccess == Mpu_RegionAccess_ReadOnly) {
		if (unprivilegedAccess == Mpu_RegionAccess_ReadOnly)
			return MPU_RASR_AP_VALUE_6;
		assert(unprivilegedAccess == Mpu_RegionAccess_NoAccess);
		return MPU_RASR_AP_VALUE_5;
	}
	assert(privilegedAccess == Mpu_RegionAccess_NoAccess);
	assert(unprivilegedAccess == Mpu_RegionAccess_NoAccess);
	return MPU_RASR_AP_VALUE_0;
}

// This function sets the attributes according to the Table B3-13 in the
// ARM®v7-M Architecture Reference Manual
static inline uint32_t
generateStronglyOrderedMemoryAttrs(void)
{
	return MPU_RASR_S_MASK;
}

// This function sets the attributes according to the Table B3-13 in the
// ARM®v7-M Architecture Reference Manual
static inline uint32_t
generateDeviceMemoryAttrs(const bool isShareable)
{
	if (isShareable)
		return MPU_RASR_S_MASK | MPU_RASR_B_MASK;
	return MPU_RASR_TEX_VALUE_2;
}

// This function sets the attributes according to the Tables B3-13 and B3-14 in the
// ARM®v7-M Architecture Reference Manual
static inline uint32_t
generateNormalMemoryAttrs(const bool isShareable,
		const Mpu_RegionCachePolicy innerCachePolicy,
		const Mpu_RegionCachePolicy outerCachePolicy)
{
	uint32_t attrs = 0;
	if (isShareable)
		attrs |= MPU_RASR_S_MASK;
	if (innerCachePolicy == outerCachePolicy) {
		switch (innerCachePolicy) {
		case Mpu_RegionCachePolicy_NonCacheable:
			attrs |= MPU_RASR_TEX_VALUE_1;
			break;
		case Mpu_RegionCachePolicy_WriteBackReadWriteAllocate:
			attrs |= MPU_RASR_TEX_VALUE_1;
			attrs |= MPU_RASR_C_MASK;
			attrs |= MPU_RASR_B_MASK;
			break;
		case Mpu_RegionCachePolicy_WriteThroughNoWriteAllocate:
			attrs |= MPU_RASR_C_MASK;
			break;
		case Mpu_RegionCachePolicy_WriteBackNoWriteAllocate:
			attrs |= MPU_RASR_C_MASK;
			attrs |= MPU_RASR_B_MASK;
			break;
		}
	} else {
		attrs |= MPU_RASR_TEX_VALUE_4
				| ((uint32_t)outerCachePolicy
						<< MPU_RASR_TEX_OFFSET);
		attrs |= (uint32_t)innerCachePolicy << MPU_RASR_B_OFFSET;
	}
	return attrs;
}

uint32_t
Mpu_generateAttributes(const Mpu_RegionMemoryType type, const bool isShareable,
		const Mpu_RegionCachePolicy innerCachePolicy,
		const Mpu_RegionCachePolicy outerCachePolicy,
		const Mpu_RegionAccess privilegedAccess,
		const Mpu_RegionAccess unprivilegedAccess)
{
	uint32_t attrs = 0;
	switch (type) {
	case Mpu_RegionMemoryType_StronglyOrdered:
		attrs |= generateStronglyOrderedMemoryAttrs();
		break;
	case Mpu_RegionMemoryType_Device:
		attrs |= generateDeviceMemoryAttrs(isShareable);
		break;
	case Mpu_RegionMemoryType_Normal:
		attrs |= generateNormalMemoryAttrs(isShareable,
				innerCachePolicy, outerCachePolicy);
		break;
	default: assert(false && "Bad Mpu Region Memory Type."); return attrs;
	}

	attrs |= generateAccessPermissionsAttrs(
			privilegedAccess, unprivilegedAccess);

	return attrs;
}

void
Mpu_setRegionConfig(Mpu *const mpu, const uint8_t region,
		const Mpu_RegionConfig *const config)
{
	assert((uint32_t)region < MPU_REGIONS_COUNT);

	uint32_t rbar = 0;
	uint32_t rasr = 0;

	if (config->isEnabled) {
		rbar = config->address & MPU_RBAR_ADDR_MASK;

		rasr |= MPU_RASR_ENABLE_MASK;
		rasr |= BIT_FIELD_VALUE(MPU_RASR_SIZE, config->size);
		rasr |= BIT_FIELD_VALUE(
				MPU_RASR_SRD, config->subregionDisableMask);

		if (!config->isExecutable)
			rasr |= MPU_RASR_XN_MASK;
		rasr |= Mpu_generateAttributes(config->memoryType,
				config->isShareable, config->innerCachePolicy,
				config->outerCachePolicy,
				config->privilegedAccess,
				config->unprivilegedAccess);
	}

	// ensure all memory operations are flushed before changing region config
	MEMORY_SYNC_BARRIER();

	mpu->reg->rnr = BIT_FIELD_VALUE(MPU_RNR_REGION, region);

	// disable the region before modifying it (according to ARM manual)
	changeBitAtOffset(&mpu->reg->rasr, MPU_RASR_ENABLE_OFFSET, false);

	mpu->reg->rbar = rbar;
	mpu->reg->rasr = rasr;

	MEMORY_SYNC_BARRIER();
}
