/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

#/*****************************************************************************
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
//# HISTORY  osd.h
//#
//# 21.06.2014 OG
//# - add: writex_altimeter()
//# - add: draw_icon_mk()
//#
//# 18.06.2014 OG
//# - add: weitere Exporte von Funktionen draw_icon...()
//# - add: MKLiPoCells_Init(), MKLiPoCells_Check()
//# - add: OSD_Element_Battery_Bar()
//# - chg: define ORIENTATION_H, ORIENTATION_V verschoben nach osd.h
//#
//# 01.06.2014 OG
//# Beginn von Auslagerungen von Code alter OSD-Screens nach osdold_screens.c/h
//#
//# 26.05.2014 OG
//# - add: #define OSDSCREEN_WAYPOINTS
//#
//# 24.05.2014 OG
//# - chg: OSD_Element_CompassDirection() - erweitert um xoffs,yoffs
//#
//# 28.04.2014 OG
//# - del: OSD_Timeout()
//#
//# 07.07.2013 OG
//# - add: defines fuer Screen-ID's (verwendet in setup.c, osd.c)
//#
//# 30.06.2013 OG
//# - chg: Benamung Statistik-Var's von mid_* auf avg_* geaendert
//#
//# 31.05.2013 OG
//# Achtung! Aenderung eeprom-Kompatibilitaet wegen typedef Erweiterung!
//# - chg: typedef: osd_statistic_BL_t fuer Mittelwerte
//# - chg: typedef: osd_statistic_t fuer Mittelwerte
//#
//# 15.05.2013 OG
//# - add: calc_BLmax()
//# - add: struct osd_BLmax_t
//#
//# 04.05.2013 OG
//# - chg: aktualisiert Kommentare in 'osd_statistic_t'
//# - del: OSD_Debug_XX()
//#
//# 02.05.2013 OG
//# - fix: struct osd_statistic_t: max_Distance von int16_t nach uint16_t
//#
//# 28.04.2013 OG
//# - chg: osd(OSDMode) -> osd(void)
//# - del: OSDDATA_Statistics()
//#
//# vorheriges: siehe osd.c
//############################################################################


#ifndef _OSD_H
#define _OSD_H

#include "../mk-data-structs.h"
#include "../timer/timer.h"

#define OSD_MAX_MOTORS          8   // max. Anzahl vom PKT unterstuetzer Motoren (absolut MAX ist 12 da der MK nicht mehr unterstuetzt)
#define MAX_GPS_USER           10   // max. Anzahl der GPS-Positionen durch Benutzer gespeichert
#define MAX_MKERR_LOG          20   // max. Anzahl Eintraege im MK-Error-Log

#define OSD_SCREEN_REFRESH      0   // Screen: Werte anzeigen
#define OSD_SCREEN_REDRAW       1   // Screen: Labels und statischer Elemente neu zeichen, Werte anzeigen


#define ORIENTATION_V           1   // fuer OSD_Element_Battery_Bar()
#define ORIENTATION_H           2


// OSD-Screen ID's
// maximal: 31 (!) wegen Bitcodierung in Config.OSD_UseScreen
#define OSDSCREEN_GENERAL       0
#define OSDSCREEN_NAVIGATION    1
#define OSDSCREEN_ELECTRIC      2
#define OSDSCREEN_MKSTATUS      3
#define OSDSCREEN_USERGPS       4
#define OSDSCREEN_3DLAGE        5
#define OSDSCREEN_STATISTICS    6
#define OSDSCREEN_OSD0          7
#define OSDSCREEN_OSD1          8
#define OSDSCREEN_OSD2          9
#define OSDSCREEN_WAYPOINTS    10


// Flags
#define OSD_FLAG_AH             0   // Altitue Hold
#define OSD_FLAG_PH             1   // Position Hold
#define OSD_FLAG_CF             2   // Care Free
#define OSD_FLAG_CH             3   // Coming Home
#define OSD_FLAG_O1             4   // Out1 (LED 1)
#define OSD_FLAG_O2             5   // Out2 (LED 2)
#define OSD_FLAG_BA             6   // LowBat warning (MK)
#define OSD_FLAG_CA             7   // Calibrate
#define OSD_FLAG_ST             8   // Start
#define OSD_FLAG_MR             9   // Motor Run
#define OSD_FLAG_FY            10   // Fly
#define OSD_FLAG_EL            11   // Emergency Landing
#define OSD_FLAG_FS            12   // RS Failsave Active
#define OSD_FLAG_GP            13   // GPS ok
#define OSD_FLAG_S0            14   // GPS-Sat not ok (GPS NOT ok)
#define OSD_FLAG_TU            15   // Vario Trim Up
#define OSD_FLAG_TD            16   // Vario Trim Down
#define OSD_FLAG_FR            17   // Free
#define OSD_FLAG_RL            18   // Range Limit
#define OSD_FLAG_SL            19   // No Serial Link
#define OSD_FLAG_TR            20   // Target Reached
#define OSD_FLAG_MC            21   // Manual Control

