/**@file
 * This file is part of the ARM BSP for the Test Environment.
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

/// \file Nvic.h
/// \addtogroup Bsp
/// \brief Header for the Nested Vectored Interrupt Controller (NVIC) driver.

#ifndef BSP_NVIC_H
#define BSP_NVIC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/// @addtogroup Nvic
/// @ingroup Bsp
/// @{

#ifdef __cplusplus
extern "C" {
#endif

/// \brief A typedef for a function that can be used to register an interrupt handler.
typedef void (*Nvic_InterruptHandler)(void);

/// \brief An enumeration of hardware interrupts handled by the microcontroller.
typedef enum {
	Nvic_Irq_NonMaskable = -14, /// \brief Exception - non-maskable IRQ
	Nvic_Irq_HardFault = -13, /// \brief Exception - hardware fault
	Nvic_Irq_MemoryManagement =
			-12, /// \brief Exception - memory management fault
	Nvic_Irq_BusFault = -11, /// \brief Exception - bus fault
	Nvic_Irq_UsageFault = -10, /// \brief Exception - usage fault
	Nvic_Irq_SVCall = -5, /// \brief Exception - supervisor call
	Nvic_Irq_DebugMonitor = -4, /// \brief Reserved for debug.
	Nvic_Irq_PendSV =
			-2, /// \brief IRQ-driven request for system-level service
	Nvic_Irq_SysTick =
			-1, /// \brief Exception - zero reached by system timer
	Nvic_Irq_Supply = 0, /// \brief Supply Controller
	Nvic_Irq_Reset = 1, /// \brief Reset Controller
	Nvic_Irq_RealTimeClock = 2, /// \brief Real-time Clock
	Nvic_Irq_RealTimeTimer = 3, /// \brief Real-time Timer
	Nvic_Irq_WatchdogTimer = 4, /// \brief Watchdog Timer
	Nvic_Irq_PowerManagement = 5, /// \brief Power Management Controller
#if defined(N7S_TARGET_SAMV71Q21) // SAMV71Q21
	Nvic_Irq_EmbeddedFlash =
			6, /// \brief Enhanced Embedded Flash Controller
	Nvic_Irq_Uart0 = 7, /// \brief UART 0
	Nvic_Irq_Uart1 = 8, /// \brief UART 1
	Nvic_Irq_PioA = 10, /// \brief PIO Controller A
	Nvic_Irq_PioB = 11, /// \brief PIO Controller B
	Nvic_Irq_PioC = 12, /// \brief PIO Controller C
	Nvic_Irq_Usart0 = 13, /// \brief USART 0
	Nvic_Irq_Usart1 = 14, /// \brief USART 1
	Nvic_Irq_Usart2 = 15, /// \brief USART 2
	Nvic_Irq_PioD = 16, /// \brief PIO Controller D
	Nvic_Irq_PioE = 17, /// \brief PIO Controller E
	Nvic_Irq_Hsmci = 18, /// \brief High-speed Multimedia Card Interface
	Nvic_Irq_Twihs0 = 19, /// \brief Two-wire Interface 0
	Nvic_Irq_Twihs1 = 20, /// \brief Two-wire Interface 1
	Nvic_Irq_Spi0 = 21, /// \brief SPI 0
	Nvic_Irq_SynchronousSerial =
			22, /// \brief Synchronous Serial Controller
	Nvic_Irq_Timer0_Channel0 = 23, /// \brief Timer Counter 0, Channel 0
	Nvic_Irq_Timer0_Channel1 = 24, /// \brief Timer Counter 0, Channel 1
	Nvic_Irq_Timer0_Channel2 = 25, /// \brief Timer Counter 0, Channel 2
	Nvic_Irq_Timer1_Channel0 = 26, /// \brief Timer Counter 1, Channel 0
	Nvic_Irq_Timer1_Channel1 = 27, /// \brief Timer Counter 1, Channel 1
	Nvic_Irq_Timer1_Channel2 = 28, /// \brief Timer Counter 1, Channel 2
	Nvic_Irq_Afec0 = 29, /// \brief Analog Front-End Controller 0
	Nvic_Irq_Dac = 30, /// \brief Digital-to-Analog Converter
	Nvic_Irq_Pwm0 = 31, /// \brief Pulse Width Modulation Controller 0
	Nvic_Irq_IntegrityCheckMonitor = 32, /// \brief Integrity Check Monitor
	Nvic_Irq_AnalogComparator = 33, /// \brief Analog Comparator Controller
	Nvic_Irq_UsbHost = 34, /// \brief USB Host / Device Controller
	Nvic_Irq_Mcan0_Irq0 = 35, /// \brief CAN 0 Line 0
	Nvic_Irq_Mcan0_Irq1 = 36, /// \brief CAN 0 Line 1
	Nvic_Irq_Mcan1_Irq0 = 37, /// \brief CAN 1 Line 0
	Nvic_Irq_Mcan1_Irq1 = 38, /// \brief CAN 1 Line 1
	Nvic_Irq_Gmac = 39, /// \brief Ethernet MAC
	Nvic_Irq_Afec1 = 40, /// \brief Analog Front-End Controller 1
	Nvic_Irq_Twihs2 = 41, /// \brief Two-wire Interface 2
	Nvic_Irq_Spi1 = 42, /// \brief SPI 1
	Nvic_Irq_Qspi = 43, /// \brief Quad I/O SPI
	Nvic_Irq_Uart2 = 44, /// \brief UART 2
	Nvic_Irq_Uart3 = 45, /// \brief UART 3
	Nvic_Irq_Uart4 = 46, /// \brief UART 4
	Nvic_Irq_Timer2_Channel0 = 47, /// \brief Timer Counter 2, Channel 0
	Nvic_Irq_Timer2_Channel1 = 48, /// \brief Timer Counter 2, Channel 1
	Nvic_Irq_Timer2_Channel2 = 49, /// \brief Timer Counter 2, Channel 2
	Nvic_Irq_Timer3_Channel0 = 50, /// \brief Timer Counter 3, Channel 0
	Nvic_Irq_Timer3_Channel1 = 51, /// \brief Timer Counter 3, Channel 1
	Nvic_Irq_Timer3_Channel2 = 52, /// \brief Timer Counter 3, Channel 2
	Nvic_Irq_MediaLB_Irq0 = 53, /// \brief MediaLB IRQ 0
	Nvic_Irq_MediaLB_Irq1 = 54, /// \brief MediaLB IRQ 1
	Nvic_Irq_Aes = 56, /// \brief Advanced Encryption Standard IRQ
	Nvic_Irq_TrueRng = 57, /// \brief True Random Number Generator
	Nvic_Irq_Xdmac = 58, /// \brief DMA Controller
	Nvic_Irq_Isi = 59, /// \brief Image Sensor Interface
	Nvic_Irq_Pwm1 = 60, /// \brief Pulse Width Modulation Controller 1
	Nvic_Irq_Fpu = 61, /// \brief Floating-Point Unit (OFC, IFC, IOC, DZC, IDC bits)
	Nvic_Irq_Sdramc = 62, /// \brief SDRAM Controller
	Nvic_Irq_ReinforcedSafetyWatchdog =
			63, /// \brief Reinforced Safety Watchdog Timer
	Nvic_Irq_CacheWarning = 64, /// \brief Cache ECC Warning
	Nvic_Irq_CacheFault = 65, /// \brief Cache ECC Fault
	Nvic_Irq_Gmac_Queue1 = 66, /// \brief Ethernet MAC Queue 1 DMA IRQ
	Nvic_Irq_Gmac_Queue2 = 67, /// \brief Ethernet MAC Queue 2 DMA IRQ
	Nvic_Irq_Fpu_Ixc =
			68, /// \brief Floating-Point Unit IRQ (cumulative exception bit)
	Nvic_Irq_I2Sound0 = 69, /// \brief Inter-IC Sound Controller 0
	Nvic_Irq_I2Sound1 = 70, /// \brief Inter-IC Sound Controller 1
	Nvic_Irq_Gmac_Queue3 = 71, /// \brief Ethernet MAC Queue 3 DMA IRQ
	Nvic_Irq_Gmac_Queue4 = 72, /// \brief Ethernet MAC Queue 4 DMA IRQ
	Nvic_Irq_Gmac_Queue5 = 73, /// \brief Ethernet MAC Queue 5 DMA IRQ
	Nvic_InterruptCount = 74, /// \brief Interrupt count
#elif defined(N7S_TARGET_SAMRH71F20) // SAMRH71F20
	Nvic_Irq_Matrix0 = 6, /// \brief Matrix 0
	Nvic_Irq_Flexcom0 = 7, /// \brief FLEXCOM 0
	Nvic_Irq_Flexcom1 = 8, /// \brief FLEXCOM 1
	Nvic_Irq_Nmic = 9, /// \brief Non-maskable Interrupt Controller
	Nvic_Irq_PioA = 10, /// \brief PIO Controller A
	Nvic_Irq_PioB = 11, /// \brief PIO Controller B
	Nvic_Irq_PioC = 12, /// \brief PIO Controller C
	Nvic_Irq_Flexcom2 = 13, /// \brief FLEXCOM 2
	Nvic_Irq_Flexcom3 = 14, /// \brief FLEXCOM 3
	Nvic_Irq_Flexcom4 = 15, /// \brief FLEXCOM 4
	Nvic_Irq_PioD = 16, /// \brief PIO Controller D
	Nvic_Irq_PioE = 17, /// \brief PIO Controller E
	Nvic_Irq_EccWarning = 18, /// \brief Cache ECC Warning
	Nvic_Irq_EccFault = 19, /// \brief Cache ECC Fault
	Nvic_Irq_Fpu = 20, /// \brief Floating-point Unit, except IXC
	Nvic_Irq_Ixc = 21, /// \brief Cumulative exception bit in FPU
	Nvic_Irq_Flexcom5 = 22, /// \brief FLEXCOM 5
	Nvic_Irq_Flexcom6 = 23, /// \brief FLEXCOM 6
	Nvic_Irq_Flexcom7 = 24, /// \brief FLEXCOM 7
	Nvic_Irq_Timer0_Channel0 = 25, /// \brief Timer Counter 0 Channel 0
	Nvic_Irq_Timer0_Channel1 = 26, /// \brief Timer Counter 0 Channel 1
	Nvic_Irq_Timer0_Channel2 = 27, /// \brief Timer Counter 0 Channel 2
	Nvic_Irq_Timer1_Channel0 = 28, /// \brief Timer Counter 1 Channel 0
	Nvic_Irq_Timer1_Channel1 = 29, /// \brief Timer Counter 1 Channel 1
	Nvic_Irq_Timer1_Channel2 = 30, /// \brief Timer Counter 1 Channel 2
	Nvic_Irq_Pwm0 = 31, /// \brief Pulse Width Modulation 0
	Nvic_Irq_Pwm1 = 32, /// \brief Pulse Width Modulation 1
	Nvic_Irq_IntegrityCheckMonitor = 33, /// \brief Integrity Check Monitor
	Nvic_Irq_PioF = 34, /// \brief PIO Controller F
	Nvic_Irq_PioG = 35, /// \brief PIO Controller G
	Nvic_Irq_Mcan0_Irq0 = 36, /// \brief MCAN Controller 0
	Nvic_Irq_Mcan0_Irq1 = 37, /// \brief MCAN 0 IRQ 1
	Nvic_Irq_Mcan1_Irq0 = 38, /// \brief MCAN Controller 1
	Nvic_Irq_Mcan1_Irq1 = 39, /// \brief MCAN 1 IRQ 1
	Nvic_Irq_TCMRAM_Fixable =
			40, /// \brief TCM RAM-HECC Fixable error detected
	Nvic_Irq_TCMRAM_Unfixable =
			41, /// \brief TCM RAM-HECC Unfixable error detected
	Nvic_Irq_FlexRAM_Fixable =
			42, /// \brief FlexRAM-HECC Fixable error detected
	Nvic_Irq_FlexRAM_Unfixable =
			43, /// \brief FlexRAM-HECC Unfixable error detected
	Nvic_Irq_Sha = 44, /// \brief Secure Hash Algorithm
	Nvic_Irq_Flexcom8 = 45, /// \brief FLEXCOM 8
	Nvic_Irq_Flexcom9 = 46, /// \brief FLEXCOM 9
	Nvic_Irq_Watchdog1 = 47, /// \brief Watchdog Timer 1
	Nvic_Irq_Qspi = 49, /// \brief Quad I/O Serial Peripheral Interface
	Nvic_Irq_Hefc = 50, /// \brief HEFC IRQ - Writing clock can be disabled
	Nvic_Irq_Hefc_Fixable = 51, /// \brief HEFC-HECC Fixable error detected
	Nvic_Irq_Hefc_Unfixable =
			52, /// \brief HEFC-HECC Unfixable error detected
	Nvic_Irq_Timer2_Channel0 = 53, /// \brief Timer Counter 2, Channel 0
	Nvic_Irq_Timer2_Channel1 = 54, /// \brief Timer Counter 2, Channel 1
	Nvic_Irq_Timer2_Channel2 = 55, /// \brief Timer Counter 2, Channel 2
	Nvic_Irq_Timer3_Channel0 = 56, /// \brief Timer Counter 3, Channel 0
	Nvic_Irq_Timer3_Channel1 = 57, /// \brief Timer Counter 3, Channel 1
	Nvic_Irq_Timer3_Channel2 = 58, /// \brief Timer Counter 3, Channel 2
	Nvic_Irq_Hemc = 59, /// \brief HEMC - SDRAM Controller
	Nvic_Irq_Hemc_Fixable = 60, /// \brief HEMC-HECC Fixable error detected
	Nvic_Irq_Hemc_Unfixable =
			61, /// \brief HEMC-HECC Unfixable error detected
	Nvic_Irq_Sfr = 62, /// \brief Special Function Register
	Nvic_Irq_TrueRng = 63, /// \brief True Random Number Generator
	Nvic_Irq_Xdmac = 64, /// \brief DMA Controller
	Nvic_Irq_Spw_Irq0 = 65, /// \brief Spacewire IRQ 0
	Nvic_Irq_Spw_Irq1 = 66, /// \brief Spacewire IRQ 1
	Nvic_Irq_Ip1553 = 68, /// \brief MIL-STD-1553
	Nvic_Irq_Gmac = 69, /// \brief Ethernet MAC
	Nvic_Irq_Gmac_Queue1 = 70, /// \brief Ethernet MAC Queue 1 DMA IRQ
	Nvic_Irq_Gmac_Queue2 = 71, /// \brief Ethernet MAC Queue 2 DMA IRQ
	Nvic_Irq_Gmac_Queue3 = 72, /// \brief Ethernet MAC Queue 3 DMA IRQ
	Nvic_Irq_Gmac_Queue4 = 73, /// \brief Ethernet MAC Queue 4 DMA IRQ
	Nvic_Irq_Gmac_Queue5 = 74, /// \brief Ethernet MAC Queue 5 DMA IRQ
	Nvic_InterruptCount = 91, /// \brief Interrupt count
#elif defined(N7S_TARGET_SAMRH707F18) // SAMRH707F18
	Nvic_Irq_Matrix0 = 6, /// \brief Matrix 0
	Nvic_Irq_Flexcom0 = 7, /// \brief FLEXCOM 0
	Nvic_Irq_Flexcom1 = 8, /// \brief FLEXCOM 1
	Nvic_Irq_Nmic = 9, /// \brief Non-maskable Interrupt Controller
	Nvic_Irq_PioA = 10, /// \brief PIO Controller A
	Nvic_Irq_PioB = 11, /// \brief PIO Controller B
	Nvic_Irq_PioC = 12, /// \brief PIO Controller C
	Nvic_Irq_Flexcom2 = 13, /// \brief FLEXCOM 2
	Nvic_Irq_Flexcom3 = 14, /// \brief FLEXCOM 3
	Nvic_Irq_PioD = 16, /// \brief PIO Controller D
	Nvic_Irq_EccWarning = 18, /// \brief Cache ECC Warning
	Nvic_Irq_EccFault = 19, /// \brief Cache ECC Fault
	Nvic_Irq_Fpu = 20, /// \brief Floating-point Unit, except IXC
	Nvic_Irq_Ixc = 21, /// \brief Cumulative exception bit in FPU
	Nvic_Irq_Crccu = 22, /// \brief CRCCU_HCBDMA
	Nvic_Irq_Adc = 23, /// \brief ADC controller
	Nvic_Irq_Dacc = 24, /// \brief DAC Controller
	Nvic_Irq_Timer0_Channel0 = 25, /// \brief Timer Counter 0 Channel 0
	Nvic_Irq_Timer0_Channel1 = 26, /// \brief Timer Counter 0 Channel 1
	Nvic_Irq_Timer0_Channel2 = 27, /// \brief Timer Counter 0 Channel 2
	Nvic_Irq_Timer1_Channel0 = 28, /// \brief Timer Counter 1 Channel 0
	Nvic_Irq_Timer1_Channel1 = 29, /// \brief Timer Counter 1 Channel 1
	Nvic_Irq_Timer1_Channel2 = 30, /// \brief Timer Counter 1 Channel 2
	Nvic_Irq_Pwm0 = 31, /// \brief Pulse Width Modulation 0
	Nvic_Irq_Pwm1 = 32, /// \brief Pulse Width Modulation 1
	Nvic_Irq_IntegrityCheckMonitor = 33, /// \brief Integrity Check Monitor
	Nvic_Irq_Mcan0_Irq0 = 36, /// \brief MCAN Controller 0
	Nvic_Irq_Mcan0_Irq1 = 37, /// \brief MCAN 0 IRQ 1
	Nvic_Irq_Mcan1_Irq0 = 38, /// \brief MCAN Controller 1
	Nvic_Irq_Mcan1_Irq1 = 39, /// \brief MCAN 1 IRQ 1
	Nvic_Irq_TCMRAM_Fixable =
			40, /// \brief TCM RAM-HECC Fixable error detected
	Nvic_Irq_TCMRAM_Unfixable =
			41, /// \brief TCM RAM-HECC Unfixable error detected
	Nvic_Irq_FlexRAM_Fixable =
			42, /// \brief FlexRAM-HECC Fixable error detected
	Nvic_Irq_FlexRAM_Unfixable =
			43, /// \brief FlexRAM-HECC Unfixable error detected
	Nvic_Irq_Sha = 44, /// \brief Secure Hash Algorithm
	Nvic_Irq_Pcc = 45, /// \brief PCC controller
	Nvic_Irq_Watchdog1 = 47, /// \brief Watchdog Timer 1
	Nvic_Irq_Hefc = 50, /// \brief HEFC IRQ - Writing clock can be disabled
	Nvic_Irq_Hefc_Fixable = 51, /// \brief HEFC-HECC Fixable error detected
	Nvic_Irq_Hefc_Unfixable =
			52, /// \brief HEFC-HECC Unfixable error detected
	Nvic_Irq_Timer2_Channel0 = 53, /// \brief Timer Counter 2, Channel 0
	Nvic_Irq_Timer2_Channel1 = 54, /// \brief Timer Counter 2, Channel 1
	Nvic_Irq_Timer2_Channel2 = 55, /// \brief Timer Counter 2, Channel 2
	Nvic_Irq_Hemc_Fixable = 60, /// \brief HEMC-HECC Fixable error detected
	Nvic_Irq_Hemc_Unfixable =
			61, /// \brief HEMC-HECC Unfixable error detected
	Nvic_Irq_TrueRng = 63, /// \brief True Random Number Generator
	Nvic_Irq_Xdmac = 64, /// \brief DMA Controller
	Nvic_Irq_Spw_Irq0 = 65, /// \brief Spacewire IRQ 0
	Nvic_Irq_Spw_Irq1 = 66, /// \brief Spacewire IRQ 1
	Nvic_Irq_Ip1553 = 68, /// \brief MIL-STD-1553
	Nvic_InterruptCount = 80, /// \brief Interrupt count
#endif // --
} Nvic_Irq;

/// \brief General numeration of driver-specific constants.
enum {
	Nvic_SystemExceptionCount = 16, /// \brief Number of system exceptions.
};

/// \brief Function used to enable an interrupt in the NVIC.
/// \param [in] irqn Numeric identifier of the interrupt to enable.
void Nvic_enableInterrupt(const Nvic_Irq irqn);

/// \brief Function used to disable an interrupt in the NVIC.
/// \param [in] irqn Numeric identifier of the interrupt to disable.
void Nvic_disableInterrupt(const Nvic_Irq irqn);

/// \brief Function used to check whether an interrupt is enabled in the NVIC.
/// \param [in] irqn Numeric identifier of the interrupt to check.
bool Nvic_isInterruptEnabled(const Nvic_Irq irqn);

/// \brief Function used to force an interrupt pending signal on an interrupt line.
/// \param [in] irqn Numeric identifier of the interrupt to trigger.
void Nvic_setInterruptPending(const Nvic_Irq irqn);

/// \brief Function used to clear an interrupt pending signal on an interrupt line.
/// \param [in] irqn Numeric identifier of the interrupt to clear.
void Nvic_clearInterruptPending(const Nvic_Irq irqn);

/// \brief Function used to check if an interrupt pending signal is asserted on an interrupt line.
/// \param [in] irqn Numeric identifier of the interrupt to check.
bool Nvic_isInterruptPending(const Nvic_Irq irqn);

/// \brief Function used to check if an interrupt is currently being processed.
/// \param [in] irqn Numeric identifier of the interrupt to check.
bool Nvic_isInterruptActive(const Nvic_Irq irqn);

/// \brief Function used to set priority of an interrupt.
/// \param [in] irqn Numeric identifier of the interrupt to modify.
/// \param [in] priority Priority to set for the interrupt.
void Nvic_setInterruptPriority(const Nvic_Irq irqn, uint8_t priority);

/// \brief Function used to check the priority of an interrupt.
/// \param [in] irqn Numeric identifier of the interrupt to check.
/// \returns Priority of the specified interrupt.
uint8_t Nvic_getInterruptPriority(const Nvic_Irq irqn);

/// \brief Function used to trigger a software-generated interrupt.
/// \param [in] irqn Numeric identifier of the interrupt to trigger.
void Nvic_triggerInterrupt(const Nvic_Irq irqn);

/// \brief Function used to configure priority grouping.
/// \param [in] priorityGroup Index of the bit splitting between group priority and
///                           subpriority.
void Nvic_setPriorityGrouping(const uint8_t priorityGroup);

/// \brief Function used to retrieve the priority grouping configuration.
/// \returns Index of the bit splitting between group priority and subpriority.
uint8_t Nvic_getPriorityGrouping(void);

/// \brief Function used to change the address of the vector table in NVIC.
/// \details Function disables interrupts globally for vector address change,
///          then re-enables them afterwards. This has potentially unwanted side effect
///          of enabling interrupts even if they were disabled before.
///          also see ::Nvic_relocateVectorTableUnsafe .
/// \param [in] address Address of the new vector table.
void Nvic_relocateVectorTable(const void *const address);

/// \brief Function used to change the address of the vector table in NVIC.
///        Unsafe version does not disable interrupts for vector address change.
/// \param [in] address Address of the new vector table.
void Nvic_relocateVectorTableUnsafe(const void *const address);

/// \brief Function used to retrieve the address of the vector table in NVIC.
/// \returns Currently used base address of the vector table.
void *Nvic_getVectorTableAddress(void);

/// \brief Function used to set the address of an interrupt handler from the currently used
///        vector table.
/// \param [in] irqn Numeric identifier of the interrupt to set the handler of.
/// \param [in] address Pointer to the function to be used as a handler of the interrupt.
void Nvic_setInterruptHandlerAddress(
		const Nvic_Irq irqn, const Nvic_InterruptHandler address);

/// \brief Enable IRQ Interrupts.
static inline void
Nvic_enableIrq(void)
{
	asm volatile("cpsie i" : : : "memory");
}

/// \brief Disable IRQ Interrupts.
static inline void
Nvic_disableIrq(void)
{
	asm volatile("cpsid i" : : : "memory");
}

/// \brief Enable Fault IRQ Interrupts.
// LCOV_EXCL_START
// Faults are used by the GCOV itself, coverage cannot be gathered.
// The code was inspected and analysed - it is executed by the tests.
static inline void
Nvic_enableFaultIrq(void)
{
	asm volatile("cpsie f" : : : "memory");
}

// LCOV_EXCL_STOP

/// \brief Disable Fault IRQ Interrupts.
// LCOV_EXCL_START
// Faults are used by the GCOV itself, coverage cannot be gathered.
// The code was inspected and analysed - it is executed by the tests.
static inline void
Nvic_disableFaultIrq(void)
{
	asm volatile("cpsid f" : : : "memory");
}

// LCOV_EXCL_STOP

/// \brief Function used to retrieve the address of an interrupt handler from the currently used
///        vector table.
/// \param [in] irqn Numeric identifier of the interrupt to retrieve the handler of.
/// \returns Pointer to the handler.
Nvic_InterruptHandler Nvic_getInterruptHandlerAddress(const Nvic_Irq irqn);

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // BSP_NVIC_H
