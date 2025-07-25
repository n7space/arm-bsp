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

/// \file NvicVectorTable.h
/// \addtogroup Bsp
/// \brief Header describing the vector table for the Cortex-M7 processor.

#ifndef BSP_NVIC_VECTORTABLE_H
#define BSP_NVIC_VECTORTABLE_H

#include "Nvic.h"

/// \brief Structure describing the layout of the vector table.
typedef struct {
	void *initialStackPointer; ///< Initial value of the stack pointer.
	Nvic_InterruptHandler
			resetHandler; ///< Initial value of the program counter (reset handler).
	Nvic_InterruptHandler
			nmiHandler; ///< Handler of the non-maskable interrupt exception.
	Nvic_InterruptHandler
			hardFaultHandler; ///< Handler of the hard fault exception.
	Nvic_InterruptHandler
			memManageHandler; ///< Handler of the memory management fault exception.
	Nvic_InterruptHandler
			busFaultHandler; ///< Handler of the bus fault exception.
	Nvic_InterruptHandler
			usageFaultHandler; ///< Handler of the usage fault exception.
	Nvic_InterruptHandler reserved0[4]; ///< Reserved.
	Nvic_InterruptHandler
			svcHandler; ///< Handler of the supervisor call exception.
	Nvic_InterruptHandler
			debugMonHandler; ///< Handler of the debug monitor exception.
	Nvic_InterruptHandler reserved1; ///< Reserved.
	Nvic_InterruptHandler pendSvHandler; ///< Handler of the PendSV request.
	Nvic_InterruptHandler
			sysTickHandler; ///< Handler of the system timer exception.
	Nvic_InterruptHandler irqHandlers
			[Nvic_InterruptCount]; ///< Table of interrupt handlers.
} Nvic_VectorTable;

#endif // BSP_NVIC_VECTORTABLE_H
