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

#include "Tic.h"

#include <assert.h>
#include <string.h>

#include <Utils/Bits.h>
#include <Utils/Utils.h>

#include "TicRegisters.h"

void
Tic_init(Tic *const tic, const Tic_Id id)
{
	(void)memset(tic, 0, sizeof(Tic));

	switch (id) {
	case Tic_Id_0:
		// cppcheck-suppress misra-c2012-11.4
		tic->regs = (volatile Tic_Registers *)TIC_TIC0_BASE_ADDRESS;
		break;
	case Tic_Id_1:
		// cppcheck-suppress misra-c2012-11.4
		tic->regs = (volatile Tic_Registers *)TIC_TIC1_BASE_ADDRESS;
		break;
	case Tic_Id_2:
		// cppcheck-suppress misra-c2012-11.4
		tic->regs = (volatile Tic_Registers *)TIC_TIC2_BASE_ADDRESS;
		break;
#if defined(N7S_TARGET_SAMV71Q21) || defined(N7S_TARGET_SAMRH71F20)
	case Tic_Id_3:
		// cppcheck-suppress misra-c2012-11.4
		tic->regs = (volatile Tic_Registers *)TIC_TIC3_BASE_ADDRESS;
		break;
#endif
	default: assert(false && "Bad Tic ID selected."); return;
	}
	tic->ticId = id;
}

void
Tic_enableChannel(Tic *const tic, const Tic_Channel channel)
{
	assert((channel < Tic_Channel_Count) && "Invalid TIC channel");
	tic->regs->channelRegs[channel].ccr = TIC_CCR_CLKEN_MASK;
}

void
Tic_disableChannel(Tic *const tic, const Tic_Channel channel)
{
	assert((channel < Tic_Channel_Count) && "Invalid TIC channel");
	tic->regs->channelRegs[channel].ccr = TIC_CCR_CLKDIS_MASK;
}

bool
Tic_isChannelEnabled(const Tic *const tic, const Tic_Channel channel)
{
	assert((channel < Tic_Channel_Count) && "Invalid TIC channel");
	return ((tic->regs->channelRegs[channel].sr & TIC_SR_CLKSTA_MASK)
			!= 0u);
}

void
Tic_triggerChannel(Tic *const tic, const Tic_Channel channel)
{
	assert((channel < Tic_Channel_Count) && "Invalid TIC channel");
	tic->regs->channelRegs[channel].ccr = TIC_CCR_SWTRG_MASK;
}

void
Tic_syncAllChannels(Tic *const tic)
{
	tic->regs->bcr = TIC_BCR_SYNC_MASK;
}

static uint32_t
prepareCaptureModeConfig(const Tic_CaptureModeConfig config)
{
	const uint32_t cmr = BIT_VALUE(TIC_CMR_CAP_LDBSTOP,
					     config.isStoppedOnRbLoading)
			| BIT_VALUE(TIC_CMR_CAP_LDBDIS,
					config.isDisabledOnRbLoading)
			| BIT_VALUE(TIC_CMR_CAP_ETRGEDG,
					config.externalTriggerEdge)
			| BIT_FIELD_VALUE(TIC_CMR_CAP_ABETRG,
					config.triggerSource)
			| BIT_FIELD_VALUE(TIC_CMR_CAP_CPCTRG,
					config.isTriggeredByRcCompare)
			| BIT_VALUE(TIC_CMR_CAP_WAVE, false)
			| BIT_FIELD_VALUE(TIC_CMR_CAP_LDRA,
					config.raLoadingEdgeSelection)
			| BIT_FIELD_VALUE(TIC_CMR_CAP_LDRB,
					config.rbLoadingEdgeSelection)
			| BIT_FIELD_VALUE(TIC_CMR_CAP_SBSMPLR,
					config.loadingEdgeSubsampling);
	return cmr;
}

