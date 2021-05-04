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
//# HISTORY  mkbase.c
//#
//# 21.06.2014 OG
//# - chg: searchMK() schaltet am Ende wieder auf die NC um sofern diese
//#        vorhanden ist
//#
//# 19.05.2014 OG
//# - chg: MKVersion_Setting_print() umgestellt auf strGet(STR_ERROR) fuer
//#        Multi-Sprachenunterstuetzung
//#
//# 14.05.2014 OG
//# - chg: include "paramset.h" geaendert auf "../mksettings/paramset.h"
//#
//# 13.05.2014 OG
//# - chg: MK_Info() - del: MKINFO_AUTO_REFRESH (nicht mehr unterstuetzt)
//# - chg: MK_Setting_write() - del: unused variable 'lsetting'
//#
//# 17.04.2014 OG
//# - chg: MK_Setting_load - Ansprechverhalten verbessert wenn Windows
//#        Mikrokoptertool aktiv ist
//#
//# 29.03.2014 OG
//# - del: MK_Show_LastGPS_Position() -> jetzt: OSDDATA_ShowLastGPSPosition()/osddata.c
//#
//# 28.03.2014 OG
//# - add: MK_Show_LastGPS_Position() - ehemals in main.c
//#
//# 24.03.2014 OG
//# - chg: searchMK(): doppeltes MK_SwitchToNC() um evtl. vorhandene NC
//#        zuverlaessiger zu erkennen
//# - fix: searchMK(): RetVal initialisiert
//# - chg: Default-Anzeigezeit von MK_Info() in searchMK() auf 5 Sek. reduziert
//#
//# 21.02.2014 OG
//# - fix: MKVersion_Setting_print() Anzeige der Settings-Nummer gekuerzt
//#
//# 20.02.2014 OG
//#- chg: searchMK() auf aktuelle Version von PKT_TitlePKTVersion() angepasst
//#
//# 20.02.2014 OG
//# - chg: MKVersion_Setting_print() um ein paar Bytes zu sparen
//# - chg: MK_Info() um ein paar Bytes zu sparen
//#
//# 16.02.2014 OG
//# - add: MK_SwitchToNC(), MK_SwitchToFC(), MK_SwitchToMAG(), MK_SwitchToGPS()
//# - add: MK_Setting_write(), MK_Setting_change()
//# - chg: umbenannt: MK_load_setting() zu MK_Setting_load()
//#
//# 13.02.2014 OG
//# - add: MKVersion_Cmp()  (Anleitung bei der Funktion beachten!)
//# - del: WrongFCVersion
//#
//# 12.02.2014 OG
//# - chg: verschiedene Verbesserungen an MK_Info(), MK_load_settings(), searckMK()
//#
//# 10.02.2014 OG
//# - add: MKVersion_Setting_print()
//#
//# 09.02.2014 OG
//# - add: MK_Info()
//#
//# 08.02.2014 OG
//# - chg: searckMK() Anpassung an MKVersion
//# - chg: MK_load_setting() Anpassung an MKVersion und Parameter geaendert
//# - add: MKVersion_Init(), MKVersion_print_at()
//#
//# 03.02.2014 OG
//# - chg: Titelanzeige in searchMK() umgestellt auf PKT_ShowTitlePKTVersion()
//# - chg: kleine Aenderung in der Anzeige "für FC..." in searchMK()
//#
//# 29.01.2014 OG
//# - Ausgliederungen aus main.c: MK_load_setting(), searchMK()
//############################################################################


#include "../cpu.h"
#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include <util/atomic.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>


//#include "../lipo/lipo.h"
#include "../main.h"
#include "../lipo/lipo.h"
#include "../lcd/lcd.h"
#include "../uart/usart.h"

#include "../uart/uart1.h"
#include "../mk-data-structs.h"
//#include "../menu.h"
#include "../timer/timer.h"
#include "../eeprom/eeprom.h"
#include "../messages.h"
#include "../utils/scrollbox.h"
#include "../utils/xutils.h"
#include "../pkt/pkt.h"
#include "../mksettings/paramset.h"
#include "mkbase.h"



//----------------------------
// MK-Versionsinformationen
// global verfuegbar
//----------------------------
MKVersion_t MKVersion;

uint8_t cmd;


//#############################################################################################
//# 1. MKVersion
//#############################################################################################


