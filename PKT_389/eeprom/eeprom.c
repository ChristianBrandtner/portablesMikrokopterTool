/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

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
//# HISTORY  eeprom.c
//#
//# 23.10.2015 Cebra
//# - add: LCD_Orientation, drehen des Displays wird wieder benötigt
//#
//# 14.10.2015 Starter
//# - chg: rename FM_Azimuth and FM_Distance to FM_Offest_lat and FM_Offset_long
//#
//# 03.08.2015 CB
//# - chg: Obsolete Parameter geändert und für FollowMe verwendet, EEprom Version bleibt gleich
//#        Parameter FM_Refresh in FM_Azimuth geändert
//#        Parameter hyst_u_min in FM_Distance geändert
//#        Initialisierung der neuen Parameter hinzugefügt
//#
//# 17.06.2014 OG
//# - chg: ReadParameter() - Updatecode auf 139
//#
//# 13.06.2014 OG
//# - chg: Delete_EEPROM(() Unterstuetzung Config.PKTOffTimeout (ehemals LCD_DisplayMode)
//#
//# 30.05.2014 OG
//# - chg: Delete_EEPROM() Config.Lipo_UOffset auf 10000 gesetzt (vorher 8500)
//#
//# 26.05.2014 OG
//# - chg: Config.LCD_DisplayMode als "OBSOLETE" markiert
//#
//# 14.05.2014 OG
//# - chg: include "../mk/paramset.h" geaendert auf "../mksettings/paramset.h"
//#
//# 07.05.2014 OG
//# - chg: EEpromversion erhoeht auf 138
//#        -> keine neuen Parameter, neue Version nur fuer geanderte
//#           Initialisierung von MKParam_Favs
//# - fix: ReadParameter() - Vorbelegung von Config.MKParam_Favs von 255 auf 0 geaendert
//#        ab EEprom-Version 138
//# - fix: Delete_EEPROM() - Vorbelegung von Config.MKParam_Favs von 255 auf 0 geaendert
//# - fix: ReadParameter() - Kommentare zu Version 136 und 137 korrigiert
//#        bzgl. der PKT-Versionsangabe und Datum ergaenzt
//#
//# 06.04.2014 OG
//# - chg: ReadParameter(): umgestellt auf lcdx_printp_center()
//# - chg: EEpromversion erhoeht auf 137
//# - add: ReadParameter(): upgrade Config.Wlan_HomeSSID, Config.Wlan_HomePassword
//# - add: ReadParameter(): upgrade Config.UseBLE, Config.MKParam_Favs
//# - add: Delete_EEPROM(): init von Config.Wlan_HomeSSID, Config.Wlan_HomePassword
//#
//# 01.04.2014 OG
//# - add: Delete_EEPROM(): init von Config.UseBLE
//# - add: Delete_EEPROM(): init von Config.MKParam_Favs
//#
//# 27.03.2014 OG
//# - chg: ReadParameter() Niederländisch wird auf Englisch umgeschaltet
//#         -> Vorbereitung zum entfernen der Niederländischen Sprache
//#
//# 11.02.2014 Cebra
//# - add: Delete EEProm: Config.OSD_ShowCellU = false;
//#
//# 03.02.2014 OG
//# - add: Config.OSD_ShowCellU in ReadParameter (EEpromversion 0x87)
//#
//# 30.01.2014 OG
//# - add: Unterstuetzung fuer USE_BLUETOOTH
//#
//# 07.07.2013 OG
//# - chg: Strings fuer WLan gekuerzt
//#
//# 04.07.2013 Cebra
//# - add: neue Parameter fuer Wlan; EEpromversion erhoeht auf 0x85
//#
//# 04.07.2013 OG
//# - add: Config.OSD_UseScreen; Epromversion erhoeht auf 0x84
//#
//# 03.07.2013 OG
//# - chg: ReadParameter() - PKT-Upgrade restrukturiert bzgl. inkrementelle
//#        Updates vorheriger Versionen
//#
//# 02.07.2013 Cebra
//# - add: neue Parameter fuer Wlan; EEpromversion erhoeht auf 83
//#
//# 02.07.2013 OG
//# - chg: frei geworden: Config.OSD_HomeMKView (nicht mehr benoetigt)
//#
//# 26.06.2013 OG
//# - chg: frei geworden: Config.PKT_StartInfo (nicht mehr benoetigt)
//#
//# 24.06.2013 OG
//# - fix: ReadParameter(): ergaenzt um bt_fixname() um ggf. ungueltige
//#        Zeichen im Bluetooth-Namen zu eliminieren
//#
//# 23.06.2013 OG
//# - chg: Delete_EEPROM(): Default von Config.Lipo_UOffset von 6000 auf 8500 gesetzt
//# - chg: Delete_EEPROM(): Default von Config.bt_name auf "PKT" gekuerzt
//# - chg: Delete_EEPROM(): Config.bt_name -> nicht mehr mit Leerzeichen auffuellen!
//#
//# 20.06.2013 CB
//# - chg: EEPROM Versionsänderung auf 82 wegen Wechsel LCD_Orientation zu OSD_ShowMKSetting
//#
//# 15.06.2013 OG
//# - chg: Config.LCD_ORIENTATION zu Config.OSD_ShowMKSetting
//#
//# 13.06.2013 OG
//# - fix: Config.PKT_Accutyp hinzugefuegt in Delete_EEPROM()
//# - chg: Default GPS-LastPosition auf 0/0 gesetzt in Delete_EEPROM()
//# - chg: Default Config.PKT_StartInfo auf false gesetzt in Delete_EEPROM()
//# - chg: Code Layout
//#
//# 31.05.2013 CB
//# - chg: EEPROM Strukturänderung auf Version 81, Versionsanpassung angepasst
//#
//# 05.05.2013 Cebra
//# - add: PKT Zeitsetup EEPROM Parameter
//# 28.03.2013 CB
//# - add: save and upgrade OSD_Statistic, GPS_User, MKErr_Log in EEProm structure variable
//############################################################################
#include "../cpu.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdbool.h>
#include <avr/wdt.h>
#include "../lcd/lcd.h"
#include "../main.h"
#include "../timer/timer.h"
#include "eeprom.h"
#include "../wi232/Wi232.h"
#include "../setup/setup.h"
#include "../bluetooth/bluetooth.h"
#include "../mk-data-structs.h"
#include "../connect.h"
#include "../tracking/ng_servo.h"
#include "../tracking/tracking.h"
#include "../osd/osd.h"
#include "../uart/uart1.h"
#include "../messages.h"
#include "../osd/osddata.h"
#include "../mksettings/paramset.h"

