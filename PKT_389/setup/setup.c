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
//# HISTORY  setup.c
//#
//# 17.11.2015 Cebra
//# - chg: Setup_Wi232: Änderungen der Wi232 Parameter wurden nicht ins Wi232 geschrieben
//#
//# 23.10.2015 Cebra
//# - add: LCD_Orientation, drehen des Displays wird wieder benötigt
//#
//# 14.10.2015 Starter
//# - FollowMe neu mit Offset
//# - enum Show_int3 fehlte für das Anzeigen von Setupwerten. Nun ergänzt
//# - Umbenennen der Pareameter Distance und Azimuth in Offset_lat und Offset_long
//#
//# 03.08.2015 CB
//# - add: FollowMe Setup um Distance und Azimuth erweitert
//#
//# 27.06.2014 OG
//# - chg: Setup_MAIN() - Reihenfolge von GPS-Maus/FollowMe/Tracking geaendert
//#
//# 25.06.2014 OG
//# - chg: Text von GPS_SHOWDEV
//# - chg: Setup_FollowMe() - deaktiviert/auskommentiert: FME_REFRESH
//# - chg: Setup_GPSMouse() - deaktiviert/auskommentiert: GPS_TYPE, GPS_ACTIVE
//#
//# 24.06.2014 OG
//# - chg: Setup_GPSMouse() angepasst auf geaendertes GPSMouse_ShowData()
//#
//# 22.06.2014 OG
//# - del: verschiedene weitere Modul-Variablen entfernt weil nicht benoetigt
//# - del: Variable CheckGPS
//# - del: BT_ShowGPSData() - ist jetzt als GPSMouse_ShowData() in gps/gpsmouse.c
//# - chg: BT_ShowGPSData() - Parameter und Rueckgabe
//# - del: BT_ShowGPSData_OLD
//#
//# 21.06.2014 CB
//# - chg: BT_SearchDevices - Timeoutanzeige bei Devicesuche
//#
//# 16.06.2014 OG
//# - chg: BT_ShowGPSData() - neues Layout und Anzeige fuer "XCnt" (=RX-Count)
//#        hinzugefuegt; XCnt zeigt die Anzahl empfangener GPS-Datenpakete an
//#        und ist ein Indikator wie schnell die BT-GPS Maus Daten sendet
//#
//# 13.06.2014 OG
//# - chg: Setup_PKTGeneric() - neue Einstellung "PKT aus nach"
//# - del: ChangeWi_SV2()
//#
//# 12.06.2014 OG
//# - del: Reset_EEprom() - ist jetzt als PKT_Reset_EEprom() in pkt.c
//#
//# 11.06.2014 OG
//# - fix: Edit_generic() funktioniert jetzt auch, wenn kein vorgelagertes
//#        Menue vorhanden ist (ggf. muss das Label angepasst werden)
//#
//# 10.06.2014 OG
//# - chg: verschiedene Funktionen umgestellt auf Wi232_Initalize()
//#
//# 08.06.2014 OG
//# - add: Setup_MKConnection() - hier ist auch das setzen der Baudrate Wi232/BT
//#        hin verschoben worden (war vorher im Wi.232 Bereicch)
//#
//# 06.06.2014 OG
//# - chg: Setup_WI232_MenuCreate()... Eintraege geloescht...
//#
//# 04.06.2014 OG
//# - chg: Setup_PKTGeneric() eine Menue-Trennlinie hinter Sommerzeit eingefuegt
//#
//# 01.06.2014 OG
//# - chg: Edit_LED_Form() Parameter 'Text' entfernt und auf strGetOSDOLD()
//#        umgestellt
//# - add: include "../osd/osdold_messages.h"
//#
//# 31.05.2014 OG
//# - chg: Edit_String() - Parameter 'Text' entfernt; angepasst auf neue
//#        Titelzeile; PKT_CtrlHook integriert; PKT Lipo-Anzeige;
//#        auf 'redraw' umgestellt
//#
//# 30.05.2014 OG
//# - chg: Edit_LipoOffset() - umgeschrieben auf neues Layout, Code optimiert,
//#        Increment jetzt +/-50 (statt vorher 20), Offset bis 15000 (statt 10000)
//# - chg: Edit_LipoOffset() - keine Parameter mehr, keine Rueckgabe
//#
//# 29.05.2014 OG
//# - chg: Setup_OSD() umgestellt auf Menuetexte statt strGet() mit Textaenderung
//#
//# 28.05.2014 OG
//# - chg: Setup's auf das neue Edit_generic() umgestellt
//# - chg: Edit_generic() - geaenderte Aufrufparameter
//#        (Text enfernt da vom Menue geerbt; optionale Hilfstexte moeglich)
//#
//# 27.05.2014 OG
//# - del: Edit_generic_int(), Edit_printout_int() - nicht mehr benoetigt da
//#        anhaengende Werte umgemappt auf Edit_generic()
//#
//# 26.05.2014 OG
//# - chg: Menuetext TIME_ZONE ergaenzt um "UTC"
//# - chg: Setup_MAIN() - neue Anordnung der Menueeintraege mit Trennlinien
//# - chg: Setup_OSDScreens() - OSDSCREEN_WAYPOINTS hinzugefuegt
//# - chg: Text "LCD aus nach..." nach "LCD aus nach"
//# - chg: Text "PKT allgemein" geaendert auf "PKT Allgemein"
//# - del: PKT_LCDINVERS inkl. Sprachen
//# - chg: Setup_PKTGeneric() - PKT_LCDINVERS (Config.LCD_DisplayMode) entfernt,
//#        nicht mehr unterstuetzt
//# - chg: Edit_printout() - Hilfstexte CHANGENORMREV1 und 2 fuer NormRev entfernt
//#
//# 23.05.2014 OG
//# - add: Edit_generic() - Anzeige Titel mit PKT_TitleFromMenuItem()
//# - fix: Setup_PKTGeneric() - PKT_LCDINVERS (Config.LCD_DisplayMode)
//#        Edit-Range umgestellt von 0,4 auf 0,1
//#
//# 11.05.2014 OG
//# - chg: die Setup-Menues umgestellt auf MenuCtrl_SetTitleFromParentItem()
//#        -> die Menues 'erben' damit ihren Titel vom aufrufenden Menuepunkt
//# - chg: ein paar Menuetexte fuer Setup_MAIN()
//#
//# 18.04.2014 OG
//# - chg: Setup_PKTGeneric() - PKT_BRIGHTNESS rausgenommen da es aktuell auch
//#        nicht mehr mit PKT 3.9m Hardware geht (spart ca. 650 Bytes)
//# - chg: Text von "Tracking Servos" gekuerzt auf "Tracking"
//#
//# 09.04.2014 OG
//# - chg: Edit_printout() Text von WlanMode 'Nein' auf 'Aus' geaendert
//#
//# 04.04.2014 OG
//# - chg: Edit_String() auf ShowTitle_P() umgestellt
//# - del: Edit_ShowTitle_P()
//# - chg: in versch. Funktionen Titel-Strings fuer Edit_String() gekuerzt
//# - chg: Edit_String() auf Edit_ShowTitle_P() umgestellt
//#
//# 02.04.2014 OG
//# - add: Edit_ShowTitle_P()
//# - add: Edit_printout(): WlanMode (Aus, AP-Mode, AdHoc)
//# - chg: Edit_printout(): Code-Optimierung
//#
//# 01.04.2014 OG
//# - add: Unterstuetzung fuer BLE (Bluetooth 4 Low Energy Zusatzmodul an SV2)
//# - chg: PCB_WiFlyPatch umbenannt zu PCB_SV2RxTxPatch
//#
//# 30.03.2014 OG
//# - chg: Sprache Hollaendisch vollstaendig entfernt
//# - chg: MenuCtrl_PushML_P() umgestellt auf MenuCtrl_PushML2_P()
//#
//# 27.03.2014 OG
//# - chg: Setup_PKTGeneric(), Edit_printout() Sprache "Niederlï¿½ndisch" nicht mehr waehlbar
//#
//# 04.02.2014 OG
//# - chg: kleine Layoutaenderung von 'PKT_Akku' in Edit_printout()
//# - add: PKT_CtrlHook() in Edit_generic(), Edit_generic_int(), Edit_LipoOffset() eingebaut
//# - add: kleiner Hilfetext in Edit_printout() bei Aenderung von Einstellung
//#        "LCD Normal/Invers" bzgl. PKT neu starten nach Aenderung
//# - chg: Anzeige von Edit_LipoOffset() geaendert und fix vom springen der
//#        Eingabe wenn Wert geaendert wird
//#
//# 03.02.2014 OG
//# - add: SHOWCELLU in Setup_OSD() ("zeige Zellensp.")
//#
//# 30.01.2014 OG
//# - add: Unterstuetzung fuer USE_BLUETOOTH
//#
//# 29.07.2013 Cebra
//# - chg: Fehler in Setup_WI232_MenuCreate, MenuCtrl_Push_P( WI_INSTALLED.. war auskommentiert, dadurch Fehler im Menu bei nicht
//#        aktivem Wi232
//#
//# 18.07.2013 Cebra
//# - chg: Edit_PKT_Accu entfernt, Edit_generic erweitert
//#
//# 15.07.2013 Cebra
//# - add: Edit_Printout um Wlan Security erweitert
//#
//# 07.07.2013 OG
//# - add: Setup_OSDScreens(), Mark_OSDScreen(), Edit_OSDScreen()
//#
//# 04.07.2013 Cebra
//# - chg: Setupmenü um Wlan erweitert
//#
//# 02.07.2013 Cebra
//# - chg: Edit_String, geändert für die Nutzung von SSID und Wlan Passwort
//#
//# 30.06.2013 OG
//# - del: Setup_OSD(): OSDS_HOMEVIEW
//#
//# 26.06.2013 OG
//# - del: USE_PKT_STARTINFO
//#
//# 24.06.2013 OG
//# - chg: vor dem editieren des Bluetooth Namens bt_fixname() eingefuegt
//#
//# 24.06.2013 OG
//# - fix: Edit_String(): Fehlermeldung bei EDIT_BT_NAME wenn erstes Zeichen = Space
//# - chg: Edit_String(): Code-Layout und diverses
//# - chg: Setup_BTM222(): Bearbeitung von BT_NAME geaendert
//# - chg: Setup_BTM222(): Menuetexte geaendert
//# - chg: Setup_MAIN(): Text im Setup-Hauptmenue von "BTM-222" auf "Bluetooth" geaendert
//#
//# 21.06.2013 OG
//# - fix: Menuetext GPSMouse "Zeige Geräteliste" (Anzeige "ä" korrigiert)
//#
//# 15.06.2013 OG
//# - add: "zeige MK-Setting" in Setup_OSD() hinzugefuegt
//#
//# 15.06.2013 OG
//# - chg: Umstrukturierungen am OSD-Setup
//# - chg: OSD Empfangsausfall von Setup_PKTGeneric() nach Setup_OSD() verschoben
//#
//# 13.06.2013 OG
//# - del: Setup_Time() - jetzt in Setup_PKTGeneric()
//# - del: Setup_PKTAccu() - jetzt in Setup_PKTGeneric()
//# - del: SETUP_PKTDEBUG in Setup_MAIN() entfernt (auskommentiert, wenn notwendig ggf. mit einem Debug define regeln)
//# - chg: diverse Aenderungen in Menutexten
//# - fix: keine LCD-Helligkeitseinstellung bei HW 3.9x mehr durch Abfrage von PCB_Version
//#
//# 31.05.2013 cebra
//# - chg: Umlaute, soweit gefunden, korrigiert
//#
//# 30.05.2013 cebra
//# - chg: doppelte Texte auf #define umgestellt
//#
//# 24.05.2013 OG
//# - chg: etliche Aenderungen rund um Bluetooth/BT-GPS Maus fuer bessere
//#        Usebility, Code-Einsparung und Code-Lesbarkeit
//# - add: Wait4KeyOK()
//# - chg: Aufrufe von MenuCtrl_Push() umgestellt auf MenuCtrl_PushML_P()
//#
//# 22.05.2013 OG
//# - chg: umgestellt auf menuctrl: BT_SelectDevice()
//#
//# 22.05.2013 OG
//# - chg: umgestellt auf menuctrl: Setup_Time(), Setup_FollowMe(), Setup_OSD()
//# - chg: weitere Speicheroptimierungen fuer USE_*
//# - chg: Menu_Setup() umbenannt zu Setup_MAIN()
//#
//# 20.05.2013 OG
//# - chg: umgestellt auf menuctrl: Menu_Setup(), Setup_PKTGeneric(), Setup_WI232()
//#        Setup_BTM222(), Setup_PKTAccu(), Setup_GPSMouse()
//# - add: GPSMouse_ShowData()
//#
//# 19.05.2013 OG
//# - chg: Aufruf von Update_PKT() umgestellt auf PKT_Update()
//#
//# 17.05.2013 OG
//# - chg: umgestellt auf utils/menuold.h
//#
//# 16.05.2013 Cebra
//# - chg: Fehler bei On/Off Settings
//#
//# 05.05.2013 Cebra
//# - add: PKT Zeitsetup
//#
//# 19.04.2013 Cebra
//# - chg: Fehlerbehandlung im GPS Menü für den Fall das kein BTM22 eingebaut oder GPS-Device konfiguriert
//#
//# 16.04.2013 Cebra
//# - chg: PROGMEM angepasst auf neue avr-libc und GCC, prog_char depreciated
//#
//# 12.04.2013 Cebra
//# - chg: edit_generic um Sprungfaktor erweitert, ermöglicht bei hohen Werten einen schnelleren Wertewechsel
//#
//# 03.04.2013 Cebra
//# - chg: find/replace Fehler bei lcd_puts_at beseitigt, GPS und BT-Maus Bereich
//#
//# 02.04.2013 Cebra
//# - chg: Textfehler bei der Einstellung "Verbindung zum MK" beseitigt
//#
//# 04.03.2013 Cebra
//# - del: OSD-Sreenmode ,is no longer necessary, last OSD-Screen is saved at shutdown
//#
//# 27.03.2013 Cebra
//# - chg: Fehler bei der Menï¿½steuerung behoben
//############################################################################
#include "../cpu.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <util/delay.h>
#include "../main.h"
#include "../setup/setup.h"
#include "../lcd/lcd.h"
#include "../timer/timer.h"
#include "../wi232/Wi232.h"
#include "../bluetooth/bluetooth.h"
#include "../connect.h"
#include "../lipo/lipo.h"
#include "../messages.h"
#include "../eeprom/eeprom.h"
#include "../tracking/tracking.h"
#include "../uart/uart1.h"
#include "../sound/pwmsine8bit.h"
#include "../tracking/servo_setup.h"
#include "../bluetooth/error.h"
#include "../stick/stick_setup.h"
#include "../utils/menuctrl.h"
#include "../utils/xutils.h"
#include "../pkt/pkt.h"
#include "../osd/osd.h"
#include "../wifly/wifly_setup.h"
#include "../gps/gpsmouse.h"
#include "../utils/scrollbox.h"
#include "../osd/osdold_messages.h"
#include "../menu.h"
#include "../mk/mkbase.h"

