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
//# HISTORY  osddata.h
//# 
//# 29.03.2014 OG
//# - add: OSDDATA_ShowLastGPSPosition()
//#
//# 11.06.2013 OG
//# - add: Menu_OSDData() - ehemals in menu.c jetzt hier
//# - del: verschiedene andere exportierte Funktionen
//#
//# 20.04.2013 OG - NEU
//############################################################################

#ifndef _OSDDATA_H
#define _OSDDATA_H

void OSDDATA_ShowLastGPSPosition( void );
void Menu_OSDData( void );

#endif
