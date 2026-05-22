/**@file
 * This file is part of the ARM BSP for the Test Environment.
 *
 * @copyright 2018-2026 N7 Space Sp. z o.o.
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

#ifndef N7S_BSP_UART_UARTREGISTERS_H
#define N7S_BSP_UART_UARTREGISTERS_H

#include <stdint.h>

typedef struct {
  uint32_t cr;  ///< 0x00 Control Register
  uint32_t mr;  ///< 0x04 Mode Register
  uint32_t ier; ///< 0x08 Interrupt Enable Register
  uint32_t idr; ///< 0x0C Interrupt Disable Register
  uint32_t imr; ///< 0x10 Interrupt Mask Register
  // On RH board the name of SR register is SCR but in fact it is doing
  // the same, thus to keep coherency renamed SCR to SR.
  uint32_t sr;   ///< 0x14 Status Register
  uint32_t rhr;  ///< 0x18 Receive Holding Register
  uint32_t thr;  ///< 0x1C Transmit Holding Register
  uint32_t brgr; ///< 0x20 Baud Rate Generator Register
#if defined(N7S_TARGET_SAMV71Q21)
  uint32_t cmpr;          ///< 0x24 Comparison Register
  uint32_t reserved1[6];  ///< 0x28 - 0x3C Reserved
  uint32_t reserved2[41]; ///< 0x40 - 0xE0 Reserved
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
  uint32_t reserved1[48]; ///< 0x24 - 0xE0 Reserved
#else
#error "No target platform specified (missing N7S_TARGET_* macro)"
#endif
  uint32_t wpmr; ///< 0xE4 Write Protection Mode Register
} Uart_Registers;

#if defined(N7S_TARGET_SAMV71Q21)
#define UART0_ADDRESS_BASE  0x400E0800u
#define UART1_ADDRESS_BASE  0x400E0A00u
#define UART2_ADDRESS_BASE  0x400E1A00u
#define UART3_ADDRESS_BASE  0x400E1C00u
#define UART4_ADDRESS_BASE  0x400E1E00u
#define UART_ADDRESS_OFFSET 0x00000200u
#elif defined(N7S_TARGET_SAMRH71F20)
#define UART0_ADDRESS_BASE  0x40010200u
#define UART1_ADDRESS_BASE  0x40014200u
#define UART2_ADDRESS_BASE  0x40018200u
#define UART3_ADDRESS_BASE  0x4001C200u
#define UART4_ADDRESS_BASE  0x40020200u
#define UART5_ADDRESS_BASE  0x40024200u
#define UART6_ADDRESS_BASE  0x40028200u
#define UART7_ADDRESS_BASE  0x4002C200u
#define UART8_ADDRESS_BASE  0x40030200u
#define UART9_ADDRESS_BASE  0x40034200u
#define UART_ADDRESS_OFFSET 0x00004000U
#elif defined(N7S_TARGET_SAMRH707F18)
#define UART0_ADDRESS_BASE  0x40020200u
#define UART1_ADDRESS_BASE  0x40024200u
#define UART2_ADDRESS_BASE  0x40028200u
#define UART3_ADDRESS_BASE  0x4002C200u
#define UART_ADDRESS_OFFSET 0x00004000U
#else
#error "No target platform specified (missing N7S_TARGET_* macro)"
#endif

#define UART_CR_RSTRX_MASK    0x00000004u
#define UART_CR_RSTRX_OFFSET  2u
#define UART_CR_RSTTX_MASK    0x00000008u
#define UART_CR_RSTTX_OFFSET  3u
#define UART_CR_RXEN_MASK     0x00000010u
#define UART_CR_RXEN_OFFSET   4u
#define UART_CR_RXDIS_MASK    0x00000020u
#define UART_CR_RXDIS_OFFSET  5u
#define UART_CR_TXEN_MASK     0x00000040u
#define UART_CR_TXEN_OFFSET   6u
#define UART_CR_TXDIS_MASK    0x00000080u
#define UART_CR_TXDIS_OFFSET  7u
#define UART_CR_RSTSTA_MASK   0x00000100u
#define UART_CR_RSTSTA_OFFSET 8u
#if defined(N7S_TARGET_SAMV71Q21)
#define UART_CR_REQCLR_MASK   0x00001000u
#define UART_CR_REQCLR_OFFSET 12u
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define UART_CR_REQCLR_MASK   0x10000000u
#define UART_CR_REQCLR_OFFSET 28u
#endif

#if defined(N7S_TARGET_SAMV71Q21)
#define UART_MR_FILTER_MASK   0x00000010u
#define UART_MR_FILTER_OFFSET 4u
#define UART_MR_BSRCCK_MASK   0x00001000u
#define UART_MR_BSRCCK_OFFSET 12u
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define UART_MR_FILTER_MASK   0x10000000u
#define UART_MR_FILTER_OFFSET 28u
#endif
#define UART_MR_PAR_MASK                     0x00000E00u
#define UART_MR_PAR_OFFSET                   9u
#define UART_MR_CHMODE_MASK                  0x0000C000u
#define UART_MR_CHMODE_OFFSET                14u
#define UART_MR_CHMODE_NORMAL_VALUE          0u
#define UART_MR_CHMODE_AUTOMATIC_VALUE       1u
#define UART_MR_CHMODE_LOCAL_LOOPBACK_VALUE  2u
#define UART_MR_CHMODE_REMOTE_LOOPBACK_VALUE 3u

#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define UART_MR_USART_MODE_MASK   0x0000000Fu
#define UART_MR_USART_MODE_OFFSET 0u
#define UART_MR_USCLKS_MASK       0x00000030u
#define UART_MR_USCLKS_OFFSET     4u
#define UART_MR_CHRL_MASK         0x000000C0u
#define UART_MR_CHRL_OFFSET       6u
#define UART_MR_SYNC_MASK         0x00000100u
#define UART_MR_SYNC_OFFSET       8u
#define UART_MR_NBSTOP_MASK       0x00003000U
#define UART_MR_NBSTOP_OFFSET     12u
#define UART_MR_INACK_MASK        0x00100000u
#define UART_MR_INACK_OFFSET      20u
#define UART_MR_ONEBIT_MASK       0x80000000u
#define UART_MR_ONEBIT_OFFSET     31u
#endif

#define UART_IER_RXRDY_MASK     0x00000001u
#define UART_IER_RXRDY_OFFSET   0u
#define UART_IER_TXRDY_MASK     0x00000002u
#define UART_IER_TXRDY_OFFSET   1u
#define UART_IER_OVRE_MASK      0x00000020u
#define UART_IER_OVRE_OFFSET    5u
#define UART_IER_FRAME_MASK     0x00000040u
#define UART_IER_FRAME_OFFSET   6u
#define UART_IER_PARE_MASK      0x00000080u
#define UART_IER_PARE_OFFSET    7u
#define UART_IER_TXEMPTY_MASK   0x00000200u
#define UART_IER_TXEMPTY_OFFSET 9u
#if defined(N7S_TARGET_SAMV71Q21)
#define UART_IER_CMP_MASK   0x00008000u
#define UART_IER_CMP_OFFSET 15u
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define UART_IER_CMP_MASK   0x00400000u
#define UART_IER_CMP_OFFSET 22u
#endif

#define UART_IDR_RXRDY_MASK     0x00000001u
#define UART_IDR_RXRDY_OFFSET   0u
#define UART_IDR_TXRDY_MASK     0x00000002u
#define UART_IDR_TXRDY_OFFSET   1u
#define UART_IDR_OVRE_MASK      0x00000020u
#define UART_IDR_OVRE_OFFSET    5u
#define UART_IDR_FRAME_MASK     0x00000040u
#define UART_IDR_FRAME_OFFSET   6u
#define UART_IDR_PARE_MASK      0x00000080u
#define UART_IDR_PARE_OFFSET    7u
#define UART_IDR_TXEMPTY_MASK   0x00000200u
#define UART_IDR_TXEMPTY_OFFSET 9u
#if defined(N7S_TARGET_SAMV71Q21)
#define UART_IDR_CMP_MASK   0x00008000u
#define UART_IDR_CMP_OFFSET 15u
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define UART_IDR_CMP_MASK   0x00400000u
#define UART_IDR_CMP_OFFSET 22u
#endif

#define UART_IMR_RXRDY_MASK     0x00000001u
#define UART_IMR_RXRDY_OFFSET   0u
#define UART_IMR_TXRDY_MASK     0x00000002u
#define UART_IMR_TXRDY_OFFSET   1u
#define UART_IMR_OVRE_MASK      0x00000020u
#define UART_IMR_OVRE_OFFSET    5u
#define UART_IMR_FRAME_MASK     0x00000040u
#define UART_IMR_FRAME_OFFSET   6u
#define UART_IMR_PARE_MASK      0x00000080u
#define UART_IMR_PARE_OFFSET    7u
#define UART_IMR_TXEMPTY_MASK   0x00000200u
#define UART_IMR_TXEMPTY_OFFSET 9u
#if defined(N7S_TARGET_SAMV71Q21)
#define UART_IMR_CMP_MASK   0x00008000u
#define UART_IMR_CMP_OFFSET 15u
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define UART_IMR_CMP_MASK   0x00400000u
#define UART_IMR_CMP_OFFSET 22u
#endif

#define UART_SR_RXRDY_MASK     0x00000001u
#define UART_SR_RXRDY_OFFSET   0u
#define UART_SR_TXRDY_MASK     0x00000002u
#define UART_SR_TXRDY_OFFSET   1u
#define UART_SR_OVRE_MASK      0x00000020u
#define UART_SR_OVRE_OFFSET    5u
#define UART_SR_FRAME_MASK     0x00000040u
#define UART_SR_FRAME_OFFSET   6u
#define UART_SR_PARE_MASK      0x00000080u
#define UART_SR_PARE_OFFSET    7u
#define UART_SR_TXEMPTY_MASK   0x00000200u
#define UART_SR_TXEMPTY_OFFSET 9u
#if defined(N7S_TARGET_SAMV71Q21)
#define UART_SR_CMP_MASK   0x00008000u
#define UART_SR_CMP_OFFSET 15u
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define UART_SR_CMP_MASK   0x00400000u
#define UART_SR_CMP_OFFSET 22u
#endif

#if defined(N7S_TARGET_SAMV71Q21)
#define UART_RHR_RXCHR_MASK   0x000000FFu
#define UART_RHR_RXCHR_OFFSET 0u
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define UART_RHR_RXCHR_MASK   0x000001FFu
#define UART_RHR_RXCHR_OFFSET 0u
#endif

#if defined(N7S_TARGET_SAMV71Q21)
#define UART_THR_TXCHR_MASK   0x000000FFu
#define UART_THR_TXCHR_OFFSET 0u
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define UART_THR_TXCHR_MASK   0x000001FFu
#define UART_THR_TXCHR_OFFSET 0u
#endif

#define UART_BRGR_CD_MASK   0x0000FFFFu
#define UART_BRGR_CD_OFFSET 0u
#if defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
#define UART_BRGR_FP_MASK   0x00070000u
#define UART_BRGR_FP_OFFSET 16u
#endif

#endif // N7S_BSP_UART_UARTREGISTERS_H
