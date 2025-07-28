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

/// \file ScbRegisters.h
/// \addtogroup Bsp
/// \brief Header containing System Control Block specific register definitions.

#ifndef BSP_SCB_REGISTERS_H
#define BSP_SCB_REGISTERS_H

#include <stdint.h>

// clang-format off

/// \brief System control registers
typedef struct {
   uint32_t actlr;          ///< 0xE000E008 Auxiliary Control Register
   uint32_t reserved0[829]; ///< 0xE000E00C - 0xE000ECFC Reserved
   uint32_t cpuid;          ///< 0xE000ED00 CPUID Base Register
   uint32_t icsr;           ///< 0xE000ED04 Interrupt Control and State Register
   uint32_t vtor;           ///< 0xE000ED08 Vector Table Offset Register
   uint32_t aircr;          ///< 0xE000ED0C Application Interrupt and Reset Control Register
   uint32_t scr;            ///< 0xE000ED10 System Control Register
   uint32_t ccr;            ///< 0xE000ED14 Configuration and Control Register
   uint32_t shpr1;          ///< 0xE000ED18 System Handler Priority Register 1
   uint32_t shpr2;          ///< 0xE000ED1C System Handler Priority Register 2
   uint32_t shpr3;          ///< 0xE000ED20 System Handler Priority Register 3
   uint32_t shcsr;          ///< 0xE000ED24 System Handler Control and State Register
   uint32_t cfsr;           ///< 0xE000ED28 Configurable Fault Status Registers
   uint32_t hfsr;           ///< 0xE000ED2C HardFault Status Register
   uint32_t dfsr;           ///< 0xE000ED30 Debug Fault Status Register
   uint32_t mmfar;          ///< 0xE000ED34 MemManage Fault Address Register
   uint32_t bfar;           ///< 0xE000ED38 BusFault Address Register
   uint32_t reserved1;      ///< 0xE000ED3C Reserved
   uint32_t idpfr0;         ///< 0xE000ED40 Processor Feature Register 0
   uint32_t idpfr1;         ///< 0xE000ED44 Processor Feature Register 1
   uint32_t iddfr0;         ///< 0xE000ED48 Debug Feature Register 0
   uint32_t idafr0;         ///< 0xE000ED4C Auxiliary Feature Register 0
   uint32_t idmmfr0;        ///< 0xE000ED50 Memory Model Feature Register 0
   uint32_t idmmfr1;        ///< 0xE000ED54 Memory Model Feature Register 1
   uint32_t idmmfr2;        ///< 0xE000ED58 Memory Model Feature Register 2
   uint32_t idmmfr3;        ///< 0xE000ED5C Memory Model Feature Register 3
   uint32_t idisar0;        ///< 0xE000ED60 Instruction Set Attributes Register 0
   uint32_t idisar1;        ///< 0xE000ED64 Instruction Set Attributes Register 1
   uint32_t idisar2;        ///< 0xE000ED68 Instruction Set Attributes Register 2
   uint32_t idisar3;        ///< 0xE000ED6C Instruction Set Attributes Register 3
   uint32_t idisar4;        ///< 0xE000ED70 Instruction Set Attributes Register 4
   uint32_t reserved2;      ///< 0xE000ED74 Reserved
   uint32_t clidr;          ///< 0xE000ED78 Cache Level ID Register
   uint32_t ctr;            ///< 0xE000ED7C Cache Type Register
   uint32_t ccsidr;         ///< 0xE000ED80 Cache Size ID Register
   uint32_t csselr;         ///< 0xE000ED84 Cache Size Selection Register
   uint32_t cpacr;          ///< 0xE000ED88 Coprocessor Access Control Register
   uint32_t reserved3[93];  ///< 0xE000ED8C - 0xE000EEFC Reserved
   uint32_t stir;           ///< 0xE000EF00 Software Triggered Interrupt Register
   uint32_t reserved4[19];  ///< 0xE000EF04 - 0xE000EF4C Reserved
   uint32_t iciallu;        ///< 0xE000EF50 Instruction cache invalidate all to Point of Unification
   uint32_t reserved6;      ///< 0xE000EF54 Reserved
   uint32_t icimvau; ///< 0xE000EF58 Instruction cache invalidate by address to Point of Unification
   uint32_t dcimvac; ///< 0xE000EF5C Data cache invalidate by address to Point of Coherency
   uint32_t dcisw;   ///< 0xE000EF60 Data cache invalidate by set/way
   uint32_t dccmvau; ///< 0xE000EF64 Data cache by address to Point of Unification
   uint32_t dccmvac; ///< 0xE000EF68 Data cache clean by address to Point of Coherency
   uint32_t dccsw;   ///< 0xE000EF6C Data cache clean by set/way
   /// \brief 0xE000EF70 Data cache clean and invalidate by address to Point of Coherency
   uint32_t dccimvac;
   uint32_t dccisw;        ///< 0xE000EF74 Data cache clean and invalidate by set/way
   uint32_t bpiall;        ///< 0xE000EF78 Not implemented
   uint32_t reserved7[5];  ///< 0xE000EF7C - 0xE000EF8C Reserved
   uint32_t itcmcr;        ///< 0xE000EF90 Instruction Tightly-Coupled Memory Control Register
   uint32_t dtcmcr;        ///< 0xE000EF94 Data Tightly-Coupled Memory Control Register
   uint32_t ahbpcr;        ///< 0xE000EF98 AHBP Control Register
   uint32_t cacr;          ///< 0xE000EF9C L1 Cache Control Register
   uint32_t ahbscr;        ///< 0xE000EFA0 AHB Slave Control Register
   uint32_t reserved8;     ///< 0xE000EFA4 Reserved
   uint32_t abfsr;         ///< 0xE000EFA8 Auxiliary Bus Fault Status Register
   uint32_t reserved9;     ///< 0xE000EFAC Reserved
   uint32_t iebr0;         ///< 0xE000EFB0 Instruction Error bank Register 0
   uint32_t iebr1;         ///< 0xE000EFB4 Instruction Error bank Register 1
   uint32_t debr0;         ///< 0xE000EFB8 Data Error bank Register 0
   uint32_t debr1;         ///< 0xE000EFBC Data Error bank Register 1
} Scb_Registers;

