/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/****************************************************************************************
 *   Copyright (C) 2011 Christian "Cebra" Brandtner, brandtner@brandtner.net            *
 *                                                                                      *
 *   This program is free software; you can redistribute it and/or modify               *
 *   it under the terms of the GNU General Public License as published by               *
 *   the Free Software Foundation; either version 2 of the License.                     *
 *                                                                                      *
 *   This program is distributed in the hope that it will be useful,                    *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of                     *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                      *
 *   GNU General Public License for more details.                                       *
 *                                                                                      *
 *   You should have received a copy of the GNU General Public License                  *
 *   along with this program; if not, write to the                                      *
 *   Free Software Foundation, Inc.,                                                    *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                          *
 *                                                                                      *
 *                                                                                      *
 *   Languagesupport:                                                                   *
 *   http://www.netrino.com/Embedded-Systems/How-To/Firmware-Internationalization       *
 *   Nigel Jones                                                                        *
 ****************************************************************************************/

//############################################################################
//# HISTORY  messages.c
//#
//# 17.11.2015 Cebra
//# - add: neuer Text STR_NOCHANGE
//#
//# 08.08.2015 cebra
//# - add: STR_WAITNMEA
//#
//# 28.06.2014 OG
//# - chg: Text von ERROR_NODATA statt "Datenverlust!" jetzt "MK Datenverlust!"
//# - chg: Text von OSD_ERROR statt "FEHLER: Datenverlust!" jetzt "MK Datenverlust!"
//#
//# 27.06.2014 OG
//# - add: STR_BT_SEARCHTIME, STR_METERS, STR_BT_LOSTDATA
//#
//# 25.06.2014 OG
//# - add: STR_SEARCH_BT_ASK, STR_SEARCH_BT, STR_NO_BT_FOUND, STR_BT_DEVICES
//#
//# 24.06.2014 OG
//# - add: STR_NOBTM222, STR_NOGPSMOUSE, STR_GPSMOUSECONNECTION
//# - add: STR_GPSMOUSECONNECT, STR_GPSMOUSEDISCONNECT
//#
//# 23.06.2014 OG
//# - add: STR_WAITSATFIX
//#
//# 13.06.2014 OG
//# - add: STR_PKT_SWITCHOFF_NOW, STR_PKT_RESTART_NOW, STR_WI232_ACTIVATE
//# - del: mehrere CONNECT.. Strings
//# - del: DISPLAY3
//#
//# 11.06.2014 OG
//# - add: strGetLanguage()
//# - add: KEYLINE5
//# - del: TESTSTRING
//#
//# 10.06.2014 OG
//# - add: STR_WI232CONFIG, STR_USBCONNECTED, STR_SEEMKWIKI
//# - add: STR_ATTENTION, STR_SWITCHOFFMK
//# - del: CONNECT21, CONNECT22, CONNECT24, CONNECT25
//#
//# 08.06.2014 OG
//# - add: STR_EXTSV2MODULE
//#
//# 04.06.2014 OG
//# - add: STR_DATA
//# - add: STR_READING, STR_LABELS
//#
//# 01.06.2014 OG
//# - del: weitere unbenoetigte Strings geloescht
//#
//# 31.05.2014 OG
//# - add: STR_WITH, STR_WITHOUT
//# - del: etliche Strings die nicht mehr benoetigt werden geloescht
//#
//# 30.05.2014 OG
//# - add: STR_HELP_LIPOOFFSET1, STR_HELP_LIPOOFFSET2
//# - add: STR_HELP_LOWBAT1, STR_HELP_LOWBAT2
//#
//# 29.05.2014 OG
//# - del: etliche Strings die nicht mehr benoetigt werden geloescht
//#
//# 28.05.2014 OG
//# - add: STR_HELP_PKTLIGHTOFF1
//#
//# 26.05.2014 OG
//# - add: STR_OSDSCREEN_WAYPOINTS
//# - del: CHANGENORMREV1, CHANGENORMREV2
//#
//# 11.05.2014 OG
//# - chg: STR_SAVE_de - jetzt "Speichern" statt "speichern"
//#
//# 06.05.2014 OG
//# - add: STR_MENUCTRL_DELITEM, STR_MENUCTRL_DELASK, STR_MENUCTRL_NOTPOSSIBLE
//# - del: STR_FAV_DELETE
//#
//# 03.05.2014 OG
//# - add: STR_FAV_ADD, STR_FAV_DELETE, STR_FAV_FULL, STR_FAV_EXIST, STR_FAV_NOTEXIST
//#
//# 28.04.2014 OG
//# - add: STR_LONGPRESS
//# - chg: OSD_ERROR_de,OSD_ERROR_en Space am Ende entfernt / Ausrufungszeichen hinzugefuegt
//#
//# 09.04.2014 OG
//# - chg: WL1 von "WiFly vorh.:" zu "WiFly Modus:"
//#
//# 04.04.2014 OG
//# - add: STR_SEARCH, STR_FOUND, STR_SET
//#
//# 03.04.2014 OG
//# - add: STR_INITIALIZE
//#
//# 02.04.2014 OG
//# - chg: Text WL1_de gekuerzt
//#
//# 01.04.2014 OG
//# - add: BLE_EXIST, MODULE_EXIST
//#
//# 30.03.2014 OG
//# - chg: Text zu WL1 geaendert von "Wlan eingebaut" zu "WiFly vorhanden"
//#        -> das ist ein Aufsteckmodul und wird nicht eingebaut
//# - chg: strGet() angepasst auf 2 Sprachen (Deutsch, Englisch)
//# - del: Hollaendisch vollstaendig geloescht
//#
//# 29.03.2014 OG
//# - chg: Test geaendert von START_LASTPOS2 zu "löschen          Nein"
//# - chg: einige Texte bzgl. OSD-Data gekuerzt wegen Umstellung auf lcdx_printp_center()
//#
//# 27.03.2014 OG
//# - add: STR_SAVE, STR_DISCARD, STR_COPY, STR_SWITCHMOTOROFF, STR_SAVING
//#
//# 19.03.2014 OG
//# - add: KEYCANCEL
//#
//# 27.02.2014 OG
//# - add: STR_ACTIVE
//#
//# 20.02.2014 OG
//# - chg: Texte von BOOT1, BOOT2, TESTSTRING
//# - add: STR_VON, STR_NACH, STR_PKT
//#
//# 17.02.2014 OG
//# - add: EDIT_SETTING
//# - add: STR_ERROR, ERROR_NODATA, MSG_LOADSETTINGS, MSG_ACTIVATESETTING
//#
//# 12.02.2014 OG
//# - del: STARTMSG2
//# - add: NOMKFOUND
//#
//# 04.02.2014 OG
//# - add: CHANGENORMREV1, CHANGENORMREV2
//#
//# 03.02.2014 OG
//# - add: SHOWCELLU
//#
//# 02.02.2014 OG
//# - chg: START_LASTPOS
//# - chg: START_LASTPOS1
//# - chg: START_LASTPOS3
//# - del: START_LASTPOSDEL
//#
//# 24.01.2014 OG
//# - del: etliche ":" aus Bezeichnungen geloescht (bei verschiedenen Einstellungen)
//#        -> woher kamen die ueberhaupt? In der Historie sehe ich dazu nichts!
//# - add: MSG_WARNUNG, UPDATE3
//# - chg: UPDATE1, UPDATE2 (PKT Update)
//# - chg: SHUTDOWN (Space's entfernt)
//# - chg: START_VERSIONCHECK3: angepasst auf neue flexibilitaet des PKT wenn
//#        FC-Revision nicht unterstuetzt wird
//# - chg: OK_de von "Ok" nach "OK"
//#
//# 15.07.2013 Cebra
//# - add: Wlan Security erweitert
//#
//# 07.07.2013 OG
//# - add: OSD-Screen Namen
//#
//# 02.07.2013 Cebra
//# - add: Menuetexte fuer Wlan
//#
//# 27.06.2013 OG
//# - del: OSD_HOMEMKVIEW
//#
//# 26.06.2013 OG
//# - del: einige nicht mehr benoetigte Texte
//# - chg: Texte geaendert von START_VERSIONCHECK...
//# - chg: START_VERSIONCHECK5 zu MSG_FEHLER2
//#
//# 15.06.2013 OG
//# - add: OSD_MKSetting (fuer Setup: ein-/ausschalten der MK-Settingsanzeige)
//# - del: DISPLAY8 (ehemals LCD-Orientierung, nicht mehr benoetigt)
//# - chg: Code-Layout
//#
//# 13.06.2013 OG
//# - chg: kleine Textaenderung an STARTMSG2
//# - chg: Code-Layout
//#
//# 13.06.2013 CB
//# - add: Texte für OSD Statistik erweitert
//#
//# 25.05.2013 OG
//# - chg: redundante Strings durch define ersetzt um Speicherplatz zu sparen
//#
//# 05.05.2013 Cebra
//# - add: Erweiterung PKT Zeitsetup
//#
//# 25.04.2013 Cebra
//# - chg: LIPO entfernt, Text doppelt
//############################################################################