static uint32_t
prepareWaveformModeConfig(const Tic_WaveformModeConfig config)
{
	const uint32_t cmr = BIT_FIELD_VALUE(TIC_CMR_WVF_CPCSTOP,
					     config.isStoppedOnRcCompare)
			| BIT_FIELD_VALUE(TIC_CMR_WVF_CPCDIS,
					config.isDisabledOnRcCompare)
			| BIT_FIELD_VALUE(TIC_CMR_WVF_EEVTEDG,
					config.externalEvent)
			| BIT_FIELD_VALUE(TIC_CMR_WVF_EEVT,
					config.externalEventSource)
			| BIT_VALUE(TIC_CMR_WVF_ENETRG,
					config.isTriggeredByExternalEvent)
			| BIT_FIELD_VALUE(
					TIC_CMR_WVF_WAVSEL, config.waveformMode)
			| BIT_VALUE(TIC_CMR_WVF_WAVE, true)
			| BIT_FIELD_VALUE(TIC_CMR_WVF_ACPA,
					config.raCompareEffectOnTioa)
			| BIT_FIELD_VALUE(TIC_CMR_WVF_ACPC,
					config.rcCompareEffectOnTioa)
			| BIT_FIELD_VALUE(TIC_CMR_WVF_AEEVT,
					config.externalEventEffectOnTioa)
			| BIT_FIELD_VALUE(TIC_CMR_WVF_ASWTRG,
					config.triggerEffectOnTioa)
			| BIT_FIELD_VALUE(TIC_CMR_WVF_BCPB,
					config.rbCompareEffectOnTiob)
			| BIT_FIELD_VALUE(TIC_CMR_WVF_BCPC,
					config.rcCompareEffectOnTiob)
			| BIT_FIELD_VALUE(TIC_CMR_WVF_BEEVT,
					config.externalEventEffectOnTiob)
			| BIT_FIELD_VALUE(TIC_CMR_WVF_BSWTRG,
					config.triggerEffectOnTiob);
	return cmr;
}

void
Tic_setChannelConfig(Tic *const tic, const Tic_Channel channel,
		const Tic_ChannelConfig *const config)
{
	uint32_t cmr;

	cmr = BIT_FIELD_VALUE(TIC_CMR_CAP_TCCLKS, config->clockSource)
			| BIT_VALUE(TIC_CMR_CAP_CLKI, config->isClockInverted)
			| BIT_FIELD_VALUE(TIC_CMR_CAP_BURST, config->burst);

	switch (config->channelMode) {
	case Tic_Mode_Capture:
		cmr |= prepareCaptureModeConfig(
				config->modeConfig.captureModeConfig);
		break;
	case Tic_Mode_Waveform:
		cmr |= prepareWaveformModeConfig(
				config->modeConfig.waveformModeConfig);
		break;
	default: assert(false && "Invalid Tic_Mode selected"); return;
	}

	if (!config->isEnabled)
		Tic_disableChannel(tic, channel);

	tic->regs->channelRegs[channel].rc =
			BIT_FIELD_VALUE(TIC_RC_RC, config->rc);

	tic->regs->channelRegs[channel].cmr = cmr;

	if (config->channelMode == Tic_Mode_Waveform) {
		// Only in this mode these registers are read/write. Consequently, they have to be set after
		// mode is set in CMR.
		tic->regs->channelRegs[channel].ra = BIT_FIELD_VALUE(TIC_RA_RA,
				config->modeConfig.waveformModeConfig.ra);
		tic->regs->channelRegs[channel].rb = BIT_FIELD_VALUE(TIC_RB_RB,
				config->modeConfig.waveformModeConfig.rb);
	}

	tic->regs->channelRegs[channel].smmr =
			BIT_VALUE(TIC_SMMR_GCEN, config->isGrayCounterEnabled)
			| BIT_FIELD_VALUE(TIC_SMMR_DOWN,
					config->doesGrayCounterCountDown);

	Tic_setChannelIrqConfig(tic, channel, &config->irqConfig);

	tic->regs->channelRegs[channel].emr =
			BIT_FIELD_VALUE(TIC_EMR_TRIGSRCA,
					config->triggerSourceForInputA)
			| BIT_FIELD_VALUE(TIC_EMR_TRIGSRCB,
					config->triggerSourceForInputB)
			| BIT_FIELD_VALUE(TIC_EMR_NODIVCLK,
					config->useUndividedPck);

	if (config->isEnabled)
		Tic_enableChannel(tic, channel);
}

