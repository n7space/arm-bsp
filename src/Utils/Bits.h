/**@file
 * This file is part of the N7-Core library used in the Test Environment.
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

/// \file Bits.h
/// \addtogroup Utils
/// \brief Auxiliary header for basic bit operations on registers.

#ifndef UTILS_BITS_H
#define UTILS_BITS_H

#include <stdbool.h>
#include <stdint.h>

/// @addtogroup Bits
/// @ingroup Utils
/// @{

/// \brief Macro used for obtaining register's bit field offset by naming convention.
/// \param [in] bitFieldName Name of the field.
// cppcheck-suppress [misra-c2012-20.10]
#define BIT_FIELD_OFFSET(bitFieldName) (bitFieldName##_OFFSET)

/// \brief Macro used for obtaining register's bit field mask by naming convention.
/// \param [in] bitFieldName Name of the field.
// cppcheck-suppress [misra-c2012-20.10]
#define BIT_FIELD_MASK(bitFieldName) (bitFieldName##_MASK)

/// \brief Macro used for convenient shifting 32-bit registers fields.
/// \param [in] bitFieldName Name of the field.
/// \param [in] value Value to be prepared for the given field.
/// \note Constant suffixed with _OFFSET and _MASK for the name of the field should exist.
/// \returns Shifted and masked value.
#define BIT_FIELD_VALUE(bitFieldName, value) \
	shiftValueLeft((uint32_t)(value), BIT_FIELD_OFFSET(bitFieldName), \
			BIT_FIELD_MASK(bitFieldName))

/// \brief Macro used for convenient shifting 32-bit registers bits.
/// \param [in] bitName Name of the bit.
/// \param [in] value Value to be prepared for the given field.
/// \note Constant suffixed with _OFFSET and _MASK for the name of the bit should exist.
/// \returns Shifted bit value.
#define BIT_VALUE(bitName, value) \
	shiftBitLeft((uint32_t)(value), BIT_FIELD_OFFSET(bitName))

/// \brief Macro used for convenient reading of masked reading from specific offset in 32-bit register.
/// \param [in] bitFieldName Name of the field.
/// \param [in] registerValue Content of the register.
/// \note Constant suffixed with _OFFSET and _MASK for the name of the field should exist.
/// \returns Masked value, shifted to the right by offset.
#define GET_FIELD_VALUE(bitFieldName, registerValue) \
	getValueAtOffset((uint32_t)(registerValue), \
			BIT_FIELD_OFFSET(bitFieldName), \
			BIT_FIELD_MASK(bitFieldName))

/// \brief Convenience macro used for writing masked value into register.
/// \param [in] bitFieldName Name of the field.
/// \param [in,out] registerAddress address of a register.
/// \param [in] fieldValue Value to be written into a bit field of register.
/// \note Constant suffixed with _OFFSET and _MASK for the name of the field should exist.
#define SET_FIELD_VALUE(bitFieldName, registerAddress, fieldValue) \
	setValueAtOffset((registerAddress), (fieldValue), \
			BIT_FIELD_OFFSET(bitFieldName), \
			BIT_FIELD_MASK(bitFieldName))

/// \brief Convenience macro used for writing masked value into register.
/// \param [in] bitFieldName Name of the field.
/// \param [in,out] registerAddress address of a register.
/// \param [in] fieldValue Value to be written into a bit field of register.
/// \note Constant suffixed with _OFFSET and _MASK for the name of the field should exist.
#define SET_BYTE_FIELD_VALUE(bitFieldName, registerAddress, fieldValue) \
	setByteValueAtOffset((registerAddress), (fieldValue), \
			BIT_FIELD_OFFSET(bitFieldName), \
			BIT_FIELD_MASK(bitFieldName))

/// \brief Macro used for convenient checking whether the value of masked field is set.
/// \param [in] bitFieldName Name of the field.
/// \param [in] registerValue Content of the register.
/// \note Constant suffixed with _OFFSET and _MASK for the name of the field should exist.
/// \returns True if any bit of masked field is non-zero, false otherwise.
#define IS_FIELD_SET(bitFieldName, registerValue) \
	isFieldSet((uint32_t)(registerValue), BIT_FIELD_MASK(bitFieldName))

/// \brief Macro used for convenient checking whether the bit of a register is set.
/// \param [in] bitFieldName Name of the field.
/// \param [in] registerValue Content of the register.
/// \note Constant suffixed with _OFFSET and _MASK for the name of the field should exist.
/// \returns True if bit is set, false otherwise.
#define IS_BIT_SET(bitFieldName, registerValue) \
	isBitSet((uint32_t)(registerValue), BIT_FIELD_OFFSET(bitFieldName))

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Shifts to left and masks provided value.
/// \param [in] value Value to be shifted.
/// \param [in] offset Value will be shifted by given offset.
/// \param [in] mask Bit mask of the shifted value.
/// \returns shifted and masked value.
static inline uint32_t
shiftValueLeft(const uint32_t value, const uint32_t offset, const uint32_t mask)
{
	const uint32_t shifted = value << offset;
	return shifted & mask;
}

/// \brief Shifts to left provided bit value.
/// \param [in] value Value to be shifted.
/// \param [in] offset Value will be shifted by given offset.
/// \returns shifted value.
static inline uint32_t
shiftBitLeft(const bool value, const uint32_t offset)
{
	const uint32_t v32 = value ? 1u : 0u;
	return v32 << offset;
}

/// \brief Changes the given bit to the desired value.
/// \param [in,out] reg Pointer to a 32 bit register which will have the bit changed.
/// \param [in] offset Offset of the bit to be changed.
/// \param [in] value Desired bit value.
static inline void
changeBitAtOffset(volatile uint32_t *const reg, const uint32_t offset,
		const bool value)
{
	const uint32_t current = *reg;
	if (value) {
		*reg = current | (UINT32_C(1) << offset);
	} else {
		*reg = current & ~(UINT32_C(1) << offset);
	}
}

/// \brief Sets the masked bits to the desired values.
/// \param [in, out] reg Pointer to a 32 bit register to be modified.
/// \param [in] value Values to be set.
/// \param [in] offset Offset of the values to be set.
/// \param [in] mask Mask indicating the bits to be changed.
static inline void
setValueAtOffset(volatile uint32_t *const reg, const uint32_t value,
		const uint32_t offset, const uint32_t mask)
{
	uint32_t regVal = *reg;
	regVal &= ~mask;
	regVal |= shiftValueLeft(value, offset, mask);
	*reg = regVal;
}

/// \brief Sets the masked bits to the desired values.
/// \param [in, out] reg Pointer to a 8 bit register to be modified.
/// \param [in] value Values to be set.
/// \param [in] offset Offset of the values to be set.
/// \param [in] mask Mask indicating the bits to be changed.
static inline void
setByteValueAtOffset(volatile uint8_t *const reg, const uint8_t value,
		const uint8_t offset, const uint8_t mask)
{
	uint8_t regVal = *reg;
	regVal &= ~mask;
	regVal |= shiftValueLeft(value, offset, mask);
	*reg = regVal;
}

/// \brief Returns a masked value from specific offset in 32-bit value.
/// \param [in] value Value (for example, register's content).
/// \param [in] offset Offset of the field.
/// \param [in] mask Bit mask of the field.
/// \returns Masked value, shifted to right.
static inline uint32_t
getValueAtOffset(const uint32_t value, const uint32_t offset,
		const uint32_t mask)
{
	const uint32_t maskedValue = value & mask;
	return maskedValue >> offset;
}

/// \brief Checks if value of provided masked field is non-zero.
/// \param [in] value Value (for example, register's content).
/// \param [in] mask Bit mask of the field.
/// \returns True if any bit of provided masked value is non-zero, false otherwise.
static inline bool
isFieldSet(const uint32_t value, const uint32_t mask)
{
	return (value & mask) != 0u;
}

/// \brief Checks if a single bit of provided value is set.
/// \param [in] value Value (for example, registers's content).
/// \param [in] offset Offset of checked bit.
/// \returns True if bit is set, false otherwise.
static inline bool
isBitSet(const uint32_t value, const uint32_t offset)
{
	return isFieldSet(value, UINT32_C(1) << offset);
}

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // UTILS_BITS_H
