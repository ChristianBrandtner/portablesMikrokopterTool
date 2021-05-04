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
//# HISTORY  setup.h
//#
//# 26.06.2014 OG
//# - add: Setup_FollowMe()
//#
//# 22.06.2014 OG
//# - del: Variable CheckGPS
//#
//# 13.06.2014 OG
//# - del: ChangeWi_SV2()
//#
//# 11.06.2014 OG
//# - del: Edit_Language()
//#
//# 31.04.2014 OG
//# - chg: Edit_String() - Parameter 'Text' entfernt
//#
//# 28.05.2014 OG
//# - chg: Edit_generic() - geaenderte Aufrufparameter
//#        - Parameter 'Text' enfernt da vom Menue geerbt
//#        - Parameter fuer Hilfetexte
//#
//# 04.04.2014 OG
//# - del: Edit_ShowTitle_P()
//#
//# 02.04.2014 OG
//# - add: Edit_ShowTitle_P()
//# - add: WlanMode
//#
//# 15.07.2013 Cebra
//# - add: Edit_Printout um WlanSecurity erweitert
//#
//# 22.05.2013 OG
//# - del: verschiedene Cleanups
//############################################################################


#ifndef _setup_H
#define _setup_H


#define EDIT_BT_NAME     1
#define EDIT_BT_PIN      2
#define EDIT_SSID        3
#define EDIT_WL_PASSWORD 4


typedef enum  // Varianten fuer die Anzeige der Werte im Setupmenüe
{
  Show_uint3, Show_int3, Show_uint5, Show_uint10th, GPSMOUSE, Wi_Netmode,
  OnOff, YesNo, Orientation, NormRev, Kontrast, Baudrate, Language, Sticktype, WlanMode, WlanSecurity, PKT_Akku, MK_Connection
} EditMode;

void Setup_MAIN(void);
void Setup_FollowMe( void );

void Display_Setup (void);
void Wi_Use (void);
void BT_Use (void);
void BT_SelectDevice (void);
uint8_t BT_CheckBTM222( void );

void Wait4KeyOK( void );

int16_t Edit_generic( int16_t Value, int16_t min, int16_t max, uint8_t what, uint8_t Addfactor, const char *strHelp1, const char *strHelp2 );
uint8_t Edit_String( const char *data, const uint8_t length, uint8_t type);

uint8_t BT_ShowGPSData( uint8_t modus );

void BTM222_Info( void );

extern uint8_t bt_name_len;
extern uint8_t length_tmp;
extern uint8_t Old_Baudrate;              //Merkzelle für alte Baudrate
extern uint8_t edit;
extern char    EditString[21];
#endif
