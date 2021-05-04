/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
 *   Copyright (C) 2008 Thomas Kaiser, thomas@ft-fanpage.de                  *
 *   Copyright (C) 2009 Peter "woggle" Mack, mac@denich.net                  *
 *   Copyright (C) 2012 Christian "Cebra" Brandtner, brandtner@brandtner.net *
 *   Copyright (C) 2012 Harald Bongartz                                      *
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
//# HISTORY  pkt.h
//#
//# 25.09.2015 Cebra
//# - add: PKTDEBUG für serielle Debugfunktionen
//#
//# 25.06.2014 OG
//# - add: PKT_AskX()
//#
//# 24.06.2014 OG
//# - add: PKT_Gauge_Begin(), PKT_Gauge_End(), PKT_Gauge_Next()
//#
//# 15.06.2014 OG
//# - add: PKT_Progress_Init(), PKT_Progress_Next()
//#
//# 14.06.2014 OG
//# - add: PKT_TitlePKTlipo() fuer optionale Anzeige der LiPo-Spannung
//#
//# 13.06.2014 OG
//# - add: PKT_Ask_Restart()
//#
//# 12.06.2014 OG
//# - add: PKT_Reset_EEprom()
//# - add: PKT_Ask(), PKT_Ask_P()
//#
//# 11.06.2014 OG
//# - add: PKT_Title(), PKT_Title_P()
//#
//# 04.06.2014 OG
//# - add: PKT_Message_Datenverlust()
//#
//# 31.05.2014 OG
//# - add: PKT_KeylineUpDown()
//#
//# 23.05.2014 OG
//# - add: PKT_TitleFromMenuItem()
//#
//# 06.05.2014 OG
//# - add: PKT_Popup(), PKT_Popup_P()
//#
//# 04.04.2014 OG
//# - fix: define ESC umbenannt zu PKTESC da es einen Namenskonflikt mit enum
//#        STR in messages.h gab
//#
//# 21.02.2014 OG
//# - chg: PKT_TitlePKTVersion()
//#
//# 17.02.2014 OG
//# - add: PKT_Message(), PKT_Message_P()
//#
//# 13.02.2014 OG
//# - add: PKT_print_PKT_center()
//#
//# 03.02.2014 OG
//# - add: PKT_ShowTitlePKTVersion()
//#
//# 13.06.2013 OG
//# - del: PC_Fast_Connect() - ersetzt durch Menu_PKTTools() in menu.c
//#
//# 19.05.2013 OG
//# - Ausgliederungen aus verschiedenen anderen Sourcen
//############################################################################

#ifndef _PKT_H
#define _PKT_H

#define PKTESC       27         // Parameter für PKT-Updatetool
#define PKTUPDATE    'u'        // starte Update
#define PKTVERSION   'v'        // sende PKT EEpromversion
#define PKTSENDCONF  'e'        // sende PKT Konfigdaten
#define PKTDEBUG     'd'        // startet die serielle Kommunikation

#define ASK_YES_NO      1
#define ASK_NO_YES      2
#define ASK_CANCEL_OK   3
#define ASK_END_OK      4
#define ASK_END_START   5


extern volatile uint8_t Gauge_active;


void PKT_Title( const char *text, uint8_t lShowLipo, uint8_t clearscreen );
void PKT_Title_P( const char *text, uint8_t lShowLipo, uint8_t clearscreen );
void PKT_TitlePKT( void );
void PKT_TitlePKTlipo( uint8_t lShowLipo );
void PKT_TitleFromMenuItem( uint8_t lShowLipo );

void PKT_KeylineUpDown( uint8_t xUp, uint8_t xDown,  uint8_t xoffs, uint8_t yoffs);

void PKT_Progress_Init( uint8_t max, int8_t yoffs, uint8_t width, uint8_t height);
uint8_t PKT_Progress_Next( void );

void PKT_Gauge_Begin( uint8_t py);
void PKT_Gauge_End( void );
void PKT_Gauge_Next( void );

void PKT_Message( const char *text, uint8_t error, uint16_t timeout, uint8_t beep, uint8_t clearscreen );
void PKT_Message_P( const char *text, uint8_t error, uint16_t timeout, uint8_t beep, uint8_t clearscreen );

void PKT_Message_Datenverlust( uint16_t timeout, uint8_t beep );

void PKT_Popup( uint16_t timeout, const char *text1, const char *text2, const char *text3, const char *text4 );
void PKT_Popup_P( uint16_t timeout, const char *text1, const char *text2, const char *text3, const char *text4 );

uint8_t PKT_Ask( uint8_t asktype, const char *text1, const char *text2, const char *headline, const char *title );
uint8_t PKT_Ask_P( uint8_t asktype, const char *text1, const char *text2, const char *headline, const char *title );
uint8_t PKT_AskX( uint8_t asktype, const char *text1, const char *text2, uint8_t text_progmem, const char *headline, uint8_t headline_progmem, const char *title, uint8_t title_progmem );

uint8_t PKT_CtrlHook(void);
uint8_t PKT_CheckUpdate(void);  // prüft im Hauptmenü ob das Updatetool ein Update machen will

void PKT_Reset_EEprom( void );
void PKT_Update(void);
void PKT_SwitchOff(void);
void PKT_Info(void);
void PKT_Ask_Restart( const char *title );

#endif  // end: #ifndef _PKT_H
