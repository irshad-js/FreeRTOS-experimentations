/****************************************************************************************
 *
 * @file dev_admin.h
 *
 * @brief Definition of device administration and validation functions
 *
 * Copyright (C) 2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************/

#ifndef DEV_ADMIN_H_
#define DEV_ADMIN_H_

#include "system_config.h"
#include "headers.h"

#define KEY_INDEX_REVOKED 0
#define KEY_INDEX_VALID 0xFFFFFFFF

/**
 * \brief Check for upgrade
 *
 * Returns true if an ongoing upgrade procedure is detected, false otherwise.
 */
bool Check_for_upgrade(product_header_data_t*);

/**
 * \brief Firmware validation
 *
 * if security is enabled, verify the image and return the result.
 */
bool FW_validation(uint32_t, image_header_data_t**, product_header_data_t*, booter_config *);

/**
 * \brief Reject upgrade
 *
 * reject the upgrade by modifying the flash header
 */
void Reject_upgrade(booter_config *bc, product_header_data_t *ph);
void Accept_upgrade(booter_config *bc, product_header_data_t *ph);
void copy_prodheader_to_ram(booter_config *bc, product_header_layout_ram *prod_header_ram, uint8_t *header_size);
void program_prodheader_to_qspi(booter_config *bc, product_header_layout_ram *prod_header_ram, uint8_t *header_size, ph_type);
void Device_administration(image_header_data_t *img);
bool key_is_revoked(uint8_t key_type, uint8_t key_index);

#endif