//------------------------------------------------------------------------------------------

ST EEMEM EEStruct;

ST Config;

//#############################################################
// ANMERKUNG OG:
//
// es ware besser wenn die gesamte Config-struct zum PC
// gesendet wird anstatt aufbreitete Textlines. Das spart
// Speicher im PKT (der ist limitiert).
//
// Die Aufbereitung der Daten (Umrechnungen) kann dann auf
// der PC Seite erfolgen.
//
// Nachteil dabei: wenn sich die Config-struct des PKT
// ändert muss auch das PKT-Tool auf dem PC angepasst werden.
//
//CB: deshalb ist das auch jetzt auskommentiert
//
//#############################################################

//char printbuff[100];

//void print_data_int8( const char *text, int8_t *variable)
//{
//  sprintf(printbuff, "%d", *variable);
//  uart1_puts_p(text); uart1_puts(printbuff); uart1_puts("\r\n");
//}
////
////
//void print_data_uint8( const char *text, uint8_t *variable)
//{
//  sprintf(printbuff, "%d", *variable);
//  uart1_puts_p(text); uart1_puts(printbuff); uart1_puts("\r\n");
//}
//
//
//void print_data_uint16( const char *text, uint16_t *variable)
//{
//  sprintf(printbuff, "%d", *variable);
//  uart1_puts_p(text); uart1_puts(printbuff); uart1_puts("\r\n");
//}
//
//
//void print_data_int16( const char *text, int16_t *variable)
//{
//  sprintf(printbuff, "%d", *variable);
//  uart1_puts_p(text); uart1_puts(printbuff); uart1_puts("\r\n");
//}
//
//
//void print_data_char( const char *text, char *variable, uint8_t length)
//{
//  uart1_puts_p(text);
//  uart1_puts(variable);
//  uart1_puts("\r\n");
//}
//
//
//void print_time( const char *text, PKTdatetime_t variable)
//{
//    uart1_puts_p(text);
//    sprintf(printbuff, "%d", variable.day);
//    uart1_puts(printbuff);
//    uart1_puts(".");
//    sprintf(printbuff, "%d", variable.month);
//    uart1_puts(printbuff);
//    uart1_puts(".");
//    sprintf(printbuff, "%d", variable.year);
//    uart1_puts(printbuff);
////    uart1_puts(" ");
////    sprintf(printbuff, "%d", variable.seconds);
////    uart1_puts(printbuff);
//    uart1_puts("\r\n");
//}
//
//
//
//void print_OSD_Statistic(void)
//{
//  print_time (PSTR("begin_StatTime:"),Config.OSD_Statistic.begin_StatTime);
//  print_time (PSTR("end_StatTime:"),Config.OSD_Statistic.end_StatTime);
//  print_data_uint16 (PSTR("total_FlyTime:"),&Config.OSD_Statistic.total_FlyTime);
//  print_data_uint16 (PSTR("last_FlyTime:"),&Config.OSD_Statistic.last_FlyTime);
//  print_data_uint16 (PSTR("count_Errorcode:"),&Config.OSD_Statistic.count_Errorcode);
//  print_data_int16 (PSTR("max_Altimeter:"),&Config.OSD_Statistic.max_Altimeter);
//  print_data_uint16 (PSTR("max_GroundSpeed:"),&Config.OSD_Statistic.max_GroundSpeed);
//  print_data_uint16 (PSTR("max_Distance:"),&Config.OSD_Statistic.max_Distance);
//  print_data_uint16 (PSTR("max_Current:"),&Config.OSD_Statistic.max_Current);
//  print_data_uint16 (PSTR("max_Capacity:"),&Config.OSD_Statistic.max_Capacity);
//  print_data_int16 (PSTR("max_Variometer:"),&Config.OSD_Statistic.max_Variometer);
//  print_data_int8 (PSTR("max_AngleNick:"),&Config.OSD_Statistic.max_AngleNick);
//  print_data_int8 (PSTR("max_AngleRoll:"),&Config.OSD_Statistic.max_AngleRoll);
//  print_data_uint8 (PSTR("RC Quality:"),&Config.OSD_Statistic.max_RCQuality);
//  print_data_int16 (PSTR("max_TopSpeed:"),&Config.OSD_Statistic.max_TopSpeed);
//  print_data_int8 (PSTR("min_AngleNick:"),&Config.OSD_Statistic.min_AngleNick);
//  print_data_int8 (PSTR("min_AngleRoll:"),&Config.OSD_Statistic.min_AngleRoll);
//  print_data_uint8 (PSTR("min_RCQuality:"),&Config.OSD_Statistic.min_RCQuality);
//  print_data_uint8 (PSTR("min_UBat:"),&Config.OSD_Statistic.min_UBat);
//  print_data_uint8 (PSTR("LiPoCells:"),&Config.OSD_Statistic.LiPoCells);
//  print_data_uint8 (PSTR("BL_Count:"),&Config.OSD_Statistic.BL_Count);
//}

