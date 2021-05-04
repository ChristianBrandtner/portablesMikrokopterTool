/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */
/* ISO-8859-1  do not change it, otherwise the LCD Implementation fails*/

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
//# HISTORY  main.c
//#
//# 14.05.2014 OG
//# - chg: include "mk/mksettings.h" geaendert auf "mksettings/mksettings.h"
//#
//# 29.03.2014 OG
//# - chg: MK_Show_LastGPS_Position() ersetzt durch OSDDATA_ShowLastGPSPosition()
//#
//# 28.03.2014 OG
//# - chg: mk_last_position() ersetzt durch MK_Show_LastGPS_Position() (mkbase.c)
//# - del: mk_last_position()
//#
//# 21.02.2014 OG
//# - chg: mk_show_lastposition() auf PKT_TitlePKTVersion() angepasst
//#
//# 19.02.2014 OG
//# - chg: main() hinzugefuegt: MKSettings_TMP_Init0()
//# - add: #include "mk/mksettings.h"
//#
//# 12.02.2014 OG
//# - del: die includes zu den alten parameter... entfernt
//#
//# 11.02.2014 OG
//# - chg: main() Aufruf von searchMK() bzgl. Parametern angepasst
//#
//# 03.02.2014 OG
//# - chg: Titelanzeige in searchMK() umgestellt auf PKT_ShowTitlePKTVersion()
//#
//# 02.02.2014 OG
//# - chg: Screen-Layout von der Anzeige der letzten MK-Position und
//#        Ausgliederung davon in mk_show_lastposition()
//#
//# 29.01.2014 OG
//# - del: #include "display.h"
//# - chg: verschoben: MK_load_setting() und searchMK() nach mk/mkbase.c
//# - add: #include "mk/mkbase.h"
//#
//# 25.01.2014 OG
//# - chg: searchMK(): kuerzerer Fehlerbeep bei falscher FC-Revision und
//#        automatisches beenden des Fehlerscreens nach 4 Sekunden
//#
//# 24.01.2014 OG
//# - fix: searchMK(): wenn MK_load_setting() keine Daten liefert wird keine
//#        falsche RC-Settings-Revisionsnummer mehr angezeigt sondern "ERR!"
//# - chg: searchMK(): Bestaetigung Fehleranzeige bei falscher FC-Revision
//#        geaendert von KEY_ESC nach KEY_ENTER - Grund: das PKT behandelt
//#        dieses Problem nun flexibler und z.B. OSD (und anderes) geht noch
//# - chg: searchMK(): beschleunigte Erkennung inkompatibler FC-Settings-Revisionen
//# - chg: timings in MK_load_setting()
//#
//# 07.1.2016 Cebra
//# - chg: setzen globale Variable für Abfrage WrongFCVersion etwas verändert
//#
//# 26.06.2013 OG
//# - del: searchMK() Layout und Code-Struktur
//# - fix: searchMK() laden der MK-Settings
//# - add: MK_load_setting() - wird ggf. spaeter zu einer bessern Stelle verschoben
//# - del: Code zu USE_PKT_STARTINFO
//#
//# 20.05.2013 OG
//# - chg: searchMK() Code fuer KEY_ESC geaendert
//#
//# 19.05.2013 OG
//# - del: PKT_Update(), PKT_CheckUpdate(), PKT_Info(), PKT_SwitchOff()
//#        -> verschoben nach pkt/pkt.c
//# - chg: Aufruf von main_menu() geaendert zu Menu_Main()
//#
//# 19.05.2013 OG
//# - add: PKT_Update(), PKT_CheckUpdate(), PKT_Info(), PKT_SwitchOff()
//#
//# 16.05.2013 Cebra
//# - chg: Startinfo wird nur noch komplett angezeigt wenn im Setup auf JA
//#
//# 15.05.2013 OG
//# - chg: define USE_PKT_STARTINFO ergaenzt (siehe main.h)
//#
//# 07.05.2013 Cebra
//# - chg: wenn NC vorhanden wird die Version der NC in NCversion gespeichert
//#        sonst die FC-Version in FCversion
//#
//# 27.04.2013 OG
//# - chg: GPS-Positionsausgabe umgestellt auf writex_gpspos()
//# - chg: einige clear-lines auf lcd_frect() umgestellt um Speicher zu sparen
//############################################################################


#include "cpu.h"
#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/eeprom.h>


//************************************************************************************
// Watchdog integrieren und abschalten, wird für Bootloader benötigt
// !!muss hier stehen bleiben!!

//--------------------------------------------------------------
void wdt_init(void) __attribute__((naked)) __attribute__((section(".init1")));

//--------------------------------------------------------------
void wdt_init(void)
{
    MCUSR = 0;
    wdt_disable();
    return;
}

//************************************************************************************
// erst ab hier weitere #includes


#include "main.h"
#include "lcd/lcd.h"
#include "uart/usart.h"
#include "mk-data-structs.h"
#include "menu.h"
#include "timer/timer.h"
#include "eeprom/eeprom.h"
#include "wi232/Wi232.h"
#include "motortest/twimaster.h"
#include "messages.h"
#include "utils/scrollbox.h"
#include "pkt/pkt.h"
#include "lipo/lipo.h"
#include "mk/mkbase.h"
#include "mksettings/mksettings.h"
#include "osd/osddata.h"



//----------------------------------------------------------------------------
// Anmerkung: 29.01.2014 OG
// - muss spaeter an geeignete Stelle verschoben werden (ggf. mkbase.c/usart.c)
//----------------------------------------------------------------------------
volatile uint8_t mode = 0;
uint8_t hardware = 0;
uint8_t current_hardware = 0;



//--------------------------------------------------------------
//--------------------------------------------------------------
int main(void)
{
	InitHWPorts();                      // Hardwareanhängige Ports konfigurieren
										// dafür wird je nach Hardware die HAL_HWxxx verwendet
                                        // Define dazu in der main.h
    hardware = NO;
    current_hardware = 0;

    MKSettings_TMP_Init0();             // TMP-Setting initialisieren

    if( Config.LastLongitude>0x00000000 && Config.LastLatitude>0x00000000 )
    {
        OSDDATA_ShowLastGPSPosition();  // letzte MK GPS-Position zeigen
    }

    searchMK(true);                     // MK suchen (true = MK_Info() anzeigen)

    while(true)
    {
        Menu_Main();
    }
}
