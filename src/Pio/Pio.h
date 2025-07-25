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

#ifndef BSP_PIO_H
#define BSP_PIO_H

#include <stdbool.h>

#include <Utils/ErrorCode.h>

#include "PioRegisters.h"

/// @addtogroup Pio
/// @ingroup Bsp
/// @{

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Mask indicating pin 0.
#define PIO_PIN_0 0x00000001u
/// \brief Mask indicating pin 1.
#define PIO_PIN_1 0x00000002u
/// \brief Mask indicating pin 2.
#define PIO_PIN_2 0x00000004u
/// \brief Mask indicating pin 3.
#define PIO_PIN_3 0x00000008u
/// \brief Mask indicating pin 4.
#define PIO_PIN_4 0x00000010u
/// \brief Mask indicating pin 5.
#define PIO_PIN_5 0x00000020u
/// \brief Mask indicating pin 6.
#define PIO_PIN_6 0x00000040u
/// \brief Mask indicating pin 7.
#define PIO_PIN_7 0x00000080u
/// \brief Mask indicating pin 8.
#define PIO_PIN_8 0x00000100u
/// \brief Mask indicating pin 9.
#define PIO_PIN_9 0x00000200u
/// \brief Mask indicating pin 10.
#define PIO_PIN_10 0x00000400u
/// \brief Mask indicating pin 11.
#define PIO_PIN_11 0x00000800u
/// \brief Mask indicating pin 12.
#define PIO_PIN_12 0x00001000u
/// \brief Mask indicating pin 13.
#define PIO_PIN_13 0x00002000u
/// \brief Mask indicating pin 14.
#define PIO_PIN_14 0x00004000u
/// \brief Mask indicating pin 15.
#define PIO_PIN_15 0x00008000u
/// \brief Mask indicating pin 16.
#define PIO_PIN_16 0x00010000u
/// \brief Mask indicating pin 17.
#define PIO_PIN_17 0x00020000u
/// \brief Mask indicating pin 18.
#define PIO_PIN_18 0x00040000u
/// \brief Mask indicating pin 19.
#define PIO_PIN_19 0x00080000u
/// \brief Mask indicating pin 20.
#define PIO_PIN_20 0x00100000u
/// \brief Mask indicating pin 21.
#define PIO_PIN_21 0x00200000u
/// \brief Mask indicating pin 22.
#define PIO_PIN_22 0x00400000u
/// \brief Mask indicating pin 23.
#define PIO_PIN_23 0x00800000u
/// \brief Mask indicating pin 24.
#define PIO_PIN_24 0x01000000u
/// \brief Mask indicating pin 25.
#define PIO_PIN_25 0x02000000u
/// \brief Mask indicating pin 26.
#define PIO_PIN_26 0x04000000u
/// \brief Mask indicating pin 27.
#define PIO_PIN_27 0x08000000u
/// \brief Mask indicating pin 28.
#define PIO_PIN_28 0x10000000u
/// \brief Mask indicating pin 29.
#define PIO_PIN_29 0x20000000u
/// \brief Mask indicating pin 30.
#define PIO_PIN_30 0x40000000u
/// \brief Mask indicating pin 31.
#define PIO_PIN_31 0x80000000u

