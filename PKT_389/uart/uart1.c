/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*************************************************************************
Title:    Interrupt UART library with receive/transmit circular buffers
Author:   Peter Fleury <pfleury@gmx.ch>   http://jump.to/fleury
File:     $Id: uart.c,v 1.6.2.2 2009/11/29 08:56:12 Peter Exp $
Software: AVR-GCC 4.1, AVR Libc 1.4.6 or higher
Hardware: any AVR with built-in UART,
License:  GNU General Public License

DESCRIPTION:
    An interrupt is generated when the UART has finished transmitting or
    receiving a byte. The interrupt handling routines use circular buffers
    for buffering received and transmitted data.

    The UART_RX_BUFFER_SIZE and UART_TX_BUFFER_SIZE variables define
    the buffer size in bytes. Note that these variables must be a
    power of 2.

USAGE:
    Refere to the header file uart.h for a description of the routines.
    See also example test_uart.c.

NOTES:
    Based on Atmel Application Note AVR306

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

*************************************************************************/

//############################################################################
//# HISTORY  uart1.c
//#
//# 08.08.2015 CB
//# - chg: Umbau ISR(USART1_RX_vect) um Behandlung der NMEA Datensätze
//#
//# 06.08.2015 CB
//# - add: uint8_t receiveNMEA              // Flag zur Empfangssteuerung in ISR(USART1_RX_vect)
//# - add: Erweiterung ISR(USART1_RX_vect) um Behandlung der NMEA Datensätze
//#
//# 26.06.2014 OG
//# - del: receiveNMEA
//#
//# 04.07.2013 Cebra
//# - add: neue Funktion uart1_peek
//############################################################################


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdbool.h>
#include "uart1.h"
#include "usart.h"
#include "../main.h"
#include "../bluetooth/bluetooth.h"
#include "../tracking/tracking.h"
#include "../avr-nmea-gps-library/nmea.h"

//
//  constants and macros
//


// size of RX/TX buffers
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1)
#define UART_TX_BUFFER_MASK ( UART_TX_BUFFER_SIZE - 1)

#if ( UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK )
#error RX buffer size is not a power of 2
#endif

#if ( UART_TX_BUFFER_SIZE & UART_TX_BUFFER_MASK )
#error TX buffer size is not a power of 2
#endif


// ATmega with two USART

#define ATMEGA_USART1

#define UART1_STATUS   UCSR1A
#define UART1_CONTROL  UCSR1B
#define UART1_DATA     UDR1
#define UART1_UDRIE    UDRIE1



//
//  module global variables
//

#if defined( ATMEGA_USART1 )
static volatile unsigned char UART1_TxBuf[UART_TX_BUFFER_SIZE];
static volatile unsigned char UART1_RxBuf[UART_RX_BUFFER_SIZE];
static volatile unsigned char UART1_TxHead;
static volatile unsigned char UART1_TxTail;
static volatile unsigned char UART1_RxHead;
static volatile unsigned char UART1_RxTmpHead;
static volatile unsigned char UART1_RxTail;
static volatile unsigned char UART1_LastRxError;

static volatile unsigned char UART1_RxTmpHead;;

volatile uint16_t UART1_RxError;

#endif

volatile uint8_t UART1_receiveNMEA = false;


void SetBaudUart1(uint8_t Baudrate)
{
  switch (Baudrate)
  {
  case Baud_2400: uart1_init( UART_BAUD_SELECT(2400,F_CPU) );  break;
  case Baud_4800: uart1_init( UART_BAUD_SELECT(4800,F_CPU) );  break;
//  case Baud_9600: uart1_init( UART_BAUD_SELECT(9600,F_CPU) );  break;
  case Baud_9600: uart1_init( 129);  break;
  case Baud_19200: uart1_init( UART_BAUD_SELECT(19200,F_CPU) );  break;
  case Baud_38400: uart1_init( UART_BAUD_SELECT(38400,F_CPU) );  break;
  case Baud_57600: uart1_init( UART_BAUD_SELECT(57600,F_CPU) );  break;
//  case Baud_57600: uart1_init( 21);  break;
//  case Baud_115200: uart1_init( UART_BAUD_SELECT(115200,F_CPU) );  break;
  case Baud_115200: uart1_init( 10 );  break;
  }
}

//
// these functions are only for ATmegas with two USART
//


#if defined( ATMEGA_USART1 )