uint8_t edit = 0;
uint16_t Pre;
char EditString[21];
uint8_t length_tmp;
uint8_t Old_Baudrate;              //Merkzelle für alte Baudrate

//############################################################################
//############################################################################

//-----------------------------
// Setup_Main()
//-----------------------------
#define SETUP_MKCONNECTION  1
#define SETUP_PKTCONFIG     2
#define SETUP_WI232         3
#define SETUP_BTM222        4
#define SETUP_SERVOCONFIG   5
#define SETUP_OSDVIEW       6
#define SETUP_TIME          7
#define SETUP_GPSMAUS       8
#define SETUP_FOLLOWME      9
#define SETUP_JOYSTICK      10
#define SETUP_PKTAKKU       11
#define SETUP_PKTUPDATE     12
#define SETUP_PKTDEBUG      13
#define SETUP_EEPROMRESET   14
#define SETUP_WIFLY         15
#define SETUP_OSDSCREENS    16
#define SETUP_BLE           17

static const char SETUP_MKCONNECTION_de[] PROGMEM = "Verbindung MK";
static const char SETUP_MKCONNECTION_en[] PROGMEM = "connection MK";

static const char SETUP_PKTCONFIG_de[] PROGMEM = "PKT Allgemein";
static const char SETUP_PKTCONFIG_en[] PROGMEM = "PKT general";

static const char SETUP_WI232_de[] PROGMEM = "Wi.232";
#define           SETUP_WI232_en              SETUP_WI232_de

static const char SETUP_BTM222_de[] PROGMEM = "Bluetooth";
#define           SETUP_BTM222_en             SETUP_BTM222_de

static const char SETUP_BLE_de[] PROGMEM = "Bluetooth BLE";
#define           SETUP_BLE_en                SETUP_BLE_de

static const char SETUP_WIFLY_de[] PROGMEM = "WLAN WiFly";
#define           SETUP_WIFLY_en              SETUP_WIFLY_de

static const char SETUP_SERVOCONFIG_de[] PROGMEM = "Tracking";
#define           SETUP_SERVOCONFIG_en        SETUP_SERVOCONFIG_de

static const char SETUP_OSDVIEW_de[] PROGMEM = "OSD Anzeige";
static const char SETUP_OSDVIEW_en[] PROGMEM = "OSD display";

static const char SETUP_OSDSCREENS_de[] PROGMEM = "OSD Screens";
static const char SETUP_OSDSCREENS_en[] PROGMEM = "OSD screens";

static const char SETUP_GPSMAUS_de[] PROGMEM = "GPS Maus";
static const char SETUP_GPSMAUS_en[] PROGMEM = "GPS mouse";

static const char SETUP_FOLLOWME_de[] PROGMEM = "Follow Me";
#define           SETUP_FOLLOWME_en           SETUP_FOLLOWME_de

static const char SETUP_JOYSTICK_de[] PROGMEM = "Joystick";
#define           SETUP_JOYSTICK_en           SETUP_JOYSTICK_de

static const char SETUP_PKTUPDATE_de[] PROGMEM = "PKT Update";
#define           SETUP_PKTUPDATE_en          SETUP_PKTUPDATE_de

static const char SETUP_PKTDEBUG_de[] PROGMEM = "Debug PKT";
#define           SETUP_PKTDEBUG_en           SETUP_PKTDEBUG_de

static const char SETUP_EEPROMRESET_de[] PROGMEM = "PKT Reset";
#define           SETUP_EEPROMRESET_en        SETUP_EEPROMRESET_de

//############################################################################

//-----------------------------
// Setup_PKTGeneric() ("PKT allgemein")
//-----------------------------
#define PKT_LANGUAGE        3
#define PKT_LIGHTOFF        4
#define PKT_BRIGHTNESS      5
#define PKT_CONTRAST        6
#define PKT_LCDORIENTATION  7
#define PKT_SOUNDMODUL      9
#define PKT_BEEPER          10
#define PKT_VOLUME          11
#define PKT_ACCUTYPE        12
#define PKT_ACCUMEASURE     13
#define TIME_ZONE           14
#define TIME_SUMMER         15
#define PKT_PKTOFF          16

static const char LANGUAGE_de[] PROGMEM = "Sprache";
static const char LANGUAGE_en[] PROGMEM = "language";

static const char TIME_ZONE_de[] PROGMEM = "Zeitzone UTC";
static const char TIME_ZONE_en[] PROGMEM = "time zone UTC";

static const char TIME_SUMMER_de[] PROGMEM = "Sommerzeit";
static const char TIME_SUMMER_en[] PROGMEM = "summer time";

static const char PKT_PKTOFF_de[] PROGMEM = "PKT aus nach";
static const char PKT_PKTOFF_en[] PROGMEM = "PKT off after";

static const char LIGHTOFF_de[] PROGMEM = "LCD aus nach";
static const char LIGHTOFF_en[] PROGMEM = "LCD off after";

//static const char BRIGHTNESS_de[]       PROGMEM = "LCD Helligkeit";
//static const char BRIGHTNESS_en[]       PROGMEM = "LCD brightness";

static const char CONTRAST_de[] PROGMEM = "LCD Kontrast";
static const char CONTRAST_en[] PROGMEM = "LCD contrast";

static const char LCDORIENTATION_de[] PROGMEM = "LCD Orientation";
static const char LCDORIENTATION_en[] PROGMEM = "LCD orientation";

static const char BEEPER_de[] PROGMEM = "Hardware Beeper";
#define           BEEPER_en             BEEPER_de

#ifdef USE_SOUND
static const char SOUNDMODUL_de[] PROGMEM = "Soundmodul";
static const char SOUNDMODUL_en[] PROGMEM = "Soundmodule";

static const char VOLUME_de[] PROGMEM = "Lautstärke Sound";
static const char VOLUME_en[] PROGMEM = "Volume Sound";
#endif

static const char ACCUTYPE_de[] PROGMEM = "Akku Typ";
static const char ACCUTYPE_en[] PROGMEM = "Accu type";

static const char ACCUMEASURE_de[] PROGMEM = "Akku Messung";
static const char ACCUMEASURE_en[] PROGMEM = "Accu measure";

//############################################################################

//-----------------------------
// Setup_MKConnection()
//-----------------------------

#define MKCONN_CONNECTION   1
#define MKCONN_BAUDRATE     2

#define           MKCONN_CONNECTION_de      SETUP_MKCONNECTION_de
#define           MKCONN_CONNECTION_en      SETUP_MKCONNECTION_en

static const char MKCONN_BAUDRATE_de[] PROGMEM = "Baudrate Wi232/BT";
#define           MKCONN_BAUDRATE_en        MKCONN_BAUDRATE_de

//############################################################################

//-----------------------------
// Setup_WI232()
//-----------------------------
#define WI_INSTALLED    1
#define WI_TXRX         2
#define WI_GROUP        3
#define WI_MODE         4
#define WI_TIMEOUT      5
#define WI_MTU          6
#define WI_INIT         7
#define WI_PCCONFIG     8

static const char WI_INSTALLED_de[] PROGMEM = "Modul eingebaut";
static const char WI_INSTALLED_en[] PROGMEM = "module built in";

static const char WI_TXRX_de[] PROGMEM = "TX/RX Kanal";
static const char WI_TXRX_en[] PROGMEM = "TX/RX Channel";

static const char WI_GROUP_de[] PROGMEM = "NetW. Gruppe";
static const char WI_GROUP_en[] PROGMEM = "NetW. Group";

static const char WI_MODE_de[] PROGMEM = "NetW. Mode";
#define           WI_MODE_en            WI_MODE_de

static const char WI_TIMEOUT_de[] PROGMEM = "TX Timeout";
#define           WI_TIMEOUT_en         WI_TIMEOUT_de

static const char WI_MTU_de[] PROGMEM = "TX MTU";
#define           WI_MTU_en             WI_MTU_de

static const char WI_INIT_de[] PROGMEM = "Initialisieren";
static const char WI_INIT_en[] PROGMEM = "initialize";

static const char WI_PCCONFIG_de[] PROGMEM = "Konfig. mit PC";
static const char WI_PCCONFIG_en[] PROGMEM = "config. with PC";

//############################################################################

//-----------------------------
// Setup_BTM222() (Bluetooth)
//-----------------------------
#define BT_INSTALLED  1
#define BT_NAME       2
#define BT_PIN        3
#define BT_INIT       4
#define BT_MAC        5
#define BT_REID       6
#define BT_PCCONFIG   7
#define BT_SHOWCONFIG   8

#define           BT_INSTALLED_de       WI_INSTALLED_de
#define           BT_INSTALLED_en       WI_INSTALLED_en

static const char BT_NAME_de[] PROGMEM = "BT Name";
static const char BT_NAME_en[] PROGMEM = "BT name";

static const char BT_PIN_de[] PROGMEM = "BT Pin";
static const char BT_PIN_en[] PROGMEM = "BT pin";

#define           BT_INIT_de            WI_INIT_de
#define           BT_INIT_en            WI_INIT_en

static const char BT_MAC_de[] PROGMEM = "zeige BT MAC";
static const char BT_MAC_en[] PROGMEM = "show BT MAC";

static const char BT_REID_de[] PROGMEM = "RE-ID Notiz";
#define           BT_REID_en            BT_REID_de

#define           BT_PCCONFIG_de        WI_PCCONFIG_de
#define           BT_PCCONFIG_en        WI_PCCONFIG_en

static const char BT_SHOWCONFIG_de[] PROGMEM = "zeige BT Konfig";
static const char BT_SHOWCONFIG_en[] PROGMEM = "show BT config";

//############################################################################

//-----------------------------
// Setup_BLE() (Bluetooth 4 Low Energy)
//-----------------------------
#define BLE_INSTALLED  1

// Text ist in messages.c: MODULE_EXIST

//############################################################################

//-----------------------------
// Setup_GPSMouse()
//-----------------------------
#define GPS_DEVICES  1
#define GPS_SEARCH   2
#define GPS_TYPE     3
#define GPS_ACTIVE   4
#define GPS_SHOWDEV  5
#define GPS_DATA     6

static const char GPS_SEARCH_de[] PROGMEM = "suche GPS-Maus";
static const char GPS_SEARCH_en[] PROGMEM = "search GPS-mouse";

static const char GPS_DEVICES_de[] PROGMEM = "Geräteliste";
static const char GPS_DEVICES_en[] PROGMEM = "devicelist";

static const char GPS_SHOWDEV_de[] PROGMEM = "aktuelle Maus";
static const char GPS_SHOWDEV_en[] PROGMEM = "act. GPS-mouse";

static const char GPS_DATA_de[] PROGMEM = "GPS-Maus Daten";
static const char GPS_DATA_en[] PROGMEM = "GPS-mouse data";

static const char GPS_TYPE_de[] PROGMEM = "GPS-Maus Typ"; // aktuell nicht verwendet
static const char GPS_TYPE_en[] PROGMEM = "GPS-mouse typ";

static const char GPS_ACTIVE_de[] PROGMEM = "GPS-Maus aktiv";// aktuell nicht verwendet
static const char GPS_ACTIVE_en[] PROGMEM = "GPS-mouse activ";

//############################################################################

//-----------------------------
// Setup_FollowMe()
//-----------------------------
#define FME_SPEED       1
#define FME_RADIUS      2

#define FME_OFFSET_LAT  3
#define FME_OFFSET_LONG 4

