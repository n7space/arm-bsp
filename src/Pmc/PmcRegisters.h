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

#ifndef BSP_PMC_REGISTERS_H
#define BSP_PMC_REGISTERS_H

#include <stdint.h>

/// \brief Structure mapping PMC registers.
typedef struct {
	uint32_t scer; ///< 0x000 System Clock Enable Register
	uint32_t scdr; ///< 0x004 System Clock Disable Register
	uint32_t scsr; ///< 0x008 System Clock Status Register
#if defined(N7S_TARGET_SAMV71Q21)
	uint32_t reserved1; ///< 0x00C Reserved
	uint32_t pcer0; ///< 0x010 Peripheral Clock Enable Register 0
	uint32_t pcdr0; ///< 0x014 Peripheral Clock Disable Register 0
	uint32_t pcsr0; ///< 0x018 Peripheral Clock Status Register 0
	uint32_t ckgrUckr; ///< 0x01C UTMI Clock Register
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	uint32_t reserved1[5]; ///< 0x00C - 0x01C Reserved
#endif
	uint32_t ckgrMor; ///< 0x020 Main Oscillator Register
	uint32_t ckgrMcfr; ///< 0x024 Main Clock Frequency Register
	uint32_t ckgrPllar; ///< 0x028 PLLA Register
#if defined(N7S_TARGET_SAMV71Q21)
	uint32_t reserved2; ///< 0x02C Reserved
	uint32_t mckr; ///< 0x030 Master Clock Register
	uint32_t reserved3; ///< 0x034 Reserved
	uint32_t usb; ///< 0x038 USB Clock Register
	uint32_t reserved4; ///< 0x03C Reserved
	uint32_t pck[8]; ///< 0x040 - 0x05C Programmable Clock Registers
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	uint32_t ckgrPllbr; ///< 0x02C PLLB Register
	uint32_t mckr; ///< 0x030 Master Clock Register
	uint32_t reserved3[3]; ///< 0x034 - 0x03C Reserved
	uint32_t pck[4]; ///< 0x040 - 0x04C Programmable Clock Registers
	uint32_t reserved4[4]; ///< 0x050 - 0x05C Reserved
#endif
	uint32_t ier; ///< 0x060 Interrupt Enable Register
	uint32_t idr; ///< 0x064 Interrupt Disable Register
	uint32_t sr; ///< 0x068 Status Register
	uint32_t imr; ///< 0x06C Interrupt Mask Register
#if defined(N7S_TARGET_SAMV71Q21)
	uint32_t fsmr; ///< 0x070 Fast Startup Mode Register
	uint32_t fspr; ///< 0x074 Fast Startup Polarity Register
	uint32_t focr; ///< 0x078 Fault Output Clear Register
	uint32_t reserved5[26]; ///< 0x07C - 0x0E0 Reserved
	uint32_t wpmr; ///< 0x0E4 Write Protection Mode Register
	uint32_t wpsr; ///< 0x0E8 Write Protection Status Register
	uint32_t reserved6[5]; ///< 0x0EC - 0x0FC Reserved
	uint32_t pcer1; ///< 0x100 Peripheral Clock Enable Register 1
	uint32_t pcdr1; ///< 0x104 Peripheral Clock Disable Register 1
	uint32_t pcsr1; ///< 0x108 Peripheral Clock Status Register 1
	uint32_t pcr; ///< 0x10C Peripheral Control Register
	uint32_t ocr; ///< 0x110 Oscillator Calibration Register
	uint32_t slpwkEr0; ///< 0x114 SleepWalking Enable Register 0
	uint32_t slpwkDr0; ///< 0x118 SleepWalking Disable Register 0
	uint32_t slpwkSr0; ///< 0x11C SleepWalking Status Register 0
	uint32_t slpwkAsr0; ///< 0x120 SleepWalking Activity Status Register 0
	uint32_t reserved7[3]; ///< 0x124 - 0x12C Reserved
	uint32_t pmmr; ///< 0x130 PLL Maximum Multiplier Value Register
	uint32_t slpwkEr1; ///< 0x134 SleepWalking Enable Register 1
	uint32_t slpwkDr1; ///< 0x138 SleepWalking Disable Register 1
	uint32_t slpwkSr1; ///< 0x13C SleepWalking Status Register 1
	uint32_t slpwkAsr1; ///< 0x140 SleepWalking Activity Status Register 1
	uint32_t slpwkAipr; ///< 0x144 SleepWalking Activity In Progress Register 1
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	uint32_t reserved5[2]; ///< 0x070 - 0x074 Reserved
	uint32_t focr; ///< 0x078 Fault Output Clear Register
	uint32_t reserved6; ///< 0x07C Reserved
	uint32_t pllCfg; ///< 0x080 PLL Configuration Register
	uint32_t reserved7[24]; ///< 0x084 - 0x0E0 Reserved
	uint32_t wpmr; ///< 0x0E4 Write Protection Mode Register
	uint32_t wpsr; ///< 0x0E8 Write Protection Status Register
	uint32_t reserved8[8]; ///< 0x0EC - 0x108 Reserved
	uint32_t pcr; ///< 0x10C Peripheral Control Register
	uint32_t ocr1; ///< 0x110 Oscillator Calibration Register 1
	uint32_t reserved9[7]; ///< 0x114 - 0x12C Reserved
	uint32_t pmmr; ///< 0x130 PLL Maximum Multiplier Value Register
	uint32_t reserved10[11]; ///< 0x134 - 0x15C Reserved
	uint32_t cpulim; ///< 0x160 CPU Monitor Limits Register
	uint32_t reserved11[3]; ///< 0x164 - 0x16C Reserved
	uint32_t csr[4]; ///< 0x170 - 0x17C Peripheral Clock Status Registers
	uint32_t reserved12[4]; ///< 0x180 - 0x18C Reserved
	uint32_t gcsr[4]; ///< 0x190 - 0x19C Generic Clock Status Registers
	uint32_t reserved13[4]; ///< 0x1A0 - 0x1AC Reserved
	uint32_t osc2; ///< 0x1B0 Oscillator Control Register 2
	uint32_t ocr2; ///< 0x1B4 Oscillator Calibration Register 2
#else
#error "No target platform specified (missing N7S_TARGET_* macro)"
#endif
} Pmc_Registers;

