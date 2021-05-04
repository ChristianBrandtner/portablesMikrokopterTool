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
//# HISTORY  usart.c
//#
//# 3.04.2015 Cebra
//# - chg: 2 weitere /r am Ende des gesendeten Strings angefügt, damit Funktion gleich wie im MK-Tool
//#
//# 24.01.2014 OG
//# - add: Debug-Code fuer die ISR-RX Funktion
//#        schaltbar via define DEBUG_FC_COMMUNICATION in main.h - der Code
//#        kann somit drin bleiben
//# - add: Source-History
//############################################################################

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include "../cpu.h"
#include <util/delay.h>
#include <stdarg.h>

#include "../main.h"
#include "usart.h"
#include "../lcd/lcd.h"
#include "../timer/timer.h"
#include "uart1.h"
#include "../eeprom/eeprom.h"
#include "../osd/osd.h"


uint8_t buffer[30];

volatile uint8_t txd_buffer[TXD_BUFFER_LEN];
volatile uint8_t txd_complete = TRUE;

volatile uint8_t rxd_buffer[RXD_BUFFER_LEN];
volatile uint8_t rxd_buffer_locked = FALSE;
volatile uint8_t ReceivedBytes = 0;
volatile uint8_t *pRxData = 0;
volatile uint8_t RxDataLen = 0;

volatile uint16_t stat_crc_error = 0;
volatile uint16_t stat_overflow_error = 0;

volatile uint8_t rx_byte;
volatile uint8_t rxFlag = 0;


#define UART_RXBUFSIZE 64
#define UART_NO_DATA          0x0100              /* no receive data available   */

volatile static uint8_t rxbuf[UART_RXBUFSIZE];
volatile static uint8_t *volatile rxhead, *volatile rxtail;


#ifdef DEBUG_FC_COMMUNICATION
    char *tmp = (char *)" ";
#endif



/*

//-----------------------------------------------------------------------------
// USART1 transmitter ISR
ISR (USART1_TX_vect)
{
	static uint16_t ptr_txd1_buffer = 0;
	uint8_t tmp_tx1;

	if(!txd1_complete) // transmission not completed
	{
		ptr_txd1_buffer++;                    // [0] was already sent
		tmp_tx1 = txd1_buffer[ptr_txd1_buffer];
		// if terminating character or end of txd buffer was reached
		if((tmp_tx1 == '\r') || (ptr_txd1_buffer == TXD_BUFFER_LEN))
		{
			ptr_txd1_buffer = 0;		// reset txd pointer
			txd1_complete = TRUE;	// stop transmission
		}
		UDR1 = tmp_tx1; // send current byte will trigger this ISR again
	}
	// transmission completed
	else ptr_txd1_buffer = 0;
}

*/


#ifdef USART_INT
//-----------------------------------------------------------------------------
// USART0 transmitter ISR
ISR (USART_TX_vect)
{
	static uint16_t ptr_txd_buffer = 0;
	uint8_t tmp_tx;

	if(!txd_complete) // transmission not completed
	{
		ptr_txd_buffer++;                    // [0] was already sent
		tmp_tx = txd_buffer[ptr_txd_buffer];
		// if terminating character or end of txd buffer was reached
		if((tmp_tx == '\r') || (ptr_txd_buffer == TXD_BUFFER_LEN))
		{
			ptr_txd_buffer = 0;		// reset txd pointer
			txd_complete = TRUE;	// stop transmission
		}
		UDR = tmp_tx; // send current byte will trigger this ISR again
	}
	// transmission completed
	else ptr_txd_buffer = 0;
}
#endif


void SetBaudUart0(uint8_t Baudrate)
{
  switch (Baudrate)
  {
  case Baud_2400: USART_Init( UART_BAUD_SELECT(2400,F_CPU) );  break;
  case Baud_4800: USART_Init( UART_BAUD_SELECT(4800,F_CPU) );  break;
  case Baud_9600: USART_Init( UART_BAUD_SELECT(9600,F_CPU) );  break;
  case Baud_19200: USART_Init( UART_BAUD_SELECT(19200,F_CPU) );  break;
  case Baud_38400: USART_Init( UART_BAUD_SELECT(38400,F_CPU) );  break;
  case Baud_57600: USART_Init( UART_BAUD_SELECT(57600,F_CPU) );  break;
//  case Baud_115200: USART_Init( UART_BAUD_SELECT(115200,F_CPU) );  break;
  // Macro erechnet falschen Wert (9,85 = 9) für 115200 Baud mit 20Mhz Quarz, zu grosse Abweichung

//#warning "Baurate prüfen wenn kein 20 Mhz Quarz verwendet wird"

  case Baud_115200: USART_Init( 10 );  break;
  }
}