#if 0
//--------------------------------------------------------------
// Function: UART1 Receive Complete interrupt
// Purpose:  called when the UART1 has received a character
//--------------------------------------------------------------
ISR(USART1_RX_vect)
{
    unsigned char tmphead;
    unsigned char data;
    unsigned char usr;
    unsigned char lastRxError;


    // read UART status register and UART data register
    usr  = UART1_STATUS;
    data = UART1_DATA;

    lastRxError = (usr & (_BV(FE1)|_BV(DOR1)) );

    // calculate buffer index
    tmphead = ( UART1_RxHead + 1) & UART_RX_BUFFER_MASK;

    if ( tmphead == UART1_RxTail )
    {
        // error: receive buffer overflow
        lastRxError = UART_BUFFER_OVERFLOW >> 8;
        UART1_RxError++;
    }
    else
    {
        // store new index
        UART1_RxHead = tmphead;
        // store received data in buffer
        UART1_RxBuf[tmphead] = data;
    }
    UART1_LastRxError = lastRxError;
}
#else

#warning Test neue UART ISR für NMEA

//--------------------------------------------------------------
// Function: UART1 Receive Complete interrupt, decode NMEA GPGGA
// Purpose:  called when the UART1 has received a character
//--------------------------------------------------------------
ISR(USART1_RX_vect)
{
    unsigned char tmphead;
    unsigned char data;
    unsigned char usr;
    unsigned char lastRxError;

    usr  = UART1_STATUS;
    data = UART1_DATA;

    lastRxError = (usr & (_BV(FE1)|_BV(DOR1)) );

    // calculate buffer index
    tmphead = ( UART1_RxHead + 1) & UART_RX_BUFFER_MASK;

    if ( tmphead == UART1_RxTail )
    {
        // error: receive buffer overflow
        lastRxError = UART_BUFFER_OVERFLOW >> 8;
        UART1_RxError++;
    }
        else
          {
           if (UART1_receiveNMEA)
              {
               fusedata(data);                  // NMEA Daten bearbeiten
              }

          else  // if (UART1_receiveNMEA)

            {
                // store new index
                UART1_RxHead = tmphead;
                // store received data in buffer
                UART1_RxBuf[tmphead] = data;
            }
    }
    UART1_LastRxError = lastRxError;
}

