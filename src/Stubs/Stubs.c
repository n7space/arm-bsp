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

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>

#include <Pio/Pio.h>
#include <Pmc/Pmc.h>
#include <SystemConfig/SystemConfig.h>

#if defined(USE_UART_IO)
#include <Uart/Uart.h>
#elif defined(USE_SDRAM_IO)
#include <Sdramc/Sdramc.h>
#endif

#include "Stubs.h"

#define GCOV_DUMMY_FD 0

#ifdef ENABLE_COVERAGE
extern void __gcov_flush(void);
#endif

#define WRITE_STRING_CONSTANT(s) _write(GCOV_DUMMY_FD + 1, (s), sizeof(s) - 1u)

extern int _eheap;
extern int _sheap;

#if defined(USE_USB_USART_IO)
static inline void
configurePioPins(void)
{
	Pio pioB;
	Pio_init(Pio_Port_B, &pioB);

	Pio_Pin_Config pinConf = { 0 };
	pinConf.control = Pio_Control_PeripheralD;
	pinConf.direction = Pio_Direction_Input;
	pinConf.pull = Pio_Pull_None;
	pinConf.filter = Pio_Filter_None;
	pinConf.isMultiDriveEnabled = false;
	pinConf.isSchmittTriggerDisabled = false;
	pinConf.irq = Pio_Irq_None;

	Pio_setPinsConfig(&pioB, PIO_PIN_4 | PIO_PIN_5, &pinConf);
}

void
Stubs_startup(void)
{
	uint32_t *CCFG_SYSIO = (uint32_t *)0x40088114;
	*CCFG_SYSIO |= 1u << 4u;

	Pmc_enablePeripheralClk(Pmc_PeripheralId_PioB);
	Pmc_enablePeripheralClk(Pmc_PeripheralId_Usart1);
	configurePioPins();

	volatile uint32_t *const US_CR = (uint32_t *)0x40028000; // USART_1
	volatile uint32_t *const US_MR = (uint32_t *)0x40028004;
	volatile uint32_t *const US_BRGR = (uint32_t *)0x40028020;

	*US_MR = 0; // Clear
	*US_MR |= 0u << 0; // USART_MODE NORMAL
	*US_MR |= 0u << 4; // USCLKS PCK
	*US_MR |= 3u << 6; // CHRL 8BIT
	*US_MR |= 0u << 8; // SYNC Asynchronous
	*US_MR |= 4u << 9; // PAR No parity
	*US_MR |= 0u << 12; // NBSTOP 1 stop bit
	*US_MR |= 0u << 14; // CHMODE Normal mode
	*US_MR |= 0u << 16; // MSBF MSB
	*US_MR |= 0u << 17; // MODE9 CHRL defines length
	*US_MR |= 0u << 18; // CLKO does not drive SCK pin
	*US_MR |= 0u << 19; // OVER 16x oversampling
	*US_MR |= 1u << 20; // INACK NSCK is not generated
	*US_MR |= 0u << 21; // DSNACK don't care - no NACK
	*US_MR |= 0u << 22; // VAR_SYNC MODSYNC defines sync
	*US_MR |= 0u << 23; // INVDATA do not invert data
	*US_MR |= 0u << 24; // MAX_ITERATION don't care - valid in ISO7816 protocol
	*US_MR |= 0u << 28; // FILTER do not filter incomming data
	*US_MR |= 0u << 29; // MAN manchester coding disabled
	*US_MR |= 0u << 30; // MODSYNC don't care - manchester disabled
	*US_MR |= 1u << 31; // ONEBIT 1 bit start frame delimiter

	// BaudRate = CLK / ((coarseDiv + fineDiv / 8) * 16)
	uint32_t coarseDiv = SystemConfig_DefaultPeriphClock
			/ (16u * LOW_LEVEL_IO_BAUDRATE);
	uint64_t fineDiv = 8uLL
			* (((uint64_t)SystemConfig_DefaultPeriphClock * 1000uLL
					   / (16uLL * LOW_LEVEL_IO_BAUDRATE))
					- (uint64_t)coarseDiv * 1000uLL);
	fineDiv /= 1000uLL;

	*US_BRGR = coarseDiv | ((uint32_t)fineDiv << 16u);
	*US_CR = 1u << 6u; // Enable the transmitter
}