#define SCB_BASE_ADDRESS                  0xE000E008u
#define SCB_DCCMVAU_ADDRESS               0xE000EF64u

#define SCB_ACTLR_DISFOLD_MASK            0x00000004u
#define SCB_ACTLR_DISFOLD_OFFSET          0u
#define SCB_ACTLR_FPEXCODIS_MASK          0x00000400u
#define SCB_ACTLR_FPEXCODIS_OFFSET        10u
#define SCB_ACTLR_DISRAMODE_MASK          0x00000800u
#define SCB_ACTLR_DISRAMODE_OFFSET        11u
#define SCB_ACTLR_DISITMATBFLUSH_MASK     0x00001000u
#define SCB_ACTLR_DISITMATBFLUSH_OFFSET   12u
#define SCB_ACTLR_DISBTACREAD_MASK        0x00002000u
#define SCB_ACTLR_DISBTACREAD_OFFSET      13u
#define SCB_ACTLR_DISBTACALLOC_MASK       0x00004000u
#define SCB_ACTLR_DISBTACALLOC_OFFSET     14u
#define SCB_ACTLR_DISCRITAXIRUR_MASK      0x00008000u
#define SCB_ACTLR_DISCRITAXIRUR_OFFSET    15u
#define SCB_ACTLR_DISDI_MASK              0x001F0000u
#define SCB_ACTLR_DISDI_OFFSET            16u
#define SCB_ACTLR_DISISSCH1_MASK          0x03E00000u
#define SCB_ACTLR_DISISSCH1_OFFSET        21u
#define SCB_ACTLR_DISDYNADD_MASK          0x04000000u
#define SCB_ACTLR_DISDYNADD_OFFSET        26u
#define SCB_ACTLR_DISCRITAXIRUW_MASK      0x08000000u
#define SCB_ACTLR_DISCRITAXIRUW_OFFSET    27u
#define SCB_ACTLR_DISFPUISSOPT_MASK       0x10000000u
#define SCB_ACTLR_DISFPUISSOPT_OFFSET     28u

