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
//# HISTORY  mksettings.c
//#
//# 14.05.2014 OG
//# - chg: include "mkbase.h" geaendert auf "../mk/mkbase.h"
//#
//# 11.05.2014 OG
//# - chg: MKSettings_Menu() umgestellt auf MenuCtrl_SetTitleFromParentItem()
//#        -> die Menues 'erben' damit ihren Titel vom aufrufenden Menuepunkt
//#
//# 29.03.2014 OG
//# - chg: versch. Funktionen: del: MenuCtrl_SetShowBatt() wegen Aenderung
//#        des Defaults auf true
//#
//# 26.03.2014 OG
//# - add: etliche Aenderungen in allen Bereichen fuer das erste Release
//#        der neuen MK-Settings
//#
//# 27.02.2014 OG
//# - chg: MKSettings_AskAction() Unterstuetzung von param_DUMMY
//#
//# 26.02.2014 OG
//# - chg: MKSettings_Copy() auf KEYLINE2 geaendert
//#
//# 23.02.2014 OG
//# - chg: MKSettings_Menu() Aufruf von MK_Parameters() geaendert
//#
//# 18.02.2014 OG - NEU
//############################################################################


#include "../cpu.h"
#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <string.h>
#include <util/atomic.h>

//#include "../lipo/lipo.h"
#include "../main.h"
#include "../lipo/lipo.h"
#include "../lcd/lcd.h"
#include "../uart/usart.h"
#include "../utils/menuctrl.h"
#include "../utils/xutils.h"

#include "../uart/uart1.h"
#include "../mk-data-structs.h"
//#include "../menu.h"
#include "../timer/timer.h"
#include "../eeprom/eeprom.h"
#include "../messages.h"
#include "../pkt/pkt.h"
#include "../mk/mkbase.h"
#include "paramset.h"
#include "mkparameters.h"
#include "mksettings.h"

//---------------------------------------------------------------------------------------------

#define F_EXIT      0
#define F_REFRESH   1

char mksettings_menu_names[6][18];      // befuellt von: MKSettings_read_Names()


//-------------------------------------
//-------------------------------------
typedef struct
{
    unsigned char   paramsetRevision;       // Revision FC-Parameterset fuer die das Temp-Setting gueltig ist
    unsigned char   *paramset;              // malloc: Pointer auf kopierte Parameter-Daten
} MKSetting_TMP_t;


MKSetting_TMP_t     MKSetting_TMP;


#define ID_SETTING_COPY     10      // fuer MKSettings_Menu()
#define ID_EDIT             20      // fuer MKSettings_AskAction()
#define ID_ACTIVATE         21      // fuer MKSettings_AskAction()
#define ID_COPY             22      // fuer MKSettings_AskAction()

#define ID_SAVE             30      // fuer MKSettings_AskSaveSetting()
#define ID_DISCARD          31      // fuer MKSettings_AskSaveSetting()


//#############################################################################################
//#
//#############################################################################################


//--------------------------------------------------------------
// MKSettings_TMP_Init0()
//
// nur fuer main.c
//--------------------------------------------------------------
void MKSettings_TMP_Init0( void )
{
    memset( &MKSetting_TMP, 0, sizeof(MKSetting_TMP_t) );
    strcpy( mksettings_menu_names[5], "PKT: --empty--");
}



//--------------------------------------------------------------
// MKSettings_TMP_Init()
//
// loeschen / initialisieren vom PKT Temp-Setting
//--------------------------------------------------------------
void MKSettings_TMP_Init( void )
{
    if( MKSetting_TMP.paramset != NULL )
    {
        free( MKSetting_TMP.paramset );
    }
    MKSettings_TMP_Init0();
}



//--------------------------------------------------------------
// from_setting = 6 :  von TMP zu einem MK-Setting
// from_setting <= 5:  von MK-Setting zu TMP
//--------------------------------------------------------------
uint8_t MKSettings_TMP_copy( uint8_t to_setting, uint8_t timeout )
{
    uint8_t written;
    uint8_t size    = paramsetSize();

    if( MKSetting_TMP.paramset == NULL )
    {
        MKSetting_TMP.paramset = malloc( size+1 );                                  // +1 fuer das erste settings-byte
    }

    if( !MKSetting_TMP.paramset )
    {
        //PKT_Message_P( *text, error, timeout, beep, clearscreen )
        PKT_Message_P( PSTR("NO RAM!"), true, 800, true, true );                    // FEHLER!  (NO RAM)
        return 0;                                                                   // -> EXIT
    }

    // von MK-Setting 1..5 zu TMP
    if( to_setting == 6 )
    {
        MKSetting_TMP.paramsetRevision = MKVersion.paramsetRevision;
        memcpy( MKSetting_TMP.paramset, (unsigned char *)(pRxData), size+1 );       // memcpy( dst, src, size))
        return 6;
    }

    // von TMP zu MK-Setting 1..5
    memcpy( (unsigned char *)(pRxData), MKSetting_TMP.paramset , size+1 );          // memcpy( dst, src, size))
    written = MK_Setting_write( to_setting, timeout);
    return written;
}



