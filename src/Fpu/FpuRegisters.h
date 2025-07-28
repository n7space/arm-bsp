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

#ifndef BSP_FPUREGISTERS_H
#define BSP_FPUREGISTERS_H

#include <stdint.h>

/// \brief Structure representing FPU control and status registers.
typedef struct {
	volatile uint32_t fpccr; ///< 0x00 Context Control Register.
	volatile uint32_t fpcar; ///< 0x04 Context Address Register.
	volatile uint32_t fpdscr; ///< 0x08 Default Status Control Register.
	volatile uint32_t mvfr0; ///< 0x0C Media and VFP Feature Register 0.
	volatile uint32_t mvfr1; ///< 0x10 Media and VFP Feature Register 1.
	volatile uint32_t mvfr2; ///< 0x14 Media and VFP Feature Register 2.
} Fpu_Registers;

typedef struct {
	volatile uint32_t cpacr; ///< Coprocessor Access Control Register.
} Fpu_CoprocessorRegisters;

#define FPU_REGISTERS_ADDRESS_BASE 0xE000EF34

#define FPU_COPROCESSOR_REGISTER_ADDRESS_BASE 0xE000ED88

/// \brief Automatically preserve FP context on exception register offset.
#define FPU_FPCCR_ASPEN_OFFSET 31u
/// \brief Automatically preserve FP context on exception register mask.
#define FPU_FPCCR_ASPEN_MASK 0x80000000u
/// \brief Enable lazy context save of FP state register offset.
#define FPU_FPCCR_LSPEN_OFFSET 30u
/// \brief Enable lazy context save of FP state register mask.
#define FPU_FPCCR_LSPEN_MASK 0x40000000u
/// \brief Debug Monitor exception pending set register offset.
#define FPU_FPCCR_MONRDY_OFFSET 8u
/// \brief Debug Monitor exception pending set register mask.
#define FPU_FPCCR_MONRDY_MASK 0x00000100u
/// \brief Bus Fault exception pending set register offset.
#define FPU_FPCCR_BFRDY_OFFSET 6u
/// \brief Bus Fault exception pending set register mask.
#define FPU_FPCCR_BFRDY_MASK 0x00000040u
/// \brief Mem Manage exception pending set register offset.
#define FPU_FPCCR_MMRDY_OFFSET 5u
/// \brief Mem Manage exception pending set register mask.
#define FPU_FPCCR_MMRDY_MASK 0x00000020u
/// \brief Hard Fault exception pending set register offset.
#define FPU_FPCCR_HFRDY_OFFSET 4u
/// \brief Hard Fault exception pending set register mask.
#define FPU_FPCCR_HFRDY_MASK 0x00000010u
/// \brief Is thread mode set register offset.
#define FPU_FPCCR_THREAD_OFFSET 3u
/// \brief Is thread mode set register mask.
#define FPU_FPCCR_THREAD_MASK 0x00000008u
/// \brief Is user mode set register offset.
#define FPU_FPCCCR_USER_OFFSET 1u
/// \brief Is user mode set register mask.
#define FPU_FPCCR_USER_MASK 0x00000002u
/// \brief Lazy FP state active register offset.
#define FPU_FPCCR_LSPACT_OFFSET 0u
/// \brief Lazy FP state active register mask.
#define FPU_FPCCR_LSPACT_MASK 0x00000001u

/// \brief FP register space allocated on an exception stack frame address register offset.
#define FPU_FPCAR_ADDRESS_OFFSET 3u
/// \brief FP register space allocated on an exception stack frame address register mask.
#define FPU_FPCAR_ADDRESS_MASK 0xFFFFFFF8u