//void SendConfigData(void)
//{

//  print_data_uint8 (strGet(START_VERSIONCHECK),&Config.Version );
//  print_data_uint8 (strGet(LOWBAT),&Config.MK_LowBat );
//  print_data_uint8 (strGet(LOWBAT),&Config.DisplayTimeout );
//  print_data_uint8 (strGet(LOWBAT),&Config.DisplayLanguage );
//  print_data_uint8 (strGet(WITXRX),&Config.WiTXRXChannel);
//  print_data_uint8 (strGet(WINETWG),&Config.WiNetworkGroup);
//  print_data_uint8 (strGet(WINETWM),&Config.WiNetworkMode);
//  print_data_uint8 (strGet(WITIMEOUT),&Config.WiTXTO);
//  print_data_uint8 (strGet(WIUART),&Config.WiUartMTU);
//  print_data_uint8 (strGet(DISPLAY8),&Config.OSD_ShowMKSetting);
//  print_data_uint8 (strGet(DISPLAY7),&Config.LCD_DisplayMode);
//  print_data_uint8 (strGet(DISPLAY6),&Config.LCD_Kontrast);
//  print_data_uint8 (strGet(DISPLAY5),&Config.LCD_Helligkeit);
//  print_data_uint8 (PSTR("USB-Betrieb:"),&Config.USBBT);
//  print_data_uint8 (PSTR("Uart-FC/Wi:"),&Config.U02SV2);
//  print_data_uint8 (PSTR("PKT-Debug:"),&Config.Debug);
//  print_data_uint8 (PSTR("Wi232 eingebaut:"),&Config.UseWi);
//  print_data_uint8 (PSTR("BTM222 eingebaut:"),&Config.UseBT);
//  print_data_uint8 (PSTR("Wi232 ok:"),&Config.WiIsSet);
//  print_data_uint8 (PSTR("BTM222 ok:"),&Config.BTIsSet);
//  print_data_uint8 (PSTR("BTM222 Slave:"),&Config.BTIsSlave);
//
//  print_data_char (PSTR("BTM222 Pin:"),&Config.bt_pin[0],bt_pin_length );
//  print_data_char (PSTR("BTM222 Name:"),&Config.bt_name[0],bt_name_length);
//  print_data_char (PSTR("BTM222 REID:"),&Config.RE_ID[0],RE_ID_length );
//  print_data_char (PSTR("BTM222 MAC:"),&Config.bt_Mac[0],bt_mac_length);
//  print_data_char (PSTR("GPS DevName:"),&Config.gps_UsedDevName[0],20);
//  print_data_char (PSTR("GPS MAC:"),&Config.gps_UsedMac[0],14);
//
//   print_data_int32 (PSTR("LastLongitude:"),&Config.LastLongitude);
////  print_data_int32(PSTR("LastLatitude:"),&Config.LastLatitude);
//
//  print_data_uint8 (PSTR("PKT_IdleBeep:"),&Config.PKT_IdleBeep);
//  print_data_uint8 (strGet(DISPLAY2),&Config.PKT_StartInfo);
//  print_data_uint16 (strGet(LIPO3),&Config.Lipo_UOffset);
//  print_data_uint8 (strGet(LIPO2),&Config.PKT_Accutyp);
//  print_data_uint8 (strGet(DISPLAY9),&Config.OSD_RCErrorbeep);
//  print_data_uint8 (strGet(OSD_Invert_Out),&Config.OSD_InvertOut);
//  print_data_uint8 (strGet(OSD_LED_Form),&Config.OSD_LEDform);
//  print_data_uint8 (strGet(OSD_Send_OSD),&Config.OSD_SendOSD);
//  print_data_uint8 (strGet(FALLSPEED),&Config.OSD_Fallspeed);
//  print_data_uint8 (strGet(OSD_VARIOBEEP),&Config.OSD_VarioBeep);
//  print_data_uint8 (strGet(OSD_HOMEMKVIEW),&Config.OSD_HomeMKView);
//  print_data_uint16 (strGet(OSD_MAHWARNING),&Config.OSD_mAh_Warning);
//  print_data_uint8 (strGet(OSD_SCREENMODE),&Config.OSD_ScreenMode);
//  print_data_uint8 (strGet(OSD_LIPOBAR),&Config.OSD_LipoBar);
//  print_data_uint8 (strGet(PKT_BAUDRATE),&Config.PKT_Baudrate);
//  print_data_uint16 (strGet(FOLLOWME_1),&Config.FM_Refresh);
//  print_data_uint16 (strGet(FOLLOWME_2),&Config.FM_Speed);
//  print_data_uint16 (strGet(FOLLOWME_3),&Config.FM_Radius);
//  print_data_uint8 (strGet(HWSOUND),&Config.HWSound);
//  print_data_uint8 (strGet(HWBEEPER),&Config.HWBeeper);
//  print_data_uint8 (PSTR("sIdxSteps:"),&Config.sIdxSteps);
//  print_data_uint8 (strGet(SV_TEST3),&Config.servo_frame);
//  print_data_uint8 (strGet(SV_SINGLESTEP),&Config.single_step);
//  print_data_uint8 (strGet(SV_COUNTTEST),&Config.repeat);
//  print_data_uint8 (strGet(SV_PAUSEEND),&Config.pause);
//  print_data_uint8 (strGet(SV_PAUSEINC),&Config.pause_step);
////  print_data_int8 (,&Config.tracking);
////  print_data_int8 (,&Config.track_tx);
//  print_data_uint16 (PSTR("Stick1_min:"),&Config.stick_min[0]);
//  print_data_uint16 (PSTR("Stick2_min:"),&Config.stick_min[1]);
//  print_data_uint16 (PSTR("Stick3_min:"),&Config.stick_min[2]);
//  print_data_uint16 (PSTR("Stick4_min:"),&Config.stick_min[3]);
//  print_data_uint16 (PSTR("Stick5_min:"),&Config.stick_min[4]);
//  print_data_uint16 (PSTR("Stick1_max:"),&Config.stick_max[0]);
//  print_data_uint16 (PSTR("Stick2_max:"),&Config.stick_max[1]);
//  print_data_uint16 (PSTR("Stick3_max:"),&Config.stick_max[2]);
//  print_data_uint16 (PSTR("Stick4_max:"),&Config.stick_max[3]);
//  print_data_uint16 (PSTR("Stick5_max:"),&Config.stick_max[4]);
//  print_data_uint8 (PSTR("Stick1_typ:"),&Config.stick_typ[0]);
//  print_data_uint8 (PSTR("Stick2_typ:"),&Config.stick_typ[1]);
//  print_data_uint8 (PSTR("Stick3_typ:"),&Config.stick_typ[2]);
//  print_data_uint8 (PSTR("Stick4_typ:"),&Config.stick_typ[3]);
//  print_data_uint8 (PSTR("Stick5_typ:"),&Config.stick_typ[4]);
//  print_data_uint8 (PSTR("Stick1_neutral:"),&Config.stick_neutral[0]);
//  print_data_uint8 (PSTR("Stick2_neutral"),&Config.stick_neutral[1]);
//  print_data_uint8 (PSTR("Stick3_neutral:"),&Config.stick_neutral[2]);
//  print_data_uint8 (PSTR("Stick4_neutral:"),&Config.stick_neutral[3]);
//  print_data_uint8 (PSTR("Stick5_neutral:"),&Config.stick_neutral[4]);
//  print_data_uint8 (strGet(LIPO_MESSUNG),&Config.Lipomessung);
//  print_OSD_Statistic();
//}

