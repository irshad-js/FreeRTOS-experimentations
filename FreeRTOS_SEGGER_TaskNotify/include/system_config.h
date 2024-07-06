/**
 *****************************************************************************************
 *
 * @file system_config.h
 *
 * @brief Definition of system config paramters.
 *
 * Copyright (C) 2017. Dialog Semiconductor, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor. All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 *
 *****************************************************************************************
 */

#ifndef SYSTEM_CONFIG_H_
#define SYSTEM_CONFIG_H_

#include "core_cm33.h"                            /*!< Cortex-M33 processor and core peripherals*/
#include <stdbool.h>
#include <stdint.h>
#include "D2522.h"
#ifndef   __STATIC_INLINE
         #define __STATIC_INLINE                        static inline
#endif

__STATIC_INLINE void ResetWD()
{
        //no need to check the busy bit, since we are writing the same value every time. (As confirmed by Jakko)
        SYS_WDOG->WATCHDOG_REG = 0xFF;

}

void HW_RESET(void);

#define TIMEOUT_2MS 20 //20 * 100uS
#define TIMEOUT_10MS 100 //100 * 100uS

#define SYSRAM_BASE_ADDRESS             0x800000
#define QSPI_FLASH_BASE_ADDRESS         0x16000000 // QSPI base address (from CPU point of view)
#define QSPI_FLASH_BASE_ADDRESS_P       0x36000000 // QSPI base address (from peripherals point of view)
#define QSPIF_C(_flash_address)         ((_flash_address) + QSPI_FLASH_BASE_ADDRESS)
#define QSPIF_P(_flash_address)         ((_flash_address) + QSPI_FLASH_BASE_ADDRESS_P)

/*--GPIO PID functions-------------------------------------------------------------------------*/
#define FUNC_GPIO              (0)
#define FUNC_UART_RX           (1)
#define FUNC_UART_TX           (2)

/*--GPIO Direction settings---------------------------------------------------------------------*/
#define DIR_INPUT              0x000
#define DIR_PULLUP             0x100
#define DIR_PULLDOWN           0x200
#define DIR_OUTPUT             0x300

 typedef enum {
        BOOT_FROM_ROM   	= 0,
        BOOT_FROM_OTP_UNCACHED  = 1,
        BOOT_FROM_QSPI_CACHED   = 2,
        BOOT_FROM_RAMS  	= 3,
        BOOT_FROM_QSPI_UNCACHED = 4,
} REMAP_BOOTMODE;

typedef struct {
        bool  dev_mode_flag;
        bool flash_found;
        uint32_t prod_header_addr;
        uint32_t timer_exp_counter;
        uint32_t stx_timing;
        bool timeout_occured;
        bool xtal_ok;
} booter_config;

/**
 * \brief Access register field mask.
 *
 * Returns a register field mask (aimed to be used with local variables).
 * e.g.
 * \code
 * uint16_t tmp;
 *
 * tmp = CRG_TOP->SYS_STAT_REG;
 *
 * if (tmp & REG_MSK(CRG_TOP, SYS_STAT_REG, XTAL16_TRIM_READY)) {
 * ...
 * \endcode
 */
