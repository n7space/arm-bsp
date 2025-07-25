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

/// \file Tic.h
/// \addtogroup Bsp
/// \brief Header for the Timer Controller - TC (TIC) driver.

#ifndef BSP_TIC_H
#define BSP_TIC_H

#include <stdbool.h>
#include <stdint.h>

#include "TicRegisters.h"

/// @addtogroup Tic
/// @ingroup Bsp
/// @{

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Enumeration listing Tic instances.
typedef enum {
	Tic_Id_0 = 0, ///< TIC instance 0.
	Tic_Id_1 = 1, ///< TIC instance 1.
	Tic_Id_2 = 2, ///< TIC instance 2.
#if defined(N7S_TARGET_SAMV71Q21) || defined(N7S_TARGET_SAMRH71F20)
	Tic_Id_3 = 3, ///< TIC instance 3.
#endif
} Tic_Id;

/// \brief Enumeration listing Tic channels.
typedef enum {
	Tic_Channel_0 = 0, ///< Tic channel 0.
	Tic_Channel_1 = 1, ///< Tic channel 1.
	Tic_Channel_2 = 2, ///< Tic channel 2.
	Tic_Channel_Count = 3, ///< Tic channel count.
} Tic_Channel;

/// \brief Enumeration listing Tic clock sources.
typedef enum {
#if defined(N7S_TARGET_SAMV71Q21)
	Tic_ClockSelection_Pck6 = 0, ///< Pck6.
#elif defined(N7S_TARGET_SAMRH71F20) || defined(N7S_TARGET_SAMRH707F18)
	Tic_ClockSelection_Gclk = 0, ///< Gclk.
#else
#error "No target platform specified (missing N7S_TARGET_* macro)"
#endif
	Tic_ClockSelection_MckBy8 = 1, ///< Mck / 8.
	Tic_ClockSelection_MckBy32 = 2, ///< Mck / 32.
	Tic_ClockSelection_MckBy128 = 3, ///< Mck / 128.
	Tic_ClockSelection_Slck = 4, ///< Slck.
	Tic_ClockSelection_Xc0 = 5, ///< External clock 0.
	Tic_ClockSelection_Xc1 = 6, ///< External clock 1.
	Tic_ClockSelection_Xc2 = 7, ///< External clock 2.
} Tic_ClockSelection;

/// \brief Enumeration listing possible burst selection values.
typedef enum {
	Tic_BurstSelection_None = 0, ///< None.
	Tic_BurstSelection_Xc0 = 1, ///< External clock 0.
	Tic_BurstSelection_Xc1 = 2, ///< External clock 1.
	Tic_BurstSelection_Xc2 = 3, ///< External clock 2.
} Tic_BurstSelection;

/// \brief Enumeration listing possible edge selection values.
typedef enum {
	Tic_EdgeSelection_None = 0, ///< None.
	Tic_EdgeSelection_Rising = 1, ///< Rising edge.
	Tic_EdgeSelection_Falling = 2, ///< Falling edge.
	Tic_EdgeSelection_Both = 3, ///< Both edges.
} Tic_EdgeSelection;

/// \brief Enumeration listing possible signal trigger selection values.
typedef enum {
	Tic_SignalTriggerSelection_Tiob = 0, ///< Trigger selection TIOBx
	Tic_SignalTriggerSelection_Tioa = 1, ///< Trigger selection TIOAx
} Tic_SignalTriggerSelection;

/// \brief Enumeration listing subsampling ratios.
typedef enum {
	Tic_EdgeSubsampling_One =
			0, ///< Load a Capture register each selected edge.
	Tic_EdgeSubsampling_Half =
			1, ///< Load a Capture register every 2 selected edges.
	Tic_EdgeSubsampling_Fourth =
			2, ///< Load a Capture register every 4 selected edges.
	Tic_EdgeSubsampling_Eighth =
			3, ///< Load a Capture register every 8 selected edges.
	Tic_EdgeSubsampling_Sixteenth =
			4, ///< Load a Capture register every 16 selected edges.
} Tic_EdgeSubsampling;

/// \brief Enumeration listing possible external event selection values.
typedef enum {
	Tic_ExternalEventSelection_Tiob = 0, ///<  TIOBx.
	Tic_ExternalEventSelection_Xc0 = 1, ///< External clock 0.
	Tic_ExternalEventSelection_Xc1 = 2, ///< External clock 1.
	Tic_ExternalEventSelection_Xc2 = 3, ///< External clock 2.
} Tic_ExternalEventSelection;

