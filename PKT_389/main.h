/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */
/* ISO-8859-1 do not change it, otherwise the LCD Implementation fails*/



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

//#  Codepage ISO-8859-1 , Wichtig!!!!! (Oktober 2017 cebra)


//############################################################################
//# HISTORY  main.h
//#
//# 16.07.2018 cebra
//# -chg:neue PKT Version 3.92a wegen FC Version 2.21c
//#
//#
//# 08.07.2018 cebra
//# -chg:neue PKT Version 3.91a wegen FC Version 2.20c
//#
//# 6.11.2017 cebra
//# -chg:neue PKT Version 3.90a wegen FC Version 2.19f (Beta)
//#
//# 31.10.2017 cebra
//# -chg:neue PKT Version 3.89b wegen FC Version 2.18
//#
//# 12.10.2017 cebra
//# -chg:neue PKT Version 3.89a wegen FC Version 2.16
//#
//# 4.1.2017 Cebra
//# - add: #include <stdbool.h> wegen Compilerfehler bei true/false
//# - chg: Umlautfehler korrigiert
//#
//# 16.04.2016 Starter
//# - Branch von FollowMeStep2 in Master gemerged
//#
//# 21.02.2016 Cebra
//# - chg: neue PKT Version 3.87c wegen neuer FC Version 2.14a
//#
//# 19.01.2016 Cebra
//# - chg: neue PKT Version 3.87a wegen neuer FC Version 2.13f
//#
//# 17.11.2015 Cebra
//# - chg: neue Version 3.85i Fehler im Wi232 Setup behoben
//#
//# 14.10.2015 Starter
//# - Offset for FollowMe jetzt activ in Version 3.85h
//# - FollowMeStep2 wird nicht mehr benötigt, nur zu Debugzwecken
//#
//# 21.09.2015 Cebra
//# - add: #defines für I2C und Sensorboard
//# - add: #include <inttypes.h>
//#
//# 20.09.2015 Starter
//# - FollowMeStep2 wird jetzt nurnoch von USE_FOLLOWME_STEP2 aktiviert
//#
//# 19.09.2015 Cebra
//# - add: Ordner 10DOF hinzugefügt für die Bearbeitung des GY-87 Sensorboard
//#        Kompass, Gyro, ACC, Luftdruck
//#
//# 03.08.2015 Cebra
//# - add: Define für GPS Debug hinzugefügt für GPS Berechnung FollowMe
//# - add: #define USE_WAYPOINTS , Code sparen
//#
//# 30.07.2015 Cebra
//# - chg: neue Version 3.85e wegen einigen Änderungen in GPS-Maus Bereich, CRC Erkennung und Weitergabe
//#
//#
//# 30.07.2015 Cebra
//# - chg: neue Version 3.85d wegen Änderungen in gpsmouse.c
//#
//#
//# 16.07.2015 Cebra
//# - chg: neue Version 3.85a wegen FC2.11a
//#
//# 09.04.2015 Cebra
//# - chg: neue Version 3.84a wegen FC2.09j
//#
//# 05.04.2015 Cebra (PKT383b)
//# - chg: Änderungen wegen Fehlfunktion mit NC 2.09h
//#        SendOutData( 'h', ADDRESS_ANY, 2, &mkdisplayCmd, 1, 0x00,1)
//#        ergänzt um 2. Parameter
//#
//# 19.03.2015 Cebra (PKT383a)
//# - chg: Anpassung an FC209d
//#
//# 27.01.2015 Cebra (PKT382a)
//# - chg: Anpassung an FC209a
//#
//# 26.09.2014 Cebra (PKT381b)
//#
//#
//# 26.09.2014 Cebra (PKT381a)
//#
//# 01.07.2014 OG (PKT380e)
//#
//# 28.06.2014 OG (PKT380dX4)
//#
//# 27.06.2014 OG (PKT380dX3)
//# - chg: USE_FOLLOWME eingeschaltet
//# - chg: USE_TRACKING abgeschaltet
//#
//# 22.06.2014 OG (PKT380dX3)
//#
//# 20.06.2014 OG (PKT380dX2)
//#
//# 18.06.2014 OG (PKT380dX1)
//#
//# 18.06.2014 OG (PKT380d)
//#
//# 05.06.2014 OG (PKT380cX5)
//#
//# 02.06.2014 OG (PKT380cX4)
//# Update auf Windows Atmel AVR Studio 6.2.1153 (Release)
//# (von ehemals Atmel AVR Studio 6.2 Beta)
//#
//# 27.05.2014 OG (PKT380cX3)
//#
//# 20.05.2014 OG (PKT380cX2)
//#
//# 19.05.2014 OG (PKT380cX1)
//# - add: USE_OSD_SCREEN_WAYPOINTS (keine Freigabe! In Entwicklung!)
//#
//# 18.05.2014 OG (PKT380cX1)
//#
//# 15.05.2014 OG (PKT380c)
//#
//# 15.05.2014 OG (PKT380bX7)
//# - chg: USE_OSD_SCREEN_3DLAGE eingeschaltet (war ungewollt abgeschaltet)
//#
//# 14.05.2014 OG (PKT380bX7)
//#
//# 13.05.2014 OG (PKT380bX6)
//# - add: USE_MAINMENU_SEPARATOR (aus menu.h hierhin verschoben)
//# - del: MKINFO_AUTO_REFRESH
//#
//# 12.05.2014 OG (PKT380bX5b)
//# - chg: keine Code-Changes - dafuer Compiler jetzt Atmel Studio 6.2 Beta
//#        (statt bisher Atmel Studio 5.1)
//#
//# 07.05.2014 OG (PKT380bX4)
//#
//# 07.05.2014 OG (PKT380bX3)
//# - fix: EEpromversion erhoeht auf 138 (siehe eeprom.c)
//#
//# 02.05.2014 OG (PKT380bX2)
//#
//# 28.04.2014 OG
//# - chg: USE_OSD_SCREEN_OLD abgeschaltet
//#
//# 20.04.2014 OG (PKT380bX1)
//#
//# 18.04.2014 OG (PKT380b)
//#
//# 17.04.2014 OG (PKT380aX1)
//# - chg: PKT Versionsanzeige geaendert auf "3.80a" statt wie frueher "3.8.0a"
//#
//# 16.04.2014 OG (PKT380a)
//#
//# 14.04.2014 OG (PKT374aX8)
//#
//# 02.04.2014 OG
//# - chg: define fuer Baud_2400 von 0 auf 7 geaendert
//#        (0/false wird benoetigt um zu erkennen ob keine Baud-Rate erkannt wurde)
//#
//# 01.04.2014 OG
//# - add: USE_SV2MODULE_BLE (Bluetooth 4 Low Energy Modul an SV2)
//#
//# 31.03.2014 OG (PKT374aX7)
//#
//# 30.03.2014 OG (PKT374aX6)
//# Sprachunterstuetzung fuer Hollaendisch vollstaendig entfernt
//#
//# 30.03.2014 OG (PKT374aX5)
//# Source-Schnipp PKT374aX5
//#
//# 01.03.2014 OG (PKT374aX5)
//#
//# 26.02.2014 OG (PKT374aX4)
//# - chg: DEBUG_NEW_PARAMETERS umbenannt zu DEBUG_PARAMSET
//#
//# 22.02.2014 OG (PKT374aX4)
//#
//# 17.02.2014 OG (PKT374aX3)
//# - chg: umbenannt: USE_MKPARAMETER -> USE_MKSETTINGS
//#
//# 14.02.2014 OG (PKT374aX2)
//# - add: MKINFO_AUTO_REFRESH
//# - del: veraltete PKT defines HWVERSION... entfernt
//#
//# 12.02.2014 OG (PKT374aX2)
//# - del: defines zu MKVERSIONnnn entfernt
//#
//# 10.02.2014 OG (PKT374aX1)
//# - add: ...
//#
//# 05.02.2014 OG (PKT374aX1)
//# - add: DEBUG_NEW_PARAMETERS - nur fuer Entwicklung!
//#
//# 30.01.2014 OG (PKT373cX4)
//# - add: USE_BLUETOOTH
//#
//# 29.01.2014 OG (3.7.3cX3)
//# - del: extern uint8_t searchMK(void) ist jetzt in mk/mkbase.c
//#
//# 24.01.2014 OG (3.7.3cX1)
//# - add: DEBUG_FC_COMMUNICATION (Auswirkung in usart.c)
//#
//# 06.01.2014 CB
//# - add: Abschaltung MK Parameter aendern bei nicht passender FC Software
//#
//# 02.12.2013 CB
//# - add: Anpassung Parameter an FC 2.01f
//#
//# 21.10.2013 CB
//# - add: Anpassung Parameter an FC 2.01a
//#
//# 16.07.2013 CB
//# - chg: neuen Versionsnummer wegen Wiflypatch
//#
//# 07.07.2013 OG
//# - add: ABO_TIMEOUT und von 3 sec auf 2 sec gesetzt (war vorher in timer.h)
//# - del: define USE_OSD_SCREEN_ELECTRIC_N
//# - del: USE_PKT_STARTINFO (wird nicht mehr benoetigt)
//# - add: DEBUG_SV2_EXTENSION um Debug-Ausgaben fuer den SV2-Patch an-/auszuschalten
//#
//# 05.07.2013 CB
//# - add: USE_WLAN
//#
//# 30.06.2013 OG
//# - add: USE_OSD_PKTHOOK
//#
//# 26.06.2013 OG
//# - del: USE_PKT_STARTINFO (wird nicht mehr benoetigt)
//#
//# 24.06.2013 OG
//# - add: USE_PKTTOOLS_SV2 - zeigt SV2-Verbindungsmenuepunkte in PKT-Tools an
//#        bzw. nicht an. Am PKT-SV2 liegen +5 Volt für die FC/NC an.
//#        Anmerkung OG: bei meinen Kompilaten wird das erstmal ausgeschaltet
//#                      sein bis 100% klar ist ob das keine Probleme macht
//#                      wenn der Kopter mit FC/NC nicht bereits anderweitig
//#                      (Netzteil/Lipo) mit Strom versorgt wird.
//#
//# 22.06.2013 Cebra
//# - chg: Fehler bei der Lipooffset Einstellung beseitigt, neue Version 3.7.0c
//#
//# 20.06.2013 Cebra
//# - chg: falsche Versionsnummer korrigiert, neu jetzt 3.7.0b
//#
//# 13.06.2013 OG
//# - del: USE_PCFASTCONNECT (nicht mehr benoetigt)
//#
//# 12.06.2013 Cebra
//# - chg: Versionswechsel auf 3.7.0a, Zielrelease für FC-Software 0.91 (bzw.1.00??)
//#
//# 10.06.2013 Cebra
//# - add: #define MKVERSION091f für FC-Version 0.91f
//#
//# 30.05.2013 Cebra
//# - add: #define MKVERSION091a für FC-Version 0.91a
//#
//# 19.05.2013 OG
//# - del: PKT_Update(), PKT_CheckUpdate(), PKT_Info(), PKT_SwitchOff()
//#        -> verschoben nach pkt/pkt.c
//# - chg: Kommentare zum Module-Support
//#
//# 19.05.2013 OG
//# - add: USE_MODULEINFO
//# - add: PKT_Update(), PKT_CheckUpdate(), PKT_Info(), PKT_SwitchOff()
//#
//# 18.05.2013 OG
//# - add: USE_JOYSTICK, USE_TRACKING, USE_OSDDATA, USE_MKPARAMETER, USE_MKDISPLAY
//#
//# 16.05.2013 OG
//# - add: USE_OSD_SCREEN_NAVIGATION, USE_OSD_SCREEN_ELECTRIC, USE_OSD_SCREEN_ELECTRIC_N,
//#        USE_OSD_SCREEN_MKSTATUS, USE_OSD_SCREEN_USERGPS, USE_OSD_SCREEN_3DLAGE,
//#        USE_OSD_SCREEN_STATISTIC
//# - add: USE_MKGPSINFO
//#
//# 15.05.2013 OG
//# - add: define USE_PKT_STARTINFO
//#
//# 13.05.2013 Cebra
//# - add: #define MKVERSION090h    // wegen GPS-Zeitausgabe NC
//#
//# 13.05.2013 Cebra
//# - add: #define USE_I2CMOTORTEST, I2C Funktionen schaltbar
//#
//# 05.05.2013 Cebra
//# - chg: #define USE_FOLLOWME
//#
//# 03.05.2013 OG
//# - del: USE_XPRINTF_LONG - spart keinen Platz mehr und ist nun dauerhaft notwendig
//#
//# 28.04.2013 OG
//# - add: define USE_PCFASTCONNECT
//# - add: define USE_FONT_BIG
//# - add: define USE_XPRINTF_LONG
//# - chg: Anordnung der Feature- & Debug-defines um einfacher via Copy&Paste
//#        die Einstellungen notieren bzw. posten zu koennen
//#
//# 03.04.2013 OG
//# - chg: define 'analognames' zu define 'USE_MKDEBUGDATA'
//# - add: USE_OSD_... defines
//############################################################################