//--------------------------------------------------------------
//--------------------------------------------------------------
void
WriteWiInitFlag(void)
{
  Config.WiIsSet = true;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void
WriteBTInitFlag(void)
{
  Config.BTIsSet = true;
}
//--------------------------------------------------------------
//--------------------------------------------------------------
void
WriteWLInitFlag(void)
{
  Config.WLIsSet = true;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void
WriteBTSlaveFlag(void)
{
  Config.BTIsSlave = true;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void
WriteBTMasterFlag(void)
{
  Config.BTIsSlave = false;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void
WriteLastPosition(uint32_t ELongitude, uint32_t ELatitude)

{
  Config.LastLongitude = ELongitude;
  Config.LastLatitude = ELatitude;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void
ReadParameter(void)
{
  eeprom_read_block((void*) &Config, (const void*) &EEStruct, sizeof(ST));

  // DEBUG !!
  //Config.Version = 0x82;
  //WriteParameter();
  //return;

  //-------------------
  // ggf. ungueltige Zeichen aus dem
  // Bluetooth-Namen eliminieren
  //-------------------
#ifdef USE_BLUETOOTH
  bt_fixname();
#endif

  //-------------------
  // 27.03.2014 OG
  // Als Vorbereitung die Niederländische Sprache
  // zu entfernen wird hier ggf. Niederländisch
  // auf Englisch umgeschaltet
  //-------------------
  if (Config.DisplayLanguage == 2)                      // 2 = ehemals Niederländisch
    Config.DisplayLanguage = 1;                         // -> wird zu 1 = Englisch

  //-------------------
  // 0. EEprom-Version nicht geaendert
  //-------------------
  if (Config.Version == EEpromVersion)                   // nichts zu tun...
    {
      return;                                            // !!! EXIT !!!
    }

  //-------------------
  // 1. PKT-Upgrade NICHT Update faehig
  //    ODER Downgrade
  //
  // 0x77 = PKT Version 3.6.6a
  //-------------------
  if ((Config.Version < 0x77) || (Config.Version > EEpromVersion))
    {
      Delete_EEPROM();
      return;                                             // !!! EXIT !!!
    }

  //-------------------
  // 2. PKT-Upgrade
  //-------------------

  //++++++++++++++++++++++++++++
  // bei Eeprom-Versionwechsel
  // IMMER loeschen
  //++++++++++++++++++++++++++++
#ifdef USE_WAYPOINTS
  PointList_Clear();                                      // Init Waypoints
#endif
  STAT_Init();                                            // Init OSD Statistik
  GPS_User_Init();                                        // Init GPS Positionen
  MKErr_Log_Init();                                       // Init MK Errorlog

  //++++++++++++++++++++++++++++
  // inkrementelle Updates
  //++++++++++++++++++++++++++++

  //-----------------------------
  //--- ab PKT v3.6.9bX6
  //-----------------------------
  if (Config.Version < 0x81)
    {
      Config.timezone = 1;
      Config.summertime = 1;
    }

  //-----------------------------
  //--- ab PKT v3.7.0b
  //-----------------------------
  if (Config.Version < 0x82)
    {
      Config.OSD_ShowMKSetting = true;
    }

  //-----------------------------
  //--- ab PKT v3.7.0e
  //-----------------------------
  if (Config.Version < 0x83)
    {
      Config.UseWL = false;
      Config.WLIsSet = false;
      strcpy_P(Config.Wlan_SSID, PSTR("PKT"));                  // Wlan
      strcpy_P(Config.Wlan_Password, PSTR("12345678"));         // Wlan
    }

  //-----------------------------
  //--- ab PKT v3.7.0eX1
  //-----------------------------
  if (Config.Version < 0x84)
    {
      Config.OSD_UseScreen = 0xffffffff;                        // alle verfuegbaren OSD-Screens eingeschaltet
    }

  //-----------------------------
  //--- ab PKT v3.7.0f
  //-----------------------------
  if (Config.Version < 0x85)
    {
      Config.Wlan_DHCP = 0;
      Config.Wlan_WPA = 0;
      Config.Wlan_Adhoc = true;
      Config.Wlan_Channel = 1;
      strcpy_P(Config.Wlan_IP, PSTR("169.254.001.001"));
      strcpy_P(Config.Wlan_Netmask, PSTR("255.255.000.000"));
      strcpy_P(Config.Wlan_Gateway, PSTR("169.254.001.001"));
    }

  //-----------------------------
  //--- ab PKT v3.7.3cX5
  //-----------------------------
  if (Config.Version < 0x87)
    {
      Config.OSD_ShowCellU = false;                      //
    }

  //-----------------------------
  //--- 01.04.2014 OG
  //--- ab PKT v3.7.4aX7
  //-----------------------------
  if (Config.Version < 136)
    {
      Config.UseBLE = false;                                            // Bluetooth 4 LowPower wird genutzt (RedBearLab BLE Mini) wird an SV2 genutzt (SV2 Patch erforderlich)
      memset(Config.MKParam_Favs, param_EOF, MAX_MKPARAM_FAVORITES);    // Array von MK-Parameter Favoriten des Benutzers
    }

  //-----------------------------
  //--- 06.04.2014 OG
  //--- ab PKT v3.7.4aX8
  //-----------------------------
  if (Config.Version < 137)
    {
      strcpy_P(Config.Wlan_HomeSSID, PSTR(""));                 // WiFly Home-Wlan: SSID (Home-WLAN)
      strcpy_P(Config.Wlan_HomePassword, PSTR(""));             // WiFly Home-Wlan: Passwort (Home-WLAN)
    }

  //-----------------------------
  //--- 07.05.2014 OG
  //--- ab PKT v3.80bX3
  //-----------------------------
  if (Config.Version < 138)
    {
      memset(Config.MKParam_Favs, 0, MAX_MKPARAM_FAVORITES); // Array von MK-Parameter Favoriten des Benutzers (Vorbelegung jetzt 0)
    }

  //-----------------------------
  //--- 17.06.2014 OG
  //--- ab PKT v3.80cX5
  //-----------------------------
  if (Config.Version < 139)
    {
      Config.PKTOffTimeout = 0;                                 // autom. PKT ausschalten nach n Minuten (0=immer an)
    }

  //-----------------------------
  //--- 23.10.2015 Cebra
  //--- ab PKT v3.85h
  //-----------------------------
  if (Config.Version < 140)
    {
      Config.LCD_Orientation = 0;                               // LCD Anzeige Richtung normal
    }

  //-------------------
  // 3. Update Message
  //-------------------
  Config.Version = EEpromVersion;                               // Update EEPROM version number
  sei();

  lcd_cls();
  lcdx_printp_center(2, PSTR("EEProm updated to"), MNORMAL, 0, 0);
  lcdx_printp_center(3, PSTR("new Version"), MNORMAL, 0, 0);
  lcdx_printp_center(5, PSTR("check settings!"), MNORMAL, 0, 0);
  lcd_printp_at(18, 7, PSTR("OK"), MNORMAL);         // Keyline

  while (!get_key_press(1 << KEY_ENTER))
    ;
  WriteParameter();
  cli();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void
WriteParameter(void)
{
  copy_line(7);
  lcd_printp_at(0, 7, PSTR("   Write EEPROM    "), MNORMAL);
  eeprom_update_block((const void*) &Config, (void*) &EEStruct, sizeof(ST));
  paste_line(7);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void
Delete_EEPROM(void)
{
  uint8_t i;

  // EEPROM auf Default setzen

  lcd_cls();
  lcd_printp_at(0, 0, PSTR(" EEPROM Parameter    "), MINVERS);
  lcd_printp_at(0, 1, PSTR("werden auf"), MNORMAL);
  lcd_printp_at(0, 2, PSTR("Standardwerte gesetzt"), MNORMAL);

  Config.MK_LowBat = 137;                                       // 13,7V
  Config.DisplayTimeout = 0;                                    // Display immer an (autom. LCD ausschalten nach n Minuten)
  Config.DisplayLanguage = 5;                                   // default undefined
  Config.WiTXRXChannel = 1;                                     // Kanal 1 MK Standard
  Config.WiNetworkGroup = 66;                                   // Gruppe 66  MK Standard
  Config.WiNetworkMode = NetMode_Normal;                        // MK Standard
  Config.WiTXTO = TWaitTime16;                                  // MK Standard
  Config.WiUartMTU = UartMTU64;                                 // MK Standard
  Config.OSD_ShowMKSetting = true;                              // Anzeige MK-Setting beim OSD Start
  Config.PKTOffTimeout = 0;                                     // PKT immer an (autom. PKT ausschalten nach n Minuten)
  Config.LCD_Kontrast = 20;                                     // Kontrast normal
  Config.LCD_Helligkeit = 100;                                  // Helligkeit in %
  Config.USBBT = 0;                                             // USB Betrieb
  Config.U02SV2 = 0;                                            // SV2 (Kabel) Standard
  Config.Debug = 0;                                             // kein Debug
  Config.UseWi = false;                                         // Wi.232 eingebaut?
  Config.UseBT = false;                                         // BT-222 eingebaut?
  Config.WiIsSet = false;                                       // Flag für die Initialisierung Wi232
  Config.BTIsSet = false;                                       // Flag für die Initialisierung Bluetooth
  Config.BTIsSlave = true;                                      // Slave Flag setzen
  Config.PKT_IdleBeep = 0;                                      // kein Piepsen bei Inaktivität
  Config.PKT_StartInfo = false;                                 //   * FREI * (ehemals: PKT Startinfo anzeigen)
  Config.OSD_RCErrorbeep = true;                                // OSD Receiveerrorbeep
  Config.OSD_InvertOut = false;                                 // LED Anzeige invertiren
  Config.OSD_LEDform = 1;                                       // Form der Anzeige ( + oder schwarz)
  Config.OSD_SendOSD = false;                                   // OSD Daten an SV2
  Config.OSD_Fallspeed = 40;                                    // maximale Sinkrate
  Config.OSD_VarioBeep = 1;                                     // Vario Beep ein
  Config.OSD_HomeMKView = true;                                 //   * FREI * (ehemals: Home Circle from MK View)
  Config.OSD_mAh_Warning = 10000;                               // mAh Warnschwelle
  Config.OSD_ScreenMode = 0;                                    // Variante des OSD Screen
  Config.OSD_LipoBar = 0;                                       // Bargraphanzeige für MK Lipo
  Config.PKT_Baudrate = Baud_57600;                             // Baudrate für BT und Wi232
  Config.PKT_Accutyp = true;                                    // verwendeter Akkutyp (true=Lipo, false=LiON)
  Config.Lipo_UOffset = 10000;                                  // Offset für PKT-Lipomessung
  Config.FM_Offest_Latitude = 0;                                // FollowMe Azimuth
  Config.FM_Offset_Longitude = 0;                               // FollowMe Distance
  Config.FM_Speed = 30;                                         // FollowMe Speed in m/s *0.1
  Config.FM_Radius = 5;                                         // Waypoint Tolerance Radius in meter
  Config.HWSound = 0;                                           // Hardware Sounderweiterung an PD7
  Config.HWBeeper = 1;                                          // Hardware Beeper an PC7
  Config.Volume = 0;                                            // Lautstaerke
  Config.LastLongitude = 0x00000000;
  Config.LastLatitude = 0x00000000;

  strcpy_P(Config.bt_pin, PSTR("0000"));
  strcpy_P(Config.bt_name, PSTR("PKT")); // Bluetooth-Name max. 10 Zeichen! - NICHT mit Leerzeichen auffüllen (bt_name_length)
  strcpy_P(Config.bt_Mac, PSTR("0000-00-000000"));
  strcpy_P(Config.RE_ID, PSTR("0000"));

  for (i = 0; i < 20; i++)
    {
      Config.gps_UsedDevName[i] = 0;                            // benutztes GPS Device Name
    }
  for (i = 0; i < 14; i++)
    {
      Config.gps_UsedMac[i] = '0';                              // benutztes GPS Device Mac Adresse
    }
  Config.gps_UseGPS = false;                                    // ist GPS aktiv?
  Config.gps_UsedGPSMouse = GPS_Bluetoothmouse1;

  //Config.WiIsSet          = false;                            // 15.07.2013 CB doppelt drin
  //Config.BTIsSet          = false;
  Config.Version = EEpromVersion;
  Config.sIdxSteps = STEPS_255;
  Config.servo[0].rev = SERVO_REV;
  Config.servo[0].min = SERVO_I0_RIGHT;
  Config.servo[0].max = SERVO_I0_LEFT;
  Config.servo[0].mid = SERVO_I0_MIDDLE;
  Config.servo[1].rev = SERVO_REV;
  Config.servo[1].min = SERVO_I0_RIGHT;
  Config.servo[1].max = SERVO_I0_LEFT;
  Config.servo[1].mid = SERVO_I0_MIDDLE;
  Config.servo_frame = SERVO_PERIODE;
  Config.single_step = SINGLE_STEP;                             // nur bei Test-Servo
  Config.repeat = REPEAT;                                       // nur bei Test-Servo
  Config.pause = PAUSE;                                         // nur bei Test-Servo
  Config.pause_step = PAUSE_STEP;                               // nur bei Test-Servo
  Config.tracking = TRACKING_MIN;
  Config.track_hyst = TRACKING_HYSTERESE;
  Config.track_tx = 0;

  for (i = 0; i < 5; i++)
    {
      Config.stick_min[i] = 30 + i;                             // Joystick
      Config.stick_max[i] = 270 + i;                            // Joystick
      Config.stick_typ[i] = 0;                                  // Joystick
      Config.stick_dir[i] = 0;                                  // Joystick
      Config.stick_neutral[i] = 0;                              // Joystick
    }

  Config.Lipomessung = true;

  Config.timezone = 1;
  Config.summertime = 1;

  strcpy_P(Config.Wlan_SSID, PSTR("PKT"));                      // Wlan
  strcpy_P(Config.Wlan_Password, PSTR("12345678"));             // Wlan
  Config.WLIsSet = false;                                       // Wlan
  Config.UseWL = false;
  Config.OSD_UseScreen = 0xffffffff;                            // alle OSD-Screens eingeschaltet
  Config.Wlan_DHCP = 0;                                         // kein DHCP
  Config.Wlan_WPA = 0;                                          // 0 = adhoc 1= WPA2
  Config.Wlan_Adhoc = true;                                     // Adhoc
  Config.Wlan_Channel = 1;                                      // Wlan Channel
  strcpy_P(Config.Wlan_IP, PSTR("169.254.001.001"));
  strcpy_P(Config.Wlan_Netmask, PSTR("255.255.000.000"));
  strcpy_P(Config.Wlan_Gateway, PSTR("169.254.001.001"));
  Config.OSD_ShowCellU = false;
  Config.UseBLE = false;

  memset(Config.MKParam_Favs, 0, MAX_MKPARAM_FAVORITES);        // Favoriten MK-Parameter (Anzahl: MAX_MKPARAM_FAVORITES = 10)

  strcpy_P(Config.Wlan_HomeSSID, PSTR(""));                     // WiFly Home-Wlan: SSID (Home-WLAN)
  strcpy_P(Config.Wlan_HomePassword, PSTR(""));                 // WiFly Home-Wlan: Passwort (Home-WLAN)
  Config.LCD_Orientation = 0;                                   // LCD-Anzeige Normal
  //-------------------------------------------------
  //-------------------------------------------------
#ifdef USE_WAYPOINTS
  PointList_Clear();
#endif
  STAT_Init();                                                  // Init OSD Statistik
  GPS_User_Init();                                              // Init GPS Positionen
  MKErr_Log_Init();                                             // Init MK Errorlog

  WriteParameter();

  //lcd_printp_at (0, 4, PSTR("Waypoints loeschen"), 0);
  //EEWayPointList_Clear();

  lcd_printp_at(0, 6, PSTR("!!Check Parameter!! "), MNORMAL);
  lcd_printp_at(18, 7, PSTR("OK"), MNORMAL);
  sei();
  set_beep(200, 0x0080, BeepNormal);

  while (!get_key_press(1 << KEY_ENTER))
    ;

//#if defined HWVERSION3_9
  clr_V_On();
//#else
//
//  wdt_enable( WDTO_250MS );
//                  while (1)
//                  {;}
//#endif
}

//--------------------------------------------------------------
//
//void EEWayPointList_Clear(void)           // löschen der Waypointliste im EEProm
//{
//  uint8_t i;
//  PKTWayPoint.Waypoint.Position.Latitude = 0;
//  PKTWayPoint.Waypoint.Position.Longitude = 0;
//  PKTWayPoint.Waypoint.Position.Altitude = 0;
//  PKTWayPoint.Waypoint.Heading = 361;

//  for(i = 0; i < MAX_WPLIST_LEN; i++)
//  {
//      PKTWayPointDirectory.WPList.WPDirectory[i] = 0;
//  }

//  for(i = 0; i < NumberOfWaypoints; i++)
//  {
//      lcd_printp (PSTR("."), 0);
//      eeprom_write_byte (&EEWayPointList[i].WPIndex, i);
//      eeprom_write_byte (&EEWayPointList[i].Waypoint.Position.Status, INVALID);
//      eeprom_write_block ((const void*)&PKTWayPoint.Waypoint.Position.Latitude, (void*)&EEWayPointList[i].Waypoint.Position.Latitude, sizeof(EEWayPointList[i].Waypoint.Position.Latitude));
//      eeprom_write_block ((const void*)&PKTWayPoint.Waypoint.Position.Longitude, (void*)&EEWayPointList[i].Waypoint.Position.Longitude, sizeof(EEWayPointList[i].Waypoint.Position.Longitude));
//      eeprom_write_block ((const void*)&PKTWayPoint.Waypoint.Position.Altitude, (void*)&EEWayPointList[i].Waypoint.Position.Altitude, sizeof(EEWayPointList[i].Waypoint.Position.Altitude));
//      eeprom_write_block ((const void*)&PKTWayPoint.Waypoint.Heading, (void*)&EEWayPointList[i].Waypoint.Heading, sizeof(EEWayPointList[i].Waypoint.Heading));
//
//      eeprom_write_byte (&EEWayPointList[i].Waypoint.ToleranceRadius, 0);     // in meters, if the MK is within that range around the target, then the next target is triggered
//      eeprom_write_byte (&EEWayPointList[i].Waypoint.HoldTime, 0);            // in seconds, if the was once in the tolerance area around a WP, this time defines the delay before the next WP is triggered
//      eeprom_write_byte (&EEWayPointList[i].Waypoint.Type, POINT_TYPE_INVALID);
//      eeprom_write_byte (&EEWayPointList[i].Waypoint.Event_Flag, 0);          // future implementation
//      eeprom_write_byte (&EEWayPointList[i].Waypoint.AltitudeRate, 0);        // no change of setpoint
//  }

//  for(i = 0; i < NumberOfWPLists; i++)
//  {
//      lcd_printp (PSTR("."), 0);
//      eeprom_write_byte (&EEWPDirectory[i].WPList.WPListnumber, i);
//      eeprom_write_byte (&EEWPDirectory[i].WPList.WPListAktiv, false);
//      eeprom_write_byte (&EEWPDirectory[i].WPList.POI_CAM_NICK_CTR, 0);
//      eeprom_write_byte (&EEWPDirectory[i].WPList.UsePOI, 0);
//      eeprom_write_block ((const void*)&PKTWayPointDirectory.WPList.WPDirectory, (void*)&EEWPDirectory[i].WPList.WPDirectory, sizeof(EEWPDirectory[i].WPList.WPDirectory));
//
//  }
//  lcd_printp (PSTR("\r\n"), 0);

//}
