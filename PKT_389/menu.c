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
//# HISTORY  menu.c
//#
//# 17.11.2015 Cebra
//# - chg: nach dem Setup_MAIN wird das Mainmenü neu gezeichnet wenn sich Wi232 Parameter geändert haben
//#        damit der MK neu gesucht werden kann.
//#
//# 14.10.2015 Starter
//# fix: -FollowMeStep2 Debug wird nur noch bei FollowMeStep2 angezeigt
//#
//# 03.08.2015 Cebra
//# - add: Menü _DefMenu_Main_NO erweitert für GPS Test , mit #define schaltbar
//#
//# 09.04.2015 Cebra
//# - add: Vorbereitung für ACC Kalibrartion aus dem Menü
//#        #ifdef USE_ACCCALIBRATION
//#
//# 27.06.2014 OG
//# - chg: Menu_PKTSpezial() - Tracking etwas nach oben verschoben
//#
//# 26.06.2014 OG
//# - chg: Menu_PKTSpezial() - ergaenzt um Setup_FollowMe()
//#
//# 18.06.2014 OG
//# - chg: Menu_PKTSpezial() umorganisiert
//#
//# 14.06.2014 OG
//# - chg: _DefMenu_Main_NO() - "PKT Spezial" wird nicht mehr angezeigt wenn
//#        kein Kopter gefunden wurde; aktuell sind dort nur Funktionen
//#        enthalten die einen Kopter benoetigen - wenn sich das mal irgendwann
//#        aendert muss man das anpassen
//#
//# 01.06.2014 OG
//# - chg: Menu_PKTSpezial() - Aufruf von Tracking und Followme ergaenzt
//#        um Abfrage MKVersion.isNC
//# - chg: _ConfigMenu_Main() umgestellt auf MKVersion.isNC/isFC
//#
//# 14.05.2014 OG
//# - chg: Aufruf von gps() geaendert zu MK_Gps_Info()
//# - chg: include "gps/gps.h" geaendert zu "mk/mkgpsinfo.h"
//#
//# 13.05.2014 OG
//# - del: USE_MAINMENU_SEPARATOR (verschoben nach main.h)
//# - chg: Menu_Main() - del unused variable 'event'
//#
//# 11.05.2014 OG
//# - chg: Menu_PKTSpezial() umgestellt auf MenuCtrl_SetTitleFromParentItem()
//#        -> die Menues 'erben' damit ihren Titel vom aufrufenden Menuepunkt
//#
//# 08.04.2014 OG
//# - chg: die Menuetrennlinien werden jetzt via define USE_MAINMENU_SEPARATOR
//#        angezeigt (das define ist hier im Code) und sind erstmal wieder
//#        abgeschaltet
//#
//# 08.04.2014 OG
//# - add: _DefMenu_Main_NC() - Menuetrennlinien hinzugefuegt
//# - add: _DefMenu_Main_FC(), _DefMenu_Main_NO() - Menuetrennlinien hinzugefuegt
//#
//# 01.04.2014 OG
//# - chg: PCB_WiFlyPatch umbenannt zu PCB_SV2RxTxPatch
//# - add: PKT-Connect(): "BLE Extender" (ext. Bluetooth 4 Low Energy Modul an SV2)
//# - fix: Menu_PKTConnect(): "WLAN Extender" nur sichtar wenn Config.UseWL gesetzt ist
//#
//# 30.03.2014 OG
//# - chg: Sprache Hollaendisch vollstaendig entfernt
//# - chg: MenuCtrl_PushML_P() umgestellt auf MenuCtrl_PushML2_P()
//#
//# 29.03.2014 OG
//# - chg: versch. Funktionen: del: MenuCtrl_SetShowBatt() wegen Aenderung
//#        des Defaults auf true
//#
//# 24.03.2014 OG
//# - chg: "MK Info" vor "MK Settings" verschoben
//#
//# 28.02.2014 OG
//# - chg: ID_MKSETTINGS in den Menues fuer NC/FC weiter nach oben geschoben
//#
//# 27.02.2014 OG
//# - chg: Menuetexte fuer Menu_PKTConnect()
//#
//# 26.02.2014 OG
//# - chg: DEBUG_NEW_PARAMETERS umbenannt zu DEBUG_PARAMSET
//#
//# 18.02.2014 OG
//# - chg: Aufruf von MK_Parameters() ist jetzt MK_Settings() und geht auf
//#        den Source mk/mksettings.c
//#
//# 17.02.2014 OG
//# - chg: aus "MK Parameters" ist "MK Settings" geworden! Entsprechende
//#        defines zu ID's, USE und Sprachen haben sich geaendert
//# - chg: umbenannt: USE_MKPARAMETER -> USE_MKSETTINGS
//#
//# 12.02.2014 OG
//# - del: die includes zu den alten parameter... entfernt
//# - chg: auf mk/mkparameters.c/h umgestellt -> MK_Parameters()
//# - chg: auf 'MKVersion.paramsetOK' umgestellt
//#
//# 05.02.2014 OG
//# - add: DEBUG_NEW_PARAMETERS in Hauptmenues fuer Entwicklung
//#
//# 30.01.2014 OG
//# - add: Unterstuetzung fuer USE_BLUETOOTH
//#
//# 29.01.2014 OG
//# - chg: Umstrukturierung vom Hauptmenue
//#          chg: "PKT Tools" -> "PKT Connect" (nur noch Connect-Funktionen drin)
//#          add: "PKT Spezial" -> MK-Motortest und Spezial-Funktionen
//#               wie Joystick, Tracking, FollowMe
//# - chg: call: display_debug() -> MK_DebugData()
//# - chg: call: display_data() -> MK_Display()
//# - chg: #include "debug.h" -> "mk/mkdebugdata.h"
//# - chg: #include "display.h" -> #include "mk/mkdisplay.h"
//# - add: #include "mk/mkbase.h"
//#
//# 25.01.2014 OG
//# - chg: MK-Display und MK-DebugData auch bei falscher FC-Revision wieder
//#        aktiviert
//#
//# 07.01.2014 Cebra
//# - chg: Abfrage WrongFCVersion etwas verändert
//#
//# 06.01.2014 Cebra
//# - add: Alle Menüpunkte die FC versionabhängig werden bei falscher FC Version ausgeblendet
//#
//# 04.10.2013 Cebra
//# - add: Motortest ohne FC = I2C-Anschluss PKT, mit FC/NC Motortest über FC
//#
//# 26.06.2013 Cebra
//# - add: Menüpunkt Wlan Rangeextender
//#
//# 24.06 2013 OG
//# - chg: Menu_PKTTools(): Menuepunkte fuer PKT-SV2-Verbindungen werden
//#        durch define USE_PKTTOOLS_SV2 (main.h) an-/ausgeschaltet
//# - chg: Menu_PKTTools(): Menuepunkte werden je nach aktiviertem Wi232 und
//#        Bluetooth ein-/ausgeblendet
//# - chg: Menuetexte fuer ID_USB2SV2 und ID_BT2SV2 geaendert
//# - chg: defines fuer Menuetexte von PKT-Tools geaendert
//# - del: verschiedene Exec_* Funktionen
//#
//# 24.06 2013 Cebra
//# - add: Menuepunkt MKUSB in PKT-Tools
//#
//# 13.06.2013 OG
//# - chg: Fastconnect auf Menu_PKTTools() umgelegt
//# - chg: Menueeintraege von Menu_PKTTools() modifiziert und reduziert
//# - chg: in allen Hauptmenues ist PKT-Setup und PKT-Info drin
//#
//# 11.06.2013 OG
//# - chg: Code zu Menu_OSDData() ausgegliedert nach osddata.c
//#
//# 24.05.2013 OG
//# - chg: Aufrufe von MenuCtrl_Push() umgestellt auf MenuCtrl_PushML_P()
//#
//# 21.05.2013 OG
//# - chg: Menu_Main() umgestellt auf MENUCTRL_EVENT
//# - fix: Menu_Main() nach Menu-Redraw searchMK()
//# - chg: Menu_Setup() umbenannt zu Setup_MAIN()
//# - del: include utils/menuold.h
//#
//# 20.05.2013 OG
//# - chg: Layout Menutitel
//#
//# 19.05.2013 OG
//# - chg: Menuepunkte Tracking und FollowMe werden nur angezeigt wenn BTM222
//#        installiert ist (Config.UseBT == true)
//# - add: Menu_PKTTools() (ehemals in tools.c)
//# - del: PC_Fast_Connect() verschoben nach pkt.c
//# - chg: main_menu() umbenannt zu Menu_Main()
//#
//# 19.05.2013 OG
//# - chg: Funktionen nach main.c verschoben
//#        CheckUpdate(), Update_PKT(), PKT_Info(), PKT_SwitchOff()
//#
//# 18.05.2013 OG
//# - chg: PKT_Info() erweitert um optionale Module-Info (via USE_MODULEINFO)
//#        und Credits chronoligisch umgedreht
//#
//# 18.05.2013 OG
//# - add: USE_JOYSTICK, USE_TRACKING, USE_OSDDATA, USE_MKPARAMETER, USE_MKDISPLAY
//# - chg: redundante Menue-Strings sind untereinander verpointert um Platz zu
//#        sparen
//# - chg: Umstellung auf neue menuctrl.c und damit starke Strukturaenderung
//#        Hinweis: autom. PKT-Update via Hauptmenue geht noch nicht - wird
//#        wieder eingebaut (wenn mï¿½glich direkt in menuctrl.c dann geht
//#        das in allen Menues)
//# - del: Ausgliederung der alten Menue-Funktionen nach utils/menuold.c
//#        inkl. not_possible()
//#
//# 16.05.2013 OG
//# - add: define USE_MKGPSINFO fuer gps()
//#
//# 05.05.2013 Cebra
//# - chg: Fehler im Menü bei nicht verfügbaren Funktionen
//#
//# 02.05.2013 OG
//# - chg: Menuetext: osddata_menuitems:  "MK Fehlerliste" -> "MK Fehler"
//# - chg: Menuetext: "OSD Anzeige" wieder zurueck zu "OSD"
//#
//# 28.04.2013 OG
//# - chg: Show_Version() auf ScrollBox umgestellt
//# - chg: Menuetext "Debug Data" zu "MK Debug Data" (kam frueher zu Verwirrungen bei mir)
//# - chg: main_menu() bzgl. Menu_OSDDATA()
//# - add: Menu_OSDDATA() (aus ehemaligem osd/osd_tools.c)
//#
//# 21.04.2013 Cebra
//# - chg: OSD-Tools im Menue integriert
//#
//# 16.04.2013 Cebra
//# - chg: PROGMEM angepasst auf neue avr-libc und GCC, prog_char depreciated
//#
//# 14.04.2013 OG
//# - WIP: Anzeige "OSD-Daten" in param_menuitems_nc, param_menuitems_no
//#
//# 04.04.2013 Cebra
//# - chg:Texttuning
//#       not_possible, für Menüpunkte nicht nicht wählbar sind
//#
//# 03.04.2013 OG
//# - fix: Anzeigefehler wenn (hardware == NC) und nicht gesetzt define 'USE_MKDEBUGDATA' (bzw. 'analognames')
//# - chg: Layout von Anzeige wenn USE_MKDEBUGDATA nicht verfuegbar (jetzt Invers)
//# - chg: define 'analognames' zu define 'USE_MKDEBUGDATA'
//#
//# 30.03.2013 CB
//# - add: automatischer Start der PKT-Updatefunktion im Mainmenüe für das Updatetool
//#
//# 28.03.2013 CB
//# - chg: Hinweis wenn Debug Data nicht möglich ist
//#
//# 10.03.2013 Cebra
//# - add: menu_select, gemeinsame Routine für alle Setupmenüs
//#
//# 27.03.2013 Cebra
//# - chg: Fehler bei der Menüsteuerung behoben
//############################################################################


