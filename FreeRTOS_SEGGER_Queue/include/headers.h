/****************************************************************************************
 *
 * @file headers.h
 *
 * @brief Definition of header (product and image) manipulation functions
 *
 * Copyright (C) 2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************/

#ifndef HEADERS_H_
#define HEADERS_H_

#include "system_config.h"

#define FLASH_CFG_SECTION_TLV_ID        0x11AA
#define SECURITY_SECTION_TLV_ID         0x22AA
#define SIGNATURE_SECTION_TLV_ID        0x33AA
#define DEVICE_ADMIN_SECTION_TLV_ID     0x44AA
#define KEY_REVOCATION_CMD_TLV_ID       0x55AA

#define KEY_TYPE_SIGN_KEY               0xA1 // Public key used for image signature verification
#define KEY_TYPE_DECR_KEY               0xA2 // Symmetric key used for on-the-fly decryption of the image
#define KEY_TYPE_DATA_KEY               0xA3 // Symmetric key used for data encryption/decryption

#define CRC_PRELOAD                     0xFFFF
#define CRC16_CCITT                     0x1021

typedef struct {
        uint32_t active_fw_address;
        uint32_t update_fw_address;
} product_header_data_t;

typedef struct {
        uint8_t Identifier[2];
        uint8_t active_fw_address[4];
        uint8_t update_fw_address[4];
        uint8_t flash_burstcmda[4];
        uint8_t flash_burstcmdb[4];
        uint8_t type_flash_config_reg[2];
        uint8_t length_flash_config_reg[2];

}product_header_layout_qspi;

typedef struct {
        product_header_layout_qspi ph_qspi;
        uint8_t reserved[234]; // 256-22 bytes to account for entire page.

}product_header_layout_ram;

typedef struct {
        uint8_t ecc_key_index;
        uint8_t sym_key_index;
        uint8_t nonce[8];
        uint8_t signature_id[2];
        uint8_t signature_size[2];
        uint8_t signature[64];
} image_security_data_t;

typedef struct {
        uint32_t size;
        uint32_t crc;
        uint8_t version[16];
        uint32_t timestamp;
        uint32_t ivt_location;
        uint32_t security_location;               // 0 if non-existing
        uint32_t device_administration_location;  // 0 if non-existing
        image_security_data_t *security;
} image_header_data_t;

typedef enum {
       PRIMARY_PH = 0,
       BACKUP_PH  = 1,
} ph_type;

/**
 * \brief Locate Product header in Flash
 *
 * The function considers the Product header to be found, if the first two bytes are "Pp"
 * and a non-zero value is found in the position expected for Active FW address. It also
 * reads and saves the Update FE address.
 */
void Locate_FLASH_Prod_header(booter_config *bc, product_header_data_t *ph);

bool image_location_is_valid(uint32_t image_location);

/**
 * \brief Parses the image header
 *
 * If the location contains a vaild image header, the image header data structure will be filled
 * accordingly. If not, the image header pointer will be NULL;
 */
void Parse_image_header(uint32_t image_header_location, image_header_data_t **img);

/**
 * \brief Parse security section from image header
 *
 * This function should be called after the initial image header parsing has been performed.
 */
void Parse_security_section(image_header_data_t *img);

/**
 * \brief check CRC on product header
 *
 */
bool Check_CRC(uint32_t product_header_location, uint16_t size);

/**
 * \brief calc CRC on product header in RAM
 *
 */
uint16_t Calc_CRC(uint8_t* product_header_location, uint8_t size);

#endif /* HEADERS_H_ */
