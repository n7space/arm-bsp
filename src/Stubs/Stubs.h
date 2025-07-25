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

#ifndef STUBS_H
#define STUBS_H

/// \file Stubs.h
/// \brief Stubs prototypes for hardware initialization.

#include <stdint.h>

#ifndef LOW_LEVEL_IO_BAUDRATE
/// \brief Default uart baud rate if not specified
#define LOW_LEVEL_IO_BAUDRATE 115200
#endif

/// \brief Performs a hardware setup procedure of Stubs module.
void Stubs_startup(void);

/// \brief Performs a hardware shutdown procedure of Stubs module.
void Stubs_shutdown(void);

/// \brief Writes provided byte to substituted standard output.
/// \param byte Byte to be written.
void Stubs_writeByte(uint8_t byte);

#endif // STUBS_H
