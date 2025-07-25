/**@file
 * This file is part of the ARM BSP for the Test Environment.
 *
 * @copyright 2020-2024 N7 Space Sp. z o.o.
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

/// \file Matrix.h
/// \addtogroup Bsp
/// \brief Header for the Bus Matrix (MATRIX) driver.

#ifndef BSP_MATRIX_H
#define BSP_MATRIX_H

#include <stdbool.h>
#include <stdint.h>

#include "MatrixRegisters.h"

/// @addtogroup Matrix
/// @ingroup Bsp
/// @{

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Enumeration listing possible Masters.
typedef enum {
	Matrix_Master_CortexM7Port0 = 0, ///< Cortex-M7 Port 0 Master.
	Matrix_Master_CortexM7Port1 = 1, ///< Cortex-M7 Port 1 Master.
	Matrix_Master_CortexM7PeripheralPort =
			2, ///< Cortex-M7 Peripheral Port (AHBP) Master.
	Matrix_Master_XdmacPort0 = 3, ///< XDMAC Port 0 Master.
	Matrix_Master_Can0Dma = 4, ///< CAN0 DMA Master.
	Matrix_Master_Can1Dma = 5, ///< CAN1 DMA Master.
#if defined(N7S_TARGET_SAMRH71F20)
	Matrix_Master_GmacDma = 6, ///< GMAC DMA Master.
#elif defined(N7S_TARGET_SAMRH707F18)
	Matrix_Master_XdmacPort1 = 6, ///< XDMAC Port 1 Master.
#else
#error "No target platform specified (missing N7S_TARGET_* macro)"
#endif
	Matrix_Master_SpwTx = 7, ///< SPW 0–TX Master.
	Matrix_Master_SpwRx = 8, ///< SPW 1–RX Master.
	Matrix_Master_SpwRmap = 9, ///< SPW 2–RMAP Master.
	Matrix_Master_Ip1553 = 10, ///< IP1553 Master.
	Matrix_Master_Icm = 11, ///< ICM Master.
#if defined(N7S_TARGET_SAMRH71F20)
	Matrix_Master_XdmacPort1 = 12, ///< XDMAC Port 1 Master.
#elif defined(N7S_TARGET_SAMRH707F18)
	Matrix_Master_Crccu = 12, ///< Crccu Master.
#endif
	Matrix_Master_Count = 13, ///< Number of all possible Masters.
} Matrix_Master;

/// \brief Enumeration listing possible Slaves.
typedef enum {
	Matrix_Slave_Flexram0 = 0, ///< FlexRAM Slave.
	Matrix_Slave_Flexram1 = 1, ///< FlexRAM Slave.
	Matrix_Slave_Hefc = 2, ///< Internal Flash (HEFC) Slave.
	Matrix_Slave_Hemc = 3, ///< HEMC Slave.
#if defined(N7S_TARGET_SAMRH71F20)
	Matrix_Slave_Qspi = 4, ///< QSPI Slave.
#elif defined(N7S_TARGET_SAMRH707F18)
	Matrix_Slave_Rom = 4, ///< Internal ROM Slave.
#endif
	Matrix_Slave_PeripheralBridge = 5, ///< Peripheral Bridge Slave.
	Matrix_Slave_AhbSlave = 6, ///< AHB Slave.
	Matrix_Slave_Flexram2 = 7, ///< FlexRAM Slave.
#if defined(N7S_TARGET_SAMRH707F18)
	Matrix_Slave_Adc = 8, ///< FlexRAM Slave.
	Matrix_Slave_Count = 9, ///< Number of all possible Slaves.
#elif defined(N7S_TARGET_SAMRH71F20)
	Matrix_Slave_Count = 8, ///< Number of all possible Slaves.
#endif
} Matrix_Slave;

/// \brief Enumeration listing possible Peripherals.
typedef enum {
#if defined(N7S_TARGET_SAMRH71F20)
	Matrix_Peripheral_Supc = 0, ///< Supply Controller.
	Matrix_Peripheral_Rstc = 1, ///< Reset Controller.
	Matrix_Peripheral_Rtc = 2, ///< Real Time Clock.
	Matrix_Peripheral_Rtt = 3, ///< Real Time Timer.
	Matrix_Peripheral_Wdt = 4, ///< Watchdog Timer.
	Matrix_Peripheral_Pmc = 5, ///< Power Management Controller.
	Matrix_Peripheral_Matrix0 = 6, ///< Matrix 0.
	Matrix_Peripheral_Flexcom0 = 7, ///< FLEXCOM 0.
	Matrix_Peripheral_Flexcom1 = 8, ///< FLEXCOM 1.
	Matrix_Peripheral_Nmic = 9, ///< NMI Controller.
	Matrix_Peripheral_PioA = 10, ///< Parallel I/O Controller A.
	Matrix_Peripheral_PioB = 11, ///< Parallel I/O Controller B.
	Matrix_Peripheral_PioC = 12, ///< Parallel I/O Controller C.
	Matrix_Peripheral_Flexcom2 = 13, ///< FLEXCOM 2.
	Matrix_Peripheral_Flexcom3 = 14, ///< FLEXCOM 3.
	Matrix_Peripheral_Flexcom4 = 15, ///< FLEXCOM 4.
	Matrix_Peripheral_PioD = 16, ///< Parallel I/O Controller D.
	Matrix_Peripheral_PioE = 17, ///< Parallel I/O Controller E.
	Matrix_Peripheral_Arm_CacheWarning = 18, ///< Arm Cache ECC Warning.
	Matrix_Peripheral_Arm_CacheFault = 19, ///< Arm Cache ECC Fault.
	Matrix_Peripheral_Arm_Fpu = 20, ///< FPU except IXC.
	Matrix_Peripheral_Arm_FpuIxc =
			21, ///< FPU Interrupt IXC (cumulative exception bit).
	Matrix_Peripheral_Flexcom5 = 22, ///< FLEXCOM 5.
	Matrix_Peripheral_Flexcom6 = 23, ///< FLEXCOM 6.
	Matrix_Peripheral_Flexcom7 = 24, ///< FLEXCOM 7.
	Matrix_Peripheral_Tc0 = 25, ///< Timer Counter Channel 0.
	Matrix_Peripheral_Tc1 = 26, ///< Timer Counter Channel 1.
	Matrix_Peripheral_Tc2 = 27, ///< Timer Counter Channel 2.
	Matrix_Peripheral_Tc3 = 28, ///< Timer Counter Channel 3.
	Matrix_Peripheral_Tc4 = 29, ///< Timer Counter Channel 4.
	Matrix_Peripheral_Tc5 = 30, ///< Timer Counter Channel 5.
	Matrix_Peripheral_Pwm0 = 31, ///< Pulse Width Modulation 0.
	Matrix_Peripheral_Pwm1 = 32, ///< Pulse Width Modulation 1.
	Matrix_Peripheral_Icm = 33, ///< Integrity Check Monitor.
	Matrix_Peripheral_PioF = 34, ///< Parallel I/O Controller F.
	Matrix_Peripheral_PioG = 35, ///< Parallel I/O Controller G.
	Matrix_Peripheral_Mcan0 = 36, ///< MCAN Controller 0.
	Matrix_Peripheral_Mcan0_Irq1 = 37, ///< MCAN 0 IRQ 1.
	Matrix_Peripheral_Mcan1 = 38, ///< MCAN Controller 1.
	Matrix_Peripheral_Mcan1_Irq1 = 39, ///< MCAN 1 IRQ 1.
	Matrix_Peripheral_TcmEcc_Fix =
			40, ///< TCM RAM–HECC Fixable error detected.
	Matrix_Peripheral_TcmEcc_NoFix =
			41, ///< TCM RAM–HECC Unfixable error detected.
	Matrix_Peripheral_FlexramEcc_Fix =
			42, ///< FlexRAM–HECC Fixable error detected.
	Matrix_Peripheral_FlexramEcc_NoFix =
			43, ///< FlexRAM–HECC Unfixable error detected.
	Matrix_Peripheral_Sha = 44, ///< Secure Hash Algorithm.
	Matrix_Peripheral_Flexcom8 = 45, ///< FLEXCOM 8.
	Matrix_Peripheral_Flexcom9 = 46, ///< FLEXCOM 9.
	Matrix_Peripheral_Wdt1 = 47, ///< Reinforced Safety Watchdog Timer 1.
	Matrix_Peripheral_Qspi = 49, ///< Quad I/O Serial Peripheral Interface.
	Matrix_Peripheral_Hefc = 50, ///< HEFC–Clock for writing in Flash.
	Matrix_Peripheral_Hefc_Fix = 51, ///< HEFC–HECC Fixable error detected.
	Matrix_Peripheral_Hefc_NoFix =
			52, ///< HEFC–HECC Unfixable error detected.
	Matrix_Peripheral_Tc6 = 53, ///< Timer Counter Channel 6.
	Matrix_Peripheral_Tc7 = 54, ///< Timer Counter Channel 7.
	Matrix_Peripheral_Tc8 = 55, ///< Timer Counter Channel 8.
	Matrix_Peripheral_Tc9 = 56, ///< Timer Counter Channel 9.
	Matrix_Peripheral_Tc10 = 57, ///< Timer Counter Channel 10.
	Matrix_Peripheral_Tc11 = 58, ///< Timer Counter Channel 11.
	Matrix_Peripheral_Hemc = 59, ///< HEMC and HSDRAMC.
	Matrix_Peripheral_Hemc_Fix = 60, ///< HEMC–HECC Fixable error detected.
	Matrix_Peripheral_Hemc_NoFix =
			61, ///< HEMC–HECC Unfixable error detected.
	Matrix_Peripheral_Sfr = 62, ///< Special Function Register.
	Matrix_Peripheral_Trng = 63, ///< True Random Generator.
	Matrix_Peripheral_Xdmac = 64, ///< Extended DMA Controller.
	Matrix_Peripheral_Spw = 65, ///< SpaceWire.
	Matrix_Peripheral_Spw_Timetick = 66, ///< SpaceWire TimeTick.
	Matrix_Peripheral_Ip1553 = 68, ///< MIL-STD-1553.
	Matrix_Peripheral_Gmac = 69, ///< Ethernet MAC.
	Matrix_Peripheral_Gmac_Queue1 = 70, ///< GMAC Priority Queue 1.
	Matrix_Peripheral_Gmac_Queue2 = 71, ///< GMAC Priority Queue 2.
	Matrix_Peripheral_Gmac_Queue3 = 72, ///< GMAC Priority Queue 3.
	Matrix_Peripheral_Gmac_Queue4 = 73, ///< GMAC Priority Queue 4.
	Matrix_Peripheral_Gmac_Queue5 = 74, ///< GMAC Priority Queue 5.
	Matrix_Peripheral_Count = 80, ///< Peripheral count.
#elif defined(N7S_TARGET_SAMRH707F18)
	Matrix_Peripheral_ReportChipId = 0, ///< ChipId Register.
	Matrix_Peripheral_Sysc = 1, ///< System Controller.
	Matrix_Peripheral_TcmEcc = 2, ///< TCM RAM-HECC.
	Matrix_Peripheral_FlexramEcc = 3, ///< FlexRAM–HECC.
	Matrix_Peripheral_Matrix0 = 32, ///< Matrix 0.
	Matrix_Peripheral_Hefc = 33, ///< HEFC.
	Matrix_Peripheral_PioA = 34, ///< Parallel I/O Controller A.
	Matrix_Peripheral_Pmc = 35, ///< Power Management Controller.
	Matrix_Peripheral_Adc = 36, ///< Analog to Digital Controller.
	Matrix_Peripheral_Dacc = 37, ///< Digital to Analog Converter Controller
	Matrix_Peripheral_Crccu = 38, ///< CRC Compute Unit.
	Matrix_Peripheral_Pcc = 39, ///< Parallel Capture Controller
	Matrix_Peripheral_Flexcom0 = 40, ///< FLEXCOM 0.
	Matrix_Peripheral_Flexcom1 = 41, ///< FLEXCOM 1.
	Matrix_Peripheral_Flexcom2 = 42, ///< FLEXCOM 2.
	Matrix_Peripheral_Flexcom3 = 43, ///< FLEXCOM 3.
	Matrix_Peripheral_Ip1553 = 47, ///< MIL-STD-1553.
	Matrix_Peripheral_SpwTx = 48, ///< SpaceWire Tx.
	Matrix_Peripheral_SpwRx = 49, ///< SpaceWire Rx.
	Matrix_Peripheral_SpwRmap = 50, ///< SpaceWire Rmap.
	Matrix_Peripheral_Mcan0 = 54, ///< MCAN Controller 0.
	Matrix_Peripheral_Mcan1 = 55, ///< MCAN Controller 1.
	Matrix_Peripheral_Pwm0 = 58, ///< Pulse Width Modulation 0.
	Matrix_Peripheral_Pwm1 = 59, ///< Pulse Width Modulation 1.
	Matrix_Peripheral_Tc0 = 60, ///< Timer Counter Channel 0.
	Matrix_Peripheral_Tc1 = 61, ///< Timer Counter Channel 1.
	Matrix_Peripheral_Tc2 = 62, ///< Timer Counter Channel 2.
	Matrix_Peripheral_Hemc = 64, ///< HEMC.
	Matrix_Peripheral_Hsmc = 65, ///< HSMC.
	Matrix_Peripheral_Icm = 67, ///< Integrity Check Monitor.
	Matrix_Peripheral_Trng = 68, ///< True Random Generator.
	Matrix_Peripheral_Sha = 69, ///< Secure Hash Algorithm.
	Matrix_Peripheral_Xdmac = 70, ///< Extended DMA Controller.
	Matrix_Peripheral_Sfr = 72, ///< Special Function Register.
	Matrix_Peripheral_Nmic = 74, ///< NMI Controller.
	Matrix_Peripheral_Count = 96, ///< PSER register bits count.
#else
#error "Missing N7S_TARGET_* macro"
#endif
} Matrix_Peripheral;

/// \brief Enumeration listing number of beats before bus re-arbitration.
typedef enum {
	/// \brief No predicted end of burst (enables 1 Kbyte burst lengths).
	/// Could indefinitely freeze the slave arbitration in case when master is capable of
	/// back-to-back undefined length bursts on single slave.
	Matrix_BurstType_Unlimited = 0,
	/// \brief Re-arbitration at each beat of the INCR burst or bursts sequence.
	Matrix_BurstType_Single = 1,
	/// \brief Re-arbitration every 4 beats.
	Matrix_BurstType_4Beat = 2,
	/// \brief Re-arbitration every 8 beats.
	Matrix_BurstType_8Beat = 3,
	/// \brief Re-arbitration every 16 beats.
	Matrix_BurstType_16Beat = 4,
	/// \brief Re-arbitration every 32 beats.
	Matrix_BurstType_32Beat = 5,
	/// \brief Re-arbitration every 64 beats.
	Matrix_BurstType_64Beat = 6,
	/// \brief Re-arbitration every 128 beats.
	Matrix_BurstType_128Beat = 7,
} Matrix_BurstType;

/// \brief Master configuration structure.
typedef struct {
	/// \brief Defines burst type. Unless duly needed, should be left at its default 0 value
	/// for power saving.
	Matrix_BurstType burstType;
	/// \brief Enables remapped address decoding.
	bool isRemappedAddressDecodingEnabled;
	/// \brief Enables error interrupts.
	bool isErrorIrqEnabled;
} Matrix_MasterConfig;

/// \brief Masters' statuses structure.
typedef struct {
	/// \brief Boolean array representing which Master triggered an interrupt.
	bool isMasterIrqTriggered[Matrix_Master_Count];
	/// \brief Master last access error address.
	uint32_t masterErrorAddress[Matrix_Master_Count];
} Matrix_MastersStatuses;

/// \brief Enumeration listing possible default Master allocation to the Slave.
typedef enum {
	Matrix_SlaveDefaultMasterType_None = 0, ///< No Default Master.
	Matrix_SlaveDefaultMasterType_Last = 1, ///< Last Default Master.
	Matrix_SlaveDefaultMasterType_Fixed = 2, ///< Fixed Default Master.
} Matrix_SlaveDefaultMasterType;

/// \brief Enumeration listing possible Master to Slave access priorities. Higher number means
/// higher priority.
typedef enum {
	Matrix_MasterPriority_0 = 0, ///< Background Transfers.
	Matrix_MasterPriority_1 = 1, ///< Bandwidth Sensitive.
	Matrix_MasterPriority_2 = 2, ///< Latency Sensitive.
	Matrix_MasterPriority_3 = 3, ///< Latency Critical.
} Matrix_MasterPriority;

/// \brief Structure representing Master access priority.
typedef struct {
	bool isLatencyQosEnabled; ///< Enables Master's latency QoS to the Slave.
	Matrix_MasterPriority
			masterPriority; ///< Defines fixed priority of Master to Slave access.
} Matrix_AccessPriority;

/// \brief Slave configuration structure.
typedef struct {
	/// \brief Sets default Master access policy for the Slave.
	Matrix_SlaveDefaultMasterType slaveDefaultMasterType;
	/// \brief Specifies Master when Matrix_SlaveDefaultMasterType_Fixed option is used.
	Matrix_Master fixedMasterForSlaveSelection;
	/// \brief AHB Bus cycles before new arbitration to access the Slave takes place.
	///        Maximum valid value is 511. If value is 0 then this feature is disabled and bursts
	///        always complete unless broken according to master's burst type configuration.
	uint16_t maximumBusGrantDurationForMasters;
	/// \brief Priority configuration for each Master when accessing the Slave.
	Matrix_AccessPriority accessPriority[Matrix_Master_Count];
} Matrix_SlaveConfig;

/// \brief Enumeration listing possible Protection Regions.
typedef enum {
	Matrix_ProtectedRegionId_0 = 0, ///< Protection Region ID 0.
	Matrix_ProtectedRegionId_1 = 1, ///< Protection Region ID 1.
	Matrix_ProtectedRegionId_2 = 2, ///< Protection Region ID 2.
	Matrix_ProtectedRegionId_3 = 3, ///< Protection Region ID 3.
	Matrix_ProtectedRegionId_4 = 4, ///< Protection Region ID 4.
	Matrix_ProtectedRegionId_5 = 5, ///< Protection Region ID 5.
	Matrix_ProtectedRegionId_6 = 6, ///< Protection Region ID 6.
	Matrix_ProtectedRegionId_7 = 7, ///< Protection Region ID 7.
	Matrix_ProtectedRegionId_Count =
			8, ///< Number of all possible Protection Regions.
} Matrix_ProtectedRegionId;

/// \brief Enumeration specifying split order of specified Protected Region.
typedef enum {
	/// \brief Sets upper part of Protected Region as user access and bottom as privileged access.
	Matrix_RegionSplitOrder_UpperUserLowerPrivileged = 0,
	/// \brief Sets upper part of Protected Region as privileged access and bottom as user access.
	Matrix_RegionSplitOrder_UpperPrivilegedLowerUser = 1,
} Matrix_RegionSplitOrder;

/// \brief Enumeration specifying split offset/size count direction.
typedef enum {
	/// \brief Sets base address at the bottom of the Slave protected region and counts upwards.
	Matrix_RegionSplitDirection_FromBottomUpwards = 0,
	/// \brief Sets base address at the top of the Slave protected region and counts downwards.
	Matrix_RegionSplitDirection_FromTopDownwards = 1,
} Matrix_RegionSplitDirection;

/// \brief Defines possible sizes and offsets used in Slave Protected Region.
typedef enum {
	Matrix_Size_4KB = 0x0, ///< Size and offset is 4 Kbytes.
	Matrix_Size_8KB = 0x1, ///< Size and offset is 8 Kbytes.
	Matrix_Size_16KB = 0x2, ///< Size and offset is 16 Kbytes.
	Matrix_Size_32KB = 0x3, ///< Size and offset is 32 Kbytes.
	Matrix_Size_64KB = 0x4, ///< Size and offset is 64 Kbytes.
	Matrix_Size_128KB = 0x5, ///< Size and offset is 128 Kbytes.
	Matrix_Size_256KB = 0x6, ///< Size and offset is 256 Kbytes.
	Matrix_Size_512KB = 0x7, ///< Size and offset is 512 Kbytes.
	Matrix_Size_1MB = 0x8, ///< Size and offset is 1 Mbyte.
	Matrix_Size_2MB = 0x9, ///< Size and offset is 2 Mbytes.
	Matrix_Size_4MB = 0xA, ///< Size and offset is 4 Mbytes.
	Matrix_Size_8MB = 0xB, ///< Size and offset is 8 Mbytes.
	Matrix_Size_16MB = 0xC, ///< Size and offset is 16 Mbytes.
	Matrix_Size_32MB = 0xD, ///< Size and offset is 32 Mbytes.
	Matrix_Size_64MB = 0xE, ///< Size and offset is 64 Mbytes.
	Matrix_Size_128MB = 0xF, ///< Size and offset is 128 Mbytes.
} Matrix_Size;

/// \brief Slave Protected Region configuration structure.
typedef struct {
	/// \brief Enables writing in privileged region.
	bool isPrivilegedRegionUserWriteAllowed;
	/// \brief Enables reading in privileged region.
	bool isPrivilegedRegionUserReadAllowed;
	/// \brief Protected Region split offset direction policy.
	Matrix_RegionSplitDirection regionSplitOffsetDirection;
	/// \brief Protected Region size.
	Matrix_Size protectedRegionSize;
	/// \brief Protected Region split offset.
	Matrix_Size regionSplitOffset;
	/// \brief Protected Region split order policy.
	Matrix_RegionSplitOrder regionOrder;
} Matrix_SlaveRegionProtectionConfig;

/// \brief Enumeration specifying access configuration of Peripheral address space.
typedef enum {
	Matrix_AccessType_Privileged = 0, ///< Privileged access.
	Matrix_AccessType_User = 1, ///< User access.
} Matrix_AccessType;

/// \brief Peripheral Protection configuration structure.
typedef struct {
	Matrix_AccessType accessType; ///< Access type.
} Matrix_PeripheralProtectionConfig;

/// \brief Matrix device descriptor.
typedef struct {
	volatile Matrix_Registers
			*regs; ///< Pointer to memory-mapped device registers.
} Matrix;

/// \brief Gets Matrix device base address.
/// \returns Base address of Matrix device.
static inline Matrix_Registers *
Matrix_getDeviceBaseAddress(void)
{
	// cppcheck-suppress misra-c2012-11.4
	return ((Matrix_Registers *)(MATRIX_BASE_ADDRESS));
}

/// \brief Initializes the structure representing a Matrix instance.
/// \param [in,out] matrix Pointer to a structure representing a Matrix instance.
/// \param [in] matrixDeviceAddress Matrix device base address.
void Matrix_init(Matrix *const matrix,
		Matrix_Registers *const matrixDeviceAddress);

/// \brief Sets Master configuration.
/// \param [in,out] matrix Pointer to a structure representing a Matrix instance.
/// \param [in] master Master to configure.
/// \param [in] config Master configuration.
void Matrix_setMasterConfig(Matrix *const matrix, const Matrix_Master master,
		const Matrix_MasterConfig *const config);

/// \brief Sets Slave configuration.
/// \param [in,out] matrix Pointer to a structure representing a Matrix instance.
/// \param [in] slave Slave to configure.
/// \param [in] config Slave configuration.
void Matrix_setSlaveConfig(Matrix *const matrix, const Matrix_Slave slave,
		const Matrix_SlaveConfig *const config);

/// \brief Gets Master configuration.
/// \param [in] matrix Pointer to a structure representing a Matrix instance.
/// \param [in] master Master to read configuration from.
/// \param [out] config Read configuration.
void Matrix_getMasterConfig(const Matrix *const matrix,
		const Matrix_Master master, Matrix_MasterConfig *const config);

/// \brief Gets Slave configuration.
/// \param [in] matrix Pointer to a structure representing a Matrix instance.
/// \param [in] slave Slave to read configuration from.
/// \param [out] config Read configuration.
void Matrix_getSlaveConfig(const Matrix *const matrix, const Matrix_Slave slave,
		Matrix_SlaveConfig *const config);

/// \brief Gets Masters' statuses.
/// \param [in] matrix Pointer to a structure representing a Matrix instance.
/// \param [out] statuses Read statuses.
void Matrix_getMastersStatuses(const Matrix *const matrix,
		Matrix_MastersStatuses *const statuses);

/// \brief Sets Matrix registers write protection.
/// \param [in,out] matrix Pointer to a structure representing a Matrix instance.
/// \param [in] enabled Enables or disables the write protection.
void Matrix_setWriteProtectionEnabled(Matrix *const matrix, const bool enabled);

/// \brief Checks if Matrix registers write protection is enabled.
/// \param [in] matrix Pointer to a structure representing a Matrix instance.
/// \retval true Register write protection is enabled.
/// \retval false Register write protection is disabled.
bool Matrix_isWriteProtectionEnabled(const Matrix *const matrix);

/// \brief Checks if Matrix registers write protection was violated.
/// \param [in] matrix Pointer to a structure representing a Matrix instance.
/// \param [out] offset Address offset of register where violation happened.
/// \retval true Register write protection was violated.
/// \retval false Register write protection was not violated.
bool Matrix_isWriteProtectionViolated(
		const Matrix *const matrix, uint16_t *const offset);

/// \brief Sets AHB Slave Protected Region configuration.
/// \param [in,out] matrix Pointer to a structure representing a Matrix instance.
/// \param [in] slave Slave ID.
/// \param [in] regionId Protected Region ID to configure.
/// \param [in] config Protected Region configuration structure.
void Matrix_setSlaveRegionProtectionConfig(Matrix *const matrix,
		const Matrix_Slave slave,
		const Matrix_ProtectedRegionId regionId,
		const Matrix_SlaveRegionProtectionConfig *const config);

/// \brief Gets AHB Slave Protected Region configuration.
/// \param [in] matrix Pointer to a structure representing a Matrix instance.
/// \param [in] slave Slave ID.
/// \param [in] regionId Protected Region ID of the region which a configuration is read.
/// \param [out] config Protected Region configuration structure.
void Matrix_getSlaveRegionProtectionConfig(const Matrix *const matrix,
		const Matrix_Slave slave,
		const Matrix_ProtectedRegionId regionId,
		Matrix_SlaveRegionProtectionConfig *const config);

/// \brief Sets APB Peripheral address space protection configuration.
/// \param [in,out] matrix Pointer to a structure representing a Matrix instance.
/// \param [in] peripheral Peripheral ID.
/// \param [in] config Peripheral Protection configuration structure.
void Matrix_setPeripheralProtectionConfig(Matrix *const matrix,
		const Matrix_Peripheral peripheral,
		const Matrix_PeripheralProtectionConfig *const config);

/// \brief Gets APB Peripheral address space protection configuration.
/// \param [in] matrix Pointer to a structure representing a Matrix instance.
/// \param [in] peripheral Peripheral ID.
/// \param [out] config Peripheral Protection configuration structure.
void Matrix_getPeripheralProtectionConfig(const Matrix *const matrix,
		const Matrix_Peripheral peripheral,
		Matrix_PeripheralProtectionConfig *const config);

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // BSP_MATRIX_H