/// \brief Enumeration listing possible Pio error codes.
typedef enum {
	/// \brief Pio control configuration does not match for the selected mask.
	Pio_ErrorCode_ControlConfigMismatch =
			ERROR_CODE_DEFINE('P', 'I', 'O', 1),
	/// \brief Pio direction configuration does not match for the selected mask.
	Pio_ErrorCode_DirectionConfigMismatch =
			ERROR_CODE_DEFINE('P', 'I', 'O', 2),
	/// \brief Pio pull-down/pull-up configuration does not match for the selected mask.
	Pio_ErrorCode_PullConfigMismatch = ERROR_CODE_DEFINE('P', 'I', 'O', 3),
	/// \brief Pio filter configuration does not match for the selected mask.
	Pio_ErrorCode_FilterConfigMismatch =
			ERROR_CODE_DEFINE('P', 'I', 'O', 4),
	/// \brief Pio interrupt configuration does not match for the selected mask.
	Pio_ErrorCode_IrqConfigMismatch = ERROR_CODE_DEFINE('P', 'I', 'O', 5),
	/// \brief Pio multi-drive configuration does not match for the selected mask.
	Pio_ErrorCode_MultiDriveConfigMismatch =
			ERROR_CODE_DEFINE('P', 'I', 'O', 6),
	/// \brief Pio Schmitt trigger configuration does not match for the selected mask.
	Pio_ErrorCode_SchmittTriggerConfigMismatch =
			ERROR_CODE_DEFINE('P', 'I', 'O', 7),
	/// \brief Pio Drive strength configuration does not match for the selected mask.
	Pio_ErrorCode_DriveStrengthConfigMismatch =
			ERROR_CODE_DEFINE('P', 'I', 'O', 8),
	/// \brief Invalid Port ID passed to Pio_Init.
	Pio_ErrorCode_InvalidPortId = ERROR_CODE_DEFINE('P', 'I', 'O', 9),
	/// \brief Invalid Pio I/O line assignment to peripherals requested.
	Pio_ErrorCode_InvalidControlConfig =
			ERROR_CODE_DEFINE('P', 'I', 'O', 10),
	/// \brief Invalid direction configuration requested.
	Pio_ErrorCode_InvalidDirectionConfig =
			ERROR_CODE_DEFINE('P', 'I', 'O', 11),
	/// \brief invalid pull-down/pull-up configuration requested.
	Pio_ErrorCode_InvalidPullConfig = ERROR_CODE_DEFINE('P', 'I', 'O', 12),
	/// \brief Invalid filter configuration requested.
	Pio_ErrorCode_InvalidFilterConfig =
			ERROR_CODE_DEFINE('P', 'I', 'O', 13),
	/// \brief Invalid interrupt configuration requested.
	Pio_ErrorCode_InvalidIrqConfig = ERROR_CODE_DEFINE('P', 'I', 'O', 14),
	/// \brief Invalid Drive strength configuration requested.
	Pio_ErrorCode_InvalidDriveStrengthConfig =
			ERROR_CODE_DEFINE('P', 'I', 'O', 15),
	/// \brief Pio Invalid pin mask.
	Pio_ErrorCode_InvalidPinMask = ERROR_CODE_DEFINE('P', 'I', 'O', 16),
} Pio_ErrorCode;

/// \brief Enumeration listing I/O ports.
typedef enum {
	Pio_Port_A = 0, ///< I/O line port A.
	Pio_Port_B = 1, ///< I/O line port B.
	Pio_Port_C = 2, ///< I/O line port C.
	Pio_Port_D = 3, ///< I/O line port D.
	Pio_Port_E = 4, ///< I/O line port E.
} Pio_Port;

/// \brief Possible I/O line assignment to peripherals.
typedef enum {
	Pio_Control_Pio, ///< General purpose I/O mode.
	Pio_Control_PeripheralA, ///< Peripheral A mode.
	Pio_Control_PeripheralB, ///< Peripheral B mode.
	Pio_Control_PeripheralC, ///< Peripheral C mode.
	Pio_Control_PeripheralD, ///< Peripheral D mode.
} Pio_Control;

/// \brief Possible I/O line flow direction.
typedef enum {
	Pio_Direction_Input, ///< Input mode.
	Pio_Direction_Output, ///< Output mode.
	Pio_Direction_SynchronousOutput, ///< Synchronous output.
} Pio_Direction;

/// \brief I/O line pull resistors.
typedef enum {
	Pio_Pull_None, ///< Pull disabled.
	Pio_Pull_Up, ///< Pull up enabled.
	Pio_Pull_Down, ///< Pull down enabled.
} Pio_Pull;

/// \brief I/O line filtering options.
typedef enum {
	Pio_Filter_None, ///< Filtering disabled.
	Pio_Filter_Glitch, ///< Clock glitch filter.
	Pio_Filter_Debounce, ///< Debounce filter.
} Pio_Filter;

/// brief I/O line drive strength
typedef enum {
	Pio_Drive_Low = 0, ///< Drive strength -4mA/6mA
	Pio_Drive_High = 1, ///< Drive strength -10mA/12mA
} Pio_Drive;

/// \brief I/O line IRQ modes.
typedef enum {
	Pio_Irq_None, ///< No interrupt condition.
	Pio_Irq_EdgeBoth, ///< Interrupt enabled on rising and falling edges.
	Pio_Irq_EdgeRising, ///< Interrupt set to rising edge.
	Pio_Irq_EdgeFalling, ///< Interrupt set to falling edge.
	Pio_Irq_LevelLow, ///< Interrupt enabled on low level.
	Pio_Irq_LevelHigh, ///< Interrupt enabled on high level.
} Pio_Irq;