static void
getCaptureModeConfig(const Tic *const tic, const Tic_Channel channel,
		Tic_ChannelConfig *const config)
{
	assert((channel < Tic_Channel_Count) && "Invalid TIC channel");
	uint32_t cmr = tic->regs->channelRegs[channel].cmr;
	Tic_CaptureModeConfig *captureConfig =
			&config->modeConfig.captureModeConfig;

	config->channelMode = Tic_Mode_Capture;

	captureConfig->isStoppedOnRbLoading =
			((cmr & TIC_CMR_CAP_LDBSTOP_MASK) != 0u);
	captureConfig->isDisabledOnRbLoading =
			((cmr & TIC_CMR_CAP_LDBDIS_MASK) != 0u);
	captureConfig->externalTriggerEdge = ((cmr & TIC_CMR_CAP_ETRGEDG_MASK)
			>> TIC_CMR_CAP_ETRGEDG_OFFSET);
	captureConfig->triggerSource = ((cmr & TIC_CMR_CAP_ABETRG_MASK)
			>> TIC_CMR_CAP_ABETRG_OFFSET);
	captureConfig->isTriggeredByRcCompare =
			((cmr & TIC_CMR_CAP_CPCTRG_MASK) != 0u);
	captureConfig->raLoadingEdgeSelection = ((cmr & TIC_CMR_CAP_LDRA_MASK)
			>> TIC_CMR_CAP_LDRA_OFFSET);
	captureConfig->rbLoadingEdgeSelection = ((cmr & TIC_CMR_CAP_LDRB_MASK)
			>> TIC_CMR_CAP_LDRB_OFFSET);
	captureConfig->loadingEdgeSubsampling =
			((cmr & TIC_CMR_CAP_SBSMPLR_MASK)
					>> TIC_CMR_CAP_SBSMPLR_OFFSET);
}

static void
getWaveformModeConfig(const Tic *const tic, const Tic_Channel channel,
		Tic_ChannelConfig *config)
{
	assert((channel < Tic_Channel_Count) && "Invalid TIC channel");
	uint32_t cmr = tic->regs->channelRegs[channel].cmr;
	Tic_WaveformModeConfig *waveConfig =
			&config->modeConfig.waveformModeConfig;

	config->channelMode = Tic_Mode_Waveform;

	waveConfig->isStoppedOnRcCompare = ((cmr & TIC_CMR_WVF_CPCSTOP_MASK)
			>> TIC_CMR_WVF_CPCSTOP_OFFSET);
	waveConfig->isDisabledOnRcCompare = ((cmr & TIC_CMR_WVF_CPCDIS_MASK)
			>> TIC_CMR_WVF_CPCDIS_OFFSET);
	waveConfig->externalEvent = ((cmr & TIC_CMR_WVF_EEVTEDG_MASK)
			>> TIC_CMR_WVF_EEVTEDG_OFFSET);
	waveConfig->externalEventSource = ((cmr & TIC_CMR_WVF_EEVT_MASK)
			>> TIC_CMR_WVF_EEVT_OFFSET);
	waveConfig->isTriggeredByExternalEvent =
			((cmr & TIC_CMR_WVF_ENETRG_MASK)
					>> TIC_CMR_WVF_ENETRG_OFFSET);
	waveConfig->waveformMode = ((cmr & TIC_CMR_WVF_WAVSEL_MASK)
			>> TIC_CMR_WVF_WAVSEL_OFFSET);
	waveConfig->raCompareEffectOnTioa = ((cmr & TIC_CMR_WVF_ACPA_MASK)
			>> TIC_CMR_WVF_ACPA_OFFSET);
	waveConfig->rcCompareEffectOnTioa = ((cmr & TIC_CMR_WVF_ACPC_MASK)
			>> TIC_CMR_WVF_ACPC_OFFSET);
	waveConfig->externalEventEffectOnTioa = ((cmr & TIC_CMR_WVF_AEEVT_MASK)
			>> TIC_CMR_WVF_AEEVT_OFFSET);
	waveConfig->triggerEffectOnTioa = ((cmr & TIC_CMR_WVF_ASWTRG_MASK)
			>> TIC_CMR_WVF_ASWTRG_OFFSET);
	waveConfig->rbCompareEffectOnTiob = ((cmr & TIC_CMR_WVF_BCPB_MASK)
			>> TIC_CMR_WVF_BCPB_OFFSET);
	waveConfig->rcCompareEffectOnTiob = ((cmr & TIC_CMR_WVF_BCPC_MASK)
			>> TIC_CMR_WVF_BCPC_OFFSET);
	waveConfig->externalEventEffectOnTiob = ((cmr & TIC_CMR_WVF_BEEVT_MASK)
			>> TIC_CMR_WVF_BEEVT_OFFSET);
	waveConfig->triggerEffectOnTiob = ((cmr & TIC_CMR_WVF_BSWTRG_MASK)
			>> TIC_CMR_WVF_BSWTRG_OFFSET);

	waveConfig->ra = (tic->regs->channelRegs[channel].ra & TIC_RA_RA_MASK)
			>> TIC_RA_RA_OFFSET;
	waveConfig->rb = (tic->regs->channelRegs[channel].rb & TIC_RB_RB_MASK)
			>> TIC_RB_RB_OFFSET;
}