//--------------------------------------------------------------
// MKVersion_Init()
//
// initialisiert MKVersion auf 0
//--------------------------------------------------------------
void MKVersion_Init( void )
{
    memset( &MKVersion, 0, sizeof(MKVersion_t) );
}



//--------------------------------------------------------------
// value = MKVersion_Cmp( FC/NCver,   SWMajor,SWMinor,SWPatch )
//
// Vergleicht eine uebergebene FC/NC-Version mit der vom MK
// ermittelten Version
//
// Parameter:
//
//  FC/NCver: MKVersion.FCVer oder MKVersion.NCVer
//
//  SWMajor,SWMinor,SWPatch: siehe nachfolgendes...
//
//---
// Beispiel aus osd.c/OSD_MK_GetData():
//
//      v = MKVersion_Cmp( MKVersion.NCVer,  0,30,'g' );    // pruefe auf NC-Version "0.30g"
//
//      if( v && (v >= GIVEN_VERSION) )                     // wenn aktuelle NC-Version >= "0.30g"...
//      { ...
//
// Sowas geht auch:
//
//      if( v && (v == GIVEN_VERSION) )                     // nur wenn aktuelle NC-Version == "0.30g"...
//
// oder:
//
//      if( v && (v < GIVEN_VERSION) )                      // nur wenn aktuelle NC-Version kleiner als "0.30g"...
//
//---
// Warum "if( v && ..." ?
//
//  Wenn das PKT keine FC/NC-Version ermitteln konnte, und somit kein
//  Vergleich moeglich ist, gibt MKVersion_Cmp() false (=0) zurueck!
//--------------------------------------------------------------
uint8_t MKVersion_Cmp( Version_t ver, unsigned char SWMajor, unsigned char SWMinor, unsigned char SWPatch )
{
    SWPatch = SWPatch -'a';

    if( ver.SWMajor > 0 || ver.SWMinor > 0 )    // NC/FC Version erkannt?
    {
        if( (ver.SWMajor == SWMajor) && (ver.SWMinor == SWMinor) && (ver.SWPatch == SWPatch) )
            return VERSION_EQUAL;           // ==2

        if( ver.SWMajor != SWMajor )
        {
            if( ver.SWMajor < SWMajor )
                return VERSION_LOWER;       // ==1
            else
                return VERSION_GREATER;     // ==3
        }

        if( ver.SWMinor != SWMinor )
        {
            if( ver.SWMinor < SWMinor )
                return VERSION_LOWER;       // ==1
            else
                return VERSION_GREATER;     // ==3
        }

        if( ver.SWPatch < SWPatch )
            return VERSION_LOWER;       // ==1
        else
            return VERSION_GREATER;     // ==3
    }
    return VERSION_NO;                  // ==0
}



//--------------------------------------------------------------
// MKVersion_print_at()
//
// zeigt die aktuelle FC, NC Version auf dem LCD an
//
// Parameter:
//   what: NC, FC,
//--------------------------------------------------------------
void MKVersion_print_at( uint8_t x, uint8_t y, uint8_t what, uint8_t drawmode, int8_t xoffs, int8_t yoffs )
{
    const char *mask  = PSTR("%1u.%02u%c");
    const char *nostr = PSTR("-----");

    switch(what)
    {
        case FC:    if( MKVersion.FCVer.SWMajor > 0 || MKVersion.FCVer.SWMinor > 0 )
                        lcdx_printf_at_P( x, y, drawmode, xoffs, yoffs, mask, MKVersion.FCVer.SWMajor, MKVersion.FCVer.SWMinor, MKVersion.FCVer.SWPatch+'a' );
                    else
                        lcdx_printp_at( x,y, nostr, drawmode, xoffs, yoffs);
                    break;

        case NC:    if( (MKVersion.NCVer.SWMajor != 0) || (MKVersion.NCVer.SWMinor != 0) )
                        lcdx_printf_at_P( x, y, drawmode, xoffs, yoffs, PSTR("%1u.%02u%c"), MKVersion.NCVer.SWMajor, MKVersion.NCVer.SWMinor, MKVersion.NCVer.SWPatch+'a' );
                    else
                        lcdx_printp_at( x,y, nostr, drawmode, xoffs, yoffs);
                    break;
    }
}



