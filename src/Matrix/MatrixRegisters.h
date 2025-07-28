/**@file
 * This file is part of the ARM BSP for the Test Environment.
 *
 * @copyright 2020-2024 N7 Space Sp. z o.o.
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

/// \file MatrixRegisters.h
/// \addtogroup Bsp
/// \brief Header containing definitions of registers of Bus Matrix (MATRIX).

#ifndef BSP_MATRIXREGISTERS_H
#define BSP_MATRIXREGISTERS_H

#include <stdint.h>

/// \brief Structure representing Priority Register for Slave.
typedef struct {
	uint32_t pras; ///< Bus Matrix Priority Register A
	uint32_t prbs; ///< Bus Matrix Priority Register B
} PriorityRegisterForSlave;

/// \brief Structure representing MATRIX registers.
typedef struct {
	uint32_t mcfg[16]; ///< Bus Matrix Master Config Register
	uint32_t scfg[16]; ///< Bus Matrix Slave Config Register
	PriorityRegisterForSlave
			prs[16]; ///< Bus Matrix Priority Register for Slave
	uint32_t mrcr; ///< Bus Matrix Master Remap Control Register
	uint32_t reserved0[19]; ///< Reserved
	uint32_t meier; ///< Master Error Interrupt Enable Register
	uint32_t meidr; ///< Master Error Interrupt Disable Register
	uint32_t meimr; ///< Master Error Interrupt Mask Register
	uint32_t mesr; ///< Master Error Status Register
	uint32_t mear[16]; ///< Master Error Address Register
	uint32_t reserved1[17]; ///< Reserved
	uint32_t wpmr; ///< Write Protection Mode Register
	uint32_t wpsr; ///< Write Protection Status Register
	uint32_t reserved2[5]; ///< Reserved
	uint32_t psr[16]; ///< Protection Slave Register
	uint32_t passr[16]; ///< Protected Areas Split Slave Register
	uint32_t prtsr[16]; ///< Protected Region Top Slave Register
	uint32_t ppselr[3]; ///< Protected Peripheral Select Register
} Matrix_Registers;

#define MATRIX_BASE_ADDRESS 0x40000000u

#define MATRIX_MCFG_ULTB_MASK 0x00000007u
#define MATRIX_MCFG_ULTB_OFFSET 0u

#define MATRIX_SCFG_SLOT_CYCLE_MASK 0x000001FFu
#define MATRIX_SCFG_SLOT_CYCLE_OFFSET 0u
#define MATRIX_SCFG_DEFMSTR_TYPE_MASK 0x00030000u
#define MATRIX_SCFG_DEFMSTR_TYPE_OFFSET 16u
#define MATRIX_SCFG_FIXED_DEFMSTR_MASK 0x003C0000u
#define MATRIX_SCFG_FIXED_DEFMSTR_OFFSET 18u

#define MATRIX_PRXS_MXPR_MASK UINT32_C(0x3)
#define MATRIX_PRXS_MXPR_OFFSET 0u
#define MATRIX_PRXS_LQOSENX_MASK UINT32_C(0x4)
#define MATRIX_PRXS_LQOSENX_OFFSET 2u
#define MATRIX_PRXS_SHIFT 4u
#define MATRIX_PRXS_COUNT 8u

#define MATRIX_WPMR_WPEN_MASK 0x00000001u
#define MATRIX_WPMR_WPEN_OFFSET 0u
#define MATRIX_WPMR_CFGFRZ_MASK 0x00000080u
#define MATRIX_WPMR_CFGFRZ_OFFSET 7u
#define MATRIX_WPMR_WPKEY_MASK 0xFFFFFF00u
#define MATRIX_WPMR_WPKEY_OFFSET 8u
#define MATRIX_WPMR_WPKEY_VALUE 0x4D4154u

#define MATRIX_WPSR_WPVS_MASK 0x00000001u
#define MATRIX_WPSR_WPVS_OFFSET 0u
#define MATRIX_WPSR_WPVSRC_MASK 0x00FFFF00u
#define MATRIX_WPSR_WPVSRC_OFFSET 8u

#define MATRIX_PSR_LAUSERHX_MASK 0x1u
#define MATRIX_PSR_LAUSERHX_OFFSET 0u
#define MATRIX_PSR_RDUSERHX_MASK 0x100u
#define MATRIX_PSR_RDUSERHX_OFFSET 8u
#define MATRIX_PSR_WRUSERHX_MASK 0x10000u
#define MATRIX_PSR_WRUSERHX_OFFSET 16u
#define MATRIX_PSR_DPSOAX_MASK 0x1000000u
#define MATRIX_PSR_DPSOAX_OFFSET 24u

#define MATRIX_PASSR_PASPLITX_MASK 0xFu
#define MATRIX_PASSR_PASPLIT_SHIFT 4u
#define MATRIX_PASSR_PASPLIT_COUNT 8u

#define MATRIX_PRTSR_PRTOPX_MASK 0xFu
#define MATRIX_PRTSR_PRTOP_SHIFT 4u
#define MATRIX_PRTSR_PRTOP_COUNT 8u

#endif // BSP_MATRIXREGISTERS_H
