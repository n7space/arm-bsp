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

/// \file  ByteBuffer.h
/// \addtogroup Utils
/// \brief Module representing bytes collection with dynamic size but fixed capacity.

#ifndef UTILS_BYTEBUFFER_H
#define UTILS_BYTEBUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup ByteBuffer
/// @ingroup Utils
/// @{

/// \brief Structure representing single buffer instance.
typedef struct {
	uint8_t *begin; ///< Pointer to beginning of data stored in buffer.
	uint8_t *end; ///< Pointer to end of data stored in buffer.
	uint8_t *capacityEnd; ///< Pointer to end of memory block reserved for buffer.
} ByteBuffer;

/// \brief ByteBuffer constructor macro, creates empty buffer with given name and capacity.
///        It creates memory block on stack, so it is mostly useful in tests.
/// \param [in] NAME name of ByteBuffer to create.
/// \param [in] CAPACITY capacity of created ByteBuffer.
// clang-format off
// cppcheck-suppress [misra-c2012-20.7, misra-c2012-20.10, misra-c2012-20.12]
#define BYTE_BUFFER_CREATE(NAME, CAPACITY)                              \
  uint8_t NAME ## MemoryBlock[(CAPACITY)] = { 0 };                      \
  ByteBuffer NAME = { .begin       = NAME ## MemoryBlock,               \
                      .end         = NAME ## MemoryBlock,               \
                      .capacityEnd = NAME ## MemoryBlock + (CAPACITY) }
// clang-format on

/// \brief ByteBuffer constructor macro, creates buffer with provided contents.
///        It creates memory block on stack, so it is mostly useful in tests.
/// \param [in] NAME name of ByteBuffer to create.
/// \param [in] ... contents of created ByteBuffer.
// clang-format off
// cppcheck-suppress [misra-c2012-20.7,misra-c2012-20.10]
#define BYTE_BUFFER_CREATE_FILLED(NAME, ...)                                            \
  uint8_t NAME ## MemoryBlock[] = __VA_ARGS__;                                          \
  ByteBuffer NAME = { .begin       = NAME ## MemoryBlock,                               \
                      .end         = NAME ## MemoryBlock + sizeof(NAME ## MemoryBlock), \
                      .capacityEnd = NAME ## MemoryBlock + sizeof(NAME ## MemoryBlock) }
// clang-format on

/// \brief ByteBuffer initialisation procedure, assigns all fields properly.
///        Should be called before any use of ByteBuffer.
/// \param [in,out] buffer pointer to ByteBuffer to initialise.
/// \param [in] memoryBlock memory block to be assigned to ByteBuffer as its storage area.
/// \param [in] memoryBlockSize size of memory block.
void ByteBuffer_init(ByteBuffer *const buffer, uint8_t *const memoryBlock,
		const size_t memoryBlockSize);

/// \brief Resets ByteBuffer underlaying memory block with provided value.
///        Does not change elements count in buffer.
/// \param [in,out] buffer pointer to ByteBuffer to reset.
/// \param [in] value value to used to reset all memory block bytes.
void ByteBuffer_memset(ByteBuffer *const buffer, const uint8_t value);

/// \brief Appends value to ByteBuffer, if it's not full, fails otherwise.
/// \param [in,out] buffer pointer to target ByteBuffer.
/// \param [in] value value to append.
/// \retval true if value was appended
/// \retval false if buffer is full
bool ByteBuffer_append(ByteBuffer *const buffer, const uint8_t value);

/// \brief Returns capacity of provided ByteBuffer.
///        Capacity is represented as maximum elements' count that could be stored in given buffer.
/// \param [in] buffer pointer to ByteBuffer.
/// \returns capacity of provided buffer.
static inline size_t
ByteBuffer_getCapacity(const ByteBuffer *const buffer)
{
	// cppcheck-suppress [misra-c2012-18.4]
	return (size_t)(buffer->capacityEnd - buffer->begin);
}

/// \brief Returns elements count in provided ByteBuffer.
/// \param [in] buffer pointer to ByteBuffer.
/// \returns count of elements currently stored in provided buffer.
static inline size_t
ByteBuffer_getCount(const ByteBuffer *const buffer)
{
	// cppcheck-suppress [misra-c2012-18.4]
	return (size_t)(buffer->end - buffer->begin);
}

/// \brief Clears provided ByteBuffer.
///        Operation will set elements count to zero,
///        will not change capacity nor underlying memory contents.
/// \param [in,out] buffer pointer to target ByteBuffer.
static inline void
ByteBuffer_clear(ByteBuffer *const buffer)
{
	buffer->end = buffer->begin;
}

/// \brief Checks if ByteBuffer is full.
/// \param [in] buffer pointer to ByteBuffer.
/// \retval true if buffer is full (elements count reached capacity)
/// \retval false otherwise
static inline bool
ByteBuffer_isFull(const ByteBuffer *const buffer)
{
	return buffer->end == buffer->capacityEnd;
}

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // UTILS_BYTEBUFFER_H