//-----------------------------------------------------------------------------
//

//
//uint8_t uart_getc_nb(uint8_t *c)
//{
//	if (rxhead==rxtail) return 0;
//	*c = *rxtail;
//	if (++rxtail == (rxbuf + UART_RXBUFSIZE)) rxtail = rxbuf;
//	return 1;
//}




ISR (USART0_RX_vect)
{
	static uint16_t crc;
	static uint8_t ptr_rxd_buffer = 0;
	uint8_t crc1, crc2;
	uint8_t c;
//	IdleTimer = 0;



	if (current_hardware == Wi232)
	{
//		rx_byte = c;
//		rxFlag = 1;
		int diff;
		uint8_t c;
		c=UDR;
		diff = rxhead - rxtail;
		if (diff < 0) diff += UART_RXBUFSIZE;
		if (diff < UART_RXBUFSIZE -1)
		{
			*rxhead = c;
			++rxhead;
			if (rxhead == (rxbuf + UART_RXBUFSIZE)) rxhead = rxbuf;
		};
//		USART_putc (c);
		return;
	}


	if (current_hardware == MKGPS)
	{
//		rx_byte = c;
//		rxFlag = 1;
		int diff;
		uint8_t c;
		c=UDR;
		diff = rxhead - rxtail;
		if (diff < 0) diff += UART_RXBUFSIZE;
		if (diff < UART_RXBUFSIZE -1)
		{
			*rxhead = c;
			++rxhead;
			if (rxhead == (rxbuf + UART_RXBUFSIZE)) rxhead = rxbuf;
		};

		return;
	}



	c = UDR;  // catch the received byte
	if (OSD_active && Config.OSD_SendOSD)          // Daten an SV2 senden
	  uart1_putc(c);


	if (rxd_buffer_locked)
		return; // if rxd buffer is locked immediately return

	// the rxd buffer is unlocked
	if ((ptr_rxd_buffer == 0) && (c == '#')) // if rxd buffer is empty and syncronisation character is received
	{
        // OG DEBUG: DEBUG_FC_COMMUNICATION (einstellbar in main.h)
     	#ifdef DEBUG_FC_COMMUNICATION
		  lcd_print( "#", MNORMAL );     // OG DEBUG
		#endif
		
		rxd_buffer[ptr_rxd_buffer++] = c; // copy 1st byte to buffer
		crc = c; // init crc
	}
	else if (ptr_rxd_buffer < RXD_BUFFER_LEN) // collect incomming bytes
	{
		if(c != '\r') // no termination character
		{
			rxd_buffer[ptr_rxd_buffer++] = c; // copy byte to rxd buffer
			crc += c; // update crc
		}
		else // termination character was received
		{
			// the last 2 bytes are no subject for checksum calculation
			// they are the checksum itself
			crc -= rxd_buffer[ptr_rxd_buffer-2];
			crc -= rxd_buffer[ptr_rxd_buffer-1];
			// calculate checksum from transmitted data
			crc %= 4096;
			crc1 = '=' + crc / 64;
			crc2 = '=' + crc % 64;
			
			// compare checksum to transmitted checksum bytes
			if((crc1 == rxd_buffer[ptr_rxd_buffer-2]) && (crc2 == rxd_buffer[ptr_rxd_buffer-1]))
			{   // checksum valid

                // OG DEBUG: DEBUG_FC_COMMUNICATION (einstellbar in main.h)
         		#ifdef DEBUG_FC_COMMUNICATION
         		  *tmp = rxd_buffer[2];
       		      lcd_print( tmp, MNORMAL );     
                  lcd_putc( 0,0, '[', MNORMAL );
                  lcd_putc( 1,0, rxd_buffer[2], MNORMAL ); 
                  lcd_putc( 2,0, ']', MNORMAL );    			x
                  if( rxd_buffer[2] != 'k' )   set_beep( 13, 0x0001, BeepNormal);
                  if( rxd_buffer[2] == 'Q' )   set_beep( 150, 0x0080, BeepNormal);
        		#endif
                
				rxd_buffer[ptr_rxd_buffer] = '\r'; // set termination character
				ReceivedBytes = ptr_rxd_buffer + 1;// store number of received bytes
				
				if (mode == rxd_buffer[2])
				{
					rxd_buffer_locked = TRUE;          // lock the rxd buffer
					// if 2nd byte is an 'R' enable watchdog that will result in an reset
					if(rxd_buffer[2] == 'R') {wdt_enable(WDTO_250MS);} // Reset-Commando
				}
			}
			else
			{	// checksum invalid
				stat_crc_error++;
				rxd_buffer_locked = FALSE; // unlock rxd buffer
			}
			ptr_rxd_buffer = 0; // reset rxd buffer pointer
		}
	}
	else // rxd buffer overrun
	{
		stat_overflow_error++;
		ptr_rxd_buffer = 0; // reset rxd buffer
		rxd_buffer_locked = FALSE; // unlock rxd buffer
	}
}


