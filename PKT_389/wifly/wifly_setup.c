
/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*
 * wifly_setup.c
 *
 *  Created on: 02.07.2013
 *      Author: cebra
 */
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
 *                        http://www.mikrokopter.de                          *
 *   Gregor "killagreg" Stobrawa for his version of the MK code              *
 *   Thomas Kaiser "thkais" for the original project. See                    *
 *                        http://www.ft-fanpage.de/mikrokopter/              *
 *                        http://forum.mikrokopter.de/topic-4061-1.html      *
 *   Claas Anders "CaScAdE" Rathje for providing the font and his C-OSD code *
 *                        http://www.mylifesucks.de/oss/c-osd/               *
 *   Harald Bongartz "HaraldB" for providing his Ideas and Code for usibility*
 *****************************************************************************/

//############################################################################
//# HISTORY  wifly_setup.c
//#
//# 08.06.2014 OG
//# - chg: Setup_WiFly() - Tipptext fuer Module-Installed ergaenzt
//#
//# 04.06.2014 OG
//# - chg: Menuetext angepasst; Menue-Separator hinzugefuegt
//#
//# 31.05.2014 OG
//# - chg: Setup's auf geaendertes Edit_String() umgestellt (Param 'Text' entfernt)
//#
//# 28.05.2014 OG
//# - chg: Setup's auf das neue Edit_generic() umgestellt
//#
//# 13.04.2014 OG
//# - ggf. Probleme beim Downgrade des WiFly auf v2.38 -> deaktiviert
//# - FAST KOMPLETT NEUER CODE
//#
//# 03.04.2014 OG
//# - add: #include "../pkt/pkt.h"
//#
//# 30.03.2014 OG
//# - chg: Text von WL_INSTALLED geaendert von "Modul eingebaut?" auf "Modul vorhanden?"
//#        -> das Modul wird aufgesteckt und nicht eingebaut...
//# - chg: ein paar englische Texte geaendert
//# - chg: Sprache Hollaendisch vollstaendig entfernt
//# - chg: MenuCtrl_PushML_P() umgestellt auf MenuCtrl_PushML2_P()
//#
//# 11.03.2014 SH
//# - add: neue Menüpunkte Reset, Version und Update
//# - chg: Menüpunkt Passwort wurde wieder aktiviert
//# - chg: beim Menüpunkt Kanal fängt die Auswahl jetzt bei 0 an (Modul sucht sich automatisch den besten Kanal)
//#
//# 12.02.2014 OG
//# - chg: Setup_WiFly() Auskommentiert: "unused variable 'z'"
//# - chg: Setup_WiFly() Auskommentiert: "unused variable 'i'"
//#
//# 02.07.2013 cebra
//# - new: wifly_setup(): Setup für WiFly WLAN-Modul
//###########################################################################


#include "../cpu.h"
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/pgmspace.h>
//#include <string.h>
//#include <util/delay.h>
//#include <string.h>

#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <util/delay.h>
#include <inttypes.h>
#include <string.h>

#include "../main.h"
#include "../messages.h"
#include "../lcd/lcd.h"
#include "../pkt/pkt.h"
#include "../utils/menuctrl.h"
#include "../utils/xutils.h"
#include "../eeprom/eeprom.h"
#include "../uart/uart1.h"
#include "../setup/setup.h"
#include "../wifly/PKT_WiFlyHQ.h"
#include "wifly_setup.h"


#ifdef USE_WLAN


//char buffer[20];      // fuer Versionsabfrage



//-----------------------------
// Setup_WiFly() (WLAN)
//-----------------------------
#define WL_INSTALLED  1
#define WL_SSID       2
#define WL_PASSWORD   3
#define WL_CHANNEL    4
#define WL_INIT       5
#define WL_RESET      6
#define WL_VERSION    7
#define WL_UPDATE     8
#define WL_PCCONFIG   9
#define WL_WPA        10

#define WL_HOMESSID       11
#define WL_HOMEPASSWORD   12
#define WL_TEST           99


static const char WL_INSTALLED_de[]     PROGMEM = "WiFly Modus";
static const char WL_INSTALLED_en[]     PROGMEM = "WiFly mode";

static const char WL_SSID_de[]          PROGMEM = "SSID";
#define           WL_SSID_en            WL_SSID_de

static const char WL_PASSWORD_de[]      PROGMEM = "Passwort";
static const char WL_PASSWORD_en[]      PROGMEM = "Password";

static const char WL_HOMESSID_de[]      PROGMEM = "Home SSID";
#define           WL_HOMESSID_en        WL_HOMESSID_de

static const char WL_HOMEPASSWORD_de[]  PROGMEM = "Home Passwort";
static const char WL_HOMEPASSWORD_en[]  PROGMEM = "Home Password";

