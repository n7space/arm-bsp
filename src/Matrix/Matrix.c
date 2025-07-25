/**@file
 * This file is part of the ARM BSP for the Test Environment.
 *
 * @copyright 2020-2025 N7 Space Sp. z o.o.
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

#include "Matrix.h"

#include <assert.h>
#include <stddef.h>

#include <Utils/Bits.h>

void
Matrix_init(Matrix *const matrix, Matrix_Registers *const matrixDeviceAddress)
{
	assert(matrix != NULL);
	assert(matrixDeviceAddress != NULL);

	matrix->regs = matrixDeviceAddress;
}

void
Matrix_setMasterConfig(Matrix *const matrix, const Matrix_Master master,
		const Matrix_MasterConfig *const config)
{
	assert(matrix != NULL);
	assert((uint32_t)master < (uint32_t)Matrix_Master_Count);
	assert(config != NULL);

	matrix->regs->mcfg[master] =
			BIT_FIELD_VALUE(MATRIX_MCFG_ULTB, config->burstType);

	changeBitAtOffset(&matrix->regs->mrcr, (uint8_t)master,
			config->isRemappedAddressDecodingEnabled);

	if (config->isErrorIrqEnabled)
		matrix->regs->meier = shiftBitLeft(true, (uint8_t)master);
	else
		matrix->regs->meidr = shiftBitLeft(true, (uint8_t)master);
}

void
Matrix_setSlaveConfig(Matrix *const matrix, const Matrix_Slave slave,
		const Matrix_SlaveConfig *const config)
{
	assert(matrix != NULL);
	assert((uint32_t)slave < (uint32_t)Matrix_Slave_Count);
	assert(config != NULL);

	matrix->regs->scfg[slave] =
			BIT_FIELD_VALUE(MATRIX_SCFG_DEFMSTR_TYPE,
					config->slaveDefaultMasterType)
			| BIT_FIELD_VALUE(MATRIX_SCFG_FIXED_DEFMSTR,
					config->fixedMasterForSlaveSelection)
			| BIT_FIELD_VALUE(MATRIX_SCFG_SLOT_CYCLE,
					config->maximumBusGrantDurationForMasters);

	uint32_t pras = 0u;
	for (uint32_t i = 0u; i < MATRIX_PRXS_COUNT; ++i) {
		pras |= shiftBitLeft(
				config->accessPriority[i].isLatencyQosEnabled,
				(uint8_t)(MATRIX_PRXS_LQOSENX_OFFSET
						+ (MATRIX_PRXS_SHIFT * i)));

		pras |= shiftValueLeft(config->accessPriority[i].masterPriority,
				(uint8_t)(MATRIX_PRXS_MXPR_OFFSET
						+ (MATRIX_PRXS_SHIFT * i)),
				MATRIX_PRXS_MXPR_MASK
						<< (MATRIX_PRXS_SHIFT * i));
	}

	uint32_t prbs = 0u;
	for (uint32_t i = 0u;
			i < ((uint32_t)Matrix_Master_Count - MATRIX_PRXS_COUNT);
			++i) {
		prbs |= shiftBitLeft(
				config->accessPriority[i + MATRIX_PRXS_COUNT]
						.isLatencyQosEnabled,
				(uint8_t)(MATRIX_PRXS_LQOSENX_OFFSET
						+ (MATRIX_PRXS_SHIFT * i)));

		prbs |= shiftValueLeft(
				config->accessPriority[i + MATRIX_PRXS_COUNT]
						.masterPriority,
				(uint8_t)(MATRIX_PRXS_MXPR_OFFSET
						+ (MATRIX_PRXS_SHIFT * i)),
				MATRIX_PRXS_MXPR_MASK
						<< (MATRIX_PRXS_SHIFT * i));
	}

	matrix->regs->prs[slave].pras = pras;
	matrix->regs->prs[slave].prbs = prbs;
}

void
Matrix_getMasterConfig(const Matrix *const matrix, const Matrix_Master master,
		Matrix_MasterConfig *const config)
{
	assert(matrix != NULL);
	assert((uint32_t)master < (uint32_t)Matrix_Master_Count);
	assert(config != NULL);

	config->burstType = (matrix->regs->mcfg[master] & MATRIX_MCFG_ULTB_MASK)
			>> MATRIX_MCFG_ULTB_OFFSET;

	config->isRemappedAddressDecodingEnabled =
			(matrix->regs->mrcr
					& shiftBitLeft(true, (uint8_t)master))
			!= 0u;

	config->isErrorIrqEnabled =
			(matrix->regs->meimr
					& shiftBitLeft(true, (uint8_t)master))
			!= 0u;
}

void
Matrix_getSlaveConfig(const Matrix *const matrix, const Matrix_Slave slave,
		Matrix_SlaveConfig *const config)
{
	assert(matrix != NULL);
	assert((uint32_t)slave < (uint32_t)Matrix_Slave_Count);
	assert(config != NULL);

	const uint32_t scfg = matrix->regs->scfg[slave];
	config->slaveDefaultMasterType = (scfg & MATRIX_SCFG_DEFMSTR_TYPE_MASK)
			>> MATRIX_SCFG_DEFMSTR_TYPE_OFFSET;
	config->fixedMasterForSlaveSelection =
			(scfg & MATRIX_SCFG_FIXED_DEFMSTR_MASK)
			>> MATRIX_SCFG_FIXED_DEFMSTR_OFFSET;
	config->maximumBusGrantDurationForMasters =
			(scfg & MATRIX_SCFG_SLOT_CYCLE_MASK)
			>> MATRIX_SCFG_SLOT_CYCLE_OFFSET;

	const uint32_t pras = matrix->regs->prs[slave].pras;
	for (uint32_t i = 0u; i < MATRIX_PRXS_COUNT; ++i) {
		config->accessPriority[i].isLatencyQosEnabled =
				(pras
						& (MATRIX_PRXS_LQOSENX_MASK << (MATRIX_PRXS_SHIFT
								   * i)))
				!= 0u;

		config->accessPriority[i].masterPriority =
				(pras
						& (MATRIX_PRXS_MXPR_MASK << (MATRIX_PRXS_SHIFT
								   * i)))
				>> (MATRIX_PRXS_MXPR_OFFSET
						+ (MATRIX_PRXS_SHIFT * i));
	}

	const uint32_t prbs = matrix->regs->prs[slave].prbs;
	for (uint32_t i = 0u;
			i < ((uint32_t)Matrix_Master_Count - MATRIX_PRXS_COUNT);
			++i) {
		config->accessPriority[i + MATRIX_PRXS_COUNT]
				.isLatencyQosEnabled =
				(prbs
						& (MATRIX_PRXS_LQOSENX_MASK << (MATRIX_PRXS_SHIFT
								   * i)))
				!= 0u;

		config->accessPriority[i + MATRIX_PRXS_COUNT].masterPriority =
				(prbs
						& (MATRIX_PRXS_MXPR_MASK << (MATRIX_PRXS_SHIFT
								   * i)))
				>> (MATRIX_PRXS_MXPR_OFFSET
						+ (MATRIX_PRXS_SHIFT * i));
	}
}

void
Matrix_getMastersStatuses(const Matrix *const matrix,
		Matrix_MastersStatuses *const statuses)
{
	assert(matrix != NULL);
	assert(statuses != NULL);

	const uint32_t mesr = matrix->regs->mesr;

	for (uint8_t i = 0u; i < (uint8_t)Matrix_Master_Count; ++i) {
		statuses->isMasterIrqTriggered[i] =
				(mesr & shiftBitLeft(true, i)) != 0u;
		statuses->masterErrorAddress[i] = matrix->regs->mear[i];
	}
}

void
Matrix_setWriteProtectionEnabled(Matrix *const matrix, const bool enabled)
{
	assert(matrix != NULL);

	matrix->regs->wpmr = BIT_VALUE(MATRIX_WPMR_WPEN, enabled)
			| BIT_FIELD_VALUE(MATRIX_WPMR_WPKEY,
					MATRIX_WPMR_WPKEY_VALUE);
}

bool
Matrix_isWriteProtectionEnabled(const Matrix *const matrix)
{
	assert(matrix != NULL);

	return (matrix->regs->wpmr & MATRIX_WPMR_WPEN_MASK) != 0u;
}

bool
Matrix_isWriteProtectionViolated(
		const Matrix *const matrix, uint16_t *const offset)
{
	assert(matrix != NULL);

	const uint32_t wpsr = matrix->regs->wpsr;

	if (offset != NULL)
		*offset = (uint16_t)((wpsr & MATRIX_WPSR_WPVSRC_MASK)
				>> MATRIX_WPSR_WPVSRC_OFFSET);

	return (wpsr & MATRIX_WPSR_WPVS_MASK) != 0u;
}

void
Matrix_setSlaveRegionProtectionConfig(Matrix *const matrix,
		const Matrix_Slave slave,
		const Matrix_ProtectedRegionId regionId,
		const Matrix_SlaveRegionProtectionConfig *const config)
{
	assert(matrix != NULL);
	assert((uint32_t)slave < (uint32_t)Matrix_Slave_Count);
	assert((uint32_t)regionId < (uint32_t)Matrix_ProtectedRegionId_Count);
	assert(config != NULL);

	changeBitAtOffset(&matrix->regs->psr[slave],
			(uint8_t)(MATRIX_PSR_WRUSERHX_OFFSET
					+ (uint32_t)regionId),
			config->isPrivilegedRegionUserWriteAllowed);

	changeBitAtOffset(&matrix->regs->psr[slave],
			(uint8_t)(MATRIX_PSR_RDUSERHX_OFFSET
					+ (uint32_t)regionId),
			config->isPrivilegedRegionUserReadAllowed);

	setValueAtOffset(&matrix->regs->psr[slave],
			config->regionSplitOffsetDirection,
			(uint8_t)(MATRIX_PSR_DPSOAX_OFFSET
					+ (uint32_t)regionId),
			MATRIX_PSR_DPSOAX_MASK << (uint32_t)regionId);

	setValueAtOffset(&matrix->regs->psr[slave], config->regionOrder,
			(uint8_t)(MATRIX_PSR_LAUSERHX_OFFSET
					+ (uint32_t)regionId),
			MATRIX_PSR_LAUSERHX_MASK << (uint32_t)regionId);

	setValueAtOffset(&matrix->regs->passr[slave], config->regionSplitOffset,
			(uint8_t)((uint32_t)regionId
					* MATRIX_PASSR_PASPLIT_SHIFT),
			MATRIX_PASSR_PASPLITX_MASK << ((uint32_t)regionId
					* MATRIX_PASSR_PASPLIT_SHIFT));

	setValueAtOffset(&matrix->regs->prtsr[slave],
			config->protectedRegionSize,
			(uint8_t)((uint32_t)regionId
					* MATRIX_PRTSR_PRTOP_SHIFT),
			MATRIX_PRTSR_PRTOPX_MASK << ((uint32_t)regionId
					* MATRIX_PRTSR_PRTOP_SHIFT));
}

void
Matrix_getSlaveRegionProtectionConfig(const Matrix *const matrix,
		const Matrix_Slave slave,
		const Matrix_ProtectedRegionId regionId,
		Matrix_SlaveRegionProtectionConfig *const config)
{
	assert(matrix != NULL);
	assert((uint32_t)slave < (uint32_t)Matrix_Slave_Count);
	assert((uint32_t)regionId < (uint32_t)Matrix_ProtectedRegionId_Count);
	assert(config != NULL);

	const uint32_t psr = matrix->regs->psr[slave];

	config->isPrivilegedRegionUserWriteAllowed =
			(psr & (MATRIX_PSR_WRUSERHX_MASK << (uint32_t)regionId))
			!= 0u;

	config->isPrivilegedRegionUserReadAllowed =
			(psr & (MATRIX_PSR_RDUSERHX_MASK << (uint32_t)regionId))
			!= 0u;

	config->regionSplitOffsetDirection =
			(Matrix_RegionSplitDirection)((psr & (MATRIX_PSR_DPSOAX_MASK << (uint32_t)regionId))
					>> ((uint32_t)regionId
							+ MATRIX_PSR_DPSOAX_OFFSET));

	config->regionOrder =
			(Matrix_RegionSplitOrder)((psr & (MATRIX_PSR_LAUSERHX_MASK << (uint32_t)regionId))
					>> ((uint32_t)regionId
							+ MATRIX_PSR_LAUSERHX_OFFSET));

	const uint32_t passr = matrix->regs->passr[slave];

	config->regionSplitOffset =
			(Matrix_Size)((passr >> ((uint32_t)regionId
						       * MATRIX_PASSR_PASPLIT_SHIFT))
					& MATRIX_PASSR_PASPLITX_MASK);

	const uint32_t prtsr = matrix->regs->prtsr[slave];

	config->protectedRegionSize =
			(Matrix_Size)((prtsr >> ((uint32_t)regionId
						       * MATRIX_PRTSR_PRTOP_SHIFT))
					& MATRIX_PRTSR_PRTOPX_MASK);
}

void
Matrix_setPeripheralProtectionConfig(Matrix *const matrix,
		const Matrix_Peripheral peripheral,
		const Matrix_PeripheralProtectionConfig *const config)
{
	assert(matrix != NULL);
	assert((uint32_t)peripheral < (uint32_t)Matrix_Peripheral_Count);

	const uint32_t registerNumber = (uint32_t)peripheral / 32u;
	const uint8_t bitNumber = (uint8_t)peripheral % 32u;

	setValueAtOffset(&matrix->regs->ppselr[registerNumber],
			config->accessType, bitNumber,
			shiftBitLeft(true, bitNumber));
}

void
Matrix_getPeripheralProtectionConfig(const Matrix *const matrix,
		const Matrix_Peripheral peripheral,
		Matrix_PeripheralProtectionConfig *const config)
{
	assert(matrix != NULL);
	assert((uint32_t)peripheral < (uint32_t)Matrix_Peripheral_Count);

	const uint32_t registerNumber = (uint32_t)peripheral / 32u;
	const uint8_t bitNumber = (uint8_t)peripheral % 32u;

	config->accessType =
			(Matrix_AccessType)((matrix->regs->ppselr[registerNumber]
							    & shiftBitLeft(true,
									    bitNumber))
					>> bitNumber);
}
