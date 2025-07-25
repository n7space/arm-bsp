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

#include "Pio.h"

#include <Utils/Utils.h>

bool
Pio_init(const Pio_Port port, Pio *const pio, ErrorCode *const errCode)
{
	switch (port) {
	case Pio_Port_A:
		pio->port = Pio_Port_A;
		// cppcheck-suppress misra-c2012-11.4
		pio->reg = (Pio_Registers *)PIOA_ADDRESS_BASE;
		break;
	case Pio_Port_B:
		pio->port = Pio_Port_B;
		// cppcheck-suppress misra-c2012-11.4
		pio->reg = (Pio_Registers *)PIOB_ADDRESS_BASE;
		break;
	case Pio_Port_C:
		pio->port = Pio_Port_C;
		// cppcheck-suppress misra-c2012-11.4
		pio->reg = (Pio_Registers *)PIOC_ADDRESS_BASE;
		break;
	case Pio_Port_D:
		pio->port = Pio_Port_D;
		// cppcheck-suppress misra-c2012-11.4
		pio->reg = (Pio_Registers *)PIOD_ADDRESS_BASE;
		break;
	case Pio_Port_E:
		pio->port = Pio_Port_E;
		// cppcheck-suppress misra-c2012-11.4
		pio->reg = (Pio_Registers *)PIOE_ADDRESS_BASE;
		break;
	default: return returnError(errCode, Pio_ErrorCode_InvalidPortId);
	}

	return true;
}

static inline bool
setControlConfig(Pio *const pio, const uint32_t pinMask,
		const Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	if (config->control == Pio_Control_Pio) {
		pio->reg->per = pinMask;
	} else {
		volatile uint32_t abcdsr1 = pio->reg->abcdsr1;
		volatile uint32_t abcdsr2 = pio->reg->abcdsr2;
		abcdsr1 &= ~pinMask;
		abcdsr2 &= ~pinMask;
		if (config->control == Pio_Control_PeripheralA) {
			pio->reg->abcdsr1 = abcdsr1;
			pio->reg->abcdsr2 = abcdsr2;
		} else if (config->control == Pio_Control_PeripheralB) {
			pio->reg->abcdsr1 = abcdsr1 | pinMask;
			pio->reg->abcdsr2 = abcdsr2;
		} else if (config->control == Pio_Control_PeripheralC) {
			pio->reg->abcdsr1 = abcdsr1;
			pio->reg->abcdsr2 = abcdsr2 | pinMask;
		} else if (config->control == Pio_Control_PeripheralD) {
			pio->reg->abcdsr1 = abcdsr1 | pinMask;
			pio->reg->abcdsr2 = abcdsr2 | pinMask;
		} else {
			return returnError(errCode,
					Pio_ErrorCode_InvalidControlConfig);
		}

		pio->reg->pdr = pinMask;
	}

	return true;
}

static inline bool
setDirectionConfig(Pio *const pio, const uint32_t pinMask,
		const Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	switch (config->direction) {
	case Pio_Direction_Input:
		pio->reg->odr = pinMask;
		pio->reg->owdr = pinMask;
		break;
	case Pio_Direction_Output:
		pio->reg->oer = pinMask;
		pio->reg->owdr = pinMask;
		break;
	case Pio_Direction_SynchronousOutput:
		pio->reg->oer = pinMask;
		pio->reg->ower = pinMask;
		break;
	default:
		return returnError(
				errCode, Pio_ErrorCode_InvalidDirectionConfig);
	}

	return true;
}

static inline bool
setPullConfig(Pio *const pio, const uint32_t pinMask,
		const Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	switch (config->pull) {
	case Pio_Pull_None:
		pio->reg->pudr = pinMask;
		pio->reg->ppddr = pinMask;
		break;
	case Pio_Pull_Up:
		pio->reg->ppddr = pinMask;
		pio->reg->puer = pinMask;
		break;
	case Pio_Pull_Down:
		pio->reg->pudr = pinMask;
		pio->reg->ppder = pinMask;
		break;
	default: return returnError(errCode, Pio_ErrorCode_InvalidPullConfig);
	}

	return true;
}