void
Stubs_shutdown(void)
{
	Pmc_disablePeripheralClk(Pmc_PeripheralId_Usart1);
}

static void
waitForTransmitterReady(void)
{
	volatile uint32_t *const US_CSR = (uint32_t *)0x40028014;
	while (((*US_CSR) & 0x00000002) == 0)
		asm volatile("nop");
}

static void
writeByte(const uint8_t data)
{
	waitForTransmitterReady();

	volatile uint32_t *const US_THR = (uint32_t *)0x4002801C;
	*US_THR = data;
}

#elif defined(USE_UART_IO)

static Uart Stubs_uart;

static void
configurePioPins(Pio *const pio, Pio_Control const peripheral,
		uint32_t const pinMask)
{
	Pio_Port_Config pioConf = { 0 };
	pioConf.pinsConfig.pull = Pio_Pull_Up;
	pioConf.pinsConfig.filter = Pio_Filter_None;
	pioConf.pinsConfig.isMultiDriveEnabled = false;
	pioConf.pinsConfig.isSchmittTriggerDisabled = false;
	pioConf.pinsConfig.irq = Pio_Irq_None;
	pioConf.debounceFilterDiv = 0;
	pioConf.pins = pinMask;
	pioConf.pinsConfig.control = peripheral;
	Pio_setPortConfig(pio, &pioConf);
}

static inline void
configureUart(void)
{
	Uart_Config conf = { 0 };
	conf.isTxEnabled = true;
	conf.isRxEnabled = true;
	conf.isTestModeEnabled = false;
	conf.parity = Uart_Parity_None;
	conf.baudRate = LOW_LEVEL_IO_BAUDRATE;
	conf.baudRateClkSrc = Uart_BaudRateClk_PeripheralCk;
	conf.baudRateClkFreq = SystemConfig_DefaultPeriphClock;

	Uart_init(LOW_LEVEL_IO_UART_ID, &Stubs_uart);
	Uart_setConfig(&Stubs_uart, &conf);
}

void
Stubs_startup(void)
{
	Pio pio;

	switch (LOW_LEVEL_IO_UART_ID) {
	case Uart_Id_0:
		Pio_init(Pio_Port_A, &pio);
		Pmc_enablePeripheralClk(Pmc_PeripheralId_PioA);
		Pmc_enablePeripheralClk(Pmc_PeripheralId_Uart0);
		configurePioPins(&pio, Pio_Control_PeripheralA,
				PIO_PIN_9 | PIO_PIN_10);
		break;
	case Uart_Id_1:
		Pio_init(Pio_Port_A, &pio);
		Pmc_enablePeripheralClk(Pmc_PeripheralId_PioA);
		Pmc_enablePeripheralClk(Pmc_PeripheralId_Uart1);
		configurePioPins(&pio, Pio_Control_PeripheralC,
				PIO_PIN_5 | PIO_PIN_6);
		break;
	case Uart_Id_2:
		Pio_init(Pio_Port_D, &pio);
		Pmc_enablePeripheralClk(Pmc_PeripheralId_PioD);
		Pmc_enablePeripheralClk(Pmc_PeripheralId_Uart2);
		configurePioPins(&pio, Pio_Control_PeripheralC,
				PIO_PIN_25 | PIO_PIN_26);
		break;
	case Uart_Id_3:
		Pio_init(Pio_Port_D, &pio);
		Pmc_enablePeripheralClk(Pmc_PeripheralId_PioD);
		Pmc_enablePeripheralClk(Pmc_PeripheralId_Uart3);
		configurePioPins(&pio, Pio_Control_PeripheralA,
				PIO_PIN_28 | PIO_PIN_30);
		break;
	case Uart_Id_4:
		Pio_init(Pio_Port_D, &pio);
		Pmc_enablePeripheralClk(Pmc_PeripheralId_PioD);
		Pmc_enablePeripheralClk(Pmc_PeripheralId_Uart4);
		configurePioPins(&pio, Pio_Control_PeripheralC,
				PIO_PIN_18 | PIO_PIN_19);
		break;
	default: assert(false);
	}
	configureUart();
}

