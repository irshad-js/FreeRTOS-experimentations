/**************************************************************************//**
 * @file     startup_ARMCM33.s
 * @brief    CMSIS Core Device Startup File for
 *           ARMCM33 Device Series
 * @version  V5.00
 * @date     21. October 2016
 ******************************************************************************/
/*
 * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

	.syntax	unified
	.arch	armv7-m

	.section .stack
	.align	3
#ifdef __STACK_SIZE
	.equ	Stack_Size, __STACK_SIZE
#else
	.equ	Stack_Size, 0x00000400
#endif
	.globl	__StackTop
	.globl	__StackLimit
__StackLimit:
	.space	Stack_Size
	.size	__StackLimit, . - __StackLimit
__StackTop:
	.size	__StackTop, . - __StackTop

	.section .heap
	.align	3
#ifdef __HEAP_SIZE
	.equ	Heap_Size, __HEAP_SIZE
#else
	.equ	Heap_Size, 0x00000C00
#endif
	.globl	__HeapBase
	.globl	__HeapLimit
__HeapBase:
	.if	Heap_Size
	.space	Heap_Size
	.endif
	.size	__HeapBase, . - __HeapBase
__HeapLimit:
	.size	__HeapLimit, . - __HeapLimit

	.section .vectors
	.align 2
	.globl	__Vectors
__Vectors:
.long   __StackTop              /* Top of Stack */
    .long   Reset_Handler           /* Reset Handler */
    .long   NMI_Handler             /* NMI Handler */
    .long   HardFault_Handler       /* Hard Fault Handler */
    .long   MemManage_Handler       /* MPU Fault Handler */
    .long   BusFault_Handler        /* Bus Fault Handler */
    .long   UsageFault_Handler      /* Usage Fault Handler */
    .long   SecureFault_Handler     /* Secure Fault Handler */
    .long   0                       /* Reserved */
    .long   0                       /* Reserved */
    .long   0                       /* Reserved */
    .long   SVC_Handler             /* SVCall Handler */
    .long   DebugMon_Handler        /* Debug Monitor Handler */
    .long   0                       /* Reserved */
    .long   PendSV_Handler          /* PendSV Handler */
    .long   SysTick_Handler         /* SysTick Handler */

    /* External interrupts */
    .long   Sensor_Node_Handler             /*  0: Sensor Node Controller        */
    .long   DMA_Handler                     /*  1: General Purpose DMA           */
    .long   Charger_Error_Handler           /*  2: Charger Error                 */
    .long   Charger_JEITA_FSM_Handler       /*  3: Charger/JEITA FSM             */
    .long   CMAC2SYS_Handler                /*  4: CMAC and mailbox              */
    .long   UART_Handler                    /*  5: UART                          */
    .long   UART2_Handler                   /*  6: UART2                         */
    .long   UART3_Handler                   /*  7: UART3                         */
    .long   I2C_Handler                     /*  8: I2C                           */
    .long   I2C2_Handler                    /*  9: I2C2                          */
    .long   SPI_Handler                     /* 10: SPI                           */
    .long   SPI2_Handler                    /* 11: SPI2                          */
    .long   PCM_Handler                     /* 12: PCM                           */
    .long   SRC_In_Handler                  /* 13: SRC Input                     */
    .long   SRC_Out_Handler                 /* 14: SRC Output                    */
    .long   USB_Handler                     /* 15: USB                           */
    .long   Timer_Handler                   /* 16: Timer                         */
    .long   Timer2_Handler                  /* 17: Timer2                        */
    .long   RTC_Handler                     /* 18: RTC                           */
    .long   Key_Wkup_GPIO_Handler           /* 19: Debounced button pressed      */
    .long   M33_Wkup_GPIO_Handler           /* 20: GPIO toggle                   */
    .long   VBUS_Handler                    /* 21: VBUS Presence                 */
    .long   MRM_Handler                     /* 22: Cache Miss Rate Monitor       */
    .long   Motor_Controller_Handler        /* 23: Motor and mailbox             */
    .long   TRNG_Handler                    /* 24: True Random Number Generation */
    .long   DCDC_Handler                    /* 25: DC-DC timeout expiration      */
    .long   XTAL32M_Ready_Handler           /* 26: XTAL32M osc trimmed & settled */
    .long   ADC_Handler                     /* 27: Analog-Digital Converter      */
    .long   ADC2_Handler                    /* 28: Analog-Digital Converter 2    */
    .long   Crypto_Handler                  /* 29: Crypto (AES or HASH)          */
    .long   RFCAL_Handler                   /* 30: RF Calibration (muxed)        */
    .long   RFDIAG_Handler                  /* 31: Baseband/Radio diagnostics    */
    .long   LCD_Controller_Handler          /* 32: Parallel LCD Controller       */
    .long   PLL_Lock_Handler                /* 33: DLL/PLL locked @96MHz         */
    .long   Timer3_Handler                  /* 34: Timer3                        */
    .long   Timer4_Handler                  /* 35: Timer4                        */
    .long   Reserver36_Handler              /* 36:                               */
    .long   Reserver37_Handler              /* 37:                               */
    .long   Reserver38_Handler              /* 38:                               */
    .long   Reserver39_Handler              /* 39:                               */
    .long   Reserver40_Handler              /* 40:                               */
    .long   Reserver41_Handler              /* 41:                               */
    .long   Reserver42_Handler              /* 42:                               */
    .long   Reserver43_Handler              /* 43:                               */
    .long   Reserver44_Handler              /* 44:                               */
    .long   Reserver45_Handler              /* 45:                               */
    .long   Reserver46_Handler              /* 46:                               */
    .long   Reserver47_Handler              /* 47:                               */
	.size	__Vectors, . - __Vectors

	.text
	.thumb
	.thumb_func
	.align	2
	.globl	Reset_Handler
	.type	Reset_Handler, %function