static const char WL_CHANNEL_de[]       PROGMEM = "Kanal";
static const char WL_CHANNEL_en[]       PROGMEM = "Channel";

static const char WL_VERSION_de[]       PROGMEM = "zeige Version";
static const char WL_VERSION_en[]       PROGMEM = "show version";

static const char WL_UPDATE_de[]        PROGMEM = "Update";
#define           WL_UPDATE_en          WL_UPDATE_de

static const char WL_PCCONFIG_de[]      PROGMEM = "WiFly einschalten";
static const char WL_PCCONFIG_en[]      PROGMEM = "WiFly power on";

static const char WL_WPA_de[]           PROGMEM = "Sicherheit";
static const char WL_WPA_en[]           PROGMEM = "Security";


//############################################################################


//--------------------------------------------------------------
// Setup_WiFly_MenuCreate()
//
// das Menue aendert sich je nachdem ob WiFly ein- oder
// ausgeschaltet ist
//--------------------------------------------------------------
void Setup_WiFly_MenuCreate( void )
{

    //---------------
    // Create
    //---------------
    MenuCtrl_Create();

    if(      Config.UseWL == WIFLY_APMODE )     MenuCtrl_SetTitle_P( PSTR("WiFly AP-Mode"));
    else if( Config.UseWL == WIFLY_ADHOC  )     MenuCtrl_SetTitle_P( PSTR("WiFly AdHoc"));
    else                                        MenuCtrl_SetTitle_P( PSTR("WiFly"));


    //---------------
    // Menuitems
    //---------------
    MenuCtrl_PushML2_P( WL_INSTALLED , MENU_ITEM, NOFUNC , WL_INSTALLED_de , WL_INSTALLED_en );

    if( Config.UseWL )
    {
      //MenuCtrl_Push_P(      WL_TEST      , MENU_ITEM, NOFUNC          , PSTR("TEST")  );

        MenuCtrl_PushML2_P(   WL_VERSION   , MENU_ITEM, NOFUNC          , WL_VERSION_de   , WL_VERSION_en  );

        MenuCtrl_PushSeparator();

        MenuCtrl_PushML2_P(   WL_SSID      , MENU_ITEM, NOFUNC          , WL_SSID_de      , WL_SSID_en     );

        if( Config.UseWL == WIFLY_APMODE )
            MenuCtrl_PushML2_P( WL_PASSWORD, MENU_ITEM, NOFUNC          , WL_PASSWORD_de  , WL_PASSWORD_en );

        MenuCtrl_PushML2_P(   WL_CHANNEL   , MENU_ITEM, NOFUNC          , WL_CHANNEL_de   , WL_CHANNEL_en  );


        if( Config.UseWL == WIFLY_APMODE )
            MenuCtrl_Push_P( WL_INIT       , MENU_ITEM, NOFUNC          , PSTR("INIT: AP-Mode")  );
        else
            MenuCtrl_Push_P( WL_INIT       , MENU_ITEM, NOFUNC          , PSTR("INIT: AdHoc")  );

        MenuCtrl_PushSeparator();

        MenuCtrl_PushML2_P( WL_HOMESSID    , MENU_ITEM, NOFUNC          , WL_HOMESSID_de      , WL_HOMESSID_en     );
        MenuCtrl_PushML2_P( WL_HOMEPASSWORD, MENU_ITEM, NOFUNC          , WL_HOMEPASSWORD_de  , WL_HOMEPASSWORD_en );
        MenuCtrl_Push_P(    WL_UPDATE      , MENU_ITEM, NOFUNC          , PSTR("UPDATE: WiFly")  );

        MenuCtrl_PushSeparator();

        MenuCtrl_Push_P(    WL_RESET       , MENU_ITEM, NOFUNC          , PSTR("RESET: WiFly")  );
        MenuCtrl_PushML2_P( WL_PCCONFIG    , MENU_ITEM, &Port_WiFly2Wi  , WL_PCCONFIG_de  , WL_PCCONFIG_en );
      //MenuCtrl_PushML2_P( WL_WPA         , MENU_ITEM, NOFUNC          , WL_WPA_de       , WL_WPA_en      );


        if( Config.UseWL == WIFLY_ADHOC )
        {
            // aktuell kann es zu Problemen kommen wenn das WiFly
            // auf v2.38 gedowngraded wird (WiFly nicht mehr ansprechbar)
            // -> deshalb: deaktiviert
            MenuCtrl_ItemActive( WL_UPDATE, false );
        }
    }
}



