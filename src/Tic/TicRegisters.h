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

/// \file TicRegisters.h
/// \addtogroup Bsp
/// \brief Header containing definitions of registers of Timer Controller - TC (TIC).

#ifndef BSP_TIC_REGISTERS_H
#define BSP_TIC_REGISTERS_H

#include <stdbool.h>
#include <stdint.h>

/// \brief Structure describing TC Channel registers.
typedef struct {
	uint32_t ccr; ///< Channel Control Register.
	uint32_t cmr; ///< Channel Mode Register.
	uint32_t smmr; ///< Stepper Motor Mode Register.
	uint32_t rab; ///< Register AB.
	uint32_t cv; ///< Counter Value Register.
	uint32_t ra; ///< Register A.
	uint32_t rb; ///< Register B.
	uint32_t rc; ///< Register C.
	uint32_t sr; ///< Status Register.
	uint32_t ier; ///< Interrupt Enable Register.
	uint32_t idr; ///< Interrupt Disable Register.
	uint32_t imr; ///< Interrupt Mask Register.
	uint32_t emr; ///< Extended Mode Register.
	uint32_t reserved[3]; ///< Reserved.
} Tic_ChannelRegisters;

/// \brief Structure describing TC registers.
typedef struct {
	Tic_ChannelRegisters channelRegs[3];
	uint32_t bcr; ///< Block Control Register.
	uint32_t bmr; ///< Block Mode Register.
	uint32_t qier; ///< QDEC Interrupt Enable Register.
	uint32_t qidr; ///< QDEC Interrupt Disable Register.
	uint32_t qimr; ///< QDEC Interrupt Mask Register.
	uint32_t qisr; ///< QDEC Interrupt Status Register.
	uint32_t fmr; ///< Fault Mode Register.
	uint32_t reserved0[2]; ///< Reserved.
	uint32_t wpmr; ///< Write Protection Mode Register.
	uint32_t reserved1[6]; ///< Reserved.
} Tic_Registers;

// clang-format off

#if defined(N7S_TARGET_SAMV71Q21)
#define TIC_TIC0_BASE_ADDRESS    0x4000C000u
#define TIC_TIC1_BASE_ADDRESS    0x40010000u
#define TIC_TIC2_BASE_ADDRESS    0x40014000u
#define TIC_TIC3_BASE_ADDRESS    0x40054000u
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define TIC_TIC0_BASE_ADDRESS    0x40070000U
#define TIC_TIC1_BASE_ADDRESS    0x40074000U
#define TIC_TIC2_BASE_ADDRESS    0x40078000U
#if defined(N7S_TARGET_SAMRH71F20)
#define TIC_TIC3_BASE_ADDRESS    0x4007C000U
#endif
#else
#error "No target platform specified (missing N7S_TARGET_* macro)"
#endif

#define TIC_WPMR_WPKEY_PASSWD_VALUE     0x54494Du

#define TIC_CCR_CLKEN_MASK              0x00000001u
#define TIC_CCR_CLKEN_OFFSET            0u
#define TIC_CCR_CLKDIS_MASK             0x00000002u
#define TIC_CCR_CLKDIS_OFFSET           1u
#define TIC_CCR_SWTRG_MASK              0x00000004u
#define TIC_CCR_SWTRG_OFFSET            2u

#define TIC_CMR_CAP_TCCLKS_MASK         0x00000007u
#define TIC_CMR_CAP_TCCLKS_OFFSET       0u
#define TIC_CMR_CAP_CLKI_MASK           0x00000008u
#define TIC_CMR_CAP_CLKI_OFFSET         3u
#define TIC_CMR_CAP_BURST_MASK          0x00000030u
#define TIC_CMR_CAP_BURST_OFFSET        4u
#define TIC_CMR_CAP_LDBSTOP_MASK        0x00000040u
#define TIC_CMR_CAP_LDBSTOP_OFFSET      6u
#define TIC_CMR_CAP_LDBDIS_MASK         0x00000080u
#define TIC_CMR_CAP_LDBDIS_OFFSET       7u
#define TIC_CMR_CAP_ETRGEDG_MASK        0x00000300u
#define TIC_CMR_CAP_ETRGEDG_OFFSET      8u
#define TIC_CMR_CAP_ABETRG_MASK         0x00000400u
#define TIC_CMR_CAP_ABETRG_OFFSET       10u
#define TIC_CMR_CAP_CPCTRG_MASK         0x00004000u
#define TIC_CMR_CAP_CPCTRG_OFFSET       14u
#define TIC_CMR_CAP_WAVE_MASK           0x00008000u
#define TIC_CMR_CAP_WAVE_OFFSET         15u
#define TIC_CMR_CAP_LDRA_MASK           0x00030000u
#define TIC_CMR_CAP_LDRA_OFFSET         16u
#define TIC_CMR_CAP_LDRB_MASK           0x000C0000u
#define TIC_CMR_CAP_LDRB_OFFSET         18u
#define TIC_CMR_CAP_SBSMPLR_MASK        0x00700000u
#define TIC_CMR_CAP_SBSMPLR_OFFSET      20u

