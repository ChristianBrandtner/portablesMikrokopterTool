/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
 *   Copyright (C) 2013 Oliver Gemesi                                        *
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
 *****************************************************************************/
//memcpy( &Config.
//############################################################################
//# HISTORY  osddata.c
//#
//# 14.06.2014 OG
//# - chg: Menu_OSDData() - Bezeichnung von "OSD Daten" und "BL Daten" geaendert
//#        zu "OSD Statistik" und "BL Statistik"
//#
//# 11.05.2014 OG
//# - chg: Text von SHOWMKERROR_de, SHOWMKERROR_en etwas gekuerzt
//# - chg: Titel von Scrollbox-Listen angepasst
//# - chg: Menu_OSDData() umgestellt auf MenuCtrl_SetTitleFromParentItem()
//#        -> die Menues 'erben' damit ihren Titel vom aufrufenden Menuepunkt
//#
//# 30.03.2014 OG
//# - chg: Sprache Hollaendisch vollstaendig entfernt
//# - chg: MenuCtrl_PushML_P() umgestellt auf MenuCtrl_PushML2_P()
//#
//# 29.03.2014 OG
//# - chg: Menu_OSDData() del: MenuCtrl_SetShowBatt() wegen Aenderung Default auf true
//# - chg: ask_delete_data() - Layout und Optimierung
//# - add: OSDDATA_ShowLastGPSPosition()
//# - del: OSDDATA_ShowLastGPS()
//#
//# 12.02.2014 OG
//# - del: die includes zu den alten parameter... entfernt
//#
//# 24.01.2014 OG
//# - fix: OSDDATA_ClearAllData(): Beep wenn Daten geloescht werden
//#        (wie bei den anderen Löschungen)
//#
//# 30.06.2013 OG
//# - add: Gesamtstrom-Mittelwert in OSDDATA_Statistics() und OSDDATA_BLStatistics()
//# - chg: Benamung Statistik-Var's von mid_* auf avg_* geaendert
//# - chg: Funktionsnamen geaendert
//#
//# 19.06.2013 OG
//# - add: last Flytime in OSDDATA_Statistics()
//# - chg: Reihenfolge in OSDDATA_Statistics() und zusaetzliche Trennline
//# - add: max Current (Gesamt) in OSDDATA_Statistics()
//#
//# 16.06.2013 OG
//# - fix: "min. Voltage" Anzeige in OSDDATA_Statistics() (war veschoben)
//#
//# 13.06.2013 OG
//# - chg: Uebersetzungen von CB in ask_delete_data() aufgenommen
//# - chg: GPS-Menuetitel geaendert
//# - fix: include pkt.h
//#
//# 11.06.2013 OG
//# - chg: ask_delete_data() erweitert um Info was geloescht wird & Layout (TODO: Uebersetzungen)
//# - add: Mittelwertanzeige fuer BL-Strom
//# - add: Anzahl BL-Statistik-Datenpakete (nur wenn USE_OSD_SCREEN_DEBUG)
//# - add: OSDDATA_BLStatistic() - die BL-Daten wurden in eigene Anzeigefunktion verschoben
//# - add: OSDDATA_ClearAllData() - alle Daten loeschen
//# - add: OSDDATA_ShowLastGPS() - letzte GPS-Position anzeigen
//# - add: Menu_OSDData() - ehemals in menu.c jetzt hier
//# - chg: PKT_CtrlHook() noch an einigen Stellen eingefuegt
//# - fix: ctrl_osddata() clear screen vor ScrollBox_Refresh
//#
//# 15.05.2013 OG
//# - chg: OSDDATA_Statistics() umgestellt auf calc_BLmax() (aus osd.c)
//#
//# 04.05.2013 OG
//# - chg: angepasst auf xutils.c
//# - add: weitere Anzeigen in OSD_Statistics()
//# - add: Content in OSDDATA_UserGPS() und OSDDATA_MkError()
//#
//# 28.04.2013 OG
//# - chg: auf die neuen Features von xprintf angepasst (siehe utils/xstring.c)
//#
//# 21.04.2013 Cebra
//# - chg: Texte "Datenlöschen" in messages.c aufgenommen
//#
//# 20.04.2013 OG - NEU
//############################################################################

