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

/// \file NvicRegisters.h
/// \addtogroup Bsp
/// \brief Header containing NVIC-specific register definitions.

#ifndef BSP_NVIC_REGISTERS_H
#define BSP_NVIC_REGISTERS_H

#include <stdint.h>

/// \brief Structure describing the layout of NVIC related registers in the memory.
typedef struct {
	uint32_t iser[8]; /// \brief Interrupt Set-Enable Registers
	uint32_t reserved0[24];
	uint32_t icer[8]; /// \brief Interrupt Clear-Enable Registers
	uint32_t reserved1[24];
	uint32_t ispr[8]; /// \brief Interrupt Set-Pending Registers
	uint32_t reserved2[24];
	uint32_t icpr[8]; /// \brief Interrupt Clear-Pending Registers
	uint32_t reserved3[24];
	uint32_t iabr[8]; /// \brief Interrupt Active Bit Registers
	uint32_t reserved4[56];
	uint8_t ipr[240]; /// \brief Interrupt Priority Registers
	uint32_t reserved5[644];
	uint32_t stir; /// \brief Software Trigger Interrupt Register
} Nvic_Registers;

/// \brief A constant defining the base address of NVIC register space in memory.
#define NVIC_BASE_ADDRESS 0xE000E100u
/// \brief A constant used to fit an interrupt priority variable into a particular register.
#define NVIC_IRQ_PRIORITY_OFFSET 5u
/// \brief A constant defining interrupt register offset for the NVIC operations.
#define NVIC_REGISTERS_OFFSET 5u
/// \brief A constant defining NVIC interrupt lines groups size.
#define NVIC_IRQ_GROUP_SIZE 32u

#endif // BSP_NVIC_REGISTERS_H