#include <avr/pgmspace.h>
#include "eeprom/eeprom.h"
#include "messages.h"


typedef enum
{
    GERMAN,
    ENGLISH,
    NETHERLAND,
    LAST_LANGUAGE,
} LANGUAGE;


static const char KEYLINE1_de[]                     PROGMEM = " \x1a     \x1b    Ende   OK";
static const char KEYLINE1_en[]                     PROGMEM = " \x1a     \x1b    end    OK";

static const char KEYLINE2_de[]                     PROGMEM = " \x18     \x19    Ende   OK";
static const char KEYLINE2_en[]                     PROGMEM = " \x18     \x19    end    OK";

static const char KEYLINE3_de[]                     PROGMEM = " \x18     \x19    Ende ";
static const char KEYLINE3_en[]                     PROGMEM = " \x18     \x19    end  ";

static const char KEYLINE4_de[]                     PROGMEM = "Ende   OK";
static const char KEYLINE4_en[]                     PROGMEM = "end    OK";

static const char KEYLINE5_de[]                     PROGMEM = " \x18     \x19    Abbr.  OK";
static const char KEYLINE5_en[]                     PROGMEM = " \x18     \x19   cancel  OK";

static const char KEYCANCEL_de[]                    PROGMEM = " Abbr.";
static const char KEYCANCEL_en[]                    PROGMEM = "cancel";

static const char BOOT1_de[]                        PROGMEM = "Taste 1 Sekunde";
static const char BOOT1_en[]                        PROGMEM = "press button";

static const char BOOT2_de[]                        PROGMEM = "festhalten!";
static const char BOOT2_en[]                        PROGMEM = "for 1 second!";

static const char START_LASTPOS_de[]                PROGMEM = "Letzte Position";
static const char START_LASTPOS_en[]                PROGMEM = "last position";

static const char START_LASTPOS1_de[]               PROGMEM = "Breitengr. Längengr.";
static const char START_LASTPOS1_en[]               PROGMEM = "latitude   longitude";

