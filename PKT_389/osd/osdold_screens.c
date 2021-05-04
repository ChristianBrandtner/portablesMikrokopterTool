/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
* hier wird Code von den alten OSD-Screens ausgelagert um osd.c zu entkernen
*****************************************************************************/

//############################################################################
//# HISTORY  osdold_screens.c
//#
//# 01.06.2014 OG  * NEU *
//# Auslagerung von Funktionen fuer alte OSD-Screens aus osd.c
//############################################################################


#include "../cpu.h"
#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include <util/atomic.h>

#include "../main.h"
#include "../lcd/lcd.h"
#include "../timer/timer.h"
#include "../uart/usart.h"
#include "../eeprom/eeprom.h"
#include "../messages.h"
#include "../sound/pwmsine8bit.h"
#include "../mk-data-structs.h"
#include "../pkt/pkt.h"
#include "../osd/osd.h"
#include "../utils/xutils.h"
#include "../mk/mkbase.h"
#include "../osd/osdold_messages.h"
#include "../osd/osdold_screens.h"



#ifdef USE_OSD_SCREEN_OLD


//--------------------------------------------------------------
// OSD_Element_AltitudeControl( x, y)
//--------------------------------------------------------------
void OSD_Element_AltitudeControl( uint8_t x, uint8_t y )
{
        //---------------------------------------------------------
        // 10.03.2013 OG:
        //   CFG2_HEIGHT_LIMIT im Augenblick nicht unterstuetzt
        //   Siehe Anmerkungen in osd()
        //---------------------------------------------------------
        /*
        if (Flags_ExtraConfig & CFG2_HEIGHT_LIMIT)
        {
            if (naviData->FCStatusFlags2 & FC_STATUS2_ALTITUDE_CONTROL)
                lcd_printp_at (x, y, strGet(OSD_ALTI_1), 0);    // Höhe begr.
            else
                lcd_printp_at (x, y, strGetOSDOLD(OSD_ALTI_0), 0);    // Höhe aus
        }
        else
        {
            if (naviData->FCStatusFlags2 & FC_STATUS2_ALTITUDE_CONTROL)
                lcd_printp_at (x, y, strGetOSDOLD(OSD_VARIO_1), 0);   // Vario Höhe
            else
                lcd_printp_at (x, y, strGetOSDOLD(OSD_VARIO_0), 0);   // Vario aus
        }
        */

            if (naviData->FCStatusFlags2 & FC_STATUS2_ALTITUDE_CONTROL)
                lcd_printp_at (x, y, strGetOSDOLD(OSD_VARIO_1), 0);   // Vario Höhe
            else
                lcd_printp_at (x, y, strGetOSDOLD(OSD_VARIO_0), 0);   // Vario aus
}


//--------------------------------------------------------------
// OSD_Element_CareFree( x, y)
//--------------------------------------------------------------
void OSD_Element_CareFree( uint8_t x, uint8_t y )
{
    lcd_printp_at (x, y, strGetOSDOLD( naviData->FCStatusFlags2 & FC_STATUS2_CAREFREE ? OSD_CARE_FREE_1 : OSD_CARE_FREE_0 ), 0);
}


//--------------------------------------------------------------
// OSD_Element_NaviMode( x, y)
//--------------------------------------------------------------
void OSD_Element_NaviMode( uint8_t x, uint8_t y )
{
    if (naviData->NCFlags & NC_FLAG_FREE)
        lcd_printp_at (x, y, strGetOSDOLD(OSD_NAVI_MODE_0), 0);   // Navi aus
    else if (naviData->NCFlags & NC_FLAG_PH)
        lcd_printp_at (x, y, strGetOSDOLD(OSD_NAVI_MODE_1), 0);   // Pos. Hold
    else if (naviData->NCFlags & NC_FLAG_CH)
        lcd_printp_at (x, y, strGetOSDOLD(OSD_NAVI_MODE_2), 0);   // Coming Home
}