#define TIC_CMR_WVF_TCCLKS_MASK         0x00000007u
#define TIC_CMR_WVF_TCCLKS_OFFSET       0u
#define TIC_CMR_WVF_CLKI_MASK           0x00000008u
#define TIC_CMR_WVF_CLKI_OFFSET         3u
#define TIC_CMR_WVF_BURST_MASK          0x00000030u
#define TIC_CMR_WVF_BURST_OFFSET        4u
#define TIC_CMR_WVF_CPCSTOP_MASK        0x00000040u
#define TIC_CMR_WVF_CPCSTOP_OFFSET      6u
#define TIC_CMR_WVF_CPCDIS_MASK         0x00000080u
#define TIC_CMR_WVF_CPCDIS_OFFSET       7u
#define TIC_CMR_WVF_EEVTEDG_MASK        0x00000300u
#define TIC_CMR_WVF_EEVTEDG_OFFSET      8u
#define TIC_CMR_WVF_EEVT_MASK           0x00000C00u
#define TIC_CMR_WVF_EEVT_OFFSET         10u
#define TIC_CMR_WVF_ENETRG_MASK         0x00001000u
#define TIC_CMR_WVF_ENETRG_OFFSET       12u
#define TIC_CMR_WVF_WAVSEL_MASK         0x00006000u
#define TIC_CMR_WVF_WAVSEL_OFFSET       13u
#define TIC_CMR_WVF_WAVE_MASK           0x00008000u
#define TIC_CMR_WVF_WAVE_OFFSET         15u
#define TIC_CMR_WVF_ACPA_MASK           0x00030000u
#define TIC_CMR_WVF_ACPA_OFFSET         16u
#define TIC_CMR_WVF_ACPC_MASK           0x000C0000u
#define TIC_CMR_WVF_ACPC_OFFSET         18u
#define TIC_CMR_WVF_AEEVT_MASK          0x00300000u
#define TIC_CMR_WVF_AEEVT_OFFSET        20u
#define TIC_CMR_WVF_ASWTRG_MASK         0x00C00000u
#define TIC_CMR_WVF_ASWTRG_OFFSET       22u
#define TIC_CMR_WVF_BCPB_MASK           0x03000000u
#define TIC_CMR_WVF_BCPB_OFFSET         24u
#define TIC_CMR_WVF_BCPC_MASK           0x0C000000u
#define TIC_CMR_WVF_BCPC_OFFSET         26u
#define TIC_CMR_WVF_BEEVT_MASK          0x30000000u
#define TIC_CMR_WVF_BEEVT_OFFSET        28u
#define TIC_CMR_WVF_BSWTRG_MASK         0xC0000000u
#define TIC_CMR_WVF_BSWTRG_OFFSET       30u

#define TIC_SMMR_GCEN_MASK              0x00000001u
#define TIC_SMMR_GCEN_OFFSET            0u
#define TIC_SMMR_DOWN_MASK              0x00000002u
#define TIC_SMMR_DOWN_OFFSET            1u

#define TIC_RAB_AB_MASK                 0xFFFFFFFFu
#define TIC_RAB_AB_OFFSET               0u

#define TIC_CV_CV_MASK                  0xFFFFFFFFu
#define TIC_CV_CV_OFFSET                0u

#define TIC_RA_RA_MASK                  0xFFFFFFFFu
#define TIC_RA_RA_OFFSET                0u

#define TIC_RB_RB_MASK                  0xFFFFFFFFu
#define TIC_RB_RB_OFFSET                0u

#define TIC_RC_RC_MASK                  0xFFFFFFFFu
#define TIC_RC_RC_OFFSET                0u

