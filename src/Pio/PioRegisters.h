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

#ifndef BSP_PIO_REGISTERS_H
#define BSP_PIO_REGISTERS_H

#include <stdint.h>

/// \brief Structure describing Pio registers.
typedef struct {
	uint32_t per; ///< 0x00 PIO Enable Register
	uint32_t pdr; ///< 0x04 PIO Disable Register
	uint32_t psr; ///< 0x08 PIO Status Register
	uint32_t reserved1; ///< 0x0C Reserved
	uint32_t oer; ///< 0x10 Output Enable Register
	uint32_t odr; ///< 0x14 Output Disable Register
	uint32_t osr; ///< 0x18 Output Status Register
	uint32_t reserved2; ///< 0x1C Reserved
	uint32_t ifer; ///< 0x20 Glitch Input Filter Enable Register
	uint32_t ifdr; ///< 0x24 Glitch Input Filter Disable Register
	uint32_t ifsr; ///< 0x28 Glitch Input Filter Status Register
	uint32_t reserved3; ///< 0x2C Reserved
	uint32_t sodr; ///< 0x30 Set Output Data Register
	uint32_t codr; ///< 0x34 Clear Output Data Register
	uint32_t odsr; ///< 0x38 Output Data Status Register
	uint32_t pdsr; ///< 0x3C Pin Data Status Register
	uint32_t ier; ///< 0x40 Interrupt Enable Register
	uint32_t idr; ///< 0x44 Interrupt Disable Register
	uint32_t imr; ///< 0x48 Interrupt Mask Register
	uint32_t isr; ///< 0x4C Interrupt Status Register
	uint32_t mder; ///< 0x50 PIO Enable Register
	uint32_t mddr; ///< 0x54 PIO Disable Register
	uint32_t mdsr; ///< 0x58 PIO Status Register
	uint32_t reserved4; ///< 0x5C Reserved
	uint32_t pudr; ///< 0x60 Pull-Up Disable Register
	uint32_t puer; ///< 0x64 Pull-Up Enable Register
	uint32_t pusr; ///< 0x68 Pad Pull-Up Status Register
	uint32_t reserved5; ///< 0x6C Reserved
	uint32_t abcdsr1; ///< 0x70 Peripheral ABCD Select Register 1
	uint32_t abcdsr2; ///< 0x74 Peripheral ABCD Select Register 2
	uint32_t reserved6[2]; ///< 0x78 - 0x7C Reserved
	uint32_t ifscdr; ///< 0x80 Input Filter Slow Clock Disable Register
	uint32_t ifscer; ///< 0x84 Input Filter Slow Clock Enable Register
	uint32_t ifscsr; ///< 0x88 Input Filter Slow Clock Status Register
	uint32_t scdr; ///< 0x8C Slow Clock Divider Debouncing Register
	uint32_t ppddr; ///< 0x90 Pad Pull-Down Disable Register
	uint32_t ppder; ///< 0x94 Pad Pull-Down Enable Register
	uint32_t ppdsr; ///< 0x98 Pad Pull-Down Status Register
	uint32_t reserved7; ///< 0x9C Reserved
	uint32_t ower; ///< 0xA0 Output Write Enable Register
	uint32_t owdr; ///< 0xA4 Output Write Disable Register
	uint32_t owsr; ///< 0xA8 Output Write Status Register
	uint32_t reserved8; ///< 0xAC Reserved
	uint32_t aimer; ///< 0xB0 Additional Interrupt Modes Enable Register
	uint32_t aimdr; ///< 0xB4 Additional Interrupt Modes Disable Register
	uint32_t aimmr; ///< 0xB8 Additional Interrupt Modes Mask Register
	uint32_t reserved9; ///< 0xBC Reserved
	uint32_t esr; ///< 0xC0 Edge Select Register
	uint32_t lsr; ///< 0xC4 Level Select Register
	uint32_t elsr; ///< 0xC8 Edge/Level Status Register
	uint32_t reserved10; ///< 0xCC Reserved
	uint32_t fellsr; ///< 0xD0 Edge/Level Status Register
	uint32_t rehlsr; ///< 0xD4 Rising Edge/High-Level Select Register
	uint32_t frlhsr; ///< 0xD8 Fall/Rise - Low/High Status Register
	uint32_t reserved11; ///< 0xDC Reserved
	uint32_t locksr; ///< 0xE0 Lock Status
	uint32_t wpmr; ///< 0xE4 Write Protection Mode Register
	uint32_t wpsr; ///< 0xE8 Write Protection Status Register
	uint32_t reserved12[5]; ///< 0xEC - 0xFC Reserved
	uint32_t schmitt; ///< 0x100 Schmitt Trigger Register
	uint32_t reserved13[5]; ///< 0x104 - 0x114 Reserved
	uint32_t driver; ///< 0x118 I/O Drive Register
	uint32_t reserved14[13]; ///< 0x11C - 0x14C Reserved
	uint32_t pcmr; ///< 0x150 Parallel Capture Mode Register
	uint32_t pcier; ///< 0x154 Parallel Capture Interrupt Enable Register
	uint32_t pcidr; ///< 0x158 Parallel Capture Interrupt Disable Register
	uint32_t pcimr; ///< 0x15C Parallel Capture Interrupt Mask Register
	uint32_t pcisr; ///< 0x160 Parallel Capture Interrupt Status Register
	uint32_t pcrhr; ///< 0x164 Parallel Capture Interrupt Status Register
} Pio_Registers;

/// \brief I/O port A register start address.
#define PIOA_ADDRESS_BASE 0x400E0E00
/// \brief I/O port B register start address.
#define PIOB_ADDRESS_BASE 0x400E1000
/// \brief I/O port C register start address.
#define PIOC_ADDRESS_BASE 0x400E1200
/// \brief I/O port D register start address.
#define PIOD_ADDRESS_BASE 0x400E1400
/// \brief I/O port E register start address.
#define PIOE_ADDRESS_BASE 0x400E1600

/// \brief Slow Clock Divider Debouncing register offset.
#define PIO_SCDR_DIV_OFFSET 0u
/// \brief Slow Clock Divider Debouncing register mask.
#define PIO_SCDR_DIV_MASK 0x00003FFFu

#endif // BSP_PIO_REGISTERS_H