#include "cpu.h"
#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <string.h>

#include "main.h"
#include "lcd/lcd.h"
#include "menu.h"
#include "mk/mkdisplay.h"
#include "mk/mkdebugdata.h"
#include "timer/timer.h"
#include "osd/osd.h"
#include "osd/osddata.h"
#include "motortest/motortest.h"
#include "mk/mkgpsinfo.h"
#include "eeprom/eeprom.h"
#include "setup/setup.h"
#include "uart/uart1.h"
#include "mk-data-structs.h"
#include "wi232/Wi232.h"

#include "connect.h"
#include "lipo/lipo.h"
#include "messages.h"
#include "bluetooth/bluetooth.h"
#include "followme/followme.h"
#include "tracking/ng_servo.h"
#include "tracking/tracking.h"
#include "stick/stick.h"
#include "utils/menuctrl.h"
#include "pkt/pkt.h"
#include "mk/mkbase.h"
#include "mksettings/paramset.h"
#include "mksettings/mksettings.h"


//############################################################################

char titlemain[]  = "PKT v                ";    // buffer fuer PKT-Versionsinfo
                   //123456789012345678901



// nur fuer Entwicklung...
static const char DEBUG_PARAMSET_Text[]         PROGMEM = "DEBUG PARAMSET";

//############################################################################

