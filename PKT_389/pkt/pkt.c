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
//# HISTORY  pkt.c
//#
//# 25.09.2015 Cebra
//# - add: Erweiterung in PKT_CheckUpdate für serielle Debugfunktionen
//#
//# 21.09.2015 Cebra
//# - add: PKT-Info um Kompass, Luftdruck und FollowMe Step2 erweitert
//#
//# 25.06.2014 OG
//# - chg: PKT_Ask(), PKT_Ask_P() auf PKT_AskX() angepasst
//# - add: PKT_AskX() - ehemals _pkt_ask(), jetzt erweitert um zusaetzliche progmen Parameter und exportiert
//#
//# 24.06.2014 OG
//# - add: PKT_Gauge_Begin(), PKT_Gauge_End(), PKT_Gauge_Next()
//#
//# 15.06.2014 OG
//# - add: PKT_Progress_Init(), PKT_Progress_Next()
//#
//# 14.06.2014 OG
//# - add: PKT_TitlePKTlipo() fuer optionale Anzeige der LiPo-Spannung
//#
//# 13.06.2014 OG
//# - add: PKT_Ask_Restart()
//#
//# 12.06.2014 OG
//# - add: PKT_Reset_EEprom()  (ehemals in setup.c)
//# - chg: PKT_Update() auf PKT_Ask_P() umgestellt
//# - add: PKT_Ask(), PKT_Ask_P(), _pkt_ask()
//#
//# 11.06.2014 OG
//# - add: PKT_Title(), PKT_Title_P(), _pkt_title()
//# - chg: PKT_TitleFromMenuItem() umgestellt auf _pkt_title()
//# - chg: PKT_TitlePKTVersion() umbenannt zu PKT_TitlePKT()
//# - fix: PKT_TitleFromMenuItem() funktioniert jetzt auch wenn kein Menue vorhanden ist
//#
//# 04.06.2014 OG
//# - add: PKT_Message_Datenverlust()
//#
//# 31.05.2014 OG
//# - fix: PKT_CtrlHook() bzgl. PKT_CheckUpdate() - seit Februar 2014 Probleme
//#        mit Update-Anforderung vom PKT-Updatetool -> PKT war zu schnell
//#        Es wurde jetzt ein Timer in PKT_CtrlHook() eingebaut der den Aufruf
//#        von PKT_CheckUpdate() etwas einbremst ohne das PKT auszubremsen.
//#        Ich hoffe, das es ist Loesung ist (bin guter Dinge nach Test's)
//# - chg: PKT_CheckUpdate() - Code optimiert
//# - add: PKT_Info() - Anzeige ob SV2-Patch vorhanden ist oder nicht (ganz oben)
//# - add: PKT_Info() - Anzeige fuer USE_OSD_SCREEN_WAYPOINTS
//# - add: PKT_KeylineUpDown()
//#
//# 23.05.2014 OG
//# - add: PKT_TitleFromMenuItem()
//# - add: #include "../utils/menuctrl.h"
//#
//# 06.05.2014 OG
//# - chg: PKT_CheckUpdate() - seit Februar funktioniert die Update-Anforderung
//#        vom PKT-Updatetool an das PKT nicht immer zuverlaessig - fgf. weil sich
//#        seitens des PKT's Timings etwas geaendert haben (durch weglassen von Code).
//#        Hier ein Versuch etwas daran wieder zu aendern... its zwar weiterhin
//#        nicht immer zuverlaessig aber vieleicht etwas besser.
//#        Wahrscheinlich kann man das nur richtig korrigieren in Verbindung mit
//#        Anpassungen am PKT-Updatetool - aber das ist ein anderes Thema...
//# - chg: PKT_Update() - ein kurzer Beep wenn das PKT-Update aufgerufen wird
//#
//# 05.05.2014 OG
//# - add: PKT_Popup(), PKT_Popup_P(), _pkt_popup()
//#
//# 11.04.2014 OG
//# - chg: _pkt_message() ergaenzt um clear_key_all()
//#
//# 09.04.2014 OG
//# - chg: PKT_Update() - umgestellt auf ShowTitle_P()
//#
//# 08.04.2014 OG
//# - chg: PKT_Update() - Text bzgl. "Druecke Start..." zentriert
//#
//# 04.04.2014 OG
//# - fix: define ESC umbenannt zu PKTESC da es einen Namenskonflikt mit enum
//#        STR in messages.h gab
//# - add: _pkt_message() zeigt einen Titel mit PKT Version und Lipo an
//#
//# 03.04.2014 OG
//# - chg: _pkt_message() abgesichert bzgl. zu langen Texten
//#
//# 27.03.2014 OG
//# - chg: PKT_SwitchOff() Anzeige optimiert
//#
//# 21.02.2014 OG
//# - fix: PKT_CheckUpdate() Zeile 0 flimmerte wegen einer nicht mehr
//#        benoetigten Textausgabe
//# - chg: PKT_SwitchOff() auf PKT_TitlePKTVersion() angepasst
//# - chg: PKT_TitlePKTVersion()
//#
//# 17.02.2014 OG
//# - add: PKT_Message(), PKT_Message_P(), _pkt_message()
//# - chg: MK_Info() auf USE_MKSETTINGS angepasst (ehemals MKPARAMETER)
//#
//# 13.02.2014 OG
//# - chg: Screenlayout von PKT_Update()
//# - chg: Screenlayout von PKT_SwitchOff()
//#
//# 11.02.2014 OG
//# - chg: PKT_Info() Anzeige der FC-Version entfernt
//#
//# 04.02.2014 OG
//# - fix: #include "../lipo/lipo.h" hinzugefuegt
//#
//# 03.02.2014 OG
//# - chg: Layout von PKT_SwitchOff() und bessere Unterstuetzung von PKT_CtrlHook()
//# - add: PKT_ShowTitleVersion()
//#
//# 30.01.2014 OG
//# - chg: geaenderte Anzeige der Reihenfolgen unterstuetzter Module in PKT_Info()
//# - add: Unterstuetzung fuer USE_BLUETOOTH
//#
//# 24.01.2014 OG
//# - chg: PKT_SwitchOff(): Layout; Code eliminiert; Beep;
//#        add PKT_Hook() (für PKT-Update)
//# - chg: PKT_Update(): Layout
//#
//# 21.10.2013 CB
//# - add: PKT_Info WyFly hinzugefügt
//#
//# 07.07.2013 OG
//# - del: PKT_Info(): USE_OSD_SCREEN_ELECTRIC_N