#define OSD_FLAG_COUNT         22


// Hier Höhenanzeigefehler Korrigieren
#define AltimeterAdjust       1.5


//-----------------------------------------------------------
// typedef: Statistiken
//-----------------------------------------------------------
typedef struct
{
    uint16_t            count;                  // Anzahl Werte BL-Daten (fuer Mittelwertberechnung)
    uint8_t             max_Current;            // in 0.1 A steps
    uint16_t            avg_Current;            // Mittelwert Current (*100 fuer Rechengenauigkeit)
    uint8_t             max_Temp;               // old BL-Ctrl will return a 255 here, the new version (>= V2.0) the temp. in °C
}  osd_statistic_BL_t;


typedef struct
{
    PKTdatetime_t       begin_StatTime;         // Datum/Zeit
    PKTdatetime_t       end_StatTime;           // Datum/Zeit
    uint16_t            total_FlyTime;          // gesamt Flugzeit seit Stat-Init
    uint16_t            last_FlyTime;           // letzte Flugzeit
    uint16_t            count_osd;              // TODO: Anzahl Werte OSD-Daten (fuer Mittelwertberechnung)
    uint16_t            count_Errorcode;        // TODO: Anzahl gemeldeter MK-Errors
    int16_t             max_Altimeter;          // max. Hoehe
    int16_t             avg_Altimeter;          // TODO: Mittelwert Hoehe ()
    s16                 max_Variometer;         // TODO: ...
    uint16_t            max_GroundSpeed;        // max. Geschwindigkeit
    uint16_t            avg_GroundSpeed;        // TODO: Mittelwert Geschwindigkeit ()
    s16                 max_TopSpeed;           // max. velocity in vertical direction in cm/s
    uint16_t            max_Distance;           // max. Entfernung
    uint16_t            avg_Distance;           // TODO: Mittelwert Entfernung ()
    uint16_t            max_Current;            // max. Strom
    uint16_t            avg_Current;            // Mittelwert Strom ()
    u8                  max_RCQuality;          // max. Empfangsqualitaet
    u8                  min_RCQuality;          // min. Empfangsqualitaet
    uint16_t            avg_RCQuality;          // TODO: Mittelwert Empfangsqualitaet ()
    uint16_t            max_Capacity;           // max. entnommene Kapazitaet
    s8                  max_AngleNick;          // max. Nick
    s8                  min_AngleNick;          // min. Nick
    s8                  max_AngleRoll;          // max. Roll
    s8                  min_AngleRoll;          // min. Roll
    uint8_t             min_UBat;               // min. Spannung (V)
    uint8_t             LiPoCells;              // Anzahl der LiPo Zellen
    uint8_t             BL_Count;               // Anzahl erkannter BL's (Motoren)
    osd_statistic_BL_t  BL[OSD_MAX_MOTORS];     // Werte der einzelnen BL's
}  osd_statistic_t;


typedef struct
{
    uint8_t             max_BL_Current_Index;   // BL-Nummer
    unsigned char       max_BL_Current;         // in 0.1 A steps
    uint8_t             max_BL_Temp_Index;      // BL-Nummer
    unsigned char       max_BL_Temp;            // old BL-Ctrl will return a 255 here, the new version (>= V2.0) the temp. in °C
}  osd_BLmax_t;


//-----------------------------------------------------------
// typedef: Aufzeichnung von MK-Errors
//-----------------------------------------------------------
typedef struct
{
    u8  Errorcode;                              // 0 --> okay
    PKTdatetime_t       set_Time;               // Datum/Zeit
    PKTdatetime_t       clear_Time;             // Datum/Zeit
}  mkerror_t;



//-----------------------------------------------------------
// typedef: PKT GPS-Positionen
//-----------------------------------------------------------
typedef struct
{
    PKTdatetime_t       timestamp;              // Zeitstempel: UTC
    s16                 Altimeter;              // barymetrische Hoehe      (entspricht: naviData->Altimeter)
    u16                 HomeDistance;           // distance to home in cm   (entspricht: naviData->HomePositionDeviation.Distance)
    GPS_Pos_t           GPSData;                // GPS-Position (mk-data-structs.h)
}  pkt_gpspos_t;