//-----------------------
// main_menu
//-----------------------
#define ID_OSD              1
#define ID_OSDDATA          2
#define ID_MKDISPLAY        3
#define ID_MKSETTINGS       4
#define ID_MKDEBUGDATA      5
#define ID_MKGPSINFO        6
#define ID_SEARCHMK         7
#define ID_PKTCONNECT       8
#define ID_PKTSETUP         9
#define ID_PKTINFO          10
#define ID_PKTSPEZIAL       11
#define ID_MKINFO           12
#define ID_MKACCCAL         13

#define ID_DEBUG_GPS        14


static const char SEARCHMK_de[]         PROGMEM = "Suche Mikrokopter";
static const char SEARCHMK_en[]         PROGMEM = "search Kopter";

static const char OSD_de[]              PROGMEM = "OSD";
#define OSD_en OSD_de

static const char OSDDATA_de[]          PROGMEM = "OSD Daten";
static const char OSDDATA_en[]          PROGMEM = "OSD Data";

static const char MKINFO_de[]           PROGMEM = "MK Info";
#define MKINFO_en  MKINFO_de

static const char MKDISPLAY_de[]        PROGMEM = "MK Display";
#define MKDISPLAY_en MKDISPLAY_de

static const char MKACCCAL_de[]         PROGMEM = "MK ACC Kalibr.";
static const char MKACCCAL_en[]         PROGMEM = "MK ACC Calibr.";

