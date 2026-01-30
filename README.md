# SAMV71Q21 Board Support Package (BSP)

Set of low-level drivers for Microchip's SAMV71Q21 and SAMRH71F20 Cortex-M7 ARM microcontrollers.

Based on pre-qualified for ECSS Criticality Category B version from https://bootloader.space.

Developed according to [European Cooperation for Space Standardization](https://ecss.nl) standards (ECSS-E-ST-40C, ECSS-Q-ST-80C).

Full version supports SAMV71Q21, SAMRH71F20, SAMRH707F18 and comes with complete ECSS compliant qualification data package (QDP).

List below includes drivers not distributed in the open-source version.

Drivers supported
 * FPU – Floating Point Unit
 * MCAN – Controller Area Network
 * MPU – Memory Protection Unit
 * NVIC – Nested Vectored Interrupt Controller
 * PIO – Parallel Input/Output Controller
 * PMC – Power Management Controller
 * PWM – Pulse Width Modulation Controller
 * RSTC – Reset Controller
 * RTC – Real-time Clock
 * SCB – System Control Block
 * SPI – Serial Peripheral Interface
 * SUPC – Supply Controller
 * SYSTICK – System timer
 * TC – Timer Counter
 * UART – Universal Asynchronous Receiver Transmitter
 * WDT – Watchdog Timer
 * XDMAC – DMA Controller

SAMV71Q21 specific drivers
 * AFEC - Analog Front-End Controller
 * DACC - Digital Analog Converter Controller
 * EEFC - Enhanced Embedded Flash Controller
 * GMAC - Ethernet
 * ISI - Image Sensor Interface
 * LPOW - Low-power modes
 * QSPI - Quad Serial Peripheral Interface
 * RSWDT - Reinforced Safety Watchdog Timer
 * RTT - Real-time Timer
 * SDRAMC - SDRAM Controller
 * TWIHS - Two-wire Interface

SAMRH71F20 specific drivers
 * FLEXCOM - Flexible Serial Communication Controller
 * FLEXRAMECC - FlexRAM Memory and Embedded Hardened ECC Controller
 * GMAC - Ethernet
 * HEFC - Hardened Embedded Flash Controller
 * HEMC - Hardened External Memory Controller (including HECC)
 * HSDRAMC - Hardened SDRAM Controller
 * HSMC - Hardened Static Memory Controller
 * MATRIX - Bus Matrix
 * RTT - Real-time Timer
 * SPW - SpaceWire
 * TCM - Tightly Coupled Memory
 * TWIHS - Two-wire Interface

SAMRH707F18 specific drivers
 * ADC - Analog-to-Digital Controller
 * DACC - Digital Analog Converter Controller
 * FLEXCOM - Flexible Serial Communication Controller
 * FLEXRAMECC - FlexRAM Memory and Embedded Hardened ECC Controller
 * HEFC - Hardened Embedded Flash Controller
 * HEMC - Hardened External Memory Controller (including HECC)
 * HSMC - Hardened Static Memory Controller
 * MATRIX - Bus Matrix
 * SPW - SpaceWire
 * TCM - Tightly Coupled Memory

RTEMS integration layer
 * RTEMS BSP implementation based on ECSS qualified BSP
 * RTEMS BSP is a minimal set of drivers needed for booting RTEMS on the target platform
 * It includes wrappers for the SYSTICK, RTC and UART drivers for the pre-qualified BSP
 * All drivers from the ECSS qualified BSP can be integrated into RTEMS – the code is portable and the IRQ layer of the drivers is an abstraction that can be integrated by users with both bare-metal and RTOS-based applications
