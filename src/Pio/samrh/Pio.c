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

#include "Pio.h"

#include <Utils/Bits.h>

bool Pio_init(const Pio_Port port, Pio* const pio, ErrorCode* const errCode)
{
  const uint32_t registerAddress = PIOA_ADDRESS_BASE + PIO_COMMON_REGISTERS_OFFSET;

  pio->commonReg = (Pio_CommonRegisters*)(registerAddress);
  switch (port)
  {
  case Pio_Port_A:
    pio->port = Pio_Port_A;
    pio->reg = (Pio_Registers*)PIOA_ADDRESS_BASE;
    break;
  case Pio_Port_B:
    pio->port = Pio_Port_B;
    pio->reg = (Pio_Registers*)PIOB_ADDRESS_BASE;
    break;
  case Pio_Port_C:
    pio->port = Pio_Port_C;
    pio->reg = (Pio_Registers*)PIOC_ADDRESS_BASE;
    break;
  case Pio_Port_D:
    pio->port = Pio_Port_D;
    pio->reg = (Pio_Registers*)PIOD_ADDRESS_BASE;
    break;
#if defined(N7S_TARGET_SAMRH71F20)
  case Pio_Port_E:
    pio->port = Pio_Port_E;
    pio->reg = (Pio_Registers*)PIOE_ADDRESS_BASE;
    break;
  case Pio_Port_F:
    pio->port = Pio_Port_F;
    pio->reg = (Pio_Registers*)PIOF_ADDRESS_BASE;
    break;
  case Pio_Port_G:
    pio->port = Pio_Port_G;
    pio->reg = (Pio_Registers*)PIOG_ADDRESS_BASE;
    break;
#endif
  default:
    return returnError(errCode, Pio_ErrorCode_InvalidPortId);
  }

  return true;
}

static inline bool setControlConfig(Pio* const pio,
  const Pio_Pin_Config* const config,
  ErrorCode* const errCode)
{
  if (config->control >= Pio_Control_ModeCount)
  {
    return returnError(errCode, Pio_ErrorCode_InvalidControlConfig);
  }
  setValueAtOffset(
    &pio->reg->cfgr, (uint32_t)config->control, PIO_CFGR_FUNC_OFFSET, PIO_CFGR_FUNC_MASK);
  return true;
}

static inline bool setDirectionConfig(Pio* const pio,
  const Pio_Pin_Config* const config,
  ErrorCode* const errCode)
{
  if (config->direction == Pio_Direction_Input)
  {
    setValueAtOffset(&pio->reg->cfgr, 0u, PIO_CFGR_DIR_OFFSET, PIO_CFGR_DIR_MASK);
  }
  else if ((config->direction == Pio_Direction_Output)
           || (config->direction == Pio_Direction_SynchronousOutput))
  {
    setValueAtOffset(&pio->reg->cfgr, 1u, PIO_CFGR_DIR_OFFSET, PIO_CFGR_DIR_MASK);
  }
  else
  {
    return returnError(errCode, Pio_ErrorCode_InvalidDirectionConfig);
  }
  return true;
}

static inline bool setPullConfig(Pio* const pio,
  const Pio_Pin_Config* const config,
  ErrorCode* const errCode)
{
  switch (config->pull)
  {
  case Pio_Pull_None:
    setValueAtOffset(&pio->reg->cfgr, 0u, PIO_CFGR_PUEN_OFFSET, PIO_CFGR_PUEN_MASK);
    setValueAtOffset(&pio->reg->cfgr, 0u, PIO_CFGR_PDEN_OFFSET, PIO_CFGR_PDEN_MASK);
    break;
  case Pio_Pull_Up:
    setValueAtOffset(&pio->reg->cfgr, 0u, PIO_CFGR_PDEN_OFFSET, PIO_CFGR_PDEN_MASK);
    setValueAtOffset(&pio->reg->cfgr, 1u, PIO_CFGR_PUEN_OFFSET, PIO_CFGR_PUEN_MASK);
    break;
  case Pio_Pull_Down:
    setValueAtOffset(&pio->reg->cfgr, 0u, PIO_CFGR_PUEN_OFFSET, PIO_CFGR_PUEN_MASK);
    setValueAtOffset(&pio->reg->cfgr, 1u, PIO_CFGR_PDEN_OFFSET, PIO_CFGR_PDEN_MASK);
    break;
  default:
    return returnError(errCode, Pio_ErrorCode_InvalidPullConfig);
  }

  return true;
}

static inline void setOpenDrain(Pio* const pio, const Pio_Pin_Config* const config)
{
  if (config->isOpenDrainEnabled)
  {
    setValueAtOffset(&pio->reg->cfgr, 1u, PIO_CFGR_OPD_OFFSET, PIO_CFGR_OPD_MASK);
  }
  else
  {
    setValueAtOffset(&pio->reg->cfgr, 0u, PIO_CFGR_OPD_OFFSET, PIO_CFGR_OPD_MASK);
  }
}

static inline void setSchmittTriggerConfig(Pio* const pio, const Pio_Pin_Config* const config)
{
  if (config->isSchmittTriggerDisabled)
  {
    setValueAtOffset(&pio->reg->cfgr, 1u, PIO_CFGR_SCHMITT_OFFSET, PIO_CFGR_SCHMITT_MASK);
  }
  else
  {
    setValueAtOffset(&pio->reg->cfgr, 0u, PIO_CFGR_SCHMITT_OFFSET, PIO_CFGR_SCHMITT_MASK);
  }
}

