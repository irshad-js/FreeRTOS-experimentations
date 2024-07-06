/**
 *****************************************************************************************
 *
 * @file uart.h
 *
 * @brief Definition of API for the UART Low Level Driver.
 *
 * Copyright (C) 2017. Dialog Semiconductor, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor. All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 *
 *****************************************************************************************
 */

/* DEFINITIONS */
#define DUMMY (0x00)
#define SOH   (0x01)
#define STX   (0x02)
#define ACK   (0x06)
#define NAK   (0x15)

#define UART_FIFO_EN            (1)
#define UART_RX_FIFO_RESET      (2)
#define UART_TX_FIFO_RESET      (4)

#define MAX_CODE_LENGTH         0x1FFFF
#define LENGTH_64K_OFFSET       0x10000

extern volatile char receive_serial_data;

typedef enum {
        HW_UART_BAUDRATE_1000000   = 0x00000200,
        HW_UART_BAUDRATE_500000    = 0x00000400,
        HW_UART_BAUDRATE_230400    = 0x0000080b,
        HW_UART_BAUDRATE_115200    = 0x00001106,
        HW_UART_BAUDRATE_57600     = 0x0000220c,
        HW_UART_BAUDRATE_38400     = 0x00003401,
        HW_UART_BAUDRATE_28800     = 0x00004507,
        HW_UART_BAUDRATE_19200     = 0x00006803,
        HW_UART_BAUDRATE_14400     = 0x00008a0e,
        HW_UART_BAUDRATE_9600      = 0x0000d005,
        HW_UART_BAUDRATE_4800      = 0x0001a00b,

} HW_UART_BAUDRATE;

void uart_baudrate_set(HW_UART_BAUDRATE baud_rate);
char uart_receive_byte(booter_config*);
void uart_send_byte(char ch);
void printf_string(char * str);
void printf_byte(int a);
void print_halfword(int a);
void print_int(int a);
bool Get_FW_from_UART(booter_config*);