static const char START_LASTPOS2_de[]               PROGMEM = "löschen          Nein";
static const char START_LASTPOS2_en[]               PROGMEM = "delete            No";

static const char START_LASTPOS3_de[]               PROGMEM = "Google Eingabe:";
static const char START_LASTPOS3_en[]               PROGMEM = "Google Input:";

static const char START_SEARCHFC_de[]               PROGMEM = "suche Mikrokopter...";
static const char START_SEARCHFC_en[]               PROGMEM = "searching Kopter...";

static const char ENDE_de[]                         PROGMEM = "Ende";
static const char ENDE_en[]                         PROGMEM = "end";

static const char OK_de[]                           PROGMEM = "OK";
static const char OK_en[]                           PROGMEM = "ok";

static const char FEHLER_de[]                       PROGMEM = "Fehler";
static const char FEHLER_en[]                       PROGMEM = "error";

static const char AKTIV_de[]                        PROGMEM = "aktiv";
static const char AKTIV_en[]                        PROGMEM = "activ";

static const char STR_SAVE_de[]                     PROGMEM = "Speichern";
static const char STR_SAVE_en[]                     PROGMEM = "save";

static const char STR_SAVING_de[]                   PROGMEM = "wird gespeichert...";
static const char STR_SAVING_en[]                   PROGMEM = "saving...";

static const char STR_DISCARD_de[]                  PROGMEM = "Verwerfen";
static const char STR_DISCARD_en[]                  PROGMEM = "discard";

static const char STR_COPY_de[]                     PROGMEM = "Kopieren";
static const char STR_COPY_en[]                     PROGMEM = "copy";

static const char STR_SEARCH_de[]                   PROGMEM = "suche";
static const char STR_SEARCH_en[]                   PROGMEM = "search";

static const char STR_FOUND_de[]                    PROGMEM = "gefunden";
static const char STR_FOUND_en[]                    PROGMEM = "found";

static const char STR_SET_de[]                      PROGMEM = "setze";
static const char STR_SET_en[]                      PROGMEM = "set";

static const char STR_WITH_de[]                     PROGMEM = "mit";
static const char STR_WITH_en[]                     PROGMEM = "with";

static const char STR_WITHOUT_de[]                  PROGMEM = "ohne";
static const char STR_WITHOUT_en[]                  PROGMEM = "without";

static const char STR_SWITCHMOTOROFF_de[]           PROGMEM = "Motoren ausschalten!";
static const char STR_SWITCHMOTOROFF_en[]           PROGMEM = "switch motors off!";

static const char ON_de[]                           PROGMEM = "Ein  ";
static const char ON_en[]                           PROGMEM = "On   ";

static const char OFF_de[]                          PROGMEM = "Aus  ";
static const char OFF_en[]                          PROGMEM = "off  ";

static const char ESC_de[]                          PROGMEM = "ESC";
#define           ESC_en                            ESC_de

static const char SHUTDOWN_de[]                     PROGMEM = "PKT ausschalten?";
static const char SHUTDOWN_en[]                     PROGMEM = "shutdown PKT?";

static const char YESNO_de[]                        PROGMEM = "Ja   Nein";
static const char YESNO_en[]                        PROGMEM = "yes    no";

static const char NOYES_de[]                        PROGMEM = "Nein   Ja";
static const char NOYES_en[]                        PROGMEM = "no    yes";

static const char DELETEDATA_de[]                   PROGMEM = "Daten löschen?";
static const char DELETEDATA_en[]                   PROGMEM = "delete data?";

static const char UPDATE1_de[]                      PROGMEM = "Verbinde PKT mit PC!";
static const char UPDATE1_en[]                      PROGMEM = "Connect PKT to PC";

static const char UPDATE2_de[]                      PROGMEM = "Drücke 'Start' für";
static const char UPDATE2_en[]                      PROGMEM = "Press 'Start' for";

static const char UPDATE3_de[]                      PROGMEM = "min. 2 Sekunden";
static const char UPDATE3_en[]                      PROGMEM = "min. 2 seconds";

static const char ENDSTART_de[]                     PROGMEM = "Ende Start";
static const char ENDSTART_en[]                     PROGMEM = "End  Start";

static const char CONNECT13_de[]                    PROGMEM = "Modul eingebaut";
static const char CONNECT13_en[]                    PROGMEM = "Modul built in";

static const char KABEL_de[]                        PROGMEM = "Kabel";
static const char KABEL_en[]                        PROGMEM = "cable";

static const char SLAVE_de[]                        PROGMEM = "Slave";
#define           SLAVE_en                          SLAVE_de

static const char NORMAL_de[]                       PROGMEM = "Normal";
#define           NORMAL_en                         NORMAL_de

static const char REVERSE_de[]                      PROGMEM = "Reverse";
static const char REVERSE_en[]                      PROGMEM = "inverse";

static const char ENDOK_de[]                        PROGMEM = "Ende   OK";
static const char ENDOK_en[]                        PROGMEM = "End    OK";

static const char EEPROM1_de[]                      PROGMEM = "PKT-EEProm";
static const char EEPROM1_en[]                      PROGMEM = "Realy delete";

static const char EEPROM2_de[]                      PROGMEM = "wirklich löschen?";
static const char EEPROM2_en[]                      PROGMEM = "PKT-EEProm?";

static const char DEUTSCH_de[]                      PROGMEM = "deutsch";
static const char DEUTSCH_en[]                      PROGMEM = "german";

static const char ENGLISCH_de[]                     PROGMEM = "englisch";
static const char ENGLISCH_en[]                     PROGMEM = "english";