static inline void
writeByte(const uint8_t data)
{
	Uart_write(&Stubs_uart, data, 10000000, NULL);
}

static inline void
waitForTransmitterReady(void)
{
	while ((Uart_getStatusRegister(&Stubs_uart) & UART_SR_TXEMPTY_MASK)
			!= 0u)
		;
}

void
Stubs_shutdown(void)
{
	Uart_shutdown(&Stubs_uart);

	switch (LOW_LEVEL_IO_UART_ID) {
	case Uart_Id_0: Pmc_disablePeripheralClk(Pmc_PeripheralId_Uart0); break;
	case Uart_Id_1: Pmc_disablePeripheralClk(Pmc_PeripheralId_Uart1); break;
	case Uart_Id_2: Pmc_disablePeripheralClk(Pmc_PeripheralId_Uart2); break;
	case Uart_Id_3: Pmc_disablePeripheralClk(Pmc_PeripheralId_Uart3); break;
	case Uart_Id_4: Pmc_disablePeripheralClk(Pmc_PeripheralId_Uart4); break;
	default: assert(false);
	}
}

#elif defined(USE_SDRAM_IO)

extern uint8_t sdramMemory_begin;
extern uint8_t sdramMemory_end;
static volatile uint32_t *stdoutByteCountPtr;
static volatile uint8_t *stdoutArray;
static volatile uint32_t stdoutArraySize;

static Sdramc Stubs_sdramc;

static inline void
assignPin(Pio *const pio, const uint32_t pin, const Pio_Control control)
{
	Pio_Pin_Config config;
	memset(&config, 0, sizeof(Pio_Pin_Config));
	config.control = control;
	config.pull = Pio_Pull_Up;
	Pio_setPinsConfig(pio, pin, &config);
}

static inline void
configurePortA(void)
{
	Pio portA;
	Pio_init(Pio_Port_A, &portA);
	Pmc_enablePeripheralClk(Pmc_PeripheralId_PioA);
	assignPin(&portA, PIO_PIN_20, Pio_Control_PeripheralC); // Connected
	assignPin(&portA, PIO_PIN_15, Pio_Control_PeripheralA); // Connected
	assignPin(&portA, PIO_PIN_16, Pio_Control_PeripheralA); // Connected
}

static inline void
configurePortC(void)
{
	Pio portC;
	Pio_init(Pio_Port_C, &portC);
	Pmc_enablePeripheralClk(Pmc_PeripheralId_PioC);
	assignPin(&portC, PIO_PIN_18, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_20, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_21, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_22, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_23, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_24, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_25, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_26, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_27, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_28, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_29, Pio_Control_PeripheralA); // Connected

	assignPin(&portC, PIO_PIN_0, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_1, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_2, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_3, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_4, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_5, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_6, Pio_Control_PeripheralA); // Connected
	assignPin(&portC, PIO_PIN_7, Pio_Control_PeripheralA); // Connected

	assignPin(&portC, PIO_PIN_15, Pio_Control_PeripheralA); // Connected
}

static inline void
configurePortD(void)
{
	Pio portD;
	Pio_init(Pio_Port_D, &portD);
	Pmc_enablePeripheralClk(Pmc_PeripheralId_PioD);
	assignPin(&portD, PIO_PIN_17, Pio_Control_PeripheralC); // Connected

	assignPin(&portD, PIO_PIN_15, Pio_Control_PeripheralC); // Connected
	assignPin(&portD, PIO_PIN_16, Pio_Control_PeripheralC); // Connected

	assignPin(&portD, PIO_PIN_13,
			Pio_Control_PeripheralC); // Connected Shared PC13
	assignPin(&portD, PIO_PIN_23, Pio_Control_PeripheralC); // Connected
	assignPin(&portD, PIO_PIN_14, Pio_Control_PeripheralC); // Connected
	assignPin(&portD, PIO_PIN_29, Pio_Control_PeripheralC); // Connected
}