#define SCB_CPUID_REVISION_MASK           0x0000000Fu
#define SCB_CPUID_REVISION_OFFSET         0u
#define SCB_CPUID_PARTNO_MASK             0x0000FFF0u
#define SCB_CPUID_PARTNO_OFFSET           4u
#define SCB_CPUID_CONSTANT_MASK           0x000F0000u
#define SCB_CPUID_CONSTANT_OFFSET         16u
#define SCB_CPUID_VARIANT_MASK            0x00F00000u
#define SCB_CPUID_VARIANT_OFFSET          20u
#define SCB_CPUID_IMPLEMENTER_MASK        0xFF000000u
#define SCB_CPUID_IMPLEMENTER_OFFSET      24u

#define SCB_ICSR_VECTACTIVE_MASK          0x000001FFu
#define SCB_ICSR_VECTACTIVE_OFFSET        0u
#define SCB_ICSR_RETTOBASE_MASK           0x00000800u
#define SCB_ICSR_RETTOBASE_OFFSET         11u
#define SCB_ICSR_VECTPENDING_MASK         0x001FF000u
#define SCB_ICSR_VECTPENDING_OFFSET       12u
#define SCB_ICSR_ISRPENDING_MASK          0x00400000u
#define SCB_ICSR_ISRPENDING_OFFSET        22u
#define SCB_ICSR_PENDSTCLR_MASK           0x02000000u
#define SCB_ICSR_PENDSTCLR_OFFSET         25u
#define SCB_ICSR_PENDSTSET_MASK           0x04000000u
#define SCB_ICSR_PENDSTSET_OFFSET         26u
#define SCB_ICSR_PENDSVCLR_MASK           0x08000000u
#define SCB_ICSR_PENDSVCLR_OFFSET         27u
#define SCB_ICSR_PENDSVSET_MASK           0x10000000u
#define SCB_ICSR_PENDSVSET_OFFSET         28u
#define SCB_ICSR_NMIPENDSET_MASK          0x80000000u
#define SCB_ICSR_NMIPENDSET_OFFSET        31u

#define SCB_VTOR_TBLOFF_MASK              0xFFFFFF80u
#define SCB_VTOR_TBLOFF_OFFSET            7u

#define SCB_AIRCR_VECTRESET_MASK          0x00000001u
#define SCB_AIRCR_VECTRESET_OFFSET        0u
#define SCB_AIRCR_VECTCLRACTIVE_MASK      0x00000002u
#define SCB_AIRCR_VECTCLRACTIVE_OFFSET    1u
#define SCB_AIRCR_SYSRESETREQ_MASK        0x00000004u
#define SCB_AIRCR_SYSRESETREQ_OFFSET      2u
#define SCB_AIRCR_PRIGROUP_MASK           0x00000700u
#define SCB_AIRCR_PRIGROUP_OFFSET         8u
#define SCB_AIRCR_ENDIANNESS_MASK         0x00008000u
#define SCB_AIRCR_ENDIANNESS_OFFSET       15u
#define SCB_AIRCR_VECTKEY_MASK            0xFFFF0000u
#define SCB_AIRCR_VECTKEY_OFFSET          16u
#define SCB_AIRCR_VECTKEYSTAT_MASK        0xFFFF0000u
#define SCB_AIRCR_VECTKEYSTAT_OFFSET      16u
#define SCB_AIRCR_VECTKEY_WRITE_KEY       UINT32_C(0x5FA)

#define SCB_SCR_SLEEPONEXIT_MASK          0x00000002u
#define SCB_SCR_SLEEPONEXIT_OFFSET        1u
#define SCB_SCR_SLEEPDEEP_MASK            0x00000004u
#define SCB_SCR_SLEEPDEEP_OFFSET          2u
#define SCB_SCR_SEVONPEND_MASK            0x00000010u
#define SCB_SCR_SEVONPEND_OFFSET          4u

