/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
 *   Copyright (C) 2013 Oliver Gemesi                                        *
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
 *****************************************************************************/

//############################################################################
//# HISTORY  menuctrl.c
//#
//# 11.06.2014 OG
//# - add: MenuCtrl_GetMenuIndex()
//#
//# 31.05.2014 OG
//# - chg: MenuCtrl_Refresh() - umgestellt auf PKT_KeylineUpDown()
//#
//# 14.05.2014 OG
//# - chg: include "../mk/paramset.h" geaendert auf "../mksettings/paramset.h"
//# - chg: include "../mk/mkparameters.h" geaendert auf "../mksettings/mkparameters.h"
//#
//# 11.05.2014 OG
//# - add: MenuCtrl_SetTitleFromParentItem()
//# - add: MenuCtrl_GetItemText(), MenuCtrl_IsItemTextPGM()
//# - chg: _MenuCtrl_Error() - auf PKT_Popup_P() umgestellt, Layout geaendert und
//#        einen neuen Fehler (NOPARENTMENU) hinzugefuegt
//#
//# 09.05.2014 OG
//# - fix: MenuCtrl_Refresh() Anzeige von deaktivierten MK-Parametern (Favoriten)
//#        Wenn MK-Paramketer-Menueeintraege deaktiviert sind werden sie vom
//#        aktuellen MK-Parameterset nicht unterstuetzt - dennoch wurde der
//#        aktuelle Wert des MK-Parameters im Menuepunkt (ganz rechts) angezeigt.
//#        Das ist jetzt behoben - es wird kein Wert angezeigt (stattdessen "***")
//#        und auch keine entsprechende editParam-Funktionen aufgerufen.
//#
//# 07.05.2014 OG
//# - add: MenuCtrl_PushSeparatorID()
//#
//# 06.05.2014 OG
//# - chg: MenuCtrl_Refresh() Keyline leicht angepasst
//# - chg: 'not possible' Anzeige (bei deaktiviertem Item) umgestellt auf
//#        PKT_Popup_P()
//# - add: MenuCtrl_GetItemIdByIndex(), MenuCtrl_GetItemCount()
//#
//# 05.05.2014 OG
//# - chg: MenuCtrl_Control()
//#        -> Unterstuetzung fuer: MenuCtrl_SetMove() - Menueeintraege verschieben
//# - chg: MenuCtrl_Refresh() - Anpassung Slider fuer MenuCtrl_SetDelete()
//# - add: MenuCtrl_SetDelete(), MenuCtrl_SetMove()
//# - add: _MenuCtrl_SwapItem()
//# - add: die Datenstruktur vom Menue wurde erweitert um die Eigenschaften
//#        canMove und canDelete
//#
//# 17.04.2014 OG
//# - chg: MENUCTRL_MAXITEMS von 22 auf 28 erhoeht
//#
//# 30.03.2014 OG
//# - del: MenuCtrl_PushML_P(), MenuCtrl_PushML() (Umstellung Sprachen abgeschlossen)
//# - add: MenuCtrl_PushML2(), MenuCtrl_PushML2_P() fuer Umstellung von 3 auf 2 Sprachen
//#
//# 29.03.2014 OG
//# - chg: default MENUDEFAULT_SHOWBATT auf true umgestellt (=PKT-Batterie anzeigen)
//# - chg: MenuCtrl_Control() umgestellt auf clear_key_all()
//#
//# 24.03.2014 OG
//# - add: MenuCtrl_PushSeparator() - fuegt eine Trennlinie im Menue ein
//# - chg: MenuCtrl_Refresh() Codeoptimierung bzgl. topspace/Zeile loeschen/Slider
//# - fix: MenuCtrl_Control() es wurde zuoft ein Screen-Refresh durchgefuehrt
//#
//# 23.03.2014 OG
//# - add: Unterstuetzung fuer MK-Parameter-Edit (mkparameters.c)
//# - chg: MenuCtrl_Refresh() Unterstuetzung fuer MENU_PARAMEDIT
//# - add: MenuCtrl_PushParamEdit()
//# - add: paramset.h und mkparameters.h
//#
//# 04.03.2014 OG
//# - fix: MenuCtrl_Refresh() bei einem blauen Display (das ist wohl schneller
//#        in der Anzeige als das S/W-Display) flackerte der Menue-Slider
//#
//# 17.02.2014 OG
//# - add: MenuCtrl_SetTopSpace()
//# - chg: MenuCtrl_Refresh() angepasst auf MenuCtrl_SetTopSpace()
//#
//# 15.02.2014 OG
//# - add: MenuCtrl_ItemSelect()
//#
//# 01.02.2014 OG
//# - chg: MENUCTRL_MAXITEMS von 16 auf 22 fuer MK_parameters
//# - fix: _MenuCtrl_Error() Anzeigezeit von Menu-Errors auf 8 Sekunden verlaengert
//#        und Anzeige von "menuctrl.c"
//#
//# 07.07.2013 OG
//# - add: MenuCtrl_ItemMarkR() - Markiert einen Menueeintrag mit einem Haken am ENDE (Rechts)
//#
//# 24.05.2013 OG
//# - chg: MenuCtrl_Push... Funktionen vereinheitlicht mit internen Aenderungen
//#        betrifft externe Funktionsaufrufe die geändert wurden
//# - chg: MenuCtrl_Refresh() Anpassungen und Optimierung
//# - add: MenuCtrl_ItemMark()
//# - del: MenuCtrl_PushSimple_P(), MenuCtrl_PushSimple()
//#
//# 23.05.2013 OG
//# - add: MenuCtrl_PushSimple_P(), MenuCtrl_PushSimple()
//# - fix: MenuCtrl_Control() Eventcode Rueckgabe bei KEY_ENTER
//#
//# 22.05.2013 OG
//# - fix: nach Aufruf einer Menuefunktion in MenuCtrl_Control() jetzt
//#        get_key_press(KEY_ALL)
//#
//# 21.05.2013 OG
//# - add: MenuCtrl_ShowLevel()
//#
//# 20.05.2013 OG
//# - chg: MenuCtrl_Control() wieder mit get_key_press(KEY_ALL) ergaenzt
//#        wenn aufgerufen mit MENUCTRL_EVENT oder erster Aufruf mit RETURN
//#
//# 20.05.2013 OG
//# - chg: Space am Anfang der Titelanzeige
//#
//# 19.05.2013 OG
//# - add: define MENU_SHOWLEVEL mit dem der aktuelle Menuelevel in der
//#        Titelzeile angezeigt werden kann
//# - fix: Redraw-Logik bei MENUCTRL_RETURN
//# - chg: MenuCtrl_Control() erweitert um PKT_Update()
//#
//# 18.05.2013 OG - NEU
//############################################################################