void
Tic_getChannelConfig(const Tic *const tic, const Tic_Channel channel,
		Tic_ChannelConfig *const config)
{
	assert((channel < Tic_Channel_Count) && "Invalid TIC channel");
	uint32_t cmr = tic->regs->channelRegs[channel].cmr;

	config->isEnabled = Tic_isChannelEnabled(tic, channel);
	config->clockSource = ((cmr & TIC_CMR_CAP_TCCLKS_MASK)
			>> TIC_CMR_CAP_TCCLKS_OFFSET);
	config->isClockInverted = ((cmr & TIC_CMR_CAP_CLKI_MASK) != 0u);
	config->burst = ((cmr & TIC_CMR_CAP_BURST_MASK)
			>> TIC_CMR_CAP_BURST_OFFSET);

	if ((cmr & TIC_CMR_CAP_WAVE_MASK) == 0u)
		getCaptureModeConfig(tic, channel, config);
	else
		getWaveformModeConfig(tic, channel, config);

	config->isGrayCounterEnabled = ((tic->regs->channelRegs[channel].smmr
							& TIC_SMMR_GCEN_MASK)
			!= 0u);
	config->doesGrayCounterCountDown =
			((tic->regs->channelRegs[channel].smmr
					 & TIC_SMMR_DOWN_MASK)
					!= 0u);

	Tic_getChannelIrqConfig(tic, channel, &config->irqConfig);

	config->triggerSourceForInputA =
			((tic->regs->channelRegs[channel].emr
					 & TIC_EMR_TRIGSRCA_MASK)
					>> TIC_EMR_TRIGSRCA_OFFSET);
	config->triggerSourceForInputB =
			((tic->regs->channelRegs[channel].emr
					 & TIC_EMR_TRIGSRCB_MASK)
					>> TIC_EMR_TRIGSRCB_OFFSET);
	config->useUndividedPck = ((tic->regs->channelRegs[channel].emr
						   & TIC_EMR_NODIVCLK_MASK)
			!= 0u);
	config->rc = (tic->regs->channelRegs[channel].rc & TIC_RC_RC_MASK)
			>> TIC_RC_RC_OFFSET;
}

