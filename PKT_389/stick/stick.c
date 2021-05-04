/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
 *   Copyright (C) 2008 Thomas Kaiser, thomas@ft-fanpage.de                  *
 *   Copyright (C) 2009 Peter "woggle" Mack, mac@denich.net                  *
 *   Copyright (C) 2011 Christian "Cebra" Brandtner, brandtner@brandtner.net *
 *   Copyright (C) 2011 Harald Bongartz                                      *
 *   Copyright (C) 2012 Martin Runkel                                        *
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


#include "../cpu.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <string.h>
#include <stdlib.h>

#include "../lcd/lcd.h"
#include "../timer/timer.h"
//#include "../tracking/servo.h"
#include "../messages.h"
#include "../lipo/lipo.h"
#include "stick.h"
#include "stick_setup.h"
#include "../eeprom/eeprom.h"
#include "../setup/setup.h"

#define SERVO_CORRECT 3.125


#include <util/delay.h>
#include <inttypes.h>
#include <stdlib.h>

#include "../main.h"
#include "../uart/uart1.h"
#include "../uart/usart.h"


#define LIMIT_MIN_MAX(value, min, max) {if(value <= min) value = min; else if(value >= max) value = max;}

	int16_t Pos_Stick[12];  // 1,5mS 
	int16_t Pos_alt[5];  //

	uint8_t BalkenPos = 0;
	uint8_t Stick_Display = 0;
	//uint8_t serialChannelRichtung = 0;
	//uint8_t serialChannelNeutral = 0;
	//uint8_t serialChannelConfig = 2;