/// \brief Alternative half-precision control bit default value register offset.
#define FPU_FPDSCR_AHP_OFFSET 26u
/// \brief Alternative half-precision control bit default value register mask.
#define FPU_FPDSCR_AHP_MASK 0x04000000u
/// \brief NaN mode control bit default value register offset.
#define FPU_FPDSCR_DN_OFFSET 25u
/// \brief NaN mode control bit default value register mask.
#define FPU_FPDSCR_DN_MASK 0x02000000u
/// \brief Flush-to-zero mode control bit default value register offset.
#define FPU_FPDSCR_FZ_OFFSET 24u
/// \brief Flush-to-zero mode control bit default value register mask.
#define FPU_FPDSCR_FZ_MASK 0x01000000u
/// \brief Rounding mode control field default value register offset.
#define FPU_FPDSCR_RMODE_OFFSET 22u
/// \brief Rounding mode control field default value register mask.
#define FPU_FPDSCR_RMODE_MASK 0x00C00000u

/// \brief Feature not supported indicator.
#define FPU_FEATURE_NOT_SUPPORTED 0x00u
/// \brief Feature supported indicator.
#define FPU_FEATURE_SUPPORTED 0xFFu

/// \brief FP rounding modes register offset.
#define FPU_MVFR0_FP_ROUNDING_MODES_OFFSET 28u
/// \brief FP rounding modes register mask.
#define FPU_MVFR0_FP_ROUNDING_MODES_MASK 0xF0000000u
/// \brief Short vectors feature register offset.
#define FPU_MVFR0_SHORT_VECTORS_OFFSET 24u
/// \brief Short vectors feature register mask.
#define FPU_MVFR0_SHORT_VECTORS_MASK 0x0F000000u
/// \brief Square root feature register offset.
#define FPU_MVFR0_SQUARE_ROOT_OFFSET 20u
/// \brief Square root feature register mask.
#define FPU_MVFR0_SQUARE_ROOT_MASK 0x00F00000u
/// \brief Divide feature register offset.
#define FPU_MVFR0_DIVIDE_OFFSET 16u
/// \brief Divide feature register mask.
#define FPU_MVFR0_DIVIDE_MASK 0x000F0000u
/// \brief FP exception trapping feature register offset.
#define FPU_MVFR0_FP_EXCEPTION_TRAPPING_OFFSET 12u
/// \brief FP exception trapping feature register mask.
#define FPU_MVFR0_FP_EXCEPTION_TRAPPING_MASK 0x0000F000u
/// \brief Double-precision feature register offset.
#define FPU_MVFR0_DOUBLE_PRECISION_OFFSET 8u
/// \brief Double-precision feature register mask.
#define FPU_MVFR0_DOUBLE_PRECISION_MASK 0x00000F00u
/// \brief Single-precision feature register offset.
#define FPU_MVFR0_SINGLE_PRECISION_OFFSET 4u
/// \brief Single-precision feature register mask.
#define FPU_MVFR0_SINGLE_PRECISION_MASK 0x000000F0u
/// \brief FP register bank size register offset.
#define FPU_MVFR0_A_SIMD_OFFSET 0u
/// \brief FP register bank size register mask.
#define FPU_MVFR0_A_SIMD_MASK 0x0000000Fu

/// \brief FP fused MAC feature register offset.
#define FPU_MVFR1_FP_FUSED_MAC_OFFSET 28u
/// \brief FP fused MAC feature register mask.
#define FPU_MVFR1_FP_FUSED_MAC_MASK 0xF0000000u
/// \brief FP half-precision and double-precision register offset.
#define FPU_MVFR1_FP_HPFP_OFFSET 24u
/// \brief FP half-precision and double-precision register mask.
#define FPU_MVFR1_FP_HPFP_MASK 0x0F000000u
/// \brief Half-precision <-> single-precision conversion supported.
#define FPU_MVFR1_FP_HPFP_HPSP_SUPPORTED 0x01u
/// \brief Half-precision <-> single-precision <-> double-precision conversion supported.
#define FPU_MVFR1_FP_HPFP_HPDP_SUPPORTED 0x02u
/// \brief Default NaN mode feature register offset.
#define FPU_MVFR1_D_NAN_MODE_OFFSET 4u
/// \brief Default NaN mode feature register mask.
#define FPU_MVFR1_D_NAN_MODE_MASK 0x000000F0u
/// \brief Flush-to-zero mode feature register offset.
#define FPU_MVFR1_FTZ_MODE_OFFSET 0u
/// \brief Flush-to-zero mode feature register mask.
#define FPU_MVFR1_FTZ_MODE_MASK 0x0000000Fu

