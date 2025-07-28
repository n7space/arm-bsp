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

/// \brief Header containing definitions of registers of SDRAM Controller
/// (SDRAMC).

#ifndef BSP_SDRAMC_REGISTERS_H
#define BSP_SDRAMC_REGISTERS_H

#include <stdbool.h>
#include <stdint.h>

/// \brief Structure representing SDRAMC registers.
typedef struct {
	volatile uint32_t mr; ///< 0x00 Mode register.
	volatile uint32_t tr; ///< 0x04 Refresh Timer register.
	volatile uint32_t cr; ///< 0x08 Configuration register.
	volatile uint32_t reserved1; ///< 0x0C Reserved.
	volatile uint32_t lpr; ///< 0x10 Low-power register.
	volatile uint32_t ier; ///< 0x14 Interrupt Enable register.
	volatile uint32_t idr; ///< 0x18 Interrupt Disable register.
	volatile uint32_t imr; ///< 0x1C Interrupt Mask register.
	volatile uint32_t isr; ///< 0x20 Interrupt Status register.
	volatile uint32_t mdr; ///< 0x24 Memory Device register.
	volatile uint32_t cfr1; ///< 0x28 Configuration register.
	volatile uint32_t ocms; ///< 0x2C OCMS.
	volatile uint32_t ocmsKey1; ///< 0x30 OCMS key 1.
	volatile uint32_t ocmsKey2; ///< 0x34 OCMS key 2.
	volatile uint32_t reserved2[50]; ///< 0x38-0xFC Reserved.
} Sdramc_Registers;

/// \brief Structure representing Bus Matrix (MATRIX) Registers.
///        Only registers relevant to SDRAMC are defined.
typedef struct {
	/// \brief Registers irrevelant to SDRAMC.
	volatile uint32_t notImplemented1[73];
	/// \brief SMC NAND Flash Chip Select Configuration register.
	volatile uint32_t ccfg_smcnfcs;
	/// \brief Registers irrevelant to SDRAMC.
	volatile uint32_t notImplemented2[54];
} Sdramc_MatrixRegisters;

/// \brief Structure representing Cache Control Identification Registers.
///        Only registers relevant to SDRAMC are defined.
typedef struct {
	volatile uint32_t ccsidr; ///< Cache Size ID register.
} Sdramc_CacheControlIdentificationRegisters;

/// \brief Structure representing Cache and Branch Predictor Registers.
///        Only registers relevant to SDRAMC are defined.
typedef struct {
	volatile uint32_t dccisw; ///< D-cache clean and invalidate by set-way.
} Sdramc_CacheAndBranchPredictorRegisters;

/// \brief SDRAMC registers base address.
#define SDRAMC_REGISTERS_ADDRESS_BASE 0x40084000

/// \brief Bus Matrix registers base address.
#define SDRAMC_MATRIX_REGISTER_BASE 0x40088000

// \brief D-cache clean and invalidate by set-way register address.
#define SDRAMC_DCCISW_REGISTER_ADDRESS 0xE000EF74

// \brief Cache Size ID register address.
#define SDRAMC_CCSIDR_REGISTER_ADDRESS 0xE000ED80

/// \brief Number of words in cache line register offset. Value is (log2(line
/// size)) - 2.
#define SDRAMC_CCSIDR_LINESIZE_OFFSET 0u
/// \brief Number of words in cache line register mask.
#define SDRAMC_CCSIDR_LINESIZE_MASK 0x00000007u

/// \brief Associativity of cache register offset. Value is (associativity - 1).
#define SDRAMC_CCSIDR_ASSOCIATIVITY_OFFSET 3u
/// \brief Associativity of cache register mask.
#define SDRAMC_CCSIDR_ASSOCIATIVITY_MASK 0x00001FF8u

/// \brief Number of sets in cache register offset. Value is (number of sets -
/// 1).
#define SDRAMC_CCSIDR_NUMSETS_OFFSET 13u
/// \brief Number of sets in cache register mask
#define SDRAMC_CCSIDR_NUMSETS_MASK 0x0FFFE000u

/// \brief SDRAM address base.
#define SDRAMC_SDRAM_ADRESS_BASE 0x70000000u

/// \brief Command Mode register offset.
#define SDRAMC_MR_MODE_OFFSET 0u
/// \brief Command Mode register mask.
#define SDRAMC_MR_MODE_MASK 0x00000007u

/// \brief Refresh Timer Count register offset.
#define SDRAMC_TR_COUNT_OFFSET 0u
/// \brief Refresh Timer Count register mask.
#define SDRAMC_TR_COUNT_MASK 0x00000FFFu

/// \brief Number of Column Bits register offset.
#define SDRAMC_CR_NC_OFFSET 0u
/// \brief Number of Column Bits register mask.
#define SDRAMC_CR_NC_MASK 0x00000003u

/// \brief Number of Row Bits register offset.
#define SDRAMC_CR_NR_OFFSET 2u
/// \brief Number of Row Bits register mask.
#define SDRAMC_CR_NR_MASK 0x0000000Cu

/// \brief Number of Banks register offset.
#define SDRAMC_CR_NB_OFFSET 4u
/// \brief Number of Banks register mask.
#define SDRAMC_CR_NB_MASK 0x00000010u

/// \brief CAS Latency register offset.
#define SDRAMC_CR_CAS_OFFSET 5u
/// \brief CAS Latency register mask.
#define SDRAMC_CR_CAS_MASK 0x00000060u

/// \brief Data Bus Width  register offset.
#define SDRAMC_CR_DBW_OFFSET 7u
/// \brief Data Bus Width register mask.
#define SDRAMC_CR_DBW_MASK 0x00000080u