#define TIC_SR_COVFS_MASK               0x00000001u
#define TIC_SR_COVFS_OFFSET             0u
#define TIC_SR_LOVRS_MASK               0x00000002u
#define TIC_SR_LOVRS_OFFSET             1u
#define TIC_SR_CPAS_MASK                0x00000004u
#define TIC_SR_CPAS_OFFSET              2u
#define TIC_SR_CPBS_MASK                0x00000008u
#define TIC_SR_CPBS_OFFSET              3u
#define TIC_SR_CPCS_MASK                0x00000010u
#define TIC_SR_CPCS_OFFSET              4u
#define TIC_SR_LDRAS_MASK               0x00000020u
#define TIC_SR_LDRAS_OFFSET             5u
#define TIC_SR_LDRBS_MASK               0x00000040u
#define TIC_SR_LDRBS_OFFSET             6u
#define TIC_SR_ETRGS_MASK               0x00000080u
#define TIC_SR_ETRGS_OFFSET             7u
#define TIC_SR_CLKSTA_MASK              0x00010000u
#define TIC_SR_CLKSTA_OFFSET            16u
#define TIC_SR_MTIOA_MASK               0x00020000u
#define TIC_SR_MTIOA_OFFSET             17u
#define TIC_SR_MTIOB_MASK               0x00040000u
#define TIC_SR_MTIOB_OFFSET             18u

#define TIC_IER_COVFS_MASK              0x00000001u
#define TIC_IER_COVFS_OFFSET            0u
#define TIC_IER_LOVRS_MASK              0x00000002u
#define TIC_IER_LOVRS_OFFSET            1u
#define TIC_IER_CPAS_MASK               0x00000004u
#define TIC_IER_CPAS_OFFSET             2u
#define TIC_IER_CPBS_MASK               0x00000008u
#define TIC_IER_CPBS_OFFSET             3u
#define TIC_IER_CPCS_MASK               0x00000010u
#define TIC_IER_CPCS_OFFSET             4u
#define TIC_IER_LDRAS_MASK              0x00000020u
#define TIC_IER_LDRAS_OFFSET            5u
#define TIC_IER_LDRBS_MASK              0x00000040u
#define TIC_IER_LDRBS_OFFSET            6u
#define TIC_IER_ETRGS_MASK              0x00000080u
#define TIC_IER_ETRGS_OFFSET            7u

#define TIC_IDR_COVFS_MASK              0x00000001u
#define TIC_IDR_COVFS_OFFSET            0u
#define TIC_IDR_LOVRS_MASK              0x00000002u
#define TIC_IDR_LOVRS_OFFSET            1u
#define TIC_IDR_CPAS_MASK               0x00000004u
#define TIC_IDR_CPAS_OFFSET             2u
#define TIC_IDR_CPBS_MASK               0x00000008u
#define TIC_IDR_CPBS_OFFSET             3u
#define TIC_IDR_CPCS_MASK               0x00000010u
#define TIC_IDR_CPCS_OFFSET             4u
#define TIC_IDR_LDRAS_MASK              0x00000020u
#define TIC_IDR_LDRAS_OFFSET            5u
#define TIC_IDR_LDRBS_MASK              0x00000040u
#define TIC_IDR_LDRBS_OFFSET            6u
#define TIC_IDR_ETRGS_MASK              0x00000080u
#define TIC_IDR_ETRGS_OFFSET            7u

#define TIC_IMR_COVFS_MASK              0x00000001u
#define TIC_IMR_COVFS_OFFSET            0u
#define TIC_IMR_LOVRS_MASK              0x00000002u
#define TIC_IMR_LOVRS_OFFSET            1u
#define TIC_IMR_CPAS_MASK               0x00000004u
#define TIC_IMR_CPAS_OFFSET             2u
#define TIC_IMR_CPBS_MASK               0x00000008u
#define TIC_IMR_CPBS_OFFSET             3u
#define TIC_IMR_CPCS_MASK               0x00000010u
#define TIC_IMR_CPCS_OFFSET             4u
#define TIC_IMR_LDRAS_MASK              0x00000020u
#define TIC_IMR_LDRAS_OFFSET            5u
#define TIC_IMR_LDRBS_MASK              0x00000040u
#define TIC_IMR_LDRBS_OFFSET            6u
#define TIC_IMR_ETRGS_MASK              0x00000080u
#define TIC_IMR_ETRGS_OFFSET            7u

#define TIC_EMR_TRIGSRCA_MASK           0x00000003u
#define TIC_EMR_TRIGSRCA_OFFSET         0u
#define TIC_EMR_TRIGSRCB_MASK           0x00000030u
#define TIC_EMR_TRIGSRCB_OFFSET         4u
#define TIC_EMR_NODIVCLK_MASK           0x00000100u
#define TIC_EMR_NODIVCLK_OFFSET         8u

#define TIC_BCR_SYNC_MASK               0x00000001u
#define TIC_BCR_SYNC_OFFSET             0u

