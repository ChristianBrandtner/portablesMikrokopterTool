/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
* hier wird Code von den alten OSD-Screens ausgelagert um messages.c zu entkernen
*****************************************************************************/

//############################################################################
//# HISTORY  osdold_messages.c
//#
//# 01.06.2014 OG  * NEU *
//# Auslagerung von Texten aus messages.c speziell fuer die alten und nicht mehr
//# unterstuetzten OSD-Screens
//############################################################################


#include <avr/pgmspace.h>
#include "../main.h"

#ifdef USE_OSD_SCREEN_OLD

#include "../eeprom/eeprom.h"
#include "osdold_messages.h"


//---------------------------------------------------------------------------------

static const char OSD_ALTI_0_de[]       PROGMEM =  "Höhe  aus ";
static const char OSD_ALTI_0_en[]       PROGMEM =  "Alti. off ";

static const char OSD_ALTI_1_de[]       PROGMEM =  "Höhe begr.";
static const char OSD_ALTI_1_en[]       PROGMEM =  "Alti.Limit";

static const char OSD_VARIO_0_de[]      PROGMEM =  "Vario aus ";
static const char OSD_VARIO_0_en[]      PROGMEM =  "Vario off ";

static const char OSD_VARIO_1_de[]      PROGMEM =  "Vario Höhe";
static const char OSD_VARIO_1_en[]      PROGMEM =  "Vario Alt.";

static const char OSD_CARE_FREE_0_de[]  PROGMEM =  "         ";
#define           OSD_CARE_FREE_0_en    OSD_CARE_FREE_0_de

static const char OSD_CARE_FREE_1_de[]  PROGMEM =  "Care Free";
#define           OSD_CARE_FREE_1_en    OSD_CARE_FREE_1_de

static const char OSD_NAVI_MODE_0_de[]  PROGMEM =  "Navi  aus  ";
static const char OSD_NAVI_MODE_0_en[]  PROGMEM =  "Navi  off  ";

static const char OSD_NAVI_MODE_1_de[]  PROGMEM =  "Pos. halten";
static const char OSD_NAVI_MODE_1_en[]  PROGMEM =  "Pos. Hold  ";

static const char OSD_NAVI_MODE_2_de[]  PROGMEM =  "Coming Home";
#define           OSD_NAVI_MODE_2_en    OSD_NAVI_MODE_2_de

static const char OSD_FLAGS_0_de[]      PROGMEM =  "         ";
#define           OSD_FLAGS_0_en        OSD_FLAGS_0_de

static const char OSD_FLAGS_1_de[]      PROGMEM =  "Justieren";
static const char OSD_FLAGS_1_en[]      PROGMEM =  "Calibrate";

static const char OSD_FLAGS_2_de[]      PROGMEM =  "Start    ";
#define           OSD_FLAGS_2_en        OSD_FLAGS_2_de

static const char OSD_FLAGS_3_de[]      PROGMEM =  "Betrieb  ";
static const char OSD_FLAGS_3_en[]      PROGMEM =  "Run      ";

static const char OSD_FLAGS_4_de[]      PROGMEM =  "Fliegen  ";
static const char OSD_FLAGS_4_en[]      PROGMEM =  "Fly      ";

static const char OSD_FLAGS_5_de[]      PROGMEM =  "Landung  ";
static const char OSD_FLAGS_5_en[]      PROGMEM =  "Landing  ";

static const char OSD_FLAGS_6_de[]      PROGMEM =  "Akku leer";
static const char OSD_FLAGS_6_en[]      PROGMEM =  "Low Bat. ";

static const char OSD_LED_Form_de[]     PROGMEM =  "Out1/2 Format";
static const char OSD_LED_Form_en[]     PROGMEM =  "Out1/2 format";

//------------------------------------------------------------------------------


const char * const strings_osdold[] PROGMEM=
{
  OSD_ALTI_0_de,            OSD_ALTI_0_en,
  OSD_ALTI_1_de,            OSD_ALTI_1_en,
  OSD_VARIO_0_de,           OSD_VARIO_0_en,
  OSD_VARIO_1_de,           OSD_VARIO_1_en,
  OSD_CARE_FREE_0_de,       OSD_CARE_FREE_0_en,
  OSD_CARE_FREE_1_de,       OSD_CARE_FREE_1_en,
  OSD_NAVI_MODE_0_de,       OSD_NAVI_MODE_0_en,
  OSD_NAVI_MODE_1_de,       OSD_NAVI_MODE_1_en,
  OSD_NAVI_MODE_2_de,       OSD_NAVI_MODE_2_en,
  OSD_FLAGS_0_de,           OSD_FLAGS_0_en,
  OSD_FLAGS_1_de,           OSD_FLAGS_1_en,
  OSD_FLAGS_2_de,           OSD_FLAGS_2_en,
  OSD_FLAGS_3_de,           OSD_FLAGS_3_en,
  OSD_FLAGS_4_de,           OSD_FLAGS_4_en,
  OSD_FLAGS_5_de,           OSD_FLAGS_5_en,
  OSD_FLAGS_6_de,           OSD_FLAGS_6_en,
  OSD_LED_Form_de,          OSD_LED_Form_en,

  //******************************************************************
  // hier stehen lassen, alle neuen Strings hier drüber einfügen
  //LAST_STR_de,       LAST_STR_en,
};


char const * strGetOSDOLD( int str_no )
{
    if( Config.DisplayLanguage > 1 )    Config.DisplayLanguage = 1;

    if( Config.DisplayLanguage == 0 )   return  (const char*) pgm_read_word( &strings_osdold[str_no*2]     );
    if( Config.DisplayLanguage == 1 )   return  (const char*) pgm_read_word( &strings_osdold[(str_no*2)+1] );

  return  (const char*) pgm_read_word( &strings_osdold[0] );
}


#endif  // ifdef: USE_OSD_SCREEN_OLD
