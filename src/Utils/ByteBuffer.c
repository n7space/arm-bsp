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

#include "ByteBuffer.h"

void
ByteBuffer_init(ByteBuffer *buffer, uint8_t *const memoryBlock,
		const size_t memoryBlockSize)
{
	buffer->begin = memoryBlock;
	buffer->end = memoryBlock;
	// cppcheck-suppress [misra-c2012-18.4]
	buffer->capacityEnd = memoryBlock + memoryBlockSize;
}

void
ByteBuffer_memset(ByteBuffer *buffer, const uint8_t value)
{
	for (uint8_t *it = buffer->begin; it != buffer->capacityEnd; ++it)
		*it = value; // cppcheck-suppress [misra-c2012-14.2]
}

bool
ByteBuffer_append(ByteBuffer *const buffer, const uint8_t value)
{
	if (buffer->end == buffer->capacityEnd)
		return false;
	*(buffer->end) = value;
	++buffer->end;
	return true;
}
