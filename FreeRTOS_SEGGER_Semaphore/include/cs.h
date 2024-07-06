/**
 *****************************************************************************************
 *
 * @file cs.h
 *
 * @brief Definition of Configuration Script handler.
 *
 * Copyright (C) 2017. Dialog Semiconductor, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor. All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 *
 *****************************************************************************************
 */
#ifndef _CS_H
#define _CS_H
#include "otp.h"

#define CS_MAX_SIZE 256*4 //256 entries of 4 bytes

#define CS_START_CMD    0xA5A5A5A5
#define CS_BOOTER_VAL   0x60000000
#define CS_DEVMODE_VAL  0x70000000
#define CS_STX_VAL      0x80000000
#define CS_SDK_VAL      0x90000000
#define CS_STOP_CMD     0x00000000
#define CS_EMPTY_VAL    0xFFFFFFFF

typedef enum {
        CS_OTP           = 0,
        CS_QSPI          = 1,
} cs_source_t;

void configuration_script_handling(booter_config*);
#endif
