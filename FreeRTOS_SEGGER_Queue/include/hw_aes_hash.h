/**
 *****************************************************************************************
 *
 * @file kw_aes_hash.h
 *
 * @brief initialization of the HASH engine.
 *
 * Copyright (C) 2017. Dialog Semiconductor, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor. All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 *
 *****************************************************************************************
 */

#ifndef HW_AES_HASH_H_
#define HW_AES_HASH_H_

#include "system_config.h"
#include <stdint.h>

/**
 * \brief Enable AES/HASH engine clock
 *
 * This function enables the AES/HASH engine clock.
 */
__STATIC_INLINE void hw_aes_hash_enable_clock(void)
{
        REG_SET_BIT(CRG_TOP, CLK_AMBA_REG, AES_CLK_ENABLE);
}

/**
 * \brief Disable AES/HASH engine clock
 *
 * This function disables the AES/HASH engine clock.
 */
__STATIC_INLINE void hw_aes_hash_disable_clock(void)
{
        REG_CLR_BIT(CRG_TOP, CLK_AMBA_REG, AES_CLK_ENABLE);
}


__STATIC_INLINE void hw_aes_hash_cfg_sha_512(unsigned int result_size)
{
        uint32_t crypto_ctrl_reg = AES_HASH->CRYPTO_CTRL_REG;

        REG_SET_FIELD(AES_HASH, CRYPTO_CTRL_REG, CRYPTO_HASH_OUT_LEN, crypto_ctrl_reg,
                (result_size > 64)? 63: (result_size == 0)? 0 : (result_size - 1));
        REG_SET_FIELD(AES_HASH, CRYPTO_CTRL_REG, CRYPTO_HASH_SEL, crypto_ctrl_reg, 1);
        REG_SET_FIELD(AES_HASH, CRYPTO_CTRL_REG, CRYPTO_ALG_MD, crypto_ctrl_reg, 1);
        REG_SET_FIELD(AES_HASH, CRYPTO_CTRL_REG, CRYPTO_ALG, crypto_ctrl_reg, 1);

        AES_HASH->CRYPTO_CTRL_REG = crypto_ctrl_reg;
}

__STATIC_INLINE void hw_aes_hash_cfg_sha1(unsigned int result_size)
{
        uint32_t crypto_ctrl_reg = AES_HASH->CRYPTO_CTRL_REG;

        REG_SET_FIELD(AES_HASH, CRYPTO_CTRL_REG, CRYPTO_HASH_OUT_LEN, crypto_ctrl_reg,
                (result_size > 64)? 63: (result_size == 0)? 0 : (result_size - 1));
        REG_SET_FIELD(AES_HASH, CRYPTO_CTRL_REG, CRYPTO_HASH_SEL, crypto_ctrl_reg, 1);
        REG_SET_FIELD(AES_HASH, CRYPTO_CTRL_REG, CRYPTO_ALG_MD, crypto_ctrl_reg, 0);
        REG_SET_FIELD(AES_HASH, CRYPTO_CTRL_REG, CRYPTO_ALG, crypto_ctrl_reg, 1);

        AES_HASH->CRYPTO_CTRL_REG = crypto_ctrl_reg;
}

/**
 * \brief Mark next input block as being last
 *
 * This function is used to configure the engine so as to consider the next input block
 * as the last of the operation. When the operation finishes, the engine's status
 * becomes "inactive".
 */
__STATIC_INLINE void hw_aes_hash_mark_input_block_as_last(void)
{
        REG_CLR_BIT(AES_HASH, CRYPTO_CTRL_REG, CRYPTO_MORE_IN);
}

/**
 * \brief Mark next input block as not being last
 *
 * This function is used to configure the engine so as to expect more input blocks
 * after the operation. When the operation finishes, the engine's status
 * becomes "waiting for input".
 */
__STATIC_INLINE void hw_aes_hash_mark_input_block_as_not_last(void)
{
        REG_SET_BIT(AES_HASH, CRYPTO_CTRL_REG, CRYPTO_MORE_IN);
}


/**
 * \brief Configure DMA for data manipulation
 *
 * This function configures the DMA machine with the source and destination buffers.
 *
 * \param[in] src The physical address of the buffer containing the input data for the operation
 * \param[in] dst The physical address (RAM or Cache RAM only) of the buffer where the output of the
 *                operation will be stored. The dst address must be NULL when configuring the DMA
 *                while the engine is waiting for more input data.
 * \param[in] len The length of the input data
 */
void hw_aes_hash_cfg_dma(const uint8_t *src, uint8_t *dst, unsigned int len);

/**
 * \brief Start AES/HASH engine operation
 *
 * This function starts a AES/HASH operation. The operation depends on the configuration that
 * has been applied before calling this function
 *
 * \sa hw_aes_hash_init
 *
 */
__STATIC_INLINE void hw_aes_hash_start(void)
{
        AES_HASH->CRYPTO_START_REG = 1;
}

/**
 * \brief AES/Hash is active.
 *
 * This function tells if the AES/Hash engine is active or not.
 *
 * \return True if the AES/Hash engine is active and false when it is inactive.
 *
 */
bool hw_aes_hash_is_active();


/**
 * \brief AES/Hash is waiting for more data.
 *
 * This function tells if the AES/Hash engine is waiting for more data or not.
 *
 * \return True if the AES/Hash engine is waiting more data and false when it is not.
 *
 */
bool hw_aes_hash_wait_for_in();


#endif /* HW_AES_HASH_H_ */