static const char YES_de[]                          PROGMEM = "Ja  ";
static const char YES_en[]                          PROGMEM = "yes ";

static const char NOO_de[]                          PROGMEM = "Nein";
static const char NOO_en[]                          PROGMEM = "no  ";

static const char OSD_3D_V_de[]                     PROGMEM = "V";
static const char OSD_3D_V_en[]                     PROGMEM = "F";

static const char OSD_3D_H_de[]                     PROGMEM = "H";
static const char OSD_3D_H_en[]                     PROGMEM = "B";

static const char OSD_3D_L_de[]                     PROGMEM = "L";
#define           OSD_3D_L_en                       OSD_3D_L_de

static const char OSD_3D_R_de[]                     PROGMEM = "R";
#define           OSD_3D_R_en                       OSD_3D_R_de

static const char OSD_3D_NICK_de[]                  PROGMEM = "Ni";
#define           OSD_3D_NICK_en                    OSD_3D_NICK_de

static const char OSD_3D_ROLL_de[]                  PROGMEM = "Ro";
#define           OSD_3D_ROLL_en                    OSD_3D_ROLL_de

static const char OSD_ERROR_de[]                    PROGMEM = "MK Datenverlust!";
static const char OSD_ERROR_en[]                    PROGMEM = "MK Data lost!";

static const char PARA_AKTIVI_de[]                  PROGMEM = "Aktivieren";
static const char PARA_AKTIVI_en[]                  PROGMEM = "activate";

static const char PARA_COPY_de[]                    PROGMEM = "Kopiere Setting";
static const char PARA_COPY_en[]                    PROGMEM = "copy settings";

static const char PARA_COPYQ_de[]                   PROGMEM = "Wirklich kopieren?";
static const char PARA_COPYQ_en[]                   PROGMEM = "really copy?";

static const char GPS2_de[]                         PROGMEM =  "gewähltes GPS Gerät  ";
static const char GPS2_en[]                         PROGMEM =  "selected GPS device  ";

static const char STATS_ITEM_0_de[]                 PROGMEM =  "max Höhe    :";
static const char STATS_ITEM_0_en[]                 PROGMEM =  "max altitude:";

static const char STATS_ITEM_1_de[]                 PROGMEM =  "max Geschw. :";
static const char STATS_ITEM_1_en[]                 PROGMEM =  "max speed   :";

static const char STATS_ITEM_2_de[]                 PROGMEM =  "max Entfern.:";
static const char STATS_ITEM_2_en[]                 PROGMEM =  "max distance:";

static const char STATS_ITEM_3_de[]                 PROGMEM =  "min Spannung:";
static const char STATS_ITEM_3_en[]                 PROGMEM =  "min voltage :";

static const char STATS_ITEM_4_de[]                 PROGMEM =  "max Zeit    :";
static const char STATS_ITEM_4_en[]                 PROGMEM =  "max time    :";

static const char STATS_ITEM_5_de[]                 PROGMEM =  "max Strom   :";
static const char STATS_ITEM_5_en[]                 PROGMEM =  "max current :";

static const char STATS_ITEM_6_de[]                 PROGMEM =  "Ent.Kapazit.:";
static const char STATS_ITEM_6_en[]                 PROGMEM =  "UsedCapacity:";

static const char POTI_de[]                         PROGMEM =  "Poti  ";
static const char POTI_en[]                         PROGMEM =  "poti  ";

static const char TASTER_de[]                       PROGMEM =  "Taster";
static const char TASTER_en[]                       PROGMEM =  "switch";

static const char STAT_OSDBL_de[]                   PROGMEM =  "OSD- und BL-Daten";
static const char STAT_OSDBL_en[]                   PROGMEM =  "OSD- and BL-Data";

static const char STAT_ERROR_de[]                   PROGMEM =  "MK Fehlerliste";
static const char STAT_ERROR_en[]                   PROGMEM =  "MK errorlist";

static const char STAT_GPS_de[]                     PROGMEM =  "User GPS-Daten";
static const char STAT_GPS_en[]                     PROGMEM =  "User GPS-data";

static const char STAT_POS_de[]                     PROGMEM =  "letzte Position";
static const char STAT_POS_en[]                     PROGMEM =  "last position";

static const char STAT_ALL_de[]                     PROGMEM =  "ALLE Daten";
static const char STAT_ALL_en[]                     PROGMEM =  "ALL data";

static const char DELETE_de[]                       PROGMEM =  "löschen?";
static const char DELETE_en[]                       PROGMEM =  "delete?";

static const char STR_OSDSCREEN_GENERAL_de[]        PROGMEM = "General";
#define           STR_OSDSCREEN_GENERAL_en          STR_OSDSCREEN_GENERAL_de

static const char STR_OSDSCREEN_NAVIGATION_de[]     PROGMEM = "Navigation";
#define           STR_OSDSCREEN_NAVIGATION_en       STR_OSDSCREEN_NAVIGATION_de

static const char STR_OSDSCREEN_WAYPOINTS_de[]      PROGMEM = "Waypoints";
#define           STR_OSDSCREEN_WAYPOINTS_en        STR_OSDSCREEN_NAVIGATION_de

static const char STR_OSDSCREEN_ELECTRIC_de[]       PROGMEM = "Electric";
#define           STR_OSDSCREEN_ELECTRIC_en         STR_OSDSCREEN_ELECTRIC_de

static const char STR_OSDSCREEN_MKSTATUS_de[]       PROGMEM = "MK-Status";
#define           STR_OSDSCREEN_MKSTATUS_en         STR_OSDSCREEN_MKSTATUS_de