//--------------------------------------------------------------
// MKVersion_Setting_print( y, drawmode, xoffs,yoffs)
//
// Zeigt das aktuelle MK-Setting aus MKVersion zentriert in der
// Zeile y auf dem PKT-Screen an
//--------------------------------------------------------------
void MKVersion_Setting_print( uint8_t y, uint8_t drawmode, uint8_t xoffs, uint8_t yoffs )
{
    char buffer[18];                                                                                // String Buffer fuer eine Zeile

    if( MKVersion.mksetting == 0 )
        xsnprintf_P( buffer, 18, PSTR("* %S *"), strGet(STR_ERROR) );                               // FC Setting konnte NICHT gelesen werden

    else if( MKVersion.mksetting != 0 && MKVersion.paramsetOK )                                     // FC-Setting wurde gelesen...
        xsnprintf_P( buffer, 18, PSTR("%u %s"), MKVersion.mksetting, MKVersion.mksettingName );     // ... passende FC-Reversion -> Nummer und Name anzeigen

    else
        xsnprintf_P( buffer, 18, PSTR("%u"), MKVersion.mksetting );                                 // ... aber FALSCHE FC-Version -> nicht den Namen anzeigen (nur die Nummer)

    lcdx_print_center( y, (unsigned char *)buffer, drawmode,  xoffs,yoffs);                         // Ausgabe Setting-Name (zentriert)
}




//#############################################################################################
//# 2. Sonstiges - MK-Suche, Settings laden, MK-Info
//#############################################################################################

#define SWITCH_DELAY  25  // alt 50