#define SCB_CCR_NONBASETHRDENA_MASK       0x00000001u
#define SCB_CCR_NONBASETHRDENA_OFFSET     0u
#define SCB_CCR_USERSETMPEND_MASK         0x00000002u
#define SCB_CCR_USERSETMPEND_OFFSET       1u
#define SCB_CCR_UNALIGN_TRP_MASK          0x00000008u
#define SCB_CCR_UNALIGN_TRP_OFFSET        3u
#define SCB_CCR_DIV_0_TRP_MASK            0x00000010u
#define SCB_CCR_DIV_0_TRP_OFFSET          4u
#define SCB_CCR_BFHFNMIGN_MASK            0x00000100u
#define SCB_CCR_BFHFNMIGN_OFFSET          8u
#define SCB_CCR_STKALIGN_MASK             0x00000200u
#define SCB_CCR_STKALIGN_OFFSET           9u
#define SCB_CCR_DC_MASK                   0x00010000u
#define SCB_CCR_DC_OFFSET                 16u
#define SCB_CCR_IC_MASK                   0x00020000u
#define SCB_CCR_IC_OFFSET                 17u
#define SCB_CCR_BP_MASK                   0x00040000u
#define SCB_CCR_BP_OFFSET                 18u

#define SCB_SHPR1_PRI_4_MASK              0x000000FFu
#define SCB_SHPR1_PRI_4_OFFSET            0u
#define SCB_SHPR1_PRI_5_MASK              0x0000FF00u
#define SCB_SHPR1_PRI_5_OFFSET            8u
#define SCB_SHPR1_PRI_6_MASK              0x00FF0000u
#define SCB_SHPR1_PRI_6_OFFSET            16u

#define SCB_SHPR2_PRI_11_MASK             0xFF000000u
#define SCB_SHPR2_PRI_11_OFFSET           24u

#define SCB_SHPR3_PRI_14_MASK             0x00FF0000u
#define SCB_SHPR3_PRI_14_OFFSET           16u
#define SCB_SHPR3_PRI_15_MASK             0xFF000000u
#define SCB_SHPR3_PRI_15_OFFSET           24u

#define SCB_SHCSR_MEMFAULTACT_MASK        0x00000001u
#define SCB_SHCSR_MEMFAULTACT_OFFSET      0u
#define SCB_SHCSR_BUSFAULTACT_MASK        0x00000002u
#define SCB_SHCSR_BUSFAULTACT_OFFSET      1u
#define SCB_SHCSR_USGFAULTACT_MASK        0x00000008u
#define SCB_SHCSR_USGFAULTACT_OFFSET      3u
#define SCB_SHCSR_SVCALLACT_MASK          0x00000080u
#define SCB_SHCSR_SVCALLACT_OFFSET        7u
#define SCB_SHCSR_MONITORACT_MASK         0x00000100u
#define SCB_SHCSR_MONITORACT_OFFSET       8u
#define SCB_SHCSR_PENDSVACT_MASK          0x00000400u
#define SCB_SHCSR_PENDSVACT_OFFSET        10u
#define SCB_SHCSR_SYSTICKACT_MASK         0x00000800u
#define SCB_SHCSR_SYSTICKACT_OFFSET       11u
#define SCB_SHCSR_USGFAULTPENDED_MASK     0x00001000u
#define SCB_SHCSR_USGFAULTPENDED_OFFSET   12u
#define SCB_SHCSR_MEMFAULTPENDED_MASK     0x00002000u
#define SCB_SHCSR_MEMFAULTPENDED_OFFSET   13u
#define SCB_SHCSR_BUSFAULTPENDED_MASK     0x00004000u
#define SCB_SHCSR_BUSFAULTPENDED_OFFSET   14u
#define SCB_SHCSR_SVCALLPENDED_MASK       0x00008000u
#define SCB_SHCSR_SVCALLPENDED_OFFSET     15u
#define SCB_SHCSR_MEMFAULTENA_MASK        0x00010000u
#define SCB_SHCSR_MEMFAULTENA_OFFSET      16u
#define SCB_SHCSR_BUSFAULTENA_MASK        0x00020000u
#define SCB_SHCSR_BUSFAULTENA_OFFSET      17u
#define SCB_SHCSR_USGFAULTENA_MASK        0x00040000u
#define SCB_SHCSR_USGFAULTENA_OFFSET      18u