//#############################################################################################
//#
//#############################################################################################

//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t MKSettings_read_Names( void )
{
    uint8_t setting;

    // die 5 Settings vom Kopter laden um die
    // Setting-Namen zu speichern
    for( setting=1; setting <= 5; setting++ )
    {
        if( !MK_Setting_load( setting, 20 ) )
            return(0);                                  // Fehler - setting konnte nicht geladen werden  (timeout = 20)
        xsnprintf( mksettings_menu_names[setting-1], 16, "%1d: %s", setting, paramGet_p(param_Name) );
    }

    // TMP-Setting
    if( MKSetting_TMP.paramset != NULL )
    {
        if( MKSetting_TMP.paramsetRevision != MKVersion.paramsetRevision )
        {
            // wenn die FC-Revision vom TMP-Setting abweicht vom zuletzt geladenen
            // dann wurde ggf. der Kopter gewechselt -> TMP-Setting verwerfen
            MKSettings_TMP_Init();
        }
        else
        {
            // den Namen aus dem TMP-Setting holen
            paramsetInit( MKSetting_TMP.paramset );
            xsnprintf( mksettings_menu_names[5], 16, "PKT: %s", paramGet_p(param_Name) );
        }
    }

    setting = MK_Setting_load( 0xff, 20);                               // aktuelles MK Setting ermitteln

    return setting;
}



//--------------------------------------------------------------
// wahl = MKSettings_AskAction( setting)
//
// Rueckgabe:
//  0 (==Ende), ID_EDIT, ID_ACTIVATE, ID_COPY
//--------------------------------------------------------------
uint8_t MKSettings_AskAction( uint8_t setting )
{
    uint8_t wahl = 0;

    //-----------------
    // Menue erstellen
    //-----------------
    MenuCtrl_Create();
    MenuCtrl_SetTitle( mksettings_menu_names[setting-1] );                          // Menuetitel ist der Settingname
    MenuCtrl_ShowLevel(false);
    MenuCtrl_SetTopSpace(1);                                                        // oben beim Menue eine Leerzeile einfuegen

    //-----------------
    // Menueeintraege
    //-----------------
    if( !paramExist(param_DUMMY) )                                                  // param_DUMMY -> das Parameterset wird nicht zum bearbeiten unterstuetzt
        MenuCtrl_Push_P( ID_EDIT    , MENU_ITEM, NOFUNC, strGet(EDIT_SETTING) );    // "Setting ändern"

    if( setting != 6)
        MenuCtrl_Push_P( ID_ACTIVATE, MENU_ITEM, NOFUNC, strGet(PARA_AKTIVI) );     // "aktivieren"

    MenuCtrl_Push_P( ID_COPY    , MENU_ITEM, NOFUNC, strGet(STR_COPY) );            // "kopieren"

    //-----------------
    // Menue Control
    //-----------------
    MenuCtrl_Control( MENUCTRL_EVENT );

    if( MenuCtrl_GetKey() != KEY_ESC )
        wahl = MenuCtrl_GetItemId();                                                // welcher Menu-Punkt (ID_CHANGE, ID_ACTIVATE)

    MenuCtrl_Destroy();
    MenuCtrl_ShowLevel(true);

    return wahl;            // 0=Ende; ID_EDIT; ID_ACTIVATE, ID_COPY
}



