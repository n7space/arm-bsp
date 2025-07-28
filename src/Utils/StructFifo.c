/**@file
 * This file is part of the N7-Core library used in the Test Environment.
 *
 * @copyright 2016-2024 N7 Space Sp. z o.o.
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

#include "StructFifo.h"

void
StructFifo_init(StructFifo *const fifo, void *const memoryBuffer,
		const size_t elementSize, const size_t elementsCount)
{
	fifo->begin = (uint8_t *)memoryBuffer;
	// cppcheck-suppress [misra-c2012-18.4]
	fifo->end = (uint8_t *)memoryBuffer + (elementSize * elementsCount);
	fifo->first = NULL;
	fifo->last = (uint8_t *)memoryBuffer;
	fifo->elementSize = elementSize;
}

bool
StructFifo_push(StructFifo *const fifo, const void *const data)
{
	if (StructFifo_isFull(fifo))
		return false;

	if (fifo->first == NULL)
		fifo->first = fifo->last;
	// cppcheck-suppress [misra-c2012-18.4]
	fifo->last = (uint8_t *)memcpy(fifo->last, data, fifo->elementSize)
			+ fifo->elementSize;
	if (fifo->last == fifo->end)
		fifo->last = fifo->begin;

	return true;
}

bool
StructFifo_pushUnaligned(StructFifo *const fifo, const void *const data)
{
	if (StructFifo_isFull(fifo))
		return false;

	if (fifo->first == NULL)
		fifo->first = fifo->last;
	// cppcheck-suppress [misra-c2012-18.4]
	fifo->last = (uint8_t *)memcpy(fifo->last, data, fifo->elementSize)
			+ fifo->elementSize;
	if (fifo->last == fifo->end)
		fifo->last = fifo->begin;

	return true;
}

bool
StructFifo_pull(StructFifo *const fifo, void *const data)
{
	if (StructFifo_isEmpty(fifo))
		return false;

	(void)memcpy(data, fifo->first, fifo->elementSize);
	fifo->first += fifo->elementSize; // cppcheck-suppress [misra-c2012-18.4]
	if (fifo->first == fifo->end)
		fifo->first = fifo->begin;
	if (fifo->first == fifo->last)
		fifo->first = NULL;
	return true;
}

bool
StructFifo_pullUnaligned(StructFifo *const fifo, void *const data)
{
	if (StructFifo_isEmpty(fifo))
		return false;

	(void)memcpy(data, fifo->first, fifo->elementSize);
	fifo->first += fifo->elementSize; // cppcheck-suppress [misra-c2012-18.4]
	if (fifo->first == fifo->end)
		fifo->first = fifo->begin;
	if (fifo->first == fifo->last)
		fifo->first = NULL;
	return true;
}

bool
StructFifo_peek(const StructFifo *const fifo, void *const data)
{
	if (StructFifo_isEmpty(fifo))
		return false;

	(void)memcpy(data, fifo->first, fifo->elementSize);
	return true;
}

bool
StructFifo_drop(StructFifo *const fifo)
{
	if (StructFifo_isEmpty(fifo))
		return false;

	// cppcheck-suppress [misra-c2012-18.4]
	fifo->first += fifo->elementSize;
	if (fifo->first == fifo->end)
		fifo->first = fifo->begin;
	if (fifo->first == fifo->last)
		fifo->first = NULL;
	return true;
}