#include "../cpu.h"
#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>

#include "../main.h"
#include "../lcd/lcd.h"
#include "../timer/timer.h"
#include "../uart/usart.h"
#include "../eeprom/eeprom.h"
#include "../messages.h"
#include "../sound/pwmsine8bit.h"
#include "../mk-data-structs.h"
#include "../osd/osd.h"
#include "../utils/scrollbox.h"
#include "../utils/xutils.h"
#include "../utils/menuctrl.h"
#include "../pkt/pkt.h"
#include "../lipo/lipo.h"
#include "../mk/mkbase.h"


#define INIT_STATISTIC   1
#define INIT_MKERROR     2
#define INIT_USERGPS     3
#define INIT_LASTPOS     4
#define INIT_ALLDATA     5


//-----------------------
// Menu_OSDDATA
//-----------------------
#define ID_SHOWSTATISTIK    1
#define ID_SHOWBLSTATISTIK  2
#define ID_SHOWMKERROR      3
#define ID_SHOWUSERGPS      4
#define ID_SHOWLASTGPS      5
#define ID_CLEARALLDATA     6


//static const char SHOWSTATISTIK_de[]        PROGMEM = "OSD Daten";
//static const char SHOWSTATISTIK_en[]        PROGMEM = "OSD data";

//static const char SHOWBLSTATISTIK_de[]      PROGMEM = "BL  Daten";
//static const char SHOWBLSTATISTIK_en[]      PROGMEM = "BL  data";

static const char SHOWSTATISTIK_de[]        PROGMEM = "OSD Statistik";
static const char SHOWSTATISTIK_en[]        PROGMEM = "OSD statistics";

static const char SHOWBLSTATISTIK_de[]      PROGMEM = "BL  Statistik";
static const char SHOWBLSTATISTIK_en[]      PROGMEM = "BL  statistics";

static const char SHOWMKERROR_de[]          PROGMEM = "MK  Fehler";
static const char SHOWMKERROR_en[]          PROGMEM = "MK  errors";

static const char SHOWUSERGPS_de[]          PROGMEM = "GPS Userdaten";
static const char SHOWUSERGPS_en[]          PROGMEM = "GPS userdata";

static const char SHOWLASTGPS_de[]          PROGMEM = "GPS letzte Pos.";
static const char SHOWLASTGPS_en[]          PROGMEM = "GPS last pos.";

static const char CLEARALLDATA_de[]         PROGMEM = "LÖSCHE alle Daten";
static const char CLEARALLDATA_en[]         PROGMEM = "DELETE all data";



