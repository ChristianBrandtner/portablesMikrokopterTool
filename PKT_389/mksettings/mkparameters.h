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
//# HISTORY  mkparameters.h
//#
//# 27.03.2014 OG
//# etliche neue Funktionen / Strukturen
//#
//# 23.02.2014 OG
//# - chg: MK_Parameters_Menu() umbenannt zu MK_Parameters()
//#
//# 12.02.2014 OG - NEU
//############################################################################

#ifndef _MKPARAMETERS_H
#define _MKPARAMETERS_H


// fuer cmd-Parameter der Edit-Funktionen
#define MKPARAM_EDIT            1       // Wert bearbeiten
#define MKPARAM_SHORTVALUE      2       // erzeugt einen 3-Zeichen String der den Wert darstellt (z.B. "Dis"); Ergebnis in strValueBuffer
#define MKPARAM_LONGVALUE       3       // erzeugt einen String der den Wert in langer Form darstellt (z.B. "Disabled"); Ergebnis in strValueBuffer


#define MKPARAM_STRBUFFER_LEN   20


//--------------------------------------------
// paramEditItem_t
// deklariert die Edit-Daten einer paramID
//--------------------------------------------
typedef struct
{   
    unsigned char paramID;                                  // paramID aus paramset.h
    void (*editfunc)(unsigned char paramID, uint8_t cmd);   // Edit-Funktion (z.B. editGeneric()); cmd = CMD_EDIT oder CMD_SHORTVALUE
    const char    *format;                                  // Parameter: String (PROGMEM) (vorallem fuer editGeneric() )
    unsigned char  min;                                     // Parameter: min (P1)
    unsigned char  max;                                     // Parameter: max (P2)
    const char    *title_de;                                // Text in PROGMEM - Menuetext und Beschreibung im Edit-Screen
    const char    *title_en;                                // Text in PROGMEM
} paramEditItem_t;



extern paramEditItem_t  paramEditItem;                                  // RAM Buffer: fuer ein Element von paramEditDef
extern char             paramEditFormat[MKPARAM_STRBUFFER_LEN];         // RAM Buffer: fuer 'str' von paramEdit (Format; editGeneric)
extern char             mkparam_strValueBuffer[MKPARAM_STRBUFFER_LEN];  // Anzeige eines Values als Klartext; Kurz (fuer das Menue) oder Lang (in der Edit-Funktion)


//---------------------------
// exportierte Funktionen
//---------------------------
unsigned char find_paramEditItem( unsigned char paramID );
void editGeneric( unsigned char paramID, uint8_t cmd );
void editBitmask( unsigned char paramID, uint8_t cmd );
void editDisableDeclCalc( unsigned char paramID, uint8_t cmd );
void editCompassOffset( unsigned char paramID, uint8_t cmd );
void editACCZLandingPulse( unsigned char paramID, uint8_t cmd );
void editNA( unsigned char paramID, uint8_t cmd );

uint8_t MK_Parameters( uint8_t setting, char *settingname );


#endif  // _MKPARAMETERS_H