static inline void
configurePortE(void)
{
	Pio portE;
	Pio_init(Pio_Port_E, &portE);
	Pmc_enablePeripheralClk(Pmc_PeripheralId_PioE);
	assignPin(&portE, PIO_PIN_0, Pio_Control_PeripheralA); // Connected
	assignPin(&portE, PIO_PIN_1, Pio_Control_PeripheralA); // Connected
	assignPin(&portE, PIO_PIN_2, Pio_Control_PeripheralA); // Connected
	assignPin(&portE, PIO_PIN_3, Pio_Control_PeripheralA); // Connected
	assignPin(&portE, PIO_PIN_4, Pio_Control_PeripheralA); // Connected
	assignPin(&portE, PIO_PIN_5, Pio_Control_PeripheralA); // Connected
}

static inline void
configurePio(void)
{
	configurePortA();
	configurePortC();
	configurePortD();
	configurePortE();
}

static inline void
configureClock(void)
{
	Pmc_enablePeripheralClk(Pmc_PeripheralId_Smc);
	Pmc_enablePeripheralClk(Pmc_PeripheralId_Sdramc);
}

static inline void
configureSdramc(Sdramc *const sdramc)
{
	Sdramc_Config config;
	config.refreshTimerCount = 1172; // Value 15.625 us for 75 MHz
	config.columnBits = Sdramc_NumberOfColumnBits_Col8;
	config.rowBits = Sdramc_NumberOfRowBits_Row11;
	config.banks = Sdramc_NumberOfBanks_Bank2;
	config.casLatency = Sdramc_CasLatency_Latency3;
	config.dataBusWidth = Sdramc_DataBusWidth_16bits;
	// These values apply to IS42S16100H chip
	config.writeRecoveryDelay = 5;
	config.rowCycleDelayAndRowRefreshCycle = 13;
	config.rowPrechargeDelay = 5;
	config.rowToColumnDelay = 5;
	config.activeToPrechargeDelay = 9;
	config.exitSelfRefreshToActiveDelay = 15;
	config.lowPowerConfiguration = Sdramc_LowPowerConfiguration_Disabled;
	config.partialArraySelfRefresh = 0;
	config.temperatureCompensatedSelfRefresh = 0;
	config.driveStrength = 0;
	config.lowPowerEnableTimeout =
			Sdramc_LowPowerEnableTimeout_LpLastXfer128;
	config.deviceType = Sdramc_MemoryDeviceType_Sdram;
	config.loadModeRegisterCommandToActiveOrRefreshCommand = 2;
	config.supportUnalignedAccess = Sdramc_SupportUnalignedAccess_Supported;
	config.isRefreshErrorStatusInterruptEnabled = false;

	Sdramc_setConfig(sdramc, &config);
}
void
Stubs_startup(void)
{
	stdoutArraySize = (uint32_t)&sdramMemory_end
			- (uint32_t)&sdramMemory_begin - sizeof(uint32_t);
	stdoutByteCountPtr = (uint32_t *)((uint32_t)&sdramMemory_begin);
	stdoutArray = (uint8_t *)((uint32_t)&sdramMemory_begin
			+ sizeof(uint32_t));

	configurePio();
	configureClock();
	Sdramc_init(&Stubs_sdramc);
	Sdramc_startup(&Stubs_sdramc);
	configureSdramc(&Stubs_sdramc);
	Sdramc_performInitializationSequence(
			&Stubs_sdramc, SystemConfig_DefaultCoreClock);

	memset(&sdramMemory_begin, 0,
			(uint32_t)&sdramMemory_end
					- (uint32_t)&sdramMemory_begin);
}

void
Stubs_shutdown(void)
{
	Sdramc_shutdown(&Stubs_sdramc);
	Pmc_disablePeripheralClk(Pmc_PeripheralId_Sdramc);
}

static void
writeByte(const uint8_t data)
{
	stdoutArray[*stdoutByteCountPtr] = data;
	++(*stdoutByteCountPtr);
	if (*stdoutByteCountPtr > (uint32_t)(stdoutArray + stdoutArraySize)) {
		--(*stdoutByteCountPtr);
		assert(false);
	}
}