Reset_Handler:
/*  Firstly it copies data from read only memory to RAM. There are two schemes
 *  to copy. One can copy more than one sections. Another can only copy
 *  one section.  The former scheme needs more instructions and read-only
 *  data to implement than the latter.
 *  Macro __STARTUP_COPY_MULTIPLE is used to choose between two schemes.  */

#ifdef __STARTUP_COPY_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of triplets, each of which specify:
 *    offset 0: LMA of start of a section to copy from
 *    offset 4: VMA of start of a section to copy to
 *    offset 8: size of the section to copy. Must be multiply of 4
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
	ldr	r4, =__copy_table_start__
	ldr	r5, =__copy_table_end__

.L_loop0:
	cmp	r4, r5
	bge	.L_loop0_done
	ldr	r1, [r4]
	ldr	r2, [r4, #4]
	ldr	r3, [r4, #8]

.L_loop0_0:
	subs	r3, #4
	ittt	ge
	ldrge	r0, [r1, r3]
	strge	r0, [r2, r3]
	bge	.L_loop0_0

	adds	r4, #12
	b	.L_loop0

.L_loop0_done:
#else
/*  Single section scheme.
 *
 *  The ranges of copy from/to are specified by following symbols
 *    __etext: LMA of start of the section to copy from. Usually end of text
 *    __data_start__: VMA of start of the section to copy to
 *    __data_end__: VMA of end of the section to copy to
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
	ldr	r1, =__etext
	ldr	r2, =__data_start__
	ldr	r3, =__data_end__

.L_loop1:
	cmp	r2, r3
	ittt	lt
	ldrlt	r0, [r1], #4
	strlt	r0, [r2], #4
	blt	.L_loop1
#endif /*__STARTUP_COPY_MULTIPLE */

/*  This part of work usually is done in C library startup code. Otherwise,
 *  define this macro to enable it in this startup.
 *
 *  There are two schemes too. One can clear multiple BSS sections. Another
 *  can only clear one section. The former is more size expensive than the
 *  latter.
 *
 *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 *  Otherwise efine macro __STARTUP_CLEAR_BSS to choose the later.
 */
#define __STARTUP_CLEAR_BSS

#ifdef __STARTUP_CLEAR_BSS_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of tuples specifying:
 *    offset 0: Start of a BSS section
 *    offset 4: Size of this BSS section. Must be multiply of 4
 */
	ldr	r3, =__zero_table_start__
	ldr	r4, =__zero_table_end__

.L_loop2:
	cmp	r3, r4
	bge	.L_loop2_done
	ldr	r1, [r3]
	ldr	r2, [r3, #4]
	movs	r0, 0

.L_loop2_0:
	subs	r2, #4
	itt	ge
	strge	r0, [r1, r2]
	bge	.L_loop2_0

	adds	r3, #8
	b	.L_loop2
.L_loop2_done:
#elif defined (__STARTUP_CLEAR_BSS)
/*  Single BSS section scheme.
 *
 *  The BSS section is specified by following symbols
 *    __bss_start__: start of the BSS section.
 *    __bss_end__: end of the BSS section.
 *
 *  Both addresses must be aligned to 4 bytes boundary.
 */
	ldr	r1, =__bss_start__
	ldr	r2, =__bss_end__

	movs	r0, 0
.L_loop3:
	cmp	r1, r2
	itt	lt
	strlt	r0, [r1], #4
	blt	.L_loop3
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

#ifndef __NO_SYSTEM_INIT
	bl	SystemInit
#endif

#ifndef __START
#define __START _start
#endif
	bl	__START

	.pool
	.size	Reset_Handler, . - Reset_Handler

	.align	1
	.thumb_func
	.weak	Default_Handler
	.type	Default_Handler, %function
Default_Handler:
	b	.
	.size	Default_Handler, . - Default_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
	.macro	def_irq_handler	handler_name
	.weak	\handler_name
	.set	\handler_name, Default_Handler
	.endm

	.thumb
	.thumb_func
	.align	1
	.weak	SVC_Handler
	.type	SVC_Handler, %function
SVC_Handler:
	b	.
	.size	SVC_Handler, . - SVC_Handler
	def_irq_handler	NMI_Handler
	def_irq_handler	HardFault_Handler
    def_irq_handler MemManage_Handler
    def_irq_handler BusFault_Handler
    def_irq_handler UsageFault_Handler
    def_irq_handler SecureFault_Handler
    def_irq_handler DebugMon_Handler
    def_irq_handler PendSV_Handler
    def_irq_handler SysTick_Handler
	def_irq_handler Sensor_Node_Handler
    def_irq_handler DMA_Handler
    def_irq_handler Charger_Error_Handler
    def_irq_handler Charger_JEITA_FSM_Handler
    def_irq_handler CMAC2SYS_Handler
    def_irq_handler UART_Handler
    def_irq_handler UART2_Handler
    def_irq_handler UART3_Handler
    def_irq_handler I2C_Handler
    def_irq_handler I2C2_Handler
    def_irq_handler SPI_Handler
    def_irq_handler SPI2_Handler
    def_irq_handler PCM_Handler
    def_irq_handler SRC_In_Handler
    def_irq_handler SRC_Out_Handler
    def_irq_handler USB_Handler
    def_irq_handler Timer_Handler
    def_irq_handler Timer2_Handler
    def_irq_handler RTC_Handler
    def_irq_handler Key_Wkup_GPIO_Handler
    def_irq_handler M33_Wkup_GPIO_Handler
    def_irq_handler VBUS_Handler
    def_irq_handler MRM_Handler
    def_irq_handler Motor_Controller_Handler
    def_irq_handler TRNG_Handler
    def_irq_handler DCDC_Handler
    def_irq_handler XTAL32M_Ready_Handler
    def_irq_handler ADC_Handler
    def_irq_handler ADC2_Handler
    def_irq_handler Crypto_Handler
    def_irq_handler RFCAL_Handler
    def_irq_handler RFDIAG_Handler
    def_irq_handler LCD_Controller_Handler
    def_irq_handler PLL_Lock_Handler
    def_irq_handler Timer3_Handler
    def_irq_handler Timer4_Handler
    def_irq_handler Reserver36_Handler
    def_irq_handler Reserver37_Handler
    def_irq_handler Reserver38_Handler
    def_irq_handler Reserver39_Handler
    def_irq_handler Reserver40_Handler
    def_irq_handler Reserver41_Handler
    def_irq_handler Reserver42_Handler
    def_irq_handler Reserver43_Handler
    def_irq_handler Reserver44_Handler
    def_irq_handler Reserver45_Handler
    def_irq_handler Reserver46_Handler
    def_irq_handler Reserver47_Handler

	.end