//--------------------------------------------------------------
// OSD_Element_StatusFlags( x, y)
//--------------------------------------------------------------
void OSD_Element_StatusFlags( uint8_t x, uint8_t y )
{
    // FC_StatusFlags 0.88
    // #define FC_STATUS_MOTOR_RUN           0x01
    // #define FC_STATUS_FLY                 0x02
    // #define FC_STATUS_CALIBRATE           0x04
    // #define FC_STATUS_START               0x08
    // #define FC_STATUS_EMERGENCY_LANDING   0x10
    // #define FC_STATUS_LOWBAT              0x20
    // #define FC_STATUS_VARIO_TRIM_UP       0x40
    // #define FC_STATUS_VARIO_TRIM_DOWN     0x80

    if (naviData->FCStatusFlags & FC_STATUS_CALIBRATE)
        lcd_printp_at (x, y, strGetOSDOLD(OSD_FLAGS_1), 0);   // Calibrate
    else if (naviData->FCStatusFlags & FC_STATUS_START)
        lcd_printp_at (x, y, strGetOSDOLD(OSD_FLAGS_2), 0);   // Start
    else if (naviData->FCStatusFlags & FC_STATUS_MOTOR_RUN)
        lcd_printp_at (x, y, strGetOSDOLD(OSD_FLAGS_3), 0);   // Run
    else if (naviData->FCStatusFlags & FC_STATUS_FLY)
        lcd_printp_at (x, y, strGetOSDOLD(OSD_FLAGS_4), 0);   // Fly
    else if (naviData->FCStatusFlags & FC_STATUS_EMERGENCY_LANDING)
        lcd_printp_at (x, y, strGetOSDOLD(OSD_FLAGS_5), 0);   // Landing
    else if (naviData->FCStatusFlags & FC_STATUS_LOWBAT)
        lcd_printp_at (x, y, strGetOSDOLD(OSD_FLAGS_6), 0);   // LowBat
    else
    //  lcd_printp_at (x, y, PSTR("         "), 0); // Clear
        lcd_printp_at (x, y, strGetOSDOLD(OSD_FLAGS_0), 0);   // Clear
}



//##############################################################
//# OLD OSD-Screens
//##############################################################

//--------------------------------------------------------------
// ehemals: OSD_ALTITUDE,Config.OSD_ScreenMode == 0
//--------------------------------------------------------------
void OSD_Screen_OSD0( void )
{
    //uint8_t xoffs;

    //-----------------------------------------
    // REDRAW static screen elements
    //-----------------------------------------
    if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    {
        // do things here for static screen elements like labels and so....
    }

    OSD_Element_AltitudeControl ( 0, 3);
    OSD_Element_Altitude        ( 11, 3, 0);
    OSD_Element_BatteryLevel    (  0, 7, 0);
    OSD_Element_Capacity        ( 13, 7);
    OSD_Element_Current         (  8, 7);
    OSD_Element_CareFree        (  0, 5);
    OSD_Element_CompassDegree   ( 13, 0, 0);
    OSD_Element_CompassDirection( 18, 0,  0,0);
    OSD_Element_CompassRose     ( 12, 1);
    OSD_Element_FlyingTime      (  0, 1);
    OSD_Element_GroundSpeed     (  0, 0);
    OSD_Element_HomeCircle      ( 16, 4, 5, 0, 0,0);
    OSD_Element_HomeDegree      ( 12, 4);
    OSD_Element_HomeDistance    ( 10, 5, 0);
    OSD_Element_Target          ( 10, 6, 0);
    //OSD_Element_TargetDegree  (  x, y);
    OSD_Element_WayPoint        (  5, 6);
    OSD_Element_LED1Output      (  0, 6);
    OSD_Element_LED2Output      (  3, 6);
    //OSD_Element_Manuell       (  x, y);
    OSD_Element_NaviMode        (  0, 4);
    //OSD_Element_RCIntensity   (  x, y);
    OSD_Element_VarioWert       ( 12, 2);
    OSD_Element_SatsInUse       ( 18, 2, 0);
    OSD_Element_StatusFlags     (  0, 2);
    OSD_Element_Variometer      (  9, 0);
}


