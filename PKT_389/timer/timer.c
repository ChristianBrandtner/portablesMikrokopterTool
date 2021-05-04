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
//# HISTORY  timer.c
//#
//#
//# 08.08.2015 CB
//# - add: timer_nmea_timeout
//#
//# 29.06.2014 OG
//# - chg: ISR( TIMER0_COMPA_vect ) - LipoCheck() wieder hinzugefuegt
//# - chg: LipoCheck() - leichte optische Anpassung und lcd-Ausgabe an ISR angepasst
//# - add: #include "../lipo/lipo.h"
//#
//# 24.06.2014 OG
//# - add: timer_gauge
//# - add: #include "../pkt/pkt.h"
//#
//# 13.06.2014 OG
//# - del: IdleTimer entfernt da nicht verwendet
//# - chg: ISR(TIMER0_COMPA_vect) - "PKT aus nach" (via timer_pkt_off)
//# - chg: ISR(TIMER0_COMPA_vect) - "LCD aus nach" umgestellt von Sekunden
//#        auf Minuten (via timer_lcd_off)
//# - chg: Code-Formattierung
//#
//# 31.05.2014 OG
//# - chg: 'timer_pktctrl' umbenannt zu 'timer_pktupdatecheck' (exklusiv fuer pkt.c)
//#
//# 25.04.2014 OG
//# - add: timer_get_displaydata
//#
//# 24.04.2014 OG
//# - add: timer1 (generischer Timer)
//# - add: timer3 (generischer Timer)
//# - add: timer_mk_timeout
//# - del: timer_get_erdata
//#
//# 29.03.2014 OG
//# - add: clear_key_all() - loescht ALLE Tasten egal ob short, long, repeat...
//#
//# 13.06.2013 cebra
//# - chg: Abfrage nach Hardwaresound bei Displaybeleuchtung entfällt
//#
//# 19.05.2013 OG
//# - add: timer_pktctrl - exclusive Verwendung von pkt.c
//#
//# 24.03.2013 OG
//# - add: UTCTime - inkrementiert wird UTCTime.seconds auf der Basis
//#        von timer_pkt_uptime (modulo 100) aktuell initialisert von
//#        der MK-NC Time durch OSD_MK_UTCTime()
//# - add: timer_get_tidata
//#
//# 21.03.2013 OG
//# - add: timer_pkt_uptime (auch in timer.h)
//# - add: timer_osd_refresh (auch in timer.h)  -> verwendet in osd.c
//# - add: timer_get_bldata (auch in timer.h)   -> verwendet in osd.c
//# - add: timer_get_erdata (auch in timer.h)   -> verwendet in osd.c
//#
//# 09.03.2013 OG
//# - add: timer2 (auch in timer.h)
//############################################################################


#include "../cpu.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <util/delay.h>
#include <inttypes.h>


#include "../main.h"
#include "../timer/timer.h"
#include "../eeprom/eeprom.h"
#include "../lcd/lcd.h"
#include "../uart/uart1.h"
#include "../bluetooth/bluetooth.h"
#include "../setup/setup.h"
#include"../tracking/tracking.h"
#include "../sound/pwmsine8bit.h"
#include "../lipo/lipo.h"
#include "../pkt/pkt.h"

//
//#if defined HWVERSION1_2W || defined HWVERSION1_2
//#include "HAL_HW1_2.h"
//#endif
//
//#if defined HWVERSION1_3W || defined HWVERSION1_3
//#include "HAL_HW1_3.h"
//#endif

//#ifdef HWVERSION3_9
#include "../HAL_HW3_9.h"
//#endif

//----------------------
// generische Timer
//----------------------
volatile uint16_t timer;
volatile uint16_t timer1;
volatile uint16_t timer2;
volatile uint16_t timer3;

//----------------------
// spezielle Timer
//----------------------
volatile uint16_t timer_pktupdatecheck;     // verwendet von pkt.c (!EXKLUSIV!)
volatile uint16_t timer_osd_refresh;        // verwendet in osd.c
volatile uint16_t timer_get_bldata;         // verwendet in osd.c
volatile uint16_t timer_get_tidata;         // verwendet in osd.c
volatile uint16_t timer_get_displaydata;    // verwendet in osd.c
volatile uint16_t timer_mk_timeout;         // verwendet u.a. in osd.c
volatile uint16_t timer_gauge;              // verwendet in pkt.c fuer den rotierenden Wartekreisel
volatile uint16_t timer_nmea_timeout;       // verwendet in nmea.c
volatile uint16_t abo_timer;

