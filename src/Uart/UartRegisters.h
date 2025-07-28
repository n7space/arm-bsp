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

#ifndef BSP_UART_REGISTERS_H
#define BSP_UART_REGISTERS_H

#include <stdint.h>

/// \brief Structure representing UART control and status registers.
typedef struct {
	volatile uint32_t cr; ///< 0x00 Control Register
	volatile uint32_t mr; ///< 0x04 Mode Register
	volatile uint32_t ier; ///< 0x08 Interrupt Enable Register
	volatile uint32_t idr; ///< 0x0C Interrupt Disable Register
	volatile uint32_t imr; ///< 0x10 Interrupt Mask Register
	volatile uint32_t sr; ///< 0x14 Status Register
	volatile uint32_t rhr; ///< 0x18 Receive Holding Register
	volatile uint32_t thr; ///< 0x1C Transmit Holding Register
	volatile uint32_t brgr; ///< 0x20 Baud Rate Generator Register
	volatile uint32_t cmpr; ///< 0x24 Comparison Register
	volatile uint32_t reserved1[6]; ///< 0x28 - 0x3C Reserved
	volatile uint32_t reserved2[41]; ///< 0x40 - 0xE0 Reserved
	volatile uint32_t wpmr; ///< 0xE4 Write Protection Mode Register
	volatile uint32_t reserved3; ///< 0xE8 Reserved
	volatile uint32_t reserved4[5]; ///< 0xEC - 0xFC Reserved
} Uart_Registers;

#define UART0_ADDRESS_BASE 0x400E0800u
#define UART1_ADDRESS_BASE 0x400E0A00u
#define UART2_ADDRESS_BASE 0x400E1A00u
#define UART3_ADDRESS_BASE 0x400E1C00u
#define UART4_ADDRESS_BASE 0x400E1E00u

#define UART_CR_RSTRX_MASK 0x00000004u
#define UART_CR_RSTRX_OFFSET 2u
#define UART_CR_RSTTX_MASK 0x00000008u
#define UART_CR_RSTTX_OFFSET 3u
#define UART_CR_RXEN_MASK 0x00000010u
#define UART_CR_RXEN_OFFSET 4u
#define UART_CR_RXDIS_MASK 0x00000020u
#define UART_CR_RXDIS_OFFSET 5u
#define UART_CR_TXEN_MASK 0x00000040u
#define UART_CR_TXEN_OFFSET 6u
#define UART_CR_TXDIS_MASK 0x00000080u
#define UART_CR_TXDIS_OFFSET 7u
#define UART_CR_RSTSTA_MASK 0x00000100u
#define UART_CR_RSTSTA_OFFSET 8u
#define UART_CR_REQCLR_MASK 0x00001000u
#define UART_CR_REQCLR_OFFSET 12u

#define UART_MR_FILTER_MASK 0x00000010u
#define UART_MR_FILTER_OFFSET 4u
#define UART_MR_PAR_MASK 0x00000E00u
#define UART_MR_PAR_OFFSET 9u
#define UART_MR_BSRCCK_MASK 0x00001000u
#define UART_MR_BSRCCK_OFFSET 12u
#define UART_MR_CHMODE_MASK 0x0000C000u
#define UART_MR_CHMODE_OFFSET 14u
#define UART_MR_CHMODE_LOCAL_LOOPBACK_VALUE 2u

#define UART_IER_RXRDY_MASK 0x00000001u
#define UART_IER_RXRDY_OFFSET 0u
#define UART_IER_TXRDY_MASK 0x00000002u
#define UART_IER_TXRDY_OFFSET 1u
#define UART_IER_OVRE_MASK 0x00000020u
#define UART_IER_OVRE_OFFSET 5u
#define UART_IER_FRAME_MASK 0x00000040u
#define UART_IER_FRAME_OFFSET 6u
#define UART_IER_PARE_MASK 0x00000080u
#define UART_IER_PARE_OFFSET 7u
#define UART_IER_TXEMPTY_MASK 0x00000200u
#define UART_IER_TXEMPTY_OFFSET 9u
#define UART_IER_CMP_MASK 0x00008000u
#define UART_IER_CMP_OFFSET 15u

#define UART_IDR_RXRDY_MASK 0x00000001u
#define UART_IDR_RXRDY_OFFSET 0u
#define UART_IDR_TXRDY_MASK 0x00000002u
#define UART_IDR_TXRDY_OFFSET 1u
#define UART_IDR_OVRE_MASK 0x00000020u
#define UART_IDR_OVRE_OFFSET 5u
#define UART_IDR_FRAME_MASK 0x00000040u
#define UART_IDR_FRAME_OFFSET 6u
#define UART_IDR_PARE_MASK 0x00000080u
#define UART_IDR_PARE_OFFSET 7u
#define UART_IDR_TXEMPTY_MASK 0x00000200u
#define UART_IDR_TXEMPTY_OFFSET 9u
#define UART_IDR_CMP_MASK 0x00008000u
#define UART_IDR_CMP_OFFSET 15u

#define UART_IMR_RXRDY_MASK 0x00000001u
#define UART_IMR_RXRDY_OFFSET 0u
#define UART_IMR_TXRDY_MASK 0x00000002u
#define UART_IMR_TXRDY_OFFSET 1u
#define UART_IMR_OVRE_MASK 0x00000020u
#define UART_IMR_OVRE_OFFSET 5u
#define UART_IMR_FRAME_MASK 0x00000040u
#define UART_IMR_FRAME_OFFSET 6u
#define UART_IMR_PARE_MASK 0x00000080u
#define UART_IMR_PARE_OFFSET 7u
#define UART_IMR_TXEMPTY_MASK 0x00000200u
#define UART_IMR_TXEMPTY_OFFSET 9u
#define UART_IMR_CMP_MASK 0x00008000u
#define UART_IMR_CMP_OFFSET 15u

#define UART_SR_RXRDY_MASK 0x00000001u
#define UART_SR_RXRDY_OFFSET 0u
#define UART_SR_TXRDY_MASK 0x00000002u
#define UART_SR_TXRDY_OFFSET 1u
#define UART_SR_OVRE_MASK 0x00000020u
#define UART_SR_OVRE_OFFSET 5u
#define UART_SR_FRAME_MASK 0x00000040u
#define UART_SR_FRAME_OFFSET 6u
#define UART_SR_PARE_MASK 0x00000080u
#define UART_SR_PARE_OFFSET 7u
#define UART_SR_TXEMPTY_MASK 0x00000200u
#define UART_SR_TXEMPTY_OFFSET 9u
#define UART_SR_CMP_MASK 0x00008000u
#define UART_SR_CMP_OFFSET 15u

#define UART_RHR_RXCHR_MASK 0x000000FFu
#define UART_RHR_RXCHR_OFFSET 0u

#define UART_THR_TXCHR_MASK 0x000000FFu
#define UART_THR_TXCHR_OFFSET 0u

#define UART_BRGR_CD_MASK 0x0000FFFFu
#define UART_BRGR_CD_OFFSET 0u

#endif // BSP_UART_REGISTERS_H