#ifndef _MAIN_H
#define _MAIN_H
#include <inttypes.h>
#include <stdbool.h>
// Softwareversion des PKT

#define PKTSWVersion "3.92a"        // PKT Version


//#########################################################################
//# Einstellungen
//#########################################################################
#define USE_MAINMENU_SEPARATOR      // Menuetrennlinien im PKT-Hauptmenues anzeigen


//#########################################################################
//# MODULE SUPPORT
//# ein-/ausgeschalten von Modulen des PKT um ggf. Speicherplatz zu sparen
//#
//# Hinweis:
//#  wenn neue USE_* hinzugefuegt werden bitte die in PKT_Info()(pkt.c)
//#  ergaenzen fuer eine aktualisierte Modul-Info
//#########################################################################

//---------------------------------------------
//- Module: Hauptfunktionen und allgemeine
//---------------------------------------------
#define USE_OSDDATA                 // zeigt die OSD-Statistikdaten an (ca. 3.5 KByte)
#define USE_MKSETTINGS              // lesen, aktivieren und bearbeiten der MK-Settings 1-5 (ca. ?? KByte)
#define USE_MKDEBUGDATA             // Anzeige MK-Debug-Data (ca. 1.7 KByte)
#define USE_MKDISPLAY               // Anzeige MK-Display (ca. 1 KByte)
#define USE_MKGPSINFO               // Anzeige MK-GPS-Daten (ca. 2 KByte)
//#define USE_SOUND                 // PKT Sounderweiterung, benoetigt entsprechende Hardware (ca. 2.5 Kbyte)
#define USE_MODULEINFO              // Anzeige installierter Module in PKT_Info()  (ca. 1.8 KByte)
#define USE_BLUETOOTH               // Bluetooth Unterstuetzung des PKT (ca. 13 KByte)
                                    // Achtung! Das schaltet USE_TRACKING, USE_FOLLOWME aus!