/// \brief I/O line configuration structure.
typedef struct {
	Pio_Control control; ///< I/O line peripheral mode.
	Pio_Direction direction; ///< I/O line flow direction.
	Pio_Pull pull; ///< I/O line pull mode.
	Pio_Filter filter; ///< I/O line filtering mode.
	bool isMultiDriveEnabled; ///< Enables open drain mode.
	Pio_Irq irq; ///< Interrupt event configuration.
	Pio_Drive driveStrength; ///< I/O drive strength
	bool isSchmittTriggerDisabled; ///< Disables Schmitt trigger.
} Pio_Pin_Config;

/// \brief I/O line set configuration structure.
typedef struct {
	uint32_t pins; ///< I/O lines affected by config.
	Pio_Pin_Config pinsConfig; ///< I/O line configuration.
	uint16_t debounceFilterDiv; ///< Slow Clock Divider Debouncing.
} Pio_Port_Config;

/// \brief Pio descriptor.
typedef struct {
	Pio_Port port; ///< I/O port.
	Pio_Registers *reg; ///< I/O port registers.
} Pio;

/// \brief Initializes Pio descriptor.
/// \param [in,out] pio Pio descriptor.
/// \param [in] port I/O line port.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Pio was initialized successfully, returned descriptor is valid.
/// \retval false Pio was not initialized successfully, returned descriptor is invalid.
bool Pio_init(const Pio_Port port, Pio *const pio, ErrorCode *const errCode);

/// \brief Sets configuration for specified I/O line set.
/// \param [in] pio Pio descriptor.
/// \param [in] config I/O line set configuration structure.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Configuration for specified I/O lines was applied successfully.
/// \retval false Configuration for specified I/O lines is invalid.
bool Pio_setPortConfig(Pio *const pio, const Pio_Port_Config *const config,
		ErrorCode *const errCode);

/// \brief Gets applied configuration from specified I/O line set.
/// \param [in] pio Pio descriptor.
/// \param [in,out] config I/O line set configuration structure.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Configuration for specified I/O lines was equal and was returned successfully.
/// \retval false Configuration for specified I/O lines differed. Returned configuration is not
/// valid.
bool Pio_getPortConfig(const Pio *const pio, Pio_Port_Config *const config,
		ErrorCode *const errCode);

/// \brief Sets configuration for specified I/O lines.
/// \param [in] pio Pio descriptor.
/// \param [in] pinMask I/O line set.
/// \param [in] config I/O line configuration structure.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Configuration for specified I/O lines was applied successfully.
/// \retval false Configuration for specified I/O lines is invalid.
bool Pio_setPinsConfig(Pio *const pio, const uint32_t pinMask,
		const Pio_Pin_Config *const config, ErrorCode *const errCode);

/// \brief Gets applied configuration from specified I/O line set.
/// \param [in] pio Pio descriptor.
/// \param [in] pinMask I/O line set.
/// \param [out] config I/O line configuration structure.
/// \param [out] errCode An error code generated during the operation.
/// \retval true Configuration for specified I/O lines was equal and was returned successfully.
/// \retval false Configuration for specified I/O lines differed. Returned configuration is not
/// valid.
bool Pio_getPinsConfig(const Pio *const pio, const uint32_t pinMask,
		Pio_Pin_Config *const config, ErrorCode *const errCode);

/// \brief Sets the data to be driven on the I/O line.
/// \param [in] pio Pio descriptor.
/// \param [in] pinMask I/O line set.
void Pio_setPins(Pio *const pio, const uint32_t pinMask);

/// \brief Clears the data to be driven on the I/O line.
/// \param [in] pio Pio descriptor.
/// \param [in] pinMask I/O line set.
void Pio_resetPins(Pio *const pio, const uint32_t pinMask);

/// \brief Gets the data from I/O port.
/// \param [in] pio Pio descriptor.
/// \returns I/O port's lines status
uint32_t Pio_getPins(const Pio *const pio);

/// \brief Sets the data to be driven on the I/O port. Works on pins with Synchronous Output.
/// \param [in] pio Pio descriptor.
/// \param [in] value Data to be driven on the I/O port.
void Pio_setPortValue(Pio *const pio, const uint32_t value);

/// \brief Returns triggered IRQ of the I/O port.
/// \param [in] pio Pio descriptor.
/// \returns I/O port's lines IRQ status.
uint32_t Pio_getIrqStatus(const Pio *const pio);

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // BSP_PIO_H
