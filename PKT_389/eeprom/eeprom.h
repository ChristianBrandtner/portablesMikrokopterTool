/*****************************************************************************
 *   Copyright (C) 2008 Thomas Kaiser, thomas@ft-fanpage.de                  *
 *   Copyright (C) 2009 Peter "woggle" Mack, mac@denich.net                  *
 *   Copyright (C) 2013 Christian "Cebra" Brandtner, brandtner@brandtner.net *
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
//# HISTORY  eeprom.h
//#
//# 23.10.2015 Cebra
//# - add: LCD_Orientation, drehen des Displays wird wieder benötigt
//# - chg: EEpromversion auf 140 erhöht, EEprom Update erweitert
//#
//# 14.10.2015 Starter
//# - chg: rename FM_Azimuth and FM_Distance to FM_Offest_lat und FM_Offset_long
//#
//# 03.08.2015 CB
//# - chg: Obsolete Parameter geändert und für FollowMe verwendet, EEprom Version bleibt gleich
//#        Parameter FM_Refresh in FM_Azimuth geändert
//#        Parameter hyst_u_min in FM_Distance geändert
//#
//# 25.06.2014 OG
//# - chg: Kommentare der eeprom Struct aktualisert bzgl. FollowMe und Tracking
//#
//# 17.06.2014 OG
//# - chg: EEpromversion erhoeht auf 139
//#
//# 13.06.2014 OG
//# - chg: LCD_DisplayMode (Obsolete) geaendert zu PKTOffTimeout
//#
//# 31.05.2014 OG
//# - Recherche welche Config-Werte ueberahupt noch im PKT-Code verwendet werden
//#   und ggf. "OBSOLETE" Anmerkung in der Config-Struct
//# - Config-Struct wurde um etliche Kommentare ergaenzt bzgl. Verwendungszweck
//#
//# 30.05.2014 OG
//# - chg: Kommentare geaendert
//#
//# 26.05.2014 OG
//# - chg: Config.LCD_DisplayMode als "OBSOLETE" markiert
//#
//# 07.05.2014 OG
//# - chg: EEpromversion erhoeht auf 138
//#        -> keine neuen Parameter, neue Version nur fuer geanderte
//#           Initialisierung von MKParam_Favs
//#
//# 06.04.2014 OG
//# - chg: EEpromversion erhoeht auf 137
//# - add: Config.UseBLE
//# - add: Config.MKParam_Favs
//#
//# 01.04.2014 OG
//# - chg: EEpromversion erhoeht auf 136
//# - chg: ACHTUNG! EEpromversion umgestellt von HEX auf DEZIMAL:  0x87 => 135
//# - add: Config.UseBLE
//# - add: Config.MKParam_Favs
//# - add: MAX_MKPARAM_FAVORITES
//#
//# 03.02.2014 OG
//# - add: Config.OSD_ShowCellU; EEpromversion erhoeht auf 0x87
//#
//# 28.08.2013 Cebra
//# - chg: LastLatitude war uint32_t, auf int32_t korrigiert
//#
//# 04.07.2013 Cebra
//# - add: neue Parameter fuer Wlan; EEpromversion erhoeht auf 85
//#
//# 04.07.2013 OG
//# - add: Config.OSD_UseScreen; Epromversion erhoeht auf 84
//#
//# 02.07.2013 Cebra
//# - add: neue Parameter fuer Wlan; EEpromversion erhoeht auf 83
//#
//# 20.06.2013 CB
//# - chg: EEPROM Versionsänderung auf 82 wegen Wechsel LCD_Orientation zu OSD_ShowMKSetting
//#
//# 15.06.2013 OG
//# - chg: Config.LCD_ORIENTATION zu Config.OSD_ShowMKSetting
//#
//# 31.05.2013 CB
//# - chg: EEPROM Strukturänderung, Listen/Statistik ganz ans Ende gesetzt
//#
//# 31.05.2013 OG
//# - chg: Eeprom Version auf 81 wegen Erweiterungen der Statistik-Daten
//# - chg: Code-Layout
//#
//# 12.04.2013 CB
//# - chg: Kommentarergänzung U02SV2
//#
//# 28.03.2013 CB
//# - add: save and upgrade OSD_Statistic, GPS_User, MKErr_Log in EEProm structure variable
//############################################################################
#ifndef _EEPROM_H
#define _EEPROM_H

#include <stdbool.h>
#include "../mk-data-structs.h"
#include "../connect.h"
#include "../tracking/ng_servo.h"
#include "../waypoints/waypoints.h"
#include "../osd/osd.h"

//[General]
//FileVersion = 2
//NumberOfWaypoints = 15
//UsePOI = 0
//POI_CAM_NICK_CTRL = 0

//[POI]
//Altitude = 1
//Latitude = 46.7140763
//Longitude = 19.2507334

//[Waypoint1]
//Latitude = 46.7145686
//Longitude = 19.2515702
//Radius = 10
//Altitude = 15
//ClimbRate = 0
//DelayTime = 4
//WP_Event_Channel_Value = 96
//Heading = 180

#define EEpromVersion         140                       // wird nach jeder Parametererweiterung hochgezählt
// Anmerkung 01.04.2014 OG: umgestellt auf DEZIMALE schreibweise (wiso war das vorher Hex?)

#define NumberOfWaypoints      55                       // Anzahl der Waypoints in der EEPromliste
#define NumberOfWPLists         5                       // Anzahl WP Listen im PKT
#define MAX_LIST_LEN           31                       // Länge Waypointlist

#define MAX_MKPARAM_FAVORITES  10                       // Anzahl der MK-Parameter Favoriten (nicht aendern!)

#define bt_pin_length           4
#define RE_ID_length            4                       // Länge der RE-ID
#define bt_name_length         10
#define bt_mac_length          14
#define GPS_Bluetoothmouse1     0                       // NMEA BT-Mouse
#define GPS_Mikrokopter         1                       // GPS Daten vom MK für Tracking

#define wlan_password_length    10                      // Länge Wlan-Passwort
#define wlan_ssid_length        10                      // Länge Wlan-SSID
#define wlan_ip_length          15
#define wlan_netmask_length     15
#define wlan_gateway_length     15

#define POINT_TYPE_INVALID     255
#define POINT_TYPE_WP            0
#define POINT_TYPE_POI           1
#define INVALID               0x00
#define MAX_WPLIST_LEN          31

//typedef struct {
//        uint8_t rev;
//        uint16_t min;
//        uint16_t max;
//        uint16_t mid;
//} servo_t;

typedef struct
{
  uint8_t WPIndex;                                      // Index in der EEpromliste
  Point_t Waypoint;                                     // Waypoint
} WayPoints;

typedef struct
{
  uint8_t WPListnumber;                                 // Nummer der WP Liste im PKT
  uint8_t WPListAktiv;                                  // Liste aktiv
  uint8_t WPDirectory[31];                              // Enthält die Indexe der Waypoints im EEPROM
  uint8_t UsePOI;
  uint8_t POI_CAM_NICK_CTR;

} WPListHeader;

typedef struct
{
  WPListHeader WPList;                                  // Waypointliste im PKT
} WPListDirectory;

typedef struct SStructure
{
  uint8_t Version;                                  // PKT-Eeprom Version!
  uint8_t MK_LowBat;                                // MK-LowBat Warnung u.a. fuer osd.c
  uint8_t DisplayTimeout;                           // autom. LCD ausschalten nach n Minuten
  uint8_t DisplayLanguage;                          // eingestellte Sprache
  uint8_t WiTXRXChannel;                            // Wi.232:
  uint8_t WiNetworkGroup;                           // Wi.232:
  uint8_t WiNetworkMode;                            // Wi.232:
  uint8_t WiTXTO;                                   // Wi.232:
  uint8_t WiUartMTU;                                // Wi.232:
  uint8_t OSD_ShowMKSetting;                        // Anzeige MK-Setting beim OSD Start
  uint8_t PKTOffTimeout;                            // autom. PKT ausschalten nach n Minuten
  uint8_t LCD_Kontrast;                             // LCD-Kontrast
  uint8_t LCD_Helligkeit;                           // LCD-Helligkeit - Aktuell nicht mehr verfuegbar weil auskommentiert! (mit HW 3.9x geht das sowiso nicht mehr!)
  uint8_t USBBT;                                    //  ## OBSOLETE ## 31.05.2014 OG: wird nicht verwendet! (wofuer war da in der Vergangenheit?)
  uint8_t U02SV2;                                   // 0=Wi232-Verbindung zum MK, 1 = Kabelverbindung zum MK
  uint8_t Debug;                                    //  ??? OBSOLETE ??? 31.05.2014 OG: wird eigentlich nicht mehr verwendet (war zum debuggen vom PKT)
  uint8_t UseWi;                                    // Wi232 wird genutzt
  uint8_t UseBT;                                    // BT wird genutzt

  uint8_t WiIsSet;                                  // Wi232 ist initialisiert
  uint8_t BTIsSet;                                  // BT ist initialisiert
  uint8_t BTIsSlave;                                // Slave Flag
  char bt_pin[bt_pin_length + 1];                   // BT Pinnummer
  char bt_name[bt_name_length + 1];                 // BT Name
  char RE_ID[RE_ID_length + 1];                     // RE-ID
  char bt_Mac[bt_mac_length + 1];                   // MAC-Adresse BTM222

  char gps_UsedDevName[20];                         // benutztes GPS Device Name
  char gps_UsedMac[14];                             // benutztes GPS Device Mac Adresse
  uint8_t gps_UseGPS;                               //  ## GGF. OBSOLETE ## 25.06.2014 OG: siehe setup.c/Setup_GPSMouse() - "ist GPS aktiv?"
  uint8_t gps_UsedGPSMouse;                         //  ## GGF. OBSOLETE ## 25.06.2014 OG: siehe setup.c/Setup_GPSMouse() und tracking.c/PKT_tracking() - "GPS Maustyp"

  int32_t LastLongitude;                            // Letzte Position
  int32_t LastLatitude;                             // Letzte Position
  uint8_t PKT_IdleBeep;                             //  ## OBSOLETE ## 31.05.2014 OG: wurde das jemals verwendet?
  uint8_t PKT_StartInfo;                            //  ## OBSOLETE ## 26.06.2013 OG: ehemals "PKT Startinfo anzeigen"
  uint16_t Lipo_UOffset;                            // Offset für die Lipospannugsmessung
  uint8_t PKT_Accutyp;                              // verwendeter Akkutyp
  uint8_t OSD_RCErrorbeep;                          // Empfangsausffallwarnung im OSD Screen
  uint8_t OSD_InvertOut;                            // Out1/2 invertiert anzeigen
  uint8_t OSD_LEDform;                              // Form der Anzeige ( + oder schwarz)
  uint8_t OSD_SendOSD;                              // OSD Daten an SV2 senden
  uint8_t OSD_Fallspeed;                            // maximale Sinkrate
  uint8_t OSD_VarioBeep;                            // Vario Beep im OSD Screen
  uint8_t OSD_HomeMKView;                           //  ## OBSOLETE ## 02.07.2013 OG: ehemals "Home Circle from MK-View"
  uint16_t OSD_mAh_Warning;                         // mAh Warnschwelle
  uint8_t OSD_ScreenMode;                           // Variante des OSD-Screen
  uint8_t OSD_LipoBar;                              // Bargraphanzeige für MK Lipo
  uint8_t PKT_Baudrate;                             // Baudrate für BT und Wi232
  int16_t FM_Offest_Latitude;                       // Azimuth für FollowMe 4.8.2015 CB
  uint16_t FM_Speed;                                // FollowMe Speed in m/s *0.1
  uint16_t FM_Radius;                               // Waypoint Tolerance Radius in meter
  uint8_t HWSound;                                  // Hardware Sounderweiterung an PD7
  uint8_t HWBeeper;                                 // Hardware Beeper an PC7
  uint8_t Volume;                                   // Lautstärke
  servo_t servo[2];                                 // Tracking:
  uint8_t sIdxSteps;                                // Tracking:
  int16_t FM_Offset_Longitude;                      // Distance für FollowMe 4.8.2015 CB
  uint8_t servo_frame;                              // Tracking:
  uint8_t single_step;                              // Tracking:
  uint8_t repeat;                                   // Tracking:
  uint8_t pause;                                    // Tracking:
  uint8_t pause_step;                               // Tracking:
  uint8_t tracking;                                 //  ## OBSOLETE ## 31.05.2014 OG: das wird nirgendwo verwendet!
  uint8_t track_hyst;                               //  ## OBSOLETE ## 31.05.2014 OG: das wird nirgendwo verwendet!
  uint8_t track_tx;                                 //  ## OBSOLETE ## 31.05.2014 OG: das wird nirgendwo verwendet!

  uint16_t stick_min[5];                            // Joystick: Stickparameter
  uint16_t stick_max[5];                            // Joystick:
  uint8_t stick_typ[5];                             // Joystick:
  uint8_t stick_dir[5];                             // Joystick:
  uint8_t stick_neutral[5];                         // Joystick: Stick neutralisierend: ja/nein
  uint8_t Lipomessung;                              // wenn Lipomessung deaktiviert(Lötbrücke öffnen), kann der Kanal als Stick verwendet werden
  int8_t timezone;                                  // Einstellbereich -12 .. 0 .. 12 Defaultwert: 1 (entspricht unserer Zeitzone)
  uint8_t summertime;                               // Einstellung: 0 oder 1 (0=Winterzeit, 1=Sommerzeit) Defaultwert: 1 (entspricht unserer Sommerzeit in D)
  uint8_t UseWL;                                    // WLAN (WiFly) wird an SV2 genutzt (SV2 Patch erforderlich)
  char Wlan_SSID[wlan_ssid_length + 1];             // Wlan SSID
  char Wlan_Password[wlan_password_length + 1];     // Wlan Passwort
  uint8_t WLIsSet;                                  //  ## OBSOLETE ## 31.05.2014 OG: ehemals "Wlan ist initialisiert"
  uint32_t OSD_UseScreen;                           // welche OSD-Screens nutzen (Bit-codiert)
  uint8_t Wlan_DHCP;                                //  ## OBSOLETE ## 31.05.2014 OG: ehemals "0-4"
  uint8_t Wlan_WPA;                                 //  ## OBSOLETE ## 31.05.2014 OG: ehemals "0-8"
  uint8_t Wlan_Adhoc;                               //  ## OBSOLETE ## 31.05.2014 OG: ehemals "false = AP , true = Adhoc"
  uint8_t Wlan_Channel;                             // 0-13
  char Wlan_IP[wlan_ip_length + 1];                 //  ## OBSOLETE ## 31.05.2014 OG: ehemals "IP-Adresse"     (16 Bytes!)
  char Wlan_Netmask[wlan_netmask_length + 1];       //  ## OBSOLETE ## 31.05.2014 OG: ehemals "SubnetMask"     (16 Bytes!)
  char Wlan_Gateway[wlan_gateway_length + 1];       //  ## OBSOLETE ## 31.05.2014 OG: ehemals "Defaultgateway" (16 Bytes!)
  uint8_t OSD_ShowCellU;                            // OSD Azeige: Anzeige UBat als Einzelzellenspannung (ja/nein)
  uint8_t UseBLE;                                   // Bluetooth 4 LowPower wird genutzt (RedBearLab BLE Mini) wird an SV2 genutzt (SV2 Patch erforderlich)
  unsigned char MKParam_Favs[MAX_MKPARAM_FAVORITES];// Array von MK-Parameter Favoriten des Benutzers

  char Wlan_HomeSSID[wlan_ssid_length + 1];         // Wlan SSID (Home-WLAN)
  char Wlan_HomePassword[wlan_password_length + 1]; // Wlan Passwort (Home-WLAN)
  uint8_t LCD_Orientation;                          // Display Anzeige um 180 Grad drehen

  //!!!neue Parameter ab hier anfügen!!!!!!

  // ab hier werden die Parameter bei EEPROMversionsänderungen gelöscht
  Point_t PointList[MAX_LIST_LEN];                      // ab EEPROM Version 78
  osd_statistic_t OSD_Statistic;                        // ab EEPROM Version 79 & 81 OSD Statistiken
  pkt_gpspos_t GPS_User[MAX_GPS_USER];                  // ab EEPROM Version 79 speichert Benutzer GPS-Positionen
  mkerror_t MKErr_Log[MAX_MKERR_LOG];                   // ab EEPROM Version 79 speichert auftretende ErrorCode's vom MK

} ST;

extern ST Config;

void ReadParameter(void);
void WriteParameter(void);
void ReadLastPosition(void);
void WriteLastPosition(uint32_t ELongitude, uint32_t ELatitude);
void WriteWiInitFlag(void);
void WriteBTInitFlag(void);
void WriteWLInitFlag(void);
void WriteBTSlaveFlag(void);
void WriteBTMasterFlag(void);
void Delete_EEPROM(void);
void SendConfigData(void);
//void EEWayPointList_Clear(void);  // löschen der Waypointliste im EEProm

#endif