/// \brief Enumeration listing Waveform modes.
typedef enum {
	Tic_WaveformMode_Up = 0, ///< Up.
	Tic_WaveformMode_UpDown = 1, ///< Up and Down.
	Tic_WaveformMode_Up_Rc =
			2, ///< Up with automatic trigger on RC compare.
	Tic_WaveformMode_UpDown_Rc =
			3, ///< Up and Down with automatic trigger on RC compare.
} Tic_WaveformMode;

/// \brief Enumeration listing possible compare/trigger effects on TIOAx/TIOBx.
typedef enum {
	Tic_TioEffect_None = 0, ///< None.
	Tic_TioEffect_Set = 1, ///< Set.
	Tic_TioEffect_Clear = 2, ///< Clear.
	Tic_TioEffect_Toggle = 3, ///< Toggle.
} Tic_TioEffect;

/// \brief Enumeration listing possible Tic modes.
typedef enum {
	Tic_Mode_Capture = 0, ///< Capture mode.
	Tic_Mode_Waveform = 1, ///< Waveform mode.
} Tic_Mode;

/// \brief Structure describing Capture mode configuration.
typedef struct {
	bool isStoppedOnRbLoading; ///< Is stopped on RB loading.
	bool isDisabledOnRbLoading; ///< Is disabled on RB loading.
	Tic_EdgeSelection
			externalTriggerEdge; ///< External trigger edge selection.
	Tic_SignalTriggerSelection
			triggerSource; ///< External trigger source selection.
	bool isTriggeredByRcCompare; ///< Is triggered by RC compare.
	Tic_EdgeSelection
			raLoadingEdgeSelection; ///< RA loading edge selection.
	Tic_EdgeSelection
			rbLoadingEdgeSelection; ///< RB loading edge selection.
	Tic_EdgeSubsampling
			loadingEdgeSubsampling; ///< Loading edge subsampling.
} Tic_CaptureModeConfig;

/// \brief Structure describing Wave mode configuration.
typedef struct {
	bool isStoppedOnRcCompare; ///< Is stopped on RC compare.
	bool isDisabledOnRcCompare; ///< Is disabled on RC compare.
	Tic_EdgeSelection externalEvent; ///< External event edge selection.
	Tic_ExternalEventSelection
			externalEventSource; ///< External event source selection.
	bool isTriggeredByExternalEvent; ///< Is triggered by an external event.
	Tic_WaveformMode waveformMode; ///< Waveform mode.
	Tic_TioEffect raCompareEffectOnTioa; ///< RA compare effect on TIOA.
	Tic_TioEffect rcCompareEffectOnTioa; ///< RC compare effect on TIOA.
	Tic_TioEffect externalEventEffectOnTioa; ///< External event effect on TIOA.
	Tic_TioEffect triggerEffectOnTioa; ///< Trigger effect on TIOA.
	Tic_TioEffect rbCompareEffectOnTiob; ///< RB compare effect on TIOB.
	Tic_TioEffect rcCompareEffectOnTiob; ///< RC compare effect on TIOB.
	Tic_TioEffect externalEventEffectOnTiob; ///< External event effect on TIOB.
	Tic_TioEffect triggerEffectOnTiob; ///< Trigger effect on TIOB.
	uint32_t ra; ///< RA value for comparisons.
	uint32_t rb; ///< RB value for comparisons.
} Tic_WaveformModeConfig;

/// \brief Enumeration listing possible interrupt sources.
typedef enum {
	Tic_Irq_CounterOverflow = TIC_IER_COVFS_OFFSET, ///< Counter overflow.
	Tic_Irq_LoadOverrun = TIC_IER_LOVRS_OFFSET, ///< Load overrun.
	Tic_Irq_RaCompare = TIC_IER_CPAS_OFFSET, ///< RA compare.
	Tic_Irq_RbCompare = TIC_IER_CPBS_OFFSET, ///< RB compare.
	Tic_Irq_RcCompare = TIC_IER_CPCS_OFFSET, ///< RC compare.
	Tic_Irq_RaLoading = TIC_IER_LDRAS_OFFSET, ///< RA loading.
	Tic_Irq_RbLoading = TIC_IER_LDRBS_OFFSET, ///< RB loading.
	Tic_Irq_ExternalTrigger = TIC_IER_ETRGS_OFFSET, ///< External trigger.
} Tic_Irq;