////--------------------------------------------------------------
//// Function: UART1 Receive Complete interrupt, decode NMEA GPGGA
//// Purpose:  called when the UART1 has received a character
////--------------------------------------------------------------
//ISR(USART1_RX_vect)
//{
//    unsigned char tmphead;
//    unsigned char data;
//    unsigned char usr;
//    unsigned char lastRxError;
//
//    static volatile uint8_t GGA;
//    static char buffer[6];
//    static volatile uint8_t bufferindex;
//    static volatile uint8_t NMEAstart,NMEAend = false;
//
//    // read UART status register and UART data register
//    usr  = UART1_STATUS;
//    data = UART1_DATA;
//
//    lastRxError = (usr & (_BV(FE1)|_BV(DOR1)) );
//
//    // calculate buffer index
//
//    tmphead = ( UART1_RxHead + 1) & UART_RX_BUFFER_MASK;
//
//    if ( tmphead == UART1_RxTail )
//    {
//        // error: receive buffer overflow
//        lastRxError = UART_BUFFER_OVERFLOW >> 8;
//        UART1_RxError++;
//    }
//    else
//      {
//        if (UART1_receiveNMEA)
//          {
//            cli();
//            switch(data)
//              {
//                case '$':                       //start of a packet
//
//                  if (!NMEAstart && GGA)         // falls mitten im  Datensatz eine neuer kommt, warum auch immer
//                    {
//                      // restore index, war kein GPGGA Datensatz
//                      UART1_RxHead = UART1_RxTmpHead;
//                      memset(buffer,0,6);
//                      bufferindex=0;                //wipe all these so they can be reused
//                    }
//                  NMEAstart = true;
//                  NMEAend = false;
//                  GGA=false;
//                  bufferindex=0;                //wipe all these so they can be reused
//
//                  UART1_RxTmpHead = tmphead-1;       // erstmal sichern, Start des NMEA Datensatzes
//                  // store new index, erstam in den Buffer schreiben
//                  UART1_RxHead = tmphead;
//                  // store received data in buffer
//                  UART1_RxBuf[tmphead] = data;
////                  USART_putc(data);
//                  sei();
//                  break;
//
//                default:                        //we have some of the CSV data
//                  if(bufferindex<6)             //dont mess up ! Dont overflow
//                  {
//                        buffer[bufferindex]=data;  //stick the character in our buffer
//                  }
//
//                  if(GGA)
//                    {
//                      // store new index, erstam in den Buffer schreiben
//                      UART1_RxHead = tmphead;
//                      // store received data in buffer
//                      UART1_RxBuf[tmphead] = data;
////                      USART_putc(data);
//
//
//                      switch ( data)
//                        {
//                         case '\r' :
//                             GGA = false;
//                             bufferindex=0;                //wipe all these so they can be reused
//                             NMEAstart = false;
//                             NMEAend = true;
//                             UART1_GPGGA++;
//                             memset(buffer,0,6);
//                             break;          // Ende mit NMEA Datensatz
//
//                         case '\n' :
//                             GGA = false;
//                             bufferindex=0;                //wipe all these so they can be reused
//                             NMEAstart = false;
//                             NMEAend = true;
//                             UART1_GPGGA++;
//                             memset(buffer,0,6);
//                             break;          // Ende mit NMEA Datensatz
//                        }
//
//                    }
//
//                  else if(NMEAstart)                  //the header, erstes Komma noch nicht gekommen
//
//                    {
//                      if(bufferindex<4)
//
//                        {
//                         bufferindex++;          //increase the position in the buffer
//
//                         // store new index, erstmal in den Buffer schreiben
//                         UART1_RxHead = tmphead;
//                         // store received data in buffer
//                         UART1_RxBuf[tmphead] = data;
////                         USART_putc(data);
//
//
//                        }
//                      else
//
//                        {
////                          cli();
//                          if(!strcmp(buffer,"GPGGA"))    //the last character will be a 0(end of string)
//
//                            {
//                             GGA=true;
//                             NMEAstart = false;
//                             // store new index, erstmal in den Buffer schreiben
//                             UART1_RxHead = tmphead;
//                             // store received data in buffer
//                             UART1_RxBuf[tmphead] = data;
////                             USART_putc(data);
////                             sei();
//                            }
//
//                          else
//
//                            {
//                             // restore index, war kein GPGGA Datensatz
//
//                             UART1_RxHead = UART1_RxTmpHead;
////                             commacount=0;
//                             memset(buffer,0,6);
//                             bufferindex=0;                //wipe all these so they can be reused
//                             NMEAstart = false;
////                             sei();
//                            }
//                        }
//                    }
//                      else if (!NMEAend && !GGA) UART1_RxHead = UART1_RxTmpHead; // Zeichen verwerfen
//
//              } // end switch (data)
//          }
//
//        else  // if (UART1_receiveNMEA)
//
//          {
//              // store new index
//              UART1_RxHead = tmphead;
//              // store received data in buffer
//              UART1_RxBuf[tmphead] = data;
//          }
//      }
//    UART1_LastRxError = lastRxError;
//}
#endif

//--------------------------------------------------------------
// Function: UART1 Data Register Empty interrupt
// Purpose:  called when the UART1 is ready to transmit the next byte
//--------------------------------------------------------------
ISR(USART1_UDRE_vect)
{
    unsigned char tmptail;


    if ( UART1_TxHead != UART1_TxTail)
    {
        // calculate and store new buffer index
        tmptail = (UART1_TxTail + 1) & UART_TX_BUFFER_MASK;
        UART1_TxTail = tmptail;
        // get one byte from buffer and write it to UART
        UART1_DATA = UART1_TxBuf[tmptail];  // start transmission
    }
    else
    {
        // tx buffer empty, disable UDRE interrupt
        UART1_CONTROL &= ~_BV(UART1_UDRIE);
    }
}


//--------------------------------------------------------------
// Function: uart1_init()
// Purpose:  initialize UART1 and set baudrate
// Input:   baudrate using macro UART_BAUD_SELECT()
// Returns:  none
//--------------------------------------------------------------
void uart1_init(unsigned int baudrate)
{
    UART1_TxHead = 0;
    UART1_TxTail = 0;
    UART1_RxHead = 0;
    UART1_RxTail = 0;
    UART1_RxError = 0;
    UART1_receiveNMEA = false;  //Empfang von NMEA Daten aus


    // Set baud rate
    if ( baudrate & 0x8000 )
    {
        UART1_STATUS = (1<<U2X1);  //Enable 2x speed
        baudrate &= ~0x8000;
    }
    UBRR1H = (unsigned char)(baudrate>>8);
    UBRR1L = (unsigned char) baudrate;

    // Enable USART receiver and transmitter and receive complete interrupt
    UART1_CONTROL = _BV(RXCIE1)|(1<<RXEN1)|(1<<TXEN1);

    // Set frame format: asynchronous, 8data, no parity, 1stop bit
#ifdef URSEL1
    UCSR1C = (1<<URSEL1)|(3<<UCSZ10);
#else
    UCSR1C = (3<<UCSZ10);
//  UCSR1C = (1<<UCSZ11) | (1<<UCSZ10); // 8data Bit
#endif



}


