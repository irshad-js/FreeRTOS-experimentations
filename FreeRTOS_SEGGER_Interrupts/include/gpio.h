//------------------------------------------------------------------------------
// (c) Copyright 2012, Dialog Semiconductor BV
// All Rights Reserved
//------------------------------------------------------------------------------
// Dialog SEMICONDUCTOR CONFIDENTIAL
//------------------------------------------------------------------------------
// This code includes Confidential, Proprietary Information and is a Trade 
// Secret of Dialog Semiconductor BV. All use, disclosure, and/or reproduction 
// is prohibited unless authorized in writing.
//------------------------------------------------------------------------------
// Description: Defines for peripherals bit fields

#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED



#define DIR_INPUT 0x000
#define DIR_PULLUP 0x100
#define DIR_PULLDOWN 0x200
#define DIR_OUTPUT 0x300

#define QD_PIN_NONE   0
#define QD_PIN_P00P01 1
#define QD_PIN_P02P03 2
#define QD_PIN_P04P05 3
#define QD_PIN_P06P07 4
#define QD_PIN_P10P11 5
#define QD_PIN_P12P13 6
#define QD_PIN_P23P24 7
#define QD_PIN_P25P26 8
#define QD_PIN_P27P28 9
#define QD_PIN_P29P200 10


#define PWM_CLOCK 1
#define PWM_NORMAL  0

#define I2C_STANDARD 1
#define I2C_FAST  2

#define DIV_BY_TEN 0
#define NO_DIV_BY_TEN 1

#define SWTIM_CLK_SYSTEM 1
#define SWTIM_CLK_32K 0

#endif