static const char STR_OSDSCREEN_USERGPS_de[]        PROGMEM = "User GPS";
#define           STR_OSDSCREEN_USERGPS_en          STR_OSDSCREEN_USERGPS_de

static const char STR_OSDSCREEN_3DLAGE_de[]         PROGMEM = "3D Lage";
#define           STR_OSDSCREEN_3DLAGE_en           STR_OSDSCREEN_3DLAGE_de

static const char STR_OSDSCREEN_STATISTIC_de[]      PROGMEM = "Statistics";
#define           STR_OSDSCREEN_STATISTIC_en        STR_OSDSCREEN_STATISTIC_de

static const char STR_OSDSCREEN_OSD0_de[]           PROGMEM = "OSD0";
#define           STR_OSDSCREEN_OSD0_en             STR_OSDSCREEN_OSD0_de

static const char STR_OSDSCREEN_OSD1_de[]           PROGMEM = "OSD1";
#define           STR_OSDSCREEN_OSD1_en             STR_OSDSCREEN_OSD1_de

static const char STR_OSDSCREEN_OSD2_de[]           PROGMEM = "OSD2";
#define           STR_OSDSCREEN_OSD2_en             STR_OSDSCREEN_OSD2_de

static const char NOMKFOUND_de[]                    PROGMEM = "keinen MK gefunden!";
static const char NOMKFOUND_en[]                    PROGMEM = "no MK found!";

static const char STR_ERROR_de[]                    PROGMEM = "FEHLER";
static const char STR_ERROR_en[]                    PROGMEM = "ERROR";

static const char ERROR_NODATA_de[]                 PROGMEM = "MK Datenverlust!";
static const char ERROR_NODATA_en[]                 PROGMEM = "MK Data lost!";

static const char MSG_LOADSETTINGS_de[]             PROGMEM = "lade Settings...";
static const char MSG_LOADSETTINGS_en[]             PROGMEM = "loading settings...";

static const char MSG_ACTIVATESETTING_de[]          PROGMEM = "aktiviere Setting...";
static const char MSG_ACTIVATESETTING_en[]          PROGMEM = "activate setting...";

static const char EDIT_SETTING_de[]                 PROGMEM = "Bearbeiten";
static const char EDIT_SETTING_en[]                 PROGMEM = "edit";

static const char STR_VON_de[]                      PROGMEM = "von";
static const char STR_VON_en[]                      PROGMEM = "from";

static const char STR_NACH_de[]                     PROGMEM = "nach";
static const char STR_NACH_en[]                     PROGMEM = "to";

static const char STR_PKT_de[]                      PROGMEM = "PKT";
#define           STR_PKT_en                        STR_PKT_de

static const char STR_ACTIVE_de[]                   PROGMEM = "AKTIV!";
static const char STR_ACTIVE_en[]                   PROGMEM = "ACTIVE!";

static const char MODULE_EXIST_de[]                 PROGMEM = "Modul vorhanden";
static const char MODULE_EXIST_en[]                 PROGMEM = "Module exist";

static const char STR_INITIALIZE_de[]               PROGMEM = "Initialisieren";
static const char STR_INITIALIZE_en[]               PROGMEM = "Initialize";

static const char STR_LONGPRESS_de[]                PROGMEM = "langer Tastendruck:";
static const char STR_LONGPRESS_en[]                PROGMEM = "long press:";

static const char STR_FAV_ADD_de[]                  PROGMEM = "Fav hinzugefügt!";
static const char STR_FAV_ADD_en[]                  PROGMEM = "fav added!";

static const char STR_FAV_FULL_de[]                 PROGMEM = "* Fav ist voll *";
static const char STR_FAV_FULL_en[]                 PROGMEM = "* fav is full *";

static const char STR_FAV_EXIST_de[]                PROGMEM = "* Fav vorhanden *";
static const char STR_FAV_EXIST_en[]                PROGMEM = "* fav exists*";

static const char STR_FAV_NOTEXIST_de[]             PROGMEM = "kein Fav vorhanden!";
static const char STR_FAV_NOTEXIST_en[]             PROGMEM = "no fav exist!";

static const char STR_MENUCTRL_DELITEM_de[]         PROGMEM = "Eintrag entfernt!";
static const char STR_MENUCTRL_DELITEM_en[]         PROGMEM = "item deleted!";

static const char STR_MENUCTRL_DELASK_de[]          PROGMEM = "Eintrag entfernen?";
static const char STR_MENUCTRL_DELASK_en[]          PROGMEM = "delete item?";

static const char STR_MENUCTRL_NOTPOSSIBLE_de[]     PROGMEM = "nicht möglich!";
static const char STR_MENUCTRL_NOTPOSSIBLE_en[]     PROGMEM = "not possible";

static const char STR_HELP_PKTOFFTIME1_de[]         PROGMEM = "Minuten (0=Aus)";
static const char STR_HELP_PKTOFFTIME1_en[]         PROGMEM = "minutes (0=off)";

static const char STR_HELP_LOWBAT1_de[]             PROGMEM = "<4.20 Einzelzelle  ";
static const char STR_HELP_LOWBAT1_en[]             PROGMEM = "<4.20 single cell  ";

static const char STR_HELP_LOWBAT2_de[]             PROGMEM = ">4.20 Gesamtspannu.";
static const char STR_HELP_LOWBAT2_en[]             PROGMEM = ">4.20 total voltage";

static const char STR_HELP_LIPOOFFSET1_de[]         PROGMEM = "verstellen, bis die";
static const char STR_HELP_LIPOOFFSET1_en[]         PROGMEM = "adjust offset until";