uint32_t timer_lcd_off = 0;                 // LCD aus nach... (der Zaehler muss 32Bit sein)
uint32_t timer_pkt_off = 0;                 // PKT aus nach... (der Zaehler muss 32Bit sein)

volatile uint16_t sound_timer;
volatile uint16_t soundpause_timer;
volatile static unsigned int tim_main;

volatile PKTdatetime_t  UTCTime;
volatile uint32_t timer_pkt_uptime = 0;     // OG: ja, 32 bit muss so sein!

uint8_t key_state = 0;                      // debounced and inverted key state:
                                            // bit = 1: key pressed
uint8_t key_press = 0;                      // key press detect
uint8_t key_long  = 0;                      // key long press
uint8_t key_rpt   = 0;                      // key repeat
uint8_t key_lrpt  = 0;                      // key long press and repeat
uint8_t key_rpts  = 0;                      // key long press and speed repeat
uint8_t repeat_speed = 0;


volatile uint8_t Display_on;                // LCD Flag Display on/off

volatile uint16_t WarnCount  = 0;           // Zähler der LIPO Warnzeit
volatile uint16_t WarnToggle = 0;           // Togglezähler zum blinken
volatile uint16_t WarnTime   = 10;          // Laenge der LIPO Warnzeit 10 Sek.
volatile uint16_t PoffTime   = 15;          // Laenge der Wartezeit vor abschalten 15 Sek.
//volatile uint16_t PoffTime   = 30;          // Laenge der Wartezeit vor abschalten 30 Sek.



unsigned int BeepTime   = 0;
unsigned int BeepMuster = 0xffff;
unsigned int BeepPrio   = 0;

volatile unsigned int CountMilliseconds = 0;



//--------------------------------------------------------------
// System (100Hz)
//--------------------------------------------------------------
void Timer0_Init( void )
{
    timer = 0;

    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS02) | (1 << CS00);
    OCR0A  = (F_CPU / (100L * 1024L)) ;

    TIMSK0 |= (1 << OCIE0A);        // enable interrupt for OCR
}



//--------------------------------------------------------------
//--------------------------------------------------------------
//void Timer1_Init (void) // Timer 1-A
//{
//  // löschen
//  TCCR1A = 0;
//  TCCR1B = 0;
//  TIMSK1 = 0;
//
//  // setzen
//  TCCR1A |=   (1 << COM1A1) | (1 << WGM11);
//  TCCR1B |=   (1 << CS11)   | (1 << CS10)   | (1 << WGM13)  | (1 << WGM12);
//
//  ICR1 = (F_CPU / 64) * 20 / 1000;
//
//  OCR1A = 470;  // ca. Servomitte
//}




//--------------------------------------------------------------
//--------------------------------------------------------------
ISR( TIMER2_COMPA_vect )
{
    PORTD |= (1 << PD7);
}


//--------------------------------------------------------------
//--------------------------------------------------------------
ISR( TIMER2_COMPB_vect )
{
    PORTD &= ~(1 << PD7);
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Timer2_Init( void )
{
    if( Config.HWSound == 1 )
    {                                                                   // Sound PWM
        TCCR2A = 0x00; //stop
        ASSR   = 0x00; //set async mode
        TCNT2  = 0x00; //setup
        OCR2A  = 0xff;
        //Fast PWM 0xFF BOTTOM MAX
        //Set OC2A on Compare Match
        //clkT2S/8 (From prescaler)
        TCCR2A |= (1 << WGM20) | (1 << WGM21) |(1 << COM2A1) | (1 << COM2A0);
        TCCR2B |= (1 << CS20);
    }
    else
    {                                                                   // Displayhelligkeit
        DDRD   |= (1 << DDD7);                                          // PD7 output
        TCCR2A |= (1 << WGM21)  | (1 << WGM20)  | (1 << COM2A1);        // non invers
        TCCR2B |= (1 << CS20);                                          // Prescaler 1/1
        TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B);
        OCR2A = 255;
    }
}



//--------------------------------------------------------------
// Sound, Timer CTC
//--------------------------------------------------------------
void Timer3_Init( void )
{
    TCCR3A = 0x00;              // stop
    TCNT3H = 0xF8;              // set count
    TCNT3L = 0x00;              // set count
    OCR3AH = 0x07;              // set compare
    OCR3AL = 0xFF;              // set compare
    TCCR3A |= (1 << WGM31);
    TCCR3B |= (1 << CS30);
    TIMSK3 |= (1 << OCIE3A);    // timer interrupt sources  2=t0 compare
}