#define TIC_BMR_TC0XC0S_MASK            0x00000003u
#define TIC_BMR_TC0XC0S_OFFSET          0u
#define TIC_BMR_TC1XC1S_MASK            0x0000000Cu
#define TIC_BMR_TC1XC1S_OFFSET          2u
#define TIC_BMR_TC2XC2S_MASK            0x00000030u
#define TIC_BMR_TC2XC2S_OFFSET          4u
#define TIC_BMR_QDEN_MASK               0x00000100u
#define TIC_BMR_QDEN_OFFSET             8u
#define TIC_BMR_POSEN_MASK              0x00000200u
#define TIC_BMR_POSEN_OFFSET            9u
#define TIC_BMR_SPEEDEN_MASK            0x00000400u
#define TIC_BMR_SPEEDEN_OFFSET          10u
#define TIC_BMR_QDTRANS_MASK            0x00000800u
#define TIC_BMR_QDTRANS_OFFSET          11u
#define TIC_BMR_EDGPHA_MASK             0x00001000u
#define TIC_BMR_EDGPHA_OFFSET           12u
#define TIC_BMR_INVA_MASK               0x00002000u
#define TIC_BMR_INVA_OFFSET             13u
#define TIC_BMR_INVB_MASK               0x00004000u
#define TIC_BMR_INVB_OFFSET             14u
#define TIC_BMR_INVIDX_MASK             0x00008000u
#define TIC_BMR_INVIDX_OFFSET           15u
#define TIC_BMR_SWAP_MASK               0x00010000u
#define TIC_BMR_SWAP_OFFSET             16u
#define TIC_BMR_IDXPHB_MASK             0x00020000u
#define TIC_BMR_IDXPHB_OFFSET           17u
#define TIC_BMR_AUTOC_MASK              0x00040000u
#define TIC_BMR_AUTOC_OFFSET            18u
#define TIC_BMR_MAXFILT_MASK            0x03F00000u
#define TIC_BMR_MAXFILT_OFFSET          20u
#define TIC_BMR_MAXCMP_MASK             0x3C000000u
#define TIC_BMR_MAXCMP_OFFSET           26u

#define TIC_QIER_IDX_MASK               0x00000001u
#define TIC_QIER_IDX_OFFSET             0u
#define TIC_QIER_DIRCHG_MASK            0x00000002u
#define TIC_QIER_DIRCHG_OFFSET          1u
#define TIC_QIER_QERR_MASK              0x00000004u
#define TIC_QIER_QERR_OFFSET            2u
#define TIC_QIER_MPE_MASK               0x00000008u
#define TIC_QIER_MPE_OFFSET             3u

#define TIC_QIDR_IDX_MASK               0x00000001u
#define TIC_QIDR_IDX_OFFSET             0u
#define TIC_QIDR_DIRCHG_MASK            0x00000002u
#define TIC_QIDR_DIRCHG_OFFSET          1u
#define TIC_QIDR_QERR_MASK              0x00000004u
#define TIC_QIDR_QERR_OFFSET            2u
#define TIC_QIDR_MPE_MASK               0x00000008u
#define TIC_QIDR_MPE_OFFSET             3u

#define TIC_QIMR_IDX_MASK               0x00000001u
#define TIC_QIMR_IDX_OFFSET             0u
#define TIC_QIMR_DIRCHG_MASK            0x00000002u
#define TIC_QIMR_DIRCHG_OFFSET          1u
#define TIC_QIMR_QERR_MASK              0x00000004u
#define TIC_QIMR_QERR_OFFSET            2u
#define TIC_QIMR_MPE_MASK               0x00000008u
#define TIC_QIMR_MPE_OFFSET             3u

#define TIC_QISR_IDX_MASK               0x00000001u
#define TIC_QISR_IDX_OFFSET             0u
#define TIC_QISR_DIRCHG_MASK            0x00000002u
#define TIC_QISR_DIRCHG_OFFSET          1u
#define TIC_QISR_QERR_MASK              0x00000004u
#define TIC_QISR_QERR_OFFSET            2u
#define TIC_QISR_MPE_MASK               0x00000008u
#define TIC_QISR_MPE_OFFSET             3u
#define TIC_QISR_DIR_MASK               0x00000100u
#define TIC_QISR_DIR_OFFSET             8u

#define TIC_FMR_ENFC0_MASK              0x00000001u
#define TIC_FMR_ENFC0_OFFSET            0u
#define TIC_FMR_ENFC1_MASK              0x00000002u
#define TIC_FMR_ENFC1_OFFSET            1u

#define TIC_WPMR_WPEN_MASK              0x00000001u
#define TIC_WPMR_WPEN_OFFSET            0u
#define TIC_WPMR_WPKEY_MASK             0xFFFFFF00u
#define TIC_WPMR_WPKEY_OFFSET           8u

// clang-format on

#endif // BSP_TIC_REGISTERS_H