/// \brief Structure describing channel interrupt configuration.
typedef struct {
	bool isCounterOverflowIrqEnabled; ///< Is counter overflow interrupt enabled.
	bool isLoadOverrunIrqEnabled; ///< Is load overrun interrupt enabled.
	bool isRaCompareIrqEnabled; ///< Is RA compare interrupt enabled.
	bool isRbCompareIrqEnabled; ///< Is RB compare interrupt enabled.
	bool isRcCompareIrqEnabled; ///< Is RC compare interrupt enabled.
	bool isRaLoadingIrqEnabled; ///< Is RA loading interrupt enabled.
	bool isRbLoadingIrqEnabled; ///< Is RB loading interrupt enabled.
	bool isExternalTriggerIrqEnabled; ///< Is external trigger interrupt enabled.
} Tic_ChannelIrqConfig;

/// \brief Enumeration listing possible trigger sources.
typedef enum {
	Tic_TriggerSourceForInput_ExternalTio = 0, ///< External TIOAx/TIOBx.
	Tic_TriggerSourceForInput_Pwm = 1, ///< PWM.
} Tic_TriggerSourceForInput;

/// \brief Enumeration listing possible external clock 0 signal selections.
typedef enum {
	Tic_ExternalClock0SignalSelection_Tclk0 = 0, ///< Tclk0.
	Tic_ExternalClock0SignalSelection_Tioa1 = 2, ///< TIOA1.
	Tic_ExternalClock0SignalSelection_Tioa2 = 3, ///< TIOA2.
} Tic_ExternalClock0SignalSelection;

/// \brief Enumeration listing possible external clock 1 signal selections.
typedef enum {
	Tic_ExternalClock1SignalSelection_Tclk1 = 0, ///< Tclk1.
	Tic_ExternalClock1SignalSelection_Tioa0 = 2, ///< TIOA0.
	Tic_ExternalClock1SignalSelection_Tioa2 = 3, ///< TIOA2.
} Tic_ExternalClock1SignalSelection;

/// \brief Enumeration listing possible external clock 2 signal selections.
typedef enum {
	Tic_ExternalClock2SignalSelection_Tclk2 = 0, ///< Tclk2.
	Tic_ExternalClock2SignalSelection_Tioa0 = 2, ///< TIOA0.
	Tic_ExternalClock2SignalSelection_Tioa1 = 3, ///< TIOA1.
} Tic_ExternalClock2SignalSelection;

/// \brief Structure describing external clock selections.
typedef struct {
	Tic_ExternalClock0SignalSelection
			xc0; ///< External clock signal selection for XC0.
	Tic_ExternalClock1SignalSelection
			xc1; ///< External clock signal selection for XC1.
	Tic_ExternalClock2SignalSelection
			xc2; ///< External clock signal selection for XC2.
} Tic_ExternalClockSignalSelection;

/// \brief Structure describing Tic channel configuration.
typedef struct {
	bool isEnabled; ///< Is channel enabled.
	Tic_ClockSelection clockSource; ///< Clock source selection.
	bool isClockInverted; ///< Is clock inverted.
	Tic_BurstSelection burst; ///< Burst selection.
	Tic_Mode channelMode; ///< Channel mode.

	struct {
		Tic_CaptureModeConfig
				captureModeConfig; ///< Capture mode config.
		Tic_WaveformModeConfig
				waveformModeConfig; ///< Waveform mode config.
	} modeConfig; ///< Mode configurations.

	bool isGrayCounterEnabled; ///< Is gray counter enabled.
	bool doesGrayCounterCountDown; ///< Does gray counter count down.
	Tic_ChannelIrqConfig irqConfig; ///< Interrupt configuration.
	Tic_TriggerSourceForInput
			triggerSourceForInputA; ///< Trigger source for input A.
	Tic_TriggerSourceForInput
			triggerSourceForInputB; ///< Trigger source for input B.
	bool useUndividedPck; ///< Use undivided Pck.
	uint32_t rc; ///< RC value for comparisons.
} Tic_ChannelConfig;

/// \brief Structure describing Tic channel status.
typedef struct {
	bool hasCounterOverflowed; ///< Has counter overflow occurred.
	bool hasLoadOverrunOccurred; ///< Has load overrun occurred.
	bool hasRaCompareOccurred; ///< Has RA compare occurred.
	bool hasRbCompareOccurred; ///< Has RB compare occurred.
	bool hasRcCompareOccurred; ///< Has RC compare occurred.
	bool hasRaLoadOccurred; ///< Has RA load occurred.
	bool hasRbLoadOccurred; ///< Has RB load occurred.
	bool hasExternalTriggerOccurred; ///< Has external trigger occurred.
	bool isChannelEnabled; ///< Is channel enabled.
	bool isTioaAsserted; ///< Is TIOA asserted.
	bool isTiobAsserted; ///< Is TIOB asserted.
} Tic_ChannelStatus;