//-----------------------------------------------------------
// global var's
//-----------------------------------------------------------
extern volatile uint8_t OSD_active;
extern volatile uint8_t error;

//-----------------------------------------------------------
// strings
//-----------------------------------------------------------
extern const char * const mkerrortext[];


//-----------------------------------------------------------
// Funktionen
//-----------------------------------------------------------
void osd( void );
void vario_beep_output (void);
void CheckMKLipo(void);
void STAT_Init(void);
void GPS_User_Init(void);
void MKErr_Log_Init(void);
void calc_BLmax( osd_BLmax_t *blmax );


void OSD_Element_Flag_Label( uint8_t xC, uint8_t yC, uint8_t item, uint8_t lOn, int8_t xoffs, int8_t yoffs);
void OSD_Element_Flag( uint8_t xC, uint8_t yC, uint8_t item, int8_t xoffs, int8_t yoffs);
void OSD_Element_Altitude( uint8_t x, uint8_t y, uint8_t nStyle );
void OSD_Element_BattLevel2( uint8_t x, uint8_t y, int8_t xoffs, int8_t yoffs );
void OSD_Element_BatteryLevel_Bar( uint8_t x, uint8_t y );
void OSD_Element_BatteryLevel_Text( uint8_t x, uint8_t y, uint8_t nStyle );
void OSD_Element_BatteryLevel( uint8_t x, uint8_t y, uint8_t nStyle );
void OSD_Element_Capacity( uint8_t x, uint8_t y );
void OSD_Element_CompassDegree( uint8_t x, uint8_t y, uint8_t nStyle );
void OSD_Element_CompassDirection( uint8_t x, uint8_t y, int8_t xoffs, int8_t yoffs );
void OSD_Element_CompassRose( uint8_t x, uint8_t y );
void OSD_Element_Current( uint8_t x, uint8_t y );
void OSD_Element_FlyingTime( uint8_t x, uint8_t y );
void OSD_Element_GroundSpeed( uint8_t x, uint8_t y );
void OSD_Element_HomeCircle( uint8_t x, uint8_t y, uint8_t breite, int8_t rOffset, int8_t xoffs, int8_t yoffs );
void OSD_Element_HomeDegree( uint8_t x, uint8_t y );
void OSD_Element_HomeDistance( uint8_t x, uint8_t y, uint8_t nStyle );
void OSD_Element_LEDOutput( uint8_t x, uint8_t y, uint8_t bitmask );
void OSD_Element_LED1Output( uint8_t x, uint8_t y );
void OSD_Element_LED2Output( uint8_t x, uint8_t y );
void OSD_Element_Manuell( uint8_t x, uint8_t y );
void OSD_Element_RCIntensity( uint8_t x, uint8_t y );
void OSD_Element_SatsInUse( uint8_t x, uint8_t y, uint8_t  nStyle );
void OSD_Element_Variometer( uint8_t x, uint8_t y );
void OSD_Element_Target( uint8_t x, uint8_t y, uint8_t nStyle );
void OSD_Element_VarioWert( uint8_t x, uint8_t y );
void OSD_Element_WayPoint( uint8_t x, uint8_t y );
void OSD_Element_TargetDegree( uint8_t x, uint8_t y );
void OSD_Element_UpDown( uint8_t x, uint8_t y, int8_t xoffs, int8_t yoffs);
void OSD_Element_Battery_Bar( uint8_t x, uint8_t y, uint8_t length, uint8_t width, uint8_t orientation);

void writex_altimeter( uint8_t x, uint8_t y, s32 Altimeter, uint8_t mode, int8_t xoffs, int8_t yoffs );

void MKLiPoCells_Init( void );
void MKLiPoCells_Check( void );

void draw_icon_satmini( uint8_t x, uint8_t y);
void draw_icon_satmini2( uint8_t x, uint8_t y);
void draw_icon_home( uint8_t x, uint8_t y);
void draw_icon_target_diamond( uint8_t x, uint8_t y);
void draw_icon_target_round( uint8_t x, uint8_t y);
void draw_icon_mk( uint8_t x, uint8_t y);

void OSD_MK_ShowTimeout( void );


//-----------------------------------------------------------
// EXPORTS NUR FUER osdold_screens.c
//-----------------------------------------------------------
extern NaviData_t          *naviData;
extern uint8_t             OSDScreenRefresh;


#endif  // _OSD_H
