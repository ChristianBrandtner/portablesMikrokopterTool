/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*
 *****************************************************************************
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
//# HISTORY  menuctrl.h
//#
//# 11.06.2014 OG
//# - add: MenuCtrl_GetMenuIndex()
//#
//# 11.05.2014 OG
//# - add: MenuCtrl_SetTitleFromParentItem()
//# - add: MenuCtrl_GetItemText(), MenuCtrl_IsItemTextPGM()
//#
//# 07.05.2014 OG
//# - add: MenuCtrl_PushSeparatorID()
//#
//# 06.05.2014 OG
//# - add: MenuCtrl_GetItemIdByIndex(), MenuCtrl_GetItemCount()
//#
//# 05.05.2014 OG
//# - add: MenuCtrl_SetDelete(), MenuCtrl_SetMove()
//#
//# 30.03.2014 OG
//# - del: MenuCtrl_PushML_P(), MenuCtrl_PushML() (Umstellung Sprachen abgeschlossen)
//# - add: MenuCtrl_PushML2(), MenuCtrl_PushML2_P() fuer Umstellung von 3 auf 2 Sprachen
//#
//# 24.03.2014 OG
//# - add: MenuCtrl_PushSeparator()
//# - add: MENU_SEPARATOR
//# - chg: Wert von MENU_REFRESH und MENU_REDRAW
//#
//# 23.03.2014 OG
//# - add: Unterstuetzung fuer MK-Parameter-Edit (mkparameters.c)
//# - add: MenuCtrl_PushParamEdit()
//# - add: define MENU_PARAMEDIT
//#
//# 17.02.2014 OG
//# - add: MenuCtrl_SetTopSpace()
//#
//# 15.02.2014 OG
//# - add: MenuCtrl_ItemSelect()
//#
//# 07.07.2013 OG
//# - add: MenuCtrl_ItemMarkR()
//#
//# 24.05.2013 OG
//# - chg: MenuCtrl_Push... Funktionen vereinheitlicht mit internen Aenderungen
//#        betrifft externe Funktionsaufrufe die geändert wurden
//# - del: MenuCtrl_PushSimple_P(), MenuCtrl_PushSimple()
//# - add: MenuCtrl_ItemMark()
//#
//# 23.05.2013 OG
//# - add: MenuCtrl_PushSimple_P(), MenuCtrl_PushSimple()
//#
//# 21.05.2013 OG
//# - add: MenuCtrl_ShowLevel()
//#
//# 17.05.2013 OG - NEU
//############################################################################

#ifndef MENUCTRL_H
#define MENUCTRL_H

#define MENU_ITEM       1   // Typ des Menueitems: normaler Eintrag ohne ">" am Ende
#define MENU_SUB        2   // Typ des Menueitems: Submenue - es wird noch ein ">" angezeigt
#define MENU_SEPARATOR  3   // nur fuer MenuCtrl_PushSeparator() fuer (verwendet von mkparameters.c)
#define MENU_PARAMEDIT  4   // nur fuer MenuCtrl_PushParamEdit() fuer (verwendet von mkparameters.c)

#define NOFUNC          0   // keine Funktion uebergeben
#define NOTEXT          0   // kein Text uebergeben

#define MENUEVENT_NONE  0   // wird von MenuCtrl_Ctrl() zurueckgegeben
#define MENUEVENT_ITEM  1   // wird von MenuCtrl_Ctrl() zurueckgegeben
#define MENUEVENT_KEY   2   // wird von MenuCtrl_Ctrl() zurueckgegeben

#define MENUCTRL_EVENT  0   // Parameter fuer MenuCtrl_Ctrl() - Ruecksprung nur bei einem Event
#define MENUCTRL_RETURN 1   // Parameter fuer MenuCtrl_Ctrl() - Ruecksprung immer


#define MENU_REFRESH    1   // MenuCtrl_Refresh()
#define MENU_REDRAW     2   // MenuCtrl_Refresh()


void MenuCtrl_Create( void );
void MenuCtrl_Destroy( void );
void MenuCtrl_ShowLevel( uint8_t value );

void MenuCtrl_SetTitle( const char *title );
void MenuCtrl_SetTitle_P( const char *title );
void MenuCtrl_SetTitleFromParentItem( void );
void MenuCtrl_SetCycle( uint8_t value );
void MenuCtrl_SetShowBatt( uint8_t value );
void MenuCtrl_SetBeep( uint8_t value );
void MenuCtrl_SetKey( uint8_t key, const char *keytext, void (*keyfunc)(void) );
void MenuCtrl_SetTopSpace( uint8_t value );
void MenuCtrl_SetMove( uint8_t value );
void MenuCtrl_SetDelete( uint8_t value );

int8_t MenuCtrl_GetMenuIndex( void );
uint8_t MenuCtrl_GetItemId( void );
uint8_t MenuCtrl_GetKey( void );
uint8_t MenuCtrl_GetItemIdByIndex( uint8_t index );
uint8_t MenuCtrl_GetItemCount( void );
const char * MenuCtrl_GetItemText( void );
uint8_t MenuCtrl_IsItemTextPGM( void );

void MenuCtrl_ItemSelect( uint8_t itemid );
void MenuCtrl_ItemActive( uint8_t itemid, uint8_t value );
void MenuCtrl_ItemMark( uint8_t itemid, uint8_t value );
void MenuCtrl_ItemMarkR( uint8_t itemid, uint8_t value );

void MenuCtrl_PushML2_P( uint8_t itemid, uint8_t itemtype, void (*menufunc)(void), const char *text_de, const char *text_en );
void MenuCtrl_PushML2( uint8_t itemid, uint8_t itemtype, void (*menufunc)(void), const char *text_de, const char *text_en );
void MenuCtrl_Push_P( uint8_t itemid, uint8_t itemtype, void (*menufunc)(void), const char *text );
void MenuCtrl_Push( uint8_t itemid, uint8_t itemtype, void (*menufunc)(void), const char *text );
void MenuCtrl_PushParamEdit( uint8_t itemid );
void MenuCtrl_PushSeparator( void );
void MenuCtrl_PushSeparatorID( uint8_t itemid );

void MenuCtrl_Refresh( uint8_t mode );
uint8_t MenuCtrl_Control( uint8_t ctrlmode );

#endif  // MENUCTRL_H
