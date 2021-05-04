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
 *   Oliver Gemesi   "olli42" for additions und changes in OSDScreen         *
 *****************************************************************************/


//--------------------
// Debug
//--------------------
//#define DEBUG_OSD_TIME            // mit dem NC-Simulator koennen keine brauchbaren Zeit/Datum Daten ermittelt werden
                                    //  am Anfang von osd() wird mit dieser Einstellung eine gueltige Fake-Zeit/Date gesetzt
//#define DEBUG_OSD_STAT_MOTORRUN   // erzwingt Statistik Aufzeichnung auch wenn die Motoren nicht laufen

//############################################################################
//# HISTORY  osd.c
//#
//# 05.04.2015 Cebra
//# - chg: SendOutData( 'h', ADDRESS_ANY, 2, &mkdisplayCmd, 1, 0x00,1) ergänzt um 2. Parameter wegen Fehlfunktion mit NC 2.09h
//#
//# 21.06.2014 OG
//# - chg: osd() - bei auftreten von mk_timeout wieder OSD_MK_Connect(MK_CONNECT)
//#        aktiviert -> dadurch wird u.a. wieder auf die NC umgeschaltet falls
//#        etwas anderes (z.B. ein anderes PKT) auf die FC umgeschaltet hat
//# - add: draw_icon_mk()
//#
//# 18.06.2014 OG
//# - add: MKLiPoCells_Init(), MKLiPoCells_Check()
//# - chg: define ORIENTATION_H, ORIENTATION_V verschoben nach osd.h
//#
//# 02.06.2014 OG
//# - fix: Beep_Waypoint() Target-Reach Beep auch bei Single-WP's (WP-Listen
//#        die nur aus einem WP bestehen)
//#
//# 01.06.2014 OG
//# Beginn von Auslagerungen von Code alter OSD-Screens nach osdold_screens.c/h
//# - add: include "../osd/osdold_screens.h"
//# - add: include "../osd/osdold_messages.h"
//# - chg: osd() - Check bzgl. NC-Hardware entfernt da das bereits durch das
//#        aufrufende Menue erledigt wird
//# - add: OSD_Screen_MKDisplay() um Beep_Waypoint() ergaenzt
//# - chg: OSD_Screen_Waypoints() umgestellt auf Beep_Waypoint()
//# - add: Beep_Waypoint()
//#
//# 31.05.2014 OG
//# - chg: OSD_Screen_MKDisplay() - umgestellt auf PKT_KeylineUpDown()
//#
//# 26.05.2014 OG
//# - add: etliche Aenderungen/Erweiterung am Screen "Waypoints" (auch neues Layout)
//#
//# 24.05.2014 OG
//# - fix: OSD_Screen_Waypoints(), OSD_Screen_MKDisplay() - Anzeige des akt. WP-Index
//# - chg: OSD_Screen_3DLage() - optisches Facelift
//# - fix: OSD_Screen_3DLage() - es wurden ggf. Kreise gezeichnet die ungueltig
//#        und in undefinierten Speicher gingen - wurde korrigiert
//# - chg: OSD0,1,2 angepasst an OSD_Element_CompassDirection() mit xoffs,yoffs
//# - chg: OSD_Element_CompassDirection() - erweitert um xoffs,yoffs
//#
//# 20.05.2014 OG
//# - chg: OSD_Element_Flag_Label() - Anpassung zur Rahmenbestimmung eines Flags
//#
//# 19.05.2014 OG
//# - fix: osd() - Tastensteuerung bei MK-Empfangsverlust
//# - chg: osd() - wenn MK-Setting nicht ermittelt werden konnte dann exit
//# - add: OSD_Screen_MKDisplay() - Anzeige von akt. Waypoint und Anzahl der Waypoints
//# - chg: OSD_Popup_MKSetting() - gibt true/false fuer Ok zurueck und Beep bei Fehler
//# - chg: OSD_Popup_MKSetting() - timeout von 15 auf 9 reduziert
//# - chg: OSD_Popup_MKSetting(), OSD_Popup_MKError() - Layout, Multi-Sprachenunterstuetzung
//#
//# 02.05.2014 OG
//# - del: Popup_Draw() (jetzt in lcd.c)
//#
//# 02.05.2014 OG
//# - chg: kleine Aenderung an Codereihenfolge von MkError_Save() wegen Compiler-Warning
//#
//# 28.04.2014 OG
//# Anmerkung OSD-MK-Display: wenn der MK-Display Modus eingeschaltet wird, dann werden die Display Daten
//# angefordert und die BL-Daten etwas reduziert. Hin und wieder kann es passieren das ein Tastendruck im
//# OSD-MK-Display-Modus nicht durch kommt (aber nicht allzu stoerend) - kann evtl. verbessert werden wenn
//# auch die OSD-Daten reduziert werden (waere jedoch einiges mehr Aufwand).
//# - Unterstuetzung von MK-Display (nur NC-Modus) im OSD
//#   (einschalten mit langem Druck dritte Taste von links, abschalten ebenso)
//# - Info-Anzeige bzgl. langem Tastendruck fuer MK-Display erweitert und deutsche Uebersetzung von "long press"
//# - Umstrukturierung von osd.c fuer MK-Display
//# - Aenderungen/Erweiterungen an verschiedenen weiteren Funktionen wie OSD_MK_GetData() fuer MK-Display
//# - add: OSD_MK_ShowTimeout() - neues Design/Layout
//# - del: OSD_Timeout() (ersetzt durch OSD_MK_ShowTimeout())
//# - chg: verschiedene Aenderungen an Timings
//# - add: neuer Timer fuer MK-Display: timer_get_displaydata
//# - fix: es gab bei einem Datenpaket-Timer einen Konflikt mit 'timer' - ein neuer Timer wurde aktiviert (timer_mk_timeout)
//#
//# 22.04.2014 OG
//# - add: OSD_Element_HomeCircleX() - soll das alte OSD_Element_HomeCircle() ersetzen, wenn
//#        die alten OSD-Screens endgueltig rausfliegen
//# - chg: Aenderung Anzeige OSD_Screen_UserGPS() (abgerundete Ecken und Code Einsparung)
//# - chg: Aenderung Anzeige OSD_Screen_MKStatus() (abgerundete Ecken und Code Einsparung)
//# - chg: Aenderung Anzeige OSD_Screen_Electric() (abgerundete Ecken und Code Einsparung)
//#
//# 01.03.2014 OG
//# - chg: OSD_Popup_MKError() auf lcdx_printp_center() umgestellt
//#
//# 16.02.2014 OG
//# - chg: OSD_Popup_MKSetting() umgestellt auf MK_Setting_load()
//#
//# 13.02.2014 OG
//# - chg: OSD_MK_GetData() prueft mit MKVersion_Cmp() auf NC-Version v0.30g
//#        ob GPS-Zeit-Datenpakete ('t') von der NC angefordert werden koennen
//#
//# 12.02.2014 OG
//# - chg: define MKVERSIONnnn rund um OSD_MK_UTCTime() entfernt
//#        -> TODO OG: auf neue MKVersion bzgl. Versionsprüfung anpassen!
//#        -> keine Unterstuetzung mehr durch zu alte NC-Versionen
//# - del: die includes zu den alten parameter... entfernt
//#
//# 10.02.2014 OG
//# - chg: OSD_Popup_MKSetting() umgestellt auf MKVersion_Setting_print()
//#
//# 08.02.2014 OG
//# - chg: OSD_Popup_MKSetting() umgestellt auf neue MKVersion Struktur
//#
//# 04.02.2014 OG
//# - fix: OSD_Element_BattLevel2() Aufruf von writex_ndigit_number_u_100th()
//#
//# 03.02.2014 OG
//# - chg: OSD_Element_BattLevel2() unterstuetzt Config.OSD_ShowCellU zur Anzeige
//#        der MK-Spannung als Einzelzelle (kalk. auf Basis der ermittelten Zellenzahl)
//#
//# 29.01.2014 OG
//# - add: neue MK-Errorcodes 32,33,34
//# - add: #include "mk/mkbase.h"
//#
//# 24.01.2014 OG
//# - chg: OSD_Popup_MKSetting(): MK-Settings wird beim Start vom OSD wieder angezeigt
//#        auch wenn die falsche FC-Revision vorhanden ist mit folgender Regel:
//#        a) wenn falsche FC-Settings-Revision dann nur die Nummer
//#        b) wenn richtige FC-Revision dann mit Nummer und Namen
//#
//# 06.01.2014 Cebra
//# - add: MK-Settingsname wird bei falscher FC Version nicht angezeigt
//#
//# 07.07.2013 OG
//# - add: OSD-Screens koennen vom Benutzer an-/ausgeschaltet werden (osd(), ScreenCtrl_Push())
//# - chg: OSD-Screen Namen jetzt in messages.c
//# - fix: MK-Error Check prueft ob die Fehlernummer gueltig ist
//# - del: alten Screen Electric (der ohne Nachkomma bei den Stroemen)
//#
//# 03.07.2013 OG
//# - chg: OSD_Popup_MKSetting() - zentrierte Anzeige des Setting-Namens
//# - chg: OSD_Popup_MKSetting() - timing
//# - chg: osd() - Screen-Umschaltung beschleunigt
//# - chg: timinigs bei OSD_MK_Connect(), osd() (werden Verbindungsfehler reduziert?)
//#
//# 02.07.2013 OG
//# - chg: CheckMKLipo() - MK-Unterspannungs-Beep nur wenn Motoren laufen
//# - del: unbenutzte Variablen
//#
//# 30.06.2013 OG
//# - add: grafische Akku-Anzeige auch fuer Screen Navigation und ElectricN
//#
//# 30.06.2013 OG
//# - add: Screen General: das Batt-Symbol zeigt grafisch den Fuellstand
//#        des MK-Lipos
//# - add: Mittelwert fuer Gesamtstrom
//# - add: calc_avg() - Mittelwertberechnung
//# - chg: Benamung Statistik-Var's von mid_* auf avg_* geaendert
//# - add: USE_OSD_PKTHOOK in osd() - aktuell noch nicht klar ob PKT_Hook()
//#        die Kommunikation stoert
//#
//# 27.06.2013 OG
//# - chg: numerische Winkelanzeige im Screen "Navigation" von -180 bis 180 Grad
//#        0 Grad = vorderer Ausleger zeigt zur Startposition
//# - fix: Homesicht des MK's (Winkelanzeige, Grafik)
//#        ab nun: keine Benutzereinstellung mehr dafuer
//#        es gilt: 0 Grad bzw. Strich nach unten -> der vordere Ausleger des MK
//#                 zeigt auf seine Startposition
//#
//# 19.06.2013 OG
//# - fix: redundante PKT-Error 40 eliminiert durch merken via old_PKTErrorcode
//#        -> hilft wenn der MK ausgeschaltet wird waehrend man in der PKT-OSD-Anzeige ist
//# - fix: last_FlyTime in osd()
//#
//# 18.06.2013 OG
//# - chg: OSD_Timeout() erweitert um Errorlogging (Code 40 = "PKT RX lost")
//# - add: Fehlerliste "PKT RX lost" (Code: 40)
//#
//# 15.06.2013 OG
//# - chg: Anzeige OSD_Popup_MKSetting() kann durch Benutzer via Setup an/ausgeschaltet werden
//#
//# 15.06.2013 OG
//# - chg: OSD_Popup_MKSetting() optimiert, Layout, fix
//# - chg: PKT_CtrlHook erstmal wieder disabled in osd() (evtl. Probleme mit Timings?)
//#
//# 13.06.2013 cebra
//# - add: Settings Popup beim Start des OSD-Screens
//#
//# 11.06.2013 OG
//# - add: Mittelwertberechnung BL-Strom [MK_GetData()]
//# - chg: Config.OSD_ScreenMode wird vor PKT_CtrlHook() gesetzt [osd()]
//#
//# 27.05.2013 OG
//# - fix: OSD_MK_UTCTime() (neue "t" Version) Problem wegen cast fuer Sekunden
//#
//# 19.05.2013 OG
//# - chg: osd() erweitert um PKT_CtrlHook() um u.a. PKT-Updates zu ermoeglichen
//#
//# 16.05.2013 OG
//# - add: USE_OSD_SCREEN_NAVIGATION, USE_OSD_SCREEN_ELECTRIC, USE_OSD_SCREEN_ELECTRIC_N,
//#        USE_OSD_SCREEN_MKSTATUS, USE_OSD_SCREEN_USERGPS, USE_OSD_SCREEN_3DLAGE,
//#        USE_OSD_SCREEN_STATISTIC
//# - add: OSD_Screen_ElectricN() - zeigt BL-Stroeme mit Nachkomma an
//# - del: define SHOW_OSD_BLCURRENT_10TH (ersetzt durch neue Screen-Variante)
//# - chg: osd() umgestellt auf OSD Screen Controler; Anpassungen dafuer an allen Screens
//# - add: Funktionen fuer Screen Controler (Screen_*())
//#
//# 15.05.2013 OG
//# - chg: OSD_Screen_Electric() zeigt BL-Strom mit Nachkomma an
//#         (einstellbar via define SHOW_OSD_BLCURRENT_10TH bzgl. neue/alte Anzeige)
//# - add: define DEBUG_OSD_STAT_MOTORRUN erzwingt Statistik Aufzeichnung auch wenn
//#        die Motoren nicht laufen
//# - fix: osd() Statistik-Ende Zeit wird nach Landung gesetzt
//# - chg: diverse Aufraeumarbeiten
//# - chg: OSD_Screen_Statistics() umgetsellt auf calc_BLmax(()
//# - add: calc_BLmax() ermittelt max. BL Current & Temp
//# - del: draw_symbol_degree(), draw_symbol_rc() - wurden ersetzt durch
//#        Zeichen im font 8x6 (SYMBOL_SMALLDEGREE, SYMBOL_RCQUALITY)
//#
//# 14.05.2013 OG
//# - add: OSD_MK_Connect() vereinheitlicht MK-Verbindungsinitialisierung
//#        und MK-Abo-Renew
//# - chg: OSD_MK_UTCTime() atomisiert via ATOMIC_BLOCK()
//# - chg: OSD_Screen_Debug_RX() verschiedene Optimierungen und Beschreibungen
//#
//# 14.05.2013 OG
//# - chg: OSD_MK_UTCTime() wird mittels define jetzt mit neuem oder mit altem
//#        Algorithmus einkompiliert.
//#        ALT bei: defined MKVERSION088n || defined MKVERSION090b || defined MKVERSION090e
//#        NEU bei: alles andere (also neuer): die neue Zeit/Datumsermittlung
//#
//# 13.05.2013 OG
//# - fix: an allen Stellen mit naviData->CompassHeading ein Modulo 360 ergaenzt
//#        um Winkelangaben der FC geradezuziehen falls der Kopter beim Mixersetup
//#        'verdreht' wurde (Problem von helitron im Forum - Winkelanzeige > 360 Grad)
//#        Ist ggf. ab Firmware NC 0.30h nicht mehr notwendig da Holger das
//#        korrigieren wollte (muesste noch geprueft werden)
//#
//# 11.05.2013 OG
//# - add: OSD_MK_UTCTime_NEW() - ab NC 0.30g (Alpha, noch nicht im Einsatz)
//#
//# 05.05.2013 OG
//# - chg: OSD_Popup_Info() & OSD_Popup_MKError() wurden vereinheitlicht
//#        mittels Popup_Draw()
//# - chg: Anzeigezeiten der Popup's etwas verkuerzt
//#
//# 03.05.2013 OG
//# - fix: OSD_MK_GetData() - griff beim lesen der BL's auf naviData fuer
//#        die Statistik - relevante naviData-Werte werden nun vor dem
//#        wechseln auf BL-Data gemerkt
//# - chg: Erfassung der Statistik Start/Ende-Zeit verbessert
//# - chg: bei Aufrufen von writex_datetime_time()/writex_datetime_time() den
//#        Parameter 'timeoffset' entfernt
//# - add: define DEBUG_OSD_TIME um eine Fake-Zeit/Datum bei Gebrauch des
//#        NC-Simulators zu setzen
//#
//# 29.04.2013 OG
//# - chg: Plausibilitaetscheck in OSD_MK_UTCTime() bzgl. des Datums
//#
//# 28.04.2013 OG
//# - chg: die alten OSD-Screens angepasst auf define USE_FONT_BIG
//#
//# 24.04.2013 OG
//# - chg: directions_p[] geaendert auf Kompatibilitaet mit Atmel Studio 6
//#
//# 21.04.2013 OG
//# - chg: define AltimeterAdjust nach osd.h verschoben
//#
//# 14.04.2013 OG
//# - add: weitere Statistik-Aufzeichnungen
//# - fix: MK_GetData() - Statistik-Aufzeichnung nur fuer wirklich vorhandene BL's
//#
//# 04.04.2013 OG
//# - fix: 3 warnings
//#
//# 04.04.2013 OG
//# - chg: MK-Errortext-Anzeige umgestellt auf naviData->Errorcode
//#        weniger Datenkomminkation mit dem MK und schnelleres Ansprechverhalten
//#
//# 03.04.2013 OG
//# - chg: defines OSD_DEMO, OSD_SCREEN_DEBUG, OSD_SCREEN_OLD
//#        umbenannt nach: USE_OSD_DEMO, USE_OSD_SCREEN_DEBUG, USE_OSD_SCREEN_OLD
//#        und verschoben nach: main.h
//#
//# 28.03.2013 OG
//# - fix: MKErr_Log_Init() - es fehlte Multiplikation mit MAX_MKERR_LOG
//# - chg: Code Formatierungen
//#
//# 28.03.2013 CB
//# - add: replace OSD_Statistic, GPS_User, MKErr_Log in EEProm structure variable
//#
//# 28.03.2013 OG
//# - fix: abo_timer wieder aktiviert fuer 'o' und 'k'
//#
//# 28.03.2013 OG
//# - chg: osd_statistic_t - erweitert um diverse Werte und Timestamps (werden apeter implementiert)
//# - add: mkerror_t MKErr_Log[MAX_MKERR_LOG] - Vorbereitung um MK-Errors zu loggen
//#
//# 27.03.2013 OG
//# - add: Datum / Zeit vom MK lesen
//# - chg: Kommunikation mit dem MK optimiert
//# - chg: auf neue messages.c angepasst
//# - struct von den Statistiken auf PKTdatetime PKTdatetime_t
//# - verschiedene andere Aenderungen
//#
//# 22.03.2013 OG
//# erhebliche Aenderungen / Ergaenzungen - hier nur das Wichtigste
//# - neue Funktion "User GPS" (UGPS) - mit langem Druck auf Taste KEY_ENTER werden die aktuellen GPS-Daten abgespeichert
//#     * gespeichert wird nur wenn wenn der MK GPS-Ok meldet
//#     * wenn Ok erfolgt ein bestaetigungs Beep; wenn nicht Ok erfolgt ein Error-Beep
//# - neuer Screen "User GPS" - Anzeige der letzten der 3 UGPS-Koordinaten (intern wird mehr gespeichert)
//# - Anzeige von MK-Error Messages als Popup mit einstellbarer Anzeigezeit (kann mit Taste abgebrochen werden)
//#     * kann getestet werden z.B. durch Ausschalten der Funke (= "RC Signal lost")
//#     * siehe: http://www.mikrokopter.de/ucwiki/ErrorCodes
//# - OSD-Info ist nun ein Popup - autom. ausblenden nach einstellbarer Zeit; im Hintergrund werden weiterhin MK-Daten empfangen und ausgewertet
//# - neuer Debug-Screen "Debug RXPackages" - zeigt die Anzahl empfangener Datenpakete der verschiedenen Bereiche (um z.B. Timings einzustellen)
//# - erweiterte Kommunikation mit dem MK fuer BL-Daten und Error-Messages
//# - Timings der MK-Datenkommunikation weitreichend einstellbar durch defines
//# - Datenstrukturen fuer OSD-Statistiken und BL-Statistiken
//# - neue timer: timer_get_erdata, timer_get_bldata, timer_osd_refresh, timer_pkt_uptime
//# - mit dem define OSD_SCREEN_OLD koennen ggf. die alten OSD-Screens ausgeblendet werden (spart ein paar KByte's)
//# - verschiedene Optimierungen
//#
//# 12.03.2013 OG
//# - add: Get_BL_Data() - BL-Ctrl via NC auslesen (BETA) (siehe dort auch Kommentare im func-Header)
//# - add: neuer Screen "Electric"
//# - chg: Layout von Screen "Navigation" - Pixelverschiebungen
//# - chg: Layout von Screen "MK-Status" - Pixelverschiebungen und Anzeige 'Strom' ersetzt durch 'entn. Kapazitaet'
//# - add: in osd() LiPO-Cell Erkennung hinzugefuegt (fuer Screen "Electric")
//#
//# 10.03.2013 OG
//# - fix: doppelte Degree-Anzeige in OSD_Element_CompassDegree()
//# - add: neuer Screen "MK-Status"
//# - add: 7 neue OSD-Flags
//# - chg: Screen-Refresh Zeit via timer2 (einstellbar durch define TIME_OSD_REFRESH)
//# - chg: mit define OSD_DEBUG_SCREEN kann ein zusaetzlicher Screen verwendet werden zum testen/entwickeln
//# - del: entfernt CFG2_HEIGHT_LIMIT in OSD_Element_AltitudeControl() (bis bessere Loesung gefunden ist)
//#
//# 08.03.2013 OG
//# - del: OSD_Screen_Element() und cleanup in osd.h
//# - add: OSD_Element_UpDown() (steigend/sinken via Pfeilen)
//# - chg: OSD_Element_UpDown() in Screen "General" und "Navigation" hinzugefuegt (rechts neben der Hoehenanzeige)
//# - chg: Screen "General" die Sat-Warnung wurde auf OSD_Element_Flag(OSD_FLAG_S0) geaendert
//# - chg: Anzeige von Flag 'nicht genug GPS-Sateliten' (OSD_FLAG_S0) auf "S!" geändert
//#
//# 07.03.2013 OG
//# - Hinweis bzgl. LowBatt-Anzeige in den Screens "General" und "Navigation":
//#      Es gibt zwei unabhängige LowBatt-Warnungen.
//#      1. die PKT LowBatt-Warnung: sie arbeitet mit der im PKT hinterlegten
//#         LowBatt Spannung und stellt den Spannungswert inkl. der Einheit "V"
//#         invers dar wenn der Warnwert erreicht wurde (inkl. lautem PKT-Peepen)
//#      2. die MK-LowBatt Warnung: hierbeit wird das Flag "BA" angezeigt wenn
//#         der MK eine LowBatt Warnung sendet
//#      Dadurch hat man nun zwei verschiedene LowBatt Warnungen die man auf Wunsch
//#      verschieden einstellen kann. Bei mir ist die PKT-LowBatt etwas niedriger
//#      eingestellt als die MK-Warnung und bedeutet "es ist aller hoechste Zeit zu landen".
//#      Die Spannung der MK-LowBat ist ein wenig hoeher eingestellt und
//#      zeigt mir "es ist bald Zeit zu landen".
//# - del: Kommunikation zu FC - siehe Kommentare in osd()
//# - chg: Kommunikation zu NC - siehe Kommentare in osd()
//# - add: neuer Screen "Navigation"
//# - chg: Layout Screen "Statistics" - Einheiten um zwei Pixel nach rechts verschoben
//# - chg: Layout von Screen "General" modifiziert (u.a. xoffs,yoffs Pixelverschiebungen)
//# - add: OSD_FLAG_BA in Screen "General"
//# - add: die OSD_Element_xyz() Funktionen in osd.h aufgenommen
//# - chg: an verschiedenen Stellen die Char-Drawmode defines MNORMAL, MINVERS, usw. eingebaut
//# - del: Kompatibilitaetscode fuer "3D-Lage" ueber Hauptmenue entfernt
//# - chg: die Funktionen OSD_Element_Switch() und OSD_Element_SwitchLabel() heissen
//#        nun OSD_Element_Flag() und OSD_Element_Flag_Label()
//# - chg: die defines OSD_SWITCH_xy heissen jetzt OSD_FLAG_xy
//# - fix: letzte GPS-Koordinaten werden jetzt permanent Config.LastLatitude, Config.LastLongitude gespeichert
//#
//# 03.03.2013 OG
//# - add: delay in Mainloop von osd() um springende Werte abzudaempfen (TEST)
//# - add: Startverzoegerung der Screens bis NaviData sich etwas stabilisiert hat (TEST)
//# - add: OSD Startup Message "connecting MK..."
//# - add: 'Emergency Landing' (EL) Anzeige in Screen "General"
//# - del: OUT1/OUT2 Anzeige in Screen "General"
//# - add: RC-Quality in Screen "General"
//# - add: func: draw_symbol_rc() (alternative RC-Quality Symbol)
//# - fix: Hoehenanzeige fuer Screens "OSD0" und "OSD1"
//# - fix: OSD_Element_SwitchLabel() angepasst fuer x=0 und y=0
//# - add: OSD_Element_Switch/Label() erweitert um OSD_SWITCH_FS
//# - fix: Screen-Redraw nach OSD_MK_TIMEOUT() und anderen Fehlermeldungen
//# - chg: messages.c: STATS_ITEM_0 bis STATS_ITEM_6 angepasst (1 char kuerzer)
//# - chg: Layout von OSD_Info() - mehr background-clear und etwas kleiner
//#
//# 02.03.2013 OG
//# - chg: keine internal func in Screen's wegen warnings bei anderen
//# - del: Screen "OSD3"
//# - fix: Hoehenanzeige in Screen "General" (Minuszeichen)
//# - add: MK LowBat Warning in Screen "General"
//# - add: neues Degree Symbol (als func) in Screen General (kleiner als das Char 0x1E)
//# - add: weitere Flags in OSD_Element_Flag()
//#
//# 01.03.2013 OG
//# - Reskrukturierung Code (+ neuer OSD-Screens und einiges mehr)
//############################################################################


