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

#ifndef UTILS_UTILS_H
#define UTILS_UTILS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/// \file  Utils.h
/// \addtogroup N7-Core
/// \brief Module containing general utility library functions.

/// @addtogroup Utils
/// @ingroup N7-Core
/// @{

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Boolean lambda prototype.
/// \returns Evaluated value.
typedef bool (*BooleanLambda)(void);

/// \brief Boolean lambda prototype with arg.
/// \param [in] arg Argument for the lambda.
/// \returns Evaluated value.
typedef bool (*BooleanArgLambda)(void *const arg);

/// \brief Compares two values and returns the smaller one.
/// \param [in] a The first of compared arguments.
/// \param [in] b The second of compared arguments.
/// \returns The smaller of the two supplied arguments.
static inline uint32_t
minUInt32(const uint32_t a, const uint32_t b)
{
	return (a < b) ? a : b;
}

/// \brief Compares two unsigned values and returns the larger one.
/// \param [in] a The first of compared arguments.
/// \param [in] b The second of compared arguments.
/// \returns The larger of the two supplied arguments.
static inline uint32_t
maxUInt32(const uint32_t a, const uint32_t b)
{
	return (a < b) ? b : a;
}

/// \brief Converts a regular uint8_t to a Binary Coded Decimal.
///        The lowest 4 bits encode units. The higher bits encode tens.
///        Output is valid for input in the range [0..99]
/// \param [in] x Regular uint8_t.
/// \returns BCD coded input.
static inline uint8_t
uint8ToBcd(const uint8_t x)
{
	return ((uint8_t)(x % 10U)) | (uint8_t)(((x / 10U) << 4U) & 0xF0U);
}

/// \brief Converts a Binary Coded Decimal to a regular uint8_t.
///        The lowest 4 bits of the output encode units. The higher bits encode tens.
///        Output is valid only for correctly formatted input - value of each of the byte halves
///        must be in range [0..9].
/// \param [in] bcd BCD coded input.
/// \returns regular uint8_t.
static inline uint8_t
bcdToUint8(const uint8_t bcd)
{
	return (uint8_t)(((uint8_t)(bcd & 0x0FU))
			+ (uint8_t)(((bcd & 0xF0U) >> 4U) * 10U));
}

/// \brief Continuously evaluates a boolean lambda until either the evaluation yields true or
///        a timeout occurs.
/// \param [in] lambda Lambda to be evaluated.
/// \param [in] timeout Timeout value.
/// \returns Whether the lambda evaluated to true before timeout occurred.
static inline bool
evaluateLambdaWithTimeout(const BooleanLambda lambda, const uint32_t timeout)
{
	for (uint32_t counter = 0; counter < timeout; ++counter)
		if (lambda()) // cppcheck-suppress [misra-c2012-14.4]
			return true;
	return false;
}

/// \brief Continuously compares a register against a mask until a match or
///        a timeout occurs.
/// \param [in] address Register's address.
/// \param [in] mask Mask the the register is compared against.
/// \param [in] timeout Timeout value.
/// \returns Whether the mask matched before timeout occurred.
static inline bool
waitForRegisterWithTimeout(const volatile uint32_t *const address,
		const uint32_t mask, const uint32_t timeout)
{
	for (uint32_t counter = 0; counter < timeout; ++counter)
		if ((*address & mask) != 0u)
			return true;
	return false;
}

/// \brief Continuously compares a register against a mask until it clears or
///        a timeout occurs.
/// \param [in] address Register's address.
/// \param [in] mask Mask the the register is compared against.
/// \param [in] timeout Timeout value.
/// \returns Whether the register cleared before timeout occurred.
static inline bool
waitForRegisterClearWithTimeout(const volatile uint32_t *const address,
		const uint32_t mask, const uint32_t timeout)
{
	for (uint32_t counter = 0; counter < timeout; ++counter)
		if ((*address & mask) == 0u)
			return true;
	return false;
}

/// \brief Continuously evaluates a boolean lambda until either the evaluation yields true or
///        a timeout occurs.
/// \param [in] lambda Lambda to be evaluated.
/// \param [in] arg Argument for lambda.
/// \param [in] timeout Timeout value.
/// \returns Whether the lambda evaluated to true before timeout occurred.
static inline bool
evaluateArgLambdaWithTimeout(const BooleanArgLambda lambda, void *const arg,
		const uint32_t timeout)
{
	for (uint32_t counter = 0; counter < timeout; ++counter)
		if (lambda(arg)) // cppcheck-suppress [misra-c2012-14.4]
			return true;
	return false;
}

/// \brief Checks whether a number is between two other numbers, excluding the other numbers.
/// \param [in] x Number to be compared.
/// \param [in] lowerBound Lower bound of the range.
/// \param [in] upperBound Upper bound of the range.
/// \returns Whether the compared number is within the defined range, excluding the range extremes.
static inline bool
isBetweenUint32(const uint32_t x, const uint32_t lowerBound,
		const uint32_t upperBound)
{
	return (x > lowerBound) && (x < upperBound);
}

/// \brief Busy wait loop, can be used to introduce short delays.
/// \param [in] iterations Number of iterations to perform.
void busyWaitLoop(const uint32_t iterations);

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // UTILS_UTILS_H
