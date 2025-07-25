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

/// \brief Header containing default PMC and EEFC configuration.

#ifndef SYSTEM_CONFIG
#define SYSTEM_CONFIG

#include <Pmc/Pmc.h>
#include <Utils/Utils.h>

/// \brief Default processor core clock (HCLK) frequency in [Hz].
enum { SystemConfig_DefaultCoreClock = 150000000 };

/// \brief Default peripheral clock (MCK) frequency in [Hz].
enum { SystemConfig_DefaultPeriphClock = 75000000 };

/// \brief Base address of the embedded flash.
enum { SystemConfig_FlashBaseAddress = 0x00400000u };

/// \brief Returns the default PMC configuration:
///        Main clock source   RC Osc.
///        RC Osc. frequency   12 MHz
///        PLL multiplier      25
///        PLL divider         1
///        Master clock source PLL
///        Master clock presc. 2
///        Master clock div.   2
/// \returns default PMC configuration
static inline Pmc_Config
SystemConfig_getPmcDefaultConfig(void)
{
	Pmc_Config config = { 0 };
	config.mainckSrc = Pmc_MainckSrc_RcOsc;
	config.rcOscFreq = Pmc_RcOscFreq_12M;
	config.pllaMul = 25 - 1;
	config.pllaDiv = 1;
	config.pllaStartupTime = 60;
	config.masterckSrc = Pmc_MasterckSrc_Pllack;
	config.masterckPresc = Pmc_MasterckPresc_2; // Core clk at 150 MHz
	config.masterckDiv = Pmc_MasterckDiv_2; // Peripherals at 75 MHz.
	config.peripheralClkEnabledFlags = 0;

	for (uint32_t i = 0; i < PMC_PCK_CLOCKS; i++)
		config.pckConfig[i].isEnabled = false;

	// Configure CAMERA PCK to be between 10 and 48 MHz.
	config.pckConfig[0].isEnabled = true;
	config.pckConfig[0].pckSrc = Pmc_PckSrc_Pllack;
	config.pckConfig[0].pckPresc = 30;

	// The UART/USART PCK shall be 3 times slower that peripheral clock.
	config.pckConfig[4].isEnabled = true;
	config.pckConfig[4].pckSrc = Pmc_PckSrc_Pllack;
	config.pckConfig[4].pckPresc = 12;

	// The CAN clock speed should not exceed the peripheral clock.
	config.pckConfig[5].isEnabled = true;
	config.pckConfig[5].pckSrc = Pmc_PckSrc_Pllack;
	config.pckConfig[5].pckPresc = 14; // 300MHz / 15 = 20MHz (datasheet)

	return config;
}

#endif // SYSTEM_CONFIG
