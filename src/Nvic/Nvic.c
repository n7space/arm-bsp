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

#include "Nvic.h"

#include <assert.h>

#include <Scb/Scb.h>

#include "NvicRegisters.h"
#include "NvicVectorTable.h"

// cppcheck-suppress misra-c2012-11.4
static volatile Nvic_Registers *const nvic =
		(volatile Nvic_Registers *)NVIC_BASE_ADDRESS;

static inline uint8_t
getRegisterOffset(const Nvic_Irq irqn)
{
	return (uint8_t)irqn >> NVIC_REGISTERS_OFFSET;
}

void
Nvic_enableInterrupt(const Nvic_Irq irqn)
{
	assert((int32_t)irqn >= 0);
	nvic->iser[getRegisterOffset(irqn)] = 1u
			<< ((uint32_t)irqn % NVIC_IRQ_GROUP_SIZE);
}

void
Nvic_disableInterrupt(const Nvic_Irq irqn)
{
	assert((int32_t)irqn >= 0);
	nvic->icer[getRegisterOffset(irqn)] = 1u
			<< ((uint32_t)irqn % NVIC_IRQ_GROUP_SIZE);
}

bool
Nvic_isInterruptEnabled(const Nvic_Irq irqn)
{
	assert((int32_t)irqn >= 0);
	return (nvic->iser[getRegisterOffset(irqn)]
			       & (1u << ((uint32_t)irqn % NVIC_IRQ_GROUP_SIZE)))
			!= 0u;
}

void
Nvic_setInterruptPending(const Nvic_Irq irqn)
{
	assert((int32_t)irqn >= 0);
	nvic->ispr[getRegisterOffset(irqn)] = 1u
			<< ((uint32_t)irqn % NVIC_IRQ_GROUP_SIZE);
}

void
Nvic_clearInterruptPending(const Nvic_Irq irqn)
{
	assert((int32_t)irqn >= 0);
	nvic->icpr[getRegisterOffset(irqn)] = 1u
			<< ((uint32_t)irqn % NVIC_IRQ_GROUP_SIZE);
}

bool
Nvic_isInterruptPending(const Nvic_Irq irqn)
{
	assert((int32_t)irqn >= 0);
	return (nvic->ispr[getRegisterOffset(irqn)]
			       & (1u << ((uint32_t)irqn % NVIC_IRQ_GROUP_SIZE)))
			!= 0u;
}

bool
Nvic_isInterruptActive(const Nvic_Irq irqn)
{
	assert((int32_t)irqn >= 0);
	return (nvic->iabr[getRegisterOffset(irqn)]
			       & (1u << ((uint32_t)irqn % NVIC_IRQ_GROUP_SIZE)))
			!= 0u;
}

void
Nvic_setInterruptPriority(const Nvic_Irq irqn, uint8_t priority)
{
	assert((int32_t)irqn >= 0);
	nvic->ipr[irqn] = (uint8_t)(((uint32_t)priority
						    << NVIC_IRQ_PRIORITY_OFFSET)
			& 0xFFu);
}

uint8_t
Nvic_getInterruptPriority(const Nvic_Irq irqn)
{
	assert((int32_t)irqn >= 0);
	return (uint8_t)(nvic->ipr[irqn] >> NVIC_IRQ_PRIORITY_OFFSET);
}

void
Nvic_triggerInterrupt(const Nvic_Irq irqn)
{
	assert((int32_t)irqn >= 0);
	nvic->stir = (uint32_t)irqn;
}

void
Nvic_setPriorityGrouping(const uint8_t priorityGroup)
{
	// cppcheck-suppress misra-c2012-11.4
	Scb_Registers *scb = (Scb_Registers *)SCB_BASE_ADDRESS;

	assert(priorityGroup <= 7u);

	uint32_t aircr = scb->aircr;
	aircr &= (uint32_t)(~SCB_AIRCR_VECTKEY_MASK);
	aircr |= (SCB_AIRCR_VECTKEY_WRITE_KEY << SCB_AIRCR_VECTKEY_OFFSET);
	aircr &= (uint32_t)(~SCB_AIRCR_PRIGROUP_MASK);
	aircr |= (((uint32_t)priorityGroup & 0x07u)
			<< SCB_AIRCR_PRIGROUP_OFFSET);
	scb->aircr = aircr;
}

uint8_t
Nvic_getPriorityGrouping(void)
{
	// cppcheck-suppress misra-c2012-11.4
	const Scb_Registers *scb = (Scb_Registers *)SCB_BASE_ADDRESS;
	return (uint8_t)((scb->aircr & SCB_AIRCR_PRIGROUP_MASK)
			>> SCB_AIRCR_PRIGROUP_OFFSET);
}

void
Nvic_relocateVectorTable(const void *const address)
{
	Nvic_disableIrq();
	// LCOV_EXCL_START
	// Faults are used by the GCOV itself, coverage cannot be gathered.
	// The code was inspected and analysed - it is executed by the tests.
	Nvic_disableFaultIrq();
	Nvic_relocateVectorTableUnsafe(address);
	MEMORY_SYNC_BARRIER();
	Nvic_enableFaultIrq();
	// LCOV_EXCL_STOP
	Nvic_enableIrq();
}

void
Nvic_relocateVectorTableUnsafe(const void *const address)
{
	// cppcheck-suppress misra-c2012-11.4
	Scb_Registers *scb = (Scb_Registers *)SCB_BASE_ADDRESS;
	// cppcheck-suppress misra-c2012-11.6
	const uint32_t addressInt = (uint32_t)address;
	assert((addressInt & SCB_VTOR_TBLOFF_MASK) == addressInt);
	scb->vtor = addressInt;
}

void *
Nvic_getVectorTableAddress(void)
{
	// cppcheck-suppress misra-c2012-11.4
	const Scb_Registers *scb = (Scb_Registers *)SCB_BASE_ADDRESS;
	return (void *)scb->vtor;
}

void
Nvic_setInterruptHandlerAddress(
		const Nvic_Irq irqn, const Nvic_InterruptHandler address)
{
	assert((int32_t)irqn >= 0);

	volatile Nvic_VectorTable *vtable = Nvic_getVectorTableAddress();
	// cppcheck-suppress misra-c2012-11.1
	vtable->irqHandlers[irqn] = address;

	MEMORY_SYNC_BARRIER();
	const uint32_t vectorTableSizeNext32Multiple = 32u
			+ (((uint32_t)Nvic_InterruptCount * 4u) & 0xFFFFFFE0u);
	(void)Scb_invalidateICacheByAddr(Nvic_getVectorTableAddress(),
			vectorTableSizeNext32Multiple);
}

Nvic_InterruptHandler
Nvic_getInterruptHandlerAddress(const Nvic_Irq irqn)
{
	assert((int32_t)irqn >= 0);

	const volatile Nvic_VectorTable *vtable = Nvic_getVectorTableAddress();
	return vtable->irqHandlers[irqn];
}