//--------------------------------------------------------------
// Setup_WiFly()
//--------------------------------------------------------------
void Setup_WiFly( void )
{
    uint8_t itemid;
    uint8_t UseWL;
    char    string[20];

    Setup_WiFly_MenuCreate();

    //---------------
    // Control
    //---------------
    while( true )
    {
        MenuCtrl_Control( MENUCTRL_EVENT );

        if( MenuCtrl_GetKey() == KEY_ESC )  break;      // ENDE

        itemid = MenuCtrl_GetItemId();                  // welcher Menu-Punkt (ID)
        edit = 0;


        //--------------------
        // TEST
        //--------------------
        /*
        if( itemid == WL_TEST )
        {
            lcd_cls();
            ShowTitle_P( PSTR("TEST"), true );
            wifly_test();
        }
        */



        //--------------------
        // Wlan_INSTALLED
        //--------------------
        if( itemid == WL_INSTALLED )
        {
            UseWL = Config.UseWL;

            Config.UseWL = Edit_generic( Config.UseWL, 0, 2, WlanMode, 1,  strGet(STR_EXTSV2MODULE),NULL);

            if( UseWL != Config.UseWL )                 // hat Benutzer Einstellung geaendert?
            {
                MenuCtrl_Destroy();                     // Menue aendern wegen wechsel Wlan vorhanden / nicht vorhanden
                Setup_WiFly_MenuCreate();
                continue;
            }
        }


        //--------------------
        // WL_VERSION
        //--------------------
        if( itemid == WL_VERSION )
        {
            wifly_version_show();
        }


        //--------------------
        // WL_SSID
        //--------------------
        if( itemid == WL_SSID )
        {
            strncpyfill( string, Config.Wlan_SSID, wlan_ssid_length+1 );                // wlan_ssid_length
            Edit_String( string, wlan_ssid_length , EDIT_SSID );

            if( edit == 1 )
            {
                strrtrim( EditString );                                                  // Leerzeichen rechts entfernen
                strncpy( Config.Wlan_SSID, EditString, wlan_ssid_length+1 );
            }
        }


        //--------------------
        // WL_PASSWORD
        //--------------------
        if( itemid == WL_PASSWORD)
        {
            strncpyfill( string, Config.Wlan_Password, wlan_password_length+1 );                    //
            Edit_String( string, wlan_password_length , EDIT_WL_PASSWORD );

            if( edit == 1 )
            {
                strrtrim( EditString );                                                  // Leerzeichen rechts entfernen
                strncpy( Config.Wlan_Password, EditString, wlan_password_length+1 );
            }
        }


        //--------------------
        // WL_HOMESSID
        //--------------------
        if( itemid == WL_HOMESSID )
        {
            strncpyfill( string, Config.Wlan_HomeSSID, wlan_ssid_length+1 );                // wlan_ssid_length
            Edit_String( string, wlan_ssid_length , EDIT_SSID );

            if( edit == 1 )
            {
                strrtrim( EditString);                                                  // Leerzeichen rechts entfernen
                strncpy( Config.Wlan_HomeSSID, EditString, wlan_ssid_length+1 );
            }
        }


        //--------------------
        // WL_HOMEPASSWORD
        //--------------------
        if( itemid == WL_HOMEPASSWORD)
        {
            strncpyfill( string, Config.Wlan_HomePassword, wlan_password_length+1 );                    //
            Edit_String( string, wlan_password_length , EDIT_WL_PASSWORD );

            if( edit == 1 )
            {
                strrtrim( EditString );                                                  // Leerzeichen rechts entfernen
                strncpy( Config.Wlan_HomePassword, EditString, wlan_password_length+1 );
            }
        }



        //--------------------
        // WL_CHANNEL
        //--------------------
        if( itemid == WL_CHANNEL )
        {
            Config.Wlan_Channel = Edit_generic( Config.Wlan_Channel,  0,13,Show_uint3,1  ,NULL,NULL);
        }


        //--------------------
        // WL_INIT
        //--------------------
        if( itemid == WL_INIT )
        {
            wifly_init( Config.UseWL ); // Config.UseWL = WIFLY_APMODE oder WIFLY_ADHOC
        }


        //--------------------
         // Wlan_RESET
         //--------------------
         if( itemid == WL_RESET )
         {
            wifly_reset();
         }


         //--------------------
         // Wlan_UPDATE
         //--------------------
         if( itemid == WL_UPDATE )
         {
            wifly_update( Config.UseWL );   // Config.UseWL = WIFLY_APMODE oder WIFLY_ADHOC
         }

         //--------------------
         // Wlan_WPA
         //--------------------
         //if( itemid == WL_WPA )
         //{
         //   Config.Wlan_WPA = Edit_generic(Config.Wlan_WPA,0,1,WL3,WlanSecurity,1);
         //}

    }  // end: while( true )

    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();

} // end: Setup_WiFly()

#endif
