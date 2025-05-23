/* ������ Umlaute m�ssen korrekt sichtbar sein, ansonsten Codepageeinstellungen pr�fen */

/************************************************************************
Title:    Interrupt UART library with receive/transmit circular buffers
Author:   Peter Fleury <pfleury@gmx.ch>   http://jump.to/fleury
File:     $Id: uart.h,v 1.8.2.1 2007/07/01 11:14:38 peter Exp $
Software: AVR-GCC 4.1, AVR Libc 1.4
Hardware: any AVR with built-in UART, tested on AT90S8515 & ATmega8 at 4 Mhz
License:  GNU General Public License
Usage:    see Doxygen manual

LICENSE:
    Copyright (C) 2006 Peter Fleury

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

************************************************************************/
//############################################################################
//# HISTORY  uart1.h
//#
//# 06.08.2015 CB
//# - add: uint8_t receiveNMEA              // Flag zur Empfangssteuerung in ISR(USART1_RX_vect)
//# - chg: UART_RX_BUFFER_SIZE war 1024, darf aber nur max 256 sein
//#
//# 26.06.2014 OG
//# - del: receiveNMEA
//#
//# 30.03.2013 CB
//# - add: uart1_flush(void); in Headerdatei aufgenommen
//###########################################################################
//
//  @defgroup pfleury_uart UART Library
//  @code #include <uart.h> @endcode
//
//  @brief Interrupt UART library using the built-in UART with transmit and receive circular buffers.
//
//  This library can be used to transmit and receive data through the built in UART.
//
//  An interrupt is generated when the UART has finished transmitting or
//  receiving a byte. The interrupt handling routines use circular buffers
//  for buffering received and transmitted data.
//
//  The UART_RX_BUFFER_SIZE and UART_TX_BUFFER_SIZE constants define
//  the size of the circular buffers in bytes. Note that these constants must be a power of 2.
//  You may need to adapt this constants to your target and your application by adding
//  CDEFS += -DUART_RX_BUFFER_SIZE=nn -DUART_RX_BUFFER_SIZE=nn to your Makefile.
//
//  @note Based on Atmel Application Note AVR306
//  @author Peter Fleury pfleury@gmx.ch  http://jump.to/fleury
//

#ifndef UART_H
#define UART_H

#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif


// constants and macros


// @brief  UART Baudrate Expression
//  @param  xtalcpu  system clock in Mhz, e.g. 4000000L for 4Mhz
//  @param  baudrate baudrate in bps, e.g. 1200, 2400, 9600
//
#define UART_BAUD_SELECT(baudRate,xtalCpu) ((xtalCpu)/((baudRate)*16l)-1)

// @brief  UART Baudrate Expression for ATmega double speed mode
//  @param  xtalcpu  system clock in Mhz, e.g. 4000000L for 4Mhz
//  @param  baudrate baudrate in bps, e.g. 1200, 2400, 9600
//
#define UART_BAUD_SELECT_DOUBLE_SPEED(baudRate,xtalCpu) (((xtalCpu)/((baudRate)*8l)-1)|0x8000)


// Size of the circular receive buffer, must be power of 2
#ifndef UART_RX_BUFFER_SIZE
#define UART_RX_BUFFER_SIZE 256         /* 2,4,8,16,32,64,128 or 256 bytes */
#endif

// Size of the circular transmit buffer, must be power of 2
#ifndef UART_TX_BUFFER_SIZE
#define UART_TX_BUFFER_SIZE 64          /* 2,4,8,16,32,64,128 or 256 bytes */
#endif

// test if the size of the circular buffers fits into SRAM
#if ( (UART_RX_BUFFER_SIZE+UART_TX_BUFFER_SIZE) >= (RAMEND-0x60 ) )
#error "size of UART_RX_BUFFER_SIZE + UART_TX_BUFFER_SIZE larger than size of SRAM"
#endif

//global variable
extern volatile uint16_t UART1_RxError;
extern volatile uint8_t  UART1_receiveNMEA;             // Flag zur Empfangssteuerung in ISR(USART1_RX_vect)


// high byte error return code of uart_getc()

#define UART_FRAME_ERROR      0x0800              // Framing Error by UART
#define UART_OVERRUN_ERROR    0x0400              // Overrun condition by UART
#define UART_BUFFER_OVERFLOW  0x0200              // receive ringbuffer overflow
#define UART_NO_DATA          0x0100              // no receive data available

#define TRACKING_RSSI           1
#define TRACKING_GPS            2
#define TRACKING_MKCOCKPIT      3
#define TRACKING_NMEA           4
#define NMEA_receive            5               // Flag zur Empfangssteuerung in ISR(USART1_RX_vect)
//
// function prototypes
//

//
//   @brief   Initialize UART and set baudrate
//   @param   baudrate Specify baudrate using macro UART_BAUD_SELECT()
//   @return  none
//
extern void uart_init(unsigned int baudrate);


//
//  @brief   Get received byte from ringbuffer
//
// Returns in the lower byte the received character and in the
// higher byte the last receive error.
// UART_NO_DATA is returned when no data is available.
//
//  @param   void
//  @return  lower byte:  received byte from ringbuffer
//  @return  higher byte: last receive status
//           - \b 0 successfully received data from UART
//           - \b UART_NO_DATA
//             <br>no receive data available
//           - \b UART_BUFFER_OVERFLOW
//             <br>Receive ringbuffer overflow.
//             We are not reading the receive buffer fast enough,
//             one or more received character have been dropped
//           - \b UART_OVERRUN_ERROR
//             <br>Overrun condition by UART.
//             A character already present in the UART UDR register was
//             not read by the interrupt handler before the next character arrived,
//             one or more received characters have been dropped.
//           - \b UART_FRAME_ERROR
//             <br>Framing Error by UART
//
extern unsigned int uart_getc(void);


//
//  @brief   Put byte to ringbuffer for transmitting via UART
//  @param   data byte to be transmitted
//  @return  none
//

// @brief  Set Baudrate USART1
extern void SetBaudUart1(uint8_t Baudrate);

// @brief  Initialize USART1 (only available on selected ATmegas) @see uart_init
extern void uart1_init(unsigned int baudrate);

// @brief  Get received byte of USART1 from ringbuffer. (only available on selected ATmega) @see uart_getc
extern unsigned int uart1_getc(void);

// @brief  Put byte to ringbuffer for transmitting via USART1 (only available on selected ATmega) @see uart_putc
//extern void uart1_putc(unsigned char data);
extern int uart1_putc(unsigned char data);

// @brief  Put string to ringbuffer for transmitting via USART1 (only available on selected ATmega) @see uart_puts
extern void uart1_puts(const char *s );

// @brief  Put string from program memory to ringbuffer for transmitting via USART1 (only available on selected ATmega) @see uart_puts_p
extern void uart1_puts_p(const char *s );

// @brief  Macro to automatically put a string constant into program memory
#define uart1_puts_P(__s)       uart1_puts_p(PSTR(__s))

extern char *utoa1(char* buffer, const unsigned int size, unsigned int value);

extern uint16_t uart1_available(void);

extern int uart1_peek(void);

// @brief   Flush bytes waiting the receive buffer.  Acutally ignores them.
extern void uart1_flush(void);


#endif // UART_H