void
Tic_setChannelIrqConfig(Tic *const tic, const Tic_Channel channel,
		const Tic_ChannelIrqConfig *const config)
{
	config->isCounterOverflowIrqEnabled
			? Tic_enableChannelIrq(
					  tic, channel, Tic_Irq_CounterOverflow)
			: Tic_disableChannelIrq(tic, channel,
					  Tic_Irq_CounterOverflow);

	config->isLoadOverrunIrqEnabled ? Tic_enableChannelIrq(tic, channel,
							  Tic_Irq_LoadOverrun)
					: Tic_disableChannelIrq(tic, channel,
							  Tic_Irq_LoadOverrun);

	config->isRaCompareIrqEnabled
			? Tic_enableChannelIrq(tic, channel, Tic_Irq_RaCompare)
			: Tic_disableChannelIrq(
					  tic, channel, Tic_Irq_RaCompare);

	config->isRbCompareIrqEnabled
			? Tic_enableChannelIrq(tic, channel, Tic_Irq_RbCompare)
			: Tic_disableChannelIrq(
					  tic, channel, Tic_Irq_RbCompare);

	config->isRcCompareIrqEnabled
			? Tic_enableChannelIrq(tic, channel, Tic_Irq_RcCompare)
			: Tic_disableChannelIrq(
					  tic, channel, Tic_Irq_RcCompare);

	config->isRaLoadingIrqEnabled
			? Tic_enableChannelIrq(tic, channel, Tic_Irq_RaLoading)
			: Tic_disableChannelIrq(
					  tic, channel, Tic_Irq_RaLoading);

	config->isRbLoadingIrqEnabled
			? Tic_enableChannelIrq(tic, channel, Tic_Irq_RbLoading)
			: Tic_disableChannelIrq(
					  tic, channel, Tic_Irq_RbLoading);

	config->isExternalTriggerIrqEnabled
			? Tic_enableChannelIrq(
					  tic, channel, Tic_Irq_ExternalTrigger)
			: Tic_disableChannelIrq(tic, channel,
					  Tic_Irq_ExternalTrigger);
}

void
Tic_getChannelIrqConfig(const Tic *const tic, const Tic_Channel channel,
		Tic_ChannelIrqConfig *const config)
{
	config->isCounterOverflowIrqEnabled = Tic_isChannelIrqEnabled(
			tic, channel, Tic_Irq_CounterOverflow);
	config->isLoadOverrunIrqEnabled = Tic_isChannelIrqEnabled(
			tic, channel, Tic_Irq_LoadOverrun);
	config->isRaCompareIrqEnabled = Tic_isChannelIrqEnabled(
			tic, channel, Tic_Irq_RaCompare);
	config->isRbCompareIrqEnabled = Tic_isChannelIrqEnabled(
			tic, channel, Tic_Irq_RbCompare);
	config->isRcCompareIrqEnabled = Tic_isChannelIrqEnabled(
			tic, channel, Tic_Irq_RcCompare);
	config->isRaLoadingIrqEnabled = Tic_isChannelIrqEnabled(
			tic, channel, Tic_Irq_RaLoading);
	config->isRbLoadingIrqEnabled = Tic_isChannelIrqEnabled(
			tic, channel, Tic_Irq_RbLoading);
	config->isExternalTriggerIrqEnabled = Tic_isChannelIrqEnabled(
			tic, channel, Tic_Irq_ExternalTrigger);
}

void
Tic_enableChannelIrq(
		Tic *const tic, const Tic_Channel channel, const Tic_Irq irq)
{
	assert((channel < Tic_Channel_Count) && "Invalid TIC channel");
	tic->regs->channelRegs[channel].ier |= (1u << (uint32_t)irq);
}

void
Tic_disableChannelIrq(
		Tic *const tic, const Tic_Channel channel, const Tic_Irq irq)
{
	assert((channel < Tic_Channel_Count) && "Invalid TIC channel");
	tic->regs->channelRegs[channel].idr |= (1u << (uint32_t)irq);
}

bool
Tic_isChannelIrqEnabled(const Tic *const tic, const Tic_Channel channel,
		const Tic_Irq irq)
{
	assert((channel < Tic_Channel_Count) && "Invalid TIC channel");
	return ((tic->regs->channelRegs[channel].imr & (1u << (uint32_t)irq))
			!= 0u);
}