static const char MKSETTINGS_de[]       PROGMEM = "MK Settings";
#define MKSETTINGS_en  MKSETTINGS_de

static const char MKDEBUGDATA_de[]      PROGMEM = "MK Daten";
static const char MKDEBUGDATA_en[]      PROGMEM = "MK Data";

static const char MKGPSINFO_de[]        PROGMEM = "MK GPS Info";
#define MKGPSINFO_en MKGPSINFO_de

static const char PKTCONNECT_de[]       PROGMEM = "PKT Connect";
#define PKTCONNECT_en PKTCONNECT_de

static const char PKTSPEZIAL_de[]       PROGMEM = "PKT Spezial";
static const char PKTSPEZIAL_en[]       PROGMEM = "PKT Special";

static const char PKTSETUP_de[]         PROGMEM = "PKT Setup";
#define PKTSETUP_en  PKTSETUP_de

static const char PKTINFO_de[]          PROGMEM = "PKT Info";
#define PKTINFO_en  PKTINFO_de


#ifdef USE_FOLLOWME_STEP2
static const char PKTGPS_de[]          PROGMEM = "PKT GPS Debug";
#define PKTGPS_en  PKTGPS_de
#endif


//############################################################################

//-----------------------------
// Menu_PKTSpezial()
//-----------------------------
#define ID_BLTESTER         60
#define ID_TRACKING         61
#define ID_JOYSTICK         62
#define ID_FOLLOWME         63
#define ID_FOLLOWMESETUP    64

static const char BLTESTER_de[]         PROGMEM = "MK Motortest";
#define BLTESTER_en                     BLTESTER_de

static const char TRACKING_de[]         PROGMEM = "Tracking";
#define TRACKING_en                     TRACKING_de

static const char JOYSTICK_de[]         PROGMEM = "Joystick";
#define JOYSTICK_en                     JOYSTICK_de

static const char FOLLOWME_de[]         PROGMEM = "Follow Me";
#define FOLLOWME_en                     FOLLOWME_de

static const char FOLLOWMESETUP_de[]    PROGMEM = "Follow Me Setup";
#define FOLLOWMESETUP_en                FOLLOWMESETUP_de

//############################################################################

//-----------------------------
// Menu_PKTConnect()
//-----------------------------
#define ID_BT2WI232         40
#define ID_USB2WI232        41
#define ID_USB2SV2          42
#define ID_BT2SV2           43
#define ID_WIFLY2WI232      44
#define ID_BLE2WI232        45



static const char BT2WI232_de[]         PROGMEM = "BT   Extender";
#define           BT2WI232_en           BT2WI232_de

static const char BLE2WI232_de[]        PROGMEM = "BLE  Extender";
#define           BLE2WI232_en          BLE2WI232_de