//-----------------------------------------------------------------------------
// Function: uart0_getc()
// Purpose:  return byte from ringbuffer
// Returns:  lower byte:  received byte from ringbuffer
//           higher byte: last receive error
//-----------------------------------------------------------------------------
char USART_getc(void)
{
	char val;

//	while(rxhead==rxtail) ;
	if (rxhead==rxtail)
		return val=0;
//	IdleTimer = 0;
	val = *rxtail;
	if (++rxtail == (rxbuf + UART_RXBUFSIZE))
		rxtail = rxbuf;

	return val;
}


uint8_t uart_getc_nb(uint8_t *c)
{
	if (rxhead==rxtail)
		return 0;
//	IdleTimer = 0;
	*c = *rxtail;
	if (++rxtail == (rxbuf + UART_RXBUFSIZE))
		rxtail = rxbuf;
	return 1;
}
//-----------------------------------------------------------------------------
//





//-----------------------------------------------------------------------------
//
void USART_Init (unsigned int baudrate)
{
	// set clock divider
//	#undef BAUD
//	#define BAUD baudrate
//	#include <util/setbaud.h>
//	UBRRH = UBRRH_VALUE;
//	UBRRL = UBRRL_VALUE;

//#ifndef F_CPU
///* In neueren Version der WinAVR/Mfile Makefile-Vorlage kann
//   F_CPU im Makefile definiert werden, eine nochmalige Definition
//   hier wuerde zu einer Compilerwarnung fuehren. Daher "Schutz" durch
//   #ifndef/#endif
//
//   Dieser "Schutz" kann zu Debugsessions führen, wenn AVRStudio
//   verwendet wird und dort eine andere, nicht zur Hardware passende
//   Taktrate eingestellt ist: Dann wird die folgende Definition
//   nicht verwendet, sondern stattdessen der Defaultwert (8 MHz?)
//   von AVRStudio - daher Ausgabe einer Warnung falls F_CPU
//   noch nicht definiert: */
//#warning "F_CPU war noch nicht definiert, wird nun nachgeholt mit 4000000"
//#define F_CPU 18432000UL  // Systemtakt in Hz - Definition als unsigned long beachten
                         // Ohne ergeben sich unten Fehler in der Berechnung
//#endif
//#define BAUD 115200UL      // Baudrate
//
//// Berechnungen
//#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
//#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
//#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.
//
//
//#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
//  #error "Systematischer Fehler der Baudrate grösser 1% und damit zu hoch!"
//#endif


	UBRRH = (unsigned char)(baudrate>>8);
	UBRRL = (unsigned char) baudrate;
//        UBRRH = (unsigned char)(BAUD_REAL>>8);
//        UBRRL = (unsigned char) BAUD_REAL;

#if USE_2X
	UCSRA |= (1 << U2X);	// enable double speed operation
#else
	UCSRA &= ~(1 << U2X);	// disable double speed operation
#endif

	// set 8N1
#if defined (__AVR_ATmega8__) || defined (__AVR_ATmega32__)
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
#else
	UCSRC = (1 << UCSZ1) | (1 << UCSZ0);
#endif
	UCSRB &= ~(1 << UCSZ2);

	// flush receive buffer
	while ( UCSRA & (1 << RXC) ) UDR;

	UCSRB |= (1 << RXEN) | (1 << TXEN);
#ifdef USART_INT
	UCSRB |= (1 << RXCIE) | (1 << TXCIE);
#else
	UCSRB |= (1 << RXCIE);
#endif

	rxhead = rxtail = rxbuf;

}