//--------------------------------------------------------------
// ehemals: OSD_ALTITUDE,Config.OSD_ScreenMode == 1
//--------------------------------------------------------------
void OSD_Screen_OSD1( void )
{
    //uint8_t xoffs;

    //-----------------------------------------
    // REDRAW static screen elements
    //-----------------------------------------
    if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    {
        // do things here for static screen elements like labels and so....
    }

    //OSD_Element_AltitudeControl( x, y);
    OSD_Element_Altitude        (  1, 1, 1);
    OSD_Element_BatteryLevel    (  0, 7, 1);
    OSD_Element_Capacity        ( 13, 7);
    OSD_Element_Current         (  8, 7);
    //OSD_Element_CareFree      (  x, y);
    OSD_Element_CompassDegree   ( 13, 0, 1);
    OSD_Element_CompassDirection( 18, 0,  0,0);
    OSD_Element_CompassRose     ( 12, 1);
    OSD_Element_FlyingTime      (  7, 6);
    OSD_Element_GroundSpeed     (  0, 0);
    OSD_Element_HomeCircle      (  1, 3, 7, 0, 0,0);
    OSD_Element_HomeDegree      (  8, 3);
    OSD_Element_HomeDistance    (  7, 2, 1);
    //OSD_Element_Target        (  x, y, 1);
    //OSD_Element_TargetDegree  (  x, y);
    //OSD_Element_WayPoint      (  x, y);
    //OSD_Element_LED1Output    (  x, y);
    //OSD_Element_LED2Output    (  x, y);
    //OSD_Element_Manuell       (  x, y);
    OSD_Element_NaviMode        (  8, 5);
    OSD_Element_RCIntensity     ( 15, 6);
    OSD_Element_VarioWert       ( 15, 2);
    OSD_Element_SatsInUse       (  8, 4, 1);
    //OSD_Element_StatusFlags   (  x, y);
    OSD_Element_Variometer      (  9, 0);
}


//--------------------------------------------------------------
// ehemals: OSD_ALTITUDE,Config.OSD_ScreenMode == 2
//--------------------------------------------------------------
void OSD_Screen_OSD2( void )
{
    //uint8_t xoffs;

    //-----------------------------------------
    // REDRAW static screen elements
    //-----------------------------------------
    if( OSDScreenRefresh == OSD_SCREEN_REDRAW )
    {
        // do things here for static screen elements like labels and so....
    }

    OSD_Element_AltitudeControl ( 0, 1);
    OSD_Element_Altitude        (  0, 4, 2);
    OSD_Element_BatteryLevel    ( 13, 7, 2);
    OSD_Element_Capacity        (  0, 7);
    OSD_Element_Current         (  8, 7);
    OSD_Element_CareFree        (  0, 3);
    OSD_Element_CompassDegree   ( 12, 3, 2);
    //OSD_Element_CompassDirection( x, y,  0,0);
    //OSD_Element_CompassRose   ( x, y);
    OSD_Element_FlyingTime      ( 15, 5);
    //OSD_Element_GroundSpeed   (  x, y);
    OSD_Element_HomeCircle      ( 16, 0, 5, 0, 0,0);
    OSD_Element_HomeDegree      ( 11, 5);
    OSD_Element_HomeDistance    (  0, 5, 2);
    OSD_Element_Target          (  0, 6, 2);
    OSD_Element_TargetDegree    ( 11, 6);
    //OSD_Element_WayPoint      (  x, y);
    OSD_Element_LED1Output      ( 12, 2);
    OSD_Element_LED2Output      ( 14, 2);
    //OSD_Element_Manuell       (  x, y);
    OSD_Element_NaviMode        (  0, 2);
    //OSD_Element_RCIntensity   (  x, y);
    OSD_Element_VarioWert       ( 15, 4);
    OSD_Element_SatsInUse       ( 10, 0, 2);
    OSD_Element_StatusFlags     (  0, 0);
    //OSD_Element_Variometer    (  x, y);
}



#endif  // ifdef: USE_OSD_SCREEN_OLD