//--------------------------------------------------------------
//
void joystick (void)
{
	// uint8_t chg = 0;
	// uint8_t Pos_Stick = 150;  // 1,5mS
	// uint8_t Pos_alt = 150;  // 
	
	//int16_t Pos_Stick[12];  // 1,5mS
	
	uint8_t chg = 0;
	//uint8_t BalkenPos = 0;
	uint8_t Stick_Nr = 0;
	//uint8_t Stick_Display = 0;
	uint8_t i = 0;

	memset (Pos_Stick, 150, 3); // füllt  3+1 Byte vom Pos_Stick[12] mit 150
	
	//int16_t Pos_alt[5];  // 
	int16_t Poti_Summe[5];  // 
	memset (Poti_Summe, 0, 5); // füllt  3+1 Byte mit 0
	int16_t Poti_Neutral[5];  // 


	// ADC- init
	//if (Config.Lipomessung == true)
	//{

	//}
	//for (uint8_t i = 0; i < 4; i++)

	if (Config.stick_typ[0] == 0) // Poti
			{
			PORTA &= ~(1<<PORTA0);
			DDRA &= ~(1<<DDA0);
			}
		else			// Taster
			{
			PORTA |= (1<<PORTA0);
			DDRA |= (1<<DDA0);
			}
	if (Config.Lipomessung == false)
	{
	if (Config.stick_typ[1] == 0) // Poti
				{
				PORTA &= ~(1<<PORTA1);
				DDRA &= ~(1<<DDA1);
				}
			else	// Taster
				{
				PORTA |= (1<<PORTA1);
				DDRA |= (1<<DDA1);
				}
	}
	if (Config.stick_typ[2] == 0)
				{
				PORTA &= ~(1<<PORTA2);
				DDRA &= ~(1<<DDA2);
				}
			else
				{
				PORTA |= (1<<PORTA2);
				DDRA |= (1<<DDA2);
				}
	if (Config.stick_typ[3] == 0)
				{
				PORTA &= ~(1<<PORTA3);
				DDRA &= ~(1<<DDA3);
				}
			else
				{
				PORTA |= (1<<PORTA3);
				DDRA |= (1<<DDA3);
				}

	//if (Config.stick_dir[i] == 0)  serialChannelRichtung &= ~(1<<i); else  serialChannelRichtung |= (1<<i);
	//if (Config.stick_neutral[i] == 0)  serialChannelNeutral &= ~(1<<i); else  serialChannelNeutral |= (1<<i);

	//if (Config.Lipomessung == true)

		//serialChannelConfig


// Ermittlung der Neutralposition
	for (uint8_t i = 0; i < 4; i++)
	{
			ADMUX = (1<<REFS0)|(0<<MUX0);  // Multiplexer selection Register: AVCC with external capacitor at AREF pin , ADC1
			ADMUX = (ADMUX & ~(0x1F)) | (i & 0x1F); // ADC[i] verwenden

			timer = 50;
			while (timer > 0);

			ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // ADC Enable, ADC Start, Prescaler 128


			while (ADCSRA & (1<<ADSC));  // wenn ADC fertig

				Poti_Neutral[i] = ((ADCW>>2)&0xff);
				LIMIT_MIN_MAX (Poti_Neutral[i],108,148);

				ADMUX = (ADMUX & ~(0x1F)) | (i & 0x1F); // ADC[i] verwenden
					// Stick_Nr 1,2,3 = Potis, Stick_Nr 1= Lipo
					ADCSRA |= (1<<ADSC); // ADC Start

			while (ADCSRA & (1<<ADSC));  // wenn ADC fertig

					Poti_Neutral[i] = ((ADCW>>2)&0xff);
					LIMIT_MIN_MAX (Poti_Neutral[i],108,148);
	}
	Stick_Nr = 0;
	ADMUX = (ADMUX & ~(0x1F)) | (Stick_Nr & 0x1F); // ADC[i] verwenden
	ADCSRA |= (1<<ADSC); // ADC Start

	/*
	Stick_Nr = 0;
	ADMUX = (1<<REFS0)|(0<<MUX0);  // Multiplexer selection Register: AVCC with external capacitor at AREF pin , ADC1
	ADMUX = (ADMUX & ~(0x1F)) | (Stick_Nr & 0x1F); // ADC[Stick_Nr] verwenden
	
	timer = 50;
	while (timer > 0);
	
	ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // ADC Enable, ADC Start, Prescaler 128

	// Stick-Neutralposition bestimmen

	while (ADCSRA & (1<<ADSC));  // wenn ADC fertig
		
		Poti_Neutral[Stick_Nr] = ((ADCW>>2)&0xff);
		LIMIT_MIN_MAX (Poti_Neutral[Stick_Nr],108,148);
		Stick_Nr = 2 ;
		ADMUX = (ADMUX & ~(0x1F)) | (Stick_Nr & 0x1F); // ADC[i] verwenden
			// Stick_Nr 1,2,3 = Potis, Stick_Nr 1= Lipo
			ADCSRA |= (1<<ADSC); // ADC Start
		
	while (ADCSRA & (1<<ADSC));  // wenn ADC fertig
		
			Poti_Neutral[Stick_Nr] = ((ADCW>>2)&0xff);
			LIMIT_MIN_MAX (Poti_Neutral[Stick_Nr],108,148);
			Stick_Nr = 0;
			ADMUX = (ADMUX & ~(0x1F)) | (Stick_Nr & 0x1F); // ADC[i] verwenden
			// Stick_Nr 1,2,3 = Potis, Stick_Nr 1= Lipo
			ADCSRA |= (1<<ADSC); // ADC Start
	*/



	//OCR1A = 150 * SERVO_CORRECT;  // Servomitte

	lcd_cls ();
	
	// Kopfzeile und Rahmen zeichnen
	
	lcd_printp (PSTR(" serielle Potis 1-5 "), 2); 
	//lcd_printp_at (7, 5, PSTR("%"), 0);
	//lcd_printp_at (16, 5, PSTR("mS"), 0);

        lcd_printp_at(0, 7, strGet(KEYLINE3), 0);
	lcd_printp_at (18, 7, PSTR("\x19O\x18"), 0);
	
	
	for (i=0 ; i< 5 ; i++)
	{
	BalkenPos = 12 + (i*8) ; 

	lcd_rect(3,BalkenPos, 120, 6, 1);  			// +-150% Rahmen
	lcd_line(23,BalkenPos,23,(BalkenPos+6),1);  	// -100%
	lcd_line(43,BalkenPos,43,(BalkenPos+6),1);  	// -50%
	lcd_frect(62,BalkenPos, 2, 6, 1);  			// 0%
	lcd_line(83,BalkenPos,83,(BalkenPos+6),1);  	// +50%
	lcd_line(103,BalkenPos,103,(BalkenPos+6),1);  	// +100%
	}
	
	// Reset auf Mittelstellung
	Pos_Stick[0] = 150;
	Poti_Summe[0] = 0;
	Pos_Stick[2] = 150;
	Poti_Summe[2] = 0;
	Pos_Stick[4] = 150;
	Poti_Summe[4] = 0;
	
	chg = 255;

	do
	{
		if (!(ADCSRA & (1<<ADSC)))  // wenn ADC fertig
		{
			
			//Pos_Stick[Stick_Nr] = 150 + 128 - ((ADCW>>2)&0xff);
			//if (serialChannelRichtung & (1<<Stick_Nr))
			if (Config.stick_dir[Stick_Nr] > 0)
			Pos_Stick[Stick_Nr] = Poti_Neutral[Stick_Nr] - ((ADCW>>2)&0xff);
			else
			Pos_Stick[Stick_Nr] = ((ADCW>>2)&0xff) - Poti_Neutral[Stick_Nr];


			LIMIT_MIN_MAX (Pos_Stick[Stick_Nr],-120,120);
			//if ((Stick_Nr==0) || (Stick_Nr==2)) // nur die Potis 1,2 sind nicht neutralisierend
			if (Config.stick_neutral[Stick_Nr]==0) //  nicht neutralisierend
			{
			Poti_Summe[Stick_Nr] += (Pos_Stick[Stick_Nr]/8) * abs(Pos_Stick[Stick_Nr]/8);
			LIMIT_MIN_MAX (Poti_Summe[Stick_Nr],-(120*128),(120*128));
			Pos_Stick[Stick_Nr]= Poti_Summe[Stick_Nr] / 128; // nicht neutralisierend
			}
			Pos_Stick[Stick_Nr] += 150;
			//LIMIT_MIN_MAX (Pos_Stick[Stick_Nr],30,270); // war 75 , 225
			LIMIT_MIN_MAX (Pos_Stick[Stick_Nr],Config.stick_min[Stick_Nr],Config.stick_max[Stick_Nr]); // war 30, 270
			if (Pos_Stick[Stick_Nr] != Pos_alt[Stick_Nr]) // nur bei Änderung
			{
			 chg |= (1<<Stick_Nr) ;
			//Pos_alt=Pos_Stick ; // verschoben
			}
			Stick_Nr ++ ;
			//if (Stick_Nr==1) Stick_Nr=2; // Lipo überspringen
			
			if (Stick_Nr==3) // Taster
			{
				if (Config.stick_dir[Stick_Nr] > 0)
				{
				if (PINA & (1 << KEY_EXT))  Pos_Stick[Stick_Nr] = Config.stick_min[Stick_Nr];
				else Pos_Stick[Stick_Nr] = Config.stick_max[Stick_Nr];
				}
				else
				{
				if (PINA & (1 << KEY_EXT))  Pos_Stick[Stick_Nr] = Config.stick_max[Stick_Nr];
				else Pos_Stick[Stick_Nr] = Config.stick_min[Stick_Nr];
				}
				
				if (Pos_Stick[Stick_Nr] != Pos_alt[Stick_Nr]) 
				{
				chg |= (1<<Stick_Nr) ;
				}
			 
			 Stick_Nr=0;
			}
			 
			 /*
			 #ifndef ohne_Lipo // MartinR
			Stick_Nr = 1; // MartinR AD-Kanal 1 überspringen wegen Lipo Überwachung
			#endif
			*/

			ADMUX = (ADMUX & ~(0x1F)) | (Stick_Nr & 0x1F); // ADC[i] verwenden
			// Stick_Nr 1,2,3 = Potis, Stick_Nr 0= Lipo
			ADCSRA |= (1<<ADSC); // ADC Start
			
			//serialPotis ();
		}
    
	

		if ((get_key_press (1 << KEY_PLUS) | get_key_long_rpt_sp ((1 << KEY_PLUS), 3)))
		//if (get_key_press (1 << KEY_PLUS))
		//if (PINA & (1 << KEY_PLUS))
		{
			if (Config.stick_neutral[4]==0) //  nicht neutralisierend
			{
			if (Config.stick_dir[4] == 0) Pos_Stick[4] ++ ; else Pos_Stick[4] -- ;
			LIMIT_MIN_MAX (Pos_Stick[4],Config.stick_min[4],Config.stick_max[4]); // war 30, 270
			}
			else
			{
				if (Config.stick_dir[4] == 0) Pos_Stick[4] = Config.stick_max[4] ; else Pos_Stick[4] = Config.stick_min[4] ;
			}
			chg |= (1<<4) ;
		}	
			
			
		else if ((get_key_press (1 << KEY_MINUS) | get_key_long_rpt_sp ((1 << KEY_MINUS), 3)))
		{
			if (Config.stick_neutral[4]==0) //  nicht neutralisierend
			{
			if (Config.stick_dir[4] == 0) Pos_Stick[4] -- ; else Pos_Stick[4] ++ ;
			LIMIT_MIN_MAX (Pos_Stick[4],Config.stick_min[4],Config.stick_max[4]); // war 30, 270
			//LIMIT_MIN_MAX (Pos_Stick[Stick_Nr],30,270); // war 75 , 225
			}
			else
			{
				if (Config.stick_dir[4] == 0) Pos_Stick[4] = Config.stick_min[4] ; else Pos_Stick[4] = Config.stick_max[4] ;
			}
			chg |= (1<<4) ;
		}	

		if (Config.stick_neutral[4]!= 0 && Pos_Stick[4]!= 150 && (PINA &(1 << KEY_PLUS)) && (PINA &(1 << KEY_MINUS)))
		{
			Pos_Stick[4] = 150 ;
			chg |= (1<<4) ;
		}

		if (get_key_press (1 << KEY_ENTER))
		{
		/*
		 for (i=0 ; i< 4 ; i++)
			{
			BalkenPos = 12 + (i*8) ; 
			
			lcd_frect (4, (BalkenPos+1), 118, 4, 0);  // Balken löschen
			lcd_frect(62, BalkenPos, 2, 6, 1);  // 0%
			}
		*/	
			Pos_Stick[0] = 150;
			Poti_Summe[0] = 0;
			Pos_Stick[2] = 150;
			Poti_Summe[2] = 0;
			Pos_Stick[4] = 150;
			Poti_Summe[4] = 0;
			BeepTime = 200;
			BeepMuster = 0x0080;
			chg = 255;
		}

		if (chg)
		{
			if (chg & (1<<0)); // Stick 1
				{
				BalkenPos = 12 + (0*8) ;
				Stick_Display = 0;
				Balken_Zeichnen () ;
			
				Pos_alt[Stick_Display]=Pos_Stick[Stick_Display];
				}
			// Stick 2 = Lipo
			if (chg & (1<<1)); // Stick 2
				{
				BalkenPos = 12 + (1*8) ;
				Stick_Display = 1;
				//if (serialChannelConfig & (0<<1))  Balken_Zeichnen () ; // nur wenn keine Lipo-Spannung
				if (Config.Lipomessung == false) Balken_Zeichnen () ; // nur wenn keine Lipo-Spannung
			
				Pos_alt[Stick_Display]=Pos_Stick[Stick_Display];
				}
			
			
			if (chg & (1<<2)); // Stick 3
				{
				BalkenPos = 12 + (2*8) ;
				Stick_Display = 2;
				Balken_Zeichnen () ;
			
				Pos_alt[Stick_Display]=Pos_Stick[Stick_Display];
				}
			
			if (chg & (1<<3)); // Stick 4 = Taster
				{
				BalkenPos = 12 + (3*8) ;
				Stick_Display = 3;
				Balken_Zeichnen () ;
			
				Pos_alt[Stick_Display]=Pos_Stick[Stick_Display];
				}
				
			if (chg & (1<<4)); // Stick 5 = Taster vom PKT
				{
				BalkenPos = 12 + (4*8) ;
				Stick_Display = 4;
				Balken_Zeichnen () ;
				//OCR1A = (((Pos_Stick[Stick_Display]-150)/1.6)+150) * SERVO_CORRECT;  // Servostellung , 1.6=0.8*0.5
			
				Pos_alt[Stick_Display]=Pos_Stick[Stick_Display];
				}
			
			chg = 0;
			serialPotis ();
		}
	}
	while (!get_key_press (1 << KEY_ESC));
	get_key_press(KEY_ALL);
	
	//#ifdef HWVERSION3_9
	//#ifndef ohne_Lipo // MartinR
	if (Config.Lipomessung == true) // MartinR: geändert
			{
			//DDRA &= ~(1<<DDA1);       // Eingang: PKT Lipo Messung
			//PORTA &= ~(1<<PORTA1);
			ADC_Init();             // ADC für Lipomessung wieder aktivieren
			}

        //ADC_Init();             // ADC für Lipomessung wieder aktivieren
	//#endif
	//#endif

}