//--------------------------------------------------------------
//--------------------------------------------------------------
void MK_SwitchToNC( void )
{
    // switch to NC
    USART_putc (0x1b);
    USART_putc (0x1b);
    USART_putc (0x55);
    USART_putc (0xaa);
    USART_putc (0x00);
    current_hardware = NC;
    _delay_ms( SWITCH_DELAY );
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void MK_SwitchToFC( void )
{
    // switch to FC
    cmd = 0x00;                 // 0 = FC, 1 = MK3MAG, 2 = MKGPS
    SendOutData('u', ADDRESS_NC, 1, &cmd, 1);
    current_hardware = FC;
    _delay_ms( SWITCH_DELAY );
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void MK_SwitchToMAG( void )
{
    // switch to MK3MAG
    cmd = 0x01;                 // 0 = FC, 1 = MK3MAG, 2 = MKGPS
    SendOutData('u', ADDRESS_NC, 1, &cmd, 1);
    current_hardware = MK3MAG;
    _delay_ms( SWITCH_DELAY );
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void MK_SwitchToGPS( void )
{
    // switch to MKGPS
    cmd = 0x02;                 // 0 = FC, 1 = MK3MAG, 2 = MKGPS
    SendOutData('u', ADDRESS_NC, 1, &cmd, 1);
    current_hardware = MKGPS;
    _delay_ms( SWITCH_DELAY );
}



//--------------------------------------------------------------
// ret = MK_Setting_load( lsetting, &mk_param_struct, showerror)
//
// holt MK-Settings - es wird auf die FC umgeschaltet!
//--------------------------------------------------------------
uint8_t MK_Setting_load( uint8_t lsetting, uint8_t timeout )
{
    // timeout = 15 bis 20 ist meist ein guter Wert

    //MK_SwitchToFC();

    mode = 'Q';                                             // Settings Request
    rxd_buffer_locked = FALSE;

    while( !rxd_buffer_locked && timeout )
    {
        MK_SwitchToFC();
        SendOutData( 'q', ADDRESS_FC, 1, &lsetting, 1);     // lsetting == 0xff -> aktuelles MK-Setting laden

        timer2 = 25;                                        // timer ist ggf. belegt von MK_Info() -> versuchen wir es mit timer2....
        while( timer2 > 0 && !rxd_buffer_locked );

        timeout--;
    }

    mode = 0;

    // Initialisierungen
    paramsetInit(0);

    if( rxd_buffer_locked )
    {
        Decode64();
        paramsetInit( (unsigned char *) pRxData );          // fuellt u.a. auch MKVersion.paramsetRevision
    }

    return MKVersion.mksetting;                             // wird von paramsetInit() gesetzt/initialisert
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t MK_Setting_write( uint8_t wsetting, uint8_t timeout)
{
    //uint8_t timeout = 50;
    uint8_t setting;

    if( !MKVersion.paramsetOK )
    {
        return 0;   // Error
    }

    //MK_SwitchToFC();      // ?

    setting = 0;
    mode    = 'S';     // Settings

    rxd_buffer_locked = FALSE;

    while( !rxd_buffer_locked && timeout && !setting)
    {
        SendOutData( 's', ADDRESS_FC, 2, &wsetting, 1, (pRxData+1), paramsetSize() );       // pRxData+1 = die mk_datastruct beginnt bei +1; bei 0 ist die Settingsnummer

        timer2 = 70;
        while( timer2 > 0 && !rxd_buffer_locked );
        timeout--;

        if( rxd_buffer_locked )
        {
            Decode64();
            setting = *pRxData;

            if( !setting )
                rxd_buffer_locked = FALSE;
        }
    }

    setting = MK_Setting_load( 0xff, 15);      // damit pRxData & MKVersion initialisiert sind

    return setting;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t MK_Setting_change( uint8_t setting )
{
    uint8_t lsetting = 0;
    uint8_t wsetting = 0;

    lsetting = MK_Setting_load( setting, 20);

    if( lsetting == setting )
    {
        wsetting = MK_Setting_write( setting, 50);
    }

    /*
    //--------------------
    // DEBUG...
    //--------------------
    lcd_printf_at_P( 0, 1, MNORMAL, PSTR("load : %1u  (req:%1u)"), lsetting, setting );
    lcd_printf_at_P( 0, 2, MNORMAL, PSTR("write: %1u  "), wsetting );
    if( (setting != lsetting) || (setting != wsetting) )
        set_beep( 1000, 0x0f0f, BeepNormal );   // Error
    timer2 = 500;  while( timer2 > 0 );
    */

    return wsetting;                                        // 0 = Fehler; oder 1..5 = geschriebenes Setting
}



//--------------------------------------------------------------
// ok = searchMK( showMKInfo )
//
// Parameter:
//  showMKInfo: true/false
//--------------------------------------------------------------
uint8_t searchMK( uint8_t showMKInfo )
{
    uint8_t timeout;
    //uint8_t setting;         // aktuelles FC-Setting (1..5)
    uint8_t RetVal      = false;
    uint8_t redraw      = true;
    uint8_t searchbar_y;
    uint8_t searchbar_w = 0;


    MKVersion_Init();

    //----------------
    // Switch to NC
    //----------------
    MK_SwitchToNC();
    _delay_ms(50);          // 24.03.2014 OG: manchmal wurde nicht auf eine vorhandene NC umgeschaltet
    MK_SwitchToNC();        //                evtl. wird das durch doppeltes Senden verbessert

    //---------------------------------------
    // MK-Suchscreen - Versionsabfrage NC
    //---------------------------------------
    mode = 'V';

    rxd_buffer_locked = FALSE;
    timeout = 50;

    //searchbar_y = 5*8-1;
    searchbar_y = 5*8+2;

    while( !rxd_buffer_locked )
    {
        //------------------------
        // MK Suchscreen anzeigen
        //------------------------
        if( redraw )
        {
            PKT_TitlePKT();                                                 // Titel mit PKT-Version anzeigen  (mit Lipo-Anzeige)
            lcdx_printp_center( 2, strGet(STR_PKT), MNORMAL,  0,2);         // "PKT"
            lcdx_printp_at( 0, 3, strGet(START_SEARCHFC), MNORMAL,  0,8);   // "suche Mikrokopter..."

            lcd_printp_at(12, 7, strGet(ENDE), MNORMAL);                    // Keyline

            lcd_rect ( 0, searchbar_y  , 126, 8, 1);                        // Rahmen fuer Bar-Anzeige

            searchbar_w = 2;

            redraw = false;
        }

        //------------------------
        // PKT-LiPo Anzeige
        //------------------------
        show_Lipo();

        //------------------------
        // MK Datenanfrage
        //------------------------
        SendOutData( 'v', ADDRESS_ANY, 0);                                  // Versions-Anfrage an MK senden

        timer = 16;                                                         // kurze Verögerung fuer Datenanfrage und Fortschrittsbalken
        while( timer > 0 );

        //------------------------
        // Fortschrittsbalken
        //------------------------
        lcd_frect( 2, searchbar_y+2,  searchbar_w, 4, 1);                   // Fortschrittsbalken zeichnen...
        searchbar_w += 2;
        if( searchbar_w >= 125 )
        {
            searchbar_w = 0;
            lcd_frect( 2, searchbar_y+2,  123, 4, 0);
        }

        //------------------------
        // Tasten abfragen
        //------------------------
        if( get_key_press(1 << KEY_ESC) )
        {
            hardware = NO;
            return false;
        }

        //------------------------------------------
        // Pruefe PKT-Update oder andere PKT-Aktion
        //------------------------------------------
        if( PKT_CtrlHook() )                                            // Update vom Updatetool angefordert?
        {
            redraw = true;
        }
    }



    //--------------------------
    // MK gefunden?
    //--------------------------
    if( rxd_buffer_locked )
    {
        Decode64();

        //---
        // TODO OG: testen mit entsprechendem Hardware Setup des MK's!
        //---
        if( (rxd_buffer[1] - 'a') == ADDRESS_FC )
        {
            hardware         = FC;
            current_hardware = hardware;
            MKVersion.isFC   = true;
            RetVal           = true;
            memcpy( &MKVersion.FCVer, (Version_t *) (pRxData), sizeof(Version_t) );
        }

        if( (rxd_buffer[1] - 'a') == ADDRESS_NC )
        {
            hardware         = NC;
            current_hardware = hardware;
            MKVersion.isNC   = true;
            RetVal           = true;
            memcpy( &MKVersion.NCVer, (Version_t *) (pRxData), sizeof(Version_t) );
        }


        //--------------------------
        // jetzt: Version der FC abfragen
        //--------------------------
        if( hardware == NC )
        {
            MK_SwitchToFC();

            mode = 'V';
            rxd_buffer_locked = FALSE;
            timeout = 40;

            while( !rxd_buffer_locked && timeout)
            {
                SendOutData( 'v', ADDRESS_FC, 0);
                timer = 20;
                while( timer > 0 );
                timeout--;
            }

            //--------------------------
            // FC gefunden!
            // -> Version kopieren
            //--------------------------
            if( rxd_buffer_locked )
            {
                Decode64();
                MKVersion.isFC = true;
                memcpy( &MKVersion.FCVer, (Version_t *) (pRxData), sizeof(Version_t) );
            }
        }

    }   // end: if( rxd_buffer_locked )


    //---------------------------------------
    // FC EEprom Version / Struktur pruefen
    //---------------------------------------
    MK_Setting_load( 0xff, 15 );                        // 0xff == aktuelles Parameterset holen; 15 == timeout; Ergebnisse in MKVersion

    mode = 0;
    rxd_buffer_locked = FALSE;

    if( MKVersion.isNC )
    {
        MK_SwitchToNC();                                // wieder auf NC umschalten als default
    }


    if( MKVersion.paramsetOK )                      // FC Revision OK?
    {
        set_beep( 25, 0xffff, BeepNormal );             // kurzer Beep (FC-Revision ok)
        if( showMKInfo )    MK_Info( 500, true );       // Anzeige von MK_Info() max. 5 Sekunden (mit Settings-Refresh)
    }
    else                                            // FC Revision nicht unterstuetzt...
    {
        set_beep( 1000, 0xffff, BeepNormal );           // langer Error-Beep (FC-Revision nicht unterstuetzt)
        if( showMKInfo )    MK_Info( 1600, true );      // Anzeige von MK_Info() max. 16 Sekunden (mit Settings-Refresh)
    }


    //timer = 50;
    //while (timer > 0);

    get_key_press(KEY_ALL);

    return RetVal;

} // searchMK()




//--------------------------------------------------------------
// chg = MK_Info( displaytimeout, refreshSettings )
//
// Parameter:
//  displaytimeout : 0 = kein autom. Timeout
//                   n = Anzeige verschwindet automatisch nach n Zeit
//                       (bei z.B. n=600 nach 6 Sekunden)
//
//  refreshSettings: true/false
//                   true = in bestimmten Zeitintervallen (RELOAD_SETTING_TIME)
//                     wird MK_Setting_load() durchgeführt um ggf. einen Kopterwechsel
//                     automatisch zu erkennen.
//                     Dazu muss MKINFO_AUTO_REFRESH in main.h eingeschaltet sein!
//
// Rueckgabe:
//  hardwarechange: true/false
//--------------------------------------------------------------
uint8_t MK_Info( uint16_t displaytimeout, uint8_t refreshSettings )
{
    uint8_t redraw;
    uint8_t old_isFC;
    uint8_t old_isNC;

    old_isFC        = MKVersion.isFC;
    old_isNC        = MKVersion.isNC;


//displaytimeout = 0;

    if( displaytimeout )
        timer = displaytimeout;

    #define RELOAD_SETTING_TIME  350                // alle 0.35 sec neue Settings vom MK laden
    timer_get_tidata = RELOAD_SETTING_TIME;         // ich brauche weitere timer... und bediene mich jetzt mal bei einem osd-timer
                                                    // (timer2 ist belegt von MK_Setting_load)

    //get_key_press(KEY_ALL);


    redraw = true;

    while( true )
    {
        //------------------------------------------
        // Screen neu zeichnen
        //------------------------------------------
        if( redraw )
        {
            if( redraw != 2 )
                lcd_cls();

            show_Lipo();                                                // LiPo direkt anzeigen damit es nicht flackert

            lcd_frect( 0, 0, 101, 8, 1);                                // Headline: Box fill Black mit Platz fuer PKT-LiPo rechts
            lcdx_printp_at( 1, 0, PSTR("MK Info"), MINVERS,  0,0);      // Titel

            if( MKVersion.isFC || MKVersion.isNC )                      // MK gefunden...
            {
                lcd_frect_round(   0, 30, 127, 22, 1, R2);              // Fill: Anzeigebereich der Settings in Schwarz

                //--------------------
                // FC-Version
                //--------------------
                lcdx_printp_at(      0, 2, PSTR("FC:") , MNORMAL, 4,-5);
                MKVersion_print_at(  4, 2, FC          , MNORMAL, 2,-5);

                //--------------------
                // FC-Revision
                //--------------------
                lcdx_printp_at(   12, 2, PSTR("Rev:"), MNORMAL, 5,-5);
                lcdx_printf_at_P( 17, 2, MNORMAL, 2,-5, PSTR("%03u"), MKVersion.paramsetRevision );

                if( !MKVersion.paramsetOK )
                    lcdx_printp_at( 11, 3, PSTR(" Rev-ERR!"), MINVERS, 5,-4);   // Fehler FC-Revision (nicht vom PKT unterstuetzt)
                else
                    lcd_frect( 11*6+5, 3*8-4, 9*6, 8, 0);                       // ggf. Fehleranzeige loeschen (nach refreshSettings)


                //--------------------
                // NC-Version
                //--------------------
                lcdx_printp_at(      0, 3, PSTR("NC:") , MNORMAL, 4,-3);
                MKVersion_print_at(  4, 3, NC          , MNORMAL, 2,-3);


                //--------------------
                // aktuelles Setting
                //--------------------
                lcdx_printp_center( 4, PSTR("MK Setting"), MINVERS,  0,1);      // "MK Setting" (zentriert)

                //lcdx_printp_at( 5, 4, PSTR("MK Setting"), MINVERS, 4,1);
                MKVersion_Setting_print( 5, MINVERS,  0,3);
            }
            else  // else: if( MKVersion.isFC || MKVersion.isNC )       // KEINEN MK gefunden...
            {
                //PKT_Message_P( *text, error, timeout, beep, clearscreen )
                PKT_Message_P( strGet(NOMKFOUND), false, 0, true, false );          // FEHLER! nodata (max. 8 Sekunden anzeigen)

            }   // end: if( MKVersion.isFC || MKVersion.isNC )


            //lcd_printp_at( 19, 7, strGet(OK), MNORMAL);               // Keyline
            lcd_printp_at(12, 7, strGet(ENDE), MNORMAL);                // Keyline

            if( displaytimeout == 0 )
                lcd_printp_at( 0, 7, PSTR("Refresh"), MNORMAL);         // Keyline: Refresh

            redraw = false;

        }  // end: if( redraw )


        //------------------------------------------
        // PKT-LiPo Anzeige
        //------------------------------------------
        show_Lipo();


        //------------------------
        // Tasten abfragen
        //------------------------
        if( get_key_press(1 << KEY_MINUS) && (displaytimeout == 0) )
        {
            searchMK( false );                                          // false = zeige nicht MK_Info() an
            redraw = true;
        }

        if( get_key_short(1 << KEY_ESC) )
        {
            break;
        }


        //------------------------------------------
        // Pruefe PKT-Update oder andere PKT-Aktion
        //------------------------------------------
        if( PKT_CtrlHook() )                                            // Update vom Updatetool angefordert?
        {
            redraw = true;
        }

        //------------------------------------------
        // displaytimeout?
        //------------------------------------------
        if( displaytimeout && !timer )
        {
            break;
        }

    } // end: while( true )

    get_key_press(KEY_ALL);

    return( (old_isFC != MKVersion.isFC) || (old_isNC != MKVersion.isNC) );

} // MK_Info()