// FOLLOW_ME Step 2
static const char FME_OFFSET_LATITUDE_de[] PROGMEM = "Offset Lat";
static const char FME_OFFSET_LATITUDE_en[] PROGMEM = "offset lat";
static const char FME_OFFSET_LONGITUDE_de[] PROGMEM = "Offset Long";
static const char FME_OFFSET_LONGITUDE_en[] PROGMEM = "offset long";

static const char FME_SPEED_de[] PROGMEM = "Speed";
#define           FME_SPEED_en                  FME_SPEED_de

static const char FME_RADIUS_de[] PROGMEM = "Toleranz Radius";
static const char FME_RADIUS_en[] PROGMEM = "tolerance radius";

//############################################################################

//-----------------------------
// Setup_OSD()
//-----------------------------
#define OSDS_SHOWCELLU      1
#define OSDS_LOWBAT         2
#define OSDS_RCERROR        3
#define OSDS_MKSETTING      4
#define OSDS_MAH            5
#define OSDS_DATASV2        6
#define OSDS_OUTFORMAT      7
#define OSDS_OUTPOLARITY    8
#define OSDS_FALLSPEED      9
#define OSDS_VARIOBEEP      10
#define OSDS_VOLTBAR        11

static const char OSDS_LOWBAT_de[] PROGMEM = "Warn Unterspannung";
static const char OSDS_LOWBAT_en[] PROGMEM = "Warn LowBat";

static const char OSDS_MAH_de[] PROGMEM = "Warn mAh";
static const char OSDS_MAH_en[] PROGMEM = "Warn mAh";

static const char OSDS_RCERROR_de[] PROGMEM = "Warn RC-Fehler";
static const char OSDS_RCERROR_en[] PROGMEM = "Warn RC-Error";

static const char OSDS_SHOWCELLU_de[] PROGMEM = "zeige Zellspannung";
static const char OSDS_SHOWCELLU_en[] PROGMEM = "show cell voltage";

static const char OSDS_MKSETTING_de[] PROGMEM = "zeige MK Setting";
static const char OSDS_MKSETTING_en[] PROGMEM = "show mk setting";

static const char OSDS_DATASV2_de[] PROGMEM = "Navi Daten an SV2";
static const char OSDS_DATASV2_en[] PROGMEM = "Navi data to SV2";

#ifdef USE_OSD_SCREEN_OLD
static const char OSDS_OUTFORMAT_de[] PROGMEM = "OUT1/2 Format";
static const char OSDS_OUTFORMAT_en[] PROGMEM = "OUT1/2 format";

static const char OSDS_FALLSPEED_de[] PROGMEM = "Max. Sinkrate m/s";
static const char OSDS_FALLSPEED_en[] PROGMEM = "max fallspeed m/s";

static const char OSDS_VARIOBEEP_de[] PROGMEM = "Variometer Beep";
static const char OSDS_VARIOBEEP_en[] PROGMEM = "Variometer beep";

static const char OSDS_OUTPOLARITY_de[] PROGMEM = "OUT1/2 Polaritï¿½t";
static const char OSDS_OUTPOLARITY_en[] PROGMEM = "OUT1/2 polarity";

static const char OSDS_VOLTBAR_de[] PROGMEM = "Volt Balken";
static const char OSDS_VOLTBAR_en[] PROGMEM = "volt bargraph";
#endif

//############################################################################

//############################################################################
//# Allgemeines
//############################################################################

