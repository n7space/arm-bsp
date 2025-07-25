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

#ifndef BSP_FPU_H
#define BSP_FPU_H

/// \file  Fpu.h
/// \addtogroup Bsp
/// \brief BSP driver for Floating Point Unit.

#include <Utils/Utils.h>

#include "FpuRegisters.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Fpu
/// @ingroup Bsp
/// @{

/// \brief FPU half-precision modes.
typedef enum {
	Fpu_AlternativeHalfPrecisionMode_Ieee7542008 =
			0, ///< IEEE 754-2008 half-precision format.
	Fpu_AlternativeHalfPrecisionMode_Alternative =
			1, ///< Alternative half-precision format.
} Fpu_AlternativeHalfPrecisionMode;

/// \brief FPU NaN modes.
typedef enum {
	/// \brief NaN operands propagate through to the output of a floating-point operation.
	Fpu_NanMode_NanOperandsPropagate = 0,
	/// \brief Any operation involving one or more NaNs returns the Default NaN.
	Fpu_NanMode_DefaultNanReturned = 1,
} Fpu_NanMode;

/// \brief Flush-to-zero modes.
typedef enum {
	Fpu_FlushToZeroMode_Disabled =
			0, ///< Flush-to-zero mode disabled, FPU is IEEE-754 compliant.
	Fpu_FlushToZeroMode_Enabled = 1, ///< Flush-to-zero mode enabled.
} Fpu_FlushToZeroMode;

/// \brief Rounding mode.
typedef enum {
	Fpu_RoundingMode_RoundToNearest = 0, ///< Round to nearest.
	Fpu_RoundingMode_RoundTowardsPlusInfinity =
			1, ///< Round towards plus infinity.
	Fpu_RoundingMode_RoundTowardsMinusInfinity =
			2, ///< Round towards minus infinity.
	Fpu_RoundingMode_RoundTowardsZero = 3, ///< Round towards zero.
} Fpu_RoundingMode;

/// \brief Coprocessor access mode.
typedef enum {
	Fpu_CoprocessorAccessMode_Denied =
			0, ///< Access generated NOCP UsageFault.
	Fpu_CoprocessorAccessMode_Priviledged =
			1, ///< Non-priviledged access generates NOCP UsageFault.
	Fpu_CoprocessorAccessMode_Reserved = 2, ///< Reserved.
	Fpu_CoprocessorAccessMode_Full = 3, ///< Full access.
} Fpu_CoprocessorAccessMode;

/// \brief Structure holding FPU configuration.
typedef struct {
	/// \brief Automatically preserve FPU context on exception.
	bool isFpContextPreservedOnException;
	/// \brief Address of the floating-point register space allocated on exception.
	uint32_t exceptionFpRegisterSpaceAddress;
	/// \brief Default half-precision mode.
	Fpu_AlternativeHalfPrecisionMode defaultHalfPrecisionMode;
	/// \brief Default NaN mode.
	Fpu_NanMode defaultNanMode;
	/// \brief Default flush-to-zero mode.
	Fpu_FlushToZeroMode defaultFlushToZeroMode;
	/// \brief Rounding mode.
	Fpu_RoundingMode defaultRoundingMode;
} Fpu_Config;

/// \brief Structure holding FPU context configuration.
typedef struct {
	Fpu_AlternativeHalfPrecisionMode
			halfPrecisionMode; ///< Half-precision mode.
	Fpu_NanMode nanMode; ///< NaN mode.
	Fpu_FlushToZeroMode flushToZeroMode; ///< Flush to zero mode.
	Fpu_RoundingMode roundingMode; ///< Rounding mode.
} Fpu_ContextConfig;

/// \brief Structure holding FPU context state.
typedef struct {
	bool isNegativeConditionCodeFlagSet; ///< Is N bit set.
	bool isZeroConditionCodeFlagSet; ///< Is Z bit set.
	bool isCarryConditionCodeFlagSet; ///< Is C bit set.
	bool isOverflowConditionCodeFlagSet; ///< Is V bit set.
	bool isInputDenormalCumulativeExceptionBitSet; ///< Is IDC bit set.
	bool isInexactCumulativeExceptionBitSet; ///< Is IXC bit set.
	bool isUnderflowCumulativeExceptionBitSet; ///< Is UFC bit set.
	bool isOverflowCumulativeExceptionBitSet; ///< Is OFC bit set.
	bool isDivisionByZeroCumulativeExceptionBitSet; ///< Is DZC bit set.
	bool isInvalidOperationCumulativeExceptionBitSet; ///< Is IOC bit set.
} Fpu_ContextState;