/// \brief Write Recovery Delay register offset.
#define SDRAMC_CR_TWR_OFFSET 8u
/// \brief Write Recovery Delay register mask.
#define SDRAMC_CR_TWR_MASK 0x00000F00u

/// \brief Row Cycle Delay and Row Refresh Cycle register offset.
#define SDRAMC_CR_TRC_TRFC_OFFSET 12u
/// \brief Row Cycle Delay and Row Refresh Cycle register mask.
#define SDRAMC_CR_TRC_TRFC_MASK 0x0000F000u

/// \brief Row Precharge Delay register offset.
#define SDRAMC_CR_TRP_OFFSET 16u
/// \brief Row Precharge Delay register mask.
#define SDRAMC_CR_TRP_MASK 0x000F0000u

/// \brief Row to Column Delay register offset.
#define SDRAMC_CR_TRCD_OFFSET 20u
/// \brief Row to Column Delay register mask.
#define SDRAMC_CR_TRCD_MASK 0x00F00000u

/// \brief Active to Precharge Delay register offset.
#define SDRAMC_CR_TRAS_OFFSET 24u
/// \brief Active to Precharge Delay register mask.
#define SDRAMC_CR_TRAS_MASK 0x0F000000u

/// \brief  register offset.
#define SDRAMC_CR_TXSR_OFFSET 28u
/// \brief  register mask.
#define SDRAMC_CR_TXSR_MASK 0xF0000000u

/// \brief Low Power Configuration Bits register offset.
#define SDRAMC_LPR_LPCB_OFFSET 0u
/// \brief Low Power Configuration Bits register mask.
#define SDRAMC_LPR_LPCB_MASK 0x00000003u

/// \brief Partial Array Self-Refresh register offset.
#define SDRAMC_LPR_PASR_OFFSET 4u
/// \brief Partial Array Self-Refresh register mask.
#define SDRAMC_LPR_PASR_MASK 0x00000070u

/// \brief Temperature Compensated Self-Refresh register offset.
#define SDRAMC_LPR_TCSR_OFFSET 8u
/// \brief Temperature Compensated Self-Refresh register mask.
#define SDRAMC_LPR_TCSR_MASK 0x00000300u

/// \brief Drive Strength register offset.
#define SDRAMC_LPR_DS_OFFSET 10u
/// \brief Drive Strength register mask.
#define SDRAMC_LPR_DS_MASK 0x00000C00u

/// \brief Time to Define When Low-power Mode is Enabled register offset.
#define SDRAMC_LPR_TIMEOUT_OFFSET 12u
/// \brief Time to Define When Low-power Mode is Enabled register mask.
#define SDRAMC_LPR_TIMEOUT_MASK 0x00003000u

/// \brief Refresh Error Interrupt Enable register offset.
#define SDRAMC_IER_RES_OFFSET 0u
/// \brief Refresh Error Interrupt Enable register mask.
#define SDRAMC_IER_RES_MASK 0x00000001u

/// \brief Refresh Error Interrupt Disable register offset.
#define SDRAMC_IDR_RES_OFFSET 0u
/// \brief Refresh Error Interrupt Disable register mask.
#define SDRAMC_IDR_RES_MASK 0x00000001u

/// \brief Refresh Error Interrupt Mask register offset.
#define SDRAMC_IMR_RES_OFFSET 0u
/// \brief Refresh Error Interrupt Mask register mask.
#define SDRAMC_IMR_RES_MASK 0x00000001u

/// \brief Refresh Error Status register offset.
#define SDRAMC_ISR_RES_OFFSET 0u
/// \brief Refresh Error Status register mask.
#define SDRAMC_ISR_RES_MASK 0x00000001u

/// \brief Memory Device Type register offset.
#define SDRAMC_MDR_MD_OFFSET 0u
/// \brief Memory Device Type register mask.
#define SDRAMC_MDR_MD_MASK 0x00000003u

/// \brief Load Mode Register Command to Active or Refresh Command register
/// offset.
#define SDRAMC_CFR1_TMRD_OFFSET 0u
/// \brief Load Mode Register Command to Active or Refresh Command register
/// mask.
#define SDRAMC_CFR1_TMRD_MASK 0x0000000Fu

/// \brief Support Unaligned Access register offset.
#define SDRAMC_CFR1_UNAL_OFFSET 8u
/// \brief Support Unaligned Access register mask.
#define SDRAMC_CFR1_UNAL_MASK 0x00000100u

/// \brief SDRAM Memory Controller Scrambling Enable register offset.
#define SDRAMC_OCMS_SDR_SE_OFFSET 0u
/// \brief SDRAM Memory Controller Scrambling Enable register mask.
#define SDRAMC_OCMS_SDR_SE_MASK 0x00000001u

/// \brief Off-chip Memory Scrambling (OCMS) Key Part 1 register offset.
#define SDRAMC_OCMS_KEY1_KEY1_OFFSET 0u
/// \brief Off-chip Memory Scrambling (OCMS) Key Part 1 register mask.
#define SDRAMC_OCMS_KEY1_KEY1_MASK 0xFFFFFFFFu

/// \brief Off-chip Memory Scrambling (OCMS) Key Part 2 register offset.
#define SDRAMC_OCMS_KEY2_KEY2_OFFSET 0u
/// \brief Off-chip Memory Scrambling (OCMS) Key Part 2 register mask.
#define SDRAMC_OCMS_KEY2_KEY2_MASK 0xFFFFFFFFu

/// \brief SDRAM Enable offset.
#define SDRAMC_MATRIX_CCFG_SMCNFCS_SDRAMEN_OFFSET 4u
/// \brief SDRAM Enable mask.
#define SDRAMC_MATRIX_CCFG_SMCNFCS_SDRAMEN_MASK 0x00000010u

#endif // BSP_SDRAMC_REGISTERS_H
