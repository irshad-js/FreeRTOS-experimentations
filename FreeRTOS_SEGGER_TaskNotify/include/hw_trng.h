/**
 *****************************************************************************************
 *
 * @file hw_trng.h
 *
 * @brief Definition TRNG functions.
 *
 * Copyright (C) 2017. Dialog Semiconductor, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor. All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 *
 *****************************************************************************************
 */

#ifndef HW_TRNG_H_
#define HW_TRNG_H_

#include <stdint.h>
#include "system_config.h"
#include "core_cm33.h"                            /*!< Cortex-M33 processor and core peripherals*/

#define MEMORY_TRNG_FIFO        0x40040000UL

/**
 * \brief TRNG callback.
 *
 * This function is called by the TRNG driver when the interrupt is fired.
 *
 * \note If the TRNG is not needed anymore the hw_trng_disable function should be called in the
 * callback function to save power.
 *
 */
typedef void (*hw_trng_cb)(void);

/**
 * \brief TRNG enable.
 *
 * This function enables the TRNG. If callback is not NULL it will be called when a TRNG interrupt
 * occurs. The interrupt is triggered when the TRNG FIFO is full.
 *
 * \param [in] callback The callback function that is called when a TRNG interrupt occurs.
 *
 * \note If the amount of random numbers needed is less than the contents of the FIFO it is faster
 * and more power efficient to use a wait loop in combination with the hw_trng_get_fifo_level
 * function. If the FIFO has the required level use the hw_trng_get_numbers function to get the
 * random numbers.
 *
 */
void hw_trng_enable(hw_trng_cb callback);

/**
 * \brief Get a random number from TRNG.
 *
 * This function reads a random number from the TRNG FIFO.
 *
 * \return A 32-bit unsigned random number.
 *
 * \warning This function does not check for number availability in the FIFO
 *
 */
__attribute__((always_inline)) __STATIC_INLINE uint32_t hw_trng_get_number(void)
{
        return *((volatile uint32_t *)MEMORY_TRNG_FIFO);
}


/**
 * \brief TRNG get FIFO level.
 *
 * This function returns the current level of the TRNG FIFO.
 *
 * \return The current level of the TRNG FIFO.
 *
 */
uint8_t hw_trng_get_fifo_level(void);

/**
 * \brief TRNG disable.
 *
 * This function stops TRNG, disables its clock and its interrupt.
 *
 */
void hw_trng_disable(void);


#endif /* HW_TRNG_H_ */