//############################################################################
//# HINWEISE:
//#
//# 1. define: USE_OSD_DEMO  (main.h)
//#    mit define OSD_DEMO wird ein Demo-Modus bei den neuen Screens einge-
//#    schaltet - damit werden u.a. alle Flag's angezeigt fuer Scree-Fotos
//#
//# 2. define: USE_OSD_SCREEN_DEBUG  (main.h)
//#    mit diesem define wird ein zusaetzlicher Screen "Debug" einkompiliert
//#    fuer Test / Experimente / Debug von OSD-Elementen
//#
//# 3. define: USE_OSD_SCREEN_OLD  (main.h)
//#    ein-/ ausschalten der alten OSD-Screens OSD0, OSD1, OSD2 (spart ca. 3 KByte)
//#
//# 4. Timings der MK-Datenkommunikation
//#    die Timings sind weitreichend enstellbar ueber die define im Abschnitt 'Timings'
//#    der Debug-Screen 'Debug RXPackages' informiert ueber die Anzahl eingegangener
//#    Datenpakete der verschiedenen Bereiche (OSD, BL-Daten, Error-Message)
//#
//# 5. Informationen zum Display
//#    DOG: 128 x 64 Pixel with 6x8 Font => 21 x 8
//#
//# 6. MK-Kommunikationsprotokoll Referenz
//#    http://www.mikrokopter.de/ucwiki/en/SerialProtocol?highlight=%28%28----%28-*%29%28\r%29%3F\n%29%28.*%29CategoryCoding%29#en.2BAC8-SerialCommands.Flight-Ctrl
//############################################################################

#include "../cpu.h"
#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include <util/atomic.h>

#include "../main.h"
#include "../lcd/lcd.h"
#include "../timer/timer.h"
#include "../uart/usart.h"
#include "../eeprom/eeprom.h"
#include "../messages.h"
#include "../sound/pwmsine8bit.h"
#include "../mk-data-structs.h"
#include "../pkt/pkt.h"
#include "../osd/osd.h"
#include "../utils/xutils.h"
#include "../mk/mkbase.h"
#include "../osd/osdold_messages.h"
#include "../osd/osdold_screens.h"


//--------------------
// Funktionen ein-/ausbinden
//--------------------
// -> siehe main.h


//--------------------
// Timings
//--------------------
#define TIME_OSD_REFRESH      45  // Screen Refresh (Steuerung via 'timer_osd_refresh') (n*10 = ms; 100 entspricht 1 Sekunde)
#define TIME_POPUP_INFO       400 // 4 Sekunden Popup-Info zeigen (kann mit Taste abgebrochen werden)
#define TIME_POPUP_MKERROR    700 // 7 Sekunden Popup-MK-Error zeigen (kann mit Taste abgebrochen werden)

#define TXINTERVAL_OSDDATA    10  // Intervall mit der der MK OSD-Daten senden soll (n*10 = ms)

#define TIME_GET_BLDATA       35  // Zeitintervall in der BL-Daten vom MK geholt werden (Steuerung via 'timer_get_bldata') (n*10 = ms; 100 entspricht 1 Sekunde)
#define TIME_READ_BLDATA      20  // fuer n Zeit werden BL-Daten vom MK gelesen (Steuerung via timer) (n*10 = ms; 100 entspricht 1 Sekunde))
#define TXINTERVAL_BLDATA     7   // Intervall mit der der MK BL-Daten senden soll (n*10 = ms)

#define TIME_GET_TIDATA     12000 // alles 120 Sekunden Zeit/Datum vom MK lesen (die Zwischenzeit wird von einem PKT-Timer uebernommen)

#define TIME_GET_DISPLAYDATA  20  // fuer n Zeit werden BL-Daten vom MK gelesen (Steuerung via timer) (n*10 = ms; 100 entspricht 1 Sekunde))


//#define MK_TIMEOUT          300 // MK-Verbindungsfehler wenn fuer n Zeit keine gueltigen Daten hereinkommen (3 sec)
#define MK_TIMEOUT            400 // MK-Verbindungsfehler wenn fuer n Zeit keine gueltigen Daten hereinkommen (4 sec)


//--------------------
// weiteres
//--------------------
#define OSD_POPUP_NONE      0
#define OSD_POPUP_INFO      1
#define OSD_POPUP_MKERROR   2

#define MK_CONNECT          1
#define MK_ABORENEW         2



// global definitions and global vars
NaviData_t          *naviData;
uint16_t            heading_home;
uint8_t             drawmode;
uint8_t             OSDScreenRefresh;


// flags from last round to check for changes
uint8_t  old_AngleNick      = 0;
uint8_t  old_AngleRoll      = 0;
uint16_t old_hh             = 0;


// aktuell nicht benoetigt - siehe Kommentar in osd.c
//mk_param_struct_t     *mk_param_struct;
//uint8_t           Flags_ExtraConfig;
//uint8_t           Flags_GlobalConfig;
//uint8_t           Flags_GlobalConfig3;


// cache old vars for blinking attribute, checkup is faster than full
// attribute write each time

volatile uint8_t OSD_active         = 0;

uint8_t Vario_Beep_Up               = 0;
uint8_t Vario_Beep_Down             = 0;
uint8_t Vario_Beep_Up_Interval      = 9;
uint8_t Vario_Beep_Down_Interval    = 6;
uint8_t Vario_Threshold             = 0;
uint8_t Vario_Threshold_Value       = 7;
uint8_t OldWP                       = 0;
uint8_t NextWP                      = 0;

uint8_t WP_old                      = 0;    // Screen: "Waypoints"
uint8_t WP_last                     = false;    // Screen: "Waypoints"



#define MAX_CELL_VOLTAGE    43  // max cell voltage for LiPO
#define MIN_CELL_VOLTAGE    32  // min cell voltage for LiPO

// Flags
volatile uint8_t error                  = 0;
uint8_t         cells                   = 0;
uint8_t         BattLowVoltageWarning   = 0;
uint8_t         CellIsChecked           = 0;
uint8_t         AkkuWarnThreshold       = 0;
uint16_t        duration                = 0;


//-----------------------------------------------------------
// Buffer
//-----------------------------------------------------------
BLData_t        blData[OSD_MAX_MOTORS];         // speichert gelesene BL-Datenpakete
pkt_gpspos_t    GPS_Current;                    // aktuelle GPS-Position
u8              old_PKTErrorcode;               // speichert den letzen Errorcode vom PKT damit dieser nicht wiederholt gespeichert wird
u8              old_MKErrorcode;                // speichert den letzen Errorcode vom MK damit dieser nicht wiederholt angezeigt wird


//-----------------------------------------------------------
// OSD Daten
//-----------------------------------------------------------
NaviData_t      osdData;                        // Buffer
NaviData_Tiny_t TinyOsdData;					// Buffer, wird nur für CamCtrlChar benötigt

//-----------------------------------------------------------
// MK-DISPLAY
//-----------------------------------------------------------
uint8_t         mkdisplayMode   = false;
uint8_t         mkdisplayCmd    = 0xff;
char            mkdisplayData[81];              // Buffer (80 +1 fuer term. Char)


//---------------------
// DEBUG
//---------------------
#ifdef USE_OSD_SCREEN_DEBUG
uint16_t        readCounterOSD;                 // Anzahl gelesener Datenpakete von NC Modus 'o' (Request OSD-Data)
uint16_t        readCounterTIME;                // Anzahl gelesener Datenpakete von NC (Time)
uint16_t        readCounterDISPLAY;
uint16_t        readCounterBL[OSD_MAX_MOTORS];  // Anzahl gelesener Datenpakete pro BL via NC Modus 'k' (BL Ctrl Status)
#endif  // USE_OSD_SCREEN_DEBUG


//---------------------
// Strings & Co.
//---------------------
static const char mkerror00[] PROGMEM = "No Error";
static const char mkerror01[] PROGMEM = "FC not compatible";
static const char mkerror02[] PROGMEM = "MK3Mag not compati.";
static const char mkerror03[] PROGMEM = "no FC communication";
static const char mkerror04[] PROGMEM = "no MK3Mag communic.";
static const char mkerror05[] PROGMEM = "no GPS communication";
static const char mkerror06[] PROGMEM = "bad compass value";
static const char mkerror07[] PROGMEM = "RC Signal lost";
static const char mkerror08[] PROGMEM = "FC spi rx error";
static const char mkerror09[] PROGMEM = "no NC communication";
static const char mkerror10[] PROGMEM = "FC Nick Gyro";
static const char mkerror11[] PROGMEM = "FC Roll Gyro";
static const char mkerror12[] PROGMEM = "FC Yaw Gyro";
static const char mkerror13[] PROGMEM = "FC Nick ACC";
static const char mkerror14[] PROGMEM = "FC Roll ACC";
static const char mkerror15[] PROGMEM = "FC Z-ACC";
static const char mkerror16[] PROGMEM = "Pressure sensor";
static const char mkerror17[] PROGMEM = "FC I2C";
static const char mkerror18[] PROGMEM = "Bl Missing";
static const char mkerror19[] PROGMEM = "Mixer Error";
static const char mkerror20[] PROGMEM = "Carefree Error";
static const char mkerror21[] PROGMEM = "GPS lost";
static const char mkerror22[] PROGMEM = "Magnet Error";
static const char mkerror23[] PROGMEM = "Motor restart";
static const char mkerror24[] PROGMEM = "BL Limitation";
static const char mkerror25[] PROGMEM = "Waypoint range";
static const char mkerror26[] PROGMEM = "No SD-Card";
static const char mkerror27[] PROGMEM = "SD Logging aborted";
static const char mkerror28[] PROGMEM = "Flying range!";
static const char mkerror29[] PROGMEM = "Max Altitude";
static const char mkerror30[] PROGMEM = "No GPS Fix";
static const char mkerror31[] PROGMEM = "compass not calibr.";
static const char mkerror32[] PROGMEM = "BL selftest";
static const char mkerror33[] PROGMEM = "no ext. compass";
static const char mkerror34[] PROGMEM = "compass sensor";

#define MAX_MKERROR_NUM        34   // maximale Error-Nummer vom MK (verwendet in osd())

static const char mkerror35[] PROGMEM = "";  // free for MK
static const char mkerror36[] PROGMEM = "";  // free for MK
static const char mkerror37[] PROGMEM = "";  // free for MK
static const char mkerror38[] PROGMEM = "";  // free for MK
static const char mkerror39[] PROGMEM = "";  // free for MK

static const char pkterror40[] PROGMEM = "PKT RX lost";


//---------------------------------------
// wenn die Liste erweitert wird,
// MAX_MKERROR_NUM in osd.h anpassen!
//---------------------------------------
const char * const mkerrortext[] PROGMEM=
{
    mkerror00,
    mkerror01,
    mkerror02,
    mkerror03,
    mkerror04,
    mkerror05,
    mkerror06,
    mkerror07,
    mkerror08,
    mkerror09,
    mkerror10,
    mkerror11,
    mkerror12,
    mkerror13,
    mkerror14,
    mkerror15,
    mkerror16,
    mkerror17,
    mkerror18,
    mkerror19,
    mkerror20,
    mkerror21,
    mkerror22,
    mkerror23,
    mkerror24,
    mkerror25,
    mkerror26,
    mkerror27,
    mkerror28,
    mkerror29,
    mkerror30,
    mkerror31,
    mkerror32,
    mkerror33,
    mkerror34,
    mkerror35,
    mkerror36,
    mkerror37,
    mkerror38,
    mkerror39,
    pkterror40
};


//char* rose = "-+-N-+-O-+-S-+-W-+-N-+-O-+-S-+-W-+-N-+-O-+-S-+-W";
const char rose[48] PROGMEM = {
    0x0e, 0x0f, 0x0e, 'N', 0x0e, 0x0f, 0x0e, 'O', 0x0e, 0x0f, 0x0e, 'S',
    0x0e, 0x0f, 0x0e, 'W', 0x0e, 0x0f, 0x0e, 'N', 0x0e, 0x0f, 0x0e, 'O',
    0x0e, 0x0f, 0x0e, 'S', 0x0e, 0x0f, 0x0e, 'W', 0x0e, 0x0f, 0x0e, 'N',
    0x0e, 0x0f, 0x0e, 'O', 0x0e, 0x0f, 0x0e, 'S', 0x0e, 0x0f, 0x0e, 'W',
};


// the center is char 19 (north), we add the current heading in 8th
// which would be 22.5 degrees, but float would bloat up the code
// and *10 / 225 would take ages... so we take the uncorrect way
static const char str_NE[] PROGMEM = "NE";
static const char str_E[]  PROGMEM = "E ";
static const char str_SE[] PROGMEM = "SE";
static const char str_S[]  PROGMEM = "S ";
static const char str_SW[] PROGMEM = "SW";
static const char str_W[]  PROGMEM = "W ";
static const char str_NW[] PROGMEM = "NW";
static const char str_N[]  PROGMEM = "N ";

const char * const directions_p[] PROGMEM=
{
    str_NE,
    str_E,
    str_SE,
    str_S,
    str_SW,
    str_W,
    str_NW,
    str_N
};


//###########################################################
//# OSD Screen Controler
//###########################################################

#define SCREENCTRL_MAX  15  // Anzahl max. zu verwaltender Screens

//---------------------------
// typedef: einzelner Screen
//---------------------------
typedef struct
{
    const char *screenname;
    void (*screen)(void);
}  screen_t;


//---------------------------
// typedef: Screenliste
//---------------------------
typedef struct
{
    uint8_t     active;
    uint8_t     count;
    screen_t    screen[SCREENCTRL_MAX];
}  screenlist_t;

screenlist_t osdscreens;


//--------------------------------------------------------------
// ScreenCtrl_Init()
//
// initialisiert die Screenliste - muss vor dem ersten
// ScreenCtrl_Push() aufgerufen werden
//--------------------------------------------------------------
void ScreenCtrl_Init( void )
{
    osdscreens.active = 0;
    osdscreens.count  = 0;
}


//--------------------------------------------------------------
// ScreenCtrl_Push( *screenname, *screenfunc)
//
// fuegt einen Screen der Screenliste hinzu (siehe osd())
//--------------------------------------------------------------
void ScreenCtrl_Push( uint8_t screenid, const char *screenname, void (*screenfunc)(void))
{
    if( osdscreens.count < SCREENCTRL_MAX )
    {
        // wenn screenid = 0 dann immer anzeigen (Screen ist nicht vom Benutzer auswaehlbar)
        if( (screenid==0) || ((Config.OSD_UseScreen & (1 << screenid)) != 0) )
        {
            osdscreens.screen[osdscreens.count].screenname = screenname;
            osdscreens.screen[osdscreens.count].screen     = screenfunc;
            osdscreens.count++;
        }
    }
}


//--------------------------------------------------------------
// ScreenCtrl_Set( screennum)
//
// aktiviert einen bestimmten Screen aus der Screenliste
//--------------------------------------------------------------
void ScreenCtrl_Set( uint8_t screennum )
{
    osdscreens.active = 0;
    if( screennum < osdscreens.count )
    {
        osdscreens.active = screennum;
    }
}


//--------------------------------------------------------------
// num = ScreenCtrl_GetNum()
//
// gibt die Nummer des aktuell aktivierten Screens zurueck
//--------------------------------------------------------------
uint8_t ScreenCtrl_GetNum( void )
{
    return osdscreens.active;
}


//--------------------------------------------------------------
// name = ScreenCtrl_GetName()
//
// gibt den Namen des aktuell aktivierten Screens zurueck
// Rueckgabe ist ein Pointer auf einen String im PROGMEM
//--------------------------------------------------------------
const char * ScreenCtrl_GetName( void )
{
    return osdscreens.screen[osdscreens.active].screenname;
}


//--------------------------------------------------------------
// ScreenCtrl_Next()
//
// zum naechsten Screen wechseln
//--------------------------------------------------------------
void ScreenCtrl_Next( void )
{
    osdscreens.active++;
    if( osdscreens.active >= osdscreens.count )
    {
        osdscreens.active = 0;
    }
    OSDScreenRefresh = OSD_SCREEN_REDRAW;
}


//--------------------------------------------------------------
// ScreenCtrl_Previous()
//
// zum vorherigen Screen wechseln
//--------------------------------------------------------------
void ScreenCtrl_Previous( void )
{
    if( osdscreens.active == 0 )
    {
        osdscreens.active = osdscreens.count-1;
    }
    else
    {
        osdscreens.active--;
    }
    OSDScreenRefresh = OSD_SCREEN_REDRAW;
}


//--------------------------------------------------------------
// ScreenCtrl_Show()
//
// ruft den aktuell Screen auf
//--------------------------------------------------------------
void ScreenCtrl_Show( void )
{
    osdscreens.screen[osdscreens.active].screen();
}


//###########################################################
//###########################################################

//--------------------------------------------------------------
//--------------------------------------------------------------
void calc_BLmax( osd_BLmax_t *blmax )
{
    uint8_t i;

    memset( blmax, 0, sizeof(osd_BLmax_t) );

    for( i=0; i<Config.OSD_Statistic.BL_Count; i++)
    {
        if( Config.OSD_Statistic.BL[i].max_Current > blmax->max_BL_Current )
        {
            blmax->max_BL_Current        = Config.OSD_Statistic.BL[i].max_Current;
            blmax->max_BL_Current_Index = i;
        }

        if( Config.OSD_Statistic.BL[i].max_Temp > blmax->max_BL_Temp )
        {
            blmax->max_BL_Temp    = Config.OSD_Statistic.BL[i].max_Temp;
            blmax->max_BL_Temp_Index = i;
        }

    }
}



//--------------------------------------------------------------
// STAT_Init()
//
// initialisiert die Statistic Werte neu
//--------------------------------------------------------------
void STAT_Init(void)
{
    // init: statistic
    memset( &Config.OSD_Statistic, 0, sizeof(osd_statistic_t) );

    Config.OSD_Statistic.min_UBat       = 255;
    Config.OSD_Statistic.min_RCQuality  = 255;
    Config.OSD_Statistic.min_AngleNick  = 126;
    Config.OSD_Statistic.min_AngleRoll  = 126;
}



//--------------------------------------------------------------
// MKErr_Log_Init()
//
// initialisiert die MK Errorcodes neu
//--------------------------------------------------------------
void MKErr_Log_Init(void)
{
        // init: MK Errorlog
        memset( &Config.MKErr_Log, 0, sizeof(mkerror_t)*MAX_MKERR_LOG );

}



//--------------------------------------------------------------
//--------------------------------------------------------------
void MkError_Save( uint8_t Errorcode )
{
    uint8_t i;

    for( i=MAX_MKERR_LOG-1; i>0; i--)
    {
        Config.MKErr_Log[i] = Config.MKErr_Log[i-1];
    }

    Config.MKErr_Log[0].Errorcode = Errorcode;
    memcpy( &Config.MKErr_Log[0].set_Time, (char *)&UTCTime, sizeof(PKTdatetime_t) );       // sichern...
}