//#define USE_WAYPOINTS               // Waypoint mit dem PKT

//Entwicklung:
//#define USE_ACCCALIBRATION         // schaltet ACC Calibration im Menü ein/aus
//#define USE_PRESSURE               // Barometric Pressure Sensor
//#define USE_SENSORS                // generelles Einschalten des Sensorboards und I2C Routinen
//#define USE_KOMPASS                // Tilt kompensierter Kompass für FollowMe Funktionen


//---------------------------------------------
//- Module: OSD-Screens
//---------------------------------------------
//#define USE_OSD_SCREEN_OLD        // OSD Screens OSD0, OSD1, OSD2 (ca. 6.6 KByte)
#define USE_OSD_SCREEN_NAVIGATION   // OSD-Screen: Navigation
#define USE_OSD_SCREEN_WAYPOINTS    // OSD-Screen: Waypoints
#define USE_OSD_SCREEN_ELECTRIC     // OSD-Screen: Electric
#define USE_OSD_SCREEN_MKSTATUS     // OSD-Screen: MK-Status
#define USE_OSD_SCREEN_USERGPS      // OSD-Screen: UserGPS
#define USE_OSD_SCREEN_3DLAGE       // OSD-Screen: 3D-Lage  (ca. 900 Bytes)
#define USE_OSD_SCREEN_STATISTIC    // OSD-Screen: Statistic