static inline bool setIrqConfig(Pio* const pio,
  const uint32_t pinMask,
  const Pio_Pin_Config* const config,
  ErrorCode* const errCode)
{
  pio->reg->idr = pinMask;
  if (config->irq >= Pio_Irq_ModeCount)
  {
    return returnError(errCode, Pio_ErrorCode_InvalidIrqConfig);
  }

  setValueAtOffset(
    &pio->reg->cfgr, (uint32_t)(config->irq), PIO_CFGR_EVTSEL_OFFSET, PIO_CFGR_EVTSEL_MASK);

  if (config->isIrqEnabled)
    pio->reg->ier = pinMask;

  return true;
}

static inline bool setDriveStrengthConfig(Pio* pio,
  const Pio_Pin_Config* const config,
  ErrorCode* const errCode)
{
  if (config->driveStrength >= Pio_Current_ModeCount)
  {
    return returnError(errCode, Pio_ErrorCode_InvalidDriveStrengthConfig);
  }
  setValueAtOffset(&pio->reg->cfgr,
    (uint32_t)(config->driveStrength),
    PIO_CFGR_DRVSTR_OFFSET,
    PIO_CFGR_DRVSTR_MASK);

  return true;
}

bool Pio_setPortConfig(Pio* const pio,
  const Pio_Port_Config* const config,
  ErrorCode* const errCode)
{
  if (!Pio_setPinsConfig(pio, config->pins, &config->pinsConfig, errCode))
    return false;

  // Set the slow clock divider for the debounce filter
  setValueAtOffset(&pio->commonReg->scdr,
    (uint32_t)config->debounceFilterDiv,
    PIO_SCDR_DIV_OFFSET,
    PIO_SCDR_DIV_MASK);

  return true;
}

bool Pio_setPinsConfig(Pio* const pio,
  const uint32_t pinMask,
  const Pio_Pin_Config* const config,
  ErrorCode* const errCode)
{
  if (pinMask == 0u)
    return returnError(errCode, Pio_ErrorCode_InvalidPinMask);

  pio->reg->mskr = pinMask;

  setOpenDrain(pio, config);
  setSchmittTriggerConfig(pio, config);

  return setControlConfig(pio, config, errCode) && setDirectionConfig(pio, config, errCode)
      && setPullConfig(pio, config, errCode) && setIrqConfig(pio, pinMask, config, errCode)
      && setDriveStrengthConfig(pio, config, errCode);
}

void Pio_setPins(Pio* const pio, const uint32_t pinMask)
{
  pio->reg->sodr = pinMask;
}

void Pio_resetPins(Pio* const pio, const uint32_t pinMask)
{
  pio->reg->codr = pinMask;
}

uint32_t Pio_getPins(const Pio* const pio)
{
  return pio->reg->pdsr;
}

void Pio_setPortMaskedValue(Pio* const pio, const uint32_t pinMask, const uint32_t value)
{
  pio->reg->mskr = pinMask;
  pio->reg->odsr = value;
}

void Pio_setPortValue(Pio* const pio, const uint32_t value)
{
  const uint32_t pinMask = 0xffffffffu;
  Pio_setPortMaskedValue(pio, pinMask, value);
}

uint32_t Pio_getIrqStatus(const Pio* const pio)
{
  return pio->reg->isr;
}

bool Pio_getPortConfig(const Pio* const pio,
  Pio_Port_Config* const config,
  ErrorCode* const errCode)
{
  config->debounceFilterDiv = (pio->commonReg->scdr & PIO_SCDR_DIV_MASK) >> PIO_SCDR_DIV_OFFSET;

  return Pio_getPinsConfig(pio, config->pins, &config->pinsConfig, errCode);
}

bool Pio_getPinsConfig(const Pio* const pio,
  const uint32_t pinMask,
  Pio_Pin_Config* const config,
  ErrorCode* const errCode)
{
  // Check if pinMask contains only 1 set bit
  if ((pinMask == 0u) || ((pinMask & (pinMask - 1u)) != 0u))
    return returnError(errCode, Pio_ErrorCode_InvalidPinMask);

  pio->reg->mskr = pinMask;
  uint32_t cfgr = pio->reg->cfgr;
  config->control = (Pio_Control)((cfgr & PIO_CFGR_FUNC_MASK) >> PIO_CFGR_FUNC_OFFSET);
  config->direction = (Pio_Direction)((cfgr & PIO_CFGR_DIR_MASK) >> PIO_CFGR_DIR_OFFSET);
  config->driveStrength = (Pio_Current)((cfgr & PIO_CFGR_DRVSTR_MASK) >> PIO_CFGR_DRVSTR_OFFSET);
  config->irq = (Pio_Irq)((cfgr & PIO_CFGR_EVTSEL_MASK) >> PIO_CFGR_EVTSEL_OFFSET);
  config->isIrqEnabled = (pio->reg->imr & pinMask) != 0u;
  config->isOpenDrainEnabled = (cfgr & PIO_CFGR_OPD_MASK) > 0u;
  config->isSchmittTriggerDisabled = (cfgr & PIO_CFGR_SCHMITT_MASK) > 0u;

  const bool pull_up_enabled = (cfgr & PIO_CFGR_PUEN_MASK) > 0u;
  const bool pull_down_enabled = (cfgr & PIO_CFGR_PDEN_MASK) > 0u;

  // Pull-up has higher priority, if both PUEN and PDEN are set, only pull-up is enabled
  if ((!pull_up_enabled) && (!pull_down_enabled))
    config->pull = Pio_Pull_None;
  else if (pull_up_enabled)
    config->pull = Pio_Pull_Up;
  else
    config->pull = Pio_Pull_Down;

  return true;
}