#if defined(N7S_TARGET_SAMV71Q21)

#define PMC_BASE_ADDRESS 0x400E0600

#define PMC_PCER0_MASK 0xFFFFFF80u
#define PMC_PCDR0_MASK 0xFFFFFF80u
#define PMC_PCSR0_MASK 0xFFFFFF80u

#define PMC_PCER1_MASK 0x1F3FFFAFu
#define PMC_PCDR1_MASK 0x1F3FFFAFu
#define PMC_PCSR1_MASK 0x1F3FFFAFu

#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define PMC_BASE_ADDRESS 0x4000C000u

#else
#error "No target platform specified (missing N7S_TARGET_* macro)"
#endif

#define PMC_SCER_PCK0_MASK 0x00000100u
#define PMC_SCER_PCK0_OFFSET 8u
#define PMC_SCER_PCK1_MASK 0x00000200u
#define PMC_SCER_PCK1_OFFSET 9u
#define PMC_SCER_PCK2_MASK 0x00000400u
#define PMC_SCER_PCK2_OFFSET 10u
#define PMC_SCER_PCK3_MASK 0x00000800u
#define PMC_SCER_PCK3_OFFSET 11u

#define PMC_SCDR_PCK0_MASK 0x00000100u
#define PMC_SCDR_PCK0_OFFSET 8u
#define PMC_SCDR_PCK1_MASK 0x00000200u
#define PMC_SCDR_PCK1_OFFSET 9u
#define PMC_SCDR_PCK2_MASK 0x00000400u
#define PMC_SCDR_PCK2_OFFSET 10u
#define PMC_SCDR_PCK3_MASK 0x00000800u
#define PMC_SCDR_PCK3_OFFSET 11u