static inline bool
setFilterConfig(Pio *const pio, const uint32_t pinMask,
		const Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	switch (config->filter) {
	case Pio_Filter_None:
		pio->reg->ifdr = pinMask;
		pio->reg->ifscdr = pinMask;
		break;
	case Pio_Filter_Glitch:
		pio->reg->ifscdr = pinMask;
		pio->reg->ifer = pinMask;
		break;
	case Pio_Filter_Debounce:
		pio->reg->ifscer = pinMask;
		pio->reg->ifer = pinMask;
		break;
	default: return returnError(errCode, Pio_ErrorCode_InvalidFilterConfig);
	}

	return true;
}

static inline void
setMultiDriveConfig(Pio *const pio, const uint32_t pinMask,
		const Pio_Pin_Config *const config)
{
	if (config->isMultiDriveEnabled)
		pio->reg->mder = pinMask;
	else
		pio->reg->mddr = pinMask;
}

static inline void
setSchmittTriggerConfig(Pio *const pio, const uint32_t pinMask,
		const Pio_Pin_Config *const config)
{
	volatile uint32_t schmitt = pio->reg->schmitt & ~pinMask;

	if (config->isSchmittTriggerDisabled)
		pio->reg->schmitt = schmitt | pinMask;
	else
		pio->reg->schmitt = schmitt;
}

static inline bool
setIrqConfig(Pio *const pio, const uint32_t pinMask,
		const Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	pio->reg->idr = pinMask;
	switch (config->irq) {
	case Pio_Irq_None: pio->reg->aimdr = pinMask; break;
	case Pio_Irq_EdgeBoth:
		pio->reg->aimdr = pinMask;
		pio->reg->ier = pinMask;
		break;
	case Pio_Irq_EdgeRising:
		pio->reg->aimer = pinMask;
		pio->reg->esr = pinMask;
		pio->reg->rehlsr = pinMask;
		pio->reg->ier = pinMask;
		break;
	case Pio_Irq_EdgeFalling:
		pio->reg->aimer = pinMask;
		pio->reg->esr = pinMask;
		pio->reg->fellsr = pinMask;
		pio->reg->ier = pinMask;
		break;
	case Pio_Irq_LevelLow:
		pio->reg->aimer = pinMask;
		pio->reg->lsr = pinMask;
		pio->reg->fellsr = pinMask;
		pio->reg->ier = pinMask;
		break;
	case Pio_Irq_LevelHigh:
		pio->reg->aimer = pinMask;
		pio->reg->lsr = pinMask;
		pio->reg->rehlsr = pinMask;
		pio->reg->ier = pinMask;
		break;
	default: return returnError(errCode, Pio_ErrorCode_InvalidIrqConfig);
	}

	return true;
}

static inline bool
setDriveStrengthConfig(Pio *const pio, const uint32_t pinMask,
		const Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	uint32_t driver = pio->reg->driver;

	if (config->driveStrength == Pio_Drive_High)
		driver |= pinMask;
	else if (config->driveStrength == Pio_Drive_Low)
		driver &= ~pinMask;
	else
		return returnError(errCode,
				Pio_ErrorCode_InvalidDriveStrengthConfig);

	pio->reg->driver = driver;

	return true;
}

bool
Pio_setPortConfig(Pio *const pio, const Pio_Port_Config *const config,
		ErrorCode *const errCode)
{
	if (!Pio_setPinsConfig(pio, config->pins, &config->pinsConfig, errCode))
		return false;

	// Set the slow clock divider for the debounce filter
	pio->reg->scdr = ((uint32_t)config->debounceFilterDiv
					 & PIO_SCDR_DIV_MASK)
			<< PIO_SCDR_DIV_OFFSET;

	return true;
}

bool
Pio_getPortConfig(const Pio *const pio, Pio_Port_Config *const config,
		ErrorCode *const errCode)
{
	config->debounceFilterDiv = (pio->reg->scdr & PIO_SCDR_DIV_MASK)
			>> PIO_SCDR_DIV_OFFSET;

	return Pio_getPinsConfig(
			pio, config->pins, &config->pinsConfig, errCode);
}