//--------------------------------------------------------------
//--------------------------------------------------------------
#ifdef USE_SOUND
ISR(TIMER3_COMPA_vect)  // Sound
{
    //void timer0_comp_isr(void){
    //was 8 KHz 125usec  sampling rate
    //now 12 KHz 83usec  sampling rate
    unsigned char oldfpart;
    signed char fullsamp;
    signed int tmp;

    TCNT3 = 0;

    if( generate )
    {
        DDRD |= (1<<DDD7);              // Port aus Ausgang
        oldfpart=waveptfpart;           // remember fractional part
        waveptfpart+=freqincfpart;      // add frac part of freq inc to wave pointer
        if( waveptfpart < oldfpart )    // did it walk off the end?
        {
            waveptipart++;              // yes, bump integer part
        }
        waveptipart+=freqincipart;      // add int part of freq increment to wave pointer
        fullsamp=sindat[waveptipart];   // get 8 bit sin sample from table (signed)
        tmp=fullsamp*iattenfac;         // cvt 7 bit x 8 = 15 bit
        OCR2A=(tmp >> 8)+128;           // cvt 15 bit signed to 8 bit unsigned
    }
    else
    {
        DDRD &= ~(1 << DDD7);           // Port auf Eingang, sperrt das Rauschen
    }
}
#endif