#include "../cpu.h"
#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <string.h>         // memset
#include <stdarg.h>

#include "../main.h"
#include "../lcd/lcd.h"
#include "../eeprom/eeprom.h"
#include "../messages.h"
#include "../lipo/lipo.h"
#include "../menu.h"
#include "../pkt/pkt.h"
#include "../mksettings/paramset.h"
#include "../mksettings/mkparameters.h"
#include "menuctrl.h"


//#############################################################################################
//# internal defines & structs
//#############################################################################################

#define MENU_SHOWLEVEL          false   // Anzeige der Menue-Verschachtelungstiefe in der Titelzeile
                                        // zeigt dem Benutzer wie weit er wieder zurueckspringen muss
                                        // um in das Hauptmenue zu kommen

#define MENUDEFAULT_CYCLE       false   // cycle: wenn Menue am Ende dann wieder zum Anfang
#define MENUDEFAULT_SHOWBATT    true    // showbatt: zeigt PKT-Batterie im Menuetitel ganz rechts
#define MENUDEFAULT_BEEP        true    // beep: wenn Cursor ueber Ende/Anfang


#define MENUCTRL_MAXMENUES  6           // Anzahl max. verschachlteter Menues
//#define MENUCTRL_MAXITEMS   22          // Anzahl max. Menu-Items pro Menue (verbraucht Speicher)
//#define MENUCTRL_MAXITEMS   28          // Anzahl max. Menu-Items pro Menue (verbraucht Speicher)
#define MENUCTRL_MAXITEMS   34          // Anzahl max. Menu-Items pro Menue (verbraucht Speicher)
#define ERROR_NOMENU        1           // Errorcode
#define ERROR_MAXMENU       2           // Errorcode
#define ERROR_MAXITEM       3           // Errorcode
#define ERROR_NOITEM        4           // Errorcode
#define ERROR_NOPARENTMENU  5           // Errorcode

//-----------------------------------------------------------
// typedef: scrollbox_key_t
//-----------------------------------------------------------
typedef struct
{
    uint8_t     active;             // Taste aktiv? (true/false)
    const char  *text;              // Tastentext
    void (*func)(void);             // ggf. Funktions-Pointer (0=keine Funktion)
}  menue_key_t;


//---------------------------
// typedef: einzelnes Menueitem
//---------------------------
typedef struct
{
    uint8_t     id;
    uint8_t     type;               // MENU_ITEM, MENU_SUB, MENU_PARAMEDIT
    uint8_t     disabled;           // true / false  (deaktiviert einen Menuepunkt)
    uint8_t     mark;               // true / false  (markiert einen Menuepunkt Links '*')
    uint8_t     markR;              // true / false  (markiert einen Menuepunkt Rechts)
    uint8_t     textPGM;            // true / false (true = text in PROGMEN; false = text im RAM)
    uint8_t     textcount;          // 1 .. NUM_LANG
    const char *text[NUM_LANG];     // de, en, nl  (NUM_LANG aus messages.h)
    void (*func)(void);
}  menue_item_t;


//---------------------------
// typedef: Menueitem Liste
//---------------------------
typedef struct
{
    uint8_t         active;         // aktives item
    uint8_t         itemcount;      // Anzahl Items
    uint8_t         scroll_pos;     // aktuelle obere Anzeigezeile
    uint8_t         display_pos;    // aktuelle obere Anzeigezeile
    uint8_t         topspace;       // obere Leerzeilen - default=0 (Vorsicht! ggf. nur bedingt einsetzbar bei Menues die ohne scrollen auskommen!)
    uint8_t         firstredraw;    // true / false  (flag fuer ein erstes redraw bei MENUCTRL_RETURN)
    uint8_t         cycle;          // true / false
    uint8_t         showbatt;       // true / false
    uint8_t         beep;           // true / false
    uint8_t         canMove;        // true / false
    uint8_t         canDelete;      // false/0 = kein entfernen; true/1 = entfernen mit Nachfrage; 2 = entfernen ohne Nachfrage
    uint8_t         titlePGM;       // true / false
    const char      *title;         // Menuetitel (oberste Zeile) (ggf. Multisprache machen?)
    uint8_t         lastkey;        // verwendet von GetKey()
    menue_key_t     key_enter;
    menue_key_t     key_enter_long;
    menue_key_t     key_esc;
    menue_key_t     key_esc_long;
    menue_item_t    item[MENUCTRL_MAXITEMS];
}  menue_t;

uint8_t showlevel = MENU_SHOWLEVEL; // Anzeige Menulevel in der Titelanzeige
int8_t  midx = -1;                  // aktives Menue; -1 = kein Menue
menue_t menu[MENUCTRL_MAXMENUES];   // Stack fuer n geschachltete Menues



//#############################################################################################
//# PRIVAT funcs
//#############################################################################################


//--------------------------------------------------------------
// INTERN
//--------------------------------------------------------------
void _MenuCtrl_Error( uint8_t error )
{
    const char *pStr = NULL;

    switch( error )
    {
        case ERROR_NOMENU:          pStr = PSTR("NOMENU");       break;
        case ERROR_MAXMENU:         pStr = PSTR("MAXMENU");      break;
        case ERROR_MAXITEM:         pStr = PSTR("MAXITEM");      break;
        case ERROR_NOITEM:          pStr = PSTR("NOITEM");       break;
        case ERROR_NOPARENTMENU:    pStr = PSTR("NOPARENTMENU"); break;
    }

    set_beep( 1000, 0x000f, BeepNormal);                                    // Beep Error
    PKT_Popup_P( 10000, PSTR("menuctrl.c"), 0, PSTR("* ERROR *"), pStr );   // 10000 = max. 100 Sekunden anzeigen
}


//--------------------------------------------------------------
// INTERN
//--------------------------------------------------------------
int8_t _MenuCtrl_FindItem( uint8_t itemid )
{
    int8_t i;

    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return -1; }

    for( i=0; i<menu[midx].itemcount; i++)
    {
        if( itemid == menu[midx].item[i].id )
        {
            return i;   // found: return index
        }
    }
    return -1;          // not found
}


//--------------------------------------------------------------
// INTERN
//--------------------------------------------------------------
void _MenuCtrl_CalcDisplayPos( void )
{
    int8_t i;

    i = menu[midx].active - menu[midx].display_pos;

    if( i < 0 )
        menu[midx].display_pos = menu[midx].active;
    else if( i > 5 )
        menu[midx].display_pos = (menu[midx].active - 5);
}