static void
waitForTransmitterReady(void)
{
	asm volatile("dmb");
	asm volatile("dsb");
}

#else
#error "Usage of stdio would result in a crash, as low level IO interface was not selected with proper #define"
#endif

int _fstat(const int file, struct stat *const st);
int
_fstat(const int file, struct stat *const st)
{
	(void)file;
	(void)st;
	return -1;
}

void *_sbrk(const intptr_t incr);
void *
_sbrk(const intptr_t incr)
{
	static unsigned char *heap = (unsigned char *)&_sheap;

	if ((heap + incr) >= (unsigned char *)&_eheap)
		return (void *)-1;

	unsigned char *const prev_heap = heap;
	heap += incr;

	return prev_heap;
}

void _kill(const int pid, const int sig);
void
_kill(const int pid, const int sig)
{
	(void)pid;
	(void)sig;
}

int _getpid(void);
int
_getpid(void)
{
	return -1;
}

static inline uint8_t
nibbleToHex(const uint8_t value)
{
	switch (value) {
	case 15: return 'F';
	case 14: return 'E';
	case 13: return 'D';
	case 12: return 'C';
	case 11: return 'B';
	case 10: return 'A';
	default: return (uint8_t)(value + '0');
	}
}

static inline void
writeByteAsHexString(const uint8_t data)
{
	writeByte(nibbleToHex((data >> 4u) & 0x0Fu));
	writeByte(nibbleToHex(data & 0x0Fu));
}

static inline void
writeIntAsHexString(const uint32_t data)
{
	writeByteAsHexString((uint8_t)((data >> 24u) & 0xFFu));
	writeByteAsHexString((uint8_t)((data >> 16u) & 0xFFu));
	writeByteAsHexString((uint8_t)((data >> 8u) & 0xFFu));
	writeByteAsHexString((uint8_t)(data & 0xFFu));
}

int _write(const int fd, const void *const buffer, const unsigned int count);
int
_write(const int fd, const void *const buffer, const unsigned int count)
{
	const uint8_t *data = (const uint8_t *)buffer;

	for (uint32_t i = 0; i < count; i++) {
		if (fd == GCOV_DUMMY_FD)
			writeByteAsHexString(data[i]);
		else
			writeByte(data[i]);
	}

	waitForTransmitterReady();

	return (int)count;
}

int _open(const char *const filename, const int oflag);
int
_open(const char *const filename, const int oflag)
{
	(void)oflag;
	WRITE_STRING_CONSTANT("\n>>>");
	_write(1, filename, strlen(filename));
	WRITE_STRING_CONSTANT("\n");
	return GCOV_DUMMY_FD;
}

int _isatty(const int file);
int
_isatty(const int file)
{
	(void)file;
	return 0;
}

int _close(const int file);
int
_close(const int file)
{
	(void)file;
	return 0;
}

int _lseek(const int file, const int ptr, const int dir);
int
_lseek(const int file, const int ptr, const int dir)
{
	(void)file;
	(void)ptr;
	(void)dir;
	return 0;
}

int _read(const int fd, void *const buffer, const unsigned int count);
int
_read(const int fd, void *const buffer, const unsigned int count)
{
	(void)fd;
	(void)buffer;
	(void)count;
	return -1;
}

void _exit(const int status);
void
_exit(const int status)
{
	WRITE_STRING_CONSTANT("\n>> EXIT STATUS: ");
	writeIntAsHexString((uint32_t)status);
	WRITE_STRING_CONSTANT("\n");

#ifdef ENABLE_COVERAGE
	WRITE_STRING_CONSTANT("\n>> COVERAGE RESULT - BEGIN <<");
	__gcov_flush();
	WRITE_STRING_CONSTANT("\n>> COVERAGE RESULT - END <<\n");
#endif

	asm volatile("BKPT #0");
	for (;;)
		;
}

void
Stubs_writeByte(const uint8_t byte)
{
	writeByte(byte);
}