bool
Pio_setPinsConfig(Pio *const pio, const uint32_t pinMask,
		const Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	if (pinMask == 0u)
		return returnError(errCode, Pio_ErrorCode_InvalidPinMask);

	setMultiDriveConfig(pio, pinMask, config);
	setSchmittTriggerConfig(pio, pinMask, config);

	return setControlConfig(pio, pinMask, config, errCode)
			&& setDirectionConfig(pio, pinMask, config, errCode)
			&& setPullConfig(pio, pinMask, config, errCode)
			&& setFilterConfig(pio, pinMask, config, errCode)
			&& setIrqConfig(pio, pinMask, config, errCode)
			&& setDriveStrengthConfig(
					pio, pinMask, config, errCode);
}

static inline bool
detectPeripheral(Pio_Pin_Config *const config, const uint32_t abcdsr1,
		const uint32_t abcdsr2, const uint32_t pinMask)
{
	if ((abcdsr1 == 0u) && (abcdsr2 == 0u))
		config->control = Pio_Control_PeripheralA;
	else if ((abcdsr1 == pinMask) && (abcdsr2 == 0u))
		config->control = Pio_Control_PeripheralB;
	else if ((abcdsr1 == 0u) && (abcdsr2 == pinMask))
		config->control = Pio_Control_PeripheralC;
	else if ((abcdsr1 == pinMask) && (abcdsr2 == pinMask))
		config->control = Pio_Control_PeripheralD;
	else
		return false;

	return true;
}

static bool
getControlConfig(const Pio *const pio, const uint32_t pinMask,
		Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	const uint32_t psr = pio->reg->psr & pinMask;
	const uint32_t abcdsr1 = pio->reg->abcdsr1 & pinMask;
	const uint32_t abcdsr2 = pio->reg->abcdsr2 & pinMask;

	if (psr == pinMask)
		config->control = Pio_Control_Pio;
	else if (psr == 0u) {
		if (!detectPeripheral(config, abcdsr1, abcdsr2, pinMask))
			return returnError(errCode,
					Pio_ErrorCode_ControlConfigMismatch);
	} else
		return returnError(
				errCode, Pio_ErrorCode_ControlConfigMismatch);

	return true;
}

static bool
getPullConfig(const Pio *const pio, const uint32_t pinMask,
		Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	const uint32_t pusr = pio->reg->pusr & pinMask;
	const uint32_t ppdsr = pio->reg->ppdsr & pinMask;

	if ((pusr == pinMask) && (ppdsr == pinMask))
		config->pull = Pio_Pull_None;
	else if ((pusr == 0u) && (ppdsr == pinMask))
		config->pull = Pio_Pull_Up;
	else if ((pusr == pinMask) && (ppdsr == 0u))
		config->pull = Pio_Pull_Down;
	else
		return returnError(errCode, Pio_ErrorCode_PullConfigMismatch);

	return true;
}

static bool
getDirectionConfig(const Pio *const pio, const uint32_t pinMask,
		Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	const uint32_t osr = pio->reg->osr & pinMask;
	const uint32_t owsr = pio->reg->owsr & pinMask;

	if ((osr == 0u) && (owsr == 0u))
		config->direction = Pio_Direction_Input;
	else if ((osr == pinMask) && (owsr == 0u))
		config->direction = Pio_Direction_Output;
	else if ((osr == pinMask) && (owsr == pinMask))
		config->direction = Pio_Direction_SynchronousOutput;
	else
		return returnError(
				errCode, Pio_ErrorCode_DirectionConfigMismatch);

	return true;
}

static bool
getFilterConfig(const Pio *const pio, const uint32_t pinMask,
		Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	const uint32_t ifsr = pio->reg->ifsr & pinMask;
	const uint32_t ifscsr = pio->reg->ifscsr & pinMask;

	if ((ifsr == 0u) && (ifscsr == 0u))
		config->filter = Pio_Filter_None;
	else if ((ifsr == pinMask) && (ifscsr == 0u))
		config->filter = Pio_Filter_Glitch;
	else if ((ifsr == pinMask) && (ifscsr == pinMask))
		config->filter = Pio_Filter_Debounce;
	else
		return returnError(errCode, Pio_ErrorCode_FilterConfigMismatch);

	return true;
}

static bool
getMultiDriveConfig(const Pio *const pio, const uint32_t pinMask,
		Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	const uint32_t mdsr = pio->reg->mdsr & pinMask;

	if (mdsr == pinMask)
		config->isMultiDriveEnabled = true;
	else if (mdsr == 0u)
		config->isMultiDriveEnabled = false;
	else
		return returnError(errCode,
				Pio_ErrorCode_MultiDriveConfigMismatch);

	return true;
}