//-----------------------------------------------------------------------------
// disable the txd pin of usart
void USART_DisableTXD (void)
{
#ifdef USART_INT
	UCSRB &= ~(1 << TXCIE);		// disable TX-Interrupt
#endif
	UCSRB &= ~(1 << TXEN);		// disable TX in USART
	DDRB  &= ~(1 << DDB3);		// set TXD pin as input
	PORTB &= ~(1 << PORTB3);	// disable pullup on TXD pin
}

//-----------------------------------------------------------------------------
// enable the txd pin of usart
void USART_EnableTXD (void)
{
	DDRB  |=  (1 << DDB3);		// set TXD pin as output
	PORTB &= ~(1 << PORTB3);	// disable pullup on TXD pin
	UCSRB |=  (1 << TXEN);		// enable TX in USART
#ifdef USART_INT
	UCSRB |=  (1 << TXCIE);		// enable TX-Interrupt
#endif
}

//-----------------------------------------------------------------------------
// short script to directly send a request thorugh usart including en- and disabling it
// where <address> is the address of the receipient, <label> is which data set to request
// and <ms> represents the milliseconds delay between data
void USART_request_mk_data (uint8_t cmd, uint8_t addr, uint8_t ms)
{
	USART_EnableTXD ();	// re-enable TXD pin

	unsigned char mstenth = ms/10;
	SendOutData(cmd, addr, 1, &mstenth, 1);
	// wait until command transmitted
	while (txd_complete == FALSE);

	USART_DisableTXD ();	// disable TXD pin again
}

//-----------------------------------------------------------------------------
//
void USART_putc (char c)
{
#ifdef USART_INT
#else
	loop_until_bit_is_set(UCSRA, UDRE);
	UDR = c;
#endif
}



//-----------------------------------------------------------------------------
//
void USART_puts (char *s)
{
#ifdef USART_INT
#else
	while (*s)
	{
		USART_putc (*s);
		s++;
	}
#endif
}

//-----------------------------------------------------------------------------
//
void USART_puts_p (const char *s)
{
#ifdef USART_INT
#else
	while (pgm_read_byte(s))
	{
		USART_putc (pgm_read_byte(s));
		s++;
	}
#endif
}

//-----------------------------------------------------------------------------
//
void SendOutData(uint8_t cmd, uint8_t addr, uint8_t numofbuffers, ...) // uint8_t *pdata, uint8_t len, ...
{
	va_list ap;
	uint16_t pt = 0;
	uint8_t a,b,c;
	uint8_t ptr = 0;
	uint16_t tmpCRC = 0;

	uint8_t *pdata = 0;
	int len = 0;

	txd_buffer[pt++] = '#';					// Start character
	txd_buffer[pt++] = 'a' + addr;	// Address (a=0; b=1,...)
	txd_buffer[pt++] = cmd;					// Command

	va_start(ap, numofbuffers);
	if(numofbuffers)
	{
		pdata = va_arg (ap, uint8_t*);
		len = va_arg (ap, int);
		ptr = 0;
		numofbuffers--;
	}

	while(len)
	{
		if(len)
		{
			a = pdata[ptr++];
			len--;
			if((!len) && numofbuffers)
			{
				pdata = va_arg(ap, uint8_t*);
				len = va_arg(ap, int);
				ptr = 0;
				numofbuffers--;
			}
		}
		else
			a = 0;

		if(len)
		{
			b = pdata[ptr++];
			len--;
			if((!len) && numofbuffers)
			{
				pdata = va_arg(ap, uint8_t*);
				len = va_arg(ap, int);
				ptr = 0;
				numofbuffers--;
			}
		}
		else
			b = 0;

		if(len)
		{
			c = pdata[ptr++];
			len--;
			if((!len) && numofbuffers)
			{
				pdata = va_arg(ap, uint8_t*);
				len = va_arg(ap, int);
				ptr = 0;
				numofbuffers--;
			}
		}
		else
			c = 0;

		txd_buffer[pt++] = '=' + (a >> 2);
		txd_buffer[pt++] = '=' + (((a & 0x03) << 4) | ((b & 0xf0) >> 4));
		txd_buffer[pt++] = '=' + (((b & 0x0f) << 2) | ((c & 0xc0) >> 6));
		txd_buffer[pt++] = '=' + ( c & 0x3f);
	}
	va_end(ap);

	for(a = 0; a < pt; a++)
	{
		tmpCRC += txd_buffer[a];
	}
	tmpCRC %= 4096;
	txd_buffer[pt++] = '=' + tmpCRC / 64;
	txd_buffer[pt++] = '=' + tmpCRC % 64;
	txd_buffer[pt++] = '\r';
	txd_buffer[pt++] = '\r';                // Add 3.4.2015 Cebra
	txd_buffer[pt++] = '\r';                // Add 3.4.2015 Cebra

	txd_complete = FALSE;
#ifdef USART_INT
	UDR = txd_buffer[0]; // initiates the transmittion (continued in the TXD ISR)
#else
	for(a = 0; a < pt; a++)
	{
		loop_until_bit_is_set(UCSRA, UDRE);
		UDR = txd_buffer[a];
	}
	txd_complete = TRUE;
#endif
}