//--------------------------------------------------------------
// INTERN
//--------------------------------------------------------------
void _MenuCtrl_Beep( void )
{
    if( menu[midx].beep )
        set_beep( 25, 0xffff, BeepNormal );
}


//--------------------------------------------------------------
// INTERN
// Dreieckstausch von zwei Menuepunkten.
// Wird verwendet um Menueitems zu verschieben.
//--------------------------------------------------------------
void _MenuCtrl_SwapItem( uint8_t itemindex_1, uint8_t itemindex_2 )
{
    menue_item_t tmpItem;

    memcpy( &tmpItem                      , &menu[midx].item[itemindex_1], sizeof(menue_item_t) );
    memcpy( &menu[midx].item[itemindex_1] , &menu[midx].item[itemindex_2], sizeof(menue_item_t) );
    memcpy( &menu[midx].item[itemindex_2] , &tmpItem, sizeof(menue_item_t) );
}



//#############################################################################################
//# PUBLIC funcs
//#############################################################################################


//--------------------------------------------------------------
// MenuCtrl_Create()
//--------------------------------------------------------------
void MenuCtrl_Create( void )
{
    if( midx >= MENUCTRL_MAXMENUES) { _MenuCtrl_Error( ERROR_MAXMENU ); return; }

    midx++;
    memset( &menu[midx], 0, sizeof(menue_t) );
    menu[midx].cycle        = MENUDEFAULT_CYCLE;
    menu[midx].showbatt     = MENUDEFAULT_SHOWBATT;
    menu[midx].beep         = MENUDEFAULT_BEEP;
}


//--------------------------------------------------------------
// MenuCtrl_Destroy()
//--------------------------------------------------------------
void MenuCtrl_Destroy( void )
{
    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return; }

    midx--;
}


//--------------------------------------------------------------
// MenuCtrl_ShowLevel( value )
//
// Diese Einstellung ist Global und wirkt sich direkt auf alle
// Menues aus!
// Man kann das also nicht fuer einzelne Menues ein-/ausschalten.
//
// Parameter:
//   value: true / false
//--------------------------------------------------------------
void MenuCtrl_ShowLevel( uint8_t value )
{
    showlevel = value;
}


//--------------------------------------------------------------
// MenuCtrl_SetTitle_P( *title )
//
// Parameter:
//   title: Menuetitel; Zeiger auf String PROGMEM
//--------------------------------------------------------------
void MenuCtrl_SetTitle_P( const char *title )
{
    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return; }

    menu[midx].title    = title;
    menu[midx].titlePGM = true;
}


//--------------------------------------------------------------
// MenuCtrl_SetTitle( *title )
//
// Parameter:
//   title: Menuetitel; Zeiger auf String im RAM
//--------------------------------------------------------------
void MenuCtrl_SetTitle( const char *title )
{
    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return; }

    menu[midx].title    = title;
    menu[midx].titlePGM = false;
}



//--------------------------------------------------------------
// MenuCtrl_SetTitleFromParentItem()
//
// setzte anhand des uebergeordneten Menuepunktes (dem aufrufenden
// Menuepunkt) den Titel des neuen Menues
//
// wird u.a. verwendet in setup.c
//--------------------------------------------------------------
void MenuCtrl_SetTitleFromParentItem( void )
{
    uint8_t     lang;
    const char *pStr;
    uint8_t     mparent;

    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU );       return; }
    if( midx < 1)   { _MenuCtrl_Error( ERROR_NOPARENTMENU ); return; }

    mparent = midx-1;

    //--------------------------
    // Sprache: Multilanguage oder 0 wenn wenn nur ein Text
    //--------------------------
    lang = (menu[mparent].item[ menu[mparent].active ].textcount == 1) ? 0 : Config.DisplayLanguage;

    //--------------------------
    // Text vom uebergeordneten Menuepunkt
    //--------------------------
    pStr = menu[mparent].item[ menu[mparent].active ].text[lang];

    //--------------------------
    // Titel setzen
    //--------------------------
    if( menu[mparent].item[ menu[mparent].active ].textPGM )
    {
        MenuCtrl_SetTitle_P( pStr );
    }
    else
    {
        MenuCtrl_SetTitle( pStr );
    }
}



//--------------------------------------------------------------
// MenuCtrl_SetCycle( value )
//
// Parameter:
//   value: true / false
//--------------------------------------------------------------
void MenuCtrl_SetCycle( uint8_t value )
{
    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return; }

    menu[midx].cycle    = value;
}



//--------------------------------------------------------------
// MenuCtrl_SetBeep( value )
//
// Parameter:
//   value: true / false
//--------------------------------------------------------------
void MenuCtrl_SetBeep( uint8_t value )
{
    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return; }

    menu[midx].beep = value;
}



//--------------------------------------------------------------
// MenuCtrl_SetMove( value )
//
// Schaltet in einem Menue die Moeglichkeit ein Menueeintraege nach
// oben oder nach unten zu verschieben.
// Zum verschieden eines Menueeintrages muessen die PLUS/MINUS Tasten
// lange gedrueckt werden.
//
// Parameter:
//   value: true / false (Default: false)
//--------------------------------------------------------------
void MenuCtrl_SetMove( uint8_t value )
{
    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return; }

    menu[midx].canMove = value;
}


//--------------------------------------------------------------
// MenuCtrl_SetDelete( value )
//
// Schaltet in einem Menue die Moeglichkeit ein Menueeintraege zu loeschen.
// Zum loeschen eines Menueeintrages muss dann die ganz rechte 'OK' Taste
// lange gedrueckt werden.
//
// Parameter:
//   value: true / false (Default: false)
//--------------------------------------------------------------
void MenuCtrl_SetDelete( uint8_t value )
{
    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return; }

    menu[midx].canDelete = value;
}


//--------------------------------------------------------------
// MenuCtrl_SetShowBatt( value )
//
// Parameter:
//   value: true / false
//--------------------------------------------------------------
void MenuCtrl_SetShowBatt( uint8_t value )
{
    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return; }

    menu[midx].showbatt = value;
}



//--------------------------------------------------------------
// MenuCtrl_SetTopSpace( n )
//
// fuegt oben im Menue n Leerzeichen ein um Abstand vom Menuetitel
// zu erhalten
//
// ACHTUNG! Das funktioniert nur mit Menues die nicht Scrollen!
// -> also weniger als 6 Eintraege haben (je nach Anzahl von TopSpace)
//
// ACHTUNG! Keine Absicherung bzgl. obigem Warnpunkt! Das kann man
// zwar machen - ist aber aktuell nicht so!
//
// Parameter:
//   value: 0..n  Anzahl der Leerzeilen ueber dem Menue
//--------------------------------------------------------------
void MenuCtrl_SetTopSpace( uint8_t n )
{
    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return; }

    menu[midx].topspace = n;
}



