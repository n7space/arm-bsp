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

/// \file StructFifo.h
/// \addtogroup Utils
/// \brief Module representing fixed-size struct queue based on circular buffer.
/// \details Should not be used directly,
///          but as 'base class' for other queues (ensuring type safety).

#ifndef UTILS_STRUCTFIFO_H
#define UTILS_STRUCTFIFO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/// @addtogroup StructFifo
/// @ingroup Utils
/// @{

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Structure representing single queue instance.
typedef struct {
	uint8_t *begin; ///< Pointer to beginning of buffer area.
	uint8_t *end; ///< Pointer to end of buffer area.
	uint8_t *first; ///< Pointer to oldest item in queue.
	uint8_t *last; ///< Pointer used as next insert location.
	size_t elementSize; ///< Size of single queue element.
} StructFifo;

/// \brief StructFifo constructor macro, creates empty queue with given name, type and capacity.
///        It creates memory block on stack, so it is mostly useful in tests.
/// \param [in] NAME name of StructFifo to create.
/// \param [in] TYPE item type of queue to create.
/// \param [in] CAPACITY capacity of created StructFifo.
// clang-format off
// cppcheck-suppress [misra-c2012-20.7, misra-c2012-20.10, misra-c2012-20.12]
#define STRUCT_FIFO_CREATE(NAME, TYPE, CAPACITY)                                                   \
  TYPE NAME ## MemoryBlock[(CAPACITY)];                                                            \
  StructFifo NAME = { .begin       = (uint8_t*)NAME ## MemoryBlock,                                \
                      .end         = (uint8_t*)(NAME ## MemoryBlock + (CAPACITY)),                 \
                      .first       = NULL,                                                         \
                      .last        = (uint8_t*)NAME ## MemoryBlock,                                \
                      .elementSize = sizeof(TYPE) }
// clang-format on

/// \brief StructFifo constructor macro, creates queue with provided contents.
///        It creates memory block on stack, so it is mostly useful in tests.
/// \param [in] NAME name of TcManager_StructFifo to create.
/// \param [in] TYPE item type of queue to create.
/// \param [in] ... contents of created queue.
// clang-format off
// cppcheck-suppress [misra-c2012-20.7, misra-c2012-20.10, misra-c2012-20.12]
#define STRUCT_FIFO_CREATE_FILLED(NAME, TYPE, ...)                      \
  TYPE NAME ## MemoryBlock[] = __VA_ARGS__;                             \
  StructFifo NAME = { .begin       = (uint8_t*)NAME ## MemoryBlock,     \
                      .end         = (uint8_t*)NAME ## MemoryBlock      \
                                       + sizeof(NAME ## MemoryBlock),   \
                      .first       = (uint8_t*)NAME ## MemoryBlock,     \
                      .last        = (uint8_t*)NAME ## MemoryBlock,     \
                      .elementSize = sizeof(TYPE) }
// clang-format on

/// \brief StructFifo initialisation procedure, assigns all fields properly.
///        Should be called before any use of StructFifo.
/// \param [in,out] fifo pointer to StructFifo to initialise.
/// \param [in] memoryBuffer memory block to be assigned to StructFifo as its storage area.
/// \param [in] elementSize size of elements stored in this queue.
/// \param [in] elementsCount maximum count of elements stored in this queue.
void StructFifo_init(StructFifo *const fifo, void *const memoryBuffer,
		const size_t elementSize, const size_t elementsCount);

/// \brief Clears queue.
/// \param [in,out] fifo queue to clear.
static inline void
StructFifo_clear(StructFifo *const fifo)
{
	fifo->first = NULL;
	fifo->last = fifo->begin;
}

/// \brief Checks if queue is full.
/// \param [in] fifo queue to check.
/// \retval true when queue is full (next push will not be accepted).
/// \retval false otherwise
static inline bool
StructFifo_isFull(const StructFifo *const fifo)
{
	return fifo->first == fifo->last;
}

/// \brief Checks if queue is empty.
/// \param [in] fifo queue to check.
/// \retval true when queue is empty (next pull will not be accepted).
/// \retval false otherwise
static inline bool
StructFifo_isEmpty(const StructFifo *const fifo)
{
	return fifo->first == NULL;
}

/// \brief Pushes given item as last in queue, copies item contents.
/// \param [in,out] fifo target queue.
/// \param [in] data data to be copied and pushed into queue.
/// \retval true on successful push
/// \retval false otherwise (queue is full)
bool StructFifo_push(StructFifo *const fifo, const void *const data);

/// \brief Pushes given item as last in queue, copies item contents.
/// \param [in,out] fifo target queue.
/// \param [in] data data to be copied and pushed into queue.
/// \retval true on successful push
/// \retval false otherwise (queue is full)
bool StructFifo_pushUnaligned(StructFifo *const fifo, const void *const data);

/// \brief Pulls first item from queue.
///        Removes pulled item from queue, copies it contents into provided address.
/// \param [in,out] fifo target queue.
/// \param [out] data address to store pulled data.
/// \retval true on successful pull
/// \retval false otherwise (queue is empty)
bool StructFifo_pull(StructFifo *const fifo, void *const data);

/// \brief Pulls first item from queue.
///        Removes pulled item from queue, copies it contents into provided address.
/// \param [in,out] fifo target queue.
/// \param [out] data address to store pulled data.
/// \retval true on successful pull
/// \retval false otherwise (queue is empty)
bool StructFifo_pullUnaligned(StructFifo *const fifo, void *const data);

/// \brief Takes a peek at the first item in the queue.
///        Copies the peeked at item's contents into the provided address.
/// \param [in,out] fifo target queue.
/// \param [out] data address to store the peeked data.
/// \retval true on successful peek.
/// \retval false otherwise (queue is empty).
bool StructFifo_peek(const StructFifo *const fifo, void *const data);

/// \brief Drops the first item from the queue.
/// \param [in,out] fifo target queue.
/// \retval true on successful drop.
/// \retval false otherwise (queue is empty).
bool StructFifo_drop(StructFifo *const fifo);

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // UTILS_STRUCTFIFO_H