#define SCB_CFSR_IACCVIOL_MASK            0x00000001u
#define SCB_CFSR_IACCVIOL_OFFSET          0u
#define SCB_CFSR_DACCVIOL_MASK            0x00000002u
#define SCB_CFSR_DACCVIOL_OFFSET          1u
#define SCB_CFSR_MUNSTKERR_MASK           0x00000008u
#define SCB_CFSR_MUNSTKERR_OFFSET         3u
#define SCB_CFSR_MSTKERR_MASK             0x00000010u
#define SCB_CFSR_MSTKERR_OFFSET           4u
#define SCB_CFSR_MLSPERR_MASK             0x00000020u
#define SCB_CFSR_MLSPERR_OFFSET           5u
#define SCB_CFSR_MMARVALID_MASK           0x00000080u
#define SCB_CFSR_MMARVALID_OFFSET         7u
#define SCB_CFSR_IBUSERR_MASK             0x00000100u
#define SCB_CFSR_IBUSERR_OFFSET           8u
#define SCB_CFSR_PRECISERR_MASK           0x00000200u
#define SCB_CFSR_PRECISERR_OFFSET         9u
#define SCB_CFSR_IMPRECISERR_MASK         0x00000400u
#define SCB_CFSR_IMPRECISERR_OFFSET       10u
#define SCB_CFSR_UNSTKERR_MASK            0x00000800u
#define SCB_CFSR_UNSTKERR_OFFSET          11u
#define SCB_CFSR_STKERR_MASK              0x00001000u
#define SCB_CFSR_STKERR_OFFSET            12u
#define SCB_CFSR_LSPERR_MASK              0x00002000u
#define SCB_CFSR_LSPERR_OFFSET            13u
#define SCB_CFSR_BFARVALID_MASK           0x00008000u
#define SCB_CFSR_BFARVALID_OFFSET         15u
#define SCB_CFSR_UNDEFINSTR_MASK          0x00010000u
#define SCB_CFSR_UNDEFINSTR_OFFSET        16u
#define SCB_CFSR_INVSTATE_MASK            0x00020000u
#define SCB_CFSR_INVSTATE_OFFSET          17u
#define SCB_CFSR_INVPC_MASK               0x00040000u
#define SCB_CFSR_INVPC_OFFSET             18u
#define SCB_CFSR_NOCP_MASK                0x00080000u
#define SCB_CFSR_NOCP_OFFSET              19u
#define SCB_CFSR_UNALIGNED_MASK           0x01000000u
#define SCB_CFSR_UNALIGNED_OFFSET         24u
#define SCB_CFSR_DIVBYZERO_MASK           0x02000000u
#define SCB_CFSR_DIVBYZERO_OFFSET         25u

#define SCB_MMSR_IACCVIOL_MASK            0x01u
#define SCB_MMSR_IACCVIOL_OFFSET          0u
#define SCB_MMSR_DACCVIOL_MASK            0x02u
#define SCB_MMSR_DACCVIOL_OFFSET          1u
#define SCB_MMSR_MUNSTKERR_MASK           0x08u
#define SCB_MMSR_MUNSTKERR_OFFSET         3u
#define SCB_MMSR_MSTKERR_MASK             0x10u
#define SCB_MMSR_MSTKERR_OFFSET           4u
#define SCB_MMSR_MLSPERR_MASK             0x20u
#define SCB_MMSR_MLSPERR_OFFSET           5u
#define SCB_MMSR_MMARVALID_MASK           0x80u
#define SCB_MMSR_MMARVALID_OFFSET         7u

#define SCB_BFSR_IBUSERR_MASK             0x01u
#define SCB_BFSR_IBUSERR_OFFSET           0u
#define SCB_BFSR_PRECISERR_MASK           0x02u
#define SCB_BFSR_PRECISERR_OFFSET         1u
#define SCB_BFSR_IMPRECISERR_MASK         0x04u
#define SCB_BFSR_IMPRECISERR_OFFSET       2u
#define SCB_BFSR_UNSTKERR_MASK            0x08u
#define SCB_BFSR_UNSTKERR_OFFSET          3u
#define SCB_BFSR_STKERR_MASK              0x10u
#define SCB_BFSR_STKERR_OFFSET            4u
#define SCB_BFSR_LSPERR_MASK              0x20u
#define SCB_BFSR_LSPERR_OFFSET            5u
#define SCB_BFSR_BFARVALID_MASK           0x80u
#define SCB_BFSR_BFARVALID_OFFSET         7u