//--------------------------------------------------------------
//
void serialPotis (void)
{
	uint8_t i = 0;
	
	memset (buffer, 0, 12); // füllt die 12+1 Byte vom buffer mit 0

	for (i=0 ; i< 5 ; i++)
	{
	buffer[i] = Pos_Stick[i]-150 ; 
	}
	
	SendOutData('y', ADDRESS_FC, 1, buffer, 12);
}
//--------------------------------------------------------------
//
void Balken_Zeichnen (void)
{
	// Balken löschen
			
		if ((Pos_Stick[Stick_Display] > Pos_alt[Stick_Display])&&(Pos_alt[Stick_Display] < 150)) // Balken links löschen
		lcd_frect ((63-((150 -Pos_alt[Stick_Display]) * 0.5)), (BalkenPos+1), (63-((150- Pos_Stick[Stick_Display]) * 0.5)), 4, 0);
		if ((Pos_Stick[Stick_Display] < Pos_alt[Stick_Display])&&(Pos_alt[Stick_Display] > 150)) // Balken rechts löschen
		lcd_frect ((63+((Pos_Stick[Stick_Display] - 150) * 0.5)), (BalkenPos+1), (63+((Pos_alt[Stick_Display] - 150) * 0.5)), 4, 0);
		
	// Balken zeichnen
		if (Pos_Stick[Stick_Display] >= 150)
			{
				lcd_frect (63, (BalkenPos+1), ((Pos_Stick[Stick_Display] - 150) * 0.5), 4, 1);
				//write_ndigit_number_u (4, 5, ((Pos_Stick[Stick_Display] - 150) * 1.25), 3, 0, 0);  // Pulse width in %
				lcd_frect(62, (BalkenPos), 2, 6, 1);  // 0%
			}
			else
			{
				lcd_frect (63 - ((150 - Pos_Stick[Stick_Display]) * 0.5), (BalkenPos+1), ((150 - Pos_Stick[Stick_Display]) * 0.5), 4, 1);
				//write_ndigit_number_u (4, 5, ((150 - Pos_Stick[Stick_Display]) * 1.25), 3, 0, 0);  // Pulse width in %
				lcd_frect(62, (BalkenPos), 2, 6, 1);  // 0%
			}
	// Raster zeichnen		
			lcd_line(3,  BalkenPos,3,  (BalkenPos+6),1);  // -150%
			lcd_line(23, BalkenPos,23, (BalkenPos+6),1);  // -100%
			lcd_line(43, BalkenPos,43, (BalkenPos+6),1);  // -50%
			lcd_line(83, BalkenPos,83, (BalkenPos+6),1);  // +50%
			lcd_line(103,BalkenPos,103,(BalkenPos+6),1);  // +100%
			lcd_line(123,BalkenPos,123,(BalkenPos+6),1);  // +150%	
}

