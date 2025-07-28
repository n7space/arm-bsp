/**@file
 * This file is part of the N7-Core library used in the Test Environment.
 *
 * @copyright 2022-2024 N7 Space Sp. z o.o.
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

/// \file  ErrorCode.h
/// \addtogroup Utils
/// \brief Module defining error code type and reporting functions.

#ifndef UTILS_ERRORCODE_H
#define UTILS_ERRORCODE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/// @addtogroup ErrorCode
/// @ingroup Utils
/// @{

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Utility macro for defining error code from 4 bytes.
#define ERROR_CODE_DEFINE(A, B, C, D) \
	(ErrorCode)(((uint32_t)((((uint32_t)(A)) & 0xFFu) << 24u)) \
			| ((uint32_t)((((uint32_t)(B)) & 0xFFu) << 16u)) \
			| ((uint32_t)((((uint32_t)(C)) & 0xFFu) << 8u)) \
			| ((uint32_t)((((uint32_t)(D)) & 0xFFu))))

/// \brief Type used to represent error code.
typedef uint32_t ErrorCode;

enum {
	/// \brief Error code value representing no error.
	ErrorCode_NoError = 0u
};

/// \brief Simplifies writing functions returning boolean and providing optional error code.
/// \details Example:
/// \code
/// bool fun(ErrorCode* const errCode)
/// {
///   ...
///   if (errorCondition)
///     return returnError(errCode, ERROR_CONDITION_CODE);
///   ...
/// }
/// \endcode
/// \param [out] errCode pointer to store error code to, can be \c NULL.
/// \param [in] returnedError error code to be set.
/// \returns always \b false
static inline bool
returnError(ErrorCode *const errCode, const ErrorCode returnedError)
{
	if (errCode != NULL)
		*errCode = returnedError;
	return false;
}

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // UTILS_ERRORCODE_H