//############################################################################



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t ask_delete_data( uint8_t initcode )
{
    uint8_t refresh = true;

    clear_key_all();

    set_beep ( 60, 0xffff, BeepNormal);

    while( true )
    {
        //--------------------------------------
        // screen refresh
        //--------------------------------------
        if( refresh )
        {
            lcd_cls();

            lcd_frect( 0, 0, 127, 7, 1);                                // Headline: Box fill Black
            lcdx_printp_at( 1, 0, strGet(DELETE), MINVERS,  0,0);       // Titel
            show_Lipo();                                                // LiPo anzeigen

            lcd_rect_round( 0, 20, 127, 27, 1, R2);                     // Rahmen fuer Benutzerfrage

            switch( initcode )
            {
               case INIT_STATISTIC: lcdx_printp_center( 2, strGet(STAT_OSDBL), MNORMAL,  0,9);     break;
               case INIT_MKERROR  : lcdx_printp_center( 2, strGet(STAT_ERROR), MNORMAL,  0,9);     break;
               case INIT_USERGPS  : lcdx_printp_center( 2, strGet(STAT_GPS)  , MNORMAL,  0,9);     break;
               case INIT_LASTPOS  : lcdx_printp_center( 2, strGet(STAT_POS)  , MNORMAL,  0,9);     break;
               case INIT_ALLDATA  : lcdx_printp_center( 2, strGet(STAT_ALL)  , MNORMAL,  0,9);     break;
            }

            lcdx_printp_center( 4, strGet(DELETE), MNORMAL,  0,4);      // "löschen?"

            lcd_printp_at( 0, 7, strGet(START_LASTPOS2) , MNORMAL);     // Keyline: "löschen          Nein"

            refresh = false;
        }

        //--------------------------------------
        // Update vom Updatetool angefordert?
        //--------------------------------------
        if( PKT_CtrlHook() )
        {
            refresh = true;
        }

        //--------------------------------------
        // loeschen
        //--------------------------------------
        if( get_key_press(1 << KEY_MINUS) )
        {
            clear_key_all();
            return true;                                                // Rueckgabe: true = loeschen
        }


        //--------------------------------------
        // Ende
        //--------------------------------------
        if( get_key_press(1 << KEY_ENTER) )
        {
            clear_key_all();
            return false;                                               // Rueckgabe: false = nicht loeschen
        }
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void ctrl_osddata( uint8_t isdata, uint8_t initcode )
{
    uint8_t key = 0;

    if( !isdata ) ScrollBox_Push_P( MNORMAL, PSTR("     no data...") );

    ScrollBox_SetKey( KEY_ENTER, " Del" );

    do
    {
        key = ScrollBox_Show();

        if( key == KEY_ENTER )
        {
            if( ask_delete_data(initcode) )
            {
                switch( initcode )
                {
                    case INIT_STATISTIC: STAT_Init();      break;
                    case INIT_MKERROR:   MKErr_Log_Init(); break;
                    case INIT_USERGPS:   GPS_User_Init();  break;
                }

                set_beep ( 400, 0xffff, BeepNormal);
                key = 0;    // exit
            }
            else
            {
                lcd_cls();
                ScrollBox_Refresh();
            }
        }

    } while( key );
}



//--------------------------------------------------------------
// add_statistic_head()
//
// fuegt Basisdaten den OSD- und BL-Listen hinzu
//--------------------------------------------------------------
void add_statistic_head( void )
{
    PKTdatetime_t   dtlocal;

    //---------------------------
    // begin: date/time
    //---------------------------
    UTCdatetime2local( &dtlocal, &Config.OSD_Statistic.begin_StatTime );
    ScrollBox_Push_P( MNORMAL, PSTR("Begin: %02u:%02u  %02u.%02u."),
                        (uint16_t)(dtlocal.seconds/3600), (uint16_t)(dtlocal.seconds/60%60),
    //lcd_printp_at( 3, 3, strGet(DELETEDATA), MNORMAL);        // "Daten loeschen?"
                        dtlocal.day, dtlocal.month
                    );

    //---------------------------
    // end: date/time
    //---------------------------
    UTCdatetime2local( &dtlocal, &Config.OSD_Statistic.end_StatTime );
    ScrollBox_Push_P( MNORMAL, PSTR("End:   %02u:%02u  %02u.%02u."),
                        (uint16_t)(dtlocal.seconds/3600), (uint16_t)(dtlocal.seconds/60%60),
                        dtlocal.day, dtlocal.month
                    );

    //+++++++++++++++++++++++++++
    // TRENNER
    //+++++++++++++++++++++++++++
    ScrollBox_PushLine();
}



//--------------------------------------------------------------
// zeigt aufgezeichnete OSD-Daten an
//--------------------------------------------------------------
void OSDDATA_Statistics( void )
{
    int16_t v;
    osd_BLmax_t blmax;

    lcd_cls();

    if( !ScrollBox_Create(25) )
        return;                                                 // kein RAM mehr

    //+++++++++++++++++++++++++++
    // max. der BL's ermitteln
    //+++++++++++++++++++++++++++
    calc_BLmax( &blmax );

    ScrollBox_Push_P( MINVERS, PSTR(" OSD Data") );

    //---------------------------
    // Basisanzeige der Stat-Liste
    //---------------------------
    add_statistic_head();

    //---------------------------
    // last Flytime
    //---------------------------
    ScrollBox_Push_P( MNORMAL, PSTR(" Last Fly  %02u:%02u min"), (Config.OSD_Statistic.last_FlyTime/60), (Config.OSD_Statistic.last_FlyTime%60) );

    //---------------------------
    // max. Altitude
    //---------------------------
    v = Config.OSD_Statistic.max_Altimeter / (30 / AltimeterAdjust);
    ScrollBox_Push_P( MNORMAL, PSTR("%cAltitude%7d m"), SYMBOL_MAX, v );

    //---------------------------
    // max. Distance
    //---------------------------
    ScrollBox_Push_P( MNORMAL, PSTR("%cDistance%7u m"), SYMBOL_MAX, Config.OSD_Statistic.max_Distance/10 );

    //---------------------------
    // max. Ground-Speed
    //---------------------------
    v = (uint16_t) (((uint32_t) Config.OSD_Statistic.max_GroundSpeed * (uint32_t) 90) / (uint32_t) 250);
    ScrollBox_Push_P( MNORMAL, PSTR("%cGrnd.Speed%3.1d kmh"), SYMBOL_MAX, v );

    //---------------------------
    // max. Vert.-Speed
    //---------------------------
    v = Config.OSD_Statistic.max_TopSpeed;
    v = (v*36/100);         // cm/s -> km/h*10
    ScrollBox_Push_P( MNORMAL, PSTR("%cVert.Speed%3.1d kmh"), SYMBOL_MAX, v );


    //+++++++++++++++++++++++++++
    // TRENNER
    //+++++++++++++++++++++++++++
    ScrollBox_PushLine();


    //---------------------------
    // Used Capacity
    //---------------------------
    ScrollBox_Push_P( MNORMAL, PSTR(" Capacity%7d mAh"), Config.OSD_Statistic.max_Capacity );

    //---------------------------
    // max. Current
    //---------------------------
    ScrollBox_Push_P( MNORMAL, PSTR("%cCurrent%6.1d A"), SYMBOL_MAX, Config.OSD_Statistic.max_Current );

    //---------------------------
    // avg. Current
    //---------------------------
    ScrollBox_Push_P( MNORMAL, PSTR("%cCurrent%6.1d A"), SYMBOL_AVG, (uint8_t)(Config.OSD_Statistic.avg_Current/100) );

    //---------------------------
    // BL-Detected
    //---------------------------
    //ScrollBox_Push_P( MNORMAL, PSTR(" BL Detected%4d"), Config.OSD_Statistic.BL_Count );

    //---------------------------
    // max. BL-Current
    //---------------------------
    ScrollBox_Push_P( MNORMAL, PSTR("%cBL%1d Curr.%4.1u A"), SYMBOL_MAX, blmax.max_BL_Current_Index+1, blmax.max_BL_Current );

    //---------------------------
    // max. BL-Temp
    //---------------------------
    ScrollBox_Push_P( MNORMAL, PSTR("%cBL%1d Temp. %5u %cC"), SYMBOL_MAX, blmax.max_BL_Temp_Index+1, blmax.max_BL_Temp, SYMBOL_BIGDEGREE );

    //---------------------------
    // min. Voltage
    //---------------------------
    v = (Config.OSD_Statistic.min_UBat == 255) ? 0 : Config.OSD_Statistic.min_UBat;
    ScrollBox_Push_P( MNORMAL, PSTR("%cVoltage%6.1d V"), SYMBOL_MIN, v );


    //+++++++++++++++++++++++++++
    // TRENNER
    //+++++++++++++++++++++++++++
    ScrollBox_PushLine();


    //---------------------------
    // max. RC-Quality
    //---------------------------
    ScrollBox_Push_P( MNORMAL, PSTR("%cRC-Quality%5d"), SYMBOL_MAX, Config.OSD_Statistic.max_RCQuality );

    //---------------------------
    // min. RC-Quality
    //---------------------------
    v = (Config.OSD_Statistic.min_RCQuality==255) ? 0 : Config.OSD_Statistic.min_RCQuality;
    ScrollBox_Push_P( MNORMAL, PSTR("%cRC-Quality%5d"), SYMBOL_MIN, v );


    //---------------------------
    // max. Nick
    //---------------------------
    ScrollBox_Push_P( MNORMAL, PSTR("%cNick%11d %c"), SYMBOL_MAX, Config.OSD_Statistic.max_AngleNick, SYMBOL_SMALLDEGREE );

    //---------------------------
    // min. Nick
    //---------------------------
    v = (Config.OSD_Statistic.min_AngleNick==126) ? 0 : Config.OSD_Statistic.min_AngleNick;
    ScrollBox_Push_P( MNORMAL, PSTR("%cNick%11d %c"), SYMBOL_MIN, v, SYMBOL_SMALLDEGREE );

    //---------------------------
    // max. Roll
    //---------------------------
    ScrollBox_Push_P( MNORMAL, PSTR("%cRoll%11d %c"), SYMBOL_MAX, Config.OSD_Statistic.max_AngleRoll, SYMBOL_SMALLDEGREE );

    //---------------------------
    // min. Roll
    //---------------------------
    v = (Config.OSD_Statistic.min_AngleRoll==126) ? 0 : Config.OSD_Statistic.min_AngleRoll;
    ScrollBox_Push_P( MNORMAL, PSTR("%cRoll%11d %c"), SYMBOL_MIN, v, SYMBOL_SMALLDEGREE );


    //+++++++++++++++++++++++++++
    // TRENNER
    //+++++++++++++++++++++++++++
    ScrollBox_PushLine();

    ctrl_osddata( true, INIT_STATISTIC );

    ScrollBox_Destroy();    // free memory
}



//--------------------------------------------------------------
// zeigt aufgezeichnete BL-Daten an
//--------------------------------------------------------------
void OSDDATA_StatisticsBL( void )
{
    uint8_t i;
    osd_BLmax_t blmax;

    lcd_cls();

    #ifdef USE_OSD_SCREEN_DEBUG
    if( !ScrollBox_Create(10 + (Config.OSD_Statistic.BL_Count*6)) )
        return;                                                         // kein RAM mehr
    #else
    if( !ScrollBox_Create(10 + (Config.OSD_Statistic.BL_Count*4)) )
        return;                                                         // kein RAM mehr
    #endif

    //+++++++++++++++++++++++++++
    // max. der BL's ermitteln
    //+++++++++++++++++++++++++++
    calc_BLmax( &blmax );

    ScrollBox_Push_P( MINVERS, PSTR(" BL Data") );

    //---------------------------
    // Basisanzeige der Stat-Liste
    //---------------------------
    add_statistic_head();

    //---------------------------
    // max. Current
    //---------------------------
    ScrollBox_Push_P( MNORMAL, PSTR("%cCurrent%6.1d A"), SYMBOL_MAX, Config.OSD_Statistic.max_Current );

    //---------------------------
    // avg. Current
    //---------------------------
    ScrollBox_Push_P( MNORMAL, PSTR("%cCurrent%6.1d A"), SYMBOL_AVG, (uint8_t)(Config.OSD_Statistic.avg_Current/100) );

    //---------------------------
    // BL-Detected
    //---------------------------
    //ScrollBox_Push_P( MNORMAL, PSTR("BL Detected %4d"), Config.OSD_Statistic.BL_Count );

    //---------------------------
    // max. BL-Current
    //---------------------------
    ScrollBox_Push_P( MNORMAL, PSTR("%cBL%1d Current%2.1u A"), SYMBOL_MAX, blmax.max_BL_Current_Index+1, blmax.max_BL_Current );

    //---------------------------
    // max. BL-Temp
    //---------------------------
    ScrollBox_Push_P( MNORMAL, PSTR("%cBL%1d Temp. %5u %cC" ), SYMBOL_MAX, blmax.max_BL_Temp_Index+1, blmax.max_BL_Temp, SYMBOL_BIGDEGREE );


    //+++++++++++++++++++++++++++
    // TRENNER
    //+++++++++++++++++++++++++++
    ScrollBox_PushLine();

    //---------------------------
    // LIST: BL-Data
    //---------------------------
    if( Config.OSD_Statistic.BL_Count > 0 )
    {
        ScrollBox_Push_P( MINVERS, PSTR(" BL Data") );

        for( i=0; i < Config.OSD_Statistic.BL_Count; i++)
        {
            ScrollBox_Push_P( MNORMAL, PSTR("%1d:%cCurrent%4.1u A"), (i+1), SYMBOL_MAX, (uint8_t)(Config.OSD_Statistic.BL[i].max_Current) );
            ScrollBox_Push_P( MNORMAL, PSTR("  %cCurrent%4.1u A"  ),        SYMBOL_AVG, (uint8_t)(Config.OSD_Statistic.BL[i].avg_Current/100) );
            ScrollBox_Push_P( MNORMAL, PSTR("  %cTemp.%8u %cC"    ),        SYMBOL_MAX, (uint8_t)(Config.OSD_Statistic.BL[i].max_Temp), SYMBOL_BIGDEGREE );

            #ifdef USE_OSD_SCREEN_DEBUG
            ScrollBox_Push_P( MNORMAL, PSTR("  RX-Pkg.%7u"), Config.OSD_Statistic.BL[i].count );
            //ScrollBox_Push_P( MNORMAL, PSTR("  MC%12u A"  ), Config.OSD_Statistic.BL[i].avg_Current );
            #endif

            //+++++++++++++++++++++++++++
            // TRENNER
            //+++++++++++++++++++++++++++
            ScrollBox_PushLine();
        }
    }

    ctrl_osddata( true, INIT_STATISTIC );

    ScrollBox_Destroy();    // free memory
}



//--------------------------------------------------------------
// zeigt aufgezeichnete Benutzer-GPS-Daten an
//--------------------------------------------------------------
void OSDDATA_UserGPS( void )
{
    uint8_t         i;
    uint8_t         isdata = 0;
    PKTdatetime_t   dtlocal;

    lcd_cls();

    if( !ScrollBox_Create( (MAX_GPS_USER*4) + 5 ) )         // Speicher reservieren
        return;                                             //   kein RAM mehr

    ScrollBox_Push_P( MINVERS, PSTR(" GPS Userdata") );

    for( i=0; i<MAX_GPS_USER; i++)
    {
        if( Config.GPS_User[i].GPSData.Latitude != 0)       // nur gueltige GPS-Koordinaten anzeigen
        {
            isdata = 1;

            ScrollBox_PushLine();

            //-----------------------------------
            // Zeile 1: Nummer / Datum / Zeit
            //-----------------------------------
            UTCdatetime2local( &dtlocal, &Config.GPS_User[i].timestamp );
            ScrollBox_Push_P( MNORMAL, PSTR("%02u.%02u.%04u  %02u:%02u:%02u"),
                                dtlocal.day, dtlocal.month, dtlocal.year,
                                (uint16_t)(dtlocal.seconds/3600), (uint16_t)(dtlocal.seconds/60%60), (uint16_t)(dtlocal.seconds%60)
                            );

/*
            sec = (uint16_t)(Config.GPS_User[i].timestamp.seconds);  // Sekunden
            ScrollBox_Push_P( MNORMAL, PSTR("%02u.%02u.%04u  %02u:%02u:%02u"),
                                Config.MKErr_Log[i].set_Time.day, Config.MKErr_Log[i].set_Time.month, Config.MKErr_Log[i].set_Time.year,
                                (uint16_t)(sec/3600), (uint16_t)(sec/60%60), (uint16_t)(sec%60)
                            );

            // Alternative Anzeige von Datum/Zeit:
            min = (uint16_t)(Config.GPS_User[i].timestamp.seconds/60);  // Minuten
            ScrollBox_Push_P( MNORMAL, PSTR("%02u:  %02u:%02u  %02u.%02u.%02u"), i+1,
                                (uint16_t)(min/60), (uint16_t)(min%60),
                                Config.GPS_User[i].timestamp.day, Config.GPS_User[i].timestamp.month, Config.GPS_User[i].timestamp.year%100
                            );

            // Alternative Anzeige von Datum/Zeit:
            ScrollBox_Push_P( MNORMAL, PSTR("%02u: %02u:%02u %02u.%02u.%04u"), i+1,
                                (uint16_t)(min/60), (uint16_t)(min%60),
                                Config.GPS_User[i].timestamp.day, Config.GPS_User[i].timestamp.month, Config.GPS_User[i].timestamp.year
                            );

            // Alternative Anzeige von Datum/Zeit:
            ScrollBox_Push_P( MNORMAL, PSTR("%02u: %02u.%02u.%04u %02u:%02u"), i+1,
                                Config.GPS_User[i].timestamp.day, Config.GPS_User[i].timestamp.month, Config.GPS_User[i].timestamp.year,
                                (uint16_t)(min/60), (uint16_t)(min%60)
                            );
*/
            //-----------------------------------
            // Zeile 2: Hoehe (GPS/Barymetrisch)
            //-----------------------------------
            ScrollBox_Push_P( MNORMAL, PSTR("%7d mG%5.1d mB"),
                                (Config.GPS_User[i].GPSData.Altitude/1000),                 // GPS-Hoehe in m
                                (Config.GPS_User[i].Altimeter / (3 / AltimeterAdjust)) );   // Barymetrische-Hoehe in dm

            //-----------------------------------
            // Zeile 3: Koordinaten (Lat/Long)
            //-----------------------------------
            ScrollBox_Push_P( MNORMAL, PSTR("%3.6ld%3.6ld"),
                                Config.GPS_User[i].GPSData.Latitude/10,
                                Config.GPS_User[i].GPSData.Longitude/10 );
        }
    }
    ScrollBox_PushLine();


    ctrl_osddata( isdata, INIT_USERGPS );

    ScrollBox_Destroy();    // free memory
}



//--------------------------------------------------------------
// OSDDATA_MkError()
//
// zeigt aufgezeichnete MK-Fehler an
//--------------------------------------------------------------
void OSDDATA_MkError( void )
{
    uint8_t         i;
    uint8_t         isdata = 0;
    PKTdatetime_t   dtlocal;

    lcd_cls();

    if( !ScrollBox_Create( (MAX_MKERR_LOG*4) + 3 ) )        // Speicher reservieren
        return;                                             //   kein RAM mehr

    ScrollBox_Push_P( MINVERS, PSTR(" MK Errors") );

    for( i=0; i<MAX_MKERR_LOG; i++)
    {
        if( Config.MKErr_Log[i].Errorcode != 0)             // nur vorhandene Errorcodes anzeigen
        {
            isdata = 1;

            ScrollBox_PushLine();

            //-----------------------------------
            // Zeile 1: Zeit / Datum
            //-----------------------------------
            UTCdatetime2local( &dtlocal, &Config.MKErr_Log[i].set_Time );
            ScrollBox_Push_P( MNORMAL, PSTR("%02u:%02u:%02u  %02u.%02u.%04u"),
                                (uint16_t)(dtlocal.seconds/3600), (uint16_t)(dtlocal.seconds/60%60), (uint16_t)(dtlocal.seconds%60),
                                dtlocal.day, dtlocal.month, dtlocal.year
                            );

            //-----------------------------------
            // Zeile 2: Error Code
            //-----------------------------------
            ScrollBox_Push_P( MNORMAL, PSTR(" Code %02u"), Config.MKErr_Log[i].Errorcode );

            //-----------------------------------
            // Zeile 3: Error Text
            //-----------------------------------
            ScrollBox_Push_P( MNORMAL, PSTR("%20S"), pgm_read_word(&mkerrortext[Config.MKErr_Log[i].Errorcode]) );
        }
    }
    ScrollBox_PushLine();

    ctrl_osddata( isdata, INIT_MKERROR );

    ScrollBox_Destroy();    // free memory
}



//--------------------------------------------------------------
// OSDDATA_ShowLastGPSPosition()
//
// zeigt die letzte aufgezeichnete GPS-Position an.
// Möglichkeit zum loeschen der GPS-Pos ist vorhanden.
//--------------------------------------------------------------
void OSDDATA_ShowLastGPSPosition( void )
{
    uint8_t redraw   = true;

    clear_key_all();

    while( true )
    {
        //------------------------
        // Screen neu zeichnen
        //------------------------
        if( redraw )
        {
            PKT_TitlePKT();                                                 // Titel mit PKT-Version anzeigen (und clearcsreen)

            lcdx_printp_at(3, 1, strGet(START_LASTPOS) , MNORMAL, 0,4);     // "Letzte Position"
            lcdx_printp_at(3, 2, strGet(START_LASTPOS3), MNORMAL, 0,4);     // "Google Eingabe"

            //----
            lcd_frect( 0, (4*7)+4, 127, 7, 1);                              // Rect: Invers
            lcdx_printp_at(1, 3, strGet(START_LASTPOS1), MINVERS, 0,8);     // "Breitengr  Längengr"

            writex_gpspos(  1, 4, Config.LastLatitude , MNORMAL,  0,10);    // Anzeige: Breitengrad
            writex_gpspos( 12, 4, Config.LastLongitude, MNORMAL, -1,10);    // Anzeige: Laengengrad

            lcd_rect( 0, (3*8)+7, 127, (2*8)+4, 1);                         // Rahmen

            //lcd_printp_at(0, 7, strGet(START_LASTPOS2), MNORMAL);           // Keyline
            lcd_printp_at(12, 7, strGet(KEYLINE4)      , MNORMAL);          // Keyline
            lcd_printp_at(18, 7, PSTR("Del")           , MNORMAL);          // Keyline

            redraw = false;
        }


        //------------------------
        // LiPo Spannung zeigen
        //------------------------
        show_Lipo();


        //------------------------
        // Tasten abfragen
        //------------------------
        if( get_key_press (1 << KEY_ESC) )                                          // Ende
        {
            break;
        }

        if( (get_key_press (1 << KEY_ENTER)) )                                      // Del
        {
            if( ask_delete_data(INIT_LASTPOS) )
            {
                set_beep ( 400, 0xffff, BeepNormal);
                WriteLastPosition( 0x00000000, 0x00000000 );                        // letzte GPS Position loeschen
                break;                                                              //  und beenden
            }
            redraw = true;
        }


        //------------------------
        // ggf. auf PKT-Update reagieren
        //------------------------
        if( PKT_CtrlHook() )
        {
            redraw = true;
        }
    }

    clear_key_all();
}




//--------------------------------------------------------------
// OSDDATA_ClearAllData()
//
// löscht alle erhobenen Daten
//--------------------------------------------------------------
void OSDDATA_ClearAllData( void )
{
    if( ask_delete_data( INIT_ALLDATA ) )
    {
        STAT_Init();
        MKErr_Log_Init();
        GPS_User_Init();                                // loeschen: User GPS
        WriteLastPosition( 0x00000000, 0x00000000 );    // loeschen: letzte GPS Position

        set_beep ( 400, 0xffff, BeepNormal);
    }
}



//--------------------------------------------------------------
// Menue fuer osddata
//--------------------------------------------------------------
#ifdef USE_OSDDATA
void Menu_OSDData( void )
{
    //---------------
    // Create
    //---------------
    MenuCtrl_Create();
    MenuCtrl_SetTitleFromParentItem();      // "OSD Daten"
  //MenuCtrl_SetTitle_P( PSTR("OSD Daten") );
  //MenuCtrl_SetShowBatt( true );


    //---------------
    // Menuitems
    //---------------
    MenuCtrl_PushML2_P( ID_SHOWSTATISTIK  , MENU_ITEM, &OSDDATA_Statistics,          SHOWSTATISTIK_de  , SHOWSTATISTIK_en   );
    MenuCtrl_PushML2_P( ID_SHOWBLSTATISTIK, MENU_ITEM, &OSDDATA_StatisticsBL,        SHOWBLSTATISTIK_de, SHOWBLSTATISTIK_en );
    MenuCtrl_PushML2_P( ID_SHOWMKERROR    , MENU_ITEM, &OSDDATA_MkError,             SHOWMKERROR_de    , SHOWMKERROR_en     );
    MenuCtrl_PushML2_P( ID_SHOWUSERGPS    , MENU_ITEM, &OSDDATA_UserGPS,             SHOWUSERGPS_de    , SHOWUSERGPS_en     );
    MenuCtrl_PushML2_P( ID_SHOWLASTGPS    , MENU_ITEM, &OSDDATA_ShowLastGPSPosition, SHOWLASTGPS_de    , SHOWLASTGPS_en     );
    MenuCtrl_PushML2_P( ID_CLEARALLDATA   , MENU_ITEM, &OSDDATA_ClearAllData,        CLEARALLDATA_de   , CLEARALLDATA_en    );

    //---------------
    // Control
    //---------------
    MenuCtrl_Control( MENUCTRL_EVENT );

    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();
}
#endif