#define SCB_UFSR_UNDEFINSTR_MASK          0x0001u
#define SCB_UFSR_UNDEFINSTR_OFFSET        0u
#define SCB_UFSR_INVSTATE_MASK            0x0002u
#define SCB_UFSR_INVSTATE_OFFSET          1u
#define SCB_UFSR_INVPC_MASK               0x0004u
#define SCB_UFSR_INVPC_OFFSET             2u
#define SCB_UFSR_NOCP_MASK                0x0008u
#define SCB_UFSR_NOCP_OFFSET              3u
#define SCB_UFSR_UNALIGNED_MASK           0x0100u
#define SCB_UFSR_UNALIGNED_OFFSET         8u
#define SCB_UFSR_DIVBYZERO_MASK           0x0200u
#define SCB_UFSR_DIVBYZERO_OFFSET         9u

#define SCB_HFSR_VECTTBL_MASK             0x00000002u
#define SCB_HFSR_VECTTBL_OFFSET           1u
#define SCB_HFSR_FORCED_MASK              0x40000000u
#define SCB_HFSR_FORCED_OFFSET            30u
#define SCB_HFSR_DEBUGEVT_MASK            0x80000000u
#define SCB_HFSR_DEBUGEVT_OFFSET          31u

#define SCB_MMAR_ADDRESS_MASK             0xFFFFFFFFu
#define SCB_MMAR_ADDRESS_OFFSET           0u

#define SCB_BFAR_ADDRESS_MASK             0xFFFFFFFFu
#define SCB_BFAR_ADDRESS_OFFSET           0u

#define SCB_CCSIDR_LINESIZE_MASK          0x00000007u
#define SCB_CCSIDR_LINESIZE_OFFSET        0u
#define SCB_CCSIDR_ASSOCIATIVITY_MASK     0x00001FF8u
#define SCB_CCSIDR_ASSOCIATIVITY_OFFSET   3u
#define SCB_CCSIDR_NUMSETS_MASK           0x0FFFE000u
#define SCB_CCSIDR_NUMSETS_OFFSET         13u
#define SCB_CCSIDR_WRITEALLOC_MASK        0x10000000u
#define SCB_CCSIDR_WRITEALLOC_OFFSET      28u
#define SCB_CCSIDR_READALLOC_MASK         0x20000000u
#define SCB_CCSIDR_READALLOC_OFFSET       29u
#define SCB_CCSIDR_WRITEBACK_MASK         0x40000000u
#define SCB_CCSIDR_WRITEBACK_OFFSET       30u
#define SCB_CCSIDR_WRITETHROUGH_MASK      0x80000000u
#define SCB_CCSIDR_WRITETHROUGH_OFFSET    31u

#define SCB_DCISW_ZERO_MASK               0x00000001u
#define SCB_DCISW_ZERO_OFFSET             0u
#define SCB_DCISW_SET_MASK                0x00003FE0u
#define SCB_DCISW_SET_OFFSET              5u
#define SCB_DCISW_WAY_MASK                0xC0000000u
#define SCB_DCISW_WAY_OFFSET              30u

#define SCB_DCCSW_ZERO_MASK               0x00000001u
#define SCB_DCCSW_ZERO_OFFSET             0u
#define SCB_DCCSW_SET_MASK                0x00003FE0u
#define SCB_DCCSW_SET_OFFSET              5u
#define SCB_DCCSW_WAY_MASK                0xC0000000u
#define SCB_DCCSW_WAY_OFFSET              30u

#define SCB_DCCISW_ZERO_MASK              0x00000001u
#define SCB_DCCISW_ZERO_OFFSET            0u
#define SCB_DCCISW_SET_MASK               0x00003FE0u
#define SCB_DCCISW_SET_OFFSET             5u
#define SCB_DCCISW_WAY_MASK               0xC0000000u
#define SCB_DCCISW_WAY_OFFSET             30u

#define SCB_CACHE_LINE_SIZE               32u

// clang-format on

#endif // BSP_SCB_REGISTERS_H