//# 26.06.2013 OG
//# - del: PKT_Info(): USE_PKT_STARTINFO
//#
//# 13.06.2013 OG
//# - add: PKT-Hardwareversion in der Titelanzeige von PKT_Info()
//# - chg: Layout Titelanzeige von PKT_Info()
//# - del: USE_PCFASTCONNECT in PKT_Info() (nicht mehr benoetigt)
//# - del: PC_Fast_Connect() - ersetzt durch Menu_PKTTools() in menu.c
//#
//# 20.05.2013 OG
//# - chg: PKT_CtrlHook() bei PKT-Update um WriteParameter() ergaenzt
//#
//# 19.05.2013 OG
//# - add: PKT_CtrlHook() ruft ggf. PKT_Update() auf und kann spaeter weitere
//#        PKT interne Sachen verarbeiten wie z.B. senden von Daten an den PC
//# - chg: PKT_CheckUpdate() umgestellt auf timer_pktctrl um Konflikte mit
//#        mit z.B. osd zu vermeiden
//# - chg: PKT_Info() bzgl. Codeverbrauch verringert
//# - fix: PKT_Info() Anzeige von USE_OSD_SCREEN_NAVIGATION
//# - add: PC_Fast_Connect() aus menu.c
//# - add: PKT_* Funktionen aus main.c bzw. menu.c
//############################################################################


#include "../cpu.h"
#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <util/atomic.h>

#include "../main.h"
#include "../lcd/lcd.h"
#include "../uart/usart.h"

#include "../uart/uart1.h"
#include "../mk-data-structs.h"
#include "../timer/timer.h"
#include "../eeprom/eeprom.h"
#include "../messages.h"
#include "../utils/scrollbox.h"
#include "../utils/xutils.h"
#include "../lipo/lipo.h"
#include "../utils/menuctrl.h"
#include "pkt.h"
#include "pkt_debugserial.h"


//#############################################################################################
//#############################################################################################

uint8_t pkt_progress_act    = 0;
uint8_t pkt_progress_max    = 0;
int8_t  pkt_progress_yoffs  = 0;
uint8_t pkt_progress_width  = 0;
uint8_t pkt_progress_height = 0;

#define PROGRESS_YOFFS  -8
#define PROGRESS_WIDTH  96



//#############################################################################################
//#############################################################################################