//---------------------------------------------
//- Module fuer spezielle Benutzergruppen
//---------------------------------------------
#define USE_FOLLOWME                // FollowMe Funktionen (ca. 3 Kbyte)
//#define USE_FOLLOWME_STEP2          // FollowMe Funktionen Abstand und Winkel einstellbar
//#define USE_JOYSTICK                // Joystick Support, benoetigt spezielle Hardware (ca. 4.1 KByte)
#define USE_WLAN                    // WLAN WiFly Modul an SV2 (ca. 6 KByte))
#define USE_SV2MODULE_BLE           // Bluetooth 4 Low Energy Modul - externes Modul an SV2 (RedBearLab BLE Mini) (ca. 200 Bytes)

//---------------------------------------------
//- Module mit geringer Relevanz
//---------------------------------------------
//#define USE_I2CMOTORTEST          // I2C Funktionen für Motortest (ca. 3.8 KByte)
//#define USE_FONT_BIG              // grosser 8x8 Font (ca. 2.2 KByte, bei Verwendung von USE_OSD_SCREEN_OLD ca. 2.4 KByte)

//---------------------------------------------
//- Module unfertig bzw. noch in der Entwicklung
//---------------------------------------------
//#define USE_TRACKING                // Antennentracking, benoetigt spezielle Hardware (ca. 6 KByte)