#define REG_MSK(base, reg, field) \
        (base ## _ ## reg ## _ ## field ## _Msk)

/**
 * \brief Access register field position.
 *
 * Returns a register field position (aimed to be used with local variables).
 */
#define REG_POS(base, reg, field) \
        (base ## _ ## reg ## _ ## field ## _Pos)

/**
 * \brief Access register field value.
 *
 * Returns a register field value (aimed to be used with local variables).
 * e.g.
 * \code
 * uint16_t tmp;
 * int counter;
 * tmp = CRG_TOP->TRIM_CTRL_REG;
 * counter = REG_GET_FIELD(CRG_TOP, TRIM_CTRL_REG, XTAL_COUNT_N, tmp);
 * ...
 * \endcode
 */
#define REG_GET_FIELD(base, reg, field, var) \
        ((var & (base ## _ ## reg ## _ ## field ## _Msk)) >> \
                (base ## _ ## reg ## _ ## field ## _Pos))

/**
 * \brief Set register field value.
 *
 * Sets a register field value (aimed to be used with local variables).
 * e.g.
 * \code
 * uint16_t tmp;
 *
 * tmp = CRG_TOP->TRIM_CTRL_REG;
 * REG_SET_FIELD(CRG_TOP, TRIM_CTRL_REG, XTAL_COUNT_N, tmp, 10);
 * REG_SET_FIELD(CRG_TOP, TRIM_CTRL_REG, XTAL_TRIM_SELECT, tmp, 2);
 * CRG_TOP->TRIM_CTRL_REG = tmp;
 * ...
 * \endcode
 */
#define REG_SET_FIELD(base, reg, field, var, val) \
        var = ((var & ~((base ## _ ## reg ## _ ## field ## _Msk))) | \
                (((val) << (base ## _ ## reg ## _ ## field ## _Pos)) & \
                (base ## _ ## reg ## _ ## field ## _Msk)))

/**
 * \brief Clear register field value.
 *
 * Clears a register field value (aimed to be used with local variables).
 * e.g.
 * \code
 * uint16_t tmp;
 *
 * tmp = CRG_TOP->TRIM_CTRL_REG;
 * REG_CLR_FIELD(CRG_TOP, TRIM_CTRL_REG, XTAL_COUNT_N, tmp);
 * REG_CLR_FIELD(CRG_TOP, TRIM_CTRL_REG, XTAL_TRIM_SELECT, tmp);
 * CRG_TOP->TRIM_CTRL_REG = tmp;
 * ...
 * \endcode
 */
#define REG_CLR_FIELD(base, reg, field, var) \
        var &= ~(base ## _ ## reg ## _ ## field ## _Msk)

/**
 * \brief Get the address of a register value by index (provided a register interval)
 *
 * \note The register interval should be an exact multiple of the register's base size. For example,
 * if the register size is 32-bit, then the interval should be 0x4, 0x8, etc. Otherwise, the result
 * will be undefined. The interval value must be in bytes. The index value (0,1,2...) is multiplied by
 * the interval value (in bytes) to find the actual offset of the register.
 *
 * Returns a register address value by index
 */
#define REG_GET_ADDR_INDEXED(base, reg, interval, index) \
        ((&base->reg) + (((intptr_t) index) * ((interval) / sizeof(base->reg))))

/**
 * \brief Return the value of a register field by index (provided a register interval).
 *
 * e.g.
 * \code
 * uint16_t val;
 * uint16_t index = 2
 *
 * val = REG_GETF_INDEXED(FTDF, FTDF_LONG_ADDR_0_0_REG, REG_EXP_SA_L, 0x10, index)
 *
 * ...
 * \endcode
 *
 * \note The register interval should be an exact multiple of the register's base size. For example,
 * if the register size is 32-bit, then the interval should be 0x4, 0x8, etc. Otherwise, the result
 * will be undefined. The interval value must be in bytes. The index value (0,1,2...) is multiplied by
 * the interval value (in bytes) to find the actual offset of the register.
 *
 */
#define REG_GETF_INDEXED(base, reg, field, interval, index) \
        (((*REG_GET_ADDR_INDEXED(base, reg, interval, index)) & \
            (base ## _ ## reg ## _ ## field ## _Msk)) >> (base ## _ ## reg ## _ ## field ## _Pos))

/**
 * \brief Return the value of a register field.
 *
 * e.g.
 * \code
 * uint16_t val;
 *
 * val = REG_GETF(CRG_TOP, TRIM_CTRL_REG, XTAL_COUNT_N);
 * ...
 * \endcode
 */
#define REG_GETF(base, reg, field) \
        (((base->reg) & (base##_##reg##_##field##_Msk)) >> (base##_##reg##_##field##_Pos))

/**
 * \brief Set the value of a register field.
 *
 * e.g.
 * \code
 *
 * REG_SETF(CRG_TOP, TRIM_CTRL_REG, XTAL_COUNT_N, new_value);
 * ...
 * \endcode
 */
#define REG_SETF(base, reg, field, new_val) \
                base->reg = ((base->reg & ~(base##_##reg##_##field##_Msk)) | \
                ((base##_##reg##_##field##_Msk) & ((new_val) << (base##_##reg##_##field##_Pos))))

/**
 * \brief Set a bit of a register.
 *
 * e.g.
 * \code
 *
 * REG_SET_BIT(CRG_TOP, CLK_TMR_REG, TMR1_ENABLE);
 * ...
 * \endcode
 */
#define REG_SET_BIT(base, reg, field) \
        do { \
                base->reg |= (1 << (base##_##reg##_##field##_Pos)); \
         } while (0)

/**
 * \brief Clear a bit of a register.
 *
 * e.g.
 * \code
 *
 * REG_CLR_BIT(CRG_TOP, CLK_TMR_REG, TMR1_ENABLE);
 * ...
 * \endcode
 */
#define REG_CLR_BIT(base, reg, field) \
        do { \
                base->reg &= ~(base##_##reg##_##field##_Msk); \
         } while (0)

/**
 * \brief Sets register bits, indicated by the mask, to a value.
 *
 * e.g.
 * \code
 * REG_SET_MASKED(RFCU_POWER, RF_CNTRL_TIMER_5_REG, 0xFF00, 0x1818);
 * \endcode
 */
#define REG_SET_MASKED(base, reg, mask, value) \
        do { \
                base->reg = (base->reg & ~(mask)) | ((value) & (mask)); \
        } while (0)

/**
 * \brief Sets 16-bit wide register bits, indicated by the field, to a value v.
 */
#define BITS16(base, reg, field, v) \
        ((uint16) (((uint16) (v) << (base ## _ ## reg ## _ ## field ## _Pos)) & \
                (base ## _ ## reg ## _ ## field ## _Msk)))

/**
 * \brief Sets 32-bit wide register bits, indicated by the field, to a value v.
 */
#define BITS32(base, reg, field, v) \
        ((uint32) (((uint32) (v) << (base ## _ ## reg ## _ ## field ## _Pos)) & \
                (base ## _ ## reg ## _ ## field ## _Msk)))

/**
 * \brief Reads 16-bit wide register bits, indicated by the field, to a variable v.
 */
#define GETBITS16(base, reg, v, field) \
        ((uint16) (((uint16) (v)) & (base ## _ ## reg ## _ ## field ## _Msk)) >> \
                (base ## _ ## reg ## _ ## field ## _Pos))

/**
 * \brief Reads 32-bit wide register bits, indicated by the field, to a variable v.
 */
#define GETBITS32(base, reg, v, field) \
        ((uint32) (((uint32) (v)) & (base ## _ ## reg ## _ ## field ## _Msk)) >> \
                (base ## _ ## reg ## _ ## field ## _Pos))

__STATIC_INLINE void Switch_to_RC32M()
{
       REG_SETF(CRG_TOP,CLK_RC32M_REG,RC32M_ENABLE,1); // Enable RC32M
       REG_SETF(CRG_TOP,CLK_CTRL_REG,SYS_CLK_SEL,1);   // make sure sys clk is RC32M (incase of sw_reset to bootrom while running on XTAL)
       while (REG_GETF(CRG_TOP, CLK_CTRL_REG, RUNNING_AT_RC32M) == 0) ;
}

__STATIC_INLINE void Switch_to_XTAL32()
{
        while (REG_GETF(CRG_XTAL, XTAL32M_STAT1_REG, XTAL32M_SETTLE_READY) == 0) ;
        REG_SETF(CRG_TOP, CLK_CTRL_REG, SYS_CLK_SEL, 0);
        while (REG_GETF(CRG_TOP, CLK_CTRL_REG, RUNNING_AT_XTAL32M) == 0) ;
}

#endif // SYSTEM_CONFIG_H_
