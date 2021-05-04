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
//# HISTORY  timer.h
//#
//# 08.08.2015 CB
//# - add: timer_nmea_timeout
//#
//# 24.06.2014 OG
//# - add: timer_gauge
//#
//# 13.06.2014 OG
//# - del: IdleTimer entfernt da nicht verwendet
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
//# - add: clear_key_all()
//#
//# 07.07.2013 OG
//# - chg: ABO_TIMEOUT verschoben nach main.h
//#
//# 19.05.2013 OG
//# - add: timer_pktctrl - exclusive Verwendung von pkt.c
//############################################################################



#ifndef _TIMER_H
#define _TIMER_H

#include "../cpu.h"
#include "../main.h"

typedef struct
{
    uint32_t    seconds;    // seconds since nnew day
    uint8_t     day;
    uint8_t     month;
    uint16_t    year;
}  PKTdatetime_t;


#define KEY_ALL             ((1 << KEY_PLUS) | (1 << KEY_MINUS) | (1 << KEY_ENTER) | (1 << KEY_ESC))
#define LONG_MASK           ((1 << KEY_PLUS) | (1 << KEY_MINUS) | (1 << KEY_ENTER) | (1 << KEY_ESC))
#define REPEAT_MASK         ((1 << KEY_PLUS) | (1 << KEY_MINUS) | (1 << KEY_ENTER) | (1 << KEY_ESC))
#define LONG_REPEAT_MASK    ((1 << KEY_PLUS) | (1 << KEY_MINUS) | (1 << KEY_ENTER) | (1 << KEY_ESC))
#define LONG_REPEAT_SP_MASK ((1 << KEY_PLUS) | (1 << KEY_MINUS) | (1 << KEY_ENTER) | (1 << KEY_ESC))

#define REPEAT_START       70   // after 700ms
#define REPEAT_NEXT        15   // every 150ms
#define REPEAT_SPEED_1     20   // every 200ms
#define REPEAT_SPEED_2      8   // every  80ms
#define REPEAT_SPEED_3      1   // every  10ms

#define BeepNormal          0   // Normal Beep
#define BeepSevere          1   // schwerer Fehler, Beep nicht unterbrechbar
#define BeepOff             2   // Beep aus

extern volatile uint8_t Display_on;

extern volatile PKTdatetime_t   UTCTime;

//----------------------
// generische Timer
//----------------------
extern volatile uint16_t timer;
extern volatile uint16_t timer1;
extern volatile uint16_t timer2;
extern volatile uint16_t timer3;

//----------------------
// spezielle Timer
//----------------------
extern volatile uint32_t timer_pkt_uptime;
extern volatile uint16_t timer_pktupdatecheck;      // verwendet von pkt.c (!EXKLUSIV! FUER NICHTS ANDERES!)
extern volatile uint16_t timer_osd_refresh;
extern volatile uint16_t timer_get_bldata;
extern volatile uint16_t timer_get_tidata;
extern volatile uint16_t timer_get_displaydata;
extern volatile uint16_t timer_mk_timeout;          // verwendet u.a. in osd.c
extern volatile uint16_t timer_gauge;              // verwendet in pkt.c fuer den rotierenden Wartekreisel
extern volatile uint16_t abo_timer;
extern volatile uint16_t timer_nmea_timeout;       // verwendet in nmea.c

extern volatile uint16_t sound_timer;
extern volatile uint16_t soundpause_timer;
//extern volatile uint16_t WarnCount;




//extern volatile unsigned int BeepTime;
extern unsigned int BeepTime;
extern unsigned int BeepMuster;


/**
* Get the value of the system timer counter.
*
* @return The value of the system timer counter.
*/
uint16_t timer_get_system_count(void);

void Timer0_Init (void);  // Systeminterrupt
void Timer1_Init (void);  // Servotester
void Timer2_Init (void);  // Displayhelligkeit
void Timer3_Init (void);  // Überwachung
uint8_t get_key_press (uint8_t key_mask);  // sofort beim drÃ¼cken
uint8_t get_key_short (uint8_t key_mask);  // erst beim loslassen
uint8_t get_key_long (uint8_t key_mask);  // verzÃ¶gert
uint8_t get_key_rpt (uint8_t key_mask);  // mit verzÃ¶gerung
uint8_t get_key_long_rpt (uint8_t key_mask);  //
uint8_t get_key_long_rpt_sp (uint8_t key_mask, uint8_t key_speed);  // mit verzÃ¶gerung und 3 versch. geschw.
void clear_key_all( void );

void set_beep ( uint16_t Time, uint16_t Muster, uint8_t Prio);
extern volatile unsigned int CountMilliseconds;


void Delay_ms(unsigned int);
unsigned int SetDelay (unsigned int t);
char CheckDelay (unsigned int t);
void LipoCheck (void); // Lowbatpin des Spannungswandlers prÃ¼fen


#endif