#define PMC_SCSR_PCK0_MASK 0x00000100u
#define PMC_SCSR_PCK0_OFFSET 8u
#define PMC_SCSR_PCK1_MASK 0x00000200u
#define PMC_SCSR_PCK1_OFFSET 9u
#define PMC_SCSR_PCK2_MASK 0x00000400u
#define PMC_SCSR_PCK2_OFFSET 10u
#define PMC_SCSR_PCK3_MASK 0x00000800u
#define PMC_SCSR_PCK3_OFFSET 11u

#if defined(N7S_TARGET_SAMV71Q21)
#define PMC_SCER_PCK4_MASK 0x00001000u
#define PMC_SCER_PCK4_OFFSET 12u
#define PMC_SCER_PCK5_MASK 0x00002000u
#define PMC_SCER_PCK5_OFFSET 13u
#define PMC_SCER_PCK6_MASK 0x00004000u
#define PMC_SCER_PCK6_OFFSET 14u
#define PMC_SCER_PCK7_MASK 0x00008000u
#define PMC_SCER_PCK7_OFFSET 15u

#define PMC_SCDR_PCK4_MASK 0x00001000u
#define PMC_SCDR_PCK4_OFFSET 12u
#define PMC_SCDR_PCK5_MASK 0x00002000u
#define PMC_SCDR_PCK5_OFFSET 13u
#define PMC_SCDR_PCK6_MASK 0x00004000u
#define PMC_SCDR_PCK6_OFFSET 14u
#define PMC_SCDR_PCK7_MASK 0x00008000u
#define PMC_SCDR_PCK7_OFFSET 15u

#define PMC_SCSR_HCLKS_MASK 0x00000001u
#define PMC_SCSR_HCLKS_OFFSET 0u

#define PMC_SCSR_PCK4_MASK 0x00001000u
#define PMC_SCSR_PCK4_OFFSET 12u
#define PMC_SCSR_PCK5_MASK 0x00002000u
#define PMC_SCSR_PCK5_OFFSET 13u
#define PMC_SCSR_PCK6_MASK 0x00004000u
#define PMC_SCSR_PCK6_OFFSET 14u
#define PMC_SCSR_PCK7_MASK 0x00008000u
#define PMC_SCSR_PCK7_OFFSET 15u
#endif

#define CKGR_MOR_MOSCXTEN_MASK 0x00000001u
#define CKGR_MOR_MOSCXTEN_OFFSET 0u
#if defined(N7S_TARGET_SAMV71Q21) || defined(N7S_TARGET_SAMRH71F20)
#define CKGR_MOR_MOSCXTBY_MASK 0x00000002u
#define CKGR_MOR_MOSCXTBY_OFFSET 1u
#elif defined(N7S_TARGET_SAMRH707F18)
#define CKGR_MOR_ZERO_MASK 0x00000002u
#define CKGR_MOR_ZERO_OFFSET 1u
#endif

#if defined(N7S_TARGET_SAMV71Q21)
#define CKGR_MOR_WAITMODE_MASK 0x00000004u
#define CKGR_MOR_WAITMODE_OFFSET 2u
#endif