static bool
getSchmittTriggerConfig(const Pio *const pio, const uint32_t pinMask,
		Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	const uint32_t schmitt = pio->reg->schmitt & pinMask;

	if (schmitt == pinMask)
		config->isSchmittTriggerDisabled = true;
	else if (schmitt == 0u)
		config->isSchmittTriggerDisabled = false;
	else
		return returnError(errCode,
				Pio_ErrorCode_SchmittTriggerConfigMismatch);

	return true;
}

static inline bool
edgeLevelIrqType(const uint32_t elsr, const uint32_t frlhsr,
		const uint32_t pinMask, Pio_Pin_Config *const config,
		ErrorCode *const errCode)
{
	if ((elsr == 0u) && (frlhsr == pinMask))
		config->irq = Pio_Irq_EdgeRising;
	else if ((elsr == 0u) && (frlhsr == 0u))
		config->irq = Pio_Irq_EdgeFalling;
	else if ((elsr == pinMask) && (frlhsr == pinMask))
		config->irq = Pio_Irq_LevelHigh;
	else if ((elsr == pinMask) && (frlhsr == 0u))
		config->irq = Pio_Irq_LevelLow;
	else
		return returnError(errCode, Pio_ErrorCode_IrqConfigMismatch);

	return true;
}

static bool
getIrqConfig(const Pio *const pio, const uint32_t pinMask,
		Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	const uint32_t imr = pio->reg->imr & pinMask;
	const uint32_t aimmr = pio->reg->aimmr & pinMask;
	const uint32_t elsr = pio->reg->elsr & pinMask;
	const uint32_t frlhsr = pio->reg->frlhsr & pinMask;

	if ((imr == 0u) && (aimmr == 0u))
		config->irq = Pio_Irq_None;
	else if ((imr == pinMask) && (aimmr == 0u))
		config->irq = Pio_Irq_EdgeBoth;
	else if ((imr == pinMask) && (aimmr == pinMask)) {
		if (!edgeLevelIrqType(elsr, frlhsr, pinMask, config, errCode))
			return false;
	} else
		return returnError(errCode, Pio_ErrorCode_IrqConfigMismatch);

	return true;
}

static bool
getDriveStrengthConfig(const Pio *const pio, const uint32_t pinMask,
		Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	const uint32_t driver = pio->reg->driver & pinMask;

	if (driver == pinMask)
		config->driveStrength = Pio_Drive_High;
	else if (driver == 0u)
		config->driveStrength = Pio_Drive_Low;
	else
		return returnError(errCode,
				Pio_ErrorCode_DriveStrengthConfigMismatch);

	return true;
}

bool
Pio_getPinsConfig(const Pio *const pio, const uint32_t pinMask,
		Pio_Pin_Config *const config, ErrorCode *const errCode)
{
	if (pinMask == 0u)
		return returnError(errCode, Pio_ErrorCode_InvalidPinMask);

	return getControlConfig(pio, pinMask, config, errCode)
			&& getPullConfig(pio, pinMask, config, errCode)
			&& getDirectionConfig(pio, pinMask, config, errCode)
			&& getFilterConfig(pio, pinMask, config, errCode)
			&& getMultiDriveConfig(pio, pinMask, config, errCode)
			&& getSchmittTriggerConfig(
					pio, pinMask, config, errCode)
			&& getIrqConfig(pio, pinMask, config, errCode)
			&& getDriveStrengthConfig(
					pio, pinMask, config, errCode);
}

void
Pio_setPins(Pio *const pio, const uint32_t pinMask)
{
	pio->reg->sodr = pinMask;
}

void
Pio_resetPins(Pio *const pio, const uint32_t pinMask)
{
	pio->reg->codr = pinMask;
}

uint32_t
Pio_getPins(const Pio *const pio)
{
	return pio->reg->pdsr;
}

void
Pio_setPortValue(Pio *const pio, const uint32_t value)
{
	pio->reg->odsr = value;
}

uint32_t
Pio_getIrqStatus(const Pio *const pio)
{
	return pio->reg->isr;
}