//--------------------------------------------------------------
// MenuCtrl_SetKey( key, *keytext, *keyfunc )
//
// Parameter:
//   key    : KEY_ENTER, KEY_ENTER_LONG, KEY_ESC, KEY_ESC_LONG
//   keytext: Pointer auf Text PROGMEM oder NOTEXT (=0)
//   keyfunc: Pointer auf Funktion oder NOFUNC (=0)
//--------------------------------------------------------------
void MenuCtrl_SetKey( uint8_t key, const char *keytext, void (*keyfunc)(void) )
{
    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return; }        // kein aktives Menue

    if( key == KEY_ENTER )
    {
        menu[midx].key_enter.active     = true;
        menu[midx].key_enter.text       = keytext;
        menu[midx].key_enter.func       = keyfunc;
    }

    if( key == KEY_ENTER_LONG )
    {
        menu[midx].key_enter_long.active = true;
        menu[midx].key_enter_long.text  = keytext;
        menu[midx].key_enter_long.func  = keyfunc;
    }

    if( key == KEY_ESC )
    {
        menu[midx].key_esc.active       = true;
        menu[midx].key_esc.text         = keytext;
        menu[midx].key_esc.func         = keyfunc;
    }

    if( key == KEY_ESC_LONG )
    {
        menu[midx].key_esc_long.active  = true;
        menu[midx].key_esc_long.text    = keytext;
        menu[midx].key_esc_long.func    = keyfunc;
    }
}


//--------------------------------------------------------------
// key = MenuCtrl_GetKey()
//
// Rueckgabe:
//   key: z.B. KEY_ENTER oder KEY_ENTER_LONG
//--------------------------------------------------------------
uint8_t MenuCtrl_GetKey( void )
{
    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return 0; }      // kein aktives Menue

    return menu[midx].lastkey;
}



//--------------------------------------------------------------
// menuindex = MenuCtrl_GetMenuIndex()
//
// gibt den aktuellen Menuindex zurueck
//
// Rueckgabe:
//   menuindex: <0 = keine Menue (-1)
//              =0 = Hauptmenue (Toplevel)
//              >0 = Untermenue Level
//--------------------------------------------------------------
int8_t MenuCtrl_GetMenuIndex( void )
{
    return midx;
}



//--------------------------------------------------------------
// itemid = MenuCtrl_GetItemId()
//
// gibt die itemID des aktuell angewaehlten Menuepunktes zurueck
//
// Rueckgabe:
//   itemid:
//--------------------------------------------------------------
uint8_t MenuCtrl_GetItemId( void )
{
    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return 0; }      // kein aktives Menue

    return menu[midx].item[ menu[midx].active ].id;
}



//--------------------------------------------------------------
// itemid = MenuCtrl_GetItemIdByIndex( index )
//
// damit koennen die aktuell vorhanden itemID's der Menuepunkte
// ermittelt wenn im Menue via SetMove oder SetDelete Menuepunkte
// durch den Benutzer verschoben oder geloescht wurden.
//
// Parameter:
//   index: von 0..itemcount-1
//--------------------------------------------------------------
uint8_t MenuCtrl_GetItemIdByIndex( uint8_t index )
{
    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return 0; }      // kein aktives Menue

    if( (menu[midx].itemcount == 0) || (index > menu[midx].itemcount-1) )   return 0;   // index ausserhalb aktiver Menuepunkte

    return menu[midx].item[ index ].id;
}



//--------------------------------------------------------------
// num = MenuCtrl_GetItemCount()
//
// gibt die Anzahl der Menuepunkte zurueck
//
// Rueckgabe:
//   num: Anzahl der Menuepunkte
//--------------------------------------------------------------
uint8_t MenuCtrl_GetItemCount( void )
{
    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return 0; }      // kein aktives Menue

    return menu[midx].itemcount;
}


//--------------------------------------------------------------
// textPtr = MenuCtrl_GetItemText()
//
// gibt einen Pointer auf den Text des aktuellen Menuepunktes zurueck
//
// ACHTUNG! Der Pointer kann auf RAM oder auch auf PGM zeigen!
//
// Um herauszufinden auf welchen Bereich der Pointer zeigt kann
// MenuCtrl_IsItemTextPGM() verwendet werden.
//
// Rueckgabe:
//   textPtr:  in RAM oder PGM
//--------------------------------------------------------------
const char * MenuCtrl_GetItemText( void )
{
    uint8_t lang;

    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return 0; }      // kein aktives Menue

    //--------------------------
    // Sprache: Multilanguage oder 0 wenn wenn nur ein Text
    //--------------------------
    lang = (menu[midx].item[ menu[midx].active ].textcount == 1) ? 0 : Config.DisplayLanguage;

    return menu[midx].item[ menu[midx].active ].text[lang];
}



//--------------------------------------------------------------
// isPGM = MenuCtrl_IsItemTextPGM()
//
// gibt true/false zurueck je nachdem der Text von MenuCtrl_GetItemText()
// im Progmem (=true) oder im RAM (=false) ist
//
// Rueckgabe:
//   isPGM: true / false (true = text in PROGMEN; false = text im RAM)
//--------------------------------------------------------------
uint8_t MenuCtrl_IsItemTextPGM( void )
{
    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return 0; }      // kein aktives Menue

    return menu[midx].item[ menu[midx].active ].textPGM;
}



//--------------------------------------------------------------
// MenuCtrl_ItemSelect( itemid )
//
// aktiviert einen Menuepunkt / setzt den Menue-Cursor darauf
//
// die Funktion ist nicht durchgetestet, funktioniert aber
// in MKSettings_Menu() / mkparameters.c
//
// es kann sein das es zu Problemen kommt wenn das Menue
// scrollt (mehr als 6 Menueeintraege)
//
// Parameter:
//   itemid: ID des Menuitems
//--------------------------------------------------------------
void MenuCtrl_ItemSelect( uint8_t itemid )
{
    int8_t i;

    i = _MenuCtrl_FindItem( itemid );

    if( i >= 0 )
    {
        menu[midx].active = i;
    }
}


//--------------------------------------------------------------
// MenuCtrl_ItemActive( itemid, value )
//
// Parameter:
//   itemid: ID des Menuitems
//   value : true / false
//--------------------------------------------------------------
void MenuCtrl_ItemActive( uint8_t itemid, uint8_t value )
{
    int8_t i;

    i = _MenuCtrl_FindItem( itemid );

    if( i >= 0 )
    {
        menu[midx].item[i].disabled = !value;
    }
}