#define CKGR_MOR_MOSCRCEN_MASK 0x00000008u
#define CKGR_MOR_MOSCRCEN_OFFSET 3u
#define CKGR_MOR_MOSCRCF_MASK 0x00000070u
#define CKGR_MOR_MOSCRCF_OFFSET 4u
#define CKGR_MOR_MOSCXTST_MASK 0x0000FF00u
#define CKGR_MOR_MOSCXTST_OFFSET 8u
#define CKGR_MOR_KEY_MASK 0x00FF0000u
#define CKGR_MOR_KEY_OFFSET 16u
#define CKGR_MOR_MOSCSEL_MASK 0x01000000u
#define CKGR_MOR_MOSCSEL_OFFSET 24u
#define CKGR_MOR_KEY_VALUE UINT32_C(0x37)

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define CKGR_MOR_CFDEN_MASK 0x02000000u
#define CKGR_MOR_CFDEN_OFFSET 25u
#define CKGR_MOR_XT32KFME_MASK 0x04000000u
#define CKGR_MOR_XT32KFME_OFFSET 26u
#define CKGR_MOR_BCPURST_MASK 0x08000000u
#define CKGR_MOR_BCPURST_OFFSET 27u
#define CKGR_MOR_BCPUNMIC_MASK 0x10000000u
#define CKGR_MOR_BCPUNMIC_OFFSET 28u
#endif

#define CKGR_MCFR_MAINF_MASK 0x0000FFFFu
#define CKGR_MCFR_MAINF_OFFSET 0u
#define CKGR_MCFR_MAINFRDY_MASK 0x00010000u
#define CKGR_MCFR_MAINFRDY_OFFSET 16u
#define CKGR_MCFR_RCMEAS_MASK 0x00100000u
#define CKGR_MCFR_RCMEAS_OFFSET 20u
#define CKGR_MCFR_CCSS_MASK 0x01000000u
#define CKGR_MCFR_CCSS_OFFSET 24u

#define CKGR_PLLAR_DIVA_MASK 0x000000FFu
#define CKGR_PLLAR_DIVA_OFFSET 0u
#define CKGR_PLLAR_PLLACOUNT_MASK 0x00003F00u
#define CKGR_PLLAR_PLLACOUNT_OFFSET 8u
#define CKGR_PLLAR_MULA_MASK 0x07FF0000u
#define CKGR_PLLAR_MULA_OFFSET 16u
#define CKGR_PLLAR_ONE_MASK 0x20000000u
#define CKGR_PLLAR_ONE_OFFSET 29u

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define CKGR_PLLAR_FREQVCO_MASK 0x0000C000u
#define CKGR_PLLAR_FREQVCO_OFFSET 14u

#define CKGR_PLLBR_DIVB_MASK 0x000000FFu
#define CKGR_PLLBR_DIVB_OFFSET 0u
#define CKGR_PLLBR_PLLBCOUNT_MASK 0x00003F00u
#define CKGR_PLLBR_PLLBCOUNT_OFFSET 8u
#define CKGR_PLLBR_FREQVCO_MASK 0x0000C000u
#define CKGR_PLLBR_FREQVCO_OFFSET 14u
#define CKGR_PLLBR_MULB_MASK 0x07FF0000u
#define CKGR_PLLBR_MULB_OFFSET 16u
#define CKGR_PLLBR_SRCB_MASK 0x60000000u
#define CKGR_PLLBR_SRCB_OFFSET 29u
#endif

#define PMC_MCKR_CSS_MASK 0x00000003u
#define PMC_MCKR_CSS_OFFSET 0u
#define PMC_MCKR_PRES_MASK 0x00000070u
#define PMC_MCKR_PRES_OFFSET 4u

#if defined(N7S_TARGET_SAMV71Q21)
#define PMC_MCKR_MDIV_MASK 0x00000300u
#define PMC_MCKR_MDIV_OFFSET 8u

#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define PMC_MCKR_MDIV_MASK 0x00000100u
#define PMC_MCKR_MDIV_OFFSET 8u
#define PMC_MCKR_ZERO_MASK 0x00002000u
#define PMC_MCKR_ZERO_OFFSET 13u
#else
#error "No target platform specified (missing N7S_TARGET_* macro)"
#endif

#define PMC_PCK_CSS_MASK 0x00000007u
#define PMC_PCK_CSS_OFFSET 0u
#define PMC_PCK_PRES_MASK 0x00000FF0u
#define PMC_PCK_PRES_OFFSET 4u