//--------------------------------------------------------------
// Timer-Interrupt (100 Hz)
//--------------------------------------------------------------
ISR( TIMER0_COMPA_vect )
{
    static uint8_t ct0 = 0;
    static uint8_t ct1 = 0;
    static uint8_t k_time_l = 0;
    static uint8_t k_time_r = 0;
    static uint8_t k_time_lr = 0;
    static uint8_t k_time_rs = 0;
    uint8_t i;

    static unsigned char cnt_1ms = 1,cnt = 0;
    unsigned char beeper_ein = 0;


    i = key_state ^ ~KEY_PIN;                                       // key changed ?
    ct0 = ~(ct0 & i);                                               // reset or count ct0
    ct1 = ct0 ^ (ct1 & i);                                          // reset or count ct1
    i &= (ct0 & ct1);                                               // count until roll over ?
    key_state ^= i;                                                 // then toggle debounced state
    key_press |= (key_state & i);                                   // 0->1: key press detect


    if( !cnt-- )
    {
        cnt = 9;
        CountMilliseconds++;
        cnt_1ms++;
    }


    //--------------------------------
    // Key pressed -> Timer Reset
    //--------------------------------
    if( i!=0 )                                                      // eine Taste wurde gedrueckt! -> Timer Rest und weiteres...
    {
        if( Display_on == 0 )                                       // ggf. Displaylicht einschalten
          set_D_LIGHT();

        Display_on    = 1;                                          // Flag Display on
        timer_lcd_off = 0;                                          // Timer Reset
        timer_pkt_off = 0;                                          // Timer Reset
    }


    //--------------------------------
    // LCD off Timeout
    // LCD ausschalten nach n Minuten
    //--------------------------------
    if( (Config.DisplayTimeout > 0) && (Display_on == 1) )
    {
        timer_lcd_off++;
        if( (timer_lcd_off/(100*60)) == Config.DisplayTimeout )     // ISR laeuft mit 100Hz; umgerechnet auf Minuten
        {
            clr_D_LIGHT();                                          // Displaylicht ausschalten
            Display_on = 0;                                         // Flag Display off
        }
    }


    //--------------------------------
    // PKT off Timeout
    // PKT ausschalten nach n Minuten
    //--------------------------------
    if( Config.PKTOffTimeout > 0 )
    {
        timer_pkt_off++;
        if( (timer_pkt_off/(100*60)) == Config.PKTOffTimeout )      // ISR laeuft mit 100Hz; umgerechnet auf Minuten
        {
            WriteParameter();                                       // am Ende alle Parameter sichern
            set_beep( 50, 0xffff, BeepNormal );                     // ein Mini-Beep zum Abschied (laenger geht nicht, wahrscheinlich wegen der ISR)
            clr_V_On();                                             // Spannung abschalten
        }
    }


    //--------------------------------
    // PKT bei Unterspannung abschalten
    //--------------------------------
    LipoCheck();


    //--------------------------------
    // Beeper
    //--------------------------------
    if( Config.HWBeeper==1 )
    {
        if( BeepTime )
        {
            if( BeepTime > 10 )         BeepTime -= 10;
            else                        BeepTime = 0;

            if( BeepTime & BeepMuster ) beeper_ein = 1;
            else                        beeper_ein = 0;
        }
        else
        {
            beeper_ein = 0;
            BeepMuster = 0xffff;
            BeepPrio   = BeepNormal;
        }

        if( beeper_ein == 1 )           set_BEEP();
        else                            clr_BEEP();
    }


    //--------------------------------
    // Sound
    //--------------------------------
    #ifdef USE_SOUND
    if (sound_timer > 0)                                            // Ton spielen
    {
        sound_timer--;
    }
    else
    {
      //TIMSK2 &= ~_BV(TOIE2);                                      // Interrupt sperren, verhindert Störgeräusche
      //TCCR2A = 0x00; //stop
        generate = 0;                                               // Sound aus
        tone_handler();
        if (soundpause_timer > 0)
        {
          soundpause_timer --;        // Ton pause
        }
    }
    #endif


    //--------------------------------
    // Tasten
    //--------------------------------
    if ((key_state & LONG_MASK) == 0)                               // check long key function
        k_time_l = REPEAT_START;                                    // start delay

    if (--k_time_l == 0)                                            // long countdown
        key_long |= (key_state & LONG_MASK);

    //------
    if ((key_state & REPEAT_MASK) == 0)                             // check repeat function
        k_time_r = 1;                                               // kein delay

    if (--k_time_r == 0)
    {
        k_time_r = REPEAT_NEXT;                                     // repeat delay
        key_rpt |= (key_state & REPEAT_MASK);
    }

    //------
    if ((key_state & LONG_REPEAT_MASK) == 0)                        // check repeat function
        k_time_lr = REPEAT_START;                                   // start delay

    if (--k_time_lr == 0)
    {
        k_time_lr = REPEAT_NEXT;                                    // repeat delay
        key_lrpt |= (key_state & LONG_REPEAT_MASK);
    }

    //------
    if ((key_state & LONG_REPEAT_SP_MASK) == 0)                     // check repeatX function
        k_time_rs = REPEAT_START;                                   // start delay


    if( --k_time_rs == 0 )                                          // repeat countdown
    {
        if( repeat_speed == 1 )
        {
            k_time_rs = REPEAT_SPEED_1;
            key_rpts |= (key_state & LONG_REPEAT_SP_MASK);
        }
        else if( repeat_speed == 2 )
        {
            k_time_rs = REPEAT_SPEED_2;
            key_rpts |= (key_state & LONG_REPEAT_SP_MASK);
        }
        else if( repeat_speed == 3 )
        {
            k_time_rs = REPEAT_SPEED_3;
            key_rpts |= (key_state & LONG_REPEAT_SP_MASK);
        }
    }


    //--------------------------------
    // generische Timer
    //--------------------------------
    if( timer > 0 )                 timer --;
    if( timer1 > 0 )                timer1 --;
    if( timer2 > 0 )                timer2 --;
    if( timer3 > 0 )                timer3 --;


    //--------------------------------
    // spezielle Timer
    //--------------------------------
    if( timer_osd_refresh > 0 )     timer_osd_refresh--;            // Timer fuer OSD-Screenrefresh (verwendet von osd.c)
    if( timer_get_bldata > 0 )      timer_get_bldata--;             // Timer um BL-Daten zu holen (verwendet von osd.c)
    if( timer_get_tidata > 0 )      timer_get_tidata--;             // Timer um Datum/Zeit vom MK zu holen (verwendet von osd.c)
    if( timer_get_displaydata > 0 ) timer_get_displaydata--;        // Timer um Display vom MK zu holen (verwendet von osd.c)
    if( timer_mk_timeout > 0 )      timer_mk_timeout--;             // verwendet u.a. von osd.c
    if( abo_timer > 0 )             abo_timer --;                   // Timer zum anfordern neuer Abo-Datenpakete wie OSD oder BL-Daten (verwendet u.a. von osd.c)
    if( timer_pktupdatecheck > 0 )  timer_pktupdatecheck--;         // Timer fuer pkt.c (PKT-Update-Check) - * FUER NICHTS ANDERES! *
    if( timer_nmea_timeout > 0 )    timer_nmea_timeout--;           // verwendet u.a. von osd.c

    //--------------------------------
    //--------------------------------
    if( Gauge_active )                                              // Gauge_active -> pkt.c
    {
        if( timer_gauge > 0 )       timer_gauge--;
        if( timer_gauge==0 )        PKT_Gauge_Next();
    }


    //--------------------------------
    // PKT Uptime Timer
    //--------------------------------
    timer_pkt_uptime++;

    if( timer_pkt_uptime % 100 == 0 )                               // theoretisch muesste noch die Tagesgrenze (0 Uhr) implementiert werden...
        UTCTime.seconds++;

} // end: ISR(TIMER0_COMPA_vect)