static const char WIFLY2WI232_de[]      PROGMEM = "WLAN Extender";
#define           WIFLY2WI232_en        WIFLY2WI232_de

static const char USB2WI232_de[]        PROGMEM = "USB  -> Wi232";
#define           USB2WI232_en          USB2WI232_de

#ifdef USE_PKTTOOLS_SV2
static const char USB2SV2_de[]          PROGMEM = "USB  -> SV2";
#define           USB2SV2_en            USB2SV2_de
#endif

#ifdef USE_PKTTOOLS_SV2
static const char BT2SV2_de[]           PROGMEM = "BT   -> SV2";
#define           BT2SV2_en             BT2SV2_de
#endif


//#############################################################################################
//# Hilfsfunktionen & Verschiedenes
//#############################################################################################


//--------------------------------------------------------------
// wird von Menu_PKTTools() verwendet
//--------------------------------------------------------------
void Exec_BLTESTER(void)
{
  if(hardware == NO) motor_test( I2C_Mode );
  if(hardware == NC) motor_test( FC_Mode );
  if(hardware == FC) motor_test( FC_Mode );

}


//#############################################################################################
//# Menu: Spezial
//#############################################################################################



//--------------------------------------------------------------
// Menue fuer 'PKTSpezial'
//--------------------------------------------------------------
void Menu_PKTSpezial( void )
{
    //---------------
    // Create
    //---------------
    MenuCtrl_Create();

    //---------------
    // Einstellungen
    //---------------
    MenuCtrl_SetTitleFromParentItem();      // "PKT Spezial"
  //MenuCtrl_SetTitle_P( PSTR("PKT Spezial") );
  //MenuCtrl_SetShowBatt( true );

    //---------------
    // Menuitems: PKTSpezial
    //---------------
    #ifdef USE_FOLLOWME
    if( Config.UseBT && MKVersion.isNC )
    {
      MenuCtrl_PushML2_P( ID_FOLLOWME     , MENU_ITEM, &FollowMe      , FOLLOWME_de     , FOLLOWME_en      );
      MenuCtrl_PushML2_P( ID_FOLLOWMESETUP, MENU_ITEM, &Setup_FollowMe, FOLLOWMESETUP_de, FOLLOWMESETUP_en );
    }
    #endif


    #ifdef USE_TRACKING
    if( Config.UseBT && MKVersion.isNC )
    {
      MenuCtrl_PushML2_P( ID_TRACKING   , MENU_ITEM, &PKT_tracking , TRACKING_de   , TRACKING_en  );
    }
    #endif


    #ifdef USE_JOYSTICK
    MenuCtrl_PushML2_P( ID_JOYSTICK   , MENU_ITEM, &joystick       , JOYSTICK_de   , JOYSTICK_en  );
    #endif


    MenuCtrl_PushSeparator();       // MENUE TRENNLINIE

    MenuCtrl_PushML2_P( ID_BLTESTER    , MENU_ITEM, &Exec_BLTESTER , BLTESTER_de    , BLTESTER_en );

    // {"Servo Tester     ","servo test       ","Servo Tester     "},
    // if((val+offset) == 2 )  servo_test();

    //---------------
    // Control
    //---------------
    MenuCtrl_Control( MENUCTRL_EVENT );

    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();
}


//#############################################################################################
//# Menu: PKT-Connect
//#############################################################################################

