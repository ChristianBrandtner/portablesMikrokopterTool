/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
* hier wird Code von den alten OSD-Screens ausgelagert um osd.c zu entkernen
*****************************************************************************/

//############################################################################
//# HISTORY  osdold_screens.h
//#
//# 01.06.2014 OG  * NEU *
//# Auslagerung von Funktionen fuer alte OSD-Screens aus osd.c
//############################################################################

#ifndef _OSDOLD_SCREENS_H
#define _OSDOLD_SCREENS_H


#include "../main.h"


#ifdef USE_OSD_SCREEN_OLD

void OSD_Element_AltitudeControl( uint8_t x, uint8_t y );
void OSD_Element_CareFree( uint8_t x, uint8_t y );
void OSD_Element_NaviMode( uint8_t x, uint8_t y );
void OSD_Element_StatusFlags( uint8_t x, uint8_t y );


void OSD_Screen_OSD0( void );
void OSD_Screen_OSD1( void );
void OSD_Screen_OSD2( void );


#endif // USE_OSD_SCREEN_OLD

#endif // _OSDOLD_SCREENS_H_
