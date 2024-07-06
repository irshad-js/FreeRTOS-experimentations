/**
 *****************************************************************************************
 *
 * @file otp.h
 *
 * @brief Definition of API for the OTP Low Level Driver.
 *
 * Copyright (C) 2017. Dialog Semiconductor, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor. All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 *
 *****************************************************************************************
 */


#ifndef OTP_H
#define OTP_H

#define OTP_MEM_START     0x10080000
#define OTP_MEM_START_P   0x30080000

// OTP Layout
#define OTP_CS_ADDRESS          0x00000C00 // 1024 bytes
#define OTP_QSPI_DECR_KEYS      0x00000B00 // 256 bytes
#define OTP_USER_DATA_KEYS      0x00000A00 // 256 bytes
#define OTP_QSPI_DECR_KEYS_INDX 0x000009E0 // 32 bytes
#define OTP_USER_DATA_KEYS_INDX 0x000009C0 // 32 bytes
#define OTP_SIGNATURE_KEYS      0x000008C0 // 256 bytes
#define OTP_SIGNATURE_KEYS_INDX 0x000008A0 // 32 bytes
#define OTP_APPLICATION_AREA    0x00000000 // 2208 bytes

#define OTPC_MODE_PDOWN   0x0
#define OTPC_MODE_DSTBY   0x1
#define OTPC_MODE_STBY    0x2
#define OTPC_MODE_READ    0x3
#define OTPC_MODE_PROG    0x4
#define OTPC_MODE_PVFY    0x5
#define OTPC_MODE_RINI    0x6

typedef enum {
        OTPC_16MHZ,
        OTPC_32MHZ,
        OTPC_48MHZ,
        OTPC_96MHZ
}OTPC_speed;

void otp_init(OTPC_speed);
void otp_readmode(void);
void otp_writemode(void);
void otp_disable(void);
void otp_prog (uint32_t cell_addr, uint32_t *pword, uint32_t nwords);

#endif //OTP_H
