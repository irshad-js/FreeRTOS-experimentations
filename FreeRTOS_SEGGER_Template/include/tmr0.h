/**
 *****************************************************************************************
 *
 * @file tmr0.h
 *
 * @brief Definition of API for the TIMER0 Low Level Driver.
 *
 * Copyright (C) 2017. Dialog Semiconductor, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor. All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 *
 *****************************************************************************************
 */
#ifndef _TMR0_H
#define _TMR0_H
#include "system_config.h"
void start_timer(uint32_t);
void stop_timer();
#endif