//--------------------------------------------------------------
// Menue fuer; PKT-Connect
//
// verbindet das PKT mit anderen Devices wie Tablet usw.
//--------------------------------------------------------------
void Menu_PKTConnect( void )
{
    //---------------
    // Create
    //---------------
    MenuCtrl_Create();

    //---------------
    // Einstellungen
    //---------------
    MenuCtrl_SetTitle_P( PSTR("PKT Connect") );     // kann nicht auf MenuCtrl_SetTitleFromParentItem() umgestellt
                                                    // werden da PKT-Connect auch durch einen Hot-Key aufgerufen
                                                    // werden kann
  //MenuCtrl_SetCycle( false );
  //MenuCtrl_SetShowBatt( true );
  //MenuCtrl_SetBeep( true );

    //---------------
    // Menuitems
    //---------------
    #ifdef USE_BLUETOOTH
    if( Config.UseBT && Config.UseWi )
        MenuCtrl_PushML2_P( ID_BT2WI232    , MENU_ITEM, &Port_BT2Wi    , BT2WI232_de    , BT2WI232_en    );
    #endif

    #ifdef USE_SV2MODULE_BLE
    if( PCB_SV2RxTxPatch && Config.UseBLE )
        MenuCtrl_PushML2_P( ID_BLE2WI232   , MENU_ITEM, &Port_BLE2Wi   , BLE2WI232_de   , BLE2WI232_en );
    #endif

    if( PCB_SV2RxTxPatch && Config.UseWL )
        MenuCtrl_PushML2_P( ID_WIFLY2WI232 , MENU_ITEM, &Port_WiFly2Wi , WIFLY2WI232_de , WIFLY2WI232_en );

    if( Config.UseWi )
        MenuCtrl_PushML2_P( ID_USB2WI232   , MENU_ITEM, &Port_USB2Wi   , USB2WI232_de   , USB2WI232_en   );

    #ifdef USE_PKTTOOLS_SV2     // am PKT-SV2 liegen +5 Volt an...
        MenuCtrl_PushML2_P( ID_USB2SV2     , MENU_ITEM, &Port_USB2FC   , USB2SV2_de     , USB2SV2_en     );

    #ifdef USE_BLUETOOTH
    if( Config.UseBT )
        MenuCtrl_PushML2_P( ID_BT2SV2      , MENU_ITEM, &Port_BT2FC    , BT2SV2_de      , BT2SV2_en      );
    #endif

    #endif   // end: #ifdef USE_PKTTOOLS_SV2

    //---------------
    // Control
    //---------------
    MenuCtrl_Control( MENUCTRL_EVENT );

    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();
}


//#############################################################################################
//# Menu: Menu_Main
//#############################################################################################