//-----------------------------------------------------------------------------
//
void Decode64 (void)
{
	uint8_t a,b,c,d;
	uint8_t ptrIn = 3;
	uint8_t ptrOut = 3;
	uint8_t len = ReceivedBytes - 6;

	while (len)
	{
		a = rxd_buffer[ptrIn++] - '=';
		b = rxd_buffer[ptrIn++] - '=';
		c = rxd_buffer[ptrIn++] - '=';
		d = rxd_buffer[ptrIn++] - '=';
		//if(ptrIn > ReceivedBytes - 3) break;

		if (len--)
			rxd_buffer[ptrOut++] = (a << 2) | (b >> 4);
		else
			break;

		if (len--)
			rxd_buffer[ptrOut++] = ((b & 0x0f) << 4) | (c >> 2);
		else
			break;

		if (len--)
			rxd_buffer[ptrOut++] = ((c & 0x03) << 6) | d;
		else
			break;
	}
	pRxData = &rxd_buffer[3];
	RxDataLen = ptrOut - 3;
}


//-----------------------------------------------------------------------------
//
void SwitchToNC (void)
{

	if(hardware == NC)
	{
		// switch to NC
		USART_putc (0x1b);
		USART_putc (0x1b);
		USART_putc (0x55);
		USART_putc (0xaa);
		USART_putc (0x00);
		current_hardware = NC;
		_delay_ms (50);
	}
}

//-----------------------------------------------------------------------------
//


//-----------------------------------------------------------------------------
//
void SwitchToWi232 (void)
{

//	if(hardware == NC)
	{
		// switch to Wi232
		current_hardware = Wi232;
		_delay_ms (50);
	}
}

//-----------------------------------------------------------------------------
//
void SwitchToFC (void)
{
	uint8_t cmd;

	if (current_hardware == NC)
	{
		// switch to FC
		cmd = 0x00;	// 0 = FC, 1 = MK3MAG, 2 = MKGPS
		SendOutData('u', ADDRESS_NC, 1, &cmd, 1);
		current_hardware = FC;
		_delay_ms (50);
	}
}

//-----------------------------------------------------------------------------
//
void SwitchToMAG (void)
{
	uint8_t cmd;

	if (current_hardware == NC)
	{
		// switch to MK3MAG
		cmd = 0x01;	// 0 = FC, 1 = MK3MAG, 2 = MKGPS
		SendOutData('u', ADDRESS_NC, 1, &cmd, 1);
		current_hardware = MK3MAG;
		_delay_ms (50);
	}
}

//-----------------------------------------------------------------------------
//
void SwitchToGPS (void)
{
	uint8_t cmd;
	if (current_hardware == NC)
	{
		// switch to MKGPS
		cmd = 0x02;	// 0 = FC, 1 = MK3MAG, 2 = MKGPS
		SendOutData('u', ADDRESS_NC, 1, &cmd, 1);
		current_hardware = MKGPS;
		_delay_ms (50);
	}
}