//--------------------------------------------------------------
// OSD_MK_Connect()
//
// Verbindung zum MK herstellen oder ABO-Renew
//
// modus: MK_CONNECT oder MK_ABORENEW
//--------------------------------------------------------------
void OSD_MK_Connect( uint8_t modus )
{
    uint8_t tx_interval;

    if( modus == MK_CONNECT )
    {
        SwitchToNC();

        // switch off: 3d data => kompatibel mit SmartOSD????
        //tx_interval   = 0;
        //SendOutData ('c', ADDRESS_ANY, 1, &tx_interval, 1);
        //_delay_ms(25);

        tx_interval = 0;
        SendOutData( 'd', ADDRESS_ANY, 1, &tx_interval, 1);     // switch off: debug

        timer = 3; while( timer > 0 );                          // short delay
    }

    tx_interval = TXINTERVAL_OSDDATA;                           // switch on: sending osd-data
    SendOutData( 'o', ADDRESS_NC, 1,  &tx_interval, 1);         //   request: OSD Data from NC every ..ms

    timer = 3; while( timer > 0 );                              // short delay

                                                                // switch on: sending bl-data
    tx_interval = TXINTERVAL_BLDATA;                            //   5 => 50 ms (send packet every n*10 ms)
    SendOutData( 'k', ADDRESS_ANY, 1, &tx_interval, 1);         //   request: BL Ctrl Status

    mode      = 'O';
    abo_timer = ABO_TIMEOUT;

    rxd_buffer_locked = FALSE;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void OSD_MK_ShowTimeout( void )
{
    if( old_PKTErrorcode != 40 )
    {
        MkError_Save( 40 );                                         // Logge: "PKT RX lost"
        old_PKTErrorcode = 40;
    }

    lcd_cls ();

    lcd_frect_round( 0, 0, 127,10, 1, R1);                          // Rect: Invers (Titel)
    lcdx_printp_center( 0, strGet(OSD_ERROR), MINVERS, 1,2);        // "FEHLER: Datenverlust!"

    lcdx_printp_at(3, 1, strGet(START_LASTPOS) , MNORMAL, 0,6);     // "Letzte Position"
    lcdx_printp_at(3, 2, strGet(START_LASTPOS3), MNORMAL, 0,6);     // "Google Eingabe"

    //----
    lcd_frect( 0, (4*7)+5, 127, 7, 1);                              // Rect: Invers
    lcdx_printp_at(1, 3, strGet(START_LASTPOS1), MINVERS, 0,9);     // "Breitengr  Längengr"

    writex_gpspos(  1, 4, Config.LastLatitude , MNORMAL,  0,11);    // Anzeige: Breitengrad
    writex_gpspos( 12, 4, Config.LastLongitude, MNORMAL, -1,11);    // Anzeige: Laengengrad

    lcd_rect( 0, (3*8)+8, 127, (2*8)+4, 1);                         // Rahmen

    lcd_printp_at(12, 7, strGet(ENDE)      , MNORMAL);              // Keyline: "Ende"

    set_beep ( 0, 0, BeepOff);                                      // Falls Spannungswarnung an war Beeper aus (ist das notwendig?)
}



/***************************************************************************************************************************************

//--------------------------------------------------------------
// OSD_MK_UTCTime()
// Fuer: defined MKVERSION088n || defined MKVERSION090b || defined MKVERSION090e
// -> erstmal keine Unterstuetzung mehr...
//--------------------------------------------------------------

//--------------------------------------------------------------
// OSD_MK_UTCTime()
//
// ALTE Funktion fuer FC < 0.90h (und dementsprechend NC < 0.30h)
//
// Setzt die PKT globale UTCTime mithilfe des MK.
//
// Foraussetzung:
// - der NC-Modus muss aktiv sein (SwitchToNC)
// - aktuell nur fuer osd.c
//
// Rueckgabe:
//  true = ok, UTCTime gespeichert
//  false = Zeit/Datum nicht gelesen
//
// Hack:
// Gelesen wird die Seite 3 des NC-Display's. Dort wird Zeit
// und Datum an den entsprechnenden Positionen via Zeichen an-
// gezeigt. Die Zeichen werden ausgelesen und in die interne UTCTime
// gespeichert.
//
// HINWEIS:
// Ab NC > v0.30b (evtl. v0.30c) wird die NC ein neues Datenpaket
// senden ("T") mit strukturierten Informationen zu Datum/Zeit.
// Das wurde von Holger eingebaut.
// Wird spaeter auch im PKT implementiert.
//--------------------------------------------------------------
int OSD_MK_UTCTime( uint8_t readtime )
{
    uint32_t sec;
    uint32_t min;
    uint32_t hour;
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  page;
    uint8_t  ok = false;


    mode = 'L';                         // read: MK-Display Page
    page = 3;                           // anfordern von Seite 3 (der NC)
    SendOutData ('l', ADDRESS_ANY, 1, &page, 1);    // request: MK-Display Page 3

    rxd_buffer_locked = FALSE;

    timer = readtime;                   // fuer max. n*10 millisec versuchen Daten zu lesen
    while( timer>0 && !ok );            // lese Daten-Pakete fuer die angegebene Zeit oder gelesen
    {
        if( rxd_buffer_locked )
        {
            Decode64();

            #ifdef USE_OSD_SCREEN_DEBUG
            readCounterTIME++;
            #endif

            sec  = (uint32_t)(pRxData[66+7+2] - '0') + 10 * (uint32_t)(pRxData[66+6+2] - '0');          // seconds: sec
            min  = (uint32_t)(pRxData[66+4+2] - '0') + 10 * (uint32_t)(pRxData[66+3+2] - '0');          // seconds: min
            hour = (uint32_t)(pRxData[66+1+2] - '0') + 10 * (uint32_t)(pRxData[66+0+2] - '0');          // seconds: hour
            sec += (min * 60) + (hour * 3600);

            year =  (uint16_t)(pRxData[46+9+2] - '0') + 10   * (uint16_t)(pRxData[46+8+2] - '0');       // year
            year += 100 * (uint16_t)(pRxData[46+7+2] - '0') + 1000 * (uint16_t)(pRxData[46+6+2] - '0'); // year

            day   = (uint8_t)(pRxData[46+4+2] - '0') + 10 * (uint8_t)(pRxData[46+3+2] - '0');           // day

            month = (uint8_t)(pRxData[46+1+2] - '0') + 10 * (uint8_t)(pRxData[46+0+2] - '0');           // month

            // in der globalen UTCTime speichern (hoffentlich funkt kein timer dazwischen)
            if( year > 2000 && year < 2200 )        // plausibilitaets check
            {
                ATOMIC_BLOCK(ATOMIC_FORCEON)
                {
                    UTCTime.seconds = sec;
                    UTCTime.day     = day;
                    UTCTime.month   = month;
                    UTCTime.year    = year;
                }
            }
            ok = true;
        }
    }

    return ok;              // wenn erfolgreich gelesen dann true (kein timeout)
}
***************************************************************************************************************************************/



//--------------------------------------------------------------
// OSD_MK_UTCTime()
//
// NEUE Ermittlung der Zeit vom MK
// ab NC v0.30g (bzw. NC 0.30h fuer FC 0.90h)
//
// TODO OG: auf neue MKVersion bzgl. Versionsprüfung anpassen!
//--------------------------------------------------------------
int OSD_MK_UTCTime( uint8_t readtime )
{
    DateTime_t  *rx_DateTime;
    uint8_t     tx_interval;
    uint8_t     ok = false;

    mode = 'T';                                         // read: MK-Display Page
    tx_interval = 1;                                    //
    SendOutData ('t', ADDRESS_NC, 1, &tx_interval, 1);  // request: DateTime

    rxd_buffer_locked = FALSE;

    timer = readtime;                                   // fuer max. n*10 millisec versuchen Daten zu lesen
    while( timer>0 && !ok );                            // lese Daten-Pakete fuer die angegebene Zeit oder gelesen
    {
        if( rxd_buffer_locked )
        {
            Decode64();
            rx_DateTime = (DateTime_t *) (pRxData);

            #ifdef USE_OSD_SCREEN_DEBUG
            readCounterTIME++;
            #endif

            if( rx_DateTime->Year > 2000 && rx_DateTime->Year < 2200 )      // Plausibilitaets Check
            {
                ATOMIC_BLOCK(ATOMIC_FORCEON)
                {
                    UTCTime.seconds = ((uint32_t)(rx_DateTime->Hour))*3600 + ((uint32_t)(rx_DateTime->Min))*60 + (uint32_t)(rx_DateTime->Sec);
                    UTCTime.day     = rx_DateTime->Day;
                    UTCTime.month   = rx_DateTime->Month;
                    UTCTime.year    = rx_DateTime->Year;
                }
            }

            ok = true;
        }
    }

    return ok;              // wenn erfolgreich gelesen dann true (kein timeout)
}



//--------------------------------------------------------------
//--------------------------------------------------------------
int32_t calc_avg( int32_t avg, int32_t nvalue, int32_t count, int32_t factor)
{
    avg = avg + (( ( nvalue * factor) - avg) / count);
    return avg;
}



//--------------------------------------------------------------
// OSD_MK_GetData()
//
// Holt Daten der BL-Ctrl via NC.
//
// Die Werte der BL's 1-12 kommen in mehr oder weniger beliebiger
// Reihenfolge an. Die Daten fuer vorhandene BL's werden oefter
// gesendet als die von nicht vorhandenen BL's.
//
// Diese Funktioen liest fuer die Zeit BL_READ_TIME die BL-Daten
// vom MK ein und ordnet diese der PKT-internen Speichstruktur zu.
//
// Hierbei ist nicht gewaehrleistet, dass die Daten von jedem BL
// in einem festen Zeitrahmen ermittelt werden. Die Folge ist
// ein 'zufaelliger' Aufbau der Anzeige der BL-Daten und auch
// eine nicht exakt bestimmbare Refreshtime der Werte.
//
// Optimieren kann man ggf. die Refreshzeit aller BL-Werte durch
// tx_intervall fuer Kommando "k" und durch BL_READ_TIME.
// Brauchbare Startwerte sind (noch experimentell):
//   tx_interval  = 5 (fuer "k")    =  50 ms
//   BL_READ_TIME = 25              = 250 ms
//
// Wenn dieses Verhalten verbessert werden soll muss ggf.
// die ISR() (usart.c) fuer Kommando "k" angepasst werden um
// in einem Schwung mehrere gesendete BL-Pakete aufeinmal
// aufzunehmen und als Paket zur Verfuegung zu stellen.
//--------------------------------------------------------------
void OSD_MK_GetData( void )
{
    BLData_t    *rx_blData;
    uint8_t     blIndex;
    uint8_t     FCStatusFlags;
    uint8_t     v;
    uint8_t     skipBL = false;

    FCStatusFlags = naviData->FCStatusFlags;                // save naviData->FCStatusFlags for use with BL-Data


    //######################################
    //# ZEIT/DATUM
    //######################################
    // lese UTC-Time vom MK
    //--------------------------------------
    if( timer_get_tidata == 0 )
    {
        //--------------------------------------------------
        // das 'T' Datenpaket der NC fuer OSD_MK_UTCTime()
        // gibt es erst ab NC v0.30g (!)
        //
        // --> Versionspruefung der NC-Firmware
        //--------------------------------------------------

        v = MKVersion_Cmp( MKVersion.NCVer,  0,30,'g' );        // pruefe auf NC-Version >= "0.30g"

        if( v && (v >= GIVEN_VERSION) )                         // wenn aktuelle NC-Version >= "0.30g"...
        {
            if( !OSD_MK_UTCTime(20) )
                timer_get_tidata = 50;                          // erfolglos: versuche es nach einer 1/2 Sekunde erneut
            else
                timer_get_tidata = TIME_GET_TIDATA;             // alle 60 Sekunden refresh - den Rest uebernimmt ein Timer des PKT
        }
    }



    //######################################
    //# MK-DISPLAY
    //######################################
    // switch to: (h)
    //--------------------------------------
    //if( timer_get_displaydata == 0 )
    if( mkdisplayMode && timer_get_displaydata == 0 )
    {
        mode              = 'H';
        rxd_buffer_locked = FALSE;

/*
        if( mkdisplayCmd != 0xff )
        {
            if( mkdisplayCmd == 0 ) mkdisplayCmd = 0xff;
            SendOutData( 'h', ADDRESS_ANY, 1, &mkdisplayCmd, 1);
        }
*/
        if( mkdisplayCmd == 0 ) mkdisplayCmd = 0xff;

        SendOutData( 'h', ADDRESS_ANY, 2, &mkdisplayCmd, 1, 0x00 ,1); // 05.04.2015 Cebra, 2.er Parameter wg NC 2.09i

        mkdisplayCmd = 0xff;        // 0xff = aktuelle Seite

        timer = 20; while( (timer>0) && !rxd_buffer_locked );                               //

        if( rxd_buffer_locked )
        {
            Decode64();
            memcpy( mkdisplayData, (const void *)&rxd_buffer[3+ 0], 80 );       // sichern...

            #ifdef USE_OSD_SCREEN_DEBUG
                readCounterDISPLAY++;
            #endif  // USE_OSD_SCREEN_DEBUG
        }

        timer_get_displaydata = TIME_GET_DISPLAYDATA;                 // n*10 ms
        skipBL                = true;
    }  // end: if( mkdisplayMode && timer_get_displaydata == 0 )



    //######################################
    //# BL-Daten
    //######################################
    // switch to: 'BL Ctrl Status' (k)
    //--------------------------------------
    if( !skipBL && timer_get_bldata == 0 )
    {
        mode        = 'K';                                  // read: BL Ctrl Status
        rxd_buffer_locked = FALSE;

        timer = TIME_READ_BLDATA;                           // fuer x Zeit werden BL-Daten gelesen
        while( timer>0 )                                    // lese Daten-Pakete fuer die angegebene Zeit
        {
            if( rxd_buffer_locked )
            {
                Decode64 ();
                rx_blData = (BLData_t *) (pRxData);

                // die BL-Daten kommen in beliebiger Reihenfolge an
                // Hier werden sie entsprechend ihres Index gesichert
                blIndex = rx_blData->Index;
                if( blIndex >= 0 && blIndex < OSD_MAX_MOTORS )
                {
                    memcpy( &blData[blIndex], rx_blData, sizeof(BLData_t));  // sichern...

                    #ifdef USE_OSD_SCREEN_DEBUG
                        readCounterBL[blIndex]++;
                    #endif  // USE_OSD_SCREEN_DEBUG

                    // Statistiken
                    if( (blData[blIndex].Status & 0xf0) && (FCStatusFlags & FC_STATUS_MOTOR_RUN) )  // nur wenn BL/Motor vorhanden und Motoren laufen
                    {
                        // BL Statistik: Anzahl empf. Datenpakete (fuer Mittelwert)
                        Config.OSD_Statistic.BL[blIndex].count++;

                        // int32_t calc_avg( int32_t avg, int32_t value, int32_t count, int32_t factor)
                        Config.OSD_Statistic.BL[blIndex].avg_Current = (uint16_t) calc_avg( (int32_t)Config.OSD_Statistic.BL[blIndex].avg_Current,
                                                                                            (int32_t)blData[blIndex].Current,
                                                                                            (int32_t)Config.OSD_Statistic.BL[blIndex].count,
                                                                                            100);

                        // ALT
                        // BL Statistik: Mittelwert: Strom (*100 um Rundungsfehler zu reduzieren)
                        //avg = (int32_t)Config.OSD_Statistic.BL[blIndex].avg_Current;
                        //avg = avg + (( ( (int32_t)blData[blIndex].Current * 100) - avg) / (int32_t)Config.OSD_Statistic.BL[blIndex].count);
                        //Config.OSD_Statistic.BL[blIndex].avg_Current = (uint16_t)avg;

                        // BL Statistik:  Max: Strom
                        if( blData[blIndex].Current     > Config.OSD_Statistic.BL[blIndex].max_Current) Config.OSD_Statistic.BL[blIndex].max_Current = blData[blIndex].Current;

                        // BL Statistik:  Max: Temperatur
                        if( blData[blIndex].Temperature > Config.OSD_Statistic.BL[blIndex].max_Temp)    Config.OSD_Statistic.BL[blIndex].max_Temp    = blData[blIndex].Temperature;
                        if( blIndex+1                   > Config.OSD_Statistic.BL_Count )               Config.OSD_Statistic.BL_Count                = blIndex+1;
                    }
                }
                rxd_buffer_locked = FALSE;
            }
        }
        timer_get_bldata = TIME_GET_BLDATA;                 // n*10 ms
    }  // end: if( timer_get_bldata == 0 )



    //--------------------------------------
    // back to: OSD-Data
    //--------------------------------------
    mode                = 'O';                          // read: OSD-Data
    rxd_buffer_locked   = FALSE;                        // ready to receive new data
    timer_mk_timeout    = MK_TIMEOUT;                   // reset osd MK_TIMEOUT timer
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void variobeep(int16_t vario)
{
#ifdef USE_SOUND
  { //start Beep

    if (vario >0 )          // MK steigt
      {
       Vario_Beep_Down = 0;                 // Down Beep freischalten
       Vario_Threshold++;

       if ((Vario_Beep_Up == 0) && (Vario_Threshold >= Vario_Threshold_Value))
        {
//         set_beep ( 100, 0xffff, BeepNormal);
         duration = 52 -vario;
//         if (duration =0); duration = 1;

//         write_ndigit_number_u (0,6,duration,5,0);

         playTone(300+vario*2,duration,Config.Volume);

//         playTone(300,duration,volume);
         Vario_Threshold = Vario_Threshold_Value;     // auf Maximalwert begrenzen
        }
       Vario_Beep_Up++;                 // Interval hochzählen in dem nicht gepiept wird
       if (Vario_Beep_Up == Vario_Beep_Up_Interval) Vario_Beep_Up = 0;
      }

    if (vario <0)          // MK fällt
      {
       Vario_Beep_Up = 0;                 // Up Beep freischalten
       Vario_Threshold++;
       if ((Vario_Beep_Down == 0) && (Vario_Threshold >= Vario_Threshold_Value))
        {
         duration = 50 -vario;
//         write_ndigit_number_u (0,7,duration,5,0);
//         if (duration < vario) ; duration = 0;
//         playTone(300,50,volume);


         playTone(300+vario*2,duration,Config.Volume);

         Vario_Threshold = Vario_Threshold_Value;     // auf Maximalwert begrenzen
        }
       Vario_Beep_Down++;                 // Interval hochzählen in dem nicht gepiept wird
       if (Vario_Beep_Down == Vario_Beep_Down_Interval) Vario_Beep_Down = 0;
      }

    if (vario == 0) Vario_Threshold = 0;  //Startverzögerung löschen
  }  // end Beep
#endif
}


//--------------------------------------------------------------
// Diese Funktion Beept unabhaengig von der Einstellung Config.OSD_VarioBeep
// Aufruf ggf. mit: if( Config.OSD_VarioBeep ) Beep_Vario();
//
// Ansonten:
// -> hier noch aufräumen in Zusammenhang mit func variobeep()
//--------------------------------------------------------------
void Beep_Vario(void)
{
    if ( (naviData->FCStatusFlags & FC_STATUS_MOTOR_RUN) && (naviData->FCStatusFlags2 & FC_STATUS2_ALTITUDE_CONTROL))
    { //start Beep
      if (naviData->Variometer <0)          // MK fällt
        {
         Vario_Beep_Up = 0;                 // Up Beep freischalten
         Vario_Threshold++;
         if ((Vario_Beep_Down == 0) && (Vario_Threshold >= Vario_Threshold_Value))
          {

          if (!Config.HWSound) set_beep ( 300, 0xffff, BeepNormal);
               else  variobeep(naviData->Variometer);

           Vario_Threshold = Vario_Threshold_Value;     // auf Maximalwert begrenzen
          }
         Vario_Beep_Down++;                 // Interval hochzählen in dem nicht gepiept wird
         if (Vario_Beep_Down == Vario_Beep_Down_Interval) Vario_Beep_Down = 0;
        }

      if (naviData->Variometer == 0) Vario_Threshold = 0;  //Startverzögerung löschen

      if (naviData->Variometer >0 )          // MK steigt
        {
         Vario_Beep_Down = 0;                 // Down Beep freischalten
         Vario_Threshold++;

         if ((Vario_Beep_Up == 0) && (Vario_Threshold >= Vario_Threshold_Value))
          {
           if (!Config.HWSound) set_beep ( 100, 0xffff, BeepNormal);
           else  variobeep(naviData->Variometer);
           Vario_Threshold = Vario_Threshold_Value;     // auf Maximalwert begrenzen
          }
         Vario_Beep_Up++;                 // Interval hochzählen in dem nicht gepiept wird
         if (Vario_Beep_Up == Vario_Beep_Up_Interval) Vario_Beep_Up = 0;
        }
    }  // end Beep
}



//--------------------------------------------------------------
// Quelle Mikrokopter FC-Software Holger + Ingo
//--------------------------------------------------------------
void CheckMKLipo( void )
{
    if( Config.MK_LowBat < 50 )     // automatische Zellenerkennung
    {
       if( CellIsChecked <= 2 )     // nur beim Start 1x prüfen
       {
            // up to 6s LiPo, less than 2s is technical impossible
            for( cells = 2; cells < 7; cells++)
            {
                if( naviData->UBat < cells * MAX_CELL_VOLTAGE)
                    break;
            }
            BattLowVoltageWarning = cells * Config.MK_LowBat;
            CellIsChecked++;
       }
    }
    else BattLowVoltageWarning = Config.MK_LowBat;

    if( naviData->UBat < BattLowVoltageWarning)
    {
        if( AkkuWarnThreshold <= 4)
        {
            AkkuWarnThreshold++;
        }
        else if( naviData->FCStatusFlags & FC_STATUS_MOTOR_RUN )
        {
            // MK-Unterspannungs-Beep nur wenn Motoren laufen
            set_beep ( 1000, 0x0020, BeepSevere);
        }
    }
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void calc_heading_home(void)
{
    int16_t degree;

    //---------------------------------------
    // warum modulo 360:
    //
    // die NC kann unter gewissen Umstaenden Winkel > 360 Grad
    // senden -> fragt H&I warum
    //---------------------------------------
    degree = ((naviData->CompassHeading % 360) - (naviData->HomePositionDeviation.Bearing % 360));

    if( degree < 0 )
        heading_home = (int16_t)(360 + degree);
    else
        heading_home = (int16_t)degree;
}


//--------------------------------------------------------------
// convert the <heading> gotton from NC into an index
uint8_t heading_conv (uint16_t heading)
{
    if (heading > 23 && heading < 68)
        return 0;       //direction = "NE";
    else if (heading > 67 && heading < 113)
        return 1;       //direction = "E ";
    else if (heading > 112 && heading < 158)
        return 2;       //direction = "SE";
    else if (heading > 157 && heading < 203)
        return 3;       //direction = "S ";
    else if (heading > 202 && heading < 248)
        return 4;       //direction = "SW";
    else if (heading > 247 && heading < 293)
        return 5;       //direction = "W ";
    else if (heading > 292 && heading < 338)
        return 6;       //direction = "NW";

    return 7;   //direction = "N ";
}


//--------------------------------------------------------------
// draw a compass rose at <x>/<y> for <heading>
void draw_compass (uint8_t x, uint8_t y, uint16_t heading, int8_t xoffs, int8_t yoffs)
{

    uint8_t front = 19 + (heading / 22);
    for (uint8_t i = 0; i < 9; i++)
        lcdx_putc (x++, y, pgm_read_byte(&rose[front - 4 + i]), MNORMAL, xoffs,yoffs);
}


//--------------------------------------------------------------
// variometer
// x, y in Pixel!
//--------------------------------------------------------------
void draw_variometer (uint8_t x, uint8_t y, uint8_t width, uint8_t hight, int16_t variometer)
{
    lcd_rect (x, y - ((hight) / 2), width, hight, 1);
    lcd_frect (x + 1, y - ((hight) / 2) + 1, width - 2, hight - 2, 0);
    lcd_line (x, y, x + width, y, 1);

    if (variometer > 0)  // steigend
    {
        switch (variometer / 5)
        {
            case 0:
                lcd_line  (x + 4, y - 1, x + 6, y - 1, 1);  //  1 >  4
            break;

            case 1:
                lcd_line  (x + 4, y - 1, x + 6, y - 1, 1);  //  1 >  4
                lcd_frect (x + 3, y - 3, 4, 1, 1);          //  5 >  9
            break;

            case 2:
                lcd_line  (x + 4, y - 1, x + 6, y - 1, 1);  //  1 >  4
                lcd_frect (x + 3, y - 3, 4, 1, 1);          //  5 >  9
                lcd_frect (x + 2, y - 5, 6, 1, 1);          // 10 > 14
            break;

            default:
                lcd_line  (x + 4, y - 1, x + 6, y - 1, 1);  //  1 >  4
                lcd_frect (x + 3, y - 3, 4, 1, 1);          //  5 >  9
                lcd_frect (x + 2, y - 5, 6, 1, 1);          // 10 > 14
                lcd_frect (x + 1, y - 6, 8, 1, 1);          // 15 >
            break;
        }
    }
    else if (variometer < 0)  // fallend
    {
        switch ((variometer) / -5)
        {
            case 0:
                lcd_line  (x + 4, y + 1, x + 6, y + 1, 1);  // - 1 > - 4
            break;

            case 1:
                lcd_line  (x + 4, y + 1, x + 6, y + 1, 1);  // - 1 > - 4
                lcd_frect (x + 3, y + 2, 4, 1, 1);          // - 5 > - 9
            break;

            case 2:
                lcd_line  (x + 4, y + 1, x + 6, y + 1, 1);  // - 1 > - 4
                lcd_frect (x + 3, y + 2, 4, 1, 1);          // - 5 > - 9
                lcd_frect (x + 2, y + 4, 6, 1, 1);          // -10 > -14
            break;

            default:
                lcd_line  (x + 4, y + 1, x + 6, y + 1, 1);  // - 1 > - 4
                lcd_frect (x + 3, y + 2, 4, 1, 1);          // - 5 > - 9
                lcd_frect (x + 2, y + 4, 6, 1, 1);          // -10 > -14
                lcd_frect (x + 1, y + 5, 8, 1, 1);          // -15 >
            break;
        }
    }
}


//--------------------------------------------------------------
// variometer 2
//
// x, y in Pixel
// x, y top, left
//--------------------------------------------------------------
/*
void draw_variometer2( uint8_t x, uint8_t y, uint8_t width, uint8_t hight, int16_t variometer)
{
    uint8_t max = 5;    // max: 5 m/sec == 100%

    lcd_rect (x, y, width, hight, 1);
    lcd_frect(x + 1, y + 1, width - 2, hight - 2, 0);
    lcd_line (x, y + ((hight) / 2), x + width, y + ((hight) / 2), 1);

}
*/


//--------------------------------------------------------------
// Home symbol
// draw Homesymbol at <x>/<y>
//--------------------------------------------------------------
void draw_homesymbol (uint8_t x, uint8_t y)
{
        x *= 6;
        y *= 8;
        y += 7;

        lcd_plot (x,y-4,1);
        lcd_line (x+1,y-1,x+1,y-5,1);
        lcd_plot (x+2,y-6,1);
        lcd_plot (x+3,y-7,1);
        lcd_plot (x+4,y-6,1);
        lcd_line (x+5,y-1,x+5,y-5,1);
        lcd_plot (x+6,y-4,1);
        lcd_plot (x+3,y-1,1);
        lcd_plot (x+3,y-2,1);
        lcd_line (x+1,y,x+5,y,1);

}


//--------------------------------------------------------------
// Target symbol
// draw Targetsymbol at <x>/<y>
//--------------------------------------------------------------
void draw_targetsymbol (uint8_t x, uint8_t y)
{
        x *= 6;
        y *= 8;
        y += 7;

        lcd_circle (x+3, y-3, 4, 1);
        lcd_line (x,y-3,x+6,y-3,1);
        lcd_line (x+3,y,x+3,y-6,1);
        lcd_circle (x+3, y-3, 2, 1);
}


//-----------------------------------------------------------
//-----------------------------------------------------------
void writex_altimeter( uint8_t x, uint8_t y, s32 Altimeter, uint8_t mode, int8_t xoffs, int8_t yoffs )
{
    if (Altimeter > (300 / AltimeterAdjust) || Altimeter < (-300 / AltimeterAdjust))    // above 10m only write full meters
        writex_ndigit_number_s ( x, y, (Altimeter / (30 / AltimeterAdjust)), 4, 0, mode, xoffs,yoffs);
    else                                                                                                    // up to 10m write meters.dm
        writex_ndigit_number_s_10th( x, y, (Altimeter / (3 / AltimeterAdjust)), 3, 0, mode, xoffs,yoffs);
}



//-----------------------------------------------------------
//--------------------------------------------------------------
void lcd_o_circle (uint16_t x, uint16_t y, int16_t breite, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    breite *= 6;
    int16_t radius = breite / 2;
    x += 2;
    x *= 6;
    x += 2;
    y += 1;
    y *= 8;
    y += 3;

    // 04.03.2012 OG: chg: x-radius von -3 auf -2 (runder auf dem display)
    //lcd_ellipse (x, y, radius - 3, radius - 5, mode);
    lcd_ellipse( x+xoffs, y+yoffs, radius - 2, radius - 5, mode);
}


//-----------------------------------------------------------
// lcd_o_circ_line( x, y, breite, deg, rOffset, mode)
//
// x, y   : in Chars
// breite : in Chars
// deg    : in Pixel
// rOffset: Beeinflusst den Schluss der Linie zum Huellkreis
//          0 = Standard
//          >0 naeher zum Huellkreis
//          <0 entfernter vom Huellkreis
// mode   : siehe: lcd_ellipse_line()
//-----------------------------------------------------------
void lcd_o_circ_line( uint16_t x, uint16_t y, uint8_t breite, uint16_t deg, int8_t rOffset, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    breite *= 6;
    int16_t radius = breite / 3;
    x += 2;
    x *= 6;
    x += 2;
    y += 1;
    y *= 8;
    y += 3;

    // 04.03.2013 OG: der Radius kann mit rOffset >0 vergroessert werden um zum Kreis aufzuschliessen
    lcd_ellipse_line( x+xoffs, y+yoffs, radius+rOffset, radius+rOffset, deg, mode);
}











//-----------------------------------------------------------
//--------------------------------------------------------------
void lcdx_o_circle( uint8_t x, uint8_t y, int8_t width, uint8_t mode)
{
    int8_t radius = width / 2;

    // 04.03.2012 OG: chg: x-radius von -3 auf -2 (runder auf dem display)
    //lcd_ellipse (x, y, radius - 3, radius - 5, mode);
    lcd_ellipse( x, y, radius - 2, radius - 5, mode);
}



//-----------------------------------------------------------
// lcd_o_circ_line( x, y, breite, deg, rOffset, mode)
//
// x, y   : in Chars
// breite : in Chars
// deg    : in Pixel
// rOffset: Beeinflusst den Schluss der Linie zum Huellkreis
//          0 = Standard
//          >0 naeher zum Huellkreis
//          <0 entfernter vom Huellkreis
// mode   : siehe: lcd_ellipse_line()
//-----------------------------------------------------------
void lcdx_o_circ_line( uint16_t x, uint16_t y, uint8_t breite, uint16_t deg, int8_t rOffset, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    breite *= 6;
    int16_t radius = breite / 3;
    x += 2;
    x *= 6;
    x += 2;
    y += 1;
    y *= 8;
    y += 3;

    // 04.03.2013 OG: der Radius kann mit rOffset >0 vergroessert werden um zum Kreis aufzuschliessen
    lcd_ellipse_line( x+xoffs, y+yoffs, radius+rOffset, radius+rOffset, deg, mode);
}



//--------------------------------------------------------------
// PARAMETER:
//   x,y : in Pixel
//--------------------------------------------------------------
void draw_icon_mk( uint8_t x, uint8_t y)
{
  //lcd_plot( x+0, y+0, 1);             // Referenz 0,0

    lcd_line( x+5, y+0, x+0, y+5, 1);   // Dach oben Links
    lcd_line( x+5, y+0, x+10, y+5, 1);  // Dach oben Rechts

    lcd_line( x+5, y+10, x+0, y+5, 1);  // Dach unten Links
    lcd_line( x+5, y+10, x+10, y+5, 1); // Dach unten Rechts

    lcd_line( x+4, y+5, x+6, y+5, 1);   // Innenkreuz Horizontal
    lcd_line( x+5, y+4, x+5, y+6, 1);   // Innenkreuz Vertikal
}



//--------------------------------------------------------------
// Variante: rund
//
// PARAMETER:
//   x,y : in Pixel
//--------------------------------------------------------------
void draw_icon_target_round( uint8_t x, uint8_t y)
{
  //lcd_plot( x+0, y+0, 1);             // Referenz 0,0

    lcd_ellipse( x+5, y+5, 5+1, 5, 1);  // Aussenkreis

  //lcd_line( x+2, y+5, x+8, y+5, 1);   // Innenkreuz Horizontal (laenger)
    lcd_line( x+3, y+5, x+7, y+5, 1);   // Innenkreuz Horizontal (kuerzer)

    lcd_line( x+5, y+3, x+5, y+7, 1);   // Innenkreuz Vertikal
}



//--------------------------------------------------------------
// Variante: eckig
//
// PARAMETER:
//   x,y : in Pixel
//--------------------------------------------------------------
void draw_icon_target_diamond( uint8_t x, uint8_t y)
{
  //lcd_plot( x+0, y+0, 1);             // Referenz 0,0

    lcd_line( x+5, y+0, x+0, y+5, 1);   // Dach oben Links
    lcd_line( x+5, y+0, x+10, y+5, 1);  // Dach oben Rechts

    lcd_line( x+5, y+10, x+0, y+5, 1);  // Dach unten Links
    lcd_line( x+5, y+10, x+10, y+5, 1); // Dach unten Rechts

    lcd_line( x+4, y+5, x+6, y+5, 1);   // Innenkreuz Horizontal
    lcd_line( x+5, y+4, x+5, y+6, 1);   // Innenkreuz Vertikal
}



//--------------------------------------------------------------
// PARAMETER:
//   x,y : in Pixel
//--------------------------------------------------------------
void draw_icon_home( uint8_t x, uint8_t y)
{
    //lcd_plot( x+0, y+0, 1);           // Referenz 0,0
    lcd_rect( x+0, y+5, 10, 8, 1);      // Mitte
    lcd_line( x+5, y+0, x+0, y+5, 1);   // Dach Links
    lcd_line( x+5, y+0, x+10, y+5, 1);  // Dach Rechts
    lcd_rect( x+4, y+10, 2, 3, 1);      // Tuere

}



//--------------------------------------------------------------
// PARAMETER:
//   x,y : in Pixel
//--------------------------------------------------------------
void draw_icon_sat( uint8_t x, uint8_t y)
{
    //lcd_plot( x+0, y+0, 1);            // Referenz 0,0
    lcd_rect( x+0, y+2, 4, 2, 1);        // linker Fluegel
    lcd_rect( x+8, y+2, 4, 2, 1);        // rechter Fluegel
    lcd_rect( x+4, y+0, 4, 6, 1);        // Mitte, oben
    lcd_line( x+6, y+7, x+2, y+11, 1);   // Strahl Links
    lcd_line( x+6, y+7, x+10, y+11, 1);  // Strahl Rechts
    lcd_line( x+1, y+12, x+11, y+12, 1); // Strahl Unten
}



//--------------------------------------------------------------
// PARAMETER:
//   x,y : in Pixel
//--------------------------------------------------------------
void draw_icon_satmini( uint8_t x, uint8_t y)
{
    //lcd_plot( x+0, y+0, 1);            // Referenz 0,0

    //lcd_line( x+3, y+3, x+0, y+6, 1);   // Strahl Links

    lcd_line( x+2, y+0, x+4, y+0, 1);   //
    lcd_line( x+0, y+1, x+6, y+1, 1);   //
    lcd_line( x+2, y+2, x+4, y+2, 1);   //
    lcd_plot( x+3, y+1, 0);             //

    lcd_line( x+3, y+3, x+1, y+5, 1);   // Strahl Links
    lcd_line( x+3, y+3, x+5, y+5, 1);   // Strahl Rechts
    lcd_line( x+0, y+6, x+6, y+6, 1);   // Strahl Unten
}



//--------------------------------------------------------------
// PARAMETER:
//   x,y : in Pixel
//--------------------------------------------------------------
void draw_icon_satmini2( uint8_t x, uint8_t y)
{
    //lcd_plot( x+0, y+0, 1);            // Referenz 0,0

    //lcd_line( x+3, y+3, x+0, y+6, 1);   // Strahl Links

    lcd_line( x+2, y-1, x+4, y-1, 1);   //
  //lcd_line( x+2, y+0, x+4, y+0, 1);   //
    lcd_line( x+0, y+0, x+6, y+0, 1);   //
    lcd_line( x+0, y+1, x+6, y+1, 1);   //
    lcd_line( x+2, y+2, x+4, y+2, 1);   //
    lcd_plot( x+3, y+0, 0);             //
    lcd_plot( x+3, y+1, 0);             //

    lcd_line( x+3, y+3, x+1, y+5, 1);   // Strahl Links
    lcd_line( x+3, y+3, x+5, y+5, 1);   // Strahl Rechts
    lcd_line( x+0, y+6, x+6, y+6, 1);   // Strahl Unten
}


//--------------------------------------------------------------
// PARAMETER:
//   x,y : in Pixel
//--------------------------------------------------------------
void draw_icon_battery( uint8_t x, uint8_t y)
{
    //lcd_plot( x+0, y+0, 1);           // Referenz 0,0
    lcd_rect( x+2, y+0, 2, 2,  1);      // der kleine Knubbel oben
    lcd_rect( x+0, y+2, 6, 15, 1);      // body
}



//--------------------------------------------------------------
// RC symbol
// alternatives Symbol fuer RC-Quality
//--------------------------------------------------------------
void draw_symbol_rc( uint8_t x, uint8_t y)
{
    x *= 6;
    y *= 8;
    y += 1;
    x += 1;

    lcd_plot( x+3, y+4, 1);
    lcd_line( x+2, y+2, x+4, y+2, 1);
    lcd_line( x+1, y+0, x+5, y+0, 1);
}




//##############################################################
//# spezielle Beeps
//##############################################################

//--------------------------------------------------------------
// Beep_Waypoint()
//
// Beep bei Waypoint Wechsel und wenn die WP-Liste abgearbeitet
// ist
//--------------------------------------------------------------
void Beep_Waypoint( void )
{
    //-----------------
    // BEEP: WP erreicht
    //-----------------
    if( naviData->WaypointIndex > WP_old )
    {
        set_beep( 30, 0xffff, BeepNormal );                         // kurzer Bestaetigungs-Beep
    }

    if(    (naviData->WaypointIndex == naviData->WaypointNumber)
        && (naviData->NCFlags & NC_FLAG_TARGET_REACHED)
        && (!WP_last)
      )
    {
        set_beep( 400, 0xffff, BeepNormal );                        // langer Bestaetigungs-Beep
        WP_last = true;
    }

    if( naviData->WaypointIndex != WP_old )
    {
        WP_old = naviData->WaypointIndex;
    }

    if(     (naviData->WaypointIndex != naviData->WaypointNumber)   // wenn aktueller WP != WP-Anzahl -> WP_last abschalten
        || !(naviData->NCFlags & NC_FLAG_TARGET_REACHED)            // wenn kein TR mehr an ist -> WP_last abschalten
      )
    {
        WP_last = false;
    }


    // alter Code zur Orientierung
    //if( OldWP != naviData->WaypointIndex )
    //{
    //    OldWP = naviData->WaypointIndex;
    //    NextWP = true;
    //}
    //
    //if( (NextWP==true) && (naviData->NCFlags & NC_FLAG_TARGET_REACHED) )
    //{
    //    set_beep( 25, 0xffff, BeepNormal );                       // kurzer Bestaetigungs-Beep
    //    NextWP = false;
    //}
}



//##############################################################
//# OSD-ELEMENTS
//##############################################################

//--------------------------------------------------------------
// OSD_Element_Flag_Label( xC, yC, item, lOn, xoffs, yoffs)
//
// xC, yC       : x,y in Characters
// item         : OSD_FLAG_AH, OSD_FLAG_PH, usw.
// lOn          : true / false
// xoffs,yoffs  : x,y Pixelverschiebung
//--------------------------------------------------------------
void OSD_Element_Flag_Label( uint8_t xC, uint8_t yC, uint8_t item, uint8_t lOn, int8_t xoffs, int8_t yoffs)
{
    int8_t  x = (xC*6)-2;
    int8_t  y = (yC*8)-1;
    uint8_t w = 14;
    uint8_t h = 8;

    const char *labels[OSD_FLAG_COUNT] =
    {
        PSTR("AH"),  // OSD_FLAG_AH  Altitue Hold
        PSTR("PH"),  // OSD_FLAG_PH  Position Hold
        PSTR("CF"),  // OSD_FLAG_CF  Care Free
        PSTR("CH"),  // OSD_FLAG_CH  Coming Home
        PSTR("o1"),  // OSD_FLAG_O1  Out1
        PSTR("o2"),  // OSD_FLAG_O2  Out2
        PSTR("BA"),  // OSD_FLAG_BA  LowBat warning (MK)
        PSTR("CA"),  // OSD_FLAG_CA  Calibrate
        PSTR("ST"),  // OSD_FLAG_ST  Start
        PSTR("MR"),  // OSD_FLAG_MR  Motor Run
        PSTR("FY"),  // OSD_FLAG_FY  Fly
        PSTR("EL"),  // OSD_FLAG_EL  Emergency Landing
        PSTR("FS"),  // OSD_FLAG_FS  RX Failsave Active
        PSTR("GP"),  // OSD_FLAG_GP  GPS Ok
        PSTR("S!"),  // OSD_FLAG_S0  GPS-Sat not ok (GPS NOT ok)
        PSTR("TU"),  // OSD_FLAG_TU  Vario Trim Up
        PSTR("TD"),  // OSD_FLAG_TD  Vario Trim Down
        PSTR("FR"),  // OSD_FLAG_FR  Free
        PSTR("RL"),  // OSD_FLAG_RL  Range Limit
        PSTR("SL"),  // OSD_FLAG_SL  No Serial Link
        PSTR("TR"),  // OSD_FLAG_TR  Target Reached
        PSTR("MC")   // OSD_FLAG_MC  Manual Control
    };

    //lcd_plot( x-2, y-2, 1);                       // Referenz

    if( yC==0 && yoffs<=0 ) { y = 0;  h = 7;  }
    if( xC==0 && xoffs<=0 ) { x = 0;  w = 12; }

    if( lOn )
    {
        lcd_frect( x+xoffs, y+yoffs, w, h, 1);                          // Filler
        lcdx_printp_at(  xC, yC, labels[item], MINVERS, xoffs,yoffs);   // Label
    }
    else
    {
        lcd_frect( x+xoffs, y+yoffs, w, h, 0);                          // clear
    }
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void OSD_Element_Flag( uint8_t xC, uint8_t yC, uint8_t item, int8_t xoffs, int8_t yoffs)
{
    uint8_t lOn = 0;

    // FC_StatusFlags 0.88
    switch( item )
    {
        // Altitue Hold
        case OSD_FLAG_AH :  lOn = (naviData->FCStatusFlags2 & FC_STATUS2_ALTITUDE_CONTROL);
                                break;

        // Position Hold
        case OSD_FLAG_PH :  lOn = (naviData->NCFlags & NC_FLAG_PH);
                                break;

        // Coming Home
        case OSD_FLAG_CH :  lOn = (naviData->NCFlags & NC_FLAG_CH);
                                break;

        // Care Free
        case OSD_FLAG_CF :  lOn = (naviData->FCStatusFlags2 & FC_STATUS2_CAREFREE);
                                break;

        // Out1
        case OSD_FLAG_O1 :  lOn = (naviData->FCStatusFlags2 & FC_STATUS2_OUT1_ACTIVE);
                                break;

        // Out2
        case OSD_FLAG_O2 :  lOn = (naviData->FCStatusFlags2 & FC_STATUS2_OUT2_ACTIVE);
                                break;

        //  LowBat warning (MK)
        case OSD_FLAG_BA :  lOn = (naviData->FCStatusFlags & FC_STATUS_LOWBAT);
                                break;

        //  Calibrate
        case OSD_FLAG_CA :  lOn = (naviData->FCStatusFlags & FC_STATUS_CALIBRATE);
                                break;

        //  Start
        case OSD_FLAG_ST :  lOn = (naviData->FCStatusFlags & FC_STATUS_START);
                                break;

        //  Motor Run
        case OSD_FLAG_MR :  lOn = (naviData->FCStatusFlags & FC_STATUS_MOTOR_RUN);
                                break;

        //  Fly
        case OSD_FLAG_FY :  lOn = (naviData->FCStatusFlags & FC_STATUS_FLY);
                                break;

        //  Emergency Landing
        case OSD_FLAG_EL :  lOn = (naviData->FCStatusFlags & FC_STATUS_EMERGENCY_LANDING);
                                break;

        //  RC Failsave Active
        case OSD_FLAG_FS :  lOn = (naviData->FCStatusFlags2 & FC_STATUS2_RC_FAILSAVE_ACTIVE);
                                break;

        //  GPS ok
        case OSD_FLAG_GP :  lOn = (naviData->NCFlags & NC_FLAG_GPS_OK);
                                break;

        //  GPS-Sat not ok (GPS NOT ok)
        case OSD_FLAG_S0 :  lOn = !(naviData->NCFlags & NC_FLAG_GPS_OK);
                                break;

        //  Vario Trim Up
        case OSD_FLAG_TU :  lOn = (naviData->FCStatusFlags & FC_STATUS_VARIO_TRIM_UP);
                                break;

        //  Vario Trim Down
        case OSD_FLAG_TD :  lOn = (naviData->FCStatusFlags & FC_STATUS_VARIO_TRIM_DOWN);
                                break;

        //  Free
        case OSD_FLAG_FR :  lOn = (naviData->NCFlags & NC_FLAG_FREE);
                                break;

        //  Range Limit
        case OSD_FLAG_RL :  lOn = (naviData->NCFlags & NC_FLAG_RANGE_LIMIT);
                                break;

        //  No Serial Link
        case OSD_FLAG_SL :  lOn = (naviData->NCFlags & NC_FLAG_NOSERIALLINK);
                                break;

        //  Target Reached
        case OSD_FLAG_TR :  lOn = (naviData->NCFlags & NC_FLAG_TARGET_REACHED);
                                break;

        //  Manual Control
        case OSD_FLAG_MC :  lOn = (naviData->NCFlags & NC_FLAG_MANUAL_CONTROL);
                                break;

    }

    OSD_Element_Flag_Label( xC, yC, item, lOn, xoffs,yoffs);
}


//--------------------------------------------------------------
// Anzeige von Steigen / Sinken
//--------------------------------------------------------------
void OSD_Element_UpDown( uint8_t x, uint8_t y, int8_t xoffs, int8_t yoffs)
{
    x = (x*6) + xoffs;
    y = (y*8) + yoffs;

    lcd_frect( x, y, 6, 7, 0);              // clear

    if( naviData->Variometer > 2 )          // steigen mehr als 0.2 m/sec (ein guter Wert muss noch in der Praxis ermittelt werden)
    {
        lcd_line( x+2, y+0, x+0, y+2, 1);
        lcd_line( x+2, y+0, x+4, y+2, 1);
    }
    else if( naviData->Variometer < -2 )    // sinken  mehr als 0.2 m/sec
    {
        lcd_line( x+2, y+6, x+0, y+4, 1);
        lcd_line( x+2, y+6, x+4, y+4, 1);
    }

}


//--------------------------------------------------------------
// OSD_Element_Altitude( x, y, nStyle)
// nStyle entspricht dem ehemaligen 'Mode'
//--------------------------------------------------------------
void OSD_Element_Altitude( uint8_t x, uint8_t y, uint8_t nStyle )
{

#ifdef USE_FONT_BIG

    switch( nStyle )
    {
        case 0 :
        case 1 :    //note:lephisto:according to several sources it's /30
                    if (naviData->Altimeter > (300 / AltimeterAdjust) || naviData->Altimeter < (-300 / AltimeterAdjust))    // above 10m only write full meters
                             write_ndigit_number_s (x, y, naviData->Altimeter / (30 / AltimeterAdjust), 4, 0,MNORMAL);
                    else    // up to 10m write meters.dm
                             write_ndigit_number_s_10th (x, y, naviData->Altimeter / (3 / AltimeterAdjust), 3, 0,MNORMAL);

                    lcd_printp_at (x+4, y, PSTR("m"), MNORMAL);
                    lcd_putc (x+5, y, 0x09, 0);  // Hoehensymbol
                    break;

        case 2 :    //note:lephisto:according to several sources it's /30
                    if (naviData->Altimeter > (300 / AltimeterAdjust) || naviData->Altimeter < (-300 / AltimeterAdjust))        // above 10m only write full meters
                        write_ndigit_number_s (x+4, y, naviData->Altimeter / (30 / AltimeterAdjust), 4, 0,MBIG);
                    else        // up to 10m write meters.dm
                        write_ndigit_number_s_10th (x+4, y, naviData->Altimeter / (3 / AltimeterAdjust), 3, 0,MBIG);
                    lcd_putc( x+8, y, 'm', MBIG);
                    lcd_printp_at (x, y, PSTR("Höhe"), MNORMAL);
                    break;
    }

#else

    if( nStyle == 2 )
    {
        lcd_printp_at (x, y, PSTR("Höhe"), MNORMAL);
        x += 4;
    }
    else
    {
        lcd_putc (x+5, y, 0x09, 0);  // Hoehensymbol
    }

    if (naviData->Altimeter > (300 / AltimeterAdjust) || naviData->Altimeter < (-300 / AltimeterAdjust))    // above 10m only write full meters
        write_ndigit_number_s (x, y, naviData->Altimeter / (30 / AltimeterAdjust), 4, 0,MNORMAL);
    else    // up to 10m write meters.dm
        write_ndigit_number_s_10th (x, y, naviData->Altimeter / (3 / AltimeterAdjust), 3, 0,MNORMAL);

    lcd_printp_at (x+4, y, PSTR("m"), MNORMAL);

#endif

}


//--------------------------------------------------------------
// fuer: Config.OSD_LipoBar==1
//--------------------------------------------------------------
void OSD_Element_BatteryLevel_Bar( uint8_t x, uint8_t y )
{
    uint16_t Balken = 0;

    drawmode = (naviData->UBat < BattLowVoltageWarning ? 2 : 0);

    if( cells > 0 )  // LipobargraphAnzeige nur wenn Anzahl der Lipozellen bekannt sind
    {
        write_ndigit_number_u (x+6, y, cells, 1, 0, drawmode);
        lcd_printp_at (x+7, y, PSTR("S"), drawmode);

        if( cells==3 )
        {
            lcd_rect(x*6, y*8, 28, 7, 1);  // Rahmen
            Balken = ((naviData->UBat-(cells*MIN_CELL_VOLTAGE))*10)/12;
            if((Balken > 0) && (Balken <28)) lcd_frect((x*6)+1, (y*8)+1, Balken, 5, 1);     // Fuellung
            if(Balken <= 26) lcd_frect(Balken+(x*6)+1, (y*8)+1, 26-Balken, 5, 0);           // loeschen
        }

        if( cells==4 ||cells==5 )
        {
           lcd_rect(x*6, y*8, 30, 7, 1);  // Rahmen
           if (cells == 4) Balken = ((naviData->UBat-(cells*MIN_CELL_VOLTAGE))*10)/15;
           if (cells == 5) Balken = ((naviData->UBat-(cells*MIN_CELL_VOLTAGE))*10)/19;
           if ((Balken > 0) && (Balken <=29)) lcd_frect((x*6)+1, (y*8)+1, Balken, 5, 1);    // Fuellung
           if (Balken <= 27) lcd_frect(Balken+(x*6)+1, (y*8)+1, 28-Balken, 5, 0);           // loeschen
        }

      } // end if: cells > 0   (TODO: Anzeige fuer cells==0 implementieren)
}



//--------------------------------------------------------------
// fuer die neuen OSD-Screens
//--------------------------------------------------------------
void OSD_Element_BattLevel2( uint8_t x, uint8_t y, int8_t xoffs, int8_t yoffs )
{
    drawmode = (naviData->UBat < BattLowVoltageWarning ? MINVERS : MNORMAL);

    if( Config.OSD_ShowCellU )
    {
        // kalk. Einzelzellen Spannungsanzeige
        writex_ndigit_number_u_100th( x, y, (uint16_t)((naviData->UBat*10)/cells), 3, 0, drawmode, xoffs,yoffs);
        lcdx_printp_at( x+4, y, PSTR("v"), drawmode, xoffs+1,yoffs);                    // Einheit (Einzelzellenanzeige ca., berechnet)
    }
    else
    {
        // Gesamtspannungsanzeige
        writex_ndigit_number_u_10th( x, y, naviData->UBat, 3, 0, drawmode, xoffs,yoffs);
        lcdx_printp_at( x+4, y, PSTR("V"), drawmode, xoffs+1,yoffs);                    // Einheit (Gesamtspannung)
    }

    lcd_line( (x+4)*6, y*8, (x+4)*6, y*8+7, (drawmode==MINVERS ? 1 : 0) );              // filler zwischen Spannung und "V"

}


//--------------------------------------------------------------
// fuer: Config.OSD_LipoBar==0
// nStyle entspricht dem ehemaligen 'Mode'
//--------------------------------------------------------------
void OSD_Element_BatteryLevel_Text( uint8_t x, uint8_t y, uint8_t nStyle )
{
#ifdef USE_FONT_BIG
    if( nStyle <= 1)
        drawmode = (naviData->UBat < BattLowVoltageWarning ? 2 : 0);  // Normal-Schrift
    else
        drawmode = (naviData->UBat < BattLowVoltageWarning ? 4 : 3);  // Fett-Schrift

    if( nStyle <= 1)
    {
        write_ndigit_number_u_10th (x, y, naviData->UBat, 3, 0, drawmode);
        lcd_printp_at (x+4, y, PSTR("V"), drawmode);
    }
    else
    {
        write_ndigit_number_u_10th (x-2, y, naviData->UBat, 3, 0, drawmode);
        lcd_printp_at (x+2, y, PSTR("V"), drawmode);
    }
#else
    drawmode = (naviData->UBat < BattLowVoltageWarning ? 2 : 0);  // Normal-Schrift

    if( nStyle == 2)
        x += 3;
    write_ndigit_number_u_10th (x, y, naviData->UBat, 3, 0, drawmode);
    lcd_printp_at (x+4, y, PSTR("V"), drawmode);
#endif
}


//--------------------------------------------------------------
// OSD_Element_Battery_Bar( x, y, length, width, orientation)
//
// neue Variante (OG 06/2013)
//
// Parameter:
//  length     : Gesamtlaenge des Bar's
//  width      : Breite in Pixel (fest)
//  orientation: ORIENTATION_V - Vertikal (x,y oberer,linker Punkt)
//               ORIENTATION_H
//--------------------------------------------------------------
void OSD_Element_Battery_Bar( uint8_t x, uint8_t y, uint8_t length, uint8_t width, uint8_t orientation)
{
    uint8_t bat_umax;
    uint8_t bat_umin;
    uint8_t bat_uact;
    int8_t  bat_p;

    // die Min/Max Spannungswerte sind jetzt erstmal fest vorgegeben bzw. berechnet, eine individuelle
    // Anpassung waere moeglich aber wenn besser ist es, wenn der User das nicht machen muesste...
    bat_umax = cells * 42;
    //bat_umin = cells * 32;
    bat_umin = cells * 34;          // 3.4 Volt pro Zelle Minimum

    bat_uact = naviData->UBat;

    bat_p = length * (bat_uact-bat_umin) / (bat_umax-bat_umin);

    if( bat_p < 0 )      bat_p = 0;
    if( bat_p > length ) bat_p = length;

    if( width == 1 )     width = 0;     // eine kleine Eigenart von frect um es zu ueberreden eine Linie zu zeichnen

    if( orientation == ORIENTATION_V )
    {
        lcd_frect( x, y             , width, length-bat_p, 0);  // clear: vertical
        lcd_frect( x, y+length-bat_p, width,        bat_p, 1);  // draw: vertical
    }
    else
    {
        lcd_frect( x+bat_p, y, length-bat_p, width,  0);    // clear:
        lcd_frect( x      , y, bat_p       , width,  1);    // draw:
    }
}


//--------------------------------------------------------------
// nStyle entspricht dem ehemaligen 'Mode'
//--------------------------------------------------------------
void OSD_Element_BatteryLevel( uint8_t x, uint8_t y, uint8_t nStyle )
{
    if( Config.OSD_LipoBar )
        OSD_Element_BatteryLevel_Bar( x, y);
    else
        OSD_Element_BatteryLevel_Text( x, y, nStyle);
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void OSD_Element_Capacity( uint8_t x, uint8_t y )
{
    drawmode = (naviData->UsedCapacity > Config.OSD_mAh_Warning ? 2 : 0);

    write_ndigit_number_u (x, y, naviData->UsedCapacity, 5, 0, drawmode);
    lcd_printp_at (x+5, y, PSTR("mAh"), drawmode);
    // BeepTime = 3000;
    // BeepMuster = 0x0020;
}



//--------------------------------------------------------------
// OSD_Element_CompassDegree( x, y, nStyle)
// nStyle entspricht dem ehemaligen 'Mode'
//--------------------------------------------------------------
void OSD_Element_CompassDegree( uint8_t x, uint8_t y, uint8_t nStyle )
{
    switch( nStyle )
    {
        case 0 :
        case 1 : write_ndigit_number_u (x, y, (naviData->CompassHeading)%360, 3, 0,MNORMAL);
                 x += 3;
                 break;

        case 2 :
                 #ifdef USE_FONT_BIG
                    write_ndigit_number_u (x, y, (naviData->CompassHeading)%360, 3, 0,MBIG);
                 #else
                    write_ndigit_number_u (x+5, y, (naviData->CompassHeading)%360, 3, 0,MNORMAL);
                 #endif

                 x += 8;
                 break;
    }
    lcd_putc( x, y, 0x1E, MNORMAL);     // degree symbol
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void OSD_Element_CompassDirection( uint8_t x, uint8_t y, int8_t xoffs, int8_t yoffs )
{
    lcdx_printp_at (x, y, (const char *) (pgm_read_word ( &(directions_p[heading_conv((naviData->CompassHeading)%360)]))), MNORMAL, xoffs,yoffs);
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void OSD_Element_CompassRose( uint8_t x, uint8_t y )
{
    draw_compass (x, y, (naviData->CompassHeading)%360, 0,0);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void OSD_Element_Current( uint8_t x, uint8_t y )
{
    write_ndigit_number_u_10th (x, y, naviData->Current, 3, 0,0);
    lcd_printp_at (x+4, y, PSTR("A"), 0);
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void OSD_Element_FlyingTime( uint8_t x, uint8_t y )
{
    write_time (x, y, naviData->FlyingTime);
    lcd_printp_at (x+5, y, PSTR("m"), 0);
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void OSD_Element_GroundSpeed( uint8_t x, uint8_t y )
{
    write_ndigit_number_u (x, y, (uint16_t) (((uint32_t) naviData->GroundSpeed * (uint32_t) 9) / (uint32_t) 250), 3, 0,0);
    lcd_printp_at (x+3, y, PSTR("Kmh"), 0);
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void OSD_Element_HomeCircle( uint8_t x, uint8_t y, uint8_t breite, int8_t rOffset, int8_t xoffs, int8_t yoffs )
{
    calc_heading_home();

    lcd_o_circle( x, y, breite, 1, xoffs,yoffs);
    lcd_o_circ_line( x, y, breite, (old_hh       + 180), rOffset, 0, xoffs,yoffs);
    lcd_o_circ_line( x, y, breite, (heading_home + 180), rOffset, 1, xoffs,yoffs);

    old_hh = heading_home;
}



//--------------------------------------------------------------
// NEU! 22.04.2014 OG
// soll das alte OSD_Element_HomeCircle() komplett ersetzen wenn
// die alten OSD-Screens endgueltig rausfliegen
//--------------------------------------------------------------
void OSD_Element_HomeCircleX( uint8_t px, uint8_t py, uint8_t rx, int8_t ry )
{
    calc_heading_home();
    lcd_ellipse_line( px, py, rx, ry, (old_hh       + 180), 0);
    lcd_ellipse_line( px, py, rx, ry, (heading_home + 180), 1);
    lcd_ellipse( px, py, rx, ry, 1);
    old_hh = heading_home;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void OSD_Element_HomeDegree( uint8_t x, uint8_t y )
{
    write_ndigit_number_u (x, y, heading_home, 3, 0,0);
    lcd_putc (x+3, y, 0x1e, 0); // degree symbol
}


//--------------------------------------------------------------
// OSD_Element_HomeDistance( x, y, nStyle)
//--------------------------------------------------------------
void OSD_Element_HomeDistance( uint8_t x, uint8_t y, uint8_t nStyle )
{
    switch( nStyle )
    {
        case 0 :
        case 1 : write_ndigit_number_u (x, y, naviData->HomePositionDeviation.Distance / 10, 3, 0,0);
                 lcd_putc (x+3, y, 'm', 0);
                 draw_homesymbol(x+4, y);
                 break;

        case 2 : lcd_printp_at (x, y, PSTR("Home"), 0);
                 write_ndigit_number_u (x+5, y, naviData->HomePositionDeviation.Distance / 10, 3, 0,0);
                 lcd_printp_at (x+8, y, PSTR("m -"), 0);
                 break;
    }
}


//--------------------------------------------------------------
// OSD_Element_LEDOutput( x, y, bitmask)
//
// bitmask: LED1 = FC_STATUS2_OUT1_ACTIVE
//          LED2 = FC_STATUS2_OUT2_ACTIVE
//--------------------------------------------------------------
void OSD_Element_LEDOutput( uint8_t x, uint8_t y, uint8_t bitmask )
{
    uint8_t lOn;

    lOn = (naviData->FCStatusFlags2 & bitmask ? 1 : 0);   // Bit gesetzt?
    lOn = (Config.OSD_InvertOut ? !lOn : lOn);            // Invertieren?
    lOn = (lOn ? 1 : 0);                                  // auf 0 oder 1 setzen

    lcd_fcircle (x * 6 + 5, y * 8 + 3, Config.OSD_LEDform, lOn);
    lcd_circle  (x * 6 + 5, y * 8 + 3, 3, 1);
}


//--------------------------------------------------------------
// OSD_Element_LED1Output( x, y)
//--------------------------------------------------------------
void OSD_Element_LED1Output( uint8_t x, uint8_t y )
{
    OSD_Element_LEDOutput( x, y, FC_STATUS2_OUT1_ACTIVE );
}


//--------------------------------------------------------------
// OSD_Element_LED2Output( x, y)
//--------------------------------------------------------------
void OSD_Element_LED2Output( uint8_t x, uint8_t y )
{
    OSD_Element_LEDOutput( x, y, FC_STATUS2_OUT2_ACTIVE );
}


//--------------------------------------------------------------
// OSD_Element_Manuell( x, y)
//--------------------------------------------------------------
void OSD_Element_Manuell( uint8_t x, uint8_t y )
{
    if (naviData->NCFlags & NC_FLAG_MANUAL_CONTROL)
        lcd_putc (x, y, 'M', 0); // rc transmitter
    else
        lcd_putc (x, y, 'X', 0); // clear
}



//--------------------------------------------------------------
// OSD_Element_RCIntensity( x, y)
//--------------------------------------------------------------
void OSD_Element_RCIntensity( uint8_t x, uint8_t y )
{
    write_ndigit_number_u (x, y, naviData->RC_Quality, 3, 0,0);
    //lcd_printp_at (x+3, y, PSTR("\x1F"), 0);        // RC-transmitter
    if (naviData->NCFlags & NC_FLAG_NOSERIALLINK)
    {
        lcd_printpns_at(x+3, y, PSTR("  "), 0);     // Clear
    }
    else
    {
        lcd_printpns_at(x+3, y, PSTR("PC"), 0);
    }
}


//--------------------------------------------------------------
// OSD_Element_SatsInUse( x, y, nStyle)
//
// nStyle == 0: "00s"
// nStyle == 1: wie 0
// nStyle == 2: "00 Sat"
//
// nStyle entspricht dem ehemaligen 'Mode'
//--------------------------------------------------------------
void OSD_Element_SatsInUse( uint8_t x, uint8_t y, uint8_t  nStyle )
{
    drawmode = (naviData->NCFlags & NC_FLAG_GPS_OK ? 0 : 2);

    switch( nStyle )
    {
        case 0 :
        case 1 : write_ndigit_number_u (x, y, naviData->SatsInUse, 2, 0, drawmode);
                 lcd_putc (x+2, y, 0x08, drawmode);
                 break;

        case 2 : write_ndigit_number_u (x, y, naviData->SatsInUse, 2, 0, drawmode);
                 lcd_printp_at (x+2, y, PSTR(" Sat"), drawmode);
                 break;
    }
}



//--------------------------------------------------------------
// OSD_Element_Variometer( x, y)
//--------------------------------------------------------------
void OSD_Element_Variometer( uint8_t x, uint8_t y )
{
    x *= 6;
    y *= 8;
    y += 7;

    draw_variometer (x, y, 10, 14, naviData->Variometer);
}


//--------------------------------------------------------------
// OSD_Element_Target( x, y, nStyle)
//
// nStyle entspricht dem ehemaligen 'Mode'
// nStyle = 0,1: "000m"
// nStyle = 2,3: "Ziel 000m -"
//--------------------------------------------------------------
void OSD_Element_Target( uint8_t x, uint8_t y, uint8_t nStyle )
{
    if( nStyle <= 1 )
    {
        write_ndigit_number_u (x, y, naviData->TargetPositionDeviation.Distance / 10, 3, 0,0);
        lcd_putc (x+3, y, 'm', 0);
        draw_targetsymbol(x+4,y);
    }
    else
    {
        lcd_printp_at (x, y, PSTR("Ziel"), 0);
        write_ndigit_number_u (x+5, y, naviData->TargetPositionDeviation.Distance / 10, 3, 0,0);
        lcd_printp_at (x+8, y, PSTR("m -"), 0);
    }
}


//--------------------------------------------------------------
// TODO:
// - pruefen ob beep hier an richtiger Stelle ist
//--------------------------------------------------------------
void OSD_Element_VarioWert( uint8_t x, uint8_t y )
{
    uint8_t FC_Fallspeed;

    FC_Fallspeed = (unsigned int)naviData->Variometer;
    FC_Fallspeed = 255-FC_Fallspeed;

    drawmode = ( (naviData->Variometer < 0) && (FC_Fallspeed > Config.OSD_Fallspeed) ? 2 : 0);


    if( Config.OSD_VarioBeep )
        Beep_Vario();                               // Beep ???

    if( drawmode == 2 )
    {
        if( !Config.HWSound )
            set_beep ( 1000, 0x0060, BeepNormal);   // muss ein Beep hier hin????
        else
            variobeep(naviData->Variometer);        // muss ein Beep hier hin????
    }

    write_ndigit_number_s_10th (x, y, naviData->Variometer, 3,0, drawmode);
    lcd_printpns_at(x+4, y, PSTR("ms"), drawmode);
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void OSD_Element_WayPoint( uint8_t x, uint8_t y )
{
    if (!OldWP == naviData->WaypointIndex)
    {
        // BeepTime = 500;
        // BeepMuster = 0x0080;
        OldWP = naviData->WaypointIndex;
        NextWP = true;
    }

    if ((NextWP==true)&& naviData->NCFlags & NC_FLAG_TARGET_REACHED)
    {
        set_beep ( 500, 0x0080, BeepNormal);
        NextWP = false;
    }

    write_ndigit_number_u (x+2, y, naviData->WaypointIndex , 2, 0,0);

    lcd_printp_at (x, y, PSTR("WP"), 0);
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void OSD_Element_TargetDegree( uint8_t x, uint8_t y )
{
    write_ndigit_number_u (x, y, naviData->TargetPositionDeviation.Bearing/ 10, 3, 0,0);
    lcd_putc (x+3, y, 0x1e, 0);     // degree symbol
}



//##############################################################
//# OSD-SCREENS
//##############################################################

//--------------------------------------------------------------
// OSD-Screen "General"
//
// OSDScreenRefresh:    0 = update values
//                  1 = redraw labels and update values
//--------------------------------------------------------------
void OSD_Screen_General( void )
{
    //-----------------------------------------
    // REDRAW
    // statische Screen Elemente die nicht
    // jedesmal neu gezeichnet werden muessen
    //-----------------------------------------
    if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    {
        // Display: 128 x 64 with 6x8 Font => 21 x 8

        // Linien: Horizontal
        lcd_line (0, 28, 127, 28, 1);  // mitte
        lcd_line (0, 51, 127, 51, 1);  // unten

        // Linien: Vertikal
        lcd_line (65, 0, 65, 50, 1);   // mitte

        //-----------------------------------------
        // Block: Oben - Links
        //-----------------------------------------
        draw_icon_battery(0,4);
        lcdx_printp_at( 7, 2, PSTR(" mA"), MNORMAL, 0,2);

        //-----------------------------------------
        // Block: Oben - Rechts
        //-----------------------------------------
        lcdx_printp_at( 12, 0, PSTR("Alt:") , MNORMAL, 0,0);
        lcdx_printp_at( 12, 1, PSTR("Dir:") , MNORMAL, 0,1);
        lcdx_putc( 20, 1, SYMBOL_SMALLDEGREE, MNORMAL, 1,1);
        lcdx_printp_at( 12, 2, PSTR("  I:") , MNORMAL, 0,2);
        lcdx_putc( 20, 2, 'A', MNORMAL, 2,2);

        //-----------------------------------------
        // Block: Unten - Links
        //-----------------------------------------
        draw_icon_sat(0,33);
        lcdx_printp_at( 6, 5, PSTR(" kmh"), MNORMAL, 0,1);

        //-----------------------------------------
        // Block: Unten - Rechts
        //-----------------------------------------
        draw_icon_home( 70, 32);
        lcdx_putc( 19, 4, 'm', MNORMAL, 2,0);
        lcdx_putc( 20, 4, '/', MNORMAL, 2,0);
//        lcdx_putc( 20, 5, SYMBOL_SMALLDEGREE, MNORMAL, 1,1);
        lcdx_putc( 19, 5, SYMBOL_SMALLDEGREE, MNORMAL, 1,1);    // verschoben wegen CamCtrl Anzeige
        //-----------------------------------------
        // unterste Zeile
        //-----------------------------------------
        draw_symbol_rc( 20, 7);         // RC-transmitter
        //lcdx_putc( 20, 7, SYMBOL_RCQUALITY, MNORMAL, 2,0);
    }

    //-----------------
    // Batt Level (Graph)
    //-----------------
    OSD_Element_Battery_Bar( 2, 8, 11, 2, ORIENTATION_V);

    //-----------------
    // Batt Level (Volt)
    //-----------------
    OSD_Element_BattLevel2( 2, 0, 0,0 );

    //-----------------
    // LowBat Warnung MK
    //-----------------
    OSD_Element_Flag( 8, 0, OSD_FLAG_BA, 0,0 );

    //-----------------
    // Flugzeit
    //-----------------
    writex_time(2, 1, naviData->FlyingTime, MNORMAL, 0,1);

    //-----------------
    // entnommene Kapazitaet (mAh)
    //-----------------
    drawmode = (naviData->UsedCapacity > Config.OSD_mAh_Warning ? MINVERS : MNORMAL);
    writex_ndigit_number_u( 2, 2, naviData->UsedCapacity, 5, 0, drawmode, 0,2);

    //-----------------
    // Höhe
    //-----------------
    writex_altimeter( 16, 0, naviData->Altimeter, MNORMAL, 0,0 );
/*
    if (naviData->Altimeter > (300 / AltimeterAdjust) || naviData->Altimeter < (-300 / AltimeterAdjust))    // above 10m only write full meters
        write_ndigit_number_s ( 16, 0, naviData->Altimeter / (30 / AltimeterAdjust), 4, 0, MNORMAL);
    else                                                                                                    // up to 10m write meters.dm
        write_ndigit_number_s_10th( 16, 0, naviData->Altimeter / (3 / AltimeterAdjust), 3, 0, MNORMAL);
*/

    //-----------------
    // steigen / sinken
    //-----------------
    OSD_Element_UpDown( 20, 0, 2,0);

    //-----------------
    // Compass Degree
    //-----------------
    writex_ndigit_number_u (17, 1, (naviData->CompassHeading)%360, 3, 0,MNORMAL, 0,1);

    //-----------------
    // Strom
    //-----------------
    //write_ndigit_number_u_10th( 16, 2, naviData->Current, 3, 0,0);  // alternativ mit Nachkomma
    writex_ndigit_number_u( 17, 2, naviData->Current/10, 3, 0,MNORMAL, 0,2);

    //-----------------
    // Sat Anzahl
    //-----------------
    write_ndigit_number_u (4, 4, naviData->SatsInUse, 2, 0,MNORMAL);

    //-----------------
    // Sat Warnung "!"
    //-----------------
    /*
    if( naviData->NCFlags & NC_FLAG_GPS_OK )
        lcd_printp_at( 9, 4, PSTR(" "), MNORMAL);
    else
        lcd_printp_at( 9, 4, PSTR("!"), MNORMAL);
    */
    OSD_Element_Flag(  8, 4, OSD_FLAG_S0, -1,0 );  // Sat Warnung (GPS not ok)

    //-----------------
    // Geschwindigkeit
    //-----------------
    writex_ndigit_number_u( 3, 5, (uint16_t) (((uint32_t) naviData->GroundSpeed * (uint32_t) 9) / (uint32_t) 250), 3, 0,MNORMAL, 0,1);

    //-----------------
    // Home Distance
    //-----------------
    write_ndigit_number_u( 16, 4, naviData->HomePositionDeviation.Distance / 10, 3, 0,MNORMAL);

    //-----------------
    // Home Winkel
    //-----------------
//    writex_ndigit_number_u( 16, 5, heading_home, 4, 0,MNORMAL, 0,1);
    writex_ndigit_number_u( 15, 5, heading_home, 4, 0,MNORMAL, 0,1);   // verschoben wegen CamCtrl Anzeige
    //-----------------
    // Flags
    //-----------------
    OSD_Element_Flag(  1, 7, OSD_FLAG_CF, 0,0 );  // Care Free
    OSD_Element_Flag(  4, 7, OSD_FLAG_AH, 0,0 );  // Altitude Hold
    OSD_Element_Flag(  7, 7, OSD_FLAG_PH, 0,0 );  // Position Hold
    OSD_Element_Flag( 10, 7, OSD_FLAG_CH, 0,0 );  // Coming Home
    OSD_Element_Flag( 13, 7, OSD_FLAG_EL, 0,0 );  // Emergency Landing

    //-----------------
    // RC-Quality (MK)
    //-----------------
    write_ndigit_number_u( 17, 7, naviData->RC_Quality, 3, 0,MNORMAL);

    #ifdef USE_OSD_DEMO
        //-----------------
        // Flags
        //-----------------
        OSD_Element_Flag_Label(  8, 0, OSD_FLAG_BA, true, 0,0 );    // DEMO: Batterie Warnung
        OSD_Element_Flag_Label(  8, 4, OSD_FLAG_S0, true, -1,0 );   // DEMO: Sat Warnung (GPS not ok)

        OSD_Element_Flag_Label(  1, 7, OSD_FLAG_CF, true, 0,0 );    // DEMO
        OSD_Element_Flag_Label(  4, 7, OSD_FLAG_AH, true, 0,0 );    // DEMO
        OSD_Element_Flag_Label(  7, 7, OSD_FLAG_PH, true, 0,0 );    // DEMO
        OSD_Element_Flag_Label( 10, 7, OSD_FLAG_CH, true, 0,0 );    // DEMO
        OSD_Element_Flag_Label( 13, 7, OSD_FLAG_EL, true, 0,0 );    // DEMO
    #endif
}



//--------------------------------------------------------------
// OSD-Screen "Navigation"
//
// OSDScreenRefresh:    0 = update values
//                  1 = redraw labels and update values
//--------------------------------------------------------------
#ifdef USE_OSD_SCREEN_NAVIGATION
void OSD_Screen_Navigation( void )
{
    int8_t xoffs, yoffs;
    int16_t degree;
    uint8_t minus;

    //-----------------------------------------
    // REDRAW
    // statische Screen Elemente die nicht
    // jedesmal neu gezeichnet werden muessen
    //-----------------------------------------
    if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    {
        // do things here for static screen elements like labels and so....

        lcd_line(  (6*6-3),  0,  (6*6-3), 11, 1);                   // Linie Vertikal links
        lcd_line( (15*6+5),  0, (15*6+5), 11, 1);                   // Linie Vertikal rechts
        lcd_line(        0, 12,      127, 12, 1);                   // Linie Horizontal

        lcdx_printp_at( 0, 2, PSTR("Alt:"), MNORMAL, 0,2);          // Hoehe
        lcdx_printp_at( 0, 5, PSTR("Home:"), MNORMAL, 0,3);         // Home Distance
    }


    //-----------------
    // Oben: Batt Level (Volt)
    //-----------------
    OSD_Element_BattLevel2( 0, 0, 0,0 );

    //-----------------
    // Oben: Batt Level Bar
    //-----------------
    //OSD_Element_Battery_Bar( x, y, length, width, orientation)
    OSD_Element_Battery_Bar( 0, 9, 30, 1, ORIENTATION_H);

    //-----------------
    // Oben: Kompass Rose
    //-----------------
    draw_compass( 6, 0, (naviData->CompassHeading)%360, 2,0);

    //-----------------
    // Oben: Flugzeit
    //-----------------
    writex_time(16, 0, naviData->FlyingTime, MNORMAL, 2,0);

    //-----------------
    // Hoehe
    //-----------------
    xoffs = 0;
    yoffs = 3;
    writex_altimeter( 0, 3, naviData->Altimeter, MNORMAL, xoffs,yoffs );

    //-----------------
    // Steigen / Sinken
    //-----------------
    OSD_Element_UpDown( 4, 3, 1,yoffs);

    //-----------------
    // Home Distance
    //-----------------
    yoffs = 3;
    writex_ndigit_number_u( 0, 6, naviData->HomePositionDeviation.Distance / 10, 4, 0,MNORMAL, 0,yoffs+1);
    lcdx_printp_at( 4, 6, PSTR("m"), MNORMAL, 2,yoffs+1);       // Home

    //-----------------
    // Home Circle
    //-----------------

//void OSD_Element_HomeCircleX( uint8_t px, uint8_t py, uint8_t rx, int8_t ry )

    xoffs = 2;
    yoffs = 3;

    //OSD_Element_HomeCircleX( 64, 38, 25, 22 );                            // entspricht dem ehemaligem Huellkreis
    OSD_Element_HomeCircleX( 64, 38, 26, 22 );                              // leicht erweiterter Huellkreis

    lcd_frect( (9*6)-3+xoffs, (4*8)-2+yoffs, (3*6)+4, (1*8)+2, 0);          // inner clear
    lcd_rect ( (9*6)-4+xoffs, (4*8)-3+yoffs, (3*6)+6, (1*8)+4, 1);          // inner rect
    lcd_frect(  61+xoffs, 57+yoffs, 2, 2, 1);                               // bottom mini rect

    degree  = (int16_t)heading_home;
    minus   = false;
    if( degree >= 180 ) degree = 360 - degree;
    else                minus = true;

    writex_ndigit_number_u( 9, 4, degree, 3, 0,MNORMAL, xoffs+1,yoffs);         // Degree (Winkel)

    if( minus && degree != 0 )
        lcd_line( (9*6)-2+xoffs, (4*8)+3+yoffs, (9*6)-1+xoffs, (4*8)+3+yoffs, 1);


    //-----------------
    // Variometer
    //-----------------
    // OG: Variometer wird erstmal nicht angezeigt weil es den Screen zu voll macht
    //     wenn doch muessen erst grafische Anpassungen an den Variometer-Code gemacht
    //     werden
    //void draw_variometer (uint8_t x, uint8_t y, uint8_t width, uint8_t hight, int16_t variometer)
    //draw_variometer( 95, 38,  7, 30, naviData->Variometer);
    //draw_variometer( 94, 38,  7, 21, naviData->Variometer);
    //draw_variometer2( 94, 28,  7, 21, naviData->Variometer);

    //-----------------
    // Flags
    //-----------------
    OSD_Element_Flag(  16, 2, OSD_FLAG_BA, -3, 0);  // MK Batt Warning
    OSD_Element_Flag(  19, 2, OSD_FLAG_CF,  0, 0);  // Carefree
    OSD_Element_Flag(  19, 4, OSD_FLAG_AH,  0,-3);  // Altitude Hold
    OSD_Element_Flag(  19, 6, OSD_FLAG_PH,  0,-6);  // Position Hold
    OSD_Element_Flag(  19, 7, OSD_FLAG_CH,  0,-1);  // Coming Home
    OSD_Element_Flag(  16, 7, OSD_FLAG_S0, -3,-1);  // GPS-Sat not ok (GPS NOT ok)


    #ifdef USE_OSD_DEMO
        //-----------------
        // Flags
        //-----------------
        OSD_Element_Flag_Label(  16, 2, OSD_FLAG_BA, true, -3,0);   // DEMO

        OSD_Element_Flag_Label(  19, 2, OSD_FLAG_CF, true, 0,0);    // DEMO
        OSD_Element_Flag_Label(  19, 4, OSD_FLAG_AH, true, 0,-3);   // DEMO
        OSD_Element_Flag_Label(  19, 6, OSD_FLAG_PH, true, 0,-6);   // DEMO
        OSD_Element_Flag_Label(  19, 7, OSD_FLAG_CH, true, 0,-1);   // DEMO

        OSD_Element_Flag_Label(  16, 7, OSD_FLAG_S0, true, -3,-1);  // DEMO
    #endif
}
#endif // USE_OSD_SCREEN_NAVIGATION




//--------------------------------------------------------------
// OSD-Screen "OSD_Screen_Waypoints_OLD"
//
// alte, alternative Variante - wenn sich die neue Variante
// durchsetzt kann das hier geloescht werden
//--------------------------------------------------------------
#ifdef USE_OSD_SCREEN_WAYPOINTS
/*
void OSD_Screen_Waypoints_OLD( void )
{
    int8_t xoffs, yoffs;

    //-----------------------------------------
    // REDRAW
    // statische Screen Elemente die nicht
    // jedesmal neu gezeichnet werden muessen
    //-----------------------------------------
    if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    {
        // do things here for static screen elements like labels and so....

        lcd_line(  (6*6-3),  0,  (6*6-3), 11, 1);                       // Linie Vertikal links
        lcd_line( (15*6+5),  0, (15*6+5), 11, 1);                       // Linie Vertikal rechts
        //lcd_line(        0, 12,      127, 12, 1);                     // Linie Horizontal

        lcd_rect_round( 0, 12, 127, 63-19+7, 1, R2);                    // Rahmen

        lcd_line(        0, 12+25,      127, 12+25, 1);                 // Trennlinie in der Mitte

        draw_icon_home( 7, 18);
        draw_icon_target_round( 7, 45);
        //draw_icon_target_diamond( 7, 45);                             // Alternative fuer draw_icon_target_round()
    }


    //-----------------
    // Oben: Batt Level (Volt)
    //-----------------
    OSD_Element_BattLevel2( 0, 0, 0,0 );

    //-----------------
    // Oben: Batt Level Bar
    //-----------------
    //OSD_Element_Battery_Bar( x, y, length, width, orientation)
    OSD_Element_Battery_Bar( 0, 9, 30, 1, ORIENTATION_H);

    //-----------------
    // Oben: Kompass Rose
    //-----------------
    //draw_compass( 6, 0, (naviData->CompassHeading)%360, 2,0);

    //-----------------
    // Oben: Flugzeit
    //-----------------
    writex_time(16, 0, naviData->FlyingTime, MNORMAL, 2,0);

    //-----------------
    // Home: Hoehe
    //-----------------
    xoffs = 6;
    yoffs = 0;
    writex_altimeter( 7, 2, naviData->Altimeter, MNORMAL, xoffs,yoffs );
    lcdx_printp_at( 3, 2, PSTR("Al:"), MNORMAL, xoffs+2,yoffs);

    //-----------------
    // Home: Steigen / Sinken
    //-----------------
    OSD_Element_UpDown( 13, 2, -2,yoffs);

    //-----------------
    // Home: Distance
    //-----------------
    yoffs = 2;
    lcdx_printp_at( 3, 3, PSTR("Ho:"), MNORMAL, xoffs+2,yoffs);
    writex_ndigit_number_u( 7, 3, naviData->HomePositionDeviation.Distance / 10, 4, 0,MNORMAL, xoffs,yoffs);
    lcdx_printp_at(11, 3, PSTR("m"), MNORMAL, xoffs+4,yoffs);       // Home

    //-----------------
    // Home: Circle
    //-----------------
    xoffs = 2;
    yoffs = 3;

  //OSD_Element_HomeCircleX( px, py, rx, ry )
  //OSD_Element_HomeCircleX( 64, 38, 26, 22 );                              // leicht erweiterter Huellkreis
    OSD_Element_HomeCircleX( 112, 24, 10, 8 );                              // leicht erweiterter Huellkreis

  //lcd_frect(  112-1, 33, 2, 1, 1);                               // bottom mini rect
    lcd_frect(  112-1, 33, 2, 0, 1);                               // bottom mini rect
    lcd_frect(  112-0, 33, 0, 1, 1);                               // bottom mini rect


    //-----------------
    // WP: Waypoint-Index und Anzahl der Waypoint's
    //-----------------
    xoffs = 6;
    yoffs = 2;
    lcdx_printp_at( 3, 5, PSTR("WP:"), MNORMAL, xoffs+2,yoffs);

    uint8_t wpindex = naviData->WaypointIndex;
    if( naviData->WaypointNumber==0 ) wpindex = 0;

    lcdx_printf_at_P( 7, 5, MNORMAL, xoffs,yoffs, PSTR("%2d/%2d"), wpindex, naviData->WaypointNumber );


    //-----------------
    // Sat Anzahl
    //-----------------
    writex_ndigit_number_u( 17, 5, naviData->SatsInUse, 2, 0,MNORMAL,  0,2);
    draw_icon_satmini( 117, 42);


    //-----------------
    // WP: Distance
    //-----------------
    yoffs = 4;
    lcdx_printp_at( 3, 6, PSTR("Di:"), MNORMAL, xoffs+2,yoffs);
    writex_ndigit_number_u( 7, 6, naviData->TargetPositionDeviation.Distance / 10, 4, 0,MNORMAL, xoffs,yoffs);
    lcdx_printp_at( 11, 6, PSTR("m"), MNORMAL, xoffs+4,yoffs);       //

    //-----------------
    // WP: Hoehe
    // Anmerkung OG: macht nicht so wirklich Sinn denke ich
    //-----------------
  //writex_ndigit_number_u( 14, 6, naviData->TargetPosition.Altitude / 1000, 4, 0,MNORMAL, xoffs,yoffs);
  //lcdx_printp_at( 18, 6, PSTR("m"), MNORMAL, xoffs+4,yoffs);       //


    //-----------------
    // Oben: Flags
    //-----------------
    // Variante 1: PH, CH, CF
  //OSD_Element_Flag(  7, 0, OSD_FLAG_PH, -2,1);  // Position Hold
  //OSD_Element_Flag(  9, 0, OSD_FLAG_CH,  5,1);  // Coming Home
  //OSD_Element_Flag( 12, 0, OSD_FLAG_CF,  6,1);  // Carefree

    // Variante 2: CF, CH, TR
    OSD_Element_Flag(  7, 0, OSD_FLAG_CF, -2,1);  // Carefree
    OSD_Element_Flag(  9, 0, OSD_FLAG_CH,  5,1);  // Coming Home
    OSD_Element_Flag( 12, 0, OSD_FLAG_TR,  6,1);  // Target Reached


    // Variante: TR neben Waypoint-Anzahl
  //OSD_Element_Flag( 13, 5, OSD_FLAG_TR,  6,2);  // Target Reached


    //-----------------
    // ggf. BEEP wenn WP erreicht
    //-----------------
    Beep_Waypoint();
}
*/
#endif // USE_OSD_SCREEN_WAYPOINTS



//--------------------------------------------------------------
// OSD-Screen "Waypoints"
//
// aktuelle Variante!
//--------------------------------------------------------------
#ifdef USE_OSD_SCREEN_WAYPOINTS
void OSD_Screen_Waypoints( void )
{
    int8_t  xoffs, yoffs;
    uint8_t v;

    //-----------------------------------------
    // REDRAW
    // statische Screen Elemente die nicht
    // jedesmal neu gezeichnet werden muessen
    //-----------------------------------------
    if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    {
        // do things here for static screen elements like labels and so....

        lcd_line(  (6*6-3),  0,  (6*6-3), 11, 1);                       // Linie Vertikal links
        lcd_line( (15*6+5),  0, (15*6+5), 11, 1);                       // Linie Vertikal rechts
        //lcd_line(        0, 12,      127, 12, 1);                     // Linie Horizontal

        lcd_rect_round( 0, 12, 127, 63-19+7, 1, R2);                    // Rahmen

        lcd_line(       0, 12+25, (15*6+5), 12+25, 1);                  // Trennlinie in der Mitte
        lcd_line( (15*6+5),  12+25, (15*6+5), 63, 1);                   // Linie Vertikal rechts, unten
        lcd_plot ((15*6+5),  12+25, 0);

        draw_icon_home( 7, 18);
        draw_icon_target_round( 7, 45);
        //draw_icon_target_diamond( 7, 45);                             // Alternative fuer draw_icon_target_round()
    }


    //-----------------
    // Oben,Links: Batt Level (Volt)
    //-----------------
    OSD_Element_BattLevel2( 0, 0, 0,0 );

    //-----------------
    // Oben,Links: Batt Level Bar
    //-----------------
    //OSD_Element_Battery_Bar( x, y, length, width, orientation)
    OSD_Element_Battery_Bar( 0, 9, 30, 1, ORIENTATION_H);

    //-----------------
    // Oben,Rechts: Flugzeit
    //-----------------
    writex_time(16, 0, naviData->FlyingTime, MNORMAL, 2,0);

    //-----------------
    // Oben,Mitte: Flags
    //-----------------
    // Variante 1: PH, CH, CF
  //OSD_Element_Flag(  7, 0, OSD_FLAG_PH, -2,1);  // Position Hold
  //OSD_Element_Flag(  9, 0, OSD_FLAG_CH,  5,1);  // Coming Home
  //OSD_Element_Flag( 12, 0, OSD_FLAG_CF,  6,1);  // Carefree

    // Variante 2: CF, CH, TR
    OSD_Element_Flag(  7, 0, OSD_FLAG_CF, -2,1);  // Carefree
    OSD_Element_Flag(  9, 0, OSD_FLAG_CH,  5,1);  // Coming Home
    OSD_Element_Flag( 12, 0, OSD_FLAG_TR,  6,1);  // Target Reached



    //-----------------
    // Home: Hoehe
    //-----------------
    xoffs = 5;
    yoffs = 0;
    writex_altimeter( 7, 2, naviData->Altimeter, MNORMAL, xoffs,yoffs );
    lcdx_printp_at( 3, 2, PSTR("Al:"), MNORMAL, xoffs+3,yoffs);

    //-----------------
    // Home: Steigen / Sinken
    //-----------------
    OSD_Element_UpDown( 13, 2, -3,yoffs);

    //-----------------
    // Home: Distance
    //-----------------
    yoffs = 2;
    lcdx_printp_at( 3, 3, PSTR("Ho:"), MNORMAL, xoffs+3,yoffs);
    writex_ndigit_number_u( 7, 3, naviData->HomePositionDeviation.Distance / 10, 4, 0,MNORMAL, xoffs,yoffs);
    lcdx_printp_at(11, 3, PSTR("m"), MNORMAL, xoffs+4,yoffs);       // Home

    //-----------------
    // Home: Circle
    //-----------------
    // orginal
    //OSD_Element_HomeCircleX( 110, 25, 11, 9 );                     // leicht erweiterter Huellkreis
    //lcd_frect(  112-3, 35, 2, 1, 1);                               // bottom mini rect

    // 1 pixel weiter links
    OSD_Element_HomeCircleX( 109, 25, 11, 9 );                     // leicht erweiterter Huellkreis
    lcd_frect(  112-4, 35, 2, 1, 1);                               // bottom mini rect

    // etwas groesser
    //OSD_Element_HomeCircleX( 109, 26, 12, 10 );                     // leicht erweiterter Huellkreis
    //lcd_frect(  112-4, 37, 2, 1, 1);                               // bottom mini rect

    //-----------------
    // Home: Sat Anzahl
    //-----------------
    //yoffs = -2; // alternativ
    yoffs = 0;
    //naviData->SatsInUse = 10;
    writex_ndigit_number_u( 17, 5, naviData->SatsInUse, 2, 0,MNORMAL,  -1,2+yoffs);
    draw_icon_satmini( 115, 42+yoffs);



    //-----------------
    // WP: Waypoint-Index und Anzahl der Waypoint's
    //-----------------
    xoffs = 5;
    yoffs = 2;
    lcdx_printp_at( 3, 5, PSTR("WP:"), MNORMAL, xoffs+3,yoffs);

    v = naviData->WaypointIndex;
    if( naviData->WaypointNumber==0 ) v = 0;

    lcdx_printf_at_P( 7, 5, MNORMAL, xoffs,yoffs, PSTR("%2d/%2d"), v, naviData->WaypointNumber );


    //-----------------
    // WP: Countdown Target-Holdtime
    //-----------------
    xoffs = 6;
    yoffs = 2;

    v = naviData->TargetHoldTime;
    if( v > 0 )
    {
        lcdx_printf_at_P( 12, 5, MINVERS, xoffs+2,yoffs, PSTR("%2d"), v );
        lcd_line(        (12*6)+xoffs+2, (5*8)+yoffs-1, (12*6)+xoffs+2+11, (5*8)+yoffs-1, 1);
        lcd_line(        (12*6)+xoffs+1, (5*8)+yoffs-1, (12*6)+xoffs+1, (5*8)+yoffs+7, 1);
    }
    else
    {
        lcd_frect( (12*6)+xoffs+1, (5*8)+yoffs-1,  (2*8)-3, 8, 0);
    }


    //-----------------
    // WP: Distance
    //-----------------
    xoffs = 5;
    yoffs = 4;
    lcdx_printp_at( 3, 6, PSTR("Di:"), MNORMAL, xoffs+3,yoffs);
    writex_ndigit_number_u( 7, 6, naviData->TargetPositionDeviation.Distance / 10, 4, 0,MNORMAL, xoffs,yoffs);
    lcdx_printp_at( 11, 6, PSTR("m"), MNORMAL, xoffs+4,yoffs);       //

    //-----------------
    // WP: Hoehe
    // Anmerkung OG: macht nicht so wirklich Sinn denke ich
    //-----------------
  //writex_ndigit_number_u( 14, 6, naviData->TargetPosition.Altitude / 1000, 4, 0,MNORMAL, xoffs,yoffs);
  //lcdx_printp_at( 18, 6, PSTR("m"), MNORMAL, xoffs+4,yoffs);       //




    //-----------------
    // ggf. BEEP wenn WP erreicht
    //-----------------
    Beep_Waypoint();
}
#endif // USE_OSD_SCREEN_WAYPOINTS



//--------------------------------------------------------------
// OSD-Screen "User GPS"
//
// OSDScreenRefresh:    OSD_SCREEN_REFESH = update values
//                  OSD_SCREEN_REDRAW = redraw labels and update values
//--------------------------------------------------------------
#ifdef USE_OSD_SCREEN_USERGPS
void OSD_Screen_UserGPS( void )
{
    uint8_t y, i;
    int8_t      yoffs;
    uint8_t show_gps_altitude = 0;  // 1=GPS-Höhe anzeigen; 0=barymetrische Höhe anzeigen


    //-----------------------------------------
    // REDRAW
    // statische Screen Elemente die nicht
    // jedesmal neu gezeichnet werden muessen
    //-----------------------------------------
    if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    {
        // do things here for static screen elements like labels and so....
        lcd_line(  (6*6-3), 0, (6*6-3), 9, 1);                      // Linie vertikal oben, links
        lcd_line( (15*6+3), 0, (15*6+3), 9, 1);                     // Linie vertikal oben, rechts
        lcd_rect_round( 0, 9, 127, 63-9, 1, R2);                    // Rahmen
        lcdx_printp_at( 6, 0, PSTR("UGPS"), MNORMAL, 2,0);
    }


    //-----------------
    // Oben: Batt Level (Volt)
    //-----------------
    OSD_Element_BattLevel2( 0, 0, 0,0 );

    //-----------------
    // Oben: Sat Ok
    //-----------------
    OSD_Element_Flag(  12, 0, OSD_FLAG_S0, 5,0);    // GPS-Sat not ok (GPS NOT ok)

    //-----------------
    // Oben: Flugzeit
    //-----------------
    writex_time( 16, 0, naviData->FlyingTime, MNORMAL, 2,0);

    //-----------------
    // die letzen 3 User-GPS Daten anzeigen
    //-----------------
    yoffs = -4;
    for(i=0; i<3; i++)
    {
        y = (i*2)+2;
        lcd_frect( 0, (y*8)+yoffs-1, 127, 7, 1);                                // inverser Hintergrund

        writex_ndigit_number_u( 0, y+0, (i+1), 1, 0 , MINVERS, 4,yoffs);        // Index
        writex_datetime_time(   4, y+0, Config.GPS_User[i].timestamp, MINVERS, 0,yoffs);

        //writex_time(    3, y+0, GPS_User[i].time_pkt, MINVERS, 0,yoffs);
        //lcdx_printp_at( 12, y+0, PSTR("Alt:")     , MINVERS, 0,yoffs);
        lcdx_printp_at( 20, y+0, PSTR("m")          , MINVERS, 0,yoffs);

        if( show_gps_altitude )                                                 // GPS-Hoehe oder barymetrische Hoehe?
        {
            // GPS Hoehe
            //lcdx_printp_at( 10, y+0, PSTR("G"), MINVERS, 2,yoffs);
            //lcd_frect( (12*6)-4, (y*8)+3+yoffs, 1, 1, 0);
            writex_ndigit_number_s( 16, y+0, Config.GPS_User[i].GPSData.Altitude/1000, 4, 0, MINVERS, 0,yoffs); // GPS Hoehe in Meter
        }
        else
        {
            // barymetrische Hoehe
            //lcdx_printp_at( 10, y+0, PSTR("B"), MINVERS, 2,yoffs);
            //lcd_frect( (12*6)-4, (y*8)+3+yoffs, 1, 1, 0);
            writex_altimeter( 16, y+0, Config.GPS_User[i].Altimeter, MINVERS, 0,yoffs );
        }

        lcd_line( 1, (y*8)+yoffs+7, 125, (y*8)+yoffs+7, 0);                     // clear: Invers unterste Linie

        writex_gpspos(  1, y+1, Config.GPS_User[i].GPSData.Latitude , MNORMAL, 0,yoffs);        // GPS Lat
        writex_gpspos( 12, y+1, Config.GPS_User[i].GPSData.Longitude, MNORMAL, 0,yoffs);        // GPS Long
        //writex_gpspos(  1, y+1, GPS_User[i].GPSData.Latitude + 12867000, MNORMAL, 0,yoffs);   // DUMMY DATEN! DEMO!
        //writex_gpspos( 12, y+1, GPS_User[i].GPSData.Longitude+ 8568000, MNORMAL, 0,yoffs);    // DUMMY DATEN! DEMO!

        yoffs++;
    }
}
#endif // USE_OSD_SCREEN_USERGPS



//--------------------------------------------------------------
// OSD-Screen "Status"
//
// OSDScreenRefresh:    0 = update values
//              1 = redraw labels and update values
//--------------------------------------------------------------
#ifdef USE_OSD_SCREEN_MKSTATUS
void OSD_Screen_MKStatus( void )
{
    int8_t      xoffs, yoffs;

    //-----------------------------------------
    // REDRAW
    // statische Screen Elemente die nicht
    // jedesmal neu gezeichnet werden muessen
    //-----------------------------------------
    if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    {
        // do things here for static screen elements like labels and so....

        lcd_line(  (6*6-3), 0, (6*6-3), 9, 1);                      // Linie vertikal oben, links
        lcd_line( (15*6+3), 0, (15*6+3), 9, 1);                     // Linie vertikal oben, rechts
        lcd_rect_round( 0, 10, 127, 63-10, 1, R2);                  // Rahmen

        lcdx_printp_at( 12,0 , PSTR("mAh"), MNORMAL, -1,0);         // "mAh" (entnommene Kapazität)
    }


    //-----------------
    // Oben: Batt Level (Volt)
    //-----------------
    OSD_Element_BattLevel2( 0, 0, 0,0 );

    //-----------------
    // Strom
    //-----------------
    //writex_ndigit_number_u_10th( 7, 0, naviData->Current, 4, 0,MNORMAL, 1,0);  // Strom mit Nachkomma

    //-----------------
    // entnommene Kapazitaet (mAh)
    //-----------------
    drawmode = (naviData->UsedCapacity > Config.OSD_mAh_Warning ? MINVERS : MNORMAL);
    //writex_ndigit_number_u( 7, 0, naviData->UsedCapacity * 10, 5, 0, drawmode, -3,0);  // DEBUG
    writex_ndigit_number_u( 7, 0, naviData->UsedCapacity, 5, 0, drawmode, -3,0);

    //-----------------
    // Oben: Flugzeit
    //-----------------
    writex_time( 16, 0, naviData->FlyingTime, MNORMAL, 2,0);

    //-----------------
    // Flags
    //-----------------
    yoffs = -2;
    xoffs = -7;
    OSD_Element_Flag(  19, 2, OSD_FLAG_CF,  0+xoffs, 0+yoffs);  // Carefree
    OSD_Element_Flag(  19, 4, OSD_FLAG_AH,  0+xoffs,-3+yoffs);  // Altitude Hold
    OSD_Element_Flag(  19, 6, OSD_FLAG_PH,  0+xoffs,-6+yoffs);  // Position Hold
    OSD_Element_Flag(  19, 7, OSD_FLAG_CH,  0+xoffs,-1+yoffs);  // Coming Home

    xoffs -= 4;
    OSD_Element_Flag(  16, 2, OSD_FLAG_BA, -3+xoffs, 0+yoffs);  // MK Batt Warning
    OSD_Element_Flag(  16, 4, OSD_FLAG_EL, -3+xoffs,-3+yoffs);  // Emergency Landing
    OSD_Element_Flag(  16, 6, OSD_FLAG_RL, -3+xoffs,-6+yoffs);  // Range Limit
    OSD_Element_Flag(  16, 7, OSD_FLAG_S0, -3+xoffs,-1+yoffs);  // GPS-Sat not ok (GPS NOT ok)

    xoffs -= 4;
    OSD_Element_Flag(  12, 2, OSD_FLAG_CA,  0+xoffs, 0+yoffs);  // Calibrate
    OSD_Element_Flag(  12, 4, OSD_FLAG_ST,  0+xoffs,-3+yoffs);  // Start
    OSD_Element_Flag(  12, 6, OSD_FLAG_MR,  0+xoffs,-6+yoffs);  // Motor Run
    OSD_Element_Flag(  12, 7, OSD_FLAG_FY,  0+xoffs,-1+yoffs);  // Fly

    xoffs -= 4;
    OSD_Element_Flag(   9, 2, OSD_FLAG_O1, -2+xoffs, 0+yoffs);  // Out1
    OSD_Element_Flag(   9, 4, OSD_FLAG_O2, -2+xoffs,-3+yoffs);  // Out2
    OSD_Element_Flag(   9, 6, OSD_FLAG_TR, -2+xoffs,-6+yoffs);  // Target Reached
    OSD_Element_Flag(   9, 7, OSD_FLAG_MC, -2+xoffs,-1+yoffs);  // Manual Control

    xoffs -= 4;
    OSD_Element_Flag(   6, 2, OSD_FLAG_TU, -4+xoffs, 0+yoffs);  // Vario Trim Up
    OSD_Element_Flag(   6, 4, OSD_FLAG_TD, -4+xoffs,-3+yoffs);  // Vario Trim Down
    OSD_Element_Flag(   6, 6, OSD_FLAG_FR, -4+xoffs,-6+yoffs);  // Free
    OSD_Element_Flag(   6, 7, OSD_FLAG_SL, -4+xoffs,-1+yoffs);  // No Serial Link


    #ifdef USE_OSD_DEMO
        //-----------------
        // Flags
        //-----------------
 /*
        PSTR("AH"),  // OSD_FLAG_AH  Altitue Hold
        PSTR("PH"),  // OSD_FLAG_PH  Position Hold
        PSTR("CF"),  // OSD_FLAG_CF  Care Free
        PSTR("CH"),  // OSD_FLAG_CH  Coming Home
        PSTR("o1"),  // OSD_FLAG_O1  Out1
        PSTR("o2"),  // OSD_FLAG_O2  Out2
        PSTR("BA"),  // OSD_FLAG_BA  LowBat warning (MK)
        PSTR("CA"),  // OSD_FLAG_CA  Calibrate
        PSTR("ST"),  // OSD_FLAG_ST  Start
        PSTR("MR"),  // OSD_FLAG_MR  Motor Run
        PSTR("FY"),  // OSD_FLAG_FY  Fly
        PSTR("EL"),  // OSD_FLAG_EL  Emergency Landing
        PSTR("FS"),  // OSD_FLAG_FS  RX Failsave Active
        PSTR("GP"),  // OSD_FLAG_GP  GPS Ok
        PSTR("S!")   // OSD_FLAG_S0  GPS-Sat not ok (GPS NOT ok)
        PSTR("TU"),  // OSD_FLAG_TU  Vario Trim Up
        PSTR("TD"),  // OSD_FLAG_TD  Vario Trim Down
        PSTR("FR"),  // OSD_FLAG_FR  Free
        PSTR("RL"),  // OSD_FLAG_RL  Range Limit
        PSTR("SL"),  // OSD_FLAG_SL  No Serial Link
        PSTR("TR"),  // OSD_FLAG_TR  Target Reached
        PSTR("MC")   // OSD_FLAG_MC  Manual Control
*/
        yoffs = -2;
        xoffs = -7;
        OSD_Element_Flag_Label(  19, 2, OSD_FLAG_CF, true,  0+xoffs, 0+yoffs);  // DEMO: Carefree
        OSD_Element_Flag_Label(  19, 4, OSD_FLAG_AH, true,  0+xoffs,-3+yoffs);  // DEMO: Altitude Hold
        OSD_Element_Flag_Label(  19, 6, OSD_FLAG_PH, true,  0+xoffs,-6+yoffs);  // DEMO: Position Hold
        OSD_Element_Flag_Label(  19, 7, OSD_FLAG_CH, true,  0+xoffs,-1+yoffs);  // DEMO: Coming Home

        xoffs -= 4;
        OSD_Element_Flag_Label(  16, 2, OSD_FLAG_BA, true, -3+xoffs, 0+yoffs);  // DEMO: MK Batt Warning
        OSD_Element_Flag_Label(  16, 4, OSD_FLAG_EL, true, -3+xoffs,-3+yoffs);  // DEMO: Emergency Landing
        OSD_Element_Flag_Label(  16, 6, OSD_FLAG_RL, true, -3+xoffs,-6+yoffs);  // DEMO: Range Limit
        OSD_Element_Flag_Label(  16, 7, OSD_FLAG_S0, true, -3+xoffs,-1+yoffs);  // DEMO: GPS-Sat not ok (GPS NOT ok)

        xoffs -= 4;
        OSD_Element_Flag_Label(  12, 2, OSD_FLAG_CA, true,  0+xoffs, 0+yoffs);  // DEMO: Calibrate
        OSD_Element_Flag_Label(  12, 4, OSD_FLAG_ST, true,  0+xoffs,-3+yoffs);  // DEMO: Start
        OSD_Element_Flag_Label(  12, 6, OSD_FLAG_MR, true,  0+xoffs,-6+yoffs);  // DEMO: Motor Run
        OSD_Element_Flag_Label(  12, 7, OSD_FLAG_FY, true,  0+xoffs,-1+yoffs);  // DEMO: Fly

        xoffs -= 4;
        OSD_Element_Flag_Label(   9, 2, OSD_FLAG_O1, true, -2+xoffs, 0+yoffs);  // DEMO: Out1
        OSD_Element_Flag_Label(   9, 4, OSD_FLAG_O2, true, -2+xoffs,-3+yoffs);  // DEMO: Out2
        OSD_Element_Flag_Label(   9, 6, OSD_FLAG_TR, true, -2+xoffs,-6+yoffs);  // DEMO: Target Reached
        OSD_Element_Flag_Label(   9, 7, OSD_FLAG_MC, true, -2+xoffs,-1+yoffs);  // DEMO: Manual Control

        xoffs -= 4;
        OSD_Element_Flag_Label(   6, 2, OSD_FLAG_TU, true, -4+xoffs, 0+yoffs);  // DEMO: Vario Trim Up
        OSD_Element_Flag_Label(   6, 4, OSD_FLAG_TD, true, -4+xoffs,-3+yoffs);  // DEMO: Vario Trim Down
        OSD_Element_Flag_Label(   6, 6, OSD_FLAG_FR, true, -4+xoffs,-6+yoffs);  // DEMO: Free
        OSD_Element_Flag_Label(   6, 7, OSD_FLAG_SL, true, -4+xoffs,-1+yoffs);  // DEMO: No Serial Link

    #endif
}
#endif // USE_OSD_SCREEN_MKSTATUS



//--------------------------------------------------------------
// OSD_Screen_Electric()
//
// Anzeige BL-Temperaturen und Stroeme
//--------------------------------------------------------------
#ifdef USE_OSD_SCREEN_ELECTRIC
void OSD_Screen_Electric( void )
{
    uint8_t x, y, x0, y0;
    int8_t  yoffs;

    //-----------------------------------------
    // REDRAW
    // statische Screen Elemente die nicht
    // jedesmal neu gezeichnet werden muessen
    //-----------------------------------------
    if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    {
        // do things here for static screen elements like labels and so....
        lcd_line (33, 0, 33, 19, 1);                                // Linie vertikal oben, links
        lcd_line (93, 0, 93, 19, 1);                                // Linie vertikal oben, rechts

        lcd_rect_round( 0, 19, 127, 63-19, 1, R2);                  // Rahmen
        lcd_line (0, 41, 127, 41, 1);                               // Linie horizontal mitte

        lcdx_printp_at( 12,0 , PSTR("mAh"), MNORMAL, -1,0);         // entnommene Kapazität
        lcdx_printp_at( 12,1 , PSTR("A")  , MNORMAL, -1,2);         // aktueller Strom

        writex_ndigit_number_u( 19, 1, cells, 1, 0, MNORMAL, 2,2);  // LiPO Cells Wert
        lcdx_printp_at( 20,1 , PSTR("s")          , MNORMAL, 2,2);  // LiPO Cells "s"

    } // end: if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    //-----------------------------------------


    //-----------------
    // Oben: Batt Level (Volt)
    //-----------------
    OSD_Element_BattLevel2( 0, 0, 0,0 );

    //-----------------
    // Oben: Batt Level Bar
    //-----------------
    //OSD_Element_Battery_Bar( x, y, length, width, orientation)
    OSD_Element_Battery_Bar( 0, 9, 30, 1, ORIENTATION_H);

    //-----------------
    // Oben: entnommene Kapazitaet (mAh)
    //-----------------
    drawmode = (naviData->UsedCapacity > Config.OSD_mAh_Warning ? MINVERS : MNORMAL);
    writex_ndigit_number_u( 7, 0, naviData->UsedCapacity, 5, 0, drawmode, -3,0);

    //-----------------
    // Oben: Flugzeit
    //-----------------
    //writex_time(16, 0, naviData->FlyingTime+900, MNORMAL, 2,0);  // DEBUG
    writex_time( 16, 0, naviData->FlyingTime, MNORMAL, 2,0);

    //-----------------
    // Strom
    //-----------------
    writex_ndigit_number_u_10th( 7, 1, naviData->Current, 4, 0,MNORMAL, -3,2);  // Strom mit Nachkomma

    //-----------------
    // BL 1-8 Temp & Strom
    //-----------------
    x0 = 1;
    y0 = 4;
    for( y=0; y<2; y++)                                 // 2 Zeilen (mit je 4 BL's/Motoren)
    {
        if( y==0 )  yoffs = -9;
        else        yoffs = -3;

        for( x=0; x<4; x++)                             // und 4 BL's/Motoren pro Zeile
        {
            if( blData[y*4+x].Status & 0xf0 )           // BL/Motor vorhanden?
            {
                if( blData[y*4+x].Temperature != 0 )    // Anzeige nur wenn Temp != 0 wegen BL-Ctrl v1 die keine Temperatur senden
                {
                    writex_ndigit_number_u( (x*5)+x0,   (y*2)+y0+0, ( blData[y*4+x].Temperature ), 3, 0,MNORMAL, 0,yoffs);  // Temperatur
                    lcdx_putc             ( (x*5)+3+x0, (y*2)+y0+0, SYMBOL_SMALLDEGREE                 ,MNORMAL, 1,yoffs);
                }

                // Variante: mit Nachkommastellen
                writex_ndigit_number_u_10th (   (x*5)+x0, (y*2)+y0+1, ( blData[y*4+x].Current), 3, 0, MNORMAL, 0,yoffs);        // Strom
            }
        }
    }

    #ifdef USE_OSD_DEMO
        OSD_Element_Flag_Label(  19, 1, OSD_FLAG_BA, true, 0,1);    // DEMO
    #endif
}
#endif  // USE_OSD_SCREEN_ELECTRIC_N




//--------------------------------------------------------------
// OSD_Screen_Statistics()
//--------------------------------------------------------------
#ifdef USE_OSD_SCREEN_STATISTIC
void OSD_Screen_Statistics( void )
{
    osd_BLmax_t blmax;
    uint8_t line = 0;


    // max. der BL's ermitteln
    calc_BLmax( &blmax );


    //---------------------------
    // max Altitude
    lcd_printp_at (0, line, strGet(STATS_ITEM_0), MNORMAL);
    write_ndigit_number_s (14, line, Config.OSD_Statistic.max_Altimeter / (30 / AltimeterAdjust), 4, 0,MNORMAL);
    lcdx_putc (18, line, 'm', MNORMAL, 2,0);

    //---------------------------
    // max Speed
    // max_GroundSpeed = 1;
    lcd_printp_at (0, ++line, strGet(STATS_ITEM_1), MNORMAL);
    write_ndigit_number_u (15, line, (uint16_t) (((uint32_t) Config.OSD_Statistic.max_GroundSpeed * (uint32_t) 9) / (uint32_t) 250), 3, 0,MNORMAL);
    lcdx_printp_at(18, line, PSTR("kmh"), MNORMAL, 2,0);

    //---------------------------
    // max Distance
    // max_Distance = 64512;
    lcd_printp_at (0, ++line, strGet(STATS_ITEM_2), MNORMAL);
    write_ndigit_number_u (14, line, Config.OSD_Statistic.max_Distance / 10, 4, 0,MNORMAL);
    lcdx_putc (18, line, 'm', MNORMAL, 2,0);

    //---------------------------
    // min voltage
    lcd_printp_at (0, ++line, strGet(STATS_ITEM_3), MNORMAL);
    if( Config.OSD_Statistic.min_UBat==255 )
        lcd_printp_at(14, line, PSTR("   0"), MNORMAL);
    else
        write_ndigit_number_u_10th (14, line, Config.OSD_Statistic.min_UBat, 3, 0,MNORMAL);
    lcdx_putc (18, line, 'V', MNORMAL, 2,0);

    //---------------------------
    // Used Capacity
    lcd_printp_at (0, ++line, strGet(STATS_ITEM_6), MNORMAL);
    write_ndigit_number_u (14, line, Config.OSD_Statistic.max_Capacity, 4, 0,MNORMAL);
    lcdx_printp_at(18, line, PSTR("mAh"), MNORMAL, 2,0);


    //---------------------------
    // max Current
    // max_Current = 1000;
    lcd_printp_at (0, ++line, strGet(STATS_ITEM_5), MNORMAL);
    write_ndigit_number_u_10th (13, line, Config.OSD_Statistic.max_Current, 4, 0,MNORMAL);
    lcdx_putc (18, line, 'A', MNORMAL, 2,0);


    //---------------------------
    // max BL-Current
    line++;
    lcd_printp_at( 0, line, PSTR("max BL  Curr:"), MNORMAL);
    write_ndigit_number_u( 6, line, blmax.max_BL_Current_Index+1, 1, 0,MNORMAL);
    write_ndigit_number_u_10th (14, line, blmax.max_BL_Current, 3, 0,MNORMAL);
    lcdx_putc (18, line, 'A', MNORMAL, 2,0);

    //---------------------------
    // max BL-Temp
    line++;
    lcd_printp_at( 0, line, PSTR("max BL  Temp:"), MNORMAL);
    write_ndigit_number_u( 6, line, blmax.max_BL_Temp_Index+1, 1, 0,MNORMAL);
    write_ndigit_number_u (14, line, blmax.max_BL_Temp, 4, 0,MNORMAL);
    lcdx_printp_at( 18, line, PSTR("\013C"), MNORMAL, 2,0);
}
#endif // USE_OSD_SCREEN_STATISTIC



//--------------------------------------------------------------
// OSD_Screen_3DLage()
//--------------------------------------------------------------
#ifdef USE_OSD_SCREEN_3DLAGE
void OSD_Screen_3DLage( void )
{
    uint16_t head_home;
    uint8_t  Nick;
    uint8_t  Roll;

    //-----------------------------------------
    // REDRAW
    // statische Screen Elemente die nicht
    // jedesmal neu gezeichnet werden muessen
    //-----------------------------------------
    if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    {
        // do things here for static screen elements like labels and so....
        lcd_rect_round( 0, 0, 127, 63-0, 1, R2);                    // Rahmen
    }


    head_home = (naviData->HomePositionDeviation.Bearing + 360 - naviData->CompassHeading) % 360;

    lcd_line(26,32,100,32,1);  // horizontal //
    lcd_line(63,0,63,63,1);    //  vertical  //

    // 45' Angel
    lcd_line(61,11,65,11,1);   //    --    //
    lcd_line(40,30,40,34,1);   //  |       //
    lcd_line(86,30,86,34,1);   //       |  //
    lcd_line(61,53,65,53,1);   //    --    //

    lcdx_printp_at( 9, 0, strGet(OSD_3D_V), MNORMAL, 0,4);      // V
    lcdx_printp_at( 3, 3, strGet(OSD_3D_L), MNORMAL, 0,0);      // L
    lcdx_printp_at(17, 3, strGet(OSD_3D_R), MNORMAL, 0,0);      // R
    lcdx_printp_at( 9, 7, strGet(OSD_3D_H), MNORMAL, 0,-3);     // H

    // Oben, Links: Ni
    lcdx_printp_at(0, 0, strGet(OSD_3D_NICK), MNORMAL, 5,4);                    // Ni
    writex_ndigit_number_s (2, 0, naviData->AngleNick, 3, 0, MNORMAL, 7,4);
    lcdx_putc (5, 0, SYMBOL_SMALLDEGREE, MNORMAL, 7,4);

    // Unten, Links: Ro
    lcdx_printp_at(0, 7, strGet(OSD_3D_ROLL), MNORMAL, 5,-3);                   // Ro
    writex_ndigit_number_s (2, 7, naviData->AngleRoll, 3, 0, MNORMAL, 7,-3);
    lcdx_putc (5, 7, SYMBOL_SMALLDEGREE, MNORMAL, 7,-3);

    // Oben, Rechts: Ko
    //lcdx_printp_at(13, 0, strGet(OSD_3D_COMPASS), MNORMAL, -4,3);             // Ko
    writex_ndigit_number_u (15, 0, (naviData->CompassHeading)%360, 3, 0, MNORMAL, -4,4);
    lcdx_putc (18, 0, SYMBOL_SMALLDEGREE, MNORMAL, -4,4);
    OSD_Element_CompassDirection( 19, 0,  -2,4 );

    Roll = ((-naviData->AngleRoll/2)+63);
    Nick = ((-naviData->AngleNick/2)+32);

    if( Roll <     (9+1) ) Roll =     (9+1);    // nicht ausserhalb des Screens zeichnen!
    if( Roll > 127-(9+1) ) Roll = 127-(9+1);    // nicht ausserhalb des Screens zeichnen!

    if( Nick <     (8+1) ) Nick =     (8+1);    // nicht ausserhalb des Screens zeichnen!
    if( Nick >  63-(8+1) ) Nick =  63-(8+1);    // nicht ausserhalb des Screens zeichnen!

    if( old_AngleRoll != 0 )                    // nicht ausserhalb des Screens zeichnen!
    {
        lcd_ellipse     ( old_AngleRoll, old_AngleNick, 9, 8, 0);
        lcd_ellipse_line( old_AngleRoll, old_AngleNick, 8, 7, old_hh, 0);
    }

    lcd_ellipse     ( Roll, Nick, 9, 8, 1);
    lcd_ellipse_line( Roll, Nick, 8, 7, head_home, 1);

    // remember last values (3DL)
    old_hh          = head_home;
    old_AngleNick   = Nick;
    old_AngleRoll   = Roll;
}
#endif // USE_OSD_SCREEN_3DLAGE




//--------------------------------------------------------------
// OSD_Screen_MKDisplay()
//
// das ist ein Spezialscreen der ausserhalb der regulaeren
// OSD-Screens aufgerufen und bedient wird!
//--------------------------------------------------------------
//#ifdef USE_OSD_SCREEN_MKDISPLAY
void OSD_Screen_MKDisplay( void )
{
    uint8_t wpindex;

    //-----------------------------------------
    // REDRAW
    // statische Screen Elemente die nicht
    // jedesmal neu gezeichnet werden muessen
    //-----------------------------------------
    if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    {
        // do things here for static screen elements like labels and so....
        lcd_line(  (6*6-3),  0,  (6*6-3), 11, 1);                               // Linie Vertikal Oben: links
        lcd_line( (15*6+5),  0, (15*6+5), 11, 1);                               // Linie Vertikal Oben: rechts
        lcd_rect_round( 0, 2*7-2, 127, 5*7+3+3, 1, R2);                         // Rahmen unten fuer Inhalt Display

        //lcd_frect_round( 6*6+0, 0, 9*6+2, 9, 1, R1);                          // Umrahmung fuer "OSD-Displ"
        //lcdx_printp_at( 7, 0, PSTR("OSD-Disp"), MINVERS,  -3,1);              // "OSD-Displ"
        //lcdx_printp_at(15, 0, PSTR("l"), MINVERS,  -4,1);                     // das "l" von "OSD-Displ" (1 Pixel nach links)

        lcdx_printp_at( 2, 7, PSTR("\x18    \x19"), MNORMAL,  0,0);             // Keyline: Links / Rechts
        PKT_KeylineUpDown( 18, 13,  0,0);                                       // Keyline: Down / Up

    } // end: if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    //-----------------------------------------


    //-----------------
    // Oben: Waypoint-Index und Anzahl der Waypoint's
    //-----------------
    wpindex = naviData->WaypointIndex;
    if( naviData->WaypointNumber==0 ) wpindex = 0;

    lcdx_printf_at_P( 7, 0, MNORMAL, -1,0, PSTR("WP:%2d/%2d"), wpindex, naviData->WaypointNumber );


    //lcdx_printf_at_P(13, 0, MNORMAL, -3,1, PSTR("R:%d"), (naviData->NCFlags & NC_FLAG_TARGET_REACHED) );
    //if ((NextWP==true)&& naviData->NCFlags & NC_FLAG_TARGET_REACHED)
    //lcd_printf_at_P( 0, 2, MNORMAL, PSTR("WP:%d"), naviData->WaypointIndex);
    //write_ndigit_number_u (x+2, y, naviData->WaypointIndex , 2, 0,0);
    //lcd_printp_at (x, y, PSTR("WP"), 0);
    //if ((NextWP==true)&& naviData->NCFlags & NC_FLAG_TARGET_REACHED)



    //-----------------
    // Oben: Batt Level (Volt)
    //-----------------
    OSD_Element_BattLevel2( 0, 0, 0,0 );

    //-----------------
    // Oben: Batt Level Bar
    //-----------------
    //OSD_Element_Battery_Bar( x, y, length, width, orientation)
    OSD_Element_Battery_Bar( 0, 9, 30, 1, ORIENTATION_H);

    //-----------------
    // Oben: Navi-Kreis
    //-----------------
    //OSD_Element_HomeCircleX( 64, 5, 6, 5, true );


    //-----------------
    // Oben: Flugzeit
    //-----------------
    writex_time(16, 0, naviData->FlyingTime, MNORMAL, 2,0);


    //------------------------------------------
    // Ausgabe auf PKT-Anzeige
    // 4 Zeilen a 20 Zeichen
    //------------------------------------------
    mkdisplayData[80] = 0;
    lcdx_print_at( 0,5, (uint8_t *) &mkdisplayData[60],  MNORMAL,  5,3);

    mkdisplayData[60] = 0;
    lcdx_print_at( 0,4, (uint8_t *) &mkdisplayData[40],  MNORMAL,  5,2);

    mkdisplayData[40] = 0;
    lcdx_print_at( 0,3, (uint8_t *) &mkdisplayData[20],  MNORMAL,  5,1);

    mkdisplayData[20] = 0;
    lcdx_print_at( 0,2, (uint8_t *) &mkdisplayData[0],   MNORMAL,  5,0);

    Beep_Waypoint();
}
//#endif  // USE_OSD_SCREEN_MKDISPLAY



//##############################################################
#ifdef USE_OSD_SCREEN_DEBUG
//##############################################################

//**************************************************************
//* OSD_DEBUG_SCREEN - Experimental-Code usw.
//* - nicht fuer die Oeffentlichkeit bestimmt
//* - gesteuert ueber define OSD_DEBUG_SCREEN
//**************************************************************

//--------------------------------------------------------------
// OSD_Screen_Debug()
//--------------------------------------------------------------
void OSD_Screen_Debug( void )
{
    //char buffer[80];
    static uint16_t debug_count = 0;
    //char buffer[30];
    //uint8_t   y, i;


    //-----------------------------------------
    // REDRAW
    // statische Screen Elemente die nicht
    // jedesmal neu gezeichnet werden muessen
    //-----------------------------------------
    if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    {
        // do things here for static screen elements like labels and so....
        lcd_printp_at( 0, 0, PSTR("Debug"), 0);
        //timer_pkt_uptime = 0;
    }

    debug_count++;
    write_ndigit_number_u( 17, 0, (debug_count), 4, 0,MNORMAL);   //


    // act. Current
    lcd_printf_at_P( 0, 2, MNORMAL, PSTR("act  Current%3.1u A"), naviData->Current);

    // max. Current
    lcd_printf_at_P( 0, 3, MNORMAL, PSTR("max  Current%3.1u A"), Config.OSD_Statistic.max_Current);

    // avg. Current
    lcd_printf_at_P( 0, 4, MNORMAL, PSTR("avg  Current%3.1u A"), (uint8_t)(Config.OSD_Statistic.avg_Current/100));

    // avg. Current DEBUG
    //lcd_printf_at_P( 0, 5, MNORMAL, PSTR("avgD Current%3.1u A"), (uint8_t)(Config.OSD_Statistic.avg_Altimeter/100));

    // stat. Packages
    lcd_printf_at_P( 0, 6, MNORMAL, PSTR("stat Pkg's%7u"), Config.OSD_Statistic.count_osd);


/*
    // DEBUG: Heading
    lcd_printf_at_P( 0, 1, MNORMAL, PSTR("NC-Errorcode:%3u"), naviData->Errorcode);

    //heading_home = (naviData->HomePositionDeviation.Bearing + 360 - naviData->CompassHeading) % 360;

    lcd_printf_at_P( 0, 3, MNORMAL, PSTR("CH:%6d%6d"), naviData->CompassHeading, naviData->CompassHeading%360);
    lcd_printf_at_P( 0, 4, MNORMAL, PSTR("HD:%6d%6d"), naviData->HomePositionDeviation.Bearing, naviData->HomePositionDeviation.Bearing%360);

    //heading_home = (naviData->HomePositionDeviation.Bearing + 360 - naviData->CompassHeading) % 360;

    //heading_home = ((naviData->CompassHeading % 360) - (naviData->HomePositionDeviation.Bearing % 360)) + 180;
    //heading_home = ((naviData->CompassHeading % 360) - (naviData->HomePositionDeviation.Bearing % 360));

    calc_heading_home();

    lcd_printf_at_P( 0, 6, MNORMAL, PSTR("xx:%6d"), heading_home);
*/

/*
    lcd_putc( 0, 3, 0x08, MNORMAL); // ASCII -   8 08 SAT Symbol
    lcd_putc( 2, 3, 0x09, MNORMAL); // ASCII -   9 09 Altitude Symbol
    lcd_putc( 4, 3, 0x0C, MNORMAL); // ASCII -  12 0C Enter Symbol
    lcd_putc( 6, 3, 0x1F, MNORMAL); // ASCII -  31 1F Antenne
    lcd_putc( 8, 3, 10, MNORMAL); // 'o'
    lcd_putc(10, 3, 13, MNORMAL); // 'o'
    //lcd_putc(10, 3, 0x06, MNORMAL);
    //lcd_putc(12, 3, 0x07, MNORMAL);

    lcd_putc( 0, 5, 0x1E, MNORMAL);
    lcd_putc( 4, 5, 0x7e, MNORMAL);
    lcd_putc( 6, 5, 0x7f, MNORMAL);
    lcd_putc( 8, 5, 0x18, MNORMAL);
    lcd_putc(10, 5, 0x19, MNORMAL);
*/

    //lcd_printp_at( 0, 1, PSTR("Free RAM:"), 0);
    //writex_ndigit_number_u ( 9,  1, get_freeRam(), 9, 0,MNORMAL, 0,0);   //
}


//--------------------------------------------------------------
// OSD_Screen_Debug_RX()
//
// Anzeige gelesener Datenpakete (fuer Feinabstimmung) und weitere
// Werte wie Zeit/Datum (mit/ohne Abgleich zur NC)
//
// Anzeige oben 1. Zeile:
//   Screenname  : "Debug-RX"
//   PKT-Uptime  : Minuten, Sekunden die das PKT aktuell eingeschaltet ist
//   Screen-Count: Anzahl der Aufrufe des Screens (abhaengig von der Refreshtime)
//
// Anzeige Zeit:
//   "N" oder "O": neuer(N) oder alter(O) Zeit-Algo fuer die NC (OSD_MK_UTCTime())
//   00:00:00    : Stunde, Minute, Sekunde (korrigiert mittels PKT-Einstellung bzgl. Zeitzone/Sommerzeit)
//   dd.mm.yyyy  : Tag, Monat, Jahr        (korrigiert mittels PKT-Einstellung bzgl. Zeitzone/Sommerzeit)
// Solange keine richtige Zeit von der NC gemeldet wird, wird die PKT-Uptime
// seit einschalten des PKT angezeigt.
//
// Anzeige unten:
//   OSD:  Anzahl gelesener OSD-Pakete der NC
//   Time: Anzahl gelesener Time-Pakete der NC (Aktualisierung ca. jede Minute)
//
//   BL:   es werden die gelesenen Datenpakete der BL's angezeigt
//         von BL1 (links oben) bis BL8 (rechts unten)
//--------------------------------------------------------------
void OSD_Screen_Debug_RX( void )
{
    static uint16_t debug_count = 0;
    uint8_t y;
    int8_t  yoffs;

    //uint8_t status;       // FC Kommunikation

    //-----------------------------------------
    // REDRAW
    // statische Screen Elemente die nicht
    // jedesmal neu gezeichnet werden muessen
    //-----------------------------------------
    if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    {
        // do things here for static screen elements like labels and so....
        lcd_frect(     0, 0, 1, 8, 1);                              // title spacer
        lcdx_printp_at( 0, 0, PSTR("Debug-RX"), MINVERS, 1,0);      // title
        lcd_line (0, 9, 127, 9, 1);                                 // Linie horizontal
        lcd_line (0, 22, 127, 22, 1);                               // Linie horizontal
        //lcd_line (0, 31, 127, 31, 1);                               // Linie horizontal
    }

    //-----------------------
    // Zeile 0
    //-----------------------
    writex_time( 11, 0, timer_pkt_uptime/100, MNORMAL, 0,0);
    debug_count++;
    write_ndigit_number_u ( 17, 0, (debug_count), 4, 0,MNORMAL);   //


    //-----------------------
    // Anzeige Zeit / Datum
    //-----------------------
    yoffs = 5;
    writex_datetime_time(  1, 1, UTCTime, MNORMAL, 5,yoffs);    // Zeit
    writex_datetime_date( 11, 1, UTCTime, MNORMAL, 0,yoffs);    // Datum

    //-----------------------
    // gelesene Pakete: OSD und Zeit
    //-----------------------
    y = 4;
    yoffs = -5;

    lcdx_printp_at( 0, y+0, PSTR("OSD:"), MNORMAL, 0,yoffs);
    writex_ndigit_number_u ( 4,  y+0, ( readCounterOSD), 5, 0,MNORMAL, 0,yoffs);   //

    lcdx_printp_at( 13, y+0, PSTR("Time:"), MNORMAL, 0,yoffs);
    writex_ndigit_number_u ( 18,  y+0, ( readCounterTIME), 3, 0,MNORMAL, 0,yoffs);   //


    //----
    lcdx_printp_at( 0, y+1, PSTR("Dis:"), MNORMAL, 0,yoffs);
    writex_ndigit_number_u ( 4,  y+1, ( readCounterDISPLAY), 5, 0,MNORMAL, 0,yoffs);   //


    //-----------------------
    // gelesene Pakete: BL
    //-----------------------
    y = 6;
    lcd_frect(     0, y*8-1, 8, 17, 1); // Box

    lcdx_printp_at( 0, y+0, PSTR("B"), MINVERS, 2,0);
    lcdx_printp_at( 0, y+1, PSTR("L"), MINVERS, 2,0);
    write_ndigit_number_u ( 2,  y+0, ( readCounterBL[0]), 4, 0,MNORMAL);   //
    write_ndigit_number_u ( 7,  y+0, ( readCounterBL[1]), 4, 0,MNORMAL);   //
    write_ndigit_number_u ( 12, y+0, ( readCounterBL[2]), 4, 0,MNORMAL);   //
    write_ndigit_number_u ( 17, y+0, ( readCounterBL[3]), 4, 0,MNORMAL);   //

    write_ndigit_number_u ( 2,  y+1, ( readCounterBL[4]), 4, 0,MNORMAL);   //
    write_ndigit_number_u ( 7,  y+1, ( readCounterBL[5]), 4, 0,MNORMAL);   //
    write_ndigit_number_u ( 12, y+1, ( readCounterBL[6]), 4, 0,MNORMAL);   //
    write_ndigit_number_u ( 17, y+1, ( readCounterBL[7]), 4, 0,MNORMAL);   //
}

//##############################################################
#endif  // USE_OSD_SCREEN_DEBUG
//##############################################################



//-----------------------------------------------------------
// ok = OSD_Popup_MKSetting()
//
// zeigt das aktuelle FC-Setting beim Start vom OSD an
//
// RUECKGABE:
//   true  = Setting konnte gelesen werden
//   false = Fehler
//-----------------------------------------------------------
uint8_t OSD_Popup_MKSetting( void )
{
    Popup_Draw( 3 );                                                // 3 Zeilen von unten nach oben fuer's Popup
    lcdx_printp_center( 2, PSTR("PKT OSD")   , MNORMAL,  0, 0);
    lcdx_printp_center( 6, PSTR("MK Setting"), MINVERS,  0,-8);

    MK_Setting_load( 0xff, 9 );                                     // 0xff == aktuelles Parameterset holen; 9 == timeout

    MKVersion_Setting_print( 7, MINVERS,  0,-4);                    // aus: mkbase.c

    if( MKVersion.mksetting == 0 )
        set_beep( 500, 0xffff, BeepNormal );                        // kein Setting - langer Beep ERROR


    clear_key_all();
    timer = 300;                                                    // ca. 3 Sekunden zeigen; Abbruch mit einer Taste moeglich
    while( timer > 0 && !get_key_press(0xff) );

    clear_key_all();
    lcd_cls();

    return (MKVersion.mksetting != 0);
}



//-----------------------------------------------------------
//-----------------------------------------------------------
void OSD_Popup_MKError( u8 mkerrorcode )
{
    Popup_Draw( 3 );                                                                                        // 3 Zeilen von unten nach oben fuer's Popup

    lcdx_printf_center_P( 6, MINVERS, 0,-8, PSTR("** MK-%S %02d **"), strGet(STR_ERROR), mkerrorcode );     // "MK-FEHLER" und Fehlernummer

    lcdx_printp_center( 7, (const char*) pgm_read_word(&mkerrortext[mkerrorcode]), MINVERS, 0,-4);          // MK-Fehlertext
}



//-----------------------------------------------------------
//-----------------------------------------------------------
void OSD_Popup_Info( uint8_t ScreenNum, const char *ScreenName)
{
    Popup_Draw( 5 );                            // 5 Zeilen von unten nach oben fuer's Popup

    lcd_line( 3, 53-21, 124, 53-21, 0);         // Linie: oben
    lcd_line( 3, 53, 124, 53, 0);               // Linie: unten

    //-----------------------
    // ScreenNummer: ScreenName
    //-----------------------
    lcdx_printf_at_P( 0, 3, MINVERS, 5,-2, PSTR("%02d: %S"), ScreenNum, ScreenName );

    //-----------------------
    // longpress Key's
    //-----------------------
    lcdx_printp_at( 0, 4, strGet(STR_LONGPRESS), MINVERS, 6,3); // "langer Tastendruck:"
    lcdx_printp_at(12, 5, PSTR("Disp"), MINVERS, 0,4);
    lcdx_printp_at(17, 5, PSTR("UGps"), MINVERS, 0,4);

    //-----------------------
    // shortpress Key's
    //-----------------------
    lcd_printp_at( 0, 7, strGet(KEYLINE3), MINVERS);
    lcd_printp_at(17, 7, PSTR("Info")    , MINVERS);
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void GPS_Pos_Save( pkt_gpspos_t *pGPS )
{
    pGPS->Altimeter         = naviData->Altimeter;                              // barymetrische Hoehe
    pGPS->HomeDistance      = naviData->HomePositionDeviation.Distance;         // Entfernung Home

    memcpy( &pGPS->GPSData, &naviData->CurrentPosition, sizeof(GPS_Pos_t) );    // sichern...
    memcpy( &pGPS->timestamp, (char *)&UTCTime, sizeof(PKTdatetime_t) );        // sichern...
}


//--------------------------------------------------------------
//--------------------------------------------------------------

//--------------------------------------------------------------
// GPS_User_Init()
//
// initialisiert die GPS Positionen neu
//--------------------------------------------------------------

void GPS_User_Init( void )
{
    memset( Config.GPS_User, 0, sizeof(pkt_gpspos_t)*MAX_GPS_USER );
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void GPS_User_Save( void )
{
    uint8_t i;

    if( naviData->NCFlags & NC_FLAG_GPS_OK )        // nur wenn MK-GPS ok ist
    {
        for( i=MAX_GPS_USER-1; i>0; i--)
        {
            Config.GPS_User[i] = Config.GPS_User[i-1];
        }
        GPS_Pos_Save( &Config.GPS_User[0] );

        set_beep( 160, 0xffff, BeepNormal );        // Beep Ok
    }
    else
    {
        set_beep( 600, 0x000f, BeepNormal );        // Beep Error (keine gueeltigen GPS-Daten)
    }
}



//##############################################################
//##############################################################


//--------------------------------------------------------------
//--------------------------------------------------------------
void MKLiPoCells_Init( void )
{
    CellIsChecked     = 0;
    cells             = 0;
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void MKLiPoCells_Check( void )
{
    if( cells == 0 )                        // Zellenzahl noch nicht ermittelt?
    {
        // up to 6s LiPo, less than 2s is technical impossible
        for( cells = 2; cells < 7; cells++)
        {
            if( naviData->UBat < cells * MAX_CELL_VOLTAGE ) break;
        }
        Config.OSD_Statistic.LiPoCells = cells;
    }
}



//##############################################################
//# OSD MAIN LOOP
//##############################################################

//--------------------------------------------------------------
// OSD MAIN LOOP
//--------------------------------------------------------------
void osd( void )
{
    uint8_t     osdexit     = false;
    uint8_t     mktimeout   = false;
    uint8_t     flying      = false;
    uint8_t     popup_state = OSD_POPUP_NONE;


    lcd_cls();

    #ifdef DEBUG_OSD_TIME
      // Fake-Zeit/Datum setzen wenn der NC-Simulator verwendet wird
      if( UTCTime.year < 2000 )
      {
          UTCTime.seconds = ((uint32_t)13*3600)+(15*60)+42;   // 13:15:42
          UTCTime.day     = 01;
          UTCTime.month   = 05;
          UTCTime.year    = 2013;
      }
    #endif


    //----------------------------------------
    // Anzeige: aktuelles MK-Setting
    //----------------------------------------
    if( (Config.OSD_ShowMKSetting) )
    {
        if( !OSD_Popup_MKSetting() )
            return;
    }


    /*
    //-----------------------------------------------------------------------------------------------
    // 07.03.2013 OG: del
    //   Dieser Teil hat immer wieder Probleme bereitet bei der Verbindung des PKT-OSD zum MK da
    //   MK_TIMEOUTs zustande kamen. Eine Recherche im Code ergab, dass die Nutzdaten die
    //   hierueber bezogen werden sich lediglich auf Flags_ExtraConfig beschraenkten (CFG2_HEIGHT_LIMIT).
    //   Siehe dazu untere Kommentare.
    //
    //   Der negative Effekt moeglicher MK_TIMEOUTs und Verzoegerungen sind es aktuell nicht Wert
    //   CFG2_HEIGHT_LIMIT zu unterstuetzen. Dieses Feature ist erstmal raus.
    //
    //   Falls gewuenscht wird, dass CFG2_HEIGHT_LIMIT wieder in das PKT-OSD kommt muss
    //   es zuverlaessig an anderer Stelle implementiert werden - und zwar nicht in osd.c
    //   weil es eine statische FC-Information ist (ggf. beim Verbindungsaufbau PKT <-> MK).
    //
    //   Hat auch aktuell Auswirkung auf den Code OSD_Element_AltitudeControl()
    //-----------------------------------------------------------------------------------------------
    //lcd_printp_at( 0, 3, PSTR("connecting MK..."), 0);
    //
    //SwitchToFC();
    //
    //status = load_setting(0xff);
    //
    //if( status == 255 )
    //{
    //    lcd_printp_at(0, 0, PSTR("Keine Settings !!"), 0);    // Keine Settings
    //    _delay_ms(2000);
    //}
    //Flags_ExtraConfig   = mk_param_struct->ExtraConfig;     // OG: wird in osd.c nur verwendet von: OSD_Element_AltitudeControl()
    //Flags_GlobalConfig  = mk_param_struct->GlobalConfig;    // OG: wird nicht in osd.c verwendet
    //Flags_GlobalConfig3 = mk_param_struct->GlobalConfig3;   // OG: wird nicht in osd.c verwendet
    */



    //-------------------------
    // MK-Display initialisieren
    //-------------------------
    memset( mkdisplayData, 0, 81 );
    mkdisplayMode   = false;
    mkdisplayCmd    = 0xff;


    //-------------------------
    // BL-DATA initialisieren
    //-------------------------
    memset( blData, 0, sizeof(BLData_t)*OSD_MAX_MOTORS );


    //-------------------------
    // Debug initialisieren
    //-------------------------
    #ifdef USE_OSD_SCREEN_DEBUG
        readCounterOSD      = 0;
        readCounterTIME     = 0;
        readCounterDISPLAY  = 0;
        memset( readCounterBL, 0, sizeof(uint16_t)*OSD_MAX_MOTORS );
    #endif  // USE_OSD_SCREEN_DEBUG



    //-------------------------
    // NC Datenkommunikation starten
    //-------------------------
    OSD_MK_Connect( MK_CONNECT );

    OSD_active  = true;                 // benoetigt fuer Navidata Ausgabe an SV2


    //-------------------------
    // Clear statistics
    //-------------------------
    //STAT_Init();

    CellIsChecked     = 0;
    cells             = 0;
    AkkuWarnThreshold = 0;
    OldWP             = 0;
    NextWP            = false;
    old_PKTErrorcode  = 0;
    old_MKErrorcode   = 0;



    //-------------------------
    // Init: OSD-Screens
    //-------------------------
    ScreenCtrl_Init();
    ScreenCtrl_Push( OSDSCREEN_GENERAL      , strGet(STR_OSDSCREEN_GENERAL)   , &OSD_Screen_General    );

    #ifdef USE_OSD_SCREEN_NAVIGATION
    ScreenCtrl_Push( OSDSCREEN_NAVIGATION   , strGet(STR_OSDSCREEN_NAVIGATION), &OSD_Screen_Navigation );
    #endif

    #ifdef USE_OSD_SCREEN_WAYPOINTS
    ScreenCtrl_Push( OSDSCREEN_WAYPOINTS    , strGet(STR_OSDSCREEN_WAYPOINTS) , &OSD_Screen_Waypoints  );
    #endif

    // *ALTERNATIVE*
    //#ifdef USE_OSD_SCREEN_WAYPOINTS
    //ScreenCtrl_Push( OSDSCREEN_WAYPOINTS    , strGet(STR_OSDSCREEN_WAYPOINTS) , &OSD_Screen_Waypoints0 );
    //#endif

    #ifdef USE_OSD_SCREEN_ELECTRIC
    ScreenCtrl_Push( OSDSCREEN_ELECTRIC     , strGet(STR_OSDSCREEN_ELECTRIC)  , &OSD_Screen_Electric   );
    #endif

    #ifdef USE_OSD_SCREEN_MKSTATUS
    ScreenCtrl_Push( OSDSCREEN_MKSTATUS     , strGet(STR_OSDSCREEN_MKSTATUS)  , &OSD_Screen_MKStatus   );
    #endif

    #ifdef USE_OSD_SCREEN_USERGPS
    ScreenCtrl_Push( OSDSCREEN_USERGPS      , strGet(STR_OSDSCREEN_USERGPS)   , &OSD_Screen_UserGPS    );
    #endif

    #ifdef USE_OSD_SCREEN_3DLAGE
    ScreenCtrl_Push( OSDSCREEN_3DLAGE       , strGet(STR_OSDSCREEN_3DLAGE)    , &OSD_Screen_3DLage     );
    #endif

    #ifdef USE_OSD_SCREEN_STATISTIC
    ScreenCtrl_Push( OSDSCREEN_STATISTICS   , strGet(STR_OSDSCREEN_STATISTIC) , &OSD_Screen_Statistics );
    #endif

    #ifdef USE_OSD_SCREEN_OLD
    ScreenCtrl_Push( OSDSCREEN_OSD0         , strGet(STR_OSDSCREEN_OSD0)      , &OSD_Screen_OSD0       );
    ScreenCtrl_Push( OSDSCREEN_OSD1         , strGet(STR_OSDSCREEN_OSD1)      , &OSD_Screen_OSD1       );
    ScreenCtrl_Push( OSDSCREEN_OSD2         , strGet(STR_OSDSCREEN_OSD2)      , &OSD_Screen_OSD2       );
    #endif

    #ifdef USE_OSD_SCREEN_DEBUG
    ScreenCtrl_Push( 0                      , PSTR("Debug")                   , &OSD_Screen_Debug      );
    ScreenCtrl_Push( 0                      , PSTR("Debug-RX")                , &OSD_Screen_Debug_RX   );
    #endif

    ScreenCtrl_Set( Config.OSD_ScreenMode );


    //-------------------------
    // Init: Timer & Flags
    //-------------------------
    OSDScreenRefresh      = OSD_SCREEN_REDRAW;
    timer_mk_timeout      = MK_TIMEOUT;
    timer_osd_refresh     = TIME_OSD_REFRESH;
    timer_get_bldata      = 0;
    timer_get_tidata      = 0;
    timer_get_displaydata = 0;


    //--------------------------------
    // OSD Main-Loop
    //--------------------------------
    while( !osdexit )
    {
        //################################
        //# Empfange/verarbeite: OSD-Daten
        //################################
        if( rxd_buffer_locked )                     // naviData Ok?
        {
            Decode64();
            memcpy( &osdData, (const void *)pRxData, sizeof(NaviData_t) );       // sichern in: osdData
            naviData = &osdData;

            #ifdef USE_OSD_SCREEN_DEBUG
                readCounterOSD++;                   // gelesene Datenpakete
            #endif

            mktimeout = false;


            //----------------------------------
            // LiPo Cell Check
            //----------------------------------
            if( cells == 0 )                        // Zellenzahl noch nicht ermittelt?
            {
                // up to 6s LiPo, less than 2s is technical impossible
                for( cells = 2; cells < 7; cells++)
                {
                    if( naviData->UBat < cells * MAX_CELL_VOLTAGE ) break;
                }
                Config.OSD_Statistic.LiPoCells = cells;
            }


            //----------------------------------
            // Winkel zu Home
            //----------------------------------
            calc_heading_home();


            //----------------------------------
            // speichere letzte GPS-Positionen
            //----------------------------------
            GPS_Pos_Save( &GPS_Current );
            Config.LastLatitude  = GPS_Current.GPSData.Latitude;    // speichere letzte Position in Config
            Config.LastLongitude = GPS_Current.GPSData.Longitude;   // speichere letzte Position in Config


            //----------------------------------
            // PKT Fehler reset
            //----------------------------------
            if( old_PKTErrorcode == 40 )                            // 40 = PKT Empfangsausfall ("PKT RX lost")
            {
                // PKT-Verbindungsfehler zuruecksetzen
                // da an dieser Stelle ja bereits wieder ein gueltiges Datenpaket
                // von der NaviCtrl empfangen wurde
                old_PKTErrorcode = 0;
                clear_key_all();
            }


            //----------------------------------
            // remember statistics (only when engines running)
            //----------------------------------
            #ifdef DEBUG_OSD_STAT_MOTORRUN
              if( true )
            #else
              if( naviData->FCStatusFlags & FC_STATUS_MOTOR_RUN )     // AM FLIEGEN -> Statistik aufzeichnen
            #endif
            {
                flying = true;

                 // --- gueltige Zeit von der NC vorhanden und noch keine Start-Zeit gesetzt?
                if( UTCTime.year != 0 && Config.OSD_Statistic.begin_StatTime.year == 0 )
                {
                    memcpy( &Config.OSD_Statistic.begin_StatTime, (char *)&UTCTime, sizeof(PKTdatetime_t) );    // Start Zeit/Datum sichern...
                }

                Config.OSD_Statistic.last_FlyTime = naviData->FlyingTime;

                Config.OSD_Statistic.count_osd++;                                                               // Anzahl OSD-Statistik Pakete

                // int32_t calc_avg( int32_t avg, int32_t value, int32_t count, int32_t factor)
                Config.OSD_Statistic.avg_Current = (uint16_t)calc_avg( (int32_t)Config.OSD_Statistic.avg_Current,
                                                                       (int32_t)naviData->Current,
                                                                       (int32_t)Config.OSD_Statistic.count_osd,
                                                                       (int32_t)100 );

                if( naviData->Altimeter > Config.OSD_Statistic.max_Altimeter )      Config.OSD_Statistic.max_Altimeter      = naviData->Altimeter;
                if( naviData->GroundSpeed > Config.OSD_Statistic.max_GroundSpeed )  Config.OSD_Statistic.max_GroundSpeed    = naviData->GroundSpeed;
                if( naviData->HomePositionDeviation.Distance > Config.OSD_Statistic.max_Distance ) Config.OSD_Statistic.max_Distance  = naviData->HomePositionDeviation.Distance;
                if( naviData->Current > Config.OSD_Statistic.max_Current )          Config.OSD_Statistic.max_Current        = naviData->Current;
                if( naviData->UsedCapacity > Config.OSD_Statistic.max_Capacity )    Config.OSD_Statistic.max_Capacity       = naviData->UsedCapacity;
                if( naviData->UBat < Config.OSD_Statistic.min_UBat )                Config.OSD_Statistic.min_UBat           = naviData->UBat;

                if( naviData->TopSpeed > Config.OSD_Statistic.max_TopSpeed )        Config.OSD_Statistic.max_TopSpeed       = naviData->TopSpeed;
                if( naviData->RC_Quality > Config.OSD_Statistic.max_RCQuality )     Config.OSD_Statistic.max_RCQuality      = naviData->RC_Quality;
                if( naviData->RC_Quality < Config.OSD_Statistic.min_RCQuality )     Config.OSD_Statistic.min_RCQuality      = naviData->RC_Quality;

                if( naviData->AngleNick < Config.OSD_Statistic.min_AngleNick )      Config.OSD_Statistic.min_AngleNick      = naviData->AngleNick;
                if( naviData->AngleNick > Config.OSD_Statistic.max_AngleNick )      Config.OSD_Statistic.max_AngleNick      = naviData->AngleNick;

                if( naviData->AngleRoll < Config.OSD_Statistic.min_AngleRoll )      Config.OSD_Statistic.min_AngleRoll      = naviData->AngleRoll;
                if( naviData->AngleRoll > Config.OSD_Statistic.max_AngleRoll )      Config.OSD_Statistic.max_AngleRoll      = naviData->AngleRoll;
            }
            else if( flying && UTCTime.year != 0 )                  // GELANDET -> Statistik beenden
            {
                // --- Ende Zeit/Datum Statistik sichern
                memcpy( &Config.OSD_Statistic.end_StatTime, (char *)&UTCTime, sizeof(PKTdatetime_t) );          // Ende Zeit/Datum sichern...
                flying = false;
            }


            //-----------------------
            // Check: Akku Warnung
            //-----------------------
            CheckMKLipo();


            //----------------------------------
            // Show: OSD-Screens
            //----------------------------------
            if( popup_state == OSD_POPUP_NONE && (timer_osd_refresh == 0 || OSDScreenRefresh == OSD_SCREEN_REDRAW) )
            {
                if( OSDScreenRefresh == OSD_SCREEN_REDRAW )     lcd_cls();

                if( !mkdisplayMode )
                    ScreenCtrl_Show();
                else
                    OSD_Screen_MKDisplay();

                timer_osd_refresh = TIME_OSD_REFRESH;
            }
            OSDScreenRefresh = OSD_SCREEN_REFRESH;


            //----------------------------------
            // Check: MK-Error
            //----------------------------------
            if( naviData->Errorcode != old_MKErrorcode && naviData->Errorcode <= MAX_MKERROR_NUM )
            {
                if( naviData->Errorcode > 0 )                   // raise Error
                {
                    // Fehler aufzeichnen
                    MkError_Save( naviData->Errorcode );

                    // Fehler Anzeigen
                    OSD_Popup_MKError( naviData->Errorcode );
                    set_beep( 1000, 0x000f, BeepNormal);        // Beep Error (MK-Error vorhanden)
                    timer2      = TIME_POPUP_MKERROR;
                    popup_state = OSD_POPUP_MKERROR;
                }
                else                                            // reset Error
                {
                    popup_state       = OSD_POPUP_NONE;
                    OSDScreenRefresh  = OSD_SCREEN_REDRAW;
                }
                old_MKErrorcode = naviData->Errorcode;
            }

            rxd_buffer_locked   = FALSE;                    // ready to receive new data


            //-------------------------------------------
            // hole weitere Daten vom MK (BL, Time, ...)
            //
            // muss am Schluss stehen da naviData-Buffer
            // dabei ueberschrieben wird!
            //-------------------------------------------
            OSD_MK_GetData();                               // holt BL-Daten und NC-Zeit


            timer_mk_timeout = MK_TIMEOUT;

        } //end: if( rxd_buffer_locked )  // OSD-Daten



        //################################
        //# der Rest...
        //################################

        //--------------------------------
        // TASTEN: MK-Timeout
        //--------------------------------
        if( mktimeout )
        {
            if( get_key_short(1 << KEY_ESC) )                   // PKT OSD EXIT
            {
                osdexit = true;
            }
        }


        //--------------------------------
        // Popup beenden
        // wenn irgendeine Taste gedrückt oder Popup-Timeout
        //--------------------------------
        if( !mktimeout && (popup_state != OSD_POPUP_NONE) && (get_key_press(255) || !timer2) )    // get_key_press(255) == alles an Tasten abfangen was moeglich ist
        {
            popup_state      = OSD_POPUP_NONE;
            OSDScreenRefresh = OSD_SCREEN_REDRAW;
            clear_key_all();
        }


        //--------------------------------
        // TASTEN: KEIN mkdisplay (OSD Modus)
        //--------------------------------
        if( !osdexit && !mktimeout && !mkdisplayMode )
        {
            if( get_key_short(1 << KEY_ESC) )                   // PKT OSD EXIT
            {
                osdexit = true;
            }

            if( get_key_long(1 << KEY_ESC) )                   // ÊINSCHALTEN: mkdisplayMode
            {
                set_beep( 25, 0xffff, BeepNormal );             // kurzer Bestaetigungs-Beep
                OSDScreenRefresh = OSD_SCREEN_REDRAW;
                mkdisplayMode    = true;
                mkdisplayCmd     = 0;                           // lesen display-Daten erzwingen
            }

            if( get_key_long (1 << KEY_ENTER) )                 // User GPS-Position sichern
            {
                GPS_User_Save();
            }

            if( get_key_short (1 << KEY_ENTER) )                // Popup: Info
            {
                if( popup_state == OSD_POPUP_NONE )
                {
                    popup_state = OSD_POPUP_INFO;
                    OSD_Popup_Info( ScreenCtrl_GetNum(), ScreenCtrl_GetName() );
                    timer2 = TIME_POPUP_INFO;
                }
            }

            if( get_key_press (1 << KEY_MINUS))                 // previous screen
            {
                ScreenCtrl_Previous();
            }

            if( get_key_press (1 << KEY_PLUS))                  // next Screen
            {
                ScreenCtrl_Next();
            }

        }



        //--------------------------------
        // TASTEN: mkdisplay AKTIV
        //--------------------------------
        if( !osdexit && !mktimeout && mkdisplayMode )
        {
            /*
            if(    get_key_long(1 << KEY_ENTER)                 // ABSCHALTEN mkdisplayMode: longpress ENTER, ESC, MINUS, PLUS schaltet mkdisplay aus
                || get_key_long(1 << KEY_ESC)
                || get_key_long(1 << KEY_MINUS)
                || get_key_long(1 << KEY_PLUS) )
            {
            */

            if( get_key_long(1 << KEY_ESC) )                    // ABSCHALTEN mkdisplayMode: longpress ESC (Taste 3 von links)
            {
                set_beep( 25, 0xffff, BeepNormal );             // kurzer Bestaetigungs-Beep beim Modus-Wechsel
                OSDScreenRefresh = OSD_SCREEN_REDRAW;
                mkdisplayMode    = false;
                clear_key_all();
            }

            if( get_key_short (1 << KEY_MINUS) )   mkdisplayCmd = 0xfe; // MK-Key: rechts (next page)
            if( get_key_short (1 << KEY_PLUS)  )   mkdisplayCmd = 0xfd; // MK-Key: links (previous page)
            if( get_key_short (1 << KEY_ESC)   )   mkdisplayCmd = 0xfb; // MK-Key: runter
            if( get_key_short (1 << KEY_ENTER) )   mkdisplayCmd = 0xf7; // MK-Key: hoch

            if( mkdisplayCmd != 0xff )                          // wenn eine MK-Display Taste gedrueckt worde sofort Daten
            {                                                   // holen und darstellen um Anzeigereaktion fuer Benutzer zu verbessern
                timer_get_displaydata = 0;                      // lesen erzwingen
                OSD_MK_GetData();                               // Daten holen
                OSD_Screen_MKDisplay();                         // MK-Display Anzeigen
            }
        }



        //-----------------------
        // abo_timer
        //-----------------------
        if( abo_timer == 0 )
        {
            OSD_MK_Connect( MK_ABORENEW );
        }


        //--------------------------
        // Daten Timeout vom MK?
        //--------------------------
        if( timer_mk_timeout == 0 )
        {
            if( !mktimeout ) OSD_MK_ShowTimeout();              // nur anzeigen wenn noch nicht im mktimeout-Modus
            set_beep ( 200, 0x0080, BeepNormal);                // Beep
            mktimeout        = true;
            timer_mk_timeout = MK_TIMEOUT;
            OSDScreenRefresh = OSD_SCREEN_REDRAW;
            OSD_MK_Connect( MK_CONNECT );                       // 21.06.2014 OG: wieder aktviert wegen Umschaltung auf NC
        }



        //--------------------------
        // Pruefe auf PKT-Update und
        // andere interne PKT-Aktionen
        //--------------------------
        #ifdef USE_OSD_PKTHOOK
          if( PKT_CtrlHook() )  OSDScreenRefresh = OSD_SCREEN_REDRAW;    // Update vom Updatetool angefordert?
        #endif

    } // END: while( !osdexit )



    //------------------------------------------
    // PKT-OSD beenden
    //------------------------------------------
    Config.OSD_ScreenMode = ScreenCtrl_GetNum();                    // merken letzter Screen


    //------------------------------------------
    // ggf. Statistik Ende Zeit/Datum sichern
    //------------------------------------------
    if( flying && UTCTime.year != 0 )                                   // wenn noch am fliegen
    {
        // --- Ende Zeit/Datum Statistik sichern
        memcpy( &Config.OSD_Statistic.end_StatTime, (char *)&UTCTime, sizeof(PKTdatetime_t) );  // Ende Zeit/Datum sichern...
    }


    OSD_active = false;
}