#define PMC_SR_MOSCXTS_MASK 0x00000001u
#define PMC_SR_MOSCXTS_OFFSET 0u
#define PMC_SR_LOCKA_MASK 0x00000002u
#define PMC_SR_LOCKA_OFFSET 1u
#define PMC_SR_MCKRDY_MASK 0x00000008u
#define PMC_SR_MCKRDY_OFFSET 3u

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define PMC_SR_LOCKB_MASK 0x00000004u
#define PMC_SR_LOCKB_OFFSET 2u
#define PMC_SR_OSCSELS_MASK 0x00000080u
#define PMC_SR_OSCSELS_OFFSET 7u
#endif

#define PMC_SR_PCKRDY0_MASK 0x00000100u
#define PMC_SR_PCKRDY0_OFFSET 8u
#define PMC_SR_PCKRDY1_MASK 0x00000200u
#define PMC_SR_PCKRDY1_OFFSET 9u
#define PMC_SR_PCKRDY2_MASK 0x00000400u
#define PMC_SR_PCKRDY2_OFFSET 10u
#define PMC_SR_PCKRDY3_MASK 0x00000800u
#define PMC_SR_PCKRDY3_OFFSET 11u
#define PMC_SR_PCKRDY4_MASK 0x00001000u
#define PMC_SR_PCKRDY4_OFFSET 12u
#define PMC_SR_PCKRDY5_MASK 0x00002000u
#define PMC_SR_PCKRDY5_OFFSET 13u
#define PMC_SR_PCKRDY6_MASK 0x00004000u
#define PMC_SR_PCKRDY6_OFFSET 14u
#define PMC_SR_MOSCSELS_MASK 0x00010000u
#define PMC_SR_MOSCSELS_OFFSET 16u
#define PMC_SR_MOSCRCS_MASK 0x00020000u
#define PMC_SR_MOSCRCS_OFFSET 17u

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define PMC_SR_CFDEV_MASK 0x00040000u
#define PMC_SR_CFDEV_OFFSET 18u
#define PMC_SR_XT32KERR_MASK 0x00200000u
#define PMC_SR_XT32KERR_OFFSET 21u
#define PMC_SR_CPUMON_MASK 0x00800000u
#define PMC_SR_CPUMON_OFFSET 23u
#endif

// clang-format off

#if defined(N7S_TARGET_SAMV71Q21)
#define PMC_FSMR_FSTT0_MASK     0x00000001
#define PMC_FSMR_FSTT0_OFFSET   0u
#define PMC_FSMR_FSTT1_MASK     0x00000002
#define PMC_FSMR_FSTT1_OFFSET   1u
#define PMC_FSMR_FSTT2_MASK     0x00000004
#define PMC_FSMR_FSTT2_OFFSET   2u
#define PMC_FSMR_FSTT3_MASK     0x00000008
#define PMC_FSMR_FSTT3_OFFSET   3u
#define PMC_FSMR_FSTT4_MASK     0x00000010
#define PMC_FSMR_FSTT4_OFFSET   4u
#define PMC_FSMR_FSTT5_MASK     0x00000020
#define PMC_FSMR_FSTT5_OFFSET   5u
#define PMC_FSMR_FSTT6_MASK     0x00000040
#define PMC_FSMR_FSTT6_OFFSET   6u
#define PMC_FSMR_FSTT7_MASK     0x00000080
#define PMC_FSMR_FSTT7_OFFSET   7u
#define PMC_FSMR_FSTT8_MASK     0x00000100
#define PMC_FSMR_FSTT8_OFFSET   8u
#define PMC_FSMR_FSTT9_MASK     0x00000200
#define PMC_FSMR_FSTT9_OFFSET   9u
#define PMC_FSMR_FSTT10_MASK    0x00000400
#define PMC_FSMR_FSTT10_OFFSET  10u
#define PMC_FSMR_FSTT11_MASK    0x00000800
#define PMC_FSMR_FSTT11_OFFSET  11u
#define PMC_FSMR_FSTT12_MASK    0x00001000
#define PMC_FSMR_FSTT12_OFFSET  12u
#define PMC_FSMR_FSTT13_MASK    0x00002000
#define PMC_FSMR_FSTT13_OFFSET  13u
#define PMC_FSMR_FSTT14_MASK    0x00004000
#define PMC_FSMR_FSTT14_OFFSET  14u
#define PMC_FSMR_FSTT15_MASK    0x00008000
#define PMC_FSMR_FSTT15_OFFSET  15u
#define PMC_FSMR_RTTAL_MASK     0x00010000
#define PMC_FSMR_RTTAL_OFFSET   16u
#define PMC_FSMR_RTCAL_MASK     0x00020000
#define PMC_FSMR_RTCAL_OFFSET   17u
#define PMC_FSMR_USBAL_MASK     0x00040000
#define PMC_FSMR_USBAL_OFFSET   18u
#define PMC_FSMR_LPM_MASK       0x00100000
#define PMC_FSMR_LPM_OFFSET     20u
#define PMC_FSMR_FLPM_MASK      0x00600000
#define PMC_FSMR_FLPM_OFFSET    21u
#define PMC_FSMR_FFLPM_MASK     0x00800000
#define PMC_FSMR_FFLPM_OFFSET   23u
#endif