//--------------------------------------------------------------
// Function: uart1_getc()
// Purpose:  return byte from ringbuffer
// Returns:  lower byte:  received byte from ringbuffer
//           higher byte: last receive error
//--------------------------------------------------------------
unsigned int uart1_getc(void)
{
    unsigned char tmptail;
    unsigned char data;


    if ( UART1_RxHead == UART1_RxTail )
    {
        return UART_NO_DATA;   // no data available
    }

    // calculate /store buffer index
    tmptail = (UART1_RxTail + 1) & UART_RX_BUFFER_MASK;
    UART1_RxTail = tmptail;

    // get data from receive buffer
    data = UART1_RxBuf[tmptail];

    return (UART1_LastRxError << 8) + data;

}/* uart1_getc */


//--------------------------------------------------------------
// Function: uart1_peek()
// Purpose:  Returns the next byte (character) of incoming serial data without removing it from the internal serial buffer
// Returns:  lower byte:  received byte from ringbuffer
//           higher byte: last receive error
//--------------------------------------------------------------
int uart1_peek(void)
{
        unsigned char tmptail;
        unsigned char data;


        if ( UART1_RxHead == UART1_RxTail )
        {
                return -1;   // no data available
        }

        // calculate /store buffer index
        tmptail = (UART1_RxTail + 1) & UART_RX_BUFFER_MASK;

        // get data from receive buffer
        data = UART1_RxBuf[tmptail];

        return (UART1_LastRxError << 8) + data;

}/* uart1_getc */


//int HardwareSerial::peek(void)
//{
//  if (_rx_buffer->head == _rx_buffer->tail) {
//    return -1;
//  } else {
//    return _rx_buffer->buffer[_rx_buffer->tail];
//  }
//}


//--------------------------------------------------------------
// Function: uart1_putc()
// Purpose:  write byte to ringbuffer for transmitting via UART
// Input:    byte to be transmitted
// Returns:  1 on succes, 0 if remote not ready
//--------------------------------------------------------------
int uart1_putc(unsigned char data)
{
    unsigned char tmphead;


    tmphead = (UART1_TxHead + 1) & UART_TX_BUFFER_MASK;

    while ( tmphead == UART1_TxTail )
    {;}  // wait for free space in buffer

    UART1_TxBuf[tmphead] = data;
    UART1_TxHead = tmphead;

    // enable UDRE interrupt
    UART1_CONTROL |= _BV(UART1_UDRIE);
    return (UART1_LastRxError << 8) + data;
}


//--------------------------------------------------------------
// Function: uart1_puts()
// Purpose:  transmit string to UART1
// Input:    string to be transmitted
// Returns:  none
//--------------------------------------------------------------
void uart1_puts(const char *s )
{
    while (*s)
        uart1_putc(*s++);
}


//--------------------------------------------------------------
// Function: uart1_puts_p()
// Purpose:  transmit string from program memory to UART1
// Input:    program memory string to be transmitted
// Returns:  none
//--------------------------------------------------------------
void uart1_puts_p(const char *progmem_s )
{
    register char c;
    while ( (c = pgm_read_byte(progmem_s++)) )
        uart1_putc(c);
}


//--------------------------------------------------------------
// Function: uart1_available()
// Purpose:  Determine the number of bytes waiting in the receive buffer
// Input:    None
// Returns:  Integer number of bytes in the receive buffer
//--------------------------------------------------------------
uint16_t uart1_available(void)
{
    return (UART_RX_BUFFER_MASK + UART1_RxHead - UART1_RxTail) % UART_RX_BUFFER_MASK;
}



//--------------------------------------------------------------
// Function: uart1_flush()
// Purpose:  Flush bytes waiting the receive buffer.  Acutally ignores them.
// Input:    None
// Returns:  None
//--------------------------------------------------------------
void uart1_flush(void)
{
    UART1_RxHead = UART1_RxTail;
}


/*************************************************************************
Function: utoa()
Purpose:  convert unsigned integer to ascii
Input:    string_buffer, string_buffer_size, unsigned integer value
Returns:  first ascii character
**************************************************************************/
char *utoa1(char* buffer, const unsigned int size, unsigned int value)
{
  char *p;

  // p points to last char
  p = buffer+size-1;

  // Set terminating Zero
  *p='\0';

  do
  {
    *--p = value%10 + '0';
    value = value/10;
  } while (value!=0 && p>buffer);

  return p;
}/* utoa */






#endif
//#endif

