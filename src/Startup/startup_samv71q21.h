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

/// \brief The stub file for startup code

#ifndef BSP_STARTUP_H
#define BSP_STARTUP_H

#include <stdint.h>

#include <Nvic/Nvic.h>

typedef struct {
	/* Stack pointer */
	void *pvStack;

	/* Cortex-M handlers */
	Nvic_InterruptHandler pfnReset_Handler;
	Nvic_InterruptHandler pfnNMI_Handler;
	Nvic_InterruptHandler pfnHardFault_Handler;
	Nvic_InterruptHandler pfnMemManage_Handler;
	Nvic_InterruptHandler pfnBusFault_Handler;
	Nvic_InterruptHandler pfnUsageFault_Handler;
	Nvic_InterruptHandler pfnReserved1_Handler;
	Nvic_InterruptHandler pfnReserved2_Handler;
	Nvic_InterruptHandler pfnReserved3_Handler;
	Nvic_InterruptHandler pfnReserved4_Handler;
	Nvic_InterruptHandler pfnSVC_Handler;
	Nvic_InterruptHandler pfnDebugMon_Handler;
	Nvic_InterruptHandler pfnReserved5_Handler;
	Nvic_InterruptHandler pfnPendSV_Handler;
	Nvic_InterruptHandler pfnSysTick_Handler;

	/* Peripheral handlers */
	Nvic_InterruptHandler pfnSUPC_Handler; /*  0 Supply Controller */
	Nvic_InterruptHandler pfnRSTC_Handler; /*  1 Reset Controller */
	Nvic_InterruptHandler pfnRTC_Handler; /*  2 Real Time Clock */
	Nvic_InterruptHandler pfnRTT_Handler; /*  3 Real Time Timer */
	Nvic_InterruptHandler pfnWDT_Handler; /*  4 Watchdog Timer */
	Nvic_InterruptHandler
			pfnPMC_Handler; /*  5 Power Management Controller */
	Nvic_InterruptHandler pfnEFC_Handler; /*  6 Enhanced Embedded Flash
						 Controller */
	Nvic_InterruptHandler pfnUART0_Handler; /*  7 UART 0 */
	Nvic_InterruptHandler pfnUART1_Handler; /*  8 UART 1 */
	Nvic_InterruptHandler pvReserved9; /* 9 reserved */
	Nvic_InterruptHandler
			pfnPIOA_Handler; /* 10 Parallel I/O Controller A */
	Nvic_InterruptHandler
			pfnPIOB_Handler; /* 11 Parallel I/O Controller B */
	Nvic_InterruptHandler
			pfnPIOC_Handler; /* 12 Parallel I/O Controller C */
	Nvic_InterruptHandler pfnUSART0_Handler; /* 13 USART 0 */
	Nvic_InterruptHandler pfnUSART1_Handler; /* 14 USART 1 */
	Nvic_InterruptHandler pfnUSART2_Handler; /* 15 USART 2 */
	Nvic_InterruptHandler
			pfnPIOD_Handler; /* 16 Parallel I/O Controller D */
	Nvic_InterruptHandler
			pfnPIOE_Handler; /* 17 Parallel I/O Controller E */
	Nvic_InterruptHandler
			pfnHSMCI_Handler; /* 18 Multimedia Card Interface */
	Nvic_InterruptHandler
			pfnTWIHS0_Handler; /* 19 Two Wire Interface 0 HS */
	Nvic_InterruptHandler
			pfnTWIHS1_Handler; /* 20 Two Wire Interface 1 HS */
	Nvic_InterruptHandler
			pfnSPI0_Handler; /* 21 Serial Peripheral Interface 0 */
	Nvic_InterruptHandler
			pfnSSC_Handler; /* 22 Synchronous Serial Controller */
	Nvic_InterruptHandler
			pfnTC0CH0_Handler; /* 23 Timer Counter 0 Channel 0 */
	Nvic_InterruptHandler
			pfnTC0CH1_Handler; /* 24 Timer Counter 0 Channel 1 */
	Nvic_InterruptHandler
			pfnTC0CH2_Handler; /* 25 Timer Counter 0 Channel 2 */
	Nvic_InterruptHandler
			pfnTC1CH0_Handler; /* 26 Timer Counter 1 Channel 0 */
	Nvic_InterruptHandler
			pfnTC1CH1_Handler; /* 27 Timer Counter 1 Channel 1 */
	Nvic_InterruptHandler
			pfnTC1CH2_Handler; /* 28 Timer Counter 1 Channel 2 */
	Nvic_InterruptHandler pfnAFEC0_Handler; /* 29 Analog Front End 0 */
	Nvic_InterruptHandler
			pfnDACC_Handler; /* 30 Digital To Analog Converter */
	Nvic_InterruptHandler pfnPWM0_Handler; /* 31 Pulse Width Modulation 0 */
	Nvic_InterruptHandler pfnICM_Handler; /* 32 Integrity Check Monitor */
	Nvic_InterruptHandler pfnACC_Handler; /* 33 Analog Comparator */
	Nvic_InterruptHandler
			pfnUSBHS_Handler; /* 34 USB Host / Device Controller */
	Nvic_InterruptHandler pfnMCAN0_Handler; /* 35 MCAN Controller 0 */
	Nvic_InterruptHandler pvReserved36; /* 36 reserved */
	Nvic_InterruptHandler pfnMCAN1_Handler; /* 37 MCAN Controller 1 */
	Nvic_InterruptHandler pvReserved38; /* 38 reserved */
	Nvic_InterruptHandler pfnGMAC_Handler; /* 39 Ethernet MAC */
	Nvic_InterruptHandler pfnAFEC1_Handler; /* 40 Analog Front End 1 */
	Nvic_InterruptHandler
			pfnTWIHS2_Handler; /* 41 Two Wire Interface 2 HS */
	Nvic_InterruptHandler
			pfnSPI1_Handler; /* 42 Serial Peripheral Interface 1 */
	Nvic_InterruptHandler pfnQSPI_Handler; /* 43 Quad I/O Serial Peripheral
						  Interface */
	Nvic_InterruptHandler pfnUART2_Handler; /* 44 UART 2 */
	Nvic_InterruptHandler pfnUART3_Handler; /* 45 UART 3 */
	Nvic_InterruptHandler pfnUART4_Handler; /* 46 UART 4 */
	Nvic_InterruptHandler
			pfnTC2CH0_Handler; /* 47 Timer Counter 2 Channel 0 */
	Nvic_InterruptHandler
			pfnTC2CH1_Handler; /* 48 Timer Counter 2 Channel 1 */
	Nvic_InterruptHandler
			pfnTC2CH2_Handler; /* 49 Timer Counter 2 Channel 2 */
	Nvic_InterruptHandler
			pfnTC3CH0_Handler; /* 50 Timer Counter 3 Channel 0 */
	Nvic_InterruptHandler
			pfnTC3CH1_Handler; /* 51 Timer Counter 3 Channel 1 */
	Nvic_InterruptHandler
			pfnTC3CH2_Handler; /* 52 Timer Counter 3 Channel 2 */
	Nvic_InterruptHandler pfnMLB_Handler; /* 53 MediaLB */
	Nvic_InterruptHandler pvReserved54; /* 54 reserved */
	Nvic_InterruptHandler pvReserved55; /* 55 reserved */
	Nvic_InterruptHandler pfnAES_Handler; /* 56 AES */
	Nvic_InterruptHandler pfnTRNG_Handler; /* 57 True Random Generator */
	Nvic_InterruptHandler pfnXDMAC_Handler; /* 58 DMA */
	Nvic_InterruptHandler pfnISI_Handler; /* 59 Camera Interface */
	Nvic_InterruptHandler pfnPWM1_Handler; /* 60 Pulse Width Modulation 1 */
	Nvic_InterruptHandler pvReserved61; /* 61 reserved */
	Nvic_InterruptHandler pfnSDRAMC_Handler; /* 62 SDRAM Controller */
	Nvic_InterruptHandler pfnRSWDT_Handler; /* 63 Reinforced Secure Watchdog
						   Timer */
} DeviceVectors;

#endif // BSP_STARTUP_H