//--------------------------------------------------------------
// MenuCtrl_ItemMark( itemid, value )
//
// Markiert einen Menueeintrag mit einem '*' am Anfang (Links).
// Wird u.a. von BT_SelectDevice() (setup.c) verwendet
//
// Parameter:
//   itemid: ID des Menuitems
//   value : true / false
//--------------------------------------------------------------
void MenuCtrl_ItemMark( uint8_t itemid, uint8_t value )
{
    int8_t i;

    i = _MenuCtrl_FindItem( itemid );

    if( i >= 0 )
    {
        menu[midx].item[i].mark = value;
    }
}


//--------------------------------------------------------------
// MenuCtrl_ItemMarkR( itemid, value )
//
// Markiert einen Menueeintrag mit einem Haken am ENDE (Rechts).
// Wird u.a. von ... (setup.c) verwendet
//
// Parameter:
//   itemid: ID des Menuitems
//   value : true / false
//--------------------------------------------------------------
void MenuCtrl_ItemMarkR( uint8_t itemid, uint8_t value )
{
    int8_t i;

    i = _MenuCtrl_FindItem( itemid );

    if( i >= 0 )
    {
        menu[midx].item[i].markR = value;
    }
}


//--------------------------------------------------------------
// INTERN
//
// fuer: MenuCtrl_PushML_P(), MenuCtrl_Push()
//--------------------------------------------------------------
void _MenuCtrl_Push( uint8_t useprogmem, uint8_t numlang, uint8_t itemid, uint8_t itemtype, void (*menufunc)(void), ... )
{
    va_list ap;
    uint8_t idx;
    uint8_t i;

    if( midx < 0  )                                 { _MenuCtrl_Error( ERROR_NOMENU );  return; }
    if( menu[midx].itemcount >= MENUCTRL_MAXITEMS ) { _MenuCtrl_Error( ERROR_MAXITEM ); return; }
    //if( numlang > NUM_LANG )                      { _MenuCtrl_Error( ERROR_MAXLANG ); return; }

    idx = menu[midx].itemcount;

    menu[midx].item[idx].id       = itemid;
    menu[midx].item[idx].type     = itemtype;   // MENU_ITEM, MENU_SUB
    menu[midx].item[idx].mark     = false;
    menu[midx].item[idx].func     = menufunc;
    menu[midx].item[idx].textPGM  = useprogmem;
    menu[midx].item[idx].textcount= numlang;

    va_start(ap, menufunc);

    for( i=0; i<numlang; i++)
    {
        menu[midx].item[idx].text[i]  = va_arg( ap, const char *);
    }

    va_end(ap);

    menu[midx].itemcount++;
}


//--------------------------------------------------------------
// MenuCtrl_PushML2_P( itemid, itemtype, *menufunc, *text_de, *text_en, *text_nl )
//
// MultiLanguage Texte fuer 2 Sprachen (DE, EN)
// PROGMEN Version
//
// Parameter:
//   itemid  : ID des Menueitems
//   itemtype: MENU_ITEM oder MENU_SUB (bei MENU_SUB wird noch ein ">" angezeigt)
//   menufunc: Pointer auf Funktion oder NOFUNC (=0) wenn keine Funktion
//   texte   : alles Zeiger auf PROGMEM
//--------------------------------------------------------------
void MenuCtrl_PushML2_P( uint8_t itemid, uint8_t itemtype, void (*menufunc)(void), const char *text_de, const char *text_en )
{
  //_MenuCtrl_Push( useprogmem, numlang, itemid, itemtype, void (*menufunc)(void), ... )
    _MenuCtrl_Push( true, NUM_LANG, itemid, itemtype, menufunc, text_de, text_en );
}