//--------------------------------------------------------------
//--------------------------------------------------------------
void _DefMenu_Main_NC( void )
{


    MenuCtrl_PushML2_P( ID_OSD        , MENU_ITEM, &osd              , OSD_de        , OSD_en         );

    #ifdef USE_OSDDATA
    MenuCtrl_PushML2_P( ID_OSDDATA    , MENU_SUB , &Menu_OSDData     , OSDDATA_de    , OSDDATA_en     );
    #endif

    #ifdef USE_MAINMENU_SEPARATOR
    MenuCtrl_PushSeparator();   // MENUE TRENNLINIE
    #endif

    MenuCtrl_PushML2_P( ID_MKINFO     , MENU_ITEM, NOFUNC            , MKINFO_de     , MKINFO_en      );

    #ifdef USE_MKSETTINGS
    if( MKVersion.paramsetOK )
    {
       MenuCtrl_PushML2_P( ID_MKSETTINGS, MENU_SUB , &MK_Settings   , MKSETTINGS_de, MKSETTINGS_en    );
    }
    #endif

    #ifdef USE_MKDISPLAY
//    ACC_Display = false;
    MenuCtrl_PushML2_P( ID_MKDISPLAY  , MENU_ITEM, &MK_Display       , MKDISPLAY_de  , MKDISPLAY_en   );
    #endif

    #ifdef USE_ACCCALIBRATION
    if ((MKVersion_Cmp( MKVersion.FCVer,  2,9,'i')  == VERSION_EQUAL) || (MKVersion_Cmp( MKVersion.FCVer,  2,9,'i')  == VERSION_GREATER))
      {
        ACC_Display = true;
        MenuCtrl_PushML2_P( ID_MKACCCAL   , MENU_ITEM, &MK_Display       , MKACCCAL_de  , MKACCCAL_en   );
      }
    #endif

    #ifdef USE_MKDEBUGDATA
    MenuCtrl_PushML2_P( ID_MKDEBUGDATA, MENU_ITEM, &MK_DebugData     , MKDEBUGDATA_de, MKDEBUGDATA_en );
    #endif


    #ifdef USE_MKGPSINFO
    MenuCtrl_PushML2_P( ID_MKGPSINFO  , MENU_ITEM, &MK_Gps_Info      , MKGPSINFO_de  , MKGPSINFO_en   );
    #endif

    #ifdef USE_MAINMENU_SEPARATOR
    MenuCtrl_PushSeparator();   // MENUE TRENNLINIE
    #endif

    MenuCtrl_PushML2_P( ID_PKTSPEZIAL , MENU_SUB , &Menu_PKTSpezial  , PKTSPEZIAL_de , PKTSPEZIAL_en  );
    MenuCtrl_PushML2_P( ID_PKTCONNECT , MENU_SUB , &Menu_PKTConnect  , PKTCONNECT_de , PKTCONNECT_en  );
    MenuCtrl_PushML2_P( ID_PKTSETUP   , MENU_SUB , NOFUNC            , PKTSETUP_de   , PKTSETUP_en    );
    MenuCtrl_PushML2_P( ID_PKTINFO    , MENU_ITEM ,&PKT_Info         , PKTINFO_de    , PKTINFO_en     );


    #ifdef DEBUG_PARAMSET
    MenuCtrl_Push_P( 0  , MENU_ITEM, &paramsetDEBUG , DEBUG_PARAMSET_Text  );
    #endif
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void _DefMenu_Main_FC( void )
{
    MenuCtrl_PushML2_P( ID_MKINFO     , MENU_ITEM, NOFUNC            , MKINFO_de     , MKINFO_en      );

    #ifdef USE_MKSETTINGS
    if( MKVersion.paramsetOK )
    {
       MenuCtrl_PushML2_P( ID_MKSETTINGS, MENU_SUB , &MK_Settings   , MKSETTINGS_de, MKSETTINGS_en    );
    }
    #endif



    #ifdef USE_MKDISPLAY
//    ACC_Display = false;
    MenuCtrl_PushML2_P( ID_MKDISPLAY  , MENU_ITEM, &MK_Display       , MKDISPLAY_de  , MKDISPLAY_en   );
    #endif

    #ifdef USE_ACCCALIBRATION
    if ((MKVersion_Cmp( MKVersion.FCVer,  2,9,'i')  == VERSION_EQUAL) || (MKVersion_Cmp( MKVersion.FCVer,  2,9,'i')  == VERSION_GREATER))
      {
        ACC_Display = true;
        MenuCtrl_PushML2_P( ID_MKACCCAL   , MENU_ITEM, &MK_Display       , MKACCCAL_de  , MKACCCAL_en   );
      }
    #endif

    #ifdef USE_MKDEBUGDATA
    MenuCtrl_PushML2_P( ID_MKDEBUGDATA, MENU_ITEM, &MK_DebugData     , MKDEBUGDATA_de, MKDEBUGDATA_en );
    #endif

    #ifdef USE_MAINMENU_SEPARATOR
    MenuCtrl_PushSeparator();   // MENUE TRENNLINIE
    #endif

    MenuCtrl_PushML2_P( ID_PKTSPEZIAL , MENU_SUB , &Menu_PKTSpezial  , PKTSPEZIAL_de , PKTSPEZIAL_en  );
    MenuCtrl_PushML2_P( ID_PKTCONNECT , MENU_SUB , &Menu_PKTConnect  , PKTCONNECT_de , PKTCONNECT_en  );
    MenuCtrl_PushML2_P( ID_PKTSETUP   , MENU_SUB , NOFUNC            , PKTSETUP_de   , PKTSETUP_en    );
    MenuCtrl_PushML2_P( ID_PKTINFO    , MENU_ITEM ,&PKT_Info         , PKTINFO_de    , PKTINFO_en     );


    #ifdef DEBUG_PARAMSET
    MenuCtrl_Push_P( 0  , MENU_ITEM, &paramsetDEBUG , DEBUG_PARAMSET_Text  );
    #endif
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void _DefMenu_Main_NO( void )
{
#ifdef USE_FOLLOWME_STEP2
    MenuCtrl_PushML2_P( ID_DEBUG_GPS  , MENU_ITEM, &Debug_GPS     , PKTGPS_de     , PKTGPS_en      );
#endif

    MenuCtrl_PushML2_P( ID_SEARCHMK  , MENU_ITEM, NOFUNC          , SEARCHMK_de   , SEARCHMK_en    );

    #ifdef USE_MAINMENU_SEPARATOR
    MenuCtrl_PushSeparator();   // MENUE TRENNLINIE
    #endif

    #ifdef USE_OSDDATA
    MenuCtrl_PushML2_P( ID_OSDDATA   , MENU_SUB , &Menu_OSDData   , OSDDATA_de    , OSDDATA_en     );
    #endif

    #ifdef USE_MAINMENU_SEPARATOR
    MenuCtrl_PushSeparator();   // MENUE TRENNLINIE
    #endif

  //MenuCtrl_PushML2_P( ID_PKTSPEZIAL, MENU_SUB , &Menu_PKTSpezial, PKTSPEZIAL_de , PKTSPEZIAL_en  );
    MenuCtrl_PushML2_P( ID_PKTCONNECT, MENU_SUB , &Menu_PKTConnect, PKTCONNECT_de , PKTCONNECT_en  );
    MenuCtrl_PushML2_P( ID_PKTSETUP  , MENU_SUB , NOFUNC          , PKTSETUP_de   , PKTSETUP_en    );
    MenuCtrl_PushML2_P( ID_PKTINFO   , MENU_ITEM, &PKT_Info       , PKTINFO_de    , PKTINFO_en     );


    #ifdef DEBUG_PARAMSET
    MenuCtrl_Push_P( 0  , MENU_ITEM, &paramsetDEBUG , DEBUG_PARAMSET_Text  );
    #endif
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void _ConfigMenu_Main( void )
{
    MenuCtrl_Create();

    MenuCtrl_SetTitle( titlemain );
    //MenuCtrl_SetShowBatt( true );

    //-----------
    // Tasten
    //-----------
    MenuCtrl_SetKey( KEY_ESC       , strGet(OFF), &PKT_SwitchOff );

    MenuCtrl_SetKey( KEY_ENTER_LONG , NOTEXT, &Menu_PKTConnect );

    if     ( MKVersion.isNC )   _DefMenu_Main_NC();
    else if( MKVersion.isFC )   _DefMenu_Main_FC();
    else                        _DefMenu_Main_NO();
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Menu_Main( void )
{
    uint8_t itemid;
    uint8_t UseBT = Config.UseBT;                       // merkt sich Bluetooth Einstellung falls durch Setup geaendert

    uart1_flush();
    ADC_Init();                                         // ADC für Lipomessung // MartinR hierher verschoben
    get_key_press(KEY_ALL);

    strncpy( &titlemain[5], PKTSWVersion, 12);          // baue Titel mit PKT-Versionsnummer zusammen

    MenuCtrl_ShowLevel( true );                         // zeige Menuelevel in der Titelzeile aller Menues
    _ConfigMenu_Main();                                 // initialisiert das Menu je nach Hardware (NO,FC,NC) verschieden


    //---------------
    // Control
    //---------------
    while( true )
    {
        MenuCtrl_Control( MENUCTRL_EVENT );             // wartet nicht auf Event, springt direkt zurueck (wegen evtl. BT-Aenderung)

        itemid = MenuCtrl_GetItemId();                  // welcher Menu-Punkt (ID)

        //--------------
        // ID_SEARCHMK
        //--------------
        if( itemid == ID_SEARCHMK )                     // neuer Hardware Modus?
        {
            if( searchMK(true) )                        // true = zeige MK_Info() an
            {
                MenuCtrl_Destroy();                     // Menu verwerfen...
                _ConfigMenu_Main();                     // ...und neues Menu initialisieren (je nach Hardware: NO,FC,NC)
            }
        }

        //--------------
        // ID_MKINFO
        //--------------
        if( itemid == ID_MKINFO )
        {
            if( MK_Info( 0, true) )                     // neuer Hardware Modus? (in MK_Info() kann man eine Neusuche des MK starten)  (true = mit Settings-Refresh)
            {
                MenuCtrl_Destroy();                     // Menu verwerfen...
                _ConfigMenu_Main();                     // ...und neues Menu initialisieren (je nach Hardware: NO,FC,NC)
            }
        }

        //--------------
        // ID_PKTSETUP
        //--------------
        if( itemid == ID_PKTSETUP )
        {
            Setup_MAIN();
            MenuCtrl_Destroy();                     // Menu verwerfen...
            _ConfigMenu_Main();                     // ...und neues Menu initialisieren (je nach Hardware: NO,FC,NC)

        }


        //--------------
        // Bluetooth geaendert?
        //--------------
        if( UseBT != Config.UseBT )                     // falls sich im PKT-Setup die Einstellung bzgl. installiertem Bluetooth Modul
        {                                               //   geaendert hat werden ggf. andere Menuepunkte angezeigt (Tracking, FollowMe)
            MenuCtrl_Destroy();                         // Menu verwerfen...
            _ConfigMenu_Main();                         // ...und neues Menu initialisieren (je nach Hardware: NO,FC,NC)
            UseBT = Config.UseBT;
        }
    }

    MenuCtrl_Destroy();
}



