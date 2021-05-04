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
//# HISTORY  scrollbox.h
//# 
//# 17.05.2013 OG
//# - del: defines KEY_xyz_LONG verschoben nach HAL_HW3_9.h
//#
//# 28.04.2013 OG
//# - add: ScrollBox_Push()
//# - chg: ScrollBox_Push_P() (keine Rueckgabe mehr)
//# - add: include <stdarg.h>
//#
//# 20.04.2013 OG - NEU
//############################################################################

#ifndef _SCROLLBOX_H
#define _SCROLLBOX_H

#include <stdarg.h>			// Notwendig! (OG)


uint8_t ScrollBox_Create( uint8_t maxlines );
void 	ScrollBox_Destroy( void );
void	ScrollBox_PushLine( void );
void	ScrollBox_Push_P( uint8_t mode, const char *format, ... );
void 	ScrollBox_Push( uint8_t mode, const char *format, ... );
void 	ScrollBox_SetKey( uint8_t key, const char *keytext );
void 	ScrollBox_Refresh( void );
uint8_t ScrollBox_Show( void );
//void ScrollBox_Debug( void );

#endif
