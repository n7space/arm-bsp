/**@file
 * This file is part of the N7-Core library used in the Test Environment.
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

#include "ByteFifo.h"

#include <assert.h>

void
ByteFifo_init(ByteFifo *const fifo, uint8_t *const memoryBlock,
		const size_t memoryBlockSize)
{
	assert(memoryBlockSize > 0u);

	fifo->begin = memoryBlock;
	// cppcheck-suppress [misra-c2012-18.4]
	fifo->end = memoryBlock + memoryBlockSize;
	fifo->first = NULL;
	fifo->last = memoryBlock;
}

void
ByteFifo_initFromBytes(ByteFifo *const fifo, uint8_t *const memoryBlock,
		const size_t memoryBlockSize)
{
	assert(memoryBlockSize > 0u);

	fifo->begin = memoryBlock;
	// cppcheck-suppress [misra-c2012-18.4]
	fifo->end = memoryBlock + memoryBlockSize;
	fifo->first = memoryBlock;
	fifo->last = memoryBlock;
}

size_t
ByteFifo_getCount(const ByteFifo *const fifo)
{
	if (ByteFifo_isEmpty(fifo))
		return 0;

	// cppcheck-suppress [misra-c2012-18.4]
	ptrdiff_t ptrDifference = fifo->last - fifo->first;
	if (ptrDifference <= 0)
		// cppcheck-suppress [misra-c2012-18.4]
		ptrDifference += fifo->end - fifo->begin;

	return (size_t)ptrDifference;
}

bool
ByteFifo_push(ByteFifo *const fifo, const uint8_t data)
{
	if (ByteFifo_isFull(fifo))
		return false;

	if (fifo->first == NULL)
		fifo->first = fifo->last;
	*fifo->last = data;
	// cppcheck-suppress [misra-c2012-18.4]
	++fifo->last;
	if (fifo->last == fifo->end)
		fifo->last = fifo->begin;

	return true;
}

bool
ByteFifo_pull(ByteFifo *const fifo, uint8_t *const data)
{
	if (ByteFifo_isEmpty(fifo))
		return false;

	*data = *fifo->first;

	// cppcheck-suppress [misra-c2012-18.4]
	++fifo->first;
	if (fifo->first == fifo->end)
		fifo->first = fifo->begin;
	if (fifo->first == fifo->last)
		fifo->first = NULL;
	return true;
}
