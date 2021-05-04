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
//# HISTORY  xutils.h
//#
//# 12.04.2014 OG
//# - chg: strncpyat(), strncpyat_P(), _strncpyat() erweitert um Parameter 'sepcharcount'
//#
//# 08.04.2014 OG
//# - add: strncpyat(), strncpyat_P()
//#
//# 28.02.2014 OG
//# - add: buffered_sprintf(), buffered_sprintf_P()
//#
//# 24.06.2013 OG
//# - add: strrtrim()
//#
//# 04.05.2013 OG
//# - chg: umbenannt zu xutils.h
//#
//# 03.05.2013 OG
//# - add: UTCdatetime2local()
//#
//# 28.04.2013 OG - NEU
//############################################################################

#ifndef _XUTILS_H
#define _XUTILS_H

#include <stdarg.h>


void _xvsnprintf( uint8_t useprogmem, char *buffer, uint8_t n, const char *format, va_list ap );
void xsnprintf( char *buffer, uint8_t n, const char *format, ... );
void xsnprintf_P( char *buffer, uint8_t n, const char *format, ... );

char *buffered_sprintf( const char *format, ... );
char *buffered_sprintf_P( const char *format, ... );

void strncpyfill( char *dst, const char *src, size_t size);
void strrtrim( char *dst);
void strncpyat( char *dst, const char *src, size_t size, const char sepchar, uint8_t sepcharcount);
void strncpyat_P( char *dst, const char *src, size_t size, const char sepchar, uint8_t sepcharcount);


void UTCdatetime2local( PKTdatetime_t *dtdst, PKTdatetime_t *dtsrc );

#endif