static const char STR_HELP_LIPOOFFSET2_de[]         PROGMEM = "Spannung passt!";
static const char STR_HELP_LIPOOFFSET2_en[]         PROGMEM = "voltage fits!";

static const char STR_DATA_de[]                     PROGMEM = "Daten";
static const char STR_DATA_en[]                     PROGMEM = "data";

static const char STR_READING_de[]                  PROGMEM = "lese";
static const char STR_READING_en[]                  PROGMEM = "reading";

static const char STR_LABELS_de[]                   PROGMEM = "Labels";
static const char STR_LABELS_en[]                   PROGMEM = "labels";

static const char STR_EXTSV2MODULE_de[]             PROGMEM = "externes SV2-Modul";
static const char STR_EXTSV2MODULE_en[]             PROGMEM = "external SV2 module";

static const char STR_ATTENTION_de[]                PROGMEM = "* ACHTUNG *";
static const char STR_ATTENTION_en[]                PROGMEM = "* ATTENTION *";

static const char STR_SWITCHOFFMK_de[]              PROGMEM = "Kopter ausschalten!";
static const char STR_SWITCHOFFMK_en[]              PROGMEM = "switch off kopter!";

static const char STR_WI232CONFIG_de[]              PROGMEM = "Wi.232 Konfig.";
static const char STR_WI232CONFIG_en[]              PROGMEM = "Wi.232 Config";

static const char STR_USBCONNECTED_de[]             PROGMEM = "mit USB verbunden";
static const char STR_USBCONNECTED_en[]             PROGMEM = "connected with USB";

static const char STR_SEEMKWIKI_de[]                PROGMEM = "siehe MK-Wiki:";
static const char STR_SEEMKWIKI_en[]                PROGMEM = "see MK-Wiki:";

static const char STR_PKT_SWITCHOFF_NOW_de[]        PROGMEM = "jetzt ausschalten?";
static const char STR_PKT_SWITCHOFF_NOW_en[]        PROGMEM = "switch off now?";

static const char STR_PKT_RESTART_NOW_de[]          PROGMEM = "PKT neu starten!";
static const char STR_PKT_RESTART_NOW_en[]          PROGMEM = "restart PKT!";

static const char STR_WI232_ACTIVATE_de[]           PROGMEM = "aktiviere Wi.232!";
static const char STR_WI232_ACTIVATE_en[]           PROGMEM = "activate Wi.232!";

static const char STR_WAITSATFIX_de[]               PROGMEM = "* warte auf Satfix *";
static const char STR_WAITSATFIX_en[]               PROGMEM = "* waiting satfix *";

static const char STR_GPSMOUSECONNECT_de[]          PROGMEM = "verbinde GPS-Maus";
static const char STR_GPSMOUSECONNECT_en[]          PROGMEM = "search gps-mouse";

static const char STR_GPSMOUSEDISCONNECT_de[]       PROGMEM = "trenne GPS-Maus";
static const char STR_GPSMOUSEDISCONNECT_en[]       PROGMEM = "disjoin gps-mouse";

static const char STR_NOBTM222_de[]                 PROGMEM = "kein BTM222 vorh.";
static const char STR_NOBTM222_en[]                 PROGMEM = "no BTM222 installed";

static const char STR_NOGPSMOUSE_de[]               PROGMEM = "keine GPS-Maus!";
static const char STR_NOGPSMOUSE_en[]               PROGMEM = "no gps-mouse!";

static const char STR_GPSMOUSECONNECTION_de[]       PROGMEM = "GPS-Maus Verbindung";
static const char STR_GPSMOUSECONNECTION_en[]       PROGMEM = "gps-mouse connection";

static const char STR_SEARCH_BT_ASK_de[]            PROGMEM = "BT Geräte suchen?";
static const char STR_SEARCH_BT_ASK_en[]            PROGMEM = "search BT devices?";

static const char STR_SEARCH_BT_de[]                PROGMEM = "suche BT Geräte";
static const char STR_SEARCH_BT_en[]                PROGMEM = "search BT devices";

static const char STR_NO_BT_FOUND_de[]              PROGMEM = "kein Gerät gefunden";
static const char STR_NO_BT_FOUND_en[]              PROGMEM = "no device found";

static const char STR_BT_DEVICES_de[]               PROGMEM = "BT Geräte";
static const char STR_BT_DEVICES_en[]               PROGMEM = "BT devices";

static const char STR_BT_SEARCHTIME_de[]            PROGMEM = "(ca. 2 Minuten)";
static const char STR_BT_SEARCHTIME_en[]            PROGMEM = "(ca. 2 minutes)";

static const char STR_BT_LOSTDATA_de[]              PROGMEM = "BT Datenverlust";
static const char STR_BT_LOSTDATA_en[]              PROGMEM = "BT data loss";

static const char STR_METERS_de[]                   PROGMEM = "Meter";
static const char STR_METERS_en[]                   PROGMEM = "meters";


static const char STR_WAITNMEA_de[]                 PROGMEM = "Warte auf GPS Daten";
static const char STR_WAITNMEA_en[]                 PROGMEM = "waiting for GPS Data";

static const char STR_NOCHANGE_de[]                 PROGMEM = "nicht geändert";
static const char STR_NOCHANGE_en[]                 PROGMEM = "no change";

//******************************************************************
// hier stehen lassen, alle neuen Strings hier dr?ber einf?gen
static const char LAST_STR_de[]         PROGMEM =  "Lastring";
#define           LAST_STR_en           LAST_STR_de
//******************************************************************