void
Tic_getChannelStatus(const Tic *const tic, const Tic_Channel channel,
		Tic_ChannelStatus *const status)
{
	assert((channel < Tic_Channel_Count) && "Invalid TIC channel");
	const uint32_t regValue = tic->regs->channelRegs[channel].sr;

	status->hasCounterOverflowed = (regValue & TIC_SR_COVFS_MASK) != 0u;
	status->hasLoadOverrunOccurred = (regValue & TIC_SR_LOVRS_MASK) != 0u;
	status->hasRaCompareOccurred = (regValue & TIC_SR_CPAS_MASK) != 0u;
	status->hasRbCompareOccurred = (regValue & TIC_SR_CPBS_MASK) != 0u;
	status->hasRcCompareOccurred = (regValue & TIC_SR_CPCS_MASK) != 0u;
	status->hasRaLoadOccurred = (regValue & TIC_SR_LDRAS_MASK) != 0u;
	status->hasRbLoadOccurred = (regValue & TIC_SR_LDRBS_MASK) != 0u;
	status->hasExternalTriggerOccurred =
			(regValue & TIC_SR_ETRGS_MASK) != 0u;
	status->isChannelEnabled = (regValue & TIC_SR_CLKSTA_MASK) != 0u;
	status->isTioaAsserted = (regValue & TIC_SR_MTIOA_MASK) != 0u;
	status->isTiobAsserted = (regValue & TIC_SR_MTIOB_MASK) != 0u;
}

uint32_t
Tic_getCounterValue(const Tic *const tic, const Tic_Channel channel)
{
	assert((channel < Tic_Channel_Count) && "Invalid TIC channel");
	return tic->regs->channelRegs[channel].cv;
}

void
Tic_configureExternalClockSignals(Tic *const tic,
		const Tic_ExternalClockSignalSelection
				*const externalClockSignals)
{
	uint32_t bmr = tic->regs->bmr;
	bmr &= (uint32_t)~(TIC_BMR_TC0XC0S_MASK | TIC_BMR_TC1XC1S_MASK
			| TIC_BMR_TC2XC2S_MASK);
	bmr |= BIT_FIELD_VALUE(TIC_BMR_TC0XC0S, externalClockSignals->xc0)
			| BIT_FIELD_VALUE(TIC_BMR_TC1XC1S,
					externalClockSignals->xc1)
			| BIT_FIELD_VALUE(TIC_BMR_TC2XC2S,
					externalClockSignals->xc2);
	tic->regs->bmr = bmr;
}

void
Tic_writeProtect(Tic *const tic, bool protect)
{
	const bool clksta = ((tic->regs->channelRegs[Tic_Channel_0].sr
					     & TIC_SR_CLKSTA_MASK)
			!= 0u);

	Tic_enableChannel(tic, Tic_Channel_0);
	tic->regs->wpmr = BIT_FIELD_VALUE(TIC_WPMR_WPKEY,
					  TIC_WPMR_WPKEY_PASSWD_VALUE)
			| BIT_VALUE(TIC_WPMR_WPEN, protect);

	if (!clksta)
		Tic_disableChannel(tic, Tic_Channel_0);
}

uint32_t
Tic_getRaValue(const Tic *const tic, const Tic_Channel channel)
{
	assert((channel < Tic_Channel_Count) && "Invalid TIC channel");
	return (tic->regs->channelRegs[channel].ra & TIC_RA_RA_MASK)
			>> TIC_RA_RA_OFFSET;
}

uint32_t
Tic_getRbValue(const Tic *const tic, const Tic_Channel channel)
{
	assert((channel < Tic_Channel_Count) && "Invalid TIC channel");
	return (tic->regs->channelRegs[channel].rb & TIC_RB_RB_MASK)
			>> TIC_RB_RB_OFFSET;
}

uint32_t
Tic_getRcValue(const Tic *const tic, const Tic_Channel channel)
{
	assert((channel < Tic_Channel_Count) && "Invalid TIC channel");
	return (tic->regs->channelRegs[channel].rc & TIC_RC_RC_MASK)
			>> TIC_RC_RC_OFFSET;
}