#define PMC_PCR_PID_MASK 0x0000007Fu
#define PMC_PCR_PID_OFFSET 0u

#if defined(N7S_TARGET_SAMV71Q21)
#define PMC_PCR_GCLKCSS_MASK 0x00000700u
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define PMC_PCR_GCLKCSS_MASK 0x00000F00u
#else
#error "No target platform specified (missing N7S_TARGET_* macro)"
#endif

#define PMC_PCR_GCLKCSS_OFFSET 8u
#define PMC_PCR_CMD_MASK 0x00001000u
#define PMC_PCR_CMD_OFFSET 12u
#define PMC_PCR_CMD_WRITE (UINT32_C(1) << PMC_PCR_CMD_OFFSET)
#define PMC_PCR_CMD_READ (UINT32_C(0) << PMC_PCR_CMD_OFFSET)
#define PMC_PCR_GCLKDIV_MASK 0x0FF00000u
#define PMC_PCR_GCLKDIV_OFFSET 20u
#define PMC_PCR_EN_MASK 0x10000000u
#define PMC_PCR_EN_OFFSET 28u
#define PMC_PCR_GCLKEN_MASK 0x20000000u
#define PMC_PCR_GCLKEN_OFFSET 29u
#define PMC_PCR_PID_MAX 127u

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define PMC_PLLCFG_SRB_MASK 0xC0000000u
#define PMC_PLLCFG_SRB_OFFSET 30u
#define PMC_PLLCFG_SCB_MASK 0x30000000u
#define PMC_PLLCFG_SCB_OFFSET 28u
#define PMC_PLLCFG_OUTCUR_PLLB_MASK 0x000F0000u
#define PMC_PLLCFG_OUTCUR_PLLB_OFFSET 16u
#define PMC_PLLCFG_SRA_MASK 0x0000C000u
#define PMC_PLLCFG_SRA_OFFSET 14u
#define PMC_PLLCFG_SCA_MASK 0x00003000u
#define PMC_PLLCFG_SCA_OFFSET 12u
#define PMC_PLLCFG_OUTCUR_PLLA_MASK 0x0000000Fu
#define PMC_PLLCFG_OUTCUR_PLLA_OFFSET 0u

