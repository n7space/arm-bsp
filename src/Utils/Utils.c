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

#include "Utils.h"

void
busyWaitLoop(const uint32_t iterations)
{
	asm volatile("nop" ::: "memory");
	for (uint32_t counter = 0; counter < iterations; counter++)
		asm volatile("nop" : "+g"(counter)::"memory");
	asm volatile("nop" ::: "memory");
}
