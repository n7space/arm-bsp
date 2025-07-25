/**@file
 * This file is part of the ARM BSP for the Test Environment.
 *
 * @copyright 2023-2024 N7 Space Sp. z o.o.
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

#ifndef N7S_BSP_PMC_CHIPCONFIG_H
#define N7S_BSP_PMC_CHIPCONFIG_H

/// \file  Pmc/ChipConfig.h
/// \brief Utility file to ease working with chip config .conf files.

#include "Pmc.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Constructs PMC configuration using constants from chip config .conf file.
/// \details The constants needs to be provided by the build system.
/// \returns PMC configuration.
static inline Pmc_Config
Pmc_ChipConfig_getConfig(void)
{
	const Pmc_Config config = {
    .mainck = {
      .src = N7S_BSP_PMC_MAINCK_SRC,
      .rcOscFreq = N7S_BSP_PMC_MAINCK_RC_OSC_FREQ,
      .xoscStartupTime = N7S_BSP_PMC_MAINCK_XOSC_STARTUP_TIME,
#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
      .detectClockFail = N7S_BSP_PMC_MAINCK_DETECT_CLOCK_FAIL,
      .xt32kMonitorEnabled = N7S_BSP_PMC_MAINCK_XT32K_MONITOR_ENABLED,
      .badCpuResetEnabled = N7S_BSP_PMC_MAINCK_BAD_CPU_RESET_ENABLED,
      .badCpuNmicIrqEnabled = N7S_BSP_PMC_MAINCK_BAD_CPU_NMIC_IRQ_ENABLED,
#endif
    },
    .pll = {
      .pllaMul = N7S_BSP_PMC_PLLA_MUL,
      .pllaDiv = N7S_BSP_PMC_PLLA_DIV,
      .pllaStartupTime = N7S_BSP_PMC_PLLA_STARTUP_TIME,
#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
      .pllaVco = N7S_BSP_PMC_PLLA_VCO,
      .pllaFilterCapacitor = N7S_BSP_PMC_PLLA_FILTER_CAPACITOR,
      .pllaFilterResistor = N7S_BSP_PMC_PLLA_FILTER_RESISTOR,
      .pllaCurrent = N7S_BSP_PMC_PLLA_CURRENT,
      .pllbSrc = N7S_BSP_PMC_PLLB_SRC,
      .pllbMul = N7S_BSP_PMC_PLLB_MUL,
      .pllbDiv = N7S_BSP_PMC_PLLB_DIV,
      .pllbStartupTime = N7S_BSP_PMC_PLLB_STARTUP_TIME,
      .pllbVco = N7S_BSP_PMC_PLLB_VCO,
      .pllbFilterCapacitor = N7S_BSP_PMC_PLLB_FILTER_CAPACITOR,
      .pllbFilterResistor = N7S_BSP_PMC_PLLB_FILTER_RESISTOR,
      .pllbCurrent = N7S_BSP_PMC_PLLB_CURRENT,
#endif
    },
    .masterck = {
      .src = N7S_BSP_PMC_MASTERCK_SRC,
      .presc = N7S_BSP_PMC_MASTERCK_PRESC,
      .divider = N7S_BSP_PMC_MASTERCK_DIVIDER,
    },
    .pck = {
      {
        .isEnabled = N7S_BSP_PMC_PCK_00_IS_ENABLED,
        .src = N7S_BSP_PMC_PCK_00_SRC,
        .presc = N7S_BSP_PMC_PCK_00_PRESC,
      },
      {
        .isEnabled = N7S_BSP_PMC_PCK_01_IS_ENABLED,
        .src = N7S_BSP_PMC_PCK_01_SRC,
        .presc = N7S_BSP_PMC_PCK_01_PRESC,
      },
      {
        .isEnabled = N7S_BSP_PMC_PCK_02_IS_ENABLED,
        .src = N7S_BSP_PMC_PCK_02_SRC,
        .presc = N7S_BSP_PMC_PCK_02_PRESC,
      },
      {
        .isEnabled = N7S_BSP_PMC_PCK_03_IS_ENABLED,
        .src = N7S_BSP_PMC_PCK_03_SRC,
        .presc = N7S_BSP_PMC_PCK_03_PRESC,
      },
#if defined(N7S_TARGET_SAMV71Q21)
      {
        .isEnabled = N7S_BSP_PMC_PCK_04_IS_ENABLED,
        .src = N7S_BSP_PMC_PCK_04_SRC,
        .presc = N7S_BSP_PMC_PCK_04_PRESC,
      },
      {
        .isEnabled = N7S_BSP_PMC_PCK_05_IS_ENABLED,
        .src = N7S_BSP_PMC_PCK_05_SRC,
        .presc = N7S_BSP_PMC_PCK_05_PRESC,
      },
      {
        .isEnabled = N7S_BSP_PMC_PCK_06_IS_ENABLED,
        .src = N7S_BSP_PMC_PCK_06_SRC,
        .presc = N7S_BSP_PMC_PCK_06_PRESC,
      },
      {
        .isEnabled = N7S_BSP_PMC_PCK_07_IS_ENABLED,
        .src = N7S_BSP_PMC_PCK_07_SRC,
        .presc = N7S_BSP_PMC_PCK_07_PRESC,
      },
#endif // N7S_TARGET_SAMV71Q21
    },
#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
    .rc2Osc = {
      .isEnabled = N7S_BSP_PMC_PCK_RC2_OSC_IS_ENABLED,
      .frequency = N7S_BSP_PMC_PCK_RC2_OSC_FREQUENCY,
    },
#endif // N7S_TARGET_SAMRH71F20
  };
	return config;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // N7S_BSP_PMC_CHIPCONFIG_H