//--------------------------------------------------------------
// MenuCtrl_PushML2( itemid, itemtype, *menufunc, *text_de, *text_en, *text_nl )
//
// MultiLanguage Texte fuer 2 Sprachen (DE, EN)
// RAM Version
//
// Parameter:
//   itemid  : ID des Menueitems
//   itemtype: MENU_ITEM oder MENU_SUB (bei MENU_SUB wird noch ein ">" angezeigt)
//   menufunc: Pointer auf Funktion oder NOFUNC (=0) wenn keine Funktion
//   texte   : alles Zeiger auf RAM
//--------------------------------------------------------------
void MenuCtrl_PushML2( uint8_t itemid, uint8_t itemtype, void (*menufunc)(void), const char *text_de, const char *text_en )
{
  //_MenuCtrl_Push( useprogmem, numlang, itemid, itemtype, void (*menufunc)(void), ... )
    _MenuCtrl_Push( false, NUM_LANG, itemid, itemtype, menufunc, text_de, text_en );
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void MenuCtrl_Push_P( uint8_t itemid, uint8_t itemtype, void (*menufunc)(void), const char *text )
{
  //_MenuCtrl_Push( useprogmem, numlang, itemid, itemtype, void (*menufunc)(void), ... )
    _MenuCtrl_Push( true, 1, itemid, itemtype, menufunc, text );
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void MenuCtrl_Push( uint8_t itemid, uint8_t itemtype, void (*menufunc)(void), const char *text )
{
  //_MenuCtrl_Push( useprogmem, numlang, itemid, itemtype, void (*menufunc)(void), ... )
    _MenuCtrl_Push( false, 1, itemid, itemtype, menufunc, text );
}


//--------------------------------------------------------------
// Spezialfunktion fuer mkparameters.c !
//
// -> siehe: mkparameters.c / Menu_EditCategory()
//
// ACHTUNG!
// Die externe Variable paramEditItem muss in mkparameters.c
// vor Aufruf dieser Funktion richtig initialisiert sein!
// Ich habe darauf verzichtet hier nochmal ein find_paramEditItem()
// aufzurufen um Speicherplatz zu sparen.
//--------------------------------------------------------------
void MenuCtrl_PushParamEdit( uint8_t paramID )
{
  //_MenuCtrl_Push( useprogmem, numlang, itemid, itemtype, void (*menufunc)(void), ... )
    _MenuCtrl_Push( true, 2, paramID, MENU_PARAMEDIT, NOFUNC, paramEditItem.title_de, paramEditItem.title_en );
}



//--------------------------------------------------------------
// MenuCtrl_PushSeparator()
//
// fuegt eine Trennlinie im Menue ein
// (die itemID ist dabei 0)
//--------------------------------------------------------------
void MenuCtrl_PushSeparator( void )
{
  //_MenuCtrl_Push( useprogmem, numlang, itemid, itemtype, void (*menufunc)(void), ... )
    _MenuCtrl_Push( true, 0, 0, MENU_SEPARATOR, NOFUNC );
}



//--------------------------------------------------------------
// MenuCtrl_PushSeparatorID( itemId )
//
// fuegt eine Trennlinie im Menue ein mit einer eigenen itemID
//--------------------------------------------------------------
void MenuCtrl_PushSeparatorID( uint8_t itemid )
{
  //_MenuCtrl_Push( useprogmem, numlang, itemid, itemtype, void (*menufunc)(void), ... )
    _MenuCtrl_Push( true, 0, itemid, MENU_SEPARATOR, NOFUNC );
}



//--------------------------------------------------------------
// MenuCtrl_Refresh( mode )
//
// Parameter:
//   mode: MENU_REDRAW || MENU_REFRESH
//--------------------------------------------------------------
void MenuCtrl_Refresh( uint8_t mode )
{
    uint8_t y;
    uint8_t item;
    uint8_t sh;
    uint8_t sy;
    uint8_t actchar;
    uint8_t markchar;
    uint8_t markcharR;
    uint8_t lang;

    if( midx < 0)   { _MenuCtrl_Error( ERROR_NOMENU ); return; }            // kein Menue vorhanden

    if( mode == MENU_REDRAW )
    {
        //--------------------------
        // Clear
        //--------------------------
        lcd_frect( 0,  8, 6, 47, 0);                                        // Clear: ganz linke Spalte des Sliders
        lcd_frect( 127-2, 0, 2, 63, 0);                                     // Clear: ganz rechts 2 Pixel

        //--------------------------
        // Titel
        //--------------------------
        if( menu[midx].title != 0 )
        {
            if( showlevel )
            {
                if( menu[midx].titlePGM )
                    lcdx_printf_at_P( 0, 0, MINVERS, 0,0 , PSTR("  %19S"), menu[midx].title );
                else
                    lcdx_printf_at_P( 0, 0, MINVERS, 0,0 , PSTR("  %19s"), menu[midx].title );

                writex_ndigit_number_u( 0, 0, midx, 1, 0, MINVERS, 1,0);    // Menuelevel zeigen
            }
            else
            {
                if( menu[midx].titlePGM )
                    lcdx_printf_at_P( 0, 0, MINVERS, 0,0 , PSTR(" %20S"), menu[midx].title );
                else
                    lcdx_printf_at_P( 0, 0, MINVERS, 0,0 , PSTR(" %20s"), menu[midx].title );
            }
        }
        else
            lcd_frect( 0, 0, 128, 7, 1);                                       // Titel: Leerzeile


        //--------------------------
        // Keyline Beschriftung
        //--------------------------
        lcdx_cls_row( 7, MNORMAL, 0);
        PKT_KeylineUpDown( 1, 7,  0,0);
        lcd_printp_at( 12, 7, strGet(KEYLINE4), MNORMAL);


        // Taste: KEY_ENTER
        if( menu[midx].key_enter.active && menu[midx].key_enter.text != 0 )
        {
            lcdx_printf_at_P( 17, 7, MNORMAL, 0,0 , PSTR("%4S"), menu[midx].key_enter.text );
        }

        // Taste: KEY_ESC
        if( menu[midx].key_esc.active && menu[midx].key_esc.text != 0 )
        {
            lcdx_printf_at_P( 12, 7, MNORMAL, 0,0 , PSTR("%5S"), menu[midx].key_esc.text );
        }
    }


    //--------------------------
    // PKT Batterie Anzeige
    //--------------------------
    if( menu[midx].showbatt )
    {
        show_Lipo();
    }


    //--------------------------
    // Zeilen
    //--------------------------
    for( y=0; y<6; y++)
    {
        item = y + menu[midx].display_pos;

        if( item < menu[midx].itemcount )
        {
            //--------------------------
            // char: aktuelle Auswahl
            //--------------------------
            actchar = ' ';
            if( item == menu[midx].active )
            {
                if( menu[midx].item[item].disabled )    actchar = 0x15;         // aktuelle Auswahl: Pfeil disabled
                else                                    actchar = 0x1d;         // aktuelle Auswahl: Pfeil
            }

            //--------------------------
            // char: markiert
            //--------------------------
            markchar = ' ';
            if( menu[midx].item[item].mark ) markchar = '*';

            //--------------------------
            // char: markiert RECHTS
            //--------------------------
            markcharR = ' ';
            if( menu[midx].item[item].type == MENU_SUB ) markcharR = 0x1d;
            if( menu[midx].item[item].markR )            markcharR = SYMBOL_CHECK;

            //--------------------------
            // Sprache: Multilanguage oder 0 wenn wenn nur ein Text
            //--------------------------
            lang = (menu[midx].item[item].textcount == 1) ? 0 : Config.DisplayLanguage;

            //--------------------------
            // Ausgabe: RAM oder PROGMEM
            //--------------------------
            if( MENU_SEPARATOR == menu[midx].item[item].type )
            {
                lcdx_printf_at_P( 1, y+1+menu[midx].topspace, MNORMAL, 0,0 , PSTR("%c%c%18S")  , actchar, markchar, PSTR("") );
                lcd_frect( 6*3, ((y+1)*8)+3, 124-(6*3), 0, 1);                                  // Trennlinie
            }
            else if( MENU_PARAMEDIT == menu[midx].item[item].type )
            {
                // SPEZIELL fuer mkparameters.c - zeigt die paramID-Bezeichnung inkl. dem aktuellen WERT an!
                strcpy_P( mkparam_strValueBuffer, PSTR("***"));                                     // Vorbelegung von mkparam_strValueBuffer
                if( !menu[midx].item[item].disabled )                                               // nur wenn Menuepunkt nicht deaktiviert (disabled = paramID nicht vorhanden)
                {                                                                                   //   -> ermittle den aktuellen Wert der paramID fuer Anzeige
                    find_paramEditItem( menu[midx].item[item].id );                                 // paramID suchen (Ergebnis in Variable 'paramEditItem')
                    paramEditItem.editfunc( paramEditItem.paramID, MKPARAM_SHORTVALUE );            // Wert von paramID in mkparam_strValueBuffer darstellen
                }
                lcdx_printf_at_P( 1, y+1+menu[midx].topspace, MNORMAL, 0,0 , PSTR("%c%c%14S %3s")  , actchar, markchar, menu[midx].item[item].text[lang], mkparam_strValueBuffer );
            }
            else
            {
                // Anmerkung zu menu[midx].topspace: NUR fuer Menues OHNE scrollen!!
                if( menu[midx].item[item].textPGM )
                {
                    // String im PGM
                    if( menu[midx].item[item].type == MENU_SUB || menu[midx].item[item].markR )
                        lcdx_printf_at_P( 1, y+1+menu[midx].topspace, MNORMAL, 0,0 , PSTR("%c%c%16S%c "), actchar, markchar, menu[midx].item[item].text[lang], markcharR );
                    else
                        lcdx_printf_at_P( 1, y+1+menu[midx].topspace, MNORMAL, 0,0 , PSTR("%c%c%17S ")  , actchar, markchar, menu[midx].item[item].text[lang] );
                }
                else
                {
                    // String im RAM
                    if( menu[midx].item[item].type == MENU_SUB || menu[midx].item[item].markR )
                        lcdx_printf_at_P( 1, y+1+menu[midx].topspace, MNORMAL, 0,0 , PSTR("%c%c%16s%c "), actchar, markchar, menu[midx].item[item].text[lang], markcharR );
                    else
                        lcdx_printf_at_P( 1, y+1+menu[midx].topspace, MNORMAL, 0,0 , PSTR("%c%c%17s ")  , actchar, markchar, menu[midx].item[item].text[lang] );
                }
            }

        } // end: if( item < menu[midx].itemcount )


        //--------------------------
        // evtl. Leerzeile loeschen
        //--------------------------
        if( (y < menu[midx].topspace) || (item >= menu[midx].itemcount+menu[midx].topspace) )
        {
            lcd_frect( 6, (y+1)*8, 128-6, 7, 0);                                    // Zeile loeschen
        }

    }


    //--------------------------
    // Slider
    //--------------------------
    #define SLIDERH   45                                                            // Finetuning der Slider-Hoehe

    lcd_frect( 0,  8+1, 1, SLIDERH, 0);                                             // Slider: full Clear

    if( menu[midx].itemcount > 6 )                                                  // Slider nur bei mehr als 6 Menueitems
    {
        // Slider: 7 zeilen * 8 pixel = 56 pixel
        sh = (SLIDERH * 6) / menu[midx].itemcount;                                  // Slider: Hoehe
        sh = (sh > SLIDERH) ? SLIDERH : sh;

        sy = 8+(menu[midx].display_pos * (SLIDERH-sh)) / (menu[midx].itemcount-6);  // Slider: Position

        //lcd_frect( 0,  8+1, 1, SLIDERH, 0);                                       // Slider: full Clear    (flackert auf blauen Display!)
        //lcd_frect( 0, 8+1, 1, sy-(8), 0);                                         // Slider: Clear / oben  (nicht mehr notwendig da Aenderung am Refresh)
        //lcd_frect( 0, sy+1+sh+1, 1, SLIDERH-(sh-sy+1), 0);                        // Slider: Clear / unten (nicht mehr notwendig da Aenderung am Refresh)
        lcd_frect( 0, sy+1, 1, sh, 1);                                              // Slider: Draw
    }
}



//--------------------------------------------------------------
// event = MenuCtrl_Control( ctrlmode )
//
// Parameter:
//   ctrlmode: MENUCTRL_EVENT || MENUCTRL_RETURN
//
// Rueckgabe:
//   event: MENUEVENT_NONE || MENUEVENT_ITEM || MENUEVENT_KEY
//--------------------------------------------------------------
uint8_t MenuCtrl_Control( uint8_t ctrlmode )
{
    uint8_t menu_event;
    uint8_t refreshmode;
    uint8_t item;
    uint8_t wahl;
    uint8_t i;
    uint8_t goUp    = false;
    uint8_t goDown  = false;

    if( midx < 0)                   { _MenuCtrl_Error( ERROR_NOMENU ); return 0; }              // kein Menue vorhanden
    if( menu[midx].itemcount == 0)  { _MenuCtrl_Error( ERROR_NOITEM ); return 0; }              // kein Menueitem vorhanden


    // bei MENUCTRL_RETURN muss man ggf. selber fuer ein Redraw sorgen
    // (beim ersten Aufruf wird es jedoch dennoch erzwungen)
    if( ctrlmode == MENUCTRL_EVENT || !menu[midx].firstredraw )
    {
        MenuCtrl_Refresh( MENU_REDRAW );
        menu[midx].firstredraw = true;
    }

    clear_key_all();

    do
    {
        menu_event         = MENUEVENT_NONE;
        menu[midx].lastkey = KEY_NONE;
        refreshmode        = false;

        //--------------------------
        // Pruefe PKT Update oder
        // andere PKT-Aktion
        //--------------------------
        if( PKT_CtrlHook() )                                                                    // Update vom Updatetool angefordert?
        {
            refreshmode = MENU_REDRAW;
        }

        //--------------------------
        // PKT Batterie Anzeige
        //--------------------------
        if( menu[midx].showbatt )
        {
            show_Lipo();
        }


        //--------------------------
        // Cursor: nach oben (UP)
        //--------------------------
        //+++++
        // UP - Verschiebemodus AKTIV
        //+++++
        if( menu[midx].canMove )
        {
            if( get_key_long(1 << KEY_MINUS) )
            {
                if( (menu[midx].active > 0) && (menu[midx].itemcount > 0) )
                {
                    _MenuCtrl_SwapItem( menu[midx].active, menu[midx].active-1 );
                    goUp = true;
                }
                else _MenuCtrl_Beep();                                                           // am oberen Ende angelangt
            }

            if( get_key_short(1 << KEY_MINUS) )     // up
            {
                goUp = true;
            }
        }   // end: UP - Verschiebemodus AKTIV


        //+++++
        // UP - Verschiebemodus NICHT aktiv
        //+++++
        if( !menu[midx].canMove )     // up
        {
            if( get_key_press(1 << KEY_MINUS) || get_key_long_rpt_sp((1 << KEY_MINUS),2) )      // up
            {
                goUp = true;
            }
        }   // end: UP - Verschiebemodus NICHT aktiv


        //+++++
        // UP - goUp
        //+++++
        if( goUp )     // up
        {
            refreshmode = MENU_REFRESH;
            if( (menu[midx].active > 0)  && (menu[midx].itemcount > 0) )
            {
                menu[midx].active--;
            }
            else if( menu[midx].cycle )
            {
                menu[midx].active = menu[midx].itemcount-1;
            }
            else
            {
                _MenuCtrl_Beep();                                                               // am oberen Ende angelangt
            }
            _MenuCtrl_CalcDisplayPos();
        }


        //--------------------------
        // Cursor: nach unten (DOWN)
        //--------------------------
        //+++++
        // DOWN - Verschiebemodus AKTIV
        //+++++
        if( menu[midx].canMove )
        {
            if( get_key_long(1 << KEY_PLUS) )
            {
                if( menu[midx].active < menu[midx].itemcount-1 )
                {
                    _MenuCtrl_SwapItem( menu[midx].active, menu[midx].active+1 );
                    goDown = true;
                }
                else _MenuCtrl_Beep();                                                           // am unteren Ende angelangt
            }

            if( get_key_short(1 << KEY_PLUS) )     // up
            {
                goDown = true;
            }
        }   // end: DOWN - Verschiebemodus AKTIV


        //+++++
        // DOWN - Verschiebemodus NICHT aktiv
        //+++++
        if( !menu[midx].canMove )     // up
        {
            if( get_key_press(1 << KEY_PLUS) || get_key_long_rpt_sp((1 << KEY_PLUS),2) )     // up
            {
                goDown = true;
            }
        }   // end: DOWN - Verschiebemodus NICHT aktiv


        //+++++
        // DOWN - goDown
        //+++++
        if( goDown )     // down
        {
            refreshmode = MENU_REFRESH;
            if( menu[midx].active < menu[midx].itemcount-1 )
            {
                menu[midx].active++;
            }
            else if( menu[midx].cycle )
            {
                menu[midx].active = 0;
            }
            else
            {
                _MenuCtrl_Beep();                                                           // am unteren Ende angelangt
            }
            _MenuCtrl_CalcDisplayPos();
        }


        //--------------------------
        //--------------------------
        item    = menu[midx].active;
        goUp    = false;
        goDown  = false;


        //--------------------------
        // KEY_ENTER
        //--------------------------
        if( get_key_short(1 << KEY_ENTER) )
        {
            refreshmode = MENU_REFRESH;
            // todo: keyfunc
            if( menu[midx].item[item].type == MENU_SEPARATOR )
            {
                // do nothing
            }
            else if( menu[midx].item[item].disabled )
            {
                set_beep( 200, 0x00ff, BeepNormal);                             // Beep
                PKT_Popup_P( 200, strGet(STR_MENUCTRL_NOTPOSSIBLE),0,0,0);      // "nicht möglich!"
                refreshmode = MENU_REDRAW;
            }
            else if( menu[midx].item[item].func != NOFUNC )
            {
                menu[midx].item[item].func();
                refreshmode = MENU_REDRAW;
            }
            else if( menu[midx].key_enter.func != NOFUNC )
            {
                menu[midx].key_enter.func();
                refreshmode = MENU_REDRAW;
            }
            else
            {
                menu[midx].lastkey = KEY_ENTER;
                menu_event = MENUEVENT_ITEM;
            }
        }


        //--------------------------
        // KEY_ENTER_LONG (Deletemodus AKTIV)
        //
        // loescht einen Menueeintrag
        //--------------------------
        if( menu[midx].canDelete && get_key_long(1 << KEY_ENTER) && (menu[midx].itemcount > 0) )
        {
            wahl = 1;
            if( menu[midx].canDelete == 1 )                                     // ==1 -> mit Nachfrage
            {
                set_beep( 200, 0xffff, BeepNormal);                             // Beep
                PKT_Popup_P( 0, strGet(STR_MENUCTRL_DELASK),PSTR(""),0,0);      // "Eintrag entfernen?"
                lcd_printp_at( 12, 7, strGet(YESNO), MINVERS);

                wahl = 0;
                while( !wahl )
                {
                    if( get_key_short(1<<KEY_ESC) )   wahl = 1;                 // "Ja"
                    if( get_key_short(1<<KEY_ENTER) ) wahl = 2;                 // "Nein"
                }
                refreshmode = MENU_REDRAW;
            }

            if( wahl==1 )
            {
                for( i=menu[midx].active; i<menu[midx].itemcount-1; i++)
                {
                    _MenuCtrl_SwapItem( i, i+1 );
                }
                menu[midx].itemcount--;

                if( (menu[midx].display_pos > 0)  &&   (menu[midx].display_pos+6 > menu[midx].itemcount)  )
                {
                    menu[midx].display_pos--;
                    menu[midx].active--;
                }

                if( menu[midx].active >= menu[midx].itemcount )
                    menu[midx].active = menu[midx].itemcount-1;

                _MenuCtrl_CalcDisplayPos();
                refreshmode = MENU_REDRAW;

                if( menu[midx].canDelete == 2 )                                 // ==2 -> direkt loeschen, ohne Nachfrage
                {
                    set_beep( 200, 0xffff, BeepNormal);                         // Beep
                    PKT_Popup_P( 200, strGet(STR_MENUCTRL_DELITEM),0,0,0);      // "Eintrag entfernt!"
                }
            }
        }


        //--------------------------
        // KEY_ENTER_LONG (NICHT Deletemodus)
        //--------------------------
        if( !menu[midx].canDelete && menu[midx].key_enter_long.active && get_key_long(1 << KEY_ENTER) )
        {
            refreshmode = MENU_REFRESH;
            if( menu[midx].key_enter_long.func != NOFUNC )
            {
                menu[midx].key_enter_long.func();
                refreshmode = MENU_REDRAW;
            }
            else
            {
                menu_event = MENUEVENT_KEY;
                menu[midx].lastkey = KEY_ENTER_LONG;
            }
        }


        //--------------------------
        // KEY_ESC_LONG
        //--------------------------
        if( menu[midx].key_esc_long.active && get_key_long(1 << KEY_ESC) )
        {
            refreshmode = MENU_REFRESH;
            if( menu[midx].key_esc_long.func != NOFUNC )
            {
                menu[midx].key_esc_long.func();
                refreshmode = MENU_REDRAW;
            }
            else
            {
                menu_event = MENUEVENT_KEY;
                menu[midx].lastkey = KEY_ESC_LONG;
            }
        }


        //--------------------------
        // KEY_ESC
        //--------------------------
        if( get_key_short(1 << KEY_ESC) )
        {
            refreshmode = MENU_REFRESH;
            if( menu[midx].key_esc.func != NOFUNC )
            {
                menu[midx].key_esc.func();
                refreshmode = MENU_REDRAW;
            }
            else
            {
                menu_event = MENUEVENT_KEY;
                menu[midx].lastkey = KEY_ESC;
            }
        }

        if( refreshmode )
        {
            MenuCtrl_Refresh( refreshmode );
            clear_key_all();
        }
    }
    while ( menu_event == MENUEVENT_NONE && ctrlmode == MENUCTRL_EVENT );

    return menu_event;
}

