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
//# HISTORY  messages.h
//#
//# 17.11.2015 Cebra
//# - add: neuer Text STR_NOCHANGE
//#
//# 08.08.2015 cebra
//# - add: STR_WAITNMEA
//#
//# 27.06.2014 OG
//# - add: STR_BT_SEARCHTIME, STR_METERS, STR_BT_LOSTDATA
//#
//# 25.06.2014 OG
//# - add: STR_SEARCH_BT_ASK, STR_SEARCH_BT, STR_NO_BT_FOUND
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
//# 06.05.2014 OG
//# - add: STR_MENUCTRL_DELITEM, STR_MENUCTRL_DELASK, STR_MENUCTRL_NOTPOSSIBLE
//# - del: STR_FAV_DELETE
//#
//# 03.05.2014 OG
//# - add: STR_FAV_ADD, STR_FAV_DELETE, STR_FAV_FULL, STR_FAV_EXIST, STR_FAV_NOTEXIST
//#
//# 28.04.2014 OG
//# - add: STR_LONGPRESS
//#
//# 04.04.2014 OG
//# - add: STR_SEARCH, STR_FOUND, STR_SET
//#
//# 03.04.2014 OG
//# - add: STR_INITIALIZE
//#
//# 01.04.2014 OG
//# - add: BLE_EXIST, MODULE_EXIST
//#
//# 30.03.2014 OG
//# - chg: NUM_LANG von 3 auf 2 reduziert (Hollaendisch vollstaendig geloescht)
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
//# - add: STR_VON, STR_NACH
//#
//# 17.02.2014 OG
//# - add: EDIT_SETTING
//# - add: STR_ERROR, ERROR_NODATA, MSG_LOADSETTINGS, MSG_ACTIVATESETTING
//#
//# 12.02.2014 OG
//# - del: START_MSG2
//# - add:: NOMKFOUND
//#
//# 04.02.2014 OG
//# - add: CHANGENORMREV1, CHANGENORMREV2
//#
//# 03.02.2014 OG
//# - add: SHOWCELLU
//#
//# 02.02.2014 OG
//# - del: START_LASTPOSDEL
//#
//# 24.01.2014 OG
//# - add: MSG_WARNUNG, UPDATE3
//#
//# 15.07.2013 Cebra
//# - add:  Wlan Security erweitert
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
//# - chg: START_VERSIONCHECK5 zu MSG_FEHLER2
//#
//# 15.06.2013 OG
//# - add: OSD_MKSetting
//#
//# 13.06.2013 CB
//# - add: ENUM Texte für OSD Statistik erweitert
//#
//############################################################################

#ifndef MESSAGES_H
#define MESSAGES_H

//---------------------------------------------------------------------------------------------------------------------
// Typdefinitionen für alle verwendeten Strings, LAST_STR muss am Ende stehen bleiben
typedef enum
{
     KEYLINE1, KEYLINE2, KEYLINE3, KEYLINE4, KEYLINE5, KEYCANCEL, BOOT1, BOOT2,
     START_LASTPOS, START_LASTPOS1, START_LASTPOS2, START_LASTPOS3,
     START_SEARCHFC, ENDE, OK, FEHLER, AKTIV, STR_SAVE, STR_SAVING, STR_DISCARD, STR_COPY, STR_SEARCH,
     STR_FOUND, STR_SET, STR_WITH, STR_WITHOUT, STR_SWITCHMOTOROFF,
     ON, OFF, ESC, SHUTDOWN, YESNO, NOYES,DELETEDATA, UPDATE1, UPDATE2, UPDATE3, ENDSTART, CONNECT13,
     KABEL, SLAVE, NORMAL, REVERSE, ENDOK, EEPROM1, EEPROM2, DEUTSCH, ENGLISCH,
     YES, NOO, OSD_3D_V, OSD_3D_H, OSD_3D_L,
     OSD_3D_R, OSD_3D_NICK, OSD_3D_ROLL, OSD_ERROR, PARA_AKTIVI, PARA_COPY, PARA_COPYQ,
     GPS2, STATS_ITEM_0, STATS_ITEM_1, STATS_ITEM_2, STATS_ITEM_3, STATS_ITEM_4, STATS_ITEM_5, STATS_ITEM_6,
     POTI, TASTER, STAT_OSDBL, STAT_ERROR, STAT_GPS, STAT_POS, STAT_ALL, DELETE,
     STR_OSDSCREEN_GENERAL, STR_OSDSCREEN_NAVIGATION, STR_OSDSCREEN_WAYPOINTS, STR_OSDSCREEN_ELECTRIC, STR_OSDSCREEN_MKSTATUS, STR_OSDSCREEN_USERGPS, STR_OSDSCREEN_3DLAGE,
     STR_OSDSCREEN_STATISTIC, STR_OSDSCREEN_OSD0, STR_OSDSCREEN_OSD1, STR_OSDSCREEN_OSD2, NOMKFOUND,
     STR_ERROR, ERROR_NODATA, MSG_LOADSETTINGS, MSG_ACTIVATESETTING, EDIT_SETTING,
     STR_VON, STR_NACH, STR_PKT, STR_ACTIVE, MODULE_EXIST, STR_INITIALIZE, STR_LONGPRESS,
     STR_FAV_ADD, STR_FAV_FULL, STR_FAV_EXIST, STR_FAV_NOTEXIST, STR_MENUCTRL_DELITEM, STR_MENUCTRL_DELASK, STR_MENUCTRL_NOTPOSSIBLE,
     STR_HELP_PKTOFFTIME1, STR_HELP_LOWBAT1, STR_HELP_LOWBAT2, STR_HELP_LIPOOFFSET1, STR_HELP_LIPOOFFSET2,
     STR_DATA, STR_READING, STR_LABELS, STR_EXTSV2MODULE, STR_ATTENTION, STR_SWITCHOFFMK, STR_WI232CONFIG, STR_USBCONNECTED, STR_SEEMKWIKI,
     STR_PKT_SWITCHOFF_NOW, STR_PKT_RESTART_NOW, STR_WI232_ACTIVATE,
     STR_WAITSATFIX, STR_GPSMOUSECONNECT, STR_GPSMOUSEDISCONNECT, STR_NOBTM222, STR_NOGPSMOUSE, STR_GPSMOUSECONNECTION, STR_SEARCH_BT_ASK,
     STR_SEARCH_BT, STR_NO_BT_FOUND, STR_BT_DEVICES, STR_BT_SEARCHTIME, STR_BT_LOSTDATA, STR_METERS,STR_WAITNMEA, STR_NOCHANGE,
     LAST_STR
} STR;


#define NUM_LANG 2  // German, English


const char * strGet( int str_no);
const char * strGetLanguage( char const *str_de, char const *str_en);

void Test_Language (void);   // bleibt für Tests



#endif /* _MESSAGES_H_ */