//--------------------------------------------------------------
// Lowbatpin des Spannungswandlers pruefen
// LBO des LT1308 wechselt zum Ende der Batterielaufzeit haeufig seinen Zustand in der Uebergangsphase zum LowBat
// Die Akkuspannung schwankt auch abhaengig vom momentanen Stromverbrauch
//--------------------------------------------------------------
void LipoCheck( void )
{
    uint8_t lcd_xpos_save;
    uint8_t lcd_ypos_save;


    if( WarnToggle == 1 )                               // Beim ersten Auftreten Warnung ausgeben, Rythmus 5/10 Sekunden
    {
        set_beep( 1000, 0x0020, BeepNormal);

        lcd_xpos_save = lcd_xpos;                       // innerhalb einer ISR -> LCD Screenpos muss gesichert werden!
        lcd_ypos_save = lcd_ypos;

        //lcdx_cls_row( y, mode, yoffs )
        lcdx_cls_row( 0, MINVERS ,0 );                              // Zeile 0 komplett invers
        lcd_printp_at( 0, 0, PSTR(" ** PKT LiPo! ** "), MINVERS);   // und Warn-Text ausgeben

        lcd_xpos = lcd_xpos_save;                                   // lcd Screenpos wieder herstellen
        lcd_ypos = lcd_ypos_save;
    }


    if( WarnToggle == WarnTime * 100 )
        WarnToggle = 0;                                 // erstmal bis hier warnen


    if( WarnToggle > 0 )
        WarnToggle++;                                   // weiter hochzaehlen


    if( PINC & (1 << LowBat) )                          // Kurzzeitige Unterspannung bearbeiten und Warnung ausgeben
    {
        WarnCount = 0;
      //if (WarnCount > 0)
      //    WarnCount--;                                // Bei LIPO OK erstmal runterzaehlen, LT1308 ueberlegt sich noch genauer ob nun ok oder nicht
    }


    if( !(PINC & (1 << LowBat)) )                       // LT1308 hat Unterspannung erkannt
    {
        WarnCount++;                                    // solange LBO low ist ZÃ¤hler hochzÃ¤hlen
        if( (WarnCount == 10) && (WarnToggle == 0) )    // mit "10" etwas unempfindlicher gegen kurze Impulse machen
            WarnToggle = 1;                             // Warnhinweis starten
    }


    if( WarnCount == (PoffTime * 100) )
    {
        set_beep( 50, 0xffff, BeepNormal );             // ein Mini-Beep zum Abschied (laenger geht nicht, wahrscheinlich wegen der ISR)
        WriteParameter();                               // am Ende alle Parameter sichern
        clr_V_On();                                     // Spannung abschalten
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
//void LipoCheck_OLD( void )
//{
//    if( WarnToggle == 1 )                               // Beim ersten Auftreten Warnung ausgeben, Rythmus 5/10 Sekunden
//    {
//        set_beep ( 1000, 0x0020, BeepNormal);
//        lcd_printp_at (0, 0, PSTR("  LIPO  !!Warnung!!  "), 2);
//    }
//
//    if( WarnToggle == WarnTime * 100 )
//        WarnToggle = 0;                                 // erstmal bis hier warnen
//
//    if( WarnToggle > 0 )
//        WarnToggle++;                                   // weiter hochzaehlen
//
//    if( PINC & (1 << LowBat) )                          // Kurzzeitige Unterspannung bearbeiten und Warnung ausgeben
//    {
//        WarnCount = 0;
//      //if (WarnCount > 0)
//      //    WarnCount--;                                // Bei LIPO OK erstmal runterzaehlen, LT1308 ueberlegt sich noch genauer ob nun ok oder nicht
//    }
//
//    if (!(PINC & (1 << LowBat)) )                       // LT1308 hat Unterspannung erkannt
//    {
//        WarnCount++;                                    // solange LBO low ist ZÃ¤hler hochzÃ¤hlen
//        if( (WarnCount == 10) && (WarnToggle == 0) )    // mit "10" etwas unempfindlicher gegen kurze Impulse machen
//            WarnToggle = 1;                             // Warnhinweis starten
//    }
//
//    if( WarnCount == (PoffTime * 100) )
//    {
//        clr_V_On();                                     // Spannung abschalten
//  }
//}




//--------------------------------------------------------------
//--------------------------------------------------------------
unsigned int SetDelay( unsigned int t )
{
    return( CountMilliseconds + t + 1 );
}



//--------------------------------------------------------------
//--------------------------------------------------------------
char CheckDelay( unsigned int t )
{
    return( ((t - CountMilliseconds) & 0x8000) >> 9 );
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Delay_ms( unsigned int w )
{
    unsigned int akt;
    akt = SetDelay(w);
    while( !CheckDelay(akt) );
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t get_key_press( uint8_t key_mask )
{
    uint8_t sreg = SREG;

    cli();                      // disable all interrupts

    key_mask &= key_press;      // read key(s)
    key_press ^= key_mask;      // clear key(s)

    SREG = sreg;                // restore status register

    return key_mask;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t get_key_short( uint8_t key_mask )
{
    uint8_t ret;
    uint8_t sreg = SREG;

    cli();                          // disable all interrupts

    ret = get_key_press (~key_state & key_mask);

    SREG = sreg;                    // restore status register

    return ret;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t get_key_long( uint8_t key_mask )
{
    uint8_t sreg = SREG;

    cli();                          // disable all interrupts

    key_mask &= key_long;           // read key(s)
    key_long ^= key_mask;           // clear key(s)

    SREG = sreg;                    // restore status register

  return get_key_press (get_key_rpt (key_mask));
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t get_key_rpt( uint8_t key_mask )
{
    uint8_t sreg = SREG;

    cli();                          // disable all interrupts

    key_mask &= key_rpt;            // read key(s)
    key_rpt ^= key_mask;            // clear key(s)

    SREG = sreg;                    // restore status register

    return key_mask;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t get_key_long_rpt( uint8_t key_mask )
{
    uint8_t sreg = SREG;

    cli();                          // disable all interrupts

    key_mask &= key_lrpt;           // read key(s)
    key_lrpt ^= key_mask;           // clear key(s)

    SREG = sreg;                    // restore status register

    return get_key_rpt (~key_press^key_mask);
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t get_key_long_rpt_sp( uint8_t key_mask, uint8_t key_speed )
{
    uint8_t sreg = SREG;

    cli();                          // disable all interrupts

    key_mask &= key_rpts;           // read key(s)
    key_rpts ^= key_mask;           // clear key(s)

    repeat_speed = key_speed;

    SREG = sreg;                    // restore status register

    return key_mask;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void clear_key_all( void )
{
    key_rpt   = 0;                  // clear key(s)
    key_rpts  = 0;                  // clear key(s)
    key_lrpt  = 0;                  // clear key(s)
    key_long  = 0;                  // clear key(s)
    key_press = 0;                  // clear key(s)
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void set_beep( uint16_t Time, uint16_t Muster, uint8_t Prio)
{
    if( Config.HWBeeper == 1 )
    {
        if( (Prio == BeepNormal) && (BeepPrio == BeepNormal) )      // BEEPER: nur setzen wenn keine hohe Prio schon aktiv ist
        {
            BeepTime   = Time;
            BeepMuster = Muster;
        }

        if( (Prio == BeepSevere) && (!BeepPrio == BeepSevere) )     // BEEPER: hohe Prio setzen
        {
            BeepPrio   = BeepSevere;
            BeepTime   = Time;
            BeepMuster = Muster;
        }

        if( Prio == BeepOff )
        {
            BeepPrio   = BeepNormal;                                // BEEPER: Beep hohe Prio aus
            BeepTime   = 0;
            BeepMuster = 0;
        }
    } // end: if( Config.HWBeeper==1 )

    #ifdef USE_SOUND
    else
    {
        if( (Prio == BeepNormal) && (BeepPrio == BeepNormal) )      // SOUND: nur setzen wenn keine hohe Prio schon aktiv ist
        {
            playTone(900,Time/10,0);
        }

        if( (Prio == BeepSevere) && (!BeepPrio == BeepSevere) )
        {
            playTone(1200,Time/10,0);
            playTone(1100,Time/10,0);
        }

        if( Prio == BeepOff )
        {
            playTone(0,0,0);
        }
    }
    #endif // end: #ifdef USE_SOUND
}