//--------------------------------------------------------------
//--------------------------------------------------------------
void PKT_Progress_Init( uint8_t max, int8_t yoffs, uint8_t width, uint8_t height)
{
    pkt_progress_act    = 0;
    pkt_progress_max    = max;
    pkt_progress_yoffs  = yoffs;
    pkt_progress_width  = 0;        // Parameter width - aktuell nicht unterstuetzt
    pkt_progress_height = 0;        // Parameter height - aktuell nicht unterstuetzt

    if( pkt_progress_width == 0 )
        pkt_progress_width = PROGRESS_WIDTH;

    lcd_rect_round( 13, 32+pkt_progress_yoffs, 102, 6, 1, R1);              // Rahmen fuer Progress
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t PKT_Progress_Next( void )
{
    uint8_t width;

    pkt_progress_act++;                                                 // Progress: hochzaehlen
    width = (pkt_progress_act*pkt_progress_width) / pkt_progress_max;           // Progress: Breite berechnen
    lcd_frect( 16, 34+pkt_progress_yoffs, width, 2,  1);                // Progress: anzeigen

    return true;
}



//#############################################################################################
//# Anzeigefunktionen
//#############################################################################################


//--------------------------------------------------------------
// PKT_KeylineUpDown( xUp, xDown,  xoffs,yoffs)
//
// gibt in der Keyline (y=7) ein Up- und Down-Zeichen aus.
// Das Up-Zeichen wird dabei um einen Pixel versetzt damit
// es besser aussieht
//
// Parameter:
//  xUp  : x-Position des Up-Zeichens in Zeile 7
//  xDown: x-Position des Down-Zeichens in Zeile 7
//  xoffs, yoffs: normalerweise 0,0
//--------------------------------------------------------------
void PKT_KeylineUpDown( uint8_t xUp, uint8_t xDown,  uint8_t xoffs, uint8_t yoffs)
{
    lcdx_printp_at( xUp  , 7, PSTR("\x1a")    , MNORMAL, xoffs,yoffs+1);    // Up
    lcdx_printp_at( xDown, 7, PSTR("\x1b")    , MNORMAL, xoffs,yoffs);      // Down
}



//#############################################################################################
//# Titel Funktionen
//#############################################################################################

//--------------------------------------------------------------
//--------------------------------------------------------------
void _pkt_title( uint8_t progmem, const char *text, uint8_t lShowLipo, uint8_t clearscreen )
{
    const char *pMask;

    if( clearscreen )   lcd_cls();

    lcd_frect( 0, 0, 127, 7, 1);                             // Titel: Invers

    if( progmem )   pMask = PSTR("%19S");
    else            pMask = PSTR("%19s");

    lcdx_printf_at_P( 1, 0, MINVERS, 0,0, pMask, text );

    if( lShowLipo )     show_Lipo();
}



//--------------------------------------------------------------
// PKT_Title( text, lShowLipo, clearscreen )
//
// text im RAM
//--------------------------------------------------------------
void PKT_Title( const char *text, uint8_t lShowLipo, uint8_t clearscreen )
{
    _pkt_title( false, text, lShowLipo, clearscreen );
}



//--------------------------------------------------------------
// PKT_Title_P( text, lShowLipo, clearscreen )
//
// text im PROGMEM
//--------------------------------------------------------------
void PKT_Title_P( const char *text, uint8_t lShowLipo, uint8_t clearscreen )
{
    _pkt_title( true, text, lShowLipo, clearscreen );
}



//--------------------------------------------------------------
// zeigt als Titel die PKT-Version (invers)
// optional: rechts die PKT Lipo-Spannung anzeigen
//--------------------------------------------------------------
void PKT_TitlePKTlipo( uint8_t lShowLipo )
{
    //PKT_Title( buffered_sprintf_P(PSTR("PKT v%S"),PSTR(PKTSWVersion)), true, false ); // showlipo, kein clearscreen
    PKT_Title( buffered_sprintf_P(PSTR("PKT v%S"),PSTR(PKTSWVersion)), lShowLipo, true );   // showlipo und clearscreen
}



//--------------------------------------------------------------
// zeigt als Titel die PKT-Version (invers)
// und rechts die PKT Lipo-Spannung
//--------------------------------------------------------------
void PKT_TitlePKT( void )
{
    PKT_TitlePKTlipo( true );
}



//--------------------------------------------------------------
// PKT_TitleFromMenuItem( lShowLipo )
//
// zeigt eine PKT-Titelzeile mit Text vom aufrufenden Menuepunkt
// optional kann die PKT LiPo-Spannung angezeigt werden
//
// wenn kein Menue vorhandenist wird die PKT-Version angezeigt
//--------------------------------------------------------------
void PKT_TitleFromMenuItem( uint8_t lShowLipo )
{
    const char *pStr;
    uint8_t     isPGM;

    if( MenuCtrl_GetMenuIndex() < 0 )
    {
        PKT_TitlePKT();                                     // kein Menue vorhanden -> PKT-Version anzeigen
        return;
    }

    pStr  = MenuCtrl_GetItemText();
    isPGM = MenuCtrl_IsItemTextPGM();

    _pkt_title( isPGM, pStr, lShowLipo, true );             // true = clearscreen
}



//#############################################################################################
//# PKT Message
//#############################################################################################

//--------------------------------------------------------------
// _pkt_message( progmem, text, error, timeout, beep, clearscreen)
//
// INTERN
// fuer: PKT_Message(), PKT_Message_P()
// Parameter? -> siehe dort...
//--------------------------------------------------------------
void _pkt_message( uint8_t progmem, const char *text, uint8_t error, uint16_t timeout, uint8_t beep, uint8_t clearscreen )
{
    char *pStr;

    if( clearscreen )
        lcd_cls();

    PKT_TitlePKT();

    if( error )
    {
        //lcd_frect_round(   0+36, (2*8)-2, 127-73, 8+2, 1, R1);        // Fill: Schwarz
        lcd_frect_round(   0+36, (2*8)-1, 127-74, 8+2, 1, R1);          // Fill: Schwarz
        lcdx_printp_center( 2, strGet(STR_ERROR), MINVERS, 0,1);        // "FEHLER"
    }
    else
        lcdx_printp_center( 2, strGet(STR_PKT)  , MNORMAL, 0,1);        // "PKT"


    //-----------------------
    // Ausgabe der Nachricht
    //-----------------------
    pStr = buffered_sprintf_P( ( progmem ? PSTR("%20S") : PSTR("%20s")), text); // max. 20 Zeichen
    strrtrim( pStr );                                                           // alle Leerzeichen rechts loeschen
    lcdx_print_center( 4, (uint8_t *)pStr, MNORMAL, 0,5);                       // Text zentriert; String im RAM
    lcd_rect_round( 0, 32, 127, 16, 1, R2);                                     // Rahmen

    if( beep )
    {
        if( error )
            set_beep( 1000, 0xffff, BeepNormal );                       // langer Error-Beep
        else
            set_beep( 25, 0xffff, BeepNormal );                         // kurzer Bestaetigungs-Beep
    }

    if( timeout )
    {
        //lcd_printp_at( 19, 7, strGet(OK), MNORMAL);                   // Keyline... tja, welche Variante nehmen wir denn nun Final??? OK oder ENDE?
        lcd_printp_at(12, 7, strGet(ENDE), MNORMAL);                    // Keyline

        clear_key_all();

        timer2 = timeout;                                               // max. Anzeigezeit (z.B. 800 = 8 Sekunden)
        while( timer2 > 0 )
        {
            //if( get_key_short(1 << KEY_ENTER) )   break;
            if( get_key_short(1 << KEY_ESC) )       break;
            PKT_CtrlHook();
        }
    }
    clear_key_all();
}



//--------------------------------------------------------------
// PKT_Message( text, error, timeout, beep, clearscreen)
//
// zeigt eine Nachricht an ggf. mit Display-Timeout, Tasten, Beep (String im RAM)
//
// Parameter:
//  text       : String im RAM
//  error      : true/false
//  timeout    : 0 = direktes Verlassen der Funktion ohne warten
//               1..nnnn = max. Anzeigezeit (timer); Keyline wird
//                  eingeblendet -> Benutzer kann auch eher beenden
//  beep       : true/false - bei true haengt die Art des Beep's von 'error' ab
//  clearscreen: true/false - ggf. Bildschirm loeschen
//--------------------------------------------------------------
void PKT_Message( const char *text, uint8_t error, uint16_t timeout, uint8_t beep, uint8_t clearscreen )
{
    _pkt_message( false, text, error, timeout, beep, clearscreen );
}



//--------------------------------------------------------------
// PKT_Message_P( text, error, timeout, beep, clearscreen)
//
// zeigt eine Nachricht an ggf. mit Display-Timeout, Tasten, Beep (String im PROGMEM)
//
// Parameter:
//  text     : String im PROGMEM
//  error    : true/false
//  timeout  : 0 = direktes Verlassen der Funktion ohne warten
//             1..nnnn = max. Anzeigezeit (timer); Keyline wird
//                eingeblendet -> Benutzer kann auch eher beenden
//  beep     : true/false - bei true haengt die Art des Beep's von 'error' ab
//  clrscreen: true/false - ggf. Bildschirm loeschen
//--------------------------------------------------------------
void PKT_Message_P( const char *text, uint8_t error, uint16_t timeout, uint8_t beep, uint8_t clearscreen )
{
    _pkt_message( true, text, error, timeout, beep, clearscreen );
}



//--------------------------------------------------------------
// PKT_Message_Datenverlust( timeout, beep)
//--------------------------------------------------------------
void PKT_Message_Datenverlust( uint16_t timeout, uint8_t beep )
{
  //_pkt_message( true, text,                 error, timeout, beep, clearscreen );
    _pkt_message( true, strGet(ERROR_NODATA), true,  timeout, beep, true );
}



//#############################################################################################
//# PKT Popup
//#############################################################################################

//--------------------------------------------------------------
// INTERN
//
// PARAMETER:
//   progmem : true  = Texte in PROGMEM
//             false = Texte im RAM (RAM geht noch nicht!)
//   timeout : Zeit bis zum automatischen beenden (z.B. 400 = 4 Sekunden)
//             wenn timeout == 0 dann direkt beenden ohne auf Tastendruck zu warten
//   text1..4: Text (die Hoehe des Popups berechnet sich aus der Anzahl der Texte)
//             -> ein nicht benutzer Text wird mit NULL oder Dezimal 0 uebergeben
//--------------------------------------------------------------
void _pkt_popup( uint8_t progmem, uint16_t timeout, const char *text1, const char *text2, const char *text3, const char *text4)
{
    uint8_t n     = 0;
    uint8_t yoffs = 0;

    if( text1 ) { n = 1; yoffs = 1; }
    if( text2 ) { n = 2; yoffs = 2; }
    if( text3 ) { n = 3; yoffs = 2; }
    if( text4 ) { n = 4; yoffs = 3; }

    Popup_Draw(n+1);

    if( text1 ) lcdx_printp_center(  8-n, text1, MINVERS, 0,-5-yoffs);          // Text zentriert; String im PROGMEM
    if( text2 ) lcdx_printp_center(  9-n, text2, MINVERS, 0,-4-yoffs);          // Text zentriert; String im PROGMEM
    if( text3 ) lcdx_printp_center( 10-n, text3, MINVERS, 0,-3-yoffs);          // Text zentriert; String im PROGMEM
    if( text4 ) lcdx_printp_center( 11-n, text4, MINVERS, 0,-2-yoffs);          // Text zentriert; String im PROGMEM

    if( timeout )
    {
        clear_key_all();

        timer2 = timeout;                                                       // max. Anzeigezeit (z.B. 800 = 8 Sekunden)
        while( timer2 > 0 )
        {
            if(    get_key_short(1<<KEY_ESC)
                || get_key_short(1<<KEY_ENTER)
                || get_key_short(1<<KEY_PLUS)
                || get_key_short(1<<KEY_MINUS) )
            {
                break;
            }
        }

        lcd_frect( 0, 58-((n+1)*8), 127,  5+((n+1)*8), 0);                      // Box clear - der Bereich des Popup's wird wieder geloescht (nur wenn timeout > 0!)
    }
    clear_key_all();
}



//--------------------------------------------------------------
// PKT_Popup( timeout, text1, text2, text3, text4 )
//
// Texte im RAM
//--------------------------------------------------------------
void PKT_Popup( uint16_t timeout, const char *text1, const char *text2, const char *text3, const char *text4 )
{
    _pkt_popup( false, timeout, text1, text2, text3, text4 );
}



//--------------------------------------------------------------
// PKT_Popup_P( timeout, text1, text2, text3, text4 )
//
// Texte im PROGMEM
//--------------------------------------------------------------
void PKT_Popup_P( uint16_t timeout, const char *text1, const char *text2, const char *text3, const char *text4 )
{
    _pkt_popup( true, timeout, text1, text2, text3, text4 );
}



//#############################################################################################
//# PKT Ask
//#############################################################################################

//--------------------------------------------------------------
// wahl = PKT_AskX( asktype, text1, text2, text_progmem, headline, headline_progmem, title, title_progmem)
//
// PARAMETER:
//   asktype         : ASK_YES_NO, ASK_NO_YES, ASK_CANCEL_OK, ASK_END_OK, ASK_END_START
//   text1           : 1. Zeile Text
//   text2           : optionale 2. Zeile Text
//   text_progmem    : text1 & text2 in PROGMEM oder RAM  (PROGMEM = true)
//   headline        : Ueberschrift
//   headline_progmem: -> PROGMEM oder RAM
//   title           : ganz oben der Titel (Zeile 0, Invers)
//   title_progmem   : -> PROGMEM oder RAM
//
// RUECKGABE:
//   true  : Ja
//   false : Nein
//--------------------------------------------------------------
uint8_t PKT_AskX( uint8_t asktype, const char *text1, const char *text2, uint8_t text_progmem, const char *headline, uint8_t headline_progmem, const char *title, uint8_t title_progmem )
{
    uint8_t redraw   = true;
    uint8_t keyenter = false;
    uint8_t retcode  = false;
    const char *pMaskP;
    const char *pMaskR;
    const char *pMask;

    pMaskP = PSTR("%S");
    pMaskR = PSTR("%s");

    set_beep( 35, 0xffff, BeepNormal );                                     // kurzer Beep

    while( true )
    {
        //------------------------
        // Screen zeichnen
        //------------------------
        if( redraw )
        {
            if(      title == NULL )    PKT_TitlePKT();                     // Titel mit PKT-Version anzeigen (und clearscreen)
            else if( title_progmem )    PKT_Title_P( title, true, true );   // uebergebenen Titel anzeigen (und clearscreen) (PROGMEM)
            else                        PKT_Title(   title, true, true );   // uebergebenen Titel anzeigen (und clearscreen) (RAM)

            if( text2 == NULL )
            {
                // 1 zeiliger Text
                pMask = (headline_progmem ? pMaskP : pMaskR);               // Formatmaske fuer headline je nach PROGMEN oder RAM
                lcdx_printf_center_P( 2, MNORMAL, 0,2, pMask, headline);    // headline

                lcd_rect_round( 0, 3*7+2+10, 127, 16+0, 1, R2);             // Rahmen

                pMask = (text_progmem ? pMaskP : pMaskR);                   // Formatmaske fuer text1 und text2 je nach PROGMEN oder RAM
                lcdx_printf_center_P( 4, MNORMAL, 0,6, pMask, text1);       // text1
            }
            else
            {
                // 2 zeiliger Text
                pMask = (headline_progmem ? pMaskP : pMaskR);               // Formatmaske fuer headline je nach PROGMEN oder RAM
                lcdx_printf_center_P( 2, MNORMAL, 0,-1, pMask, headline);   // headline

                lcd_rect_round(0, 4*8-5, 127, 16+7, 1, R2);                 // Rahmen

                pMask = (text_progmem ? pMaskP : pMaskR);                   // Formatmaske fuer text1 und text2 je nach PROGMEN oder RAM
                lcdx_printf_center_P( 4, MNORMAL, 0,-1, pMask, text1);      // text1
                lcdx_printf_center_P( 5, MNORMAL, 0, 0, pMask, text2);      // text2
            }


            //------------------------
            // Keyline
            //------------------------
            switch( asktype )
            {
                case ASK_NO_YES:     lcd_printp_at(12, 7, strGet(NOYES), MNORMAL);               // Keyline: "Nein   Ja"
                                     break;

                case ASK_YES_NO:     lcd_printp_at(12, 7, strGet(YESNO), MNORMAL);               // Keyline: "Nein   Ja"
                                     break;

                case ASK_CANCEL_OK:  lcd_printp_at(11, 7, strGet(KEYCANCEL), MNORMAL);           // Keyline: "Abbr.   OK"
                                     lcd_printp_at(19, 7, PSTR("OK")       , MNORMAL);
                                     break;

                case ASK_END_OK:     lcd_printp_at(12, 7, strGet(KEYLINE4), MNORMAL);            // Keyline: "Ende   OK"
                                     break;

                case ASK_END_START:  lcd_printp_at(11, 7, strGet(ENDSTART), MNORMAL);            // Keyline: "Ende Start"
                                     break;
            }

            redraw = false;
        }

        //------------------------
        // LiPo Spannung zeigen
        //------------------------
        show_Lipo();

        //------------------------
        // ggf. auf PKT-Update reagieren
        //------------------------
        if( PKT_CtrlHook() )
        {
            redraw = true;
        }

        //------------------------
        // Tasten abfragen
        //------------------------
        if( get_key_short(1 << KEY_ESC) )       // 3. Taste
        {
            break;
        }

        if( get_key_press(1 << KEY_ENTER) )     // 4. Taste
        {
            keyenter = true;
            break;
        }
    } // end: while( true )


    //-------------------------
    retcode = keyenter;
    if( asktype == ASK_YES_NO ) retcode = !retcode;

    clear_key_all();

    return retcode;
}


//--------------------------------------------------------------
// wahl = PKT_Ask( asktype, text1, text2, headline, title)
//
// text1 und text2 im RAM
//--------------------------------------------------------------
uint8_t PKT_Ask( uint8_t asktype, const char *text1, const char *text2, const char *headline, const char *title )
{
  //wahl = PKT_AskX( asktype, text1, text2, text_progmem, headline, headline_progmem, title, title_progmem)
    return PKT_AskX( asktype, text1, text2, false, headline, true, title, true);
  //return _pkt_ask( false, asktype, text1, text2, headline, title);
}



//--------------------------------------------------------------
// wahl = PKT_Ask_P( asktype, text1, text2, headline, title)
//
// text1 und text2 im PROGMEM
//--------------------------------------------------------------
uint8_t PKT_Ask_P( uint8_t asktype, const char *text1, const char *text2, const char *headline, const char *title )
{
  //wahl = PKT_AskX( asktype, text1, text2, text_progmem, headline, headline_progmem, title, title_progmem)
    return PKT_AskX( asktype, text1, text2, true, headline, true, title, true);
  //return _pkt_ask( true, asktype, text1, text2, headline, title);
}


//#############################################################################################
//# PKT Gauge
//#############################################################################################

uint8_t Gauge_px;
uint8_t Gauge_py;
uint8_t Gauge_rx;
uint8_t Gauge_ry;
uint8_t Gauge_step;
volatile uint8_t Gauge_active;          // wird in timer.c abgefragt

#define GAUGE_DEFAULT_PX    64          // Default X (Pixel)
#define GAUGE_DEFAULT_PY    40          // Default Y (Pixel)
#define GAUGE_DEFAULT_RX    10          // Default Radius X Gauge-Kreis
#define GAUGE_DEFAULT_RY     8          // Default Radius Y Gauge-Kreis

#define GAUGE_TIMER         40          // alle n aktualisieren (Timer Intervall)
#define GAUGE_DEGREE        45          // Winkel von Schritt zu Schritt
#define GAUGE_LROFFS        -2          // die Laenge der Gauge-Linie verringern/vergroessern

//--------------------------------------------------------------
// PKT_Gauge_Begin( py )
//--------------------------------------------------------------
void PKT_Gauge_Begin( uint8_t py )
{
    Gauge_px = GAUGE_DEFAULT_PX;
    Gauge_py = GAUGE_DEFAULT_PY;
    Gauge_rx = GAUGE_DEFAULT_RX;
    Gauge_ry = GAUGE_DEFAULT_RY;

    if( py>0 ) Gauge_py = py;

    Gauge_step   = 0;
    timer_gauge  = GAUGE_TIMER;
    Gauge_active = true;

    lcd_ellipse_line( Gauge_px, Gauge_py, Gauge_rx+GAUGE_LROFFS, Gauge_ry+GAUGE_LROFFS, (0), 1);
    lcd_ellipse( Gauge_px, Gauge_py, Gauge_rx, Gauge_ry, 1);
}


//--------------------------------------------------------------
// PKT_Gauge_End()
//--------------------------------------------------------------
void PKT_Gauge_End( void )
{
    Gauge_active = false;
}


//  uint8_t old_step;

//--------------------------------------------------------------
// PKT_Gauge_Next()
//
// INTERN fuer Timer ISR!
//--------------------------------------------------------------
void PKT_Gauge_Next( void )
{
    uint8_t old_step;

    old_step = Gauge_step;
    Gauge_step++;
    if( Gauge_step >= (360/GAUGE_DEGREE))   Gauge_step = 0;

    lcd_ellipse_line( Gauge_px, Gauge_py, Gauge_rx+GAUGE_LROFFS, Gauge_ry+GAUGE_LROFFS, (old_step*GAUGE_DEGREE)  , 0);
    lcd_ellipse_line( Gauge_px, Gauge_py, Gauge_rx+GAUGE_LROFFS, Gauge_ry+GAUGE_LROFFS, (Gauge_step*GAUGE_DEGREE), 1);

    timer_gauge  = GAUGE_TIMER;
}



//#############################################################################################
//# PKT Kontrollfunktionen
//#############################################################################################

//--------------------------------------------------------------
//--------------------------------------------------------------
void PKT_Reset_EEprom( void )
{
    // Text1: "PKT-EEProm"        (EEPROM1)
    // Text2: "wirklich löschen?" (EEPROM2)
    // Head : "* ACHTUNG '"       (STR_ATTENTION)
    // Titel: PKT-Version         (NULL)

    if( PKT_Ask_P( ASK_YES_NO, strGet(EEPROM1), strGet(EEPROM2), strGet(STR_ATTENTION), NULL) )
    {
        Delete_EEPROM();
        clr_V_On();
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void PKT_SwitchOff( void )
{
    // Text1: "PKT ausschalten?"  (SHUTDOWN)
    // Text2: NULL
    // Head : "PKT"               (STR_PKT)
    // Titel: PKT-Version         (NULL)

    if( PKT_Ask_P( ASK_NO_YES, strGet(SHUTDOWN), NULL, strGet(STR_PKT), NULL) )
    {
        WriteParameter();   // am Ende alle Parameter sichern
        clr_V_On();         // Spannung abschalten
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void PKT_Ask_Restart( const char *title )
{
    // Text1: "jetzt ausschalten?"  (STR_PKT_SWITCHOFF_NOW)
    // Text2: NULL
    // Head : "PKT neu starten!"    (STR_PKT_RESTART_NOW)
    // Titel: via Parameter

    if( PKT_Ask_P( ASK_NO_YES, strGet(STR_PKT_SWITCHOFF_NOW), NULL, strGet(STR_PKT_RESTART_NOW), title) )
    {
        WriteParameter();       // am Ende alle Parameter sichern
        clr_V_On();             // Spannung abschalten
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void PKT_Update( void )
{
    // Text1: "Drücke 'Start' für"   (UPDATE2)
    // Text2: "min. 2 Sekunden"      (UPDATE3)
    // Head : "Verbinde PKT mit PC!" (UPDATE1)
    // Titel: "PKT Update"

    if( PKT_Ask_P( ASK_END_START, strGet(UPDATE2), strGet(UPDATE3), strGet(UPDATE1), PSTR("PKT Update")) )
    {
        uart1_putc('S');            // Startzeichen zum Updatetool schicken
        wdt_enable( WDTO_250MS );   // start bootloader with Reset, Hold KEY_ENTER
        while(true);
    }
}



//void PKT_Update( void )
//{
//    ShowTitle_P( PSTR("PKT Update"), true );
//    lcdx_printp_center( 2, strGet(UPDATE1), MNORMAL, 0,-1);      // "Verbinde PKT mit PC!"
//    lcd_rect_round(0, 4*8-5, 127, 16+7, 1, R2);                  // Rahmen
//    lcdx_printp_center( 4, strGet(UPDATE2), MNORMAL, 0,-1);      // "Drücke 'Start' für"
//    lcdx_printp_center( 5, strGet(UPDATE3), MNORMAL, 0, 0);      // "min. 2 Sekunden"
//    lcd_printp_at(11, 7, strGet(ENDSTART), MNORMAL);           // Keyline
//    //PKTupdate = false; // Flag löschen
//
//    set_beep( 80, 0x000f, BeepNormal);                           // kurzer Beep
//
//    do
//    {
//        if( get_key_press (1 << KEY_ESC) )
//        {
//            //get_key_press(KEY_ALL);
//            clear_key_all();
//            return;
//        }
//    }
//    while( !(get_key_press (1 << KEY_ENTER)) );
//
//    uart1_putc('S');          // Startzeichen zum Updatetool schicken
//    wdt_enable( WDTO_250MS ); // start bootloader with Reset, Hold KEY_ENTER
//    while(true);
//}



//--------------------------------------------------------------
// prueft im Menue ob das Updatetool ein Update machen will
//--------------------------------------------------------------
uint8_t PKT_CheckUpdate(void)
{
    unsigned int uart_data;
    uint8_t state         = 0;
    uint8_t update        = false;
    uint8_t error         = false;
    uint8_t SendVersion   = false;
    uint8_t UsbDebug      = false;
    char s[7];


    //----------------------------
    // empfangen
    //----------------------------
    if( uart1_available() > 0 )             // Zeichen von der USB-Schnittstelle vorhanden?
    {
        timer_pktupdatecheck = 100;         // Timeouttimer um die Funktion abzubrechen

        while( !timer_pktupdatecheck==0 && !update && !error && !SendVersion && !UsbDebug)
        {
            uart_data = uart1_getc();
            if( !((uart_data & 0xFF00) == UART_NO_DATA) )
            {
                switch (state)
                {
                    case  0:    if( uart_data == PKTESC )
                                {
                                    state = 1;
                                }
                                else error = true;
                                break;

                    case  1:    if( uart_data == PKTESC )
                                {
                                    state = 2;
                                }
                                else error = true;
                                break;

                    case  2:    if(      uart_data == PKTUPDATE ) {update = true;}
                                else if (uart_data == PKTDEBUG) {UsbDebug = true;}
                                else if (uart_data == PKTVERSION) {SendVersion = true;}
                                else error = true;
                                break;
                } // end: switch
            } // end: if
        } // end: while


        //----------------------------
        // Rueckmeldungen
        //----------------------------
        if( error )
        {
            uart1_putc('E');                            // Rueckmeldungen zum Updatetool
        }
        else if( update )
        {
            uart1_putc('U');
        }
        else if( SendVersion )
        {
            uart1_putc('V');
            itoa( EEpromVersion, s, 10 );
            uart1_puts(s);
         }
        else if( UsbDebug )
        {
            // Hier die Debugfunktionen und Ausgaben mit einbinden
           PKT_Debugfunktion();

        }
        else if( timer_pktupdatecheck==0 )
        {
            uart1_putc('T');
        }



         //uart1_flush();
    }

    return update;
}



//--------------------------------------------------------------
// retcode = PKT_CtrlHook()
//
// ruft ggf. PKT_Update() auf und kann spaeter weitere PKT interne
// Sachen verarbeiten wenn der Hook (=Nagel) in entsprechende
// Routienen eingebaut ist (z.B. senden von Daten an den PC)
//
// Aktuell verwendet von: menuctrl, scrollbox, osd
//
// Rueckgabe:
//   retcode: 0 = nichts wurde gemacht (false)
//            1 = es wurde etwas durchgefuehrt und die Aufruffunktion
//                muss gff. einen Screenrefresh durchfuehren
//                (z.B. wenn der PKT-Update Screen gezeigt wurde)
//--------------------------------------------------------------
uint8_t PKT_CtrlHook( void )
{
    uint8_t retcode = 0;

    // 31.05.2014 OG: PKT_CheckUpdate() darf nicht in 'Hoechstgeschwindigkeit'
    // aufgerufen werden da es sonst seitens des PKT-Updatetools ggf. zu einer
    // Fehlermeldung kommt - deswegen hier timer_pktupdatecheck der PKT_CheckUpdate
    // etwas einbremst sich aber nicht negativ auf die PKT-Performance auswirkt.
    if( !timer_pktupdatecheck )
    {
        if( PKT_CheckUpdate() )                  // Update vom Updatetool angefordert?
        {
            WriteParameter();
            PKT_Update();
            retcode = 1;
        }
        timer_pktupdatecheck = 10;
    }

    return retcode;
}



//--------------------------------------------------------------
// zeigt PKT-Version und Credits / Entwickler
//--------------------------------------------------------------
void PKT_Info( void )
{
    char *HWV_39m = "3.9m";
    char *HWV_39x = "3.9x";
    char *HWV     = "????";

    if( PCB_Version == PKT39m )    HWV = HWV_39m;
    if( PCB_Version == PKT39x )    HWV = HWV_39x;

    lcd_cls();

    if( !ScrollBox_Create(55) )     // max. 55 Zeilen
        return;                     //   kein RAM mehr

    ScrollBox_Push_P( MINVERS, PSTR(" PKT v%8S %s"), PSTR(PKTSWVersion), HWV );     // "3.6.8a..." und die PKT-Hardwareversion
    ScrollBox_Push_P( MINVERS, PSTR(" %S SV2-Patch"), (PCB_SV2RxTxPatch ? strGet(STR_WITH) : strGet(STR_WITHOUT)) );
    ScrollBox_Push_P( MNORMAL, PSTR("") );
    ScrollBox_Push_P( MNORMAL, PSTR("(C) GNU GPL License") );
    ScrollBox_Push_P( MNORMAL, PSTR("    NO WARRANTY") );
    ScrollBox_Push_P( MNORMAL, PSTR("") );


#ifdef USE_MODULEINFO

    const char *fmt = PSTR("%15S %4S");

    //--- MODULE SUPPORT INFO ---
    ScrollBox_Push_P( MINVERS, PSTR(" Modules installed") );

    #ifdef USE_BLUETOOTH
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Bluetooth"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Bluetooth"), strGet(NOO) );
    #endif


    #ifdef USE_WLAN
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("WLAN WyFly"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("WLAN WyFly"), strGet(NOO) );
    #endif


    #ifdef USE_MKSETTINGS
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("MK Settings"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("MK Settings"), strGet(NOO) );
    #endif


    #ifdef USE_MKDEBUGDATA
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("MK Debug Data"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("MK Debug Data"), strGet(NOO) );
    #endif


    #ifdef USE_MKDISPLAY
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("MK Display"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("MK Display"), strGet(NOO) );
    #endif


    #ifdef USE_OSDDATA
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD-Data"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD-Data"), strGet(NOO) );
    #endif


    #ifdef USE_OSD_SCREEN_OLD
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD Old Screens"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD Old Screens"), strGet(NOO) );
    #endif


    #ifdef USE_OSD_SCREEN_NAVIGATION
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD Navigation"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD Navigation"), strGet(NOO) );
    #endif


    #ifdef USE_OSD_SCREEN_WAYPOINTS
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD Waypoints"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD Waypoints"), strGet(NOO) );
    #endif


    #ifdef USE_OSD_SCREEN_ELECTRIC
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD Electric"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD Electric"), strGet(NOO) );
    #endif


    #ifdef USE_OSD_SCREEN_MKSTATUS
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD MKStatus"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD MKStatus"), strGet(NOO) );
    #endif


    #ifdef USE_OSD_SCREEN_USERGPS
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD UserGPS"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD UserGPS"), strGet(NOO) );
    #endif


    #ifdef USE_OSD_SCREEN_3DLAGE
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD 3D-Lage"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD 3D-Lage"), strGet(NOO) );
    #endif


    #ifdef USE_OSD_SCREEN_STATISTIC
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD Statistic"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("OSD Statistic"), strGet(NOO) );
    #endif


    #ifdef USE_TRACKING
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Tracking"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Tracking"), strGet(NOO) );
    #endif


    #ifdef USE_FOLLOWME
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Follow Me"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Follow Me"), strGet(NOO) );
    #endif

    #ifdef USE_FOLLOWME_STEP2
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Follow Me Step2"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Follow Me Step2"), strGet(NOO) );
    #endif