#define PMC_OSC2_KEY_MASK 0x00FF0000u
#define PMC_OSC2_KEY_OFFSET 16u
#define PMC_OSC2_KEY_VALUE UINT32_C(0x37)
#define PMC_OSC2_EN_WR_CALIB_MASK 0x00000100u
#define PMC_OSC2_EN_WR_CALIB_OFFSET 8u
#define PMC_OSC2_OSCRCF_MASK 0x00000030u
#define PMC_OSC2_OSCRCF_OFFSET 4u
#define PMC_OSC2_EN_MASK 0x00000001u
#define PMC_OSC2_EN_OFFSET 0u

#define PMC_OCR_SEL12_MASK 0x80000000u
#define PMC_OCR_SEL12_OFFSET 31u
#define PMC_OCR_CAL10_FREQ_MASK 0x1C000000u
#define PMC_OCR_CAL10_FREQ_OFFSET 26u
#define PMC_OCR_CAL10_TEMP_MASK 0x03000000u
#define PMC_OCR_CAL10_TEMP_OFFSET 24u
#define PMC_OCR_SEL10_MASK 0x00800000u
#define PMC_OCR_SEL10_OFFSET 23u
#define PMC_OCR_CAL12_FREQ_MASK 0x001C0000u
#define PMC_OCR_CAL12_FREQ_OFFSET 18u
#define PMC_OCR_CAL12_TEMP_MASK 0x00030000u
#define PMC_OCR_CAL12_TEMP_OFFSET 16u
#define PMC_OCR_SEL8_MASK 0x00008000u
#define PMC_OCR_SEL8_OFFSET 15u
#define PMC_OCR_CAL8_FREQ_MASK 0x00001C00u
#define PMC_OCR_CAL8_FREQ_OFFSET 10u
#define PMC_OCR_CAL8_TEMP_MASK 0x00000300u
#define PMC_OCR_CAL8_TEMP_OFFSET 8u
#define PMC_OCR_SEL4_MASK 0x00000080u
#define PMC_OCR_SEL4_OFFSET 7u
#define PMC_OCR_CAL4_FREQ_MASK 0x0000001Cu
#define PMC_OCR_CAL4_FREQ_OFFSET 2u
#define PMC_OCR_CAL4_TEMP_MASK 0x00000003u
#define PMC_OCR_CAL4_TEMP_OFFSET 0u

#define PMC_OCR2_SEL10_MASK 0x80000000u
#define PMC_OCR2_SEL10_OFFSET 31u
#define PMC_OCR2_CAL10_FREQ_MASK 0x1C000000u
#define PMC_OCR2_CAL10_FREQ_OFFSET 26u
#define PMC_OCR2_CAL10_TEMP_MASK 0x03000000u
#define PMC_OCR2_CAL10_TEMP_OFFSET 24u
#define PMC_OCR2_SEL12_MASK 0x00800000u
#define PMC_OCR2_SEL12_OFFSET 23u
#define PMC_OCR2_CAL12_FREQ_MASK 0x001C0000u
#define PMC_OCR2_CAL12_FREQ_OFFSET 18u
#define PMC_OCR2_CAL12_TEMP_MASK 0x00030000u
#define PMC_OCR2_CAL12_TEMP_OFFSET 16u
#define PMC_OCR2_SEL8_MASK 0x00008000u
#define PMC_OCR2_SEL8_OFFSET 15u
#define PMC_OCR2_CAL8_FREQ_MASK 0x00001C00u
#define PMC_OCR2_CAL8_FREQ_OFFSET 10u
#define PMC_OCR2_CAL8_TEMP_MASK 0x00000300u
#define PMC_OCR2_CAL8_TEMP_OFFSET 8u
#define PMC_OCR2_SEL4_MASK 0x00000080u
#define PMC_OCR2_SEL4_OFFSET 7u
#define PMC_OCR2_CAL4_FREQ_MASK 0x0000001Cu
#define PMC_OCR2_CAL4_FREQ_OFFSET 2u
#define PMC_OCR2_CAL4_TEMP_MASK 0x00000003u
#define PMC_OCR2_CAL4_TEMP_OFFSET 0u
#endif

// clang-format on

#endif // BSP_PMC_REGISTERS_H