/// \brief Structure describing Tic.
typedef struct {
	/// \brief Tic instance ID.
	Tic_Id ticId;
	/// \brief Pointer to Tic registers.
	volatile Tic_Registers *regs;
} Tic;

/// \brief Initializes Tic instance.
/// \param [in,out] tic Pointer to Tic instance.
/// \param [in] id Instance ID.
void Tic_init(Tic *const tic, const Tic_Id id);

/// \brief Enable channel.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to be enabled.
void Tic_enableChannel(Tic *const tic, const Tic_Channel channel);

/// \brief Disable channel.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to be disabled.
void Tic_disableChannel(Tic *const tic, const Tic_Channel channel);

/// \brief Is channel enabled.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to be queried.
/// \returns Whether the channel is enabled.
bool Tic_isChannelEnabled(const Tic *const tic, const Tic_Channel channel);

/// \brief Trigger channel.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to be triggered.
void Tic_triggerChannel(Tic *const tic, const Tic_Channel channel);

/// \brief Synchronize all channels.
/// \param [in] tic Pointer to Tic instance.
void Tic_syncAllChannels(Tic *const tic);

/// \brief Set channel configuration.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to be configured.
/// \param [in] config Channel configuration.
void Tic_setChannelConfig(Tic *const tic, const Tic_Channel channel,
		const Tic_ChannelConfig *const config);

/// \brief Get channel configuration.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to be queried.
/// \param [out] config Channel configuration.
void Tic_getChannelConfig(const Tic *const tic, const Tic_Channel channel,
		Tic_ChannelConfig *const config);

/// \brief Set channel interrupt configuration.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to be configured.
/// \param [in] config Channel interrupt configuration.
void Tic_setChannelIrqConfig(Tic *const tic, const Tic_Channel channel,
		const Tic_ChannelIrqConfig *const config);

/// \brief Get channel interrupt configuration.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to queried.
/// \param [out] config Channel interrupt configuration.
void Tic_getChannelIrqConfig(const Tic *const tic, const Tic_Channel channel,
		Tic_ChannelIrqConfig *const config);

/// \brief Enable channel interrupt.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to be configured.
/// \param [in] irq Interrupt to be enabled.
void Tic_enableChannelIrq(
		Tic *const tic, const Tic_Channel channel, const Tic_Irq irq);

/// \brief Disable channel interrupt.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to be configured.
/// \param [in] irq Interrupt to be disabled.
void Tic_disableChannelIrq(
		Tic *const tic, const Tic_Channel channel, const Tic_Irq irq);

/// \brief Is channel interrupt enabled.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to be queried.
/// \param [in] irq Interrupt to be queried.
/// \returns Whether the given interrupt is enabled.
bool Tic_isChannelIrqEnabled(const Tic *const tic, const Tic_Channel channel,
		const Tic_Irq irq);

/// \brief Get channel status.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to be queried.
/// \param [out] status Channel status.
void Tic_getChannelStatus(const Tic *const tic, const Tic_Channel channel,
		Tic_ChannelStatus *const status);

/// \brief Get counter value.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to be queried.
/// \returns Counter value.
uint32_t Tic_getCounterValue(const Tic *const tic, const Tic_Channel channel);

/// \brief Set external clock signals configuration.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] externalClockSignals External clock signals configuration.
void Tic_configureExternalClockSignals(Tic *const tic,
		const Tic_ExternalClockSignalSelection
				*const externalClockSignals);

/// \brief Set Tic write protection.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] protect Whether Tic is to be write protected.
void Tic_writeProtect(Tic *const tic, bool protect);

/// \brief Gets RA value for the given channel (intended for use in capture mode).
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to be queried.
/// \returns The current RA value.
uint32_t Tic_getRaValue(const Tic *const tic, const Tic_Channel channel);

/// \brief Gets RB value for the given channel (intended for use in capture mode).
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to be queried.
/// \returns The current RB value.
uint32_t Tic_getRbValue(const Tic *const tic, const Tic_Channel channel);

/// \brief Gets RC value for the given channel.
/// \param [in] tic Pointer to Tic instance.
/// \param [in] channel Channel to be queried.
/// \returns The current RC value.
uint32_t Tic_getRcValue(const Tic *const tic, const Tic_Channel channel);

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // BSP_TIC_H
