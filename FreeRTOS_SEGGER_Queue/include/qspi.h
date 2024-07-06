/**
 *****************************************************************************************
 *
 * @file qspi.h
 *
 * @brief Definition of API for the qspi Low Level Driver.
 *
 * Copyright (C) 2017. Dialog Semiconductor, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor. All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 *
 *****************************************************************************************
 */
#ifndef _QSPI_H
#define _QSPI_h

#include <stddef.h>
#include "system_config.h"

#define QSPI_STATREG_PR_CMD     0x01
#define QSPI_READ_CMD           0x03
#define QSPI_PAGEPR_CMD         0x02
#define QSPI_STATREG_R_CMD      0x05
#define QSPI_WREN_CMD           0x06
#define QSPI_SECTOR_ERASE_CMD   0x20
#define QSPI_RESET_EN_CMD       0x66
#define QSPI_RESET_CMD          0x99
#define QSPI_EXIT_DPD_CMD       0xAB

#define FLASH_SECTOR_SIZE       0x1000 // 4KB
#define FLASH_PAGE_SIZE         0x100  // 256 bytes


void qspi_init(void);
void qspi_reset(void);
int qspi_loader(booter_config *bc);

void qspi_set_bus_mode(uint8_t mode);
void qspi_flash_wr(uint8_t);
void qspi_flash_page_wr(uint32_t address, uint8_t* data, uint8_t length);
void qspi_flash_sector_erase(uint32_t address);
void qspi_flash_cfg_read(uint8_t rd_cmd, uint32_t address);
void qspi_flash_rd_buf(uint8_t* rd_buf, size_t rd_buf_size);
uint8_t qspi_flash_rd(void);
uint8_t qspi_flash_rd_addr(uint32_t);
void qspi_flash_rd_addr_buf(uint32_t address, uint8_t *rd_buf, size_t rd_buf_size);

/**
* \brief  Chip Select Disable + Enable
*
* \param  [in] void
*
*/
__STATIC_INLINE void close_open_qspi(void)
{
        QSPIC->QSPIC_CTRLBUS_REG =   QSPIC_QSPIC_CTRLBUS_REG_QSPIC_DIS_CS_Msk;
        QSPIC->QSPIC_CTRLBUS_REG =  QSPIC_QSPIC_CTRLBUS_REG_QSPIC_EN_CS_Msk;
}

/**
* \brief  Chip Select Enable
*
* \param  [in] void
*
*/
__STATIC_INLINE void open_qspi(void)
{
        QSPIC->QSPIC_CTRLBUS_REG =  QSPIC_QSPIC_CTRLBUS_REG_QSPIC_EN_CS_Msk;
}

/**
* \brief  Chip Select Disable
*
* \param  [in] void
*
*
*/
__STATIC_INLINE void close_qspi(void)
{
        QSPIC->QSPIC_CTRLBUS_REG =   QSPIC_QSPIC_CTRLBUS_REG_QSPIC_DIS_CS_Msk;
}

#endif