//--------------------------------------------------------------
// wahl = MKSettings_AskSaveSetting( setting)
//
// Rueckgabe:
//  0 (==Ende), ID_SAVE, ID_DISCARD
//--------------------------------------------------------------
uint8_t MKSettings_AskSaveSetting( uint8_t setting )
{
    uint8_t wahl = 0;

    //-----------------
    // Menue erstellen
    //-----------------
    MenuCtrl_Create();
    MenuCtrl_SetTitle( mksettings_menu_names[setting-1] );                          // Menuetitel ist der Settingname
    MenuCtrl_ShowLevel(false);
    MenuCtrl_SetTopSpace(1);                                                        // oben beim Menue eine Leerzeile einfuegen

    //-----------------
    // Menueeintraege
    //-----------------
    MenuCtrl_Push_P( ID_SAVE    , MENU_ITEM, NOFUNC, strGet(STR_SAVE) );            // "speichern"
    MenuCtrl_Push_P( ID_DISCARD , MENU_ITEM, NOFUNC, strGet(STR_DISCARD) );         // "verwerfen"


    //-----------------
    // Menue Control
    //-----------------
    MenuCtrl_Control( MENUCTRL_EVENT );

    if( MenuCtrl_GetKey() != KEY_ESC )
        wahl = MenuCtrl_GetItemId();                                                // welcher Menu-Punkt (ID_CHANGE, ID_ACTIVATE)

    MenuCtrl_Destroy();
    MenuCtrl_ShowLevel(true);

    return wahl;            // 0=Ende; ID_EDIT; ID_ACTIVATE, ID_COPY
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t MKSettings_Copy( uint8_t from_setting )
{
    const char *mask = PSTR("%15s");
    uint8_t redraw      = true;
    uint8_t loaded      = 0;
    uint8_t to_setting;
    uint8_t written;
    uint8_t key;
    int8_t  yoffs;


    lcd_cls();

    to_setting = from_setting;

    while( true )
    {
        //------------------------
        // anzeigen
        //------------------------
        if( redraw )
        {
            lcd_frect( 0, 0, 127, 7, 1);                                        // Titel: Invers
            lcd_printp_at( 1, 0, strGet(PARA_COPY), MINVERS);

            yoffs = -1;
            lcdx_printp_at( 0, 2, strGet(STR_VON), MNORMAL,  0,yoffs);          // "von"
            lcdx_printf_at_P( 5, 2, MNORMAL, 3,yoffs, mask, mksettings_menu_names[from_setting-1] );

            yoffs = -7;
            lcd_frect( 5*6, 4*8+yoffs, 15*6, 7, 0);
            lcdx_printp_at( 0, 4, strGet(STR_NACH), MNORMAL,  0,yoffs);         // "nach"
            lcdx_printf_at_P( 5, 4, MNORMAL, 3,yoffs, mask, mksettings_menu_names[to_setting-1] );

            lcd_printp_at(0, 7, strGet(KEYLINE2), MNORMAL);                     // Keyline:  <-  ->  Ende  OK

            redraw = false;
        }

        //------------------------
        // Tasten abfragen
        //------------------------
        if( get_key_press(1 << KEY_ESC) )
        {
            return 99;  // nur "ENDE"
        }

        if( get_key_press(1 << KEY_PLUS) )
        {
            if( to_setting == 6 )   to_setting = 1;
            else                    to_setting++;
            redraw = true;
        }

        if( get_key_press(1 << KEY_MINUS) )
        {
            if( to_setting == 1 )   to_setting = 6;
            else                    to_setting--;
            redraw = true;
        }

        //-------------------------------
        // Taste: OK = Setting kopieren?
        //-------------------------------
        if( get_key_press(1 << KEY_ENTER) )
        {
            lcdx_printp_center( 5, strGet(PARA_COPYQ), MNORMAL, 0,1);           // "Wirklich kopieren?" (zentriert)
            lcd_rect_round( 0, 5*8-3, 127, 7+7, 1, R2);                         // Rahmen um die Frage

            lcd_frect( 0, 7*8, 127, 7, 0);                                      // Keyline loeschen
            lcd_printp_at(12, 7, strGet(NOYES), MNORMAL);                       // neue Keyline: "Nein   Ja"

            set_beep( 25, 0xffff, BeepNormal );                                 // kurzer Beep

            key = 0;
            while( !key )                                                       // Abfrage: Ja / Nein
            {
                key = get_key_press(1 << KEY_ENTER) ? KEY_ENTER : key;          // => "Ja" (=Setting kopieren)
                key = get_key_press(1 << KEY_ESC)   ? KEY_ESC   : key;          // => "Nein"
                PKT_CtrlHook();
            }

            //---------------------------
            // "Ja" -> Setting kopieren!
            //---------------------------
            if( key == KEY_ENTER )                                              // => "Ja" -> Setting kopieren!
            {
                if( from_setting != 6 )                                             // kein PKT TMP-Setting
                {
                    loaded = MK_Setting_load( from_setting, 20 );                   // timeout = 20

                    if( loaded != from_setting )                                        // Fehler beim laden - Datenverlust?
                    {
                        //PKT_Message_P( *text, error, timeout, beep, clearscreen )
                        PKT_Message_P( strGet(ERROR_NODATA), true, 800, true, true );   // "Datenverlust!" (max. 8 Sekunden anzeigen)
                        return 0;                                                       // 0 = Ende/Abbruch
                    }
                }

                if( to_setting == 6 )                                               // von Setting 1..5 nach TMP speichern
                {
                    MKSettings_TMP_copy( 6, 10 );
                    return loaded;
                }

                // Anzeige "speichern..."
                lcd_frect( 0, (8*4), 128, (8*4), 0);                            // Anzeigebereich löschen
                lcdx_printp_center( 4, strGet(STR_SAVING), MNORMAL, 0,9);       // Text zentriert; String in PROGMEM
                lcd_rect_round( 0, 37, 127, 14, 1, R2);                         // Rahmen


                if( from_setting == 6 )                                             // von TMP nach Setting 1..5
                {
                    written = MKSettings_TMP_copy( to_setting, 30 );
                    return written;
                }


                if( to_setting <= 5 )                                               // 'echtes' MK Setting speichern
                {
                    written = MK_Setting_write( to_setting, 30 );                   // Timeout = 40
                    return written;
                }
            }

            if( key == KEY_ESC )    // => "Nein" -> nicht kopieren
            {
                lcd_cls();
                redraw = true;
            }

        }  //end: if( get_key_press(1 << KEY_ENTER) )


        //------------------------------------------
        // Pruefe PKT-Update oder andere PKT-Aktion
        //------------------------------------------
        if( PKT_CtrlHook() )                                                // Update vom Updatetool angefordert?
        {
            lcd_cls();
            redraw = true;
        }

    }  // end: while( true )

    return 0;
}



//--------------------------------------------------------------
// ret = MKSettings_Menu()
//
// Rueckgabe:
//  0 = Ende/Fehler/Abbruch
//  1 = Refresh
//--------------------------------------------------------------
uint8_t MKSettings_Menu( void )
{
    uint8_t i;
    uint8_t active_setting;
    uint8_t setting;
    uint8_t wahl;
    uint8_t wahl2;
    uint8_t changed;


    //PKT_Message_P( *text, error, timeout, beep, clearscreen )
    PKT_Message_P( strGet(MSG_LOADSETTINGS), false, 0, true, true );                    // "lade Settings..."


    active_setting = MKSettings_read_Names();

    if( !active_setting )                                                               // Fehler: settings konnten nicht geladen werden...
    {
        //PKT_Message_P( *text, error, timeout, beep, clearscreen )
        PKT_Message_P( strGet(ERROR_NODATA), true, 800, true, true );                   // "Datenverlust!" (max. 8 Sekunden anzeigen)
        return F_EXIT;                                                                  // F_EXIT = Ende/Abbruch
    }


    //---------------
    // Create
    //---------------
    MenuCtrl_Create();

    //---------------
    // Einstellungen
    //---------------
    MenuCtrl_SetTitleFromParentItem();          // "MK Settings"
  //MenuCtrl_SetTitle_P( PSTR("MK Settings") );
  //MenuCtrl_SetCycle( false );
  //MenuCtrl_SetShowBatt( true );


    //---------------
    // Menue-Punkte
    //---------------
    for( i=0; i< ( MKSetting_TMP.paramset!=NULL ? 6 : 5); i++ )
    {
        MenuCtrl_Push( i+1, MENU_SUB, NOFUNC, mksettings_menu_names[i] );               // Setting 1..5
    }

    MenuCtrl_ItemSelect( active_setting );                                              // Menucursor auf aktives Setting setzen
    MenuCtrl_ItemMark( active_setting, true);                                           // aktives Setting markieren


    //---------------
    // Control
    //---------------
    while( true )
    {
        MenuCtrl_Control( MENUCTRL_EVENT );

        if( MenuCtrl_GetKey() == KEY_ESC )                                      // Ende?
        {
            break;                                                              // Ende
        }

        setting = MenuCtrl_GetItemId();                                          // welcher Menu-Punkt (ID)

        if( setting >=1 && setting <= 6 )
        {
            wahl = MKSettings_AskAction( setting );

            //--------------
            // bearbeiten
            //--------------
            if( wahl == ID_EDIT )
            {
                // TODO: Fehler abfangen!
                MK_Setting_load( setting, 25 );                                         // timeout = 25
                changed = MK_Parameters( setting, mksettings_menu_names[setting-1] );

                if( changed && (setting!=6) )
                {
                    wahl2 = MKSettings_AskSaveSetting( setting );
                    if( wahl2 == ID_SAVE )
                    {
                        lcd_frect( 0, (8*7), 128, 7, 0);                                // Keyline löschen
                        lcdx_printp_center( 4, strGet(STR_SAVING), MNORMAL, 0,9);      // Text zentriert; String in PROGMEM
                        lcd_rect_round( 0, 37, 127, 14, 1, R2);                         // Rahmen

                        setting = MK_Setting_write( setting, 50);

                        if( !setting )
                        {
                            //PKT_Message_P( *text, error, timeout, beep, clearscreen )
                            PKT_Message_P( strGet(ERROR_NODATA), true, 800, true, true );           // FEHLER! nodata (max. 8 Sekunden anzeigen)
                            //MenuCtrl_Destroy();
                            //return F_EXIT;                                                      // F_EXIT = Ende/Abbruch
                        }
                    }
                }
            }


            //--------------
            // aktivieren
            //--------------
            if( wahl == ID_ACTIVATE )
            {
                //PKT_Message_P( *text, error, timeout, beep, clearscreen )
                PKT_Message_P( strGet(MSG_ACTIVATESETTING), false, 0, true, true );     // "aktiviere Setting..."

                active_setting = MK_Setting_change( setting );

                if( !active_setting )
                {
                    //PKT_Message_P( *text, error, timeout, beep, clearscreen )
                    PKT_Message_P( strGet(ERROR_NODATA), true, 800, true, true );       // FEHLER! nodata (max. 8 Sekunden anzeigen)
                    //MenuCtrl_Destroy();
                    //return F_EXIT;                                                      // F_EXIT = Ende/Abbruch
                }
                else
                {
                    // neues Setting markieren
                    for( i=1; i<=5; i++ ) MenuCtrl_ItemMark( i, false);                     // Markierungen loeschen
                    MenuCtrl_ItemMark( active_setting, true);                               // aktives Setting markieren
                }

            }  // end: if( wahl == ID_ACTIVATE )


            //--------------
            // kopieren
            //--------------
            if( wahl == ID_COPY )
            {
                active_setting = MKSettings_Copy( setting );

                if( !active_setting )
                {
                    //PKT_Message_P( *text, error, timeout, beep, clearscreen )
                    PKT_Message_P( strGet(ERROR_NODATA), true, 800, true, true );       // FEHLER! nodata (max. 8 Sekunden anzeigen)
                    //MenuCtrl_Destroy();
                    //return F_EXIT;                                                      // F_EXIT = Ende/Abbruch
                }

                if( active_setting != 99 )                                              // =99 bedeutet: User hat die Funktion abgebrochen...
                {
                    MenuCtrl_Destroy();                                                 // ok, kein Abbruch durch den User -> Menue neu aufbauen
                    return F_REFRESH;                                                   // da sich ggf. Settings-Namen geaendert haben
                }
            }  // end: if( wahl == ID_COPY )

        }

    }  // end: while( true )


    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();

    return F_EXIT;
}



//#############################################################################################
//#
//#############################################################################################


//--------------------------------------------------------------
// das muss ueberarbeitet werden....
//--------------------------------------------------------------
static uint8_t check_motorOff(void)
{
    NaviData_t *naviData;

    if( hardware == NC )  // Prüfung funktioniert nur mit NC
    {
        SwitchToNC();
        SendOutData( 'o', ADDRESS_NC, 1,  10, 1);
        mode = 'O';
        rxd_buffer_locked = FALSE;
        timer = 200;

        while( !rxd_buffer_locked && timer>0 );

        if( rxd_buffer_locked )                                 // naviData Ok?
        {
            // timer = MK_TIMEOUT;
            Decode64();
            naviData = (NaviData_t *) pRxData;

            if( naviData->FCStatusFlags & FC_STATUS_MOTOR_RUN )
                return false;
            else
                return true;
        }

        return false;
    }

    return true;    // hmm, wenn man nur eine FC hat dann wird hier immer gemeldet "Motoren sind aus" ?
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void MK_Settings( void )
{
    //MKSettings_TMP_Init();

    //if( true )
    if( !check_motorOff() )
    {
         //PKT_Message_P( *text, error, timeout, beep, clearscreen )
        PKT_Message_P( strGet(STR_SWITCHMOTOROFF), false, 400, true, true );        // "Motoren ausschalten!"
        return;
    }

    while( MKSettings_Menu() == F_REFRESH );
}

