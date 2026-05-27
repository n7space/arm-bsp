/**@file
 * This file is part of the ARM BSP for the Test Environment.
 *
 * @copyright 2020-2026 N7 Space Sp. z o.o.
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

#ifndef N7S_BSP_PIO_SAMRH71F20_PIOREGISTERS_H
#define N7S_BSP_PIO_SAMRH71F20_PIOREGISTERS_H

#include <stdint.h>

/// \brief Structure describing Pio registers.
typedef struct {
  uint32_t mskr;         ///< 0x00 PIO Mask Register
  uint32_t cfgr;         ///< 0x04 PIO Configuration Register
  uint32_t pdsr;         ///< 0x08 PIO Pin Data Status Register
  uint32_t locksr;       ///< 0x0C PIO Lock Status Register
  uint32_t sodr;         ///< 0x10 PIO Set Output Data Register
  uint32_t codr;         ///< 0x14 PIO Clear Output Data Register
  uint32_t odsr;         ///< 0x18 PIO Output Data Status Register
  uint32_t reserved1;    ///< 0x1C Reserved
  uint32_t ier;          ///< 0x20 PIO Interrupt Enable Register
  uint32_t idr;          ///< 0x24 PIO Interrupt Disable Register
  uint32_t imr;          ///< 0x28 PIO Interrupt Mask Register
  uint32_t isr;          ///< 0x2C PIO Interrupt Status Register
  uint32_t reserved2[3]; ///< 0x30 - 0x3B Reserved
  uint32_t iofr;         ///< 0x3C PIO I/O Freeze Configuration Register
} Pio_Registers;

typedef struct {
  uint32_t scdr;          ///< 0x00 Pio Slow Clock Divider Debouncing
  uint32_t reserved1[55]; ///< 0x04 - 0xDF Reserved
  uint32_t wpmr;          ///< 0xE0
  uint32_t wpsr;          ///< 0xE4
} Pio_CommonRegisters;

/// \brief I/O port A register start address.
#define PIOA_ADDRESS_BASE 0x40008000u
/// \brief I/O port B register start address.
#define PIOB_ADDRESS_BASE 0x40008040u
/// \brief I/O port C register start address.
#define PIOC_ADDRESS_BASE 0x40008080u
/// \brief I/O port D register start address.
#define PIOD_ADDRESS_BASE 0x400080C0u

#if defined(N7S_TARGET_SAMRH71F20)
/// \brief I/O port E register start address.
#define PIOE_ADDRESS_BASE 0x40008100u
/// \brief I/O port F register start address.
#define PIOF_ADDRESS_BASE 0x40008140u
/// \brief I/O port G register start address.
#define PIOG_ADDRESS_BASE 0x40008180u
#endif

/// \brief I/O Line Function register offset.
#define PIO_CFGR_FUNC_OFFSET    0u
/// \brief I/O Line Function register mask.
#define PIO_CFGR_FUNC_MASK      0x00000007u
/// \brief Direction register offset.
#define PIO_CFGR_DIR_OFFSET     8u
/// \brief Direction register mask.
#define PIO_CFGR_DIR_MASK       0x00000100u
/// \brief Pull-Up Enable register offset.
#define PIO_CFGR_PUEN_OFFSET    9u
/// \brief Pull-Up Enable register mask.
#define PIO_CFGR_PUEN_MASK      0x00000200u
/// \brief Pull-Down Enable register offset.
#define PIO_CFGR_PDEN_OFFSET    10u
/// \brief Pull-Down Enable register mask.
#define PIO_CFGR_PDEN_MASK      0x00000400u
/// \brief Open Drain register offset.
#define PIO_CFGR_OPD_OFFSET     14u
/// \brief Open Drain register mask.
#define PIO_CFGR_OPD_MASK       0x00004000u
/// \brief Schmitt Trigger register offset.
#define PIO_CFGR_SCHMITT_OFFSET 15u
/// \brief Schmitt Trigger register mask.
#define PIO_CFGR_SCHMITT_MASK   0x00008000u
/// \brief Drive Strength register offset.
#define PIO_CFGR_DRVSTR_OFFSET  16u
/// \brief Drive Strength register mask.
#define PIO_CFGR_DRVSTR_MASK    0x00070000u
/// \brief Event Selection register offset.
#define PIO_CFGR_EVTSEL_OFFSET  24u
/// \brief Event Selection register mask.
#define PIO_CFGR_EVTSEL_MASK    0x07000000u
/// \brief Physical Configuration Freeze Status (read-only) register offset.
#define PIO_CFGR_PCFS_OFFSET    29u
/// \brief Physical Configuration Freeze Status (read-only) register mask.
#define PIO_CFGR_PCFS_MASK      0x20000000u
/// \brief Interrupt Configuration Freeze Status (read-only) register offset.
#define PIO_CFGR_ICFS_OFFSET    0x30u
/// \brief Interrupt Configuration Freeze Status (read-only) register mask.
#define PIO_CFGR_ICFS_MASK      0x40000000u

/// \brief Freeze Physical Configuration register offset.
#define PIO_IOFR_FPHY_OFFSET   0u
/// \brief Freeze Physical Configuration register mask.
#define PIO_IOFR_FPHY_MASK     0x00000001u
/// \brief Freeze Interrupt Configuration register offset.
#define PIO_IOFR_FINT_OFFSET   1u
/// \brief Freeze Interrupt Configuration register mask.
#define PIO_IOFR_FINT_MASK     0x00000002u
/// \brief Freeze Key register offset.
#define PIO_IOFR_FRZKEY_OFFSET 8u
/// \brief Freeze Key register mask.
#define PIO_IOFR_FRSKEY_MASK   0xFFFFFF00u

// Common registers

/// \brief Pio Generic Registers address.
#define PIO_COMMON_REGISTERS_OFFSET 0x00000500u

/// \brief Slow Clock Divider Debouncing register offset.
#define PIO_SCDR_DIV_OFFSET 0u
/// \brief Slow Clock Divider Debouncing register mask.
#define PIO_SCDR_DIV_MASK   0x00003FFFu

/// \brief Write Protection Enable register offset
#define PIO_WPMR_WPEN_OFFSET   0u
/// \brief Write Protection Enable register mask.
#define PIO_WPMR_WPEN_MASK     0x00000001u
/// \brief Write Protection Interrupt Enable register offset.
#define PIO_WPMR_WPITEN_OFFSET 1u
/// \brief Write Protection Interrupt Enable register mask.
#define PIO_WPMR_WPITEN_MASK   0x00000002u
/// \brief Write Protection Key register offset.
#define PIO_WPMR_WPKEY_OFFSET  8u
/// \brief Write Protection Key register mask.
#define PIO_WPMR_WPKEY_MASK    0xFFFFFF00u
/// \brief Write Protection Violation Status register offset.
#define PIO_WPSR_WPVS_OFFSET   0u
/// \brief Write Protection Violation Status register mask.
#define PIO_WPSR_WPVS_MASK     0x00000001u
/// \brief Write Protection Violation Source register offset.
#define PIO_WPSR_WPVSRC_OFFSET 8u
/// \brief Write Protection Violation Source register mask.
#define PIO_WPSR_WPVSRC_MASK   0x00FFFF00u

#endif // N7S_BSP_PIO_SAMRH71F20_PIOREGISTERS_H