//--------------------------------------------------------------
//--------------------------------------------------------------
void
Wait4KeyOK(void)
{
  set_beep(60, 0xffff, BeepNormal);
  lcd_printp_at(19, 7, strGet(OK), MNORMAL);
  while (!get_key_press(1 << KEY_ENTER))
    PKT_CtrlHook();   // PKT-Update usw...
  clear_key_all();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void
Wait4KeyENDE(uint8_t beep)
{
  if (beep)
    set_beep(60, 0xffff, BeepNormal);
  lcd_printp_at(12, 7, strGet(ENDE), MNORMAL);
  while (!get_key_press(1 << KEY_ESC))
    {
      show_Lipo();
      PKT_CtrlHook();   // PKT-Update usw...
    }
  clear_key_all();
}

//############################################################################
//# BT-Funktionen (GPS-Maus)
//############################################################################

#ifdef USE_BLUETOOTH

//--------------------------------------------------------------
// BT_CheckBTM222()
//
// prueft ob das Bluetooth Modul im PKT installiert ist
//--------------------------------------------------------------
uint8_t BT_CheckBTM222( void )
  {
    if( !Config.UseBT )
      {
        // PKT_Message_P( text, error, timeout, beep, clearscreen)
        PKT_Message_P( strGet(STR_NOBTM222), true, 1000, true, true);// 1000 = max. 10 Sekunden anzeigen; "kein BTM222 vorh."
        return false;// -> kein BTM222 eingebaut
      }

    return true;
  }

//--------------------------------------------------------------
// BT_SelectDevice()
//
// Auswahl eines BT-Device aus einer Liste von BT-Devices
//--------------------------------------------------------------
void BT_SelectDevice( void )
  {
    uint8_t itemid;
    uint8_t event;
    uint8_t i;

    //-------------------------------
    // BTM222 Modul vorhanden?
    //-------------------------------
    if( !BT_CheckBTM222() ) return;

    //-------------------------------
    // BT Device-Suche durchgefuert?
    //-------------------------------
    if( bt_devicecount == 0 )
      {
        // PKT_Message_P( text, error, timeout, beep, clearscreen)
        PKT_Message_P( PSTR("Suche durchführen!"), true, 1000, true, true);// 1000 = max. 10 Sekunden anzeigen; "kein BTM222 vorh."
        return;
      }

    //---------------
    // Create
    //---------------
    MenuCtrl_Create();
    MenuCtrl_SetTitle_P( strGet(STR_BT_DEVICES) );// "BT Gerï¿½te"

    //---------------
    // Menuitems
    //---------------
    for( i = 0; i < bt_devicecount; i++ )
      {
        MenuCtrl_Push( i, MENU_ITEM, NOFUNC, device_list[i].DevName );

        if( strncmp(device_list[i].mac,Config.gps_UsedMac,14) == 0 )
        MenuCtrl_ItemMark( i, true );           // aktuelles BT-Device markieren
      }

    //---------------
    // Control
    //---------------
    event = MenuCtrl_Control( MENUCTRL_EVENT );

    if( event == MENUEVENT_ITEM )
      {
        itemid = MenuCtrl_GetItemId();                // welcher Menu-Punkt (ID)
        strncpy( Config.gps_UsedMac , device_list[itemid].mac , 14);
        strncpy( Config.gps_UsedDevName, device_list[itemid].DevName, 20);
      }

    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();
  }

//--------------------------------------------------------------
// BT_SearchDevices()
//
// sucht BT-Devices
//--------------------------------------------------------------
void BT_SearchDevices( void )
  {
    uint8_t wahl;

    char *title;

    //-------------------
    // 1. Frage: suchen?
    //-------------------
    if( Config.gps_UsedDevName[0] != 0 )// ist eine BT GPS-Maus bereits vorhanden?
    title = Config.gps_UsedDevName;//   dann zeige im Titel den Namen der GPS-Maus an
    else
    title = "BT Devices";

    set_beep( 50, 0xffff, BeepNormal);

    //wahl = PKT_AskX( asktype, text1, text2, text_progmem, headline, headline_progmem, title, title_progmem)
    wahl = PKT_AskX( ASK_END_START, strGet(STR_SEARCH_BT_ASK), strGet(STR_BT_SEARCHTIME), true, MenuCtrl_GetItemText(), true, title, false);// "BT Gerï¿½te suchen?"

    //-------------------
    // 2. BT suchen
    //-------------------
    if( wahl )
      {
        if( BT_CheckBTM222() )
          {
            //PKT_Title_P( text, lShowLipo, clearscreen )
            PKT_Title_P( PSTR("BT Suche"), true, true );

            lcdx_printp_center( 2, strGet(STR_SEARCH_BT), MNORMAL, 0,1);// "suche BT Gerï¿½te"

            //PKT_Gauge_Begin( 0 );                                                       // Gauge: 0 = Default fuer y verwenden

            set_BTOn();
            bt_downlink_init();

            lcd_printp_at( 11, 7, strGet(KEYCANCEL), MNORMAL);// Keyline: "Abbr." bzw "Cancel"
            bt_searchDevice();

            set_BTOff();

            //PKT_Gauge_End();                                                            // Gauge: Ende

            if( bt_devicecount==0 )
              {
                //PKT_Message_P( text, error, timeout, beep, clearscreen)
                PKT_Message_P( strGet(STR_NO_BT_FOUND), true, 2000, true, true);
              }
            else
              {
                set_beep( 50, 0xffff, BeepNormal);
                BT_SelectDevice();
              }
          }
      }
  }

#endif   // end: #ifdef USE_BLUETOOTH

//############################################################################
//# Edit-Funktionen
//############################################################################

//--------------------------------------------------------------
// Ausgabe von Werten fuer Edit_generic()
//--------------------------------------------------------------
void
Edit_generic_printout(int16_t Value, int16_t min, int16_t max, uint8_t what,
    int8_t yoffs)
{
  const char *pStr = 0;   // PGM
  const char *pstr = 0;   // RAM
  //uint8_t x = 0;
  //uint8_t y = 3;

  switch (what)
    {
  case Show_uint3:
    pstr = buffered_sprintf_P(PSTR("%3d"), Value);
    break;

  case Show_uint5:
    pstr = buffered_sprintf_P(PSTR("%5d"), Value);
    break;

  case Show_int3:
    pstr = buffered_sprintf_P(PSTR("%3d"), Value);
    break;

  case Show_uint10th:
    pstr = buffered_sprintf_P(PSTR("%2.1d"), Value);
    break;

  case MK_Connection:
    switch (Value)
      {
    case 0:
      pStr = PSTR("Wi.232");
      break;
    case 1:
      pStr = strGet(KABEL);
      break;
      }

    if ((Value == 0) && (Config.UseWi == false))
      {
        lcd_rect_round(0, 37, 127, 21 - 6, 1, R2);                     // Rahmen
        lcdx_printp_center(6, strGet(STR_WI232_ACTIVATE), MNORMAL, 0, -7); // "aktiviere Wi.232!"
      }
    else
      {
        lcd_frect(0, 37, 127, 21 - 6, 0);               // Hilfebereich loeschen
      }
    break;

  case GPSMOUSE:
    switch (Value)
      {
    case GPS_Bluetoothmouse1:
      pStr = PSTR("BT-Mouse");
      break;
    case GPS_Mikrokopter:
      pStr = PSTR("Mikrokopter");
      break;
    default:
      pStr = PSTR("unknown");
      }
    break;

  case Wi_Netmode:
    switch (Value)
      {
    case false:
      pStr = strGet(SLAVE);
      break;
    case true:
      pStr = strGet(NORMAL);
      break;
      }
    break;

  case OnOff:
    switch (Value)
      {
    case 0:
      pStr = strGet(OFF);
      break;
    case 1:
      pStr = strGet(ON);
      break;
      }
    break;

  case YesNo:
    switch (Value)
      {
    case 0:
      pStr = strGet(NOO);
      break;
    case 1:
      pStr = strGet(YES);
      break;
      }
    break;

  case WlanMode:
    switch (Value)
      {
    case 0:
      pStr = strGet(OFF);
      break;
    case 1:
      pStr = PSTR("AP-Mode");
      break;
    case 2:
      pStr = PSTR("AdHoc");
      break;
      }
    break;

  case NormRev:
    switch (Value)
      // wird noch von stick/stick_setup.c verwendet
      {
    case 0:
      pStr = strGet(NORMAL);
      break;
    case 1:
      pStr = strGet(REVERSE);
      break;
      }
    break;

  case Kontrast:
    if (Value >= max)
      {
        Value = max;
        set_beep(200, 0x0080, BeepNormal);
      }

    if (Value == min)
      {
        Value = min;
        set_beep(200, 0x0080, BeepNormal);
      }

    lcd_set_contrast(Value);

    pstr = buffered_sprintf_P(PSTR("%3d"), Value);
    break;

  case Baudrate:
    pstr = buffered_sprintf_P(PSTR("%ld"), Baud_to_uint32(Value));
    break;

  case Language:
    switch (Value)
      {
    case 0:
      pStr = strGet(DEUTSCH);
      break;
    case 1:
      pStr = strGet(ENGLISCH);
      break;
      }
    break;

  case Sticktype:
    switch (Value)
      {
    case false:
      pStr = strGet(POTI);
      break;
    case true:
      pStr = strGet(TASTER);
      break;
      }
    break;

    /*
     case WlanSecurity:      // Anmerkung 02.04.2014 OG: wird aktuell nicht benoetigt weil anders geloest
     switch (Value)
     {
     case    0x0 :   lcd_printp_at(15, 2, PSTR("Adhoc"), 0);
     break;
     case    0x1 :   lcd_printp_at(15, 2, PSTR("WPA  "), 0);
     break;
     }
     break;
     */

  case PKT_Akku:
    switch (Value)
      {
    case true:
      pStr = PSTR("Lipo");
      break;
    case false:
      pStr = PSTR("LiIon");
      break;
      }
    break;
    } // end: switch( what )

  //lcdx_printf_at_P( uint8_t x, uint8_t y, uint8_t mode, int8_t xoffs, int8_t yoffs, const char *format, ... )

  if (pStr)
    lcdx_printf_at_P(0, 3, MNORMAL, 0, yoffs, PSTR("> %16S"), pStr);   // PGM
  if (pstr)
    lcdx_printf_at_P(0, 3, MNORMAL, 0, yoffs, PSTR("> %16s"), pstr);   // RAM
}

//--------------------------------------------------------------
// int16_t Edit_generic( Value, min, max, what, Addfactor, strHelp1, strHelp2)
//
// generische Funktion zum aendern von Setup-Werten
//
// PARAMETER:
//   Value: Ausgangswert der zu aendern ist
//   min  : minimaler Wert
//   max  : maximaler Wert
//   what : Typ des Wertes, abhaengig davon wird der Wert dargestellt, ENUM dafuer in setup.h
//   Addfactor : Sprungwert der beim erhï¿½hen und erniedrigen addiert wird
//   strHelp1 : Hilefetext 1 (PGM)
//   strHelp2 : Hilefetext 2 (PGM)
//S
// RUECKGABE:
//   int16_t Return    : Ergebnis der Veraenderung des Wertes
//--------------------------------------------------------------
int16_t
Edit_generic(int16_t Value, int16_t min, int16_t max, uint8_t what,
    uint8_t Addfactor, const char *strHelp1, const char *strHelp2)
{
  uint8_t redraw = true;
  uint8_t noCancel = false;
  int8_t yoffs = 0;     // yoffs fuer das Edit-Label
  int8_t yoffsV = 4;     // yoffs fuer den Eingabebereich
  const char *pStr;

  Pre = Value;

  do
    {
      //------------------------
      // Screen neu zeichnen
      //------------------------
      if (redraw)
        {
          PKT_TitleFromMenuItem(true); // Titel mit PKT-LiPo Anzeige und clearcsreen

          //------------------------
          // Platz reservieren fuer
          // spezielle Typen
          //------------------------
          if (what == MK_Connection)
            {
              yoffs = -2;                           // yoffs fuer das Edit-Label
              yoffsV = 0;                       // yoffs fuer den Eingabebereich
            }

          //------------------------
          // 1 zeiliger Hilfstext
          //------------------------
          if (strHelp1 != NULL && strHelp2 == NULL)
            {
              lcd_rect_round(0, 37, 127, 21 - 6, 1, R2);               // Rahmen
              lcdx_printp_center(6, strHelp1, MNORMAL, 0, -7);   // Hilfe-Text 1
              yoffs = -2;                           // yoffs fuer das Edit-Label
              yoffsV = 0;                       // yoffs fuer den Eingabebereich
            }

          //------------------------
          // 2 zeiliger Hilfstext
          //------------------------
          if (strHelp2 != NULL)
            {
              lcd_rect_round(0, 32, 127, 21, 1, R2);                 // Rahmen
              lcdx_printp_center(5, strHelp1, MNORMAL, 0, -5);   // Hilfe-Text 1
              lcdx_printp_center(6, strHelp2, MNORMAL, 0, -5);   // Hilfe-Text 2
              yoffs = -4;                           // yoffs fuer das Edit-Label
              yoffsV = -2;                      // yoffs fuer den Eingabebereich
            }

          //------------------------
          // Label
          //------------------------
          pStr = PSTR("???");
          if (MenuCtrl_GetMenuIndex() >= 0)
            pStr = MenuCtrl_GetItemText(); // das Label wird vom Menueeintrag uebernommen (Menuetext muss im PGM sein!)
          else
            {
              if (what == Language) // Ausnahme: wird ggf. beim Start vom PKT von HAL_HW3_9.c aufgerufen
                {
                  pStr = strGetLanguage(LANGUAGE_de, LANGUAGE_en);
                  noCancel = true;
                }
            }
          lcdx_printf_at_P(0, 2, MNORMAL, 0, yoffs, PSTR("%S:"), pStr); // Ausgabe des Labels

          lcd_printp_at(0, 7, strGet(KEYLINE5), MNORMAL); // Keyline:  <-   ->   Abbr.   OK
          if (noCancel)
            lcd_printp_at(11, 7, PSTR("      "), MNORMAL); // Keyline: "Abbr." loeschen wenn noCancel

          Edit_generic_printout(Value, min, max, what, yoffsV); // aktuellen Eingabewert anzeigen
          redraw = false;
        }

      //------------------------
      // PKT CtrlHook
      //------------------------
      if (PKT_CtrlHook())
        {
          redraw = true;
        }

      //------------------------
      // Key: PLUS
      //------------------------
      if ((get_key_press(1 << KEY_PLUS)
          || get_key_long_rpt_sp((1 << KEY_PLUS), 2)))
        {
          if (Value <= (max - Addfactor))
            Value = Value + Addfactor;
          else
            Value = min;

          Edit_generic_printout(Value, min, max, what, yoffsV);
        }

      //------------------------
      // Key: MINUS
      //------------------------
      if ((get_key_press(1 << KEY_MINUS)
          || get_key_long_rpt_sp((1 << KEY_MINUS), 2)))
        {
          if (Value >= (min + Addfactor))
            Value = Value - Addfactor;
          else
            Value = max;

          Edit_generic_printout(Value, min, max, what, yoffsV);
        }

      //------------------------
      // Key: ENTER
      //------------------------
      if (get_key_press(1 << KEY_ENTER))
        {
          return Value;
        }

      //------------------------
      // Key: ESC
      //------------------------
      if (!noCancel && get_key_press(1 << KEY_ESC))      // Ende ohne speichern
        {
          break;
        }
    }
  while (true);

  get_key_press (KEY_ALL);
  return Pre;
}

//--------------------------------------------------------------
// bei HW 3.9x geht das nicht mehr wegen dem Platinenlayout
// bei HW 3.9m geht es theoretisch noch - aber durch einen Bug scheinbar
//             nicht mehr. Kann nur von jemanden mit HW 3.9m repariert werden.
// ALSO -> raus und Speicher sparen
//--------------------------------------------------------------
/*
 uint8_t Edit_DisplayHelligkeit(uint8_t Value, uint8_t min, uint8_t max)
 {
 float ValCorr = 2.55;  // (Value * ValCorr) maximal 255
 Pre = Value;
 OCR2A = Value * ValCorr;

 lcd_cls();
 //  lcd_printp_at (0, 2, Text, 0);
 lcdx_printf_at_P( 0, 2, MNORMAL, 0,0, PSTR("%S:"), MenuCtrl_GetItemText() );    // Menuetext muss im PGM sein! (aktuell keine Unterscheidung RAM/PGM)
 write_ndigit_number_u (16, 2, Value, 3, 0,0);
 lcd_printp_at (17, 2, PSTR("%"), 0);
 //  lcd_printp_at (0, 7, PSTR(KEY_LINE_2), 0);
 lcd_printp_at(0, 7, strGet(KEYLINE2), 0);

 do
 {
 write_ndigit_number_u (16, 2,Value, 3, 0,0);
 lcd_frect ((8*0), (8*4), (Value * (16*8) / 100), 6, 1);

 if ((get_key_press (1 << KEY_PLUS) || get_key_long_rpt_sp ((1 << KEY_PLUS), 3)) && (Value < max))
 {
 Value++;

 if (Value >= max)
 {
 Value = max;
 set_beep ( 200, 0x0080, BeepNormal);
 }
 else
 OCR2A = Value * ValCorr;

 }

 if ((get_key_press (1 << KEY_MINUS) || get_key_long_rpt_sp ((1 << KEY_MINUS), 3)) && (Value > min))
 {
 lcd_frect (((Value - 1) * (16*8) / 100), (8*4), (16*8), 6, 0);
 Value--;

 if (Value == min)
 {
 Value = min;
 set_beep ( 200, 0x0080, BeepNormal);
 }
 else
 OCR2A = Value * ValCorr;

 }

 if (get_key_press (1 << KEY_ENTER))
 {
 OCR2A = Value * ValCorr;
 Config.LCD_Helligkeit = Value;
 //          WriteParameter();
 edit = 0;
 return Value;
 }
 }

 while (!get_key_press (1 << KEY_ESC));
 {
 get_key_press(KEY_ALL);
 OCR2A = Pre * ValCorr;
 Config.LCD_Helligkeit = Pre;
 //      WriteParameter();
 return Pre;
 }
 }
 */

//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t
Edit_String(const char *data, const uint8_t length, uint8_t type)
{
  uint8_t redraw = true;
  uint8_t x = 1;
  uint8_t I = 0;
  uint8_t lOk = false;
  uint8_t i;

  for (i = 0; i < length; i++)
    {
      EditString[i] = data[i];
    }

  do
    {
      //-------------------------
      // Screen zeichnen
      //-------------------------
      if (redraw)
        {
          PKT_TitleFromMenuItem(true);      // true = showlipo (mit clearscreen)

          for (i = 0; i < length; i++)
            {
              lcd_putc((i * 2) + 1, 3, EditString[i], MNORMAL);
              lcd_printp_at((i * 2) + 2, 3, PSTR(" "), MNORMAL);
            }

          lcd_rect((x * 6) - 3, (8 * 3) - 2, 10, 10, 1);
          lcd_printp_at(13, 6, PSTR("C"), MNORMAL);
          lcd_printp_at(0, 7, PSTR(" \x17     \x16     \x19     OK"), MNORMAL); // Keyline

          redraw = false;
        }

      //-------------------------
      // PKT-LiPo anzeigen
      //-------------------------
      show_Lipo();

      //-------------------------
      // PKT Update-Anforderung?
      //-------------------------
      if (PKT_CtrlHook())
        {
          redraw = true;
        }

      if ((type == EDIT_BT_NAME) || (type == EDIT_SSID)
          || (type == EDIT_WL_PASSWORD))  // Name
        {
          if ((get_key_press(1 << KEY_PLUS)
              || get_key_long_rpt_sp((1 << KEY_PLUS), 2))
              && EditString[I] < 'z')
            {
              EditString[I]++;

              if (EditString[I] >= 0x00 && EditString[I] < ' ')
                EditString[I] = ' ';

              if (EditString[I] > ' ' && EditString[I] < '0')
                EditString[I] = '0';

              if (EditString[I] > '9' && EditString[I] < 'A')
                EditString[I] = 'A';

              if (EditString[I] > 'Z' && EditString[I] < 'a')
                EditString[I] = 'a';

              lcd_putc(x, 3, EditString[I], 0);
              edit = 1;
            }

          if ((get_key_press(1 << KEY_MINUS)
              || get_key_long_rpt_sp((1 << KEY_MINUS), 2))
              && EditString[I] > ' ')
            {
              EditString[I]--;

              if (EditString[I] < 'a' && EditString[I] > 'Z')
                EditString[I] = 'Z';

              if (EditString[I] < 'A' && EditString[I] > '9')
                EditString[I] = '9';

              if (EditString[I] < '0' && EditString[I] > ' ')
                EditString[I] = ' ';

              lcd_putc(x, 3, EditString[I], 0);
              edit = 1;
            }
        }
      else if (type == EDIT_BT_PIN)  // PIN
        {
          if ((get_key_press(1 << KEY_PLUS)
              || get_key_long_rpt_sp((1 << KEY_PLUS), 1))
              && (EditString[I] < '9'))
            {
              EditString[I]++;

              if (EditString[I] >= 0x00 && EditString[I] < ' ')
                EditString[I] = ' ';

              if (EditString[I] > ' ' && EditString[I] < '0')
                EditString[I] = '0';

              lcd_putc(x, 3, EditString[I], 0);
              edit = 1;
            }

          if ((get_key_press(1 << KEY_MINUS)
              || get_key_long_rpt_sp((1 << KEY_MINUS), 1))
              && (EditString[I] > '0'))
            {
              EditString[I]--;

              if (EditString[I] < 'A' && EditString[I] > '9')
                EditString[I] = '9';

              lcd_putc(x, 3, EditString[I], 0);
              edit = 1;
            }
        }

      if (get_key_long(1 << KEY_ESC))
        {
          if (type == EDIT_BT_NAME || type == EDIT_SSID
              || type == EDIT_WL_PASSWORD)
            EditString[I] = ' ';  // Zeichen loeschen
          if (type == EDIT_BT_PIN)
            EditString[I] = '0';  // Zeichen setzen
          lcd_putc(x, 3, EditString[I], 0);
          edit = 1;
        }

      if (get_key_short(1 << KEY_ESC))
        {
          if (type == EDIT_BT_NAME)
            length_tmp = bt_name_length;
          if (type == EDIT_BT_PIN)
            length_tmp = bt_pin_length;
          if (type == EDIT_SSID)
            length_tmp = wlan_ssid_length;
          if (type == EDIT_WL_PASSWORD)
            length_tmp = wlan_password_length;

          if ((x / 2) + 2 > length_tmp)
            {
              lcd_rect((x * 6) - 3, (8 * 3) - 2, 10, 10, 0);
              x = 1;
              lcd_rect((x * 6) - 3, (8 * 3) - 2, 10, 10, 1);
              I = 0;
            }
          else
            {
              lcd_rect((x * 6) - 3, (8 * 3) - 2, 10, 10, 0);
              x++;
              x++;
              lcd_rect((x * 6) - 3, (8 * 3) - 2, 10, 10, 1);
              I++;  //Zeiger auf Zeichen
            }
        }

      if (get_key_press(1 << KEY_ENTER))
        {
          lOk = true;
          if (type == EDIT_BT_NAME && EditString[0] == ' ') // BT-Name: 1. Zeichen darf nicht Space sein
            {
              lcdx_printp_at(0, 5, PSTR(" FEHLER! 1. Zeichen! "), MNORMAL, 0,
                  -4);
              set_beep(300, 0xf00f, BeepNormal);
              _delay_ms(2000);
              lcd_frect(0, 5 * 8 - 4, 127, 7, 0);          // loesche Fehlertext

              lcd_rect((x * 6) - 3, (8 * 3) - 2, 10, 10, 0); // setze Cursor auf Position 1
              x = 1;
              lcd_rect((x * 6) - 3, (8 * 3) - 2, 10, 10, 1);
              I = 0;

              get_key_press (KEY_ALL);
              lOk = false;
            }
        }

    }
  while (!lOk);

  return 1;
}

//--------------------------------------------------------------
// min,max sind in Setup_PKTGeneric() festgelegt
//--------------------------------------------------------------
void
Edit_LipoOffset(void)
{
  uint8_t redraw = true;
  uint8_t inc = 50;           // in diesen Schritten hoch/runter zaehlen
  uint16_t min = 0;            // min. Offset Wert
  uint16_t max = 15000;        // max. Offset Wert
  uint16_t oldValue;

  if (Config.Lipo_UOffset % inc) // ggf. wurde frueher ein anderer Wert fuer das Increment 'inc' verwendet
    Config.Lipo_UOffset = (uint16_t)(Config.Lipo_UOffset / inc) * inc; // hier wird der Wert auf das aktuelle 'inc' angepasst

  oldValue = Config.Lipo_UOffset;

  do
    {
      //-------------------------
      // Screen zeichnen
      //-------------------------
      if (redraw)
        {
          PKT_TitleFromMenuItem(true);      // true = showlipo (mit clearscreen)

          lcdx_printf_at_P(0, 2, MNORMAL, 0, -4, PSTR("%S:"),
              MenuCtrl_GetItemText()); // Menuetext muss im PGM sein! (aktuell keine Unterscheidung RAM/PGM)

          lcd_rect_round(0, 32, 127, 21, 1, R2);                       // Rahmen
          lcdx_printp_at(1, 5, strGet(STR_HELP_LIPOOFFSET1), MNORMAL, 0, -5); // Hilfe-Text 1
          lcdx_printp_at(1, 6, strGet(STR_HELP_LIPOOFFSET2), MNORMAL, 0, -5); // Hilfe-Text 2
          lcd_printp_at(0, 7, strGet(KEYLINE5), MNORMAL);             // Keyline
          redraw = false;
        }

      //-------------------------
      // Wert ausgeben
      //-------------------------
      lcdx_printf_at_P(0, 3, MNORMAL, 0, -2, PSTR("> %5d      => %1.2dv"),
          Config.Lipo_UOffset, volt_avg);
      show_Lipo();

      //-------------------------
      // PKT Update-Anforderung?
      //-------------------------
      if (PKT_CtrlHook())
        {
          redraw = true;
        }

      //-------------------------
      // Tasten
      //-------------------------
      if ((get_key_press(1 << KEY_PLUS)
          || get_key_long_rpt_sp((1 << KEY_PLUS), 2))
          && (Config.Lipo_UOffset < max))
        {
          Config.Lipo_UOffset = Config.Lipo_UOffset + inc;
        }

      if ((get_key_press(1 << KEY_MINUS)
          || get_key_long_rpt_sp((1 << KEY_MINUS), 2))
          && (Config.Lipo_UOffset > min))
        {
          Config.Lipo_UOffset = Config.Lipo_UOffset - inc;
        }

      if (get_key_press(1 << KEY_ENTER))
        {
          clear_key_all();
          return;
        }

    }
  while (!get_key_press(1 << KEY_ESC));

  clear_key_all();

  Config.Lipo_UOffset = oldValue;   // Abbruch, orginalen Wert wieder herstellen
  return;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
#ifdef USE_OSD_SCREEN_OLD
uint8_t Edit_LED_Form (uint8_t Value, uint8_t min, uint8_t max )
  {
    Pre = Value;
    lcd_cls();

    lcd_printp_at(0, 2, strGetOSDOLD(OSD_LED_Form), MNORMAL);

    switch (Value)
      {
        case 0x1:
        lcd_circle (14 * 6 + 5, 2 * 8 + 3, 3, 1);   // kreis

        lcd_fcircle (16 * 6 + 5, 2 * 8 + 3, 3, 0);// lï¿½schen
        lcd_circle (16 * 6 + 5, 2 * 8 + 3, 3, 1);// kreis
        lcd_fcircle (16 * 6 + 5, 2 * 8 + 3, 1, 1);// plus
        break;
        case 0x3 :
        lcd_circle (14 * 6 + 5, 2 * 8 + 3, 3, 1);// kreis

        lcd_fcircle (16 * 6 + 5, 2 * 8 + 3, 3, 1);// schwarz
        break;
        break;
      }

    lcd_printp_at(0, 7, strGet(KEYLINE2), 0);

    do
      {
        if ((get_key_press (1 << KEY_PLUS)) && (Value == 1))
          {
            Value = 3;
            lcd_circle (14 * 6 + 5, 2 * 8 + 3, 3, 1);   // kreis

            lcd_fcircle (16 * 6 + 5, 2 * 8 + 3, 3, 1);// schwarz
          }

        if ((get_key_press (1 << KEY_MINUS)) && (Value == 3))
          {
            Value = 1;
            lcd_circle (14 * 6 + 5, 2 * 8 + 3, 3, 1);   // kreis

            lcd_fcircle (16 * 6 + 5, 2 * 8 + 3, 3, 0);// lï¿½schen
            lcd_circle (16 * 6 + 5, 2 * 8 + 3, 3, 1);// kreis
            lcd_fcircle (16 * 6 + 5, 2 * 8 + 3, 1, 1);// plus
          }

        if (get_key_press (1 << KEY_ENTER))
          {
            edit = 1;
            Config.OSD_LEDform = Value;
//          WriteParameter();
            edit = 0;
            return Value;
          }
      }

    while (!get_key_press (1 << KEY_ESC));
      {
        get_key_press(KEY_ALL);
        edit = 0;
        Config.OSD_LEDform = Pre;
        return Pre;
      }
  }
#endif // USE_OSD_SCREEN_OLD

//--------------------------------------------------------------
// Show_MAC( progmem, headline, mac)
//--------------------------------------------------------------
void
Show_MAC(uint8_t progmem, const char *headline, const char *mac)
{
  uint8_t i;
  uint8_t z;

  lcd_cls();
  PKT_TitleFromMenuItem(true);

  if (progmem)
    lcdx_printp_center(2, headline, MNORMAL, 0, 2);
  else
    lcdx_print_center(2, (uint8_t *) headline, MNORMAL, 0, 2);

  z = 0;
  for (i = 0; i < 13; i++)
    {
      lcdx_putc(z + 2, 5, mac[i], MNORMAL, 0, -1);
      if ((i % 2 == 1) && (i < 11))
        {
          z++;
          lcdx_printp_at(z + 2, 5, PSTR(":"), MNORMAL, 0, -1);
        }
      z++;
    }
  lcd_rect_round(0, 34, 127, 16, 1, R2);               // Rahmen
  Wait4KeyENDE(false);
}

//--------------------------------------------------------------
// Setup_OSD()
//--------------------------------------------------------------
void
Setup_OSD(void)
{
  uint8_t itemid;

  //---------------
  // Create
  //---------------
  MenuCtrl_Create();
  MenuCtrl_SetTitleFromParentItem();      // "OSD Anzeige"

  //---------------
  // Menuitems
  //---------------
  MenuCtrl_PushML2_P( OSDS_LOWBAT, MENU_ITEM, NOFUNC, OSDS_LOWBAT_de, OSDS_LOWBAT_en);
  //MenuCtrl_PushML2_P( OSDS_MAH          , MENU_ITEM, NOFUNC , OSDS_MAH_de         , OSDS_MAH_en         );
  MenuCtrl_PushML2_P( OSDS_RCERROR, MENU_ITEM, NOFUNC, OSDS_RCERROR_de, OSDS_RCERROR_en);

  MenuCtrl_PushSeparator();       // MENUE TRENNLINIE

  MenuCtrl_PushML2_P( OSDS_SHOWCELLU, MENU_ITEM, NOFUNC, OSDS_SHOWCELLU_de, OSDS_SHOWCELLU_en);
  MenuCtrl_PushML2_P( OSDS_MKSETTING, MENU_ITEM, NOFUNC, OSDS_MKSETTING_de, OSDS_MKSETTING_en);

#ifdef USE_OSD_SCREEN_OLD
  MenuCtrl_PushSeparator();   // MENUE TRENNLINIE
  MenuCtrl_PushML2_P( OSDS_FALLSPEED , MENU_ITEM, NOFUNC , OSDS_FALLSPEED_de , OSDS_FALLSPEED_en );
  MenuCtrl_PushML2_P( OSDS_VARIOBEEP , MENU_ITEM, NOFUNC , OSDS_VARIOBEEP_de , OSDS_VARIOBEEP_en );
  MenuCtrl_PushML2_P( OSDS_VOLTBAR , MENU_ITEM, NOFUNC , OSDS_VOLTBAR_de , OSDS_VOLTBAR_en );
  MenuCtrl_PushML2_P( OSDS_OUTFORMAT , MENU_ITEM, NOFUNC , OSDS_OUTFORMAT_de , OSDS_OUTFORMAT_en );
  MenuCtrl_PushML2_P( OSDS_OUTPOLARITY , MENU_ITEM, NOFUNC , OSDS_OUTPOLARITY_de , OSDS_OUTPOLARITY_en );
#endif

  MenuCtrl_PushSeparator();       // MENUE TRENNLINIE

  MenuCtrl_PushML2_P( OSDS_DATASV2, MENU_ITEM, NOFUNC, OSDS_DATASV2_de, OSDS_DATASV2_en);

  //---------------
  // Control
  //---------------
  while (true)
    {
      MenuCtrl_Control (MENUCTRL_EVENT);

      if (MenuCtrl_GetKey() == KEY_ESC)
        break;      // ENDE

      itemid = MenuCtrl_GetItemId();                  // welcher Menu-Punkt (ID)

      if (itemid == OSDS_LOWBAT)
        {
          Config.MK_LowBat = Edit_generic(Config.MK_LowBat, 32, 255, Show_uint10th, 1, strGet(STR_HELP_LOWBAT1), strGet(STR_HELP_LOWBAT2));
        }
      if (itemid == OSDS_SHOWCELLU)
        {
          Config.OSD_ShowCellU = Edit_generic(Config.OSD_ShowCellU, 0, 1, YesNo, 1, NULL, NULL);
        }
      if (itemid == OSDS_MAH)
        {
          Config.OSD_mAh_Warning = Edit_generic(Config.OSD_mAh_Warning, 0, 30000, Show_uint5, 100, NULL, NULL);
        }
      if (itemid == OSDS_RCERROR)
        {
          Config.OSD_RCErrorbeep = Edit_generic(Config.OSD_RCErrorbeep, 0, 1, OnOff, 1, NULL, NULL);
        }
      if (itemid == OSDS_MKSETTING)
        {
          Config.OSD_ShowMKSetting = Edit_generic(Config.OSD_ShowMKSetting, 0, 1, YesNo, 1, NULL, NULL);
        }
      if (itemid == OSDS_DATASV2)
        {
          Config.OSD_SendOSD = Edit_generic(Config.OSD_SendOSD, 0, 1, YesNo, 1, NULL, NULL);
        }

#ifdef USE_OSD_SCREEN_OLD
      if( itemid == OSDS_FALLSPEED )
        { Config.OSD_Fallspeed = Edit_generic( Config.OSD_Fallspeed , 0,247, Show_uint10th,1 ,NULL,NULL);}
      if( itemid == OSDS_VARIOBEEP )
        { Config.OSD_VarioBeep = Edit_generic( Config.OSD_VarioBeep , 0, 1, YesNo,1 ,NULL,NULL);}
      if( itemid == OSDS_OUTFORMAT )
        { Config.OSD_LEDform = Edit_LED_Form(Config.OSD_LEDform , 1, 3 );}
      if( itemid == OSDS_OUTPOLARITY )
        { Config.OSD_InvertOut = Edit_generic( Config.OSD_InvertOut , 0, 1, YesNo,1 ,NULL,NULL);}
      if( itemid == OSDS_VOLTBAR )
        { Config.OSD_LipoBar = Edit_generic( Config.OSD_LipoBar , 0, 1, YesNo,1 ,NULL,NULL);}
#endif
    }

  //---------------
  // Destroy
  //---------------
  MenuCtrl_Destroy();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void
Mark_OSDScreen(uint8_t osdscreenid)
{
  MenuCtrl_ItemMarkR(osdscreenid,
      (Config.OSD_UseScreen & (1 << osdscreenid)) != 0);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void
Edit_OSDScreen(uint8_t osdscreenid)
{
  if ((Config.OSD_UseScreen & (1 << osdscreenid)) == 0)
    Config.OSD_UseScreen = (Config.OSD_UseScreen | (1 << osdscreenid));
  else
    Config.OSD_UseScreen = (Config.OSD_UseScreen ^ (1 << osdscreenid));

  Mark_OSDScreen(osdscreenid);
}

//--------------------------------------------------------------
// Setup_OSDScreens()
//--------------------------------------------------------------
void
Setup_OSDScreens(void)
{
  uint8_t itemid;

  //---------------
  // Create
  //---------------
  MenuCtrl_Create();
  MenuCtrl_SetTitleFromParentItem();      // "OSD Screens"

  //---------------
  // Menuitems
  //---------------
#ifdef USE_OSD_SCREEN_NAVIGATION
  MenuCtrl_Push_P( OSDSCREEN_NAVIGATION, MENU_ITEM, NOFUNC, strGet(STR_OSDSCREEN_NAVIGATION) ); Mark_OSDScreen( OSDSCREEN_NAVIGATION );
#endif

#ifdef USE_OSD_SCREEN_WAYPOINTS
  MenuCtrl_Push_P( OSDSCREEN_WAYPOINTS , MENU_ITEM, NOFUNC, strGet(STR_OSDSCREEN_WAYPOINTS) ); Mark_OSDScreen( OSDSCREEN_WAYPOINTS );
#endif

#ifdef USE_OSD_SCREEN_ELECTRIC
  MenuCtrl_Push_P( OSDSCREEN_ELECTRIC , MENU_ITEM, NOFUNC, strGet(STR_OSDSCREEN_ELECTRIC) ); Mark_OSDScreen( OSDSCREEN_ELECTRIC );
#endif

#ifdef USE_OSD_SCREEN_ELECTRIC_N
  MenuCtrl_Push_P( OSDSCREEN_ELECTRIC , MENU_ITEM, NOFUNC, strGet(STR_OSDSCREEN_ELECTRIC) ); Mark_OSDScreen( OSDSCREEN_ELECTRIC );
#endif

#ifdef USE_OSD_SCREEN_MKSTATUS
  MenuCtrl_Push_P( OSDSCREEN_MKSTATUS , MENU_ITEM, NOFUNC, strGet(STR_OSDSCREEN_MKSTATUS) ); Mark_OSDScreen( OSDSCREEN_MKSTATUS );
#endif

#ifdef USE_OSD_SCREEN_USERGPS
  MenuCtrl_Push_P( OSDSCREEN_USERGPS , MENU_ITEM, NOFUNC, strGet(STR_OSDSCREEN_USERGPS) ); Mark_OSDScreen( OSDSCREEN_USERGPS );
#endif

#ifdef USE_OSD_SCREEN_3DLAGE
  MenuCtrl_Push_P( OSDSCREEN_3DLAGE , MENU_ITEM, NOFUNC, strGet(STR_OSDSCREEN_3DLAGE) ); Mark_OSDScreen( OSDSCREEN_3DLAGE );
#endif

#ifdef USE_OSD_SCREEN_STATISTIC
  MenuCtrl_Push_P( OSDSCREEN_STATISTICS, MENU_ITEM, NOFUNC, strGet(STR_OSDSCREEN_STATISTIC) ); Mark_OSDScreen( OSDSCREEN_STATISTICS );
#endif

#ifdef USE_OSD_SCREEN_OLD
  MenuCtrl_Push_P( OSDSCREEN_OSD0, MENU_ITEM, NOFUNC, strGet(STR_OSDSCREEN_OSD0) ); Mark_OSDScreen( OSDSCREEN_OSD0 );
  MenuCtrl_Push_P( OSDSCREEN_OSD1, MENU_ITEM, NOFUNC, strGet(STR_OSDSCREEN_OSD1) ); Mark_OSDScreen( OSDSCREEN_OSD1 );
  MenuCtrl_Push_P( OSDSCREEN_OSD2, MENU_ITEM, NOFUNC, strGet(STR_OSDSCREEN_OSD2) ); Mark_OSDScreen( OSDSCREEN_OSD2 );
#endif

  //---------------
  // Control
  //---------------
  while (true)
    {
      MenuCtrl_Control (MENUCTRL_EVENT);

      if (MenuCtrl_GetKey() == KEY_ESC)
        break;      // ENDE

      itemid = MenuCtrl_GetItemId();                  // welcher Menu-Punkt (ID)

      Edit_OSDScreen(itemid);
    }

  //---------------
  // Destroy
  //---------------
  MenuCtrl_Destroy();
}

//--------------------------------------------------------------
// Setup_FollowMe()
//--------------------------------------------------------------
#ifdef USE_FOLLOWME
void Setup_FollowMe( void )
  {
    uint8_t itemid;

    //---------------
    // Create
    //---------------
    MenuCtrl_Create();
    MenuCtrl_SetTitle_P( PSTR("FollowMe Setup") );

    //---------------
    // Menuitems
    //---------------
    //--------------------------------------------------------------------------------------------------------
    // 25.06.2014 OG - auskommentiert weil nicht verwendet. FollowMe wird aktuell ueber die Daten-Updaterate
    // der BT GPS-Maus getriggert!
    //--------------------------------------------------------------------------------------------------------
    //MenuCtrl_PushML2_P( FME_REFRESH , MENU_ITEM, NOFUNC , FME_REFRESH_de , FME_REFRESH_en );

    MenuCtrl_PushML2_P( FME_OFFSET_LAT , MENU_ITEM, NOFUNC, FME_OFFSET_LATITUDE_de , FME_OFFSET_LATITUDE_en );
    MenuCtrl_PushML2_P( FME_OFFSET_LONG, MENU_ITEM, NOFUNC, FME_OFFSET_LONGITUDE_de, FME_OFFSET_LONGITUDE_en );

    MenuCtrl_PushML2_P( FME_SPEED , MENU_ITEM, NOFUNC, FME_SPEED_de , FME_SPEED_en );
    MenuCtrl_PushML2_P( FME_RADIUS , MENU_ITEM, NOFUNC, FME_RADIUS_de , FME_RADIUS_en );
    MenuCtrl_PushSeparator();// MENUE TRENNLINIE
    MenuCtrl_PushML2_P( GPS_DATA , MENU_ITEM, NOFUNC, GPS_DATA_de , GPS_DATA_en );

    //---------------
    // Control
    //---------------
    while( true )
      {
        MenuCtrl_Control( MENUCTRL_EVENT );

        if( MenuCtrl_GetKey() == KEY_ESC ) break;      // ENDE

        itemid = MenuCtrl_GetItemId();// welcher Menu-Punkt (ID)

        //if( itemid == FME_REFRESH ) { Config.FM_Refresh = Edit_generic( Config.FM_Refresh   ,250,60000, Show_uint3,1   ,NULL,NULL);  }

        // FollowMeStep2: Eingabe in m, intern als mm behandeln
        if( itemid == FME_OFFSET_LAT )
          { Config.FM_Offest_Latitude = Edit_generic( Config.FM_Offest_Latitude / 1000, -30, 30, Show_int3, 1, strGet(STR_METERS), NULL) * 1000;}
        if( itemid == FME_OFFSET_LONG)
          { Config.FM_Offset_Longitude = Edit_generic( Config.FM_Offset_Longitude / 1000, -30, 30, Show_int3, 1, strGet(STR_METERS), NULL) * 1000;}

        // FollowMe:
        if( itemid == FME_SPEED )
          { Config.FM_Speed = Edit_generic( Config.FM_Speed , 0, 100, Show_uint3, 1, PSTR("0.1 m/s") , NULL);}
        if( itemid == FME_RADIUS )
          { Config.FM_Radius = Edit_generic( Config.FM_Radius, 1, 20, Show_uint3, 1, strGet(STR_METERS), NULL);}

        //--------------------
        // GPS_DATA
        //--------------------
        if( itemid == GPS_DATA )
          {
            GPSMouse_ShowData( GPSMOUSE_SHOW_TIME, 0 ); // 0 = beenden mit Verbindungstrennung
          }
      }

    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();
  }
#endif // USE_FOLLOWME

//--------------------------------------------------------------
// Setup_GPSMouse()
//--------------------------------------------------------------
#ifdef USE_BLUETOOTH

void Setup_GPSMouse( void )
  {
    uint8_t itemid;

    //---------------
    // Create
    //---------------
    MenuCtrl_Create();
    MenuCtrl_SetTitleFromParentItem();// "GPS Maus"

    //---------------
    // Menuitems
    //---------------
    MenuCtrl_PushML2_P( GPS_SEARCH , MENU_ITEM, &BT_SearchDevices , GPS_SEARCH_de , GPS_SEARCH_en );
    MenuCtrl_PushML2_P( GPS_DEVICES , MENU_ITEM, &BT_SelectDevice , GPS_DEVICES_de , GPS_DEVICES_en );

    //--------------------------------------------------------------------------------------------------------
    // 25.06.2014 OG - auskommentiert weil erstmal nicht weiter benoetigt. Wurde zwar verwendet in tracking.c
    // aber auch dort wurde betroffener Code deaktiviert - siehe Anmerkung tracking.c / PKT_tracking()
    //--------------------------------------------------------------------------------------------------------
    //MenuCtrl_PushML2_P( GPS_TYPE    , MENU_ITEM, NOFUNC             , GPS_TYPE_de    , GPS_TYPE_en    );

    //--------------------------------------------------------------------------------------------------------
    // 25.06.2014 OG - auskommentiert weil nirgendwo verwendet
    //--------------------------------------------------------------------------------------------------------
    //MenuCtrl_PushML2_P( GPS_ACTIVE  , MENU_ITEM, NOFUNC             , GPS_ACTIVE_de  , GPS_ACTIVE_en  );

    MenuCtrl_PushSeparator();// MENUE TRENNLINIE
    MenuCtrl_PushML2_P( GPS_SHOWDEV , MENU_ITEM, NOFUNC , GPS_SHOWDEV_de , GPS_SHOWDEV_en );
    MenuCtrl_PushML2_P( GPS_DATA , MENU_ITEM, NOFUNC , GPS_DATA_de , GPS_DATA_en );

    //---------------
    // Control
    //---------------
    while( true )
      {
        MenuCtrl_Control( MENUCTRL_EVENT );

        if( MenuCtrl_GetKey() == KEY_ESC ) break;      // ENDE

        itemid = MenuCtrl_GetItemId();// welcher Menu-Punkt (ID)

        //--------------------
        // GPS_TYPE
        //--------------------
        /*
         if( itemid == GPS_TYPE )
         {
         Config.gps_UsedGPSMouse  =  Edit_generic( Config.gps_UsedGPSMouse, 0,1,GPSMOUSE,1  ,NULL,NULL);
         }
         */

        //--------------------
        // GPS_ACTIVE
        //--------------------
        /*
         if( itemid == GPS_ACTIVE )
         {
         Config.gps_UseGPS = Edit_generic( Config.gps_UseGPS, 0,1,YesNo,1  ,NULL,NULL);
         }
         */

        //--------------------
        // GPS_SHOWDEV
        //--------------------
        if( itemid == GPS_SHOWDEV )
          {
            //Show_MAC( progmem, headline, mac)
            Show_MAC( false, Config.gps_UsedDevName, Config.gps_UsedMac);
          }

        //--------------------
        // GPS_DATA
        //--------------------
        if( itemid == GPS_DATA )
          {
            GPSMouse_ShowData( GPSMOUSE_SHOW_TIME, 0 ); // 0 = beenden mit Verbindungstrennung
          }

      }

    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();
  }

//--------------------------------------------------------------
// zeigt die Konfig des BTM-222
//--------------------------------------------------------------
void BTM222_Info( void )
  {

    lcd_cls();

    set_Modul_On( Bluetooth );

    if( !ScrollBox_Create(55) )     // max. 55 Zeilen
    return;//   kein RAM mehr

    ScrollBox_Push_P( MNORMAL, PSTR("BTM-222 Config") );
    ScrollBox_Push_P( MNORMAL, PSTR("") );
    bt_showsettings();
    ScrollBox_Push_P( MNORMAL, PSTR("End of Config") );

    ScrollBox_Show();
    ScrollBox_Destroy();// free memory

    set_Modul_On( USB );

  }

//--------------------------------------------------------------
// Setup_BTM222_MenuCreate()
//
// das Menue aendert sich je nachdem ob BTM222 ein- oder
// ausgeschaltet ist
//--------------------------------------------------------------
void Setup_BTM222_MenuCreate( void )
  {
    //---------------
    // Create
    //---------------
    MenuCtrl_Create();
    MenuCtrl_SetTitleFromParentItem();// "Bluetooth"

    //---------------
    // Menuitems
    //---------------
    MenuCtrl_PushML2_P( BT_INSTALLED , MENU_ITEM, NOFUNC , BT_INSTALLED_de , BT_INSTALLED_en );

    if( Config.UseBT )
      {
        MenuCtrl_PushSeparator();   // MENUE TRENNLINIE
        MenuCtrl_PushML2_P( BT_NAME , MENU_ITEM, NOFUNC , BT_NAME_de , BT_NAME_en );
        MenuCtrl_PushML2_P( BT_PIN , MENU_ITEM, NOFUNC , BT_PIN_de , BT_PIN_en );
        MenuCtrl_PushML2_P( BT_REID , MENU_ITEM, NOFUNC , BT_REID_de , BT_REID_en );
        MenuCtrl_PushSeparator();// MENUE TRENNLINIE
        MenuCtrl_PushML2_P( BT_INIT , MENU_ITEM, NOFUNC , BT_INIT_de , BT_INIT_en );
        MenuCtrl_PushML2_P( BT_PCCONFIG , MENU_ITEM, &Port_FC2CFG_BT , BT_PCCONFIG_de , BT_PCCONFIG_en );
        MenuCtrl_PushML2_P( BT_SHOWCONFIG, MENU_ITEM, &BTM222_Info , BT_SHOWCONFIG_de, BT_SHOWCONFIG_en);
        MenuCtrl_PushSeparator();// MENUE TRENNLINIE
        MenuCtrl_PushML2_P( BT_MAC , MENU_ITEM, NOFUNC , BT_MAC_de , BT_MAC_en );
      }
  }

#endif  // end: #ifdef USE_BLUETOOTH

//--------------------------------------------------------------
// Setup_BTM222()
//--------------------------------------------------------------
#ifdef USE_BLUETOOTH

void Setup_BTM222( void )
  {
    uint8_t itemid;
    uint8_t UseBT;
    uint8_t i;
    char string[11];

    Setup_BTM222_MenuCreate();

    //---------------
    // Control
    //---------------
    while( true )
      {
        MenuCtrl_Control( MENUCTRL_EVENT );

        if( MenuCtrl_GetKey() == KEY_ESC ) break;      // ENDE

        itemid = MenuCtrl_GetItemId();// welcher Menu-Punkt (ID)
        edit = 0;

        //--------------------
        // BT_INSTALLED
        //--------------------
        if( itemid == BT_INSTALLED )
          {
            UseBT = Config.UseBT;
            Config.UseBT = Edit_generic( Config.UseBT ,0,1, YesNo,1 ,NULL,NULL);

            if( UseBT != Config.UseBT )  // hat Benutzer Einstellung geaendert?
              {
                if( Config.UseBT )   // BTM222 wurde aktiviert - initialisieren
                  {
                    BTM222_Initalize();

                    //if( bt_init() )
                    //    Config.BTIsSet = true;
                    //else
                    //    Config.BTIsSet = false;
                  }

                MenuCtrl_Destroy(); // Menue aendern wegen wechsel Wi232 vorhanden / nicht vorhanden
                Setup_BTM222_MenuCreate();
                continue;
              }
          }

        //--------------------
        // BT_NAME
        //--------------------
        if( itemid == BT_NAME )
          {
            bt_fixname();
            strncpyfill( string, Config.bt_name, bt_name_length+1 ); // bt_name_length=10 + 1 Terminierende 0
            Edit_String( string, bt_name_length, EDIT_BT_NAME );

            if( edit == 1 )
              {
                strrtrim( EditString);           // Leerzeichen rechts entfernen
                //lcd_print_at( 0, 5, EditString, 0);   // DEBUG
                //lcdx_printf_at( 17, 5, MNORMAL, 0,0, "%3d", strlen(EditString) );
                //_delay_ms(8000);

                strncpy( Config.bt_name, EditString, bt_name_length+1 );
              }
          }

        //--------------------
        // BT_PIN
        //--------------------
        if( itemid == BT_PIN )
          {
            for( i = 0; i < bt_pin_length; i++)
              {
                string[i] = Config.bt_pin[i];
              }
            string[bt_pin_length] = 0;
            Edit_String( string, bt_pin_length, EDIT_BT_PIN );

            if (edit == 1)
              {
                for( i = 0; i < bt_pin_length; i++)
                  {
                    Config.bt_pin[i] = EditString[i];
                  }
              }
          }

        //--------------------
        // BT_INIT
        //--------------------
        if( itemid == BT_INIT )
          {
            //lcd_cls ();
            //Old_Baudrate = Config.PKT_Baudrate;

            BTM222_Initalize();

            /*
             if( bt_init() == true )
             {
             lcd_printp_at( 0, 3, PSTR("BT Init ok"), MNORMAL);
             WriteBTInitFlag();
             }
             else
             {
             lcd_printp_at( 0, 3, PSTR("BT Init Error"), MNORMAL);
             Config.BTIsSet = false;
             set_beep( 1000, 0x0040, BeepNormal);
             }
             _delay_ms(2500);    // 2.5 Sekunden anzeigen fuer Benutzer
             */
          }

        //--------------------
        // BT_MAC
        //--------------------
        if( itemid == BT_MAC )
          {
            //Show_MAC( progmem, headline, mac)
            Show_MAC( true, PSTR("BTM-222 MAC"), Config.bt_Mac);
          }

        //--------------------
        // BT_REID
        //--------------------
        if( itemid == BT_REID )
          {
            for( i = 0; i < RE_ID_length; i++)
              {
                string[i] = Config.RE_ID[i];
              }
            string[RE_ID_length] = 0;
            Edit_String( string, RE_ID_length, EDIT_BT_PIN );

            if( edit == 1 )
              {
                for( i = 0; i < RE_ID_length; i++)
                  {
                    Config.RE_ID[i] = EditString[i];
                  }
              }
          }

      }  // end: while( true )

    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();

  } // end: Setup_BTM222()

#endif  // end: #ifdef USE_BLUETOOTH

//--------------------------------------------------------------
// Setup_BLE()
//--------------------------------------------------------------
void
Setup_BLE(void)
{
  uint8_t itemid;

  //---------------
  // Create
  //---------------
  MenuCtrl_Create();
  MenuCtrl_SetTitleFromParentItem();                  // "Bluetooth BLE"

  //---------------
  // Menuitems
  //---------------
  MenuCtrl_Push_P( BLE_INSTALLED, MENU_ITEM, NOFUNC, strGet(MODULE_EXIST));

  //---------------
  // Control
  //---------------
  while (true)
    {
      MenuCtrl_Control (MENUCTRL_EVENT);

      if (MenuCtrl_GetKey() == KEY_ESC)
        break;      // ENDE

      itemid = MenuCtrl_GetItemId();                  // welcher Menu-Punkt (ID)

      //--------------------
      // BT_INSTALLED
      //--------------------
      if (itemid == BT_INSTALLED)
        {
          Config.UseBLE = Edit_generic(Config.UseBLE, 0, 1, YesNo, 1,
              strGet(STR_EXTSV2MODULE), NULL);
        }
    }  // end: while( true )

  //---------------
  // Destroy
  //---------------
  MenuCtrl_Destroy();

} // end: Setup_BTM222()

//--------------------------------------------------------------
// Setup_MKConnection()
//--------------------------------------------------------------
void
Setup_MKConnection(void)
{
  uint8_t itemid;
  uint8_t old;

  //---------------
  // Create
  //---------------
  MenuCtrl_Create();
  MenuCtrl_SetTitleFromParentItem();                  // "Bluetooth BLE"

  //---------------
  // Menuitems
  //---------------
  MenuCtrl_PushML2_P( MKCONN_CONNECTION, MENU_ITEM, NOFUNC,
  MKCONN_CONNECTION_de, MKCONN_CONNECTION_en);
  MenuCtrl_PushSeparator();   // MENUE TRENNLINIE
  MenuCtrl_PushML2_P( MKCONN_BAUDRATE, MENU_ITEM, NOFUNC, MKCONN_BAUDRATE_de,
  MKCONN_BAUDRATE_en);

  //---------------
  // Control
  //---------------
  while (true)
    {
      MenuCtrl_Control (MENUCTRL_EVENT);

      if (MenuCtrl_GetKey() == KEY_ESC)
        break;      // ENDE

      itemid = MenuCtrl_GetItemId();                  // welcher Menu-Punkt (ID)

      //--------------------
      // MKCONN_CONNECTION
      //--------------------
      if (itemid == MKCONN_CONNECTION)
        {
          old = Config.U02SV2;
          Config.U02SV2 = Edit_generic(Config.U02SV2, 0, 1, MK_Connection, 1,
              NULL, NULL);

          if (Config.U02SV2 != old)                     // Verbindung geaendert?
            {
              if (Config.U02SV2 == 1)
                Change_Output (Uart02FC);                          // 1 = Kabel
              else
                Change_Output (Uart02Wi);                          // 0 = Wi.232
            }

          if ((Config.U02SV2 == 0) && (Config.UseWi == false)) // FEHLER: Wi.232 nicht aktiviert  (Config.U02SV2 = 0 -> entspricht Wi.232)
            {
              //PKT_Message_P( text                      , error, timeout, beep, clear)
              PKT_Message_P(strGet(STR_WI232_ACTIVATE), true, 3000, true, true);
            }
        }

      //--------------------
      // MKCONN_BAUDRATE
      //--------------------
      if (itemid == MKCONN_BAUDRATE)
        {
          old = Config.PKT_Baudrate;
          Config.PKT_Baudrate = Edit_generic(Config.PKT_Baudrate, 1, 6,
              Baudrate, 1, PSTR("Standard: ! 57600 !"), NULL);

          if (Config.PKT_Baudrate != old)
            {
              // Wi.232 umkonfigurieren
              if (Config.UseWi)
                {
                  Wi232_Initalize();
                }

              // BTM222 umkonfigurieren
#ifdef USE_BLUETOOTH
              if( Config.UseBT )
                {
                  //set_BTOn();
                  BTM222_Initalize();
                  //set_BTOff();
                }
#endif  // end: #ifdef USE_BLUETOOTH
            }
        }

    }  // end: while( true )

  //---------------
  // Destroy
  //---------------
  MenuCtrl_Destroy();

} // end: Setup_BTM222()

//--------------------------------------------------------------
// Setup_WI232_MenuCreate()
//
// das Menue aendert sich je nachdem ob Wi.232 ein- oder
// ausgeschaltet ist
//--------------------------------------------------------------
void
Setup_WI232_MenuCreate(void)
{
  //---------------
  // Create
  //---------------
  MenuCtrl_Create();
  MenuCtrl_SetTitleFromParentItem();      // "Wi.232"

  //---------------
  // Menuitems
  //---------------
  MenuCtrl_Push_P( WI_INSTALLED, MENU_ITEM, NOFUNC, strGet(CONNECT13));

  if (Config.UseWi)
    {
      MenuCtrl_PushSeparator();   // MENUE TRENNLINIE
      MenuCtrl_PushML2_P( WI_TXRX, MENU_ITEM, NOFUNC, WI_TXRX_de, WI_TXRX_en);
      MenuCtrl_PushML2_P( WI_GROUP, MENU_ITEM, NOFUNC, WI_GROUP_de,
          WI_GROUP_en);

      // 06.06.2014 OG
      //MenuCtrl_PushML2_P( WI_MODE      , MENU_ITEM, NOFUNC           , WI_MODE_de     , WI_MODE_en     );
      //MenuCtrl_PushML2_P( WI_TIMEOUT   , MENU_ITEM, NOFUNC           , WI_TIMEOUT_de  , WI_TIMEOUT_en  );
      //MenuCtrl_PushML2_P( WI_MTU       , MENU_ITEM, NOFUNC           , WI_MTU_de      , WI_MTU_en      );

      MenuCtrl_PushSeparator();   // MENUE TRENNLINIE
      MenuCtrl_PushML2_P( WI_INIT, MENU_ITEM, NOFUNC, WI_INIT_de, WI_INIT_en);

      //MenuCtrl_PushML2_P( WI_PCCONFIG  , MENU_ITEM, &Port_USB2CFG_Wi , WI_PCCONFIG_de , WI_PCCONFIG_en );
      MenuCtrl_PushML2_P( WI_PCCONFIG, MENU_ITEM, &Wi232_ConfigPC,
          WI_PCCONFIG_de, WI_PCCONFIG_en);

    }
}

//--------------------------------------------------------------
// Setup_WI232()
//--------------------------------------------------------------
void
Setup_WI232(void)
{
  uint8_t itemid;
  uint8_t UseWi;
  uint8_t SaveValue;


  Setup_WI232_MenuCreate();

  //---------------
  // Control
  //---------------
  while (true)
    {
      MenuCtrl_Control (MENUCTRL_EVENT);

      if (MenuCtrl_GetKey() == KEY_ESC)
        break;      // ENDE

      itemid = MenuCtrl_GetItemId();                  // welcher Menu-Punkt (ID)
      edit = 0;

      if (itemid == WI_TXRX)
        {
          SaveValue = Config.WiTXRXChannel;
          Config.WiTXRXChannel = Edit_generic(Config.WiTXRXChannel, 0, 5, Show_uint3, 1, NULL, NULL);

          if (SaveValue != Config.WiTXRXChannel)                        // Daten geï¿½ndert?
            {
              if (Wi232_Initalize() != 0)
                {
                  Config.WiTXRXChannel = SaveValue;                     // Verhindert Inkonsistenz zwischen Wi223 und EEprom bei Abbruch oder Fehler
                  set_beep( 200, 0x00ff, BeepNormal);                   // Beep
                  PKT_Popup_P( 200, strGet(STR_NOCHANGE),0,0,0);        // "keine ï¿½nderung!"
                }
              else
                {
                  MKVersion_Init();                                     // nach ï¿½nderung der WI232 Parameter muss der MK neu gesucht werden.
                }
            }

        }

      if (itemid == WI_GROUP)
        {
          SaveValue = Config.WiNetworkGroup;
          Config.WiNetworkGroup = Edit_generic(Config.WiNetworkGroup, 0, 127, Show_uint3, 1, NULL, NULL);

          if (SaveValue != Config.WiNetworkGroup)                       // Daten geï¿½ndert?
            {
              if (Wi232_Initalize() != 0)
                {
                  Config.WiNetworkGroup = SaveValue;                    // Verhindert Inkonsistenz zwischen Wi223 und EEprom bei Abbruch oder Fehler
                  set_beep( 200, 0x00ff, BeepNormal);                   // Beep
                  PKT_Popup_P( 200, strGet(STR_NOCHANGE),0,0,0);        // "keine ï¿½nderung!"

                }
              else
                {
                  MKVersion_Init();                                     // nach ï¿½nderung der WI232 Parameter muss der MK neu gesucht werden.
                }
            }

        }
      //if( itemid == WI_MODE      ) { Config.WiNetworkMode     = Edit_generic( Config.WiNetworkMode    , 0,   1, Wi_Netmode,1  ,NULL,NULL);  }
      //if( itemid == WI_TIMEOUT   ) { Config.WiTXTO            = Edit_generic( Config.WiTXTO           , 0, 127, Show_uint3,1  ,NULL,NULL);  }
      //if( itemid == WI_MTU       ) { Config.WiUartMTU         = Edit_generic( Config.WiUartMTU        , 0, 127, Show_uint3,1  ,NULL,NULL);  }
      if (itemid == WI_INIT)
        {
          Wi232_Initalize();
        }

      //--------------------
      // WI_INSTALLED
      //--------------------
      if (itemid == WI_INSTALLED)
        {
          UseWi = Config.UseWi;
          Config.UseWi = Edit_generic(Config.UseWi, 0, 1, YesNo, 1, NULL, NULL);
          if (UseWi != Config.UseWi)      // hat Benutzer Einstellung geaendert?
            {
              if (Config.UseWi)
                Wi232_Initalize();                // Wi232 wurde aktiviert: init

              MenuCtrl_Destroy(); // Menue aendern wegen wechsel Wi232 vorhanden / nicht vorhanden
              Setup_WI232_MenuCreate();
              continue;
            }
        }
    }

  //---------------
  // Destroy
  //---------------
  MenuCtrl_Destroy();
}

//--------------------------------------------------------------
// Setup_PKTGeneric()
//
// entspricht "PKT allgemein"
//--------------------------------------------------------------
void
Setup_PKTGeneric(void)
{
  uint8_t itemid;
  uint8_t old;

  //---------------
  // Create
  //---------------
  MenuCtrl_Create();

  //---------------
  // Einstellungen
  //---------------
  MenuCtrl_SetTitleFromParentItem();      // "PKT allgemein"
  //MenuCtrl_SetCycle( false );
  //MenuCtrl_SetShowBatt( false );
  //MenuCtrl_SetBeep( true );

  //---------------
  // Menuitems
  //---------------
  MenuCtrl_PushML2_P( PKT_LANGUAGE, MENU_ITEM, NOFUNC, LANGUAGE_de,LANGUAGE_en);
  MenuCtrl_PushML2_P( TIME_ZONE, MENU_ITEM, NOFUNC, TIME_ZONE_de, TIME_ZONE_en);
  MenuCtrl_PushML2_P( TIME_SUMMER, MENU_ITEM, NOFUNC, TIME_SUMMER_de,TIME_SUMMER_en);

  MenuCtrl_PushSeparator();   // MENUE TRENNLINIE

  MenuCtrl_PushML2_P( PKT_PKTOFF, MENU_ITEM, NOFUNC, PKT_PKTOFF_de,PKT_PKTOFF_en);
  MenuCtrl_PushML2_P( PKT_LIGHTOFF, MENU_ITEM, NOFUNC, LIGHTOFF_de,LIGHTOFF_en);

  MenuCtrl_PushSeparator();   // MENUE TRENNLINIE

  // 18.04.2014 OG: rausgenommen da es aktuell auch nicht mehr mit PKT 3.9m Hardware geht
  //if( PCB_Version == PKT39m )     // Helligkeit nur bei PKT39m - bei der PKT39x geht das nicht mehr
  //  MenuCtrl_PushML2_P( PKT_BRIGHTNESS   , MENU_ITEM, NOFUNC , BRIGHTNESS_de   , BRIGHTNESS_en  );

  MenuCtrl_PushML2_P( PKT_CONTRAST, MENU_ITEM, NOFUNC, CONTRAST_de,CONTRAST_en);
  MenuCtrl_PushML2_P( PKT_LCDORIENTATION, MENU_ITEM, NOFUNC, LCDORIENTATION_de,LCDORIENTATION_en);
  MenuCtrl_PushML2_P( PKT_BEEPER, MENU_ITEM, NOFUNC, BEEPER_de, BEEPER_en);

#ifdef USE_SOUND
  MenuCtrl_PushML2_P( PKT_SOUNDMODUL , MENU_ITEM, NOFUNC , SOUNDMODUL_de , SOUNDMODUL_en );
  MenuCtrl_PushML2_P( PKT_VOLUME , MENU_ITEM, NOFUNC , VOLUME_de , VOLUME_en );
#endif

  MenuCtrl_PushML2_P( PKT_ACCUTYPE, MENU_ITEM, NOFUNC, ACCUTYPE_de,ACCUTYPE_en);
  MenuCtrl_PushML2_P( PKT_ACCUMEASURE, MENU_ITEM, NOFUNC, ACCUMEASURE_de, ACCUMEASURE_en);

  //---------------
  // Control
  //---------------
  while (true)
    {
      MenuCtrl_Control (MENUCTRL_EVENT);

      if (MenuCtrl_GetKey() == KEY_ESC)
        break;

      itemid = MenuCtrl_GetItemId();        // welcher Menu-Punkt (ID)

      if (itemid == PKT_LANGUAGE)
        {
          old = Config.DisplayLanguage;
          Config.DisplayLanguage = Edit_generic(Config.DisplayLanguage, 0, 1,
              Language, 1, NULL, NULL);

          if (old != Config.DisplayLanguage)
            {
              PKT_Ask_Restart (
              strGetLanguage(LANGUAGE_de, LANGUAGE_en) );}
            }

      if (itemid == TIME_ZONE)
        Config.timezone = Edit_generic(Config.timezone, -12, 12, Show_uint3, 1,PSTR("+1 = Berlin  "), PSTR("-5 = New York"));
      if (itemid == TIME_SUMMER)
        Config.summertime = Edit_generic(Config.summertime, 0, 1, YesNo, 1,NULL, NULL);
      if (itemid == PKT_LIGHTOFF)
        Config.DisplayTimeout = Edit_generic(Config.DisplayTimeout, 0, 254,Show_uint3, 1, strGet(STR_HELP_PKTOFFTIME1), NULL);
      if (itemid == PKT_PKTOFF)
        Config.PKTOffTimeout = Edit_generic(Config.PKTOffTimeout, 0, 254, Show_uint3, 1, strGet(STR_HELP_PKTOFFTIME1), NULL);
      if (itemid == PKT_BEEPER)
        Config.HWBeeper = Edit_generic(Config.HWBeeper, 0, 1, YesNo, 1, NULL, NULL);
      if (itemid == PKT_SOUNDMODUL)
        Config.HWSound = Edit_generic(Config.HWSound, 0, 1, YesNo, 1, NULL, NULL);
      if (itemid == PKT_VOLUME)
        Config.Volume = Edit_generic(Config.Volume, 0, 50, Show_uint3, 1, NULL, NULL);
      if (itemid == PKT_CONTRAST)
        {
          Config.LCD_Kontrast = Edit_generic(Config.LCD_Kontrast, 10, 40, Kontrast, 1, NULL, NULL);
          lcd_set_contrast(Config.LCD_Kontrast);
        }
      if (itemid == PKT_LCDORIENTATION)
        {
          Config.LCD_Orientation = Edit_generic(Config.LCD_Orientation, 0, 1, NormRev, 1, NULL, NULL);
          lcd_set_orientation(Config.LCD_Orientation);
        }

      //if( itemid == PKT_BRIGHTNESS    )   Config.LCD_Helligkeit     = Edit_DisplayHelligkeit(Config.LCD_Helligkeit,0,100);
      if (itemid == PKT_ACCUTYPE)
        Config.PKT_Accutyp = Edit_generic(Config.PKT_Accutyp, 0, 1, PKT_Akku, 1, NULL, NULL);
      if (itemid == PKT_ACCUMEASURE)
        Edit_LipoOffset();
    }

  //---------------
  // Destroy
  //---------------
  MenuCtrl_Destroy();
}

//--------------------------------------------------------------
// Setup_Main()
//
// Das Hauptmenue fuer das gesamte Setup
//--------------------------------------------------------------
void
Setup_MAIN(void)
{
  uint8_t itemid;

  //---------------
  // Create
  //---------------
  MenuCtrl_Create();

  //---------------
  // Einstellungen
  //---------------
  MenuCtrl_SetTitleFromParentItem();      // "PKT Setup"
  //MenuCtrl_SetTitle_P( PSTR("PKT Setup") );
  //MenuCtrl_SetCycle( false );
  //MenuCtrl_SetShowBatt( false );
  //MenuCtrl_SetBeep( true );

  //---------------
  // Menuitems
  //---------------
  MenuCtrl_PushML2_P( SETUP_PKTCONFIG, MENU_SUB, &Setup_PKTGeneric, SETUP_PKTCONFIG_de, SETUP_PKTCONFIG_en);
  MenuCtrl_PushML2_P( SETUP_OSDVIEW, MENU_SUB, &Setup_OSD, SETUP_OSDVIEW_de, SETUP_OSDVIEW_en);
  MenuCtrl_PushML2_P( SETUP_OSDSCREENS, MENU_SUB, &Setup_OSDScreens, SETUP_OSDSCREENS_de, SETUP_OSDSCREENS_en);

  MenuCtrl_PushSeparator();       // MENUE TRENNLINIE

  MenuCtrl_PushML2_P( SETUP_MKCONNECTION, MENU_SUB, &Setup_MKConnection, SETUP_MKCONNECTION_de, SETUP_MKCONNECTION_en);

  MenuCtrl_PushML2_P( SETUP_WI232, MENU_SUB, &Setup_WI232, SETUP_WI232_de, SETUP_WI232_en);

#ifdef USE_BLUETOOTH
  MenuCtrl_PushML2_P( SETUP_BTM222 , MENU_SUB , &Setup_BTM222 , SETUP_BTM222_de , SETUP_BTM222_en );
#endif

#if (defined(USE_SV2MODULE_BLE) || defined(USE_WLAN))
  if( PCB_SV2RxTxPatch )          // nur sichtbar mit SV2 Patch!
  MenuCtrl_PushSeparator();// MENUE TRENNLINIE
#endif

#ifdef USE_SV2MODULE_BLE
  if( PCB_SV2RxTxPatch )          // nur sichtbar mit SV2 Patch!
  MenuCtrl_PushML2_P( SETUP_BLE , MENU_SUB , &Setup_BLE , SETUP_BLE_de , SETUP_BLE_en );
#endif

#ifdef USE_WLAN
  if( PCB_SV2RxTxPatch )          // nur sichtbar mit SV2 Patch!
  MenuCtrl_PushML2_P( SETUP_WIFLY , MENU_SUB , &Setup_WiFly , SETUP_WIFLY_de , SETUP_WIFLY_en );
#endif

  MenuCtrl_PushSeparator();       // MENUE TRENNLINIE

#ifdef USE_BLUETOOTH
  MenuCtrl_PushML2_P( SETUP_GPSMAUS , MENU_SUB , &Setup_GPSMouse , SETUP_GPSMAUS_de , SETUP_GPSMAUS_en );
#endif

#ifdef USE_FOLLOWME
  MenuCtrl_PushML2_P( SETUP_FOLLOWME , MENU_SUB , &Setup_FollowMe , SETUP_FOLLOWME_de , SETUP_FOLLOWME_en );
#endif

#ifdef USE_TRACKING
  MenuCtrl_PushML2_P( SETUP_SERVOCONFIG , MENU_SUB , &Setup_ServoTracking, SETUP_SERVOCONFIG_de , SETUP_SERVOCONFIG_en ); // tracking/servo_setup.c
#endif

#ifdef USE_JOYSTICK
  MenuCtrl_PushML2_P( SETUP_JOYSTICK , MENU_SUB , &Setup_Joysticks , SETUP_JOYSTICK_de , SETUP_JOYSTICK_en );
#endif

  MenuCtrl_PushSeparator();   // MENUE TRENNLINIE

  MenuCtrl_PushML2_P( SETUP_PKTUPDATE, MENU_ITEM, &PKT_Update, SETUP_PKTUPDATE_de, SETUP_PKTUPDATE_en);
  //MenuCtrl_PushML2_P( SETUP_PKTDEBUG     , MENU_ITEM, NOFUNC              , SETUP_PKTDEBUG_de     , SETUP_PKTDEBUG_en     );
  MenuCtrl_PushML2_P( SETUP_EEPROMRESET, MENU_ITEM, &PKT_Reset_EEprom, SETUP_EEPROMRESET_de, SETUP_EEPROMRESET_en);

  //---------------
  // Control
  //---------------
  while (true)
    {
      MenuCtrl_Control (MENUCTRL_EVENT);
      if (MenuCtrl_GetKey() == KEY_ESC)
        break;

      itemid = MenuCtrl_GetItemId();              // welcher Menu-Punkt (ID)

      //--------------------
      // SETUP_PKTDEBUG
      //--------------------
      if (itemid == SETUP_PKTDEBUG)
        {
          Config.Debug = Edit_generic(Config.Debug, 0, 1, Show_uint3, 1, NULL,
              NULL);
        }

    }

  //---------------
  // Destroy
  //---------------
  MenuCtrl_Destroy();
}
