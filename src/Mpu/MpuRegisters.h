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

#ifndef BSP_MPUREGISTERS_H
#define BSP_MPUREGISTERS_H

#include <stdint.h>

typedef struct {
	uint32_t type; ///< 0x90 MPU Type Register
	uint32_t ctrl; ///< 0x94 MPU Control Register
	uint32_t rnr; ///< 0x98 MPU Region Number Register
	uint32_t rbar; ///< 0x9C MPU Region Base Address Register
	uint32_t rasr; ///< 0xA0 MPU Attribute and Size Register
	uint32_t rbarA1; ///< 0xA4 MPU Region Base Address Register Alias1
	uint32_t rasrA1; ///< 0xA8 MPU Attribute and Size Register Alias1
	uint32_t rbarA2; ///< 0xAC MPU Region Base Address Register Alias2
	uint32_t rasrA2; ///< 0xB0 MPU Attribute and Size Register Alias2
	uint32_t rbarA3; ///< 0xB4 MPU Region Base Address Register Alias3
	uint32_t rasrA3; ///< 0xB8 MPU Attribute and Size Register Alias3
} Mpu_Registers;

#define MPU_ADDRESS_BASE 0xE000ED90u

#define MPU_TYPE_SEPARATE_MASK 0x00000001u
#define MPU_TYPE_SEPARATE_OFFSET 0u
#define MPU_TYPE_DREGION_MASK 0x0000FF00u
#define MPU_TYPE_DREGION_OFFSET 8u
#define MPU_TYPE_IREGION_MASK 0x00FF0000u
#define MPU_TYPE_IREGION_OFFSET 16u

#define MPU_CTRL_ENABLE_MASK 0x00000001u
#define MPU_CTRL_ENABLE_OFFSET 0u
#define MPU_CTRL_HFNMIENA_ENABLE_MASK 0x00000002u
#define MPU_CTRL_HFNMIENA_ENABLE_OFFSET 1u
#define MPU_CTRL_PRIVDEFENA_ENABLE_MASK 0x00000004u
#define MPU_CTRL_PRIVDEFENA_ENABLE_OFFSET 2u

#define MPU_RNR_REGION_MASK 0x000000FFu
#define MPU_RNR_REGION_OFFSET 0u

#define MPU_RBAR_REGION_MASK 0x0000000Fu
#define MPU_RBAR_REGION_OFFSET 0u
#define MPU_RBAR_VALID_MASK 0x00000010u
#define MPU_RBAR_VALID_OFFSET 4u
#define MPU_RBAR_ADDR_MASK 0xFFFFFFE0u
#define MPU_RBAR_ADDR_OFFSET 5u

#define MPU_RASR_ENABLE_MASK 0x00000001u
#define MPU_RASR_ENABLE_OFFSET 0u
#define MPU_RASR_SIZE_MASK 0x0000003Eu
#define MPU_RASR_SIZE_OFFSET 1u
#define MPU_RASR_SRD_MASK 0x0000FF00u
#define MPU_RASR_SRD_OFFSET 8u
#define MPU_RASR_B_MASK 0x00010000u
#define MPU_RASR_B_OFFSET 16u
#define MPU_RASR_C_MASK 0x00020000u
#define MPU_RASR_C_OFFSET 17u
#define MPU_RASR_S_MASK 0x00040000u
#define MPU_RASR_S_OFFSET 18u
#define MPU_RASR_TEX_MASK 0x00380000u
#define MPU_RASR_TEX_OFFSET 19u
#define MPU_RASR_AP_MASK 0x07000000u
#define MPU_RASR_AP_OFFSET 24u
#define MPU_RASR_XN_MASK 0x10000000u
#define MPU_RASR_XN_OFFSET 28u

#define MPU_RASR_TEX_VALUE_0 (UINT32_C(0) << MPU_RASR_TEX_OFFSET)
#define MPU_RASR_TEX_VALUE_1 (UINT32_C(1) << MPU_RASR_TEX_OFFSET)
#define MPU_RASR_TEX_VALUE_2 (UINT32_C(2) << MPU_RASR_TEX_OFFSET)
#define MPU_RASR_TEX_VALUE_3 (UINT32_C(3) << MPU_RASR_TEX_OFFSET)
#define MPU_RASR_TEX_VALUE_4 (UINT32_C(4) << MPU_RASR_TEX_OFFSET)

#define MPU_RASR_AP_VALUE_0 (UINT32_C(0) << MPU_RASR_AP_OFFSET)
#define MPU_RASR_AP_VALUE_1 (UINT32_C(1) << MPU_RASR_AP_OFFSET)
#define MPU_RASR_AP_VALUE_2 (UINT32_C(2) << MPU_RASR_AP_OFFSET)
#define MPU_RASR_AP_VALUE_3 (UINT32_C(3) << MPU_RASR_AP_OFFSET)
#define MPU_RASR_AP_VALUE_5 (UINT32_C(5) << MPU_RASR_AP_OFFSET)
#define MPU_RASR_AP_VALUE_6 (UINT32_C(6) << MPU_RASR_AP_OFFSET)

#endif // BSP_MPUREGISTERS_H