//   ist für mögliche Erweiterung um IMU vorgesehen
//    #ifdef USE_KOMPASS
//        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Compass"), strGet(YES) );
//    #else
//        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Compass"), strGet(NOO) );
//    #endif
//
//    #ifdef USE_PRESSURE
//        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Air Pressure"), strGet(YES) );
//    #else
//        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Air Pressure"), strGet(NOO) );
//    #endif




    #ifdef USE_JOYSTICK
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Joystick"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Joystick"), strGet(NOO) );
    #endif


    #ifdef USE_I2CMOTORTEST
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("I2C Motortest"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("I2C Motortest"), strGet(NOO) );
    #endif


    #ifdef USE_SOUND
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Sound"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Sound"), strGet(NOO) );
    #endif


    #ifdef USE_FONT_BIG
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Font Big"), strGet(YES) );
    #else
        ScrollBox_Push_P( MNORMAL, fmt, PSTR("Font Big"), strGet(NOO) );
    #endif

    ScrollBox_Push_P( MNORMAL, PSTR("") );

#endif // USE_MODULEINFO


    //--- CREDITS INFO ---
    ScrollBox_Push_P( MINVERS, PSTR(" Credits") );
    ScrollBox_Push_P( MNORMAL, PSTR("2013 Oliver Gemesi") );
    ScrollBox_Push_P( MNORMAL, PSTR("2012 Chr. Brandtner" ) );
    ScrollBox_Push_P( MNORMAL, PSTR("     Harald Bongartz") );
    ScrollBox_Push_P( MNORMAL, PSTR("2012 Martin Runkel") );
    ScrollBox_Push_P( MNORMAL, PSTR("2012 gebad") );
    ScrollBox_Push_P( MNORMAL, PSTR("2010 Sebastian Boehm") );
    ScrollBox_Push_P( MNORMAL, PSTR("2009-2010 Peter Mack") );
    ScrollBox_Push_P( MNORMAL, PSTR("2008 Thomas Kaiser") );

    ScrollBox_Show();
    ScrollBox_Destroy();            // free memory
}