/// \brief Miscellaneous features register offset.
#define FPU_MVFR2_VFP_MISC_OFFSET 4u
/// \brief Miscellaneous features register mask.
#define FPU_MVFR2_VFP_MISC_MASK 0x000000F0u

/// \brief Coprocessor 10 register offset.
#define FPU_CPACR_CP10_OFFSET 20u
/// \brief Coprocessor 10 register mask.
#define FPU_CPACR_CP10_MASK 0x00300000u
/// \brief Coprocessor 11 register offset.
#define FPU_CPACR_CP11_OFFSET 22u
/// \brief Coprocessor 11 register mask.
#define FPU_CPACR_CP11_MASK 0x00C00000u

/// \brief Negative condition code flag register offset.
#define FPU_FPSCR_N_OFFSET 31u
/// \brief Negative condition code flag register mask.
#define FPU_FPSCR_N_MASK 0x80000000u
/// \brief Zero condition code flag register offset.
#define FPU_FPSCR_Z_OFFSET 30u
/// \brief Zero condition code flag register mask.
#define FPU_FPSCR_Z_MASK 0x40000000u
/// \brief Carry condition code flag register offset.
#define FPU_FPSCR_C_OFFSET 29u
/// \brief Carry condition code flag register mask.
#define FPU_FPSCR_C_MASK 0x20000000u
/// \brief Overflow condition code flag register offset.
#define FPU_FPSCR_V_OFFSET 28u
/// \brief Overflow condition code flag register mask.
#define FPU_FPSCR_V_MASK 0x10000000u
/// \brief Alternative half-precision control bit register offset.
#define FPU_FPSCR_AHP_OFFSET 26u
/// \brief Alternative half-precision control bit register mask.
#define FPU_FPSCR_AHP_MASK 0x04000000u
/// \brief Default NaN mode control bit register offset.
#define FPU_FPSCR_DN_OFFSET 25u
/// \brief Default NaN mode control bit register mask.
#define FPU_FPSCR_DN_MASK 0x02000000u
/// \brief Flush-to-zero mode control bit register offset.
#define FPU_FPSCR_FZ_OFFSET 24u
/// \brief Flush-to-zero mode control bit register mask.
#define FPU_FPSCR_FZ_MASK 0x01000000u
/// \brief Rounding model control field register offset.
#define FPU_FPSCR_RMODE_OFFSET 22u
/// \brief Rounding model control field register mask.
#define FPU_FPSCR_RMODE_MASK 0x00C00000u
/// \brief Input Denormal cumulative exception bit register offset.
#define FPU_FPSCR_IDC_OFFSET 7u
/// \brief Input Denormal cumulative exception bit register mask.
#define FPU_FPSCR_IDC_MASK 0x00000080u
/// \brief Inexact cumulative exception bit register offset.
#define FPU_FPSCR_IXC_OFFSET 4u
/// \brief Inexact cumulative exception bit register mask.
#define FPU_FPSCR_IXC_MASK 0x00000010u
/// \brief Underflow cumulative exception bit register offset.
#define FPU_FPSCR_UFC_OFFSET 3u
/// \brief Underflow cumulative exception bit register mask.
#define FPU_FPSCR_UFC_MASK 0x00000008u
/// \brief Overflow cumulative exception bit register offset.
#define FPU_FPSCR_OFC_OFFSET 2u
/// \brief Overflow cumulative exception bit register mask.
#define FPU_FPSCR_OFC_MASK 0x00000004u
/// \brief Division by Zero cumulative exception bit register offset.
#define FPU_FPSCR_DZC_OFFSET 1u
/// \brief Division by Zero cumulative exception bit register mask.
#define FPU_FPSCR_DZC_MASK 0x00000002u
/// \brief Invalid Operation cumulative exception bit register offset.
#define FPU_FPSCR_IOC_OFFSET 0u
/// \brief Invalid Operation cumulative exception bit register mask.
#define FPU_FPSCR_IOC_MASK 0x00000001u

#endif // BSP_FPUREGISTERS_H
