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
//# HISTORY  mkbase.h
//#
//# 14.05.2014 OG
//# - chg: include "paramset.h" geaendert auf "../mksettings/paramset.h"
//#
//# 29.03.2014 OG
//# - del: MK_Show_LastGPS_Position() -> jetzt: OSDDATA_ShowLastGPSPosition()/osddata.c
//#
//# 28.03.2014 OG
//# - add: MK_Show_LastGPS_Position() - ehemals in main.c
//#
//# 16.02.2014 OG
//# - add: MK_SwitchToNC(), MK_SwitchToFC(), MK_SwitchToMAG(), MK_SwitchToGPS()
//# - add: MK_Setting_write(), MK_Setting_change()
//# - chg: umbenannt: MK_load_setting() zu MK_Setting_load()
//#
//# 13.02.2014 OG
//# - add: MKVersion_Cmp()
//# - add: defines zu VERSION... fuer MKVersion_Cmp()
//# - del: WrongFCVersion
//#
//# 10.02.2014 OG
//# - add: MKVersion_Setting_print()
//#
//# 09.02.2014 OG
//# - add: MK_Info()
//# - add: MKVersion_print_at
//#
//# 08.02.2014 OG
//# - chg: MK_load_setting() Parameter geaendert
//# - add: extern MKVersion_t MKVersion
//#
//# 29.01.2014 OG
//# - Ausgliederungen aus main.c
//############################################################################

#ifndef _MKBASE_H
#define _MKBASE_H

//#include "../mksettings/paramset.h"
#include "../mk-data-structs.h"


//-------------------------------------
//-------------------------------------
typedef struct
{
    unsigned char   isFC;                   // true / false - FC vorhanden?     -> wird gesetzt durch searchMK()
    unsigned char   isNC;                   // true / false - NC vorhanden?     -> wird gesetzt durch searchMK()
    Version_t       FCVer;                  //                                  -> wird gesetzt durch searchMK()
    Version_t       NCVer;                  //                                  -> wird gesetzt durch searchMK()
    unsigned char   paramsetOK;             // true wenn Revision in paramset.c vorhanden und initialisiert     -> wird gesetzt druch paramsetInit()/paramset.c
    unsigned char   paramsetRevision;       // Revision FC-Parameterset                                         -> wird gesetzt druch paramsetInit()/paramset.c
    uint8_t         mksetting;              //      -> wird gesetzt druch paramsetInit()/paramset.c
    unsigned char   mksettingName[13];      //      -> wird gesetzt druch paramsetInit()/paramset.c
} MKVersion_t;


//-------------------------------------
// zur Orientierung: Version_t
//-------------------------------------
//typedef struct
//{
//    unsigned char SWMajor;
//    unsigned char SWMinor;
//    unsigned char ProtoMajor;
//    unsigned char ProtoMinor;
//    unsigned char SWPatch;
//    unsigned char HardwareError[5];
//} __attribute__((packed)) Version_t;

//----------------------------
// MK-Versionsinformationen
// global verfuegbar
//----------------------------
extern MKVersion_t MKVersion;


//------------------------------------------------------
// defines fuer den Versionsvergleich von FC/NC
// siehe: MKVersion_Cmp()... (mkbase.c)
//------------------------------------------------------
#define GIVEN_VERSION       2   // das macht die Sache leserlicher fuer resultierenden Ergebnisse! => siehe Anmerkungen: MKVersion_Cmp() !
#define VERSION_NO          0
#define VERSION_LOWER       1
#define VERSION_EQUAL       2
#define VERSION_GREATER     3


void    MKVersion_Init( void );
void    MKVersion_print_at( uint8_t x, uint8_t y, uint8_t what, uint8_t drawmode, int8_t xoffs, int8_t yoffs );
void    MKVersion_Setting_print( uint8_t y, uint8_t drawmode, uint8_t xoffs, uint8_t yoffs );
uint8_t MKVersion_Cmp( Version_t ver, unsigned char SWMajor, unsigned char SWMinor, unsigned char SWPatch );

void    MK_SwitchToNC( void );
void    MK_SwitchToFC( void );
void    MK_SwitchToMAG( void );
void    MK_SwitchToGPS( void );

uint8_t MK_Setting_load( uint8_t lsetting, uint8_t timeout );
uint8_t MK_Setting_write( uint8_t wsetting, uint8_t timeout);
uint8_t MK_Setting_change( uint8_t setting );

uint8_t MK_Info( uint16_t displaytimeout, uint8_t refreshSettings );

uint8_t searchMK( uint8_t showMKInfo );


#endif  // end: #ifndef _MKBASE_H
