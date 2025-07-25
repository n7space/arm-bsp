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

#ifndef BSP_PMC_PERIPHERALID_H
#define BSP_PMC_PERIPHERALID_H

/// \brief Enumeration used to select the peripheral during Pmc operations.
typedef enum {
#if defined(N7S_TARGET_SAMV71Q21)
	Pmc_PeripheralId_Uart0 = 7,
	Pmc_PeripheralId_Uart1 = 8,
	Pmc_PeripheralId_Smc = 9,
	Pmc_PeripheralId_PioA = 10,
	Pmc_PeripheralId_PioB = 11,
	Pmc_PeripheralId_PioC = 12,
	Pmc_PeripheralId_Usart0 = 13,
	Pmc_PeripheralId_Usart1 = 14,
	Pmc_PeripheralId_Usart2 = 15,
	Pmc_PeripheralId_PioD = 16,
	Pmc_PeripheralId_PioE = 17,
	Pmc_PeripheralId_Hsmci = 18,
	Pmc_PeripheralId_Twihs0 = 19,
	Pmc_PeripheralId_Twihs1 = 20,
	Pmc_PeripheralId_Spi0 = 21,
	Pmc_PeripheralId_Ssc = 22,
	Pmc_PeripheralId_Tc0Ch0 = 23,
	Pmc_PeripheralId_Tc0Ch1 = 24,
	Pmc_PeripheralId_Tc0Ch2 = 25,
	Pmc_PeripheralId_Tc1Ch0 = 26,
	Pmc_PeripheralId_Tc1Ch1 = 27,
	Pmc_PeripheralId_Tc1Ch2 = 28,
	Pmc_PeripheralId_Afec0 = 29,
	Pmc_PeripheralId_Dacc = 30,
	Pmc_PeripheralId_Pwm0 = 31,
	Pmc_PeripheralId_Icm = 32,
	Pmc_PeripheralId_Acc = 33,
	Pmc_PeripheralId_Usbhs = 34,
	Pmc_PeripheralId_Mcan0 = 35,
	Pmc_PeripheralId_Mcan1 = 37,
	Pmc_PeripheralId_Gmac = 39,
	Pmc_PeripheralId_Afec1 = 40,
	Pmc_PeripheralId_Twihs2 = 41,
	Pmc_PeripheralId_Spi1 = 42,
	Pmc_PeripheralId_Qspi = 43,
	Pmc_PeripheralId_Uart2 = 44,
	Pmc_PeripheralId_Uart3 = 45,
	Pmc_PeripheralId_Uart4 = 46,
	Pmc_PeripheralId_Tc2Ch0 = 47,
	Pmc_PeripheralId_Tc2Ch1 = 48,
	Pmc_PeripheralId_Tc2Ch2 = 49,
	Pmc_PeripheralId_Tc3Ch0 = 50,
	Pmc_PeripheralId_Tc3Ch1 = 51,
	Pmc_PeripheralId_Tc3Ch2 = 52,
	Pmc_PeripheralId_Mlb = 53,
	Pmc_PeripheralId_Aes = 56,
	Pmc_PeripheralId_Trng = 57,
	Pmc_PeripheralId_Xdmac = 58,
	Pmc_PeripheralId_Isi = 59,
	Pmc_PeripheralId_Pwm1 = 60,
	Pmc_PeripheralId_Sdramc = 62,
	Pmc_PeripheralCount = 128,
#elif defined(N7S_TARGET_SAMRH71F20)
	Pmc_PeripheralId_Flexcom0 = 7,
	Pmc_PeripheralId_Flexcom1 = 8,
	Pmc_PeripheralId_PioA = 10,
	Pmc_PeripheralId_Flexcom2 = 13,
	Pmc_PeripheralId_Flexcom3 = 14,
	Pmc_PeripheralId_Flexcom4 = 15,
	Pmc_PeripheralId_Flexcom5 = 22,
	Pmc_PeripheralId_Flexcom6 = 23,
	Pmc_PeripheralId_Flexcom7 = 24,
	Pmc_PeripheralId_Tc0Ch0 = 25,
	Pmc_PeripheralId_Tc0Ch1 = 26,
	Pmc_PeripheralId_Tc0Ch2 = 27,
	Pmc_PeripheralId_Tc1Ch0 = 28,
	Pmc_PeripheralId_Tc1Ch1 = 29,
	Pmc_PeripheralId_Tc1Ch2 = 30,
	Pmc_PeripheralId_Pwm0 = 31,
	Pmc_PeripheralId_Pwm1 = 32,
	Pmc_PeripheralId_Icm = 33,
	Pmc_PeripheralId_Mcan0 = 36,
	Pmc_PeripheralId_Mcan1 = 38,
	Pmc_PeripheralId_Sha = 44,
	Pmc_PeripheralId_Flexcom8 = 45,
	Pmc_PeripheralId_Flexcom9 = 46,
	Pmc_PeripheralId_Qspi = 49,
	Pmc_PeripheralId_Hefc = 50,
	Pmc_PeripheralId_Hefc_Fix = 51,
	Pmc_PeripheralId_Hefc_NoFix = 52,
	Pmc_PeripheralId_Tc2Ch0 = 53,
	Pmc_PeripheralId_Tc2Ch1 = 54,
	Pmc_PeripheralId_Tc2Ch2 = 55,
	Pmc_PeripheralId_Tc3Ch0 = 56,
	Pmc_PeripheralId_Tc3Ch1 = 57,
	Pmc_PeripheralId_Tc3Ch2 = 58,
	Pmc_PeripheralId_Hemc = 59,
	Pmc_PeripheralId_Hemc_Fix = 60,
	Pmc_PeripheralId_Hemc_NoFix = 61,
	Pmc_PeripheralId_Sfr = 62,
	Pmc_PeripheralId_Trng = 63,
	Pmc_PeripheralId_Xdmac = 64,
	Pmc_PeripheralId_Spw0 = 65,
	Pmc_PeripheralId_Spw1 = 66,
	Pmc_PeripheralId_Ip1553 = 68,
	Pmc_PeripheralId_Gmac = 69,
	Pmc_PeripheralCount = 128,
#elif defined(N7S_TARGET_SAMRH707F18)
	Pmc_PeripheralId_Flexcom0 = 7,
	Pmc_PeripheralId_Flexcom1 = 8,
	Pmc_PeripheralId_PioA = 10,
	Pmc_PeripheralId_Flexcom2 = 13,
	Pmc_PeripheralId_Flexcom3 = 14,
	Pmc_PeripheralId_CrcCu = 22,
	Pmc_PeripheralId_Dac = 24,
	Pmc_PeripheralId_Tc0Ch0 = 25,
	Pmc_PeripheralId_Tc0Ch1 = 26,
	Pmc_PeripheralId_Tc0Ch2 = 27,
	Pmc_PeripheralId_Tc1Ch0 = 28,
	Pmc_PeripheralId_Tc1Ch1 = 29,
	Pmc_PeripheralId_Tc1Ch2 = 30,
	Pmc_PeripheralId_Pwm0 = 31,
	Pmc_PeripheralId_Pwm1 = 32,
	Pmc_PeripheralId_Icm = 33,
	Pmc_PeripheralId_Mcan0 = 36,
	Pmc_PeripheralId_Mcan1 = 38,
	Pmc_PeripheralId_Sha = 44,
	Pmc_PeripheralId_Pcc = 45,
	Pmc_PeripheralId_Hefc = 50,
	Pmc_PeripheralId_Tc2Ch0 = 53,
	Pmc_PeripheralId_Tc2Ch1 = 54,
	Pmc_PeripheralId_Tc2Ch2 = 55,
	Pmc_PeripheralId_Hemc = 59,
	Pmc_PeripheralId_Hemc_Fix = 60,
	Pmc_PeripheralId_Hemc_NoFix = 61,
	Pmc_PeripheralId_Trng = 63,
	Pmc_PeripheralId_Xdmac = 64,
	Pmc_PeripheralId_Spw0 = 65,
	Pmc_PeripheralId_Spw1 = 66,
	Pmc_PeripheralId_Ip1553 = 68,
	Pmc_PeripheralCount = 80,
#else
#error "No target platform specified (missing N7S_TARGET_* macro)"
#endif
} Pmc_PeripheralId;

#endif // BSP_PMC_PERIPHERALID_H