/// \brief FPU register bank sizes.
typedef enum {
	Fpu_RegisterBankSize_16 = 1, ///< 16 x 64-bit registers
} Fpu_RegisterBankSize;

/// \brief Structure holding FPU features.
typedef struct {
	bool areRoundingModesSupported; ///< Are rounding modes supported.
	bool areShortVectorsSupported; ///< Are short vectors supported.
	bool areSquareRootOperationsSupported; ///< Are square root operations supported.
	bool areDivideOperationsSupported; ///< Are divide operations supported.
	bool isExceptionTrappingSupported; ///< Is exception trapping supported.
	bool areDoublePrecisionOperationsSupported; ///< Are double precision operations supported.
	bool areSinglePrecisionOperationsSupported; ///< Are single precision operations supported.
	Fpu_RegisterBankSize fpuRegisterBankSize; ///< FPU register bank size.
	bool areFpFusedMacOperationsSupported; ///< FP fused MAC operations supported.
	bool isNanValuePropagationSupported; ///< NaN value propagation supported.
	/// \brief Full denormalized number arithmetic supported.
	bool isFullDenormalizedNumberArithmeticSupported;
	bool areMiscellaneousFeaturesSupported; ///< Miscellaneous features supported.
} Fpu_Features;

/// \brief Structure holding FPU status.
typedef struct {
	/// \brief Debug Monitor exception pending could be set.
	bool couldDebugMonitorExceptionPendingbeBeSet;
	/// \brief Bus Fault exception pending could be set.
	bool couldBusFaultExceptionPendingbeBeSet;
	/// \brief Mem Manage exception pending could be set.
	bool couldMemManageExceptionPendingbeBeSet;
	/// \brief Hard Fault exception pending could be set.
	bool couldHardFaultExceptionPendingbeBeSet;
	/// \brief Was processor in thread mode.
	bool wasProcessorInThreadMode;
	/// \brief Was processor in user mode.
	bool wasProcessorInUserMode;
	/// \brief Is lazy FP state preservation active.
	bool isLazyFpStatePreservationActive;
} Fpu_Status;

/// \brief Structure representing FPU.
typedef struct {
	Fpu_Registers *registers; ///< Pointer to FPU registers.
	Fpu_CoprocessorRegisters *
			coprocessorRegisters; ///< Pointer to coprocessor registers.
} Fpu;

/// \brief Enables hardware support for floating point operations.
/// \param [in,out] fpu Pointer to a structure representing FPU.
void Fpu_startup(Fpu *const fpu);

/// \brief Disables hardware support for floating point operations.
/// \param [in,out] fpu Pointer to a structure representing FPU.
void Fpu_shutdown(Fpu *const fpu);

/// \brief Initializes the structure representing FPU.
/// \param [in,out] fpu Pointer to a structure representing FPU.
void Fpu_init(Fpu *const fpu);

/// \brief Sets the FPU configuration.
/// \param [in,out] fpu Pointer to a structure representing FPU.
/// \param [in] config FPU configuration.
void Fpu_setConfig(Fpu *const fpu, const Fpu_Config *const config);

/// \brief Gets the FPU configuration.
/// \param [in] fpu Pointer to a structure representing FPU.
/// \param [out] config FPU configuration.
void Fpu_getConfig(const Fpu *const fpu, Fpu_Config *const config);

/// \brief Sets the FPU context configuration.
/// \param [in] config FPU context configuration.
void Fpu_setContextConfig(const Fpu_ContextConfig *const config);

/// \brief Gets the FPU context configuration.
/// \param [out] config FPU context configuration.
void Fpu_getContextConfig(Fpu_ContextConfig *const config);

/// \brief Gets the FPU context state.
/// \param [out] state FPU context state.
void Fpu_getContextState(Fpu_ContextState *const state);

/// \brief Clears FPU exception flags.
void Fpu_clearExceptions(void);

/// \brief Gets the FPU features.
/// \param [in] fpu Pointer to a structure representing FPU.
/// \param [out] features Pointer to a structure containing FPU features.
void Fpu_getFeatures(const Fpu *const fpu, Fpu_Features *const features);

/// \brief Gets the FPU status.
/// \param [in] fpu Pointer to a structure representing FPU.
/// \param [out] status Pointer to a structure containing FPU status.
void Fpu_getStatus(const Fpu *const fpu, Fpu_Status *const status);

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // BSP_FPU_H
