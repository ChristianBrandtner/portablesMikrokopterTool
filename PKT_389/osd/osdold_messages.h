/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
* hier wird Code von den alten OSD-Screens ausgelagert um messages.c zu entkernen
*****************************************************************************/

//############################################################################
//# HISTORY  osdold_messages.h
//#
//# 01.06.2014 OG  * NEU *
//# Auslagerung von Texten aus messages.c speziell fuer die alten und nicht mehr
//# unterstuetzten OSD-Screens
//############################################################################

#ifndef OSDOLD_MESSAGES_H
#define OSDOLD_MESSAGES_H


#include "../main.h"


#ifdef USE_OSD_SCREEN_OLD

//---------------------------------------------------------------------------------------------------------------------
// Typdefinitionen für alle verwendeten Strings
//LAST_STR muss am Ende stehen bleiben
typedef enum
{
    OSD_ALTI_0, OSD_ALTI_1, OSD_VARIO_0, OSD_VARIO_1, OSD_CARE_FREE_0, OSD_CARE_FREE_1,
    OSD_NAVI_MODE_0, OSD_NAVI_MODE_1, OSD_NAVI_MODE_2,
    OSD_FLAGS_0, OSD_FLAGS_1, OSD_FLAGS_2, OSD_FLAGS_3, OSD_FLAGS_4, OSD_FLAGS_5, OSD_FLAGS_6,
    OSD_LED_Form
} OSDOLDSTR;


char const * strGetOSDOLD( int str_no );


#endif // USE_OSD_SCREEN_OLD

#endif /* OSDOLD_MESSAGES_H_ */