//--------------------------------------------------------------
//--------------------------------------------------------------
const char * const strings[] PROGMEM =
{
  KEYLINE1_de,                  KEYLINE1_en,
  KEYLINE2_de,                  KEYLINE2_en,
  KEYLINE3_de,                  KEYLINE3_en,
  KEYLINE4_de,                  KEYLINE4_en,
  KEYLINE5_de,                  KEYLINE5_en,
  KEYCANCEL_de,                 KEYCANCEL_en,
  BOOT1_de,                     BOOT1_en,
  BOOT2_de,                     BOOT2_en,
  START_LASTPOS_de,             START_LASTPOS_en,
  START_LASTPOS1_de,            START_LASTPOS1_en,
  START_LASTPOS2_de,            START_LASTPOS2_en,
  START_LASTPOS3_de,            START_LASTPOS3_en,
  START_SEARCHFC_de,            START_SEARCHFC_en,
  ENDE_de,                      ENDE_en,
  OK_de,                        OK_en,
  FEHLER_de,                    FEHLER_en,
  AKTIV_de,                     AKTIV_en,
  STR_SAVE_de,                  STR_SAVE_en,
  STR_SAVING_de,                STR_SAVING_en,
  STR_DISCARD_de,               STR_DISCARD_en,
  STR_COPY_de,                  STR_COPY_en,
  STR_SEARCH_de,                STR_SEARCH_en,
  STR_FOUND_de,                 STR_FOUND_en,
  STR_SET_de,                   STR_SET_en,
  STR_WITH_de,                  STR_WITH_en,
  STR_WITHOUT_de,               STR_WITHOUT_en,
  STR_SWITCHMOTOROFF_de,        STR_SWITCHMOTOROFF_en,
  ON_de,                        ON_en,
  OFF_de,                       OFF_en,
  ESC_de,                       ESC_en,
  SHUTDOWN_de,                  SHUTDOWN_en,
  YESNO_de,                     YESNO_en,
  NOYES_de,                     NOYES_en,
  DELETEDATA_de,                DELETEDATA_en,
  UPDATE1_de,                   UPDATE1_en,
  UPDATE2_de,                   UPDATE2_en,
  UPDATE3_de,                   UPDATE3_en,
  ENDSTART_de,                  ENDSTART_en,
  CONNECT13_de,                 CONNECT13_en,
  KABEL_de,                     KABEL_en,
  SLAVE_de,                     SLAVE_en,
  NORMAL_de,                    NORMAL_en,
  REVERSE_de,                   REVERSE_en,
  ENDOK_de,                     ENDOK_en,
  EEPROM1_de,                   EEPROM1_en,
  EEPROM2_de,                   EEPROM2_en,
  DEUTSCH_de,                   DEUTSCH_en,
  ENGLISCH_de,                  ENGLISCH_en,
  YES_de,                       YES_en,
  NOO_de,                       NOO_en,
  OSD_3D_V_de,                  OSD_3D_V_en,
  OSD_3D_H_de,                  OSD_3D_H_en,
  OSD_3D_L_de,                  OSD_3D_L_en,
  OSD_3D_R_de,                  OSD_3D_R_en,
  OSD_3D_NICK_de,               OSD_3D_NICK_en,
  OSD_3D_ROLL_de,               OSD_3D_ROLL_en,
  OSD_ERROR_de,                 OSD_ERROR_en,
  PARA_AKTIVI_de,               PARA_AKTIVI_en,
  PARA_COPY_de,                 PARA_COPY_en,
  PARA_COPYQ_de,                PARA_COPYQ_en,
  GPS2_de,                      GPS2_en,
  STATS_ITEM_0_de,              STATS_ITEM_0_en,
  STATS_ITEM_1_de,              STATS_ITEM_1_en,
  STATS_ITEM_2_de,              STATS_ITEM_2_en,
  STATS_ITEM_3_de,              STATS_ITEM_3_en,
  STATS_ITEM_4_de,              STATS_ITEM_4_en,
  STATS_ITEM_5_de,              STATS_ITEM_5_en,
  STATS_ITEM_6_de,              STATS_ITEM_6_en,
  POTI_de,                      POTI_en,
  TASTER_de,                    TASTER_en,
  STAT_OSDBL_de,                STAT_OSDBL_en,
  STAT_ERROR_de,                STAT_ERROR_en,
  STAT_GPS_de,                  STAT_GPS_en,
  STAT_POS_de,                  STAT_POS_en,
  STAT_ALL_de,                  STAT_ALL_en,
  DELETE_de,                    DELETE_en,
  STR_OSDSCREEN_GENERAL_de,     STR_OSDSCREEN_GENERAL_en,
  STR_OSDSCREEN_NAVIGATION_de,  STR_OSDSCREEN_NAVIGATION_en,
  STR_OSDSCREEN_WAYPOINTS_de,   STR_OSDSCREEN_WAYPOINTS_en,
  STR_OSDSCREEN_ELECTRIC_de,    STR_OSDSCREEN_ELECTRIC_en,
  STR_OSDSCREEN_MKSTATUS_de,    STR_OSDSCREEN_MKSTATUS_en,
  STR_OSDSCREEN_USERGPS_de,     STR_OSDSCREEN_USERGPS_en,
  STR_OSDSCREEN_3DLAGE_de,      STR_OSDSCREEN_3DLAGE_en,
  STR_OSDSCREEN_STATISTIC_de,   STR_OSDSCREEN_STATISTIC_en,
  STR_OSDSCREEN_OSD0_de,        STR_OSDSCREEN_OSD0_en,
  STR_OSDSCREEN_OSD1_de,        STR_OSDSCREEN_OSD1_en,
  STR_OSDSCREEN_OSD2_de,        STR_OSDSCREEN_OSD2_en,
  NOMKFOUND_de,                 NOMKFOUND_en,
  STR_ERROR_de,                 STR_ERROR_en,
  ERROR_NODATA_de,              ERROR_NODATA_en,
  MSG_LOADSETTINGS_de,          MSG_LOADSETTINGS_en,
  MSG_ACTIVATESETTING_de,       MSG_ACTIVATESETTING_en,
  EDIT_SETTING_de,              EDIT_SETTING_en,
  STR_VON_de,                   STR_VON_en,
  STR_NACH_de,                  STR_NACH_en,
  STR_PKT_de,                   STR_PKT_en,
  STR_ACTIVE_de,                STR_ACTIVE_en,
  MODULE_EXIST_de,              MODULE_EXIST_en,
  STR_INITIALIZE_de,            STR_INITIALIZE_en,
  STR_LONGPRESS_de,             STR_LONGPRESS_en,
  STR_FAV_ADD_de,               STR_FAV_ADD_en,
  STR_FAV_FULL_de,              STR_FAV_FULL_en,
  STR_FAV_EXIST_de,             STR_FAV_EXIST_en,
  STR_FAV_NOTEXIST_de,          STR_FAV_NOTEXIST_en,
  STR_MENUCTRL_DELITEM_de,      STR_MENUCTRL_DELITEM_en,
  STR_MENUCTRL_DELASK_de,       STR_MENUCTRL_DELASK_en,
  STR_MENUCTRL_NOTPOSSIBLE_de,  STR_MENUCTRL_NOTPOSSIBLE_en,
  STR_HELP_PKTOFFTIME1_de,      STR_HELP_PKTOFFTIME1_en,
  STR_HELP_LOWBAT1_de,          STR_HELP_LOWBAT1_en,
  STR_HELP_LOWBAT2_de,          STR_HELP_LOWBAT2_en,
  STR_HELP_LIPOOFFSET1_de,      STR_HELP_LIPOOFFSET1_en,
  STR_HELP_LIPOOFFSET2_de,      STR_HELP_LIPOOFFSET2_en,
  STR_DATA_de,                  STR_DATA_en,
  STR_READING_de,               STR_READING_en,
  STR_LABELS_de,                STR_LABELS_en,
  STR_EXTSV2MODULE_de,          STR_EXTSV2MODULE_en,
  STR_ATTENTION_de,             STR_ATTENTION_en,
  STR_SWITCHOFFMK_de,           STR_SWITCHOFFMK_en,
  STR_WI232CONFIG_de,           STR_WI232CONFIG_en,
  STR_USBCONNECTED_de,          STR_USBCONNECTED_en,
  STR_SEEMKWIKI_de,             STR_SEEMKWIKI_en,
  STR_PKT_SWITCHOFF_NOW_de,     STR_PKT_SWITCHOFF_NOW_en,
  STR_PKT_RESTART_NOW_de,       STR_PKT_RESTART_NOW_en,
  STR_WI232_ACTIVATE_de,        STR_WI232_ACTIVATE_en,
  STR_WAITSATFIX_de,            STR_WAITSATFIX_en,
  STR_GPSMOUSECONNECT_de,       STR_GPSMOUSECONNECT_en,
  STR_GPSMOUSEDISCONNECT_de,    STR_GPSMOUSEDISCONNECT_en,
  STR_NOBTM222_de,              STR_NOBTM222_en,
  STR_NOGPSMOUSE_de,            STR_NOGPSMOUSE_en,
  STR_GPSMOUSECONNECTION_de,    STR_GPSMOUSECONNECTION_en,
  STR_SEARCH_BT_ASK_de,         STR_SEARCH_BT_ASK_en,
  STR_SEARCH_BT_de,             STR_SEARCH_BT_en,
  STR_NO_BT_FOUND_de,           STR_NO_BT_FOUND_de,
  STR_BT_DEVICES_de,            STR_BT_DEVICES_en,
  STR_BT_SEARCHTIME_de,         STR_BT_SEARCHTIME_en,
  STR_BT_LOSTDATA_de,           STR_BT_LOSTDATA_en,
  STR_METERS_de,                STR_METERS_en,
  STR_WAITNMEA_de,              STR_WAITNMEA_en,
  STR_NOCHANGE_de,              STR_NOCHANGE_en,

  //******************************************************************
  // hier stehen lassen, alle neuen Strings hier dr?ber einf?gen
  LAST_STR_de,                  LAST_STR_en,

}; // end: strings



//--------------------------------------------------------------
// gibt je nach konfigurierter Sprache den entsprechenden String
// aus der obigen Stringtabelle zureuck
//--------------------------------------------------------------
const char * strGet( int str_no)
{
    if( Config.DisplayLanguage > 1 )    Config.DisplayLanguage = 1;

    if( Config.DisplayLanguage == 1 )   return (const char*) pgm_read_word( &strings[(str_no*2)+1] );       // Englisch (=1)

    return  (const char*) pgm_read_word( &strings[str_no*2] );                                              // Deutsch  (=0)
}



//--------------------------------------------------------------
// gibt je nach konfigurierter Sprache Parameter str_de oder
// Parameter str_en zurueck
//
// wird z.B. in Verbindung mit de/en-Menuetexten verwendet
//--------------------------------------------------------------
const char * strGetLanguage( char const *str_de, char const *str_en)
{
    if( Config.DisplayLanguage > 1 )    Config.DisplayLanguage = 1;

    if( Config.DisplayLanguage == 1 )   return  str_en;     // Englisch (=1)

    return  str_de;                                         // Deutsch  (=0)
}