//---------------------------------------------
//- zusaetzliche Optionen
//---------------------------------------------
//#define USE_OSD_PKTHOOK
#define USE_PKTTOOLS_SV2            // im Menue von PKT-Tools erscheinen Punkte zur Verbindung zum MK via
                                    // 10-Pol-Kabel am PKT-SV2. Der PKT-SV2-Anschluss liefert zur FC/NC +5 Volt!


//#########################################################################
//# Debug Module & Einstellungen
//# Nur fuer die Entwicklung - fuer Veroeffentlichung alles abschalten!
//#########################################################################
//#define IgnoreFCVersion           // keine FC-Revisions Pruefung
//#define DEBUG_SV2_EXTENSION       // ...
//#define USE_OSD_DEMO              // zeigt Demo-Daten in den OSD-Screens (sofern vom Screen unterstuetzt) (fuer Fotos) (nicht fuer die Oeffentlichkeit)
//#define USE_OSD_SCREEN_DEBUG      // zusaetzliche Debug-Screen's aktivieren (ca. 1 KByte) (nicht fuer die Oeffentlichkeit)
//#define DEBUG                     // ?? Funktion unbekannt!
//#define DEBUG_FC_COMMUNICATION    // in usart.c Debugausgaben auf dem PKT bzgl. Datenempfang vom Kopter
//#define DEBUG_PARAMSET            // nur fuer Entwicklung! Fuer Release ABSCHALTEN!
//#define DEBUG_GPS                 // Entwicklung der GPS Berechnung für FollowMe (wird im Moment nicht genutzt)
//#define DEBUGSERIAL               // Debugausgaben über USB-Seriell


//#########################################################################
//# Einstellungen
//#########################################################################
//#define ABO_TIMEOUT       300 // 3 sec
#define ABO_TIMEOUT       200   // 2 sec




//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+ ggf. abhaengige USE-defines deaktivieren
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef USE_BLUETOOTH
  #undef USE_TRACKING
  #undef USE_FOLLOWME
#endif



// Fusebits für Hardware 1.2 D7 DC FC
// Fusebits für Hardware 1.3 D7 DC FC
// Fusebits für Hardware 3.x D7 DC FC
// avrdude -pm1284p -cavr109 -P/dev/ttyUSB1 -b115200 -V -Uflash:w:Dateiname.hex:a


// hier die entsprechende Hardwareversion der Leiterplatte einstellen
#define HWVERSION3_9   // Hardware Cebra Oktober 2011 ATmega1284P

#ifdef HWVERSION3_9
#include "HAL_HW3_9.h"
#endif

#define NO     0
#define NC     1
#define FC     2
#define MK3MAG 3
#define MKGPS  4
#define Wi232  5

// CB #define ENABLE_PWM


//                      Baud Rate
#define Baud_9600       1
#define Baud_19200      2
#define Baud_38400      3
#define Baud_57600      4
#define Baud_115200     5
#define Baud_4800       6
#define Baud_2400       7


//----------------------------------------------------------------------------
// Anmerkung: 29.01.2014 OG
// - muss spaeter an geeignete Stelle verschoben werden (ggf. mkbase.c/usart.c)
//----------------------------------------------------------------------------
extern volatile uint8_t mode;
extern uint8_t hardware;
extern uint8_t current_hardware;

#endif
