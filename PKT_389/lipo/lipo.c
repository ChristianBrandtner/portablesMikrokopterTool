/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
 *   Copyright (C) 2008 Thomas Kaiser, thomas@ft-fanpage.de                  *
 *   Copyright (C) 2009 Peter "woggle" Mack, mac@denich.net                  *
 *   based on the key handling by Peter Dannegger                            *
 *     see www.mikrocontroller.net                                           *
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
//# HISTORY  lipo.c
//#
//# 15.06.2014 OG
//# - fix; show_Lipo() - wenn die Spannung unter 3.20 Volt ging, wurde wieder
//#        ein voller Akkubalken angezeigt aufgrund einer Subtraktion - fixed
//#
//# 21.02.2014 OG
//# - chg: show_Lipo() neu geschrieben - etwas smarter, ein paar Bytes kleiner
//#
//# 20.02.2014 OG
//# - chg: Codeformattierungen
//############################################################################


#include "../cpu.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "../main.h"
#include "../lcd/lcd.h"
#include "lipo.h"
#include "../eeprom/eeprom.h"


// Global variables
double    accumulator = 0;  //!< Accumulated 10-bit samples
double    Vin         = 0;  //!< 16-bit float number result
short     temp        = 0;  //!< Temporary variable
short     samples     = 0;  //!< Number of conversions
uint16_t  volt_avg    = 0;



//--------------------------------------------------------------
//! ADC interrupt routine
//--------------------------------------------------------------
ISR (ADC_vect)
{
  accumulator += ADCW;
  samples++;

  if( samples>4095 )
  {
    oversampled();
  }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void ADC_Init (void)
{
    DDRA   &= ~(1<<DDA1);       // MartinR
    PORTA  &= ~(1<<PORTA1);  // MartinR
    ADMUX  = (0<<REFS1) | (1<<REFS0);      // externe 5V Referenzspannung nutzen
    ADMUX  = (ADMUX & ~(0x1F)) | (1 & 0x1F); // ADC1 verwenden
    ADCSRA = (1<<ADEN)|(1<<ADIE)|(1<<ADSC)|(1<<ADATE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // Prescaler 128, Freilaufend, Interrupte frei
}




//--------------------------------------------------------------
// Error compensation, Scaling 16-bit result, Rounding up,
//  Calculate 16-bit result, Resets variables
//
// Quelle AVR121: Enhancing ADC resolution by versampling
//--------------------------------------------------------------
void oversampled(void)
{
    cli();
    accumulator += Config.Lipo_UOffset;         //5150 Offset error compensation

    //  accumulator *= 0.9993;                  // Gain error compensation
    accumulator *= 0.9600;                      //0.9800 Gain error compensation
    temp=(int)accumulator%64;
    accumulator/=64;                            // Scaling the answer

    if(temp>=32)
    {
      accumulator += 1;                         // Round up
    }

    // Vin = (accumulator/65536)*4.910;         // Calculating 16-bit result

    Vin =accumulator/7.5;
    volt_avg = Vin;

    // write_ndigit_number_u(0, 3, Vin, 5, 0,0);
    // write_ndigit_number_u(0, 4, volt_avg, 5, 0,0);

    samples     = 0;
    accumulator = 0;

    sei();
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void show_Lipo( void )
{
    int16_t w = 0;

    //--------------------------------------------
    // Batterie Symbol zeichnen
    //--------------------------------------------
    lcd_rect(104, 0, 23, 7, 1);     // Rahmen aussen
    lcd_rect(105, 1, 21, 5, 0);     // Rahmen innen
    lcd_frect(102, 0, 1, 7, 0);     // ganz links etwas loeschen
    lcd_rect(103,2,1,3,1);          // Batterie "Kappe"


    //--------------------------------------------
    // Batterie Balken berechnen / zeichnen
    //--------------------------------------------
    //
    // Config.PKT_Accutyp
    //    true  = LiPO Akku mit max. 4.2 Volt (420)
    //    false = LiON Akku mit max. 4.1 Volt (410)

    if( volt_avg > 320 )
    {
        w = (volt_avg-320)/(Config.PKT_Accutyp ? 4.8 : 4.5);            // die Faktoren 4.8 (kann bis 5.0) und 4.5 fuer LiPo / LiON wurden per Test ermittelt
        //lcdx_printf_at_P( 12, 1, MNORMAL, 0,0, PSTR("v=%3d"), w );    // Debug Anzeige um Faktoren zu ermitteln
    }

    if(w<0)  w =  0;
    if(w>20) w = 20;                                                    // nicht mehr als 20 Pixel Breite

    if( w>0 )
      lcd_frect( 106+(20-w), 2, w-1, 3, 1);                             // Batterie Balken: fill

    if( w<20 )
      lcd_frect( 106, 2, 19-w, 3, 0);                                   // Batterie Balken: clear
}

