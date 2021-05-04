/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
 *   Copyright (C) 2008 Thomas Kaiser, thomas@ft-fanpage.de                  *
 *   Copyright (C) 2009 Peter "woggle" Mack, mac@denich.net                  *
 *   Copyright (C) 2011 Christian "Cebra" Brandtner, brandtner@brandtner.net *
 *   Copyright (C) 2011 Harald Bongartz                                      *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License.          *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.               *
 *                                                                           *
 *                                                                           *
 *   Credits to:                                                             *
 *   Holger Buss & Ingo Busker from mikrokopter.de for the MK project + SVN  *
 *                          http://www.mikrokopter.de                        *
 *   Gregor "killagreg" Stobrawa for his version of the MK code              *
 *   Thomas Kaiser "thkais" for the original project. See                    *
 *                          http://www.ft-fanpage.de/mikrokopter/            *
 *                          http://forum.mikrokopter.de/topic-4061-1.html    *
 *   Claas Anders "CaScAdE" Rathje for providing the font and his C-OSD code *
 *                          http://www.mylifesucks.de/oss/c-osd/             *
 *   Harald Bongartz "HaraldB" for providing his Ideas and Code for usibility*
 *****************************************************************************/

//############################################################################
//# HISTORY  usart.h
//#
//# 22.01.2015 Cebra
//# - chg: RXD_BUFFER_LEN und TXD_BUFFER_LEN vergroessert 230 auf 250 wegen
//#	 	   neuer FC-Version (FC v2.14)
//#
//# 24.01.2014 OG
//# - fix: RXD_BUFFER_LEN und TXD_BUFFER_LEN vergroessert 180 auf 230 wegen 
//#        neuer FC-Version (FC v2.03)
//# - add: Source-History
//############################################################################


#ifndef _USART_H
#define _USART_H

//--------------------------------------------------------------
//
#ifndef FALSE
#define FALSE	0
#endif
#ifndef TRUE
#define TRUE	1
#endif

// addresses
#define ADDRESS_ANY	0
#define ADDRESS_FC	1
#define ADDRESS_NC	2
#define ADDRESS_MAG	3

// must be at least 4('#'+Addr+'CmdID'+'\r')+ (80 * 4)/3 = 111 bytes
//#define TXD_BUFFER_LEN  180
//#define RXD_BUFFER_LEN  180

#define TXD_BUFFER_LEN  250
#define RXD_BUFFER_LEN  250

// Baud rate of the USART
#define USART_BAUD 57600
//#define USART_BAUD 125000

//--------------------------------------------------------------
//
extern uint8_t buffer[30];

extern volatile uint8_t txd_buffer[TXD_BUFFER_LEN];
extern volatile uint8_t txd_complete;
extern volatile uint8_t txd1_buffer[TXD_BUFFER_LEN];
extern volatile uint8_t txd1_complete;
extern volatile uint8_t rxd_buffer[RXD_BUFFER_LEN];
extern volatile uint8_t rxd_buffer_locked;
extern volatile uint8_t ReceivedBytes;
extern volatile uint8_t *pRxData;
extern volatile uint8_t RxDataLen;

extern volatile uint16_t stat_crc_error;
extern volatile uint16_t stat_overflow_error;

extern volatile uint8_t rxFlag;
extern volatile uint8_t rx_byte;

//--------------------------------------------------------------
//
void SetBaudUart0(uint8_t Baudrate);
void USART_Init (unsigned int baudrate);
void USART_DisableTXD (void);
void USART_EnableTXD (void);
void USART_request_mk_data (uint8_t cmd, uint8_t addr, uint8_t ms);

void USART_putc (char c);
void USART_puts (char *s);
void USART_puts_p (const char *s);


extern char USART_getc(void);
void SendOutData (uint8_t cmd, uint8_t addr, uint8_t numofbuffers, ...); // uint8_t *pdata, uint8_t len, ...
//void SendOutData(uint8_t cmd, uint8_t addr, uint8_t numofbuffers, uint8_t *pdata, uint8_t len); // uint8_t *pdata, uint8_t len, ...
void Decode64 (void);

void SwitchToNC (void);
void SwitchToFC (void);
void SwitchToMAG (void);
void SwitchToGPS (void);
void SwitchToWi232 (void);
void debug1(void);

uint8_t uart_getc_nb(uint8_t*);

//--------------------------------------------------------------
//Anpassen der seriellen Schnittstellen Register
#define USART_RXC_vect USART0_RX_vect
//--------------------------------------------------------------
#define UCSRA	UCSR0A
#define UCSRB	UCSR0B
#define UCSRC	UCSR0C
#define UDR		UDR0
#define UBRRL	UBRR0L
#define UBRRH	UBRR0H

// UCSRA
#define	RXC		RXC0
#define TXC		TXC0
#define UDRE	UDRE0
#define FE		FE0
#define UPE		UPE0
#define U2X		U2X0
#define MPCM	MPCM0

// UCSRB
#define RXCIE	RXCIE0
#define TXCIE	TXCIE0
#define UDRIE	UDRIE0
#define TXEN	TXEN0
#define RXEN	RXEN0
#define UCSZ2	UCSZ02
#define RXB8	RXB80
#define TXB8	TXB80

// UCSRC
#define UMSEL1	UMSEL01
#define UMSEL0	UMSEL00
#define UPM1	UPM01
#define UPM0	UPM00
#define USBS	USBS0
#define UCSZ1	UCSZ01
#define UCSZ0	UCSZ00
#define UCPOL	UCPOL0


#endif

