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
//# HISTORY  paramset.c
//#
//# 16-07-2018 Cebra
//# - add: paramset_114 (FC2.21c)
//# - add: Parameter param_ExternalGpsChannel
//#
//# 08.07.2018 Cebra
//#	- add: paramset_113 (FC2.20)
//# - add: param_GimbalOut1Channel, param_GimbalOut2Channel
//#
//# 06.11.2017 Cebra
//#	- chg: paramset_111 -> paramset_112
//# - add: GimbalYawChannel, neuer Parameter während 2.19 Beta
//#
//# 31.10.2017 Cebra
//# - add: param_ExtraConfig_FlyZone_Required ab FC Version 2.18
//# - add: param_GlobalConfig_Landoutside_Flyzone, param_GlobalConfig3_RiseFirstWP
//#
//# 13.10.2017 Cebra
//#	- add: paramset_109 (FC2.16)
//#	- add: paramset_110 (FC2.18)
//#	- add: paramset_111 (FC2.19)
//#	- add: param_FailsafeAltitude
//# - add: param_GlobalConfig_Landoutside_Flyzone
//#
//#
//# 21.02.2016 Cebra
//# - add: #define param_reserved (FC2.14a)
//# - add: paramset_108 (FC2.14a)
//#
//# 18.01.2016 Cebra
//# - add: unsigned char CamCtrlModeChannel; (FC2.13f)
//#        unsigned char CamCtrlZoomChannel; (FC2.13f)
//# - add: paramset_107 (FC2.13a)
//#
//# 16.07.2015 Cebra (PKT385a)
//# - add: unsigned char SingleWpControlChannel; (FC2.11a)
//#        unsigned char MenuKeyChannel; (FC2.11a)
//# - add: paramset_106 (FC2.11a)
//#
//# 09.04.2015 Cebra (PKT384a)
//# - add: unsigned char ServoFS_Pos[5] (FC 2.09i)
//# - add: paramset_105 (FC2.09j)
//#
//# 19.03.2015 Cebra (PKT383a)
//# - add: param_LandingPulse (FC 2.09d)
//#
//# 26.01.2015 Cebra
//# - add: Function transform_ComingHomeOrientation
//#        Ändert die Bits 4+5 für ServoCompInvert
//#        in Konfigurationstabelle der paramSubID:  param_ComingHomeOrientation neu
//#
//# 26.09.2014 Cebra
//# - add: Parameterset Rev. 103  (FC 2.07f)
//# - chg: param_Hoehe_GPS_Z zu param_Hoehe_Verstaerkung im Paramset 103
//#
//# 14.05.2014 OG
//# - chg: include "mkbase.h" geaendert auf "../mk/mkbase.h"
//#
//# 10.05.2014 OG
//# - add: transform_CompassOffset_DisableDeclCalc() - transformiert true/false
//#        in das Byte von param_CompassOffset
//#
//# 09.05.2014 OG
//# - chg: paramSet() - angepasst auf Transform-Funktionen
//# - chg: paramGet() - angepasst auf Transform-Funktionen
//# - add: eine neue Transformations-Zwischenschicht implementiert - abgebildet
//#        ueber die neue Tabelle paramTransform.
//#        Hier koennen Transform-Funktionen hinterlegt werden die einen
//#        ubyte8 Wert veraendern.
//#        Die Transformation wird von paramSet()/paramGet() aufgerufen.
//#
//# 17.04.2014 OG
//# - FIX: _paramset_getsubitemid(): Pruefung von_Revision/bis_Revision
//#        korrigiert - fuehrte vorher ggf. zu falschen Anzeigen von SubItems!
//# - chg: param_BitConfig_MotorOffLed1 auf bis_Revision 101 begrenzt
//# - chg: param_BitConfig_MotorOffLed2 auf bis_Revision 101 begrenzt
//#
//# 09.04.2014 OG
//# - chg: Rev. 101 auf param_DUMMY umgestellt um Platz zu sparen
//#
//# 08.04.2014 OG
//# - add: Parameterset Rev. 102  (FC 2.05g)
//# - fix: _paramset_getsubitemid() - Vergleich bzgl. von/bis_Revision umgedreht
//#         und equal zugefuegt
//#
//# 06.04.2014 OG
//# - add: Parameterset Rev. 101  (FC 2.05f)
//#        -> param_Servo3OnValue, param_Servo3OffValue
//#        -> param_Servo4OnValue, param_Servo4OffValue
//#
//# 28.03.2014 OG
//# - add: Parameterset Rev. 100 (FC 2.05e)
//#        (param_AutoPhotoDistance, param_AutoPhotoAtitudes, param_SingleWpSpeed)
//#
//# 26.03.2014 OG
//# - add: param_CompassOffset_DisableDeclCalc (Sub-Item)
//#
//# 24.03.2014 OG
//# - chg: _paramset_getsubitemid() erweitert Unterstuetzung von/bis FC-Revisionen
//# - add: paramSubItem_t mit Unterstuetzung fuer von/bis FC-Revisionen
//#
//# 27.02.2014 OG
//# - chg: die Revisions-Tabellen 90 bis 94 mittels param_DUMMY gekuerzt
//#        -> keine Parameter-Bearbeitung bei denen moeglich
//# - add: vollstaendige Unterstuetzung von paramSubID's via paramGet(), paramSet()
//#
//# 26.02.2014 OG
//# - add: paramSubItem[] - Zugriffstabelle auf Bit- und Bytefelder innerhalb
//#        einer paramID
//# - chg: bei den paramset_nnn[] Tabellen 'const' ergaenzt
//# - chg: paramsetTest() umbenannt zu paramsetDEBUG()
//#
//# 19.02.2014 OG
//# - fix: paramsetInit() Parameter pData wurde nicht konsequent verwendet
//#
//# 14.02.2014 OG
//# - add: diverse Zugriffsfunktionen fuer paramID's und paramSet's
//# - add: Rev-Tabellen von FC v0.88e bis FC v2.03d
//#
//# 05.02.2014 OG - NEU
//############################################################################


//############################################################################
//# INHALT
//#
//# 1a. Konfigurationtabellen fuer verschiedene FC-Revisionen
//# 1b. Zuweisungstabelle: Revision -> paramset_xxx
//# 2.  Konfigurationstabelle der paramSubID's (Bit- und Bytefelder)
//# 3a. interne Zugriffsfunktionen
//# 3b. oeffentliche Zugriffsfunktionen fuer paramID / paramSubID  Elemente
//# 3c. oeffentliche Zugriffsfunktionen fuer das gesamte Paramset (Int usw.)
//# x.  TEST / DEBUG
//############################################################################


#include "../cpu.h"
#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <string.h>
#include <util/atomic.h>

#include "../main.h"
#include "../lipo/lipo.h"
#include "../lcd/lcd.h"
#include "../uart/usart.h"

#include "../uart/uart1.h"
#include "../mk-data-structs.h"
#include "../timer/timer.h"
#include "../eeprom/eeprom.h"
#include "../messages.h"
#include "../utils/scrollbox.h"
#include "../pkt/pkt.h"
#include "../mk/mkbase.h"
#include "paramset.h"



//---------------------------
// fuer transform_...
//---------------------------
#define GETVALUE      1   // Wert setzen
#define SETVALUE      2   // Wert lesen



paramRevItem_t  *paramsetRevTable;      // Zeiger auf aktive Revision-Table oder 0 wenn nicht gesetzt   -> wird gesetzt druch paramsetInit() / paramset.c
unsigned char   *mkparamset;            // nur temp. gueltig solange der PKT-RX-Buffer (pRxData) noch nicht wieder ueberschrieben -> wird gesetzt druch paramsetInit() / paramset.c



//#############################################################################################
//# 1a. Konfigurationtabellen fuer verschiedene FC-Revisionen
//#############################################################################################


//---------------------------------------------------------------------------------------
// die nachfolgenden Tabellen entsprechen der Struktur von paramRevItem_t aus paramset.h
//---------------------------------------------------------------------------------------


//-----------------------------------------------
// FC-Parameter Revision: 90
//
// ab FC-Version: ab ??? bis ???
// gefunden in  : 0.88e
//
// Tabelle ist definiert in der
// nachfolgenden paramset_091
//
// STRUKTUR-DIFF zu 0:
//
// DEFINE-DIFF zu 0:
//-----------------------------------------------



//-----------------------------------------------
// FC-Parameter Revision: 91
//
// ab FC-Version: ab ??? bis ???
// gefunden in  : 0.88m, 0.88n
//
// STRUKTUR-DIFF zu 090:
// - keine Aenderung der internen Datenstruktur erkennbar!
//
// DEFINE-DIFF zu 090:
// - add: #define PID_SPEAK_HOTT_CFG
// - add: #define CFG3_MOTOR_SWITCH_MODE
// - add: #define CFG3_NO_GPSFIX_NO_START
//-----------------------------------------------
//
//
//    + paramID (paramRevItem_t -> paramID)
//    |
//    |                                + size in Bytes (paramRevItem_t -> size)
//    |                                |
//    |                                |
//    |                                |
paramRevItem_t const paramset_091[] PROGMEM =
{
    { param_DUMMY                   , 111 },    //  n-Bytes Fueller... keine Unterstuetzung fuer paramEdit
    { param_Name                    ,  12 },    //  char Name[12];
    { param_DUMMY                   ,   1 },    //  n-Bytes Fueller... keine Unterstuetzung fuer paramEdit

    { param_EOF                     ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_091[]



//############################################################################################################################################################



//-----------------------------------------------
// FC-Parameter Revision: 92
//
// ab FC-Version: ab ??? bis ???
// gefunden in  : 0.90d
//
// Tabelle ist definiert in der
// nachfolgenden paramset_093
//
// STRUKTUR-DIFF zu 091:
// -add: param_NaviOut1Parameter
//#
// DEFINE-DIFF zu 091:
// -add: #define CFG3_USE_NC_FOR_OUT1     0x20
// -add: #define CFG3_SPEAK_ALL
// -add: #define SERVO_NICK_INV 0x01
// -add: #define SERVO_ROLL_INV 0x02
// -add: #define SERVO_RELATIVE 0x04
//-----------------------------------------------



//-----------------------------------------------
// FC-Parameter Revision: 93
//
// ab FC-Version: ab ??? bis ???
// gefunden in  : 0.90e, 0.90g, 0.90j
//
// STRUKTUR-DIFF zu 092:
// - keine Aenderung der internen Datenstruktur erkennbar!
//
// DEFINE-DIFF zu 092:
// - keine Aenderung erkennbar!
//-----------------------------------------------
paramRevItem_t const paramset_093[] PROGMEM =
{
    { param_DUMMY                   , 112 },    //  n-Bytes Fueller... keine Unterstuetzung fuer paramEdit
    { param_Name                    ,  12 },    //  char Name[12];
    { param_DUMMY                   ,   1 },    //  n-Bytes Fueller... keine Unterstuetzung fuer paramEdit

    { param_EOF                     ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_093[]



//-----------------------------------------------
// FC-Parameter Revision: 94
//
// ab FC-Version: ab ??? bis ???
// gefunden in  : 0.91a
//
// STRUKTUR-DIFF zu 093:
// - add: param_StartLandChannel
// - add: param_LandingSpeed
//
// DEFINE-DIFF zu 093:
// etwas undurchsichtig...
// - add: #define EE_DUMMY
// - add: #define PID_HARDWARE_VERSION
//-----------------------------------------------
paramRevItem_t const paramset_094[] PROGMEM =
{
    { param_DUMMY                   , 114 },    //  n-Bytes Fueller... keine Unterstuetzung fuer paramEdit
    { param_Name                    ,  12 },    //  char Name[12];
    { param_DUMMY                   ,   1 },    //  n-Bytes Fueller... keine Unterstuetzung fuer paramEdit

    { param_EOF                     ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_094[]





//-----------------------------------------------
// FC-Parameter Revision: 95
//
// ab FC-Version: ab ??? bis ???
// gefunden in  : 0.90L, 2.00a, , 2.00e
//
// STRUKTUR-DIFF zu 094:
// - rename: param_MaxHoehe             -> param_HoeheChannel
// - rename: param_NaviGpsModeControl   -> param_NaviGpsModeChannel
// - rename: param_NaviGpsACC           -> param_NaviGpsA
// - rename: param_CareFreeModeControl  -> param_CareFreeChannel
//
// DEFINE-DIFF zu 094:
// - keine Aenderung
//-----------------------------------------------
paramRevItem_t const paramset_095[] PROGMEM =
{
    { param_Revision                ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung           ,  12 },    //  unsigned char Kanalbelegung[12];      // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig            ,   1 },    //  unsigned char GlobalConfig;           // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas            ,   1 },    //  unsigned char Hoehe_MinGas;           // Wert : 0-100
    { param_Luftdruck_D             ,   1 },    //  unsigned char Luftdruck_D;            // Wert : 0-250
    { param_HoeheChannel            ,   1 },    //  unsigned char HoeheChannel;           // Wert : 0-32
    { param_Hoehe_P                 ,   1 },    //  unsigned char Hoehe_P;                // Wert : 0-32
    { param_Hoehe_Verstaerkung      ,   1 },    //  unsigned char Hoehe_Verstaerkung;     // Wert : 0-50
    { param_Hoehe_ACC_Wirkung       ,   1 },    //  unsigned char Hoehe_ACC_Wirkung;      // Wert : 0-250
    { param_Hoehe_HoverBand         ,   1 },    //  unsigned char Hoehe_HoverBand;        // Wert : 0-250
    { param_Hoehe_GPS_Z             ,   1 },    //  unsigned char Hoehe_GPS_Z;            // Wert : 0-250
    { param_Hoehe_StickNeutralPoint ,   1 },    //  unsigned char Hoehe_StickNeutralPoint;// Wert : 0-250
    { param_Stick_P                 ,   1 },    //  unsigned char Stick_P;                // Wert : 1-6
    { param_Stick_D                 ,   1 },    //  unsigned char Stick_D;                // Wert : 0-64
    { param_StickGier_P             ,   1 },    //  unsigned char StickGier_P;            // Wert : 1-20
    { param_Gas_Min                 ,   1 },    //  unsigned char Gas_Min;                // Wert : 0-32
    { param_Gas_Max                 ,   1 },    //  unsigned char Gas_Max;                // Wert : 33-250
    { param_GyroAccFaktor           ,   1 },    //  unsigned char GyroAccFaktor;          // Wert : 1-64
    { param_KompassWirkung          ,   1 },    //  unsigned char KompassWirkung;         // Wert : 0-32
    { param_Gyro_P                  ,   1 },    //  unsigned char Gyro_P;                 // Wert : 10-250
    { param_Gyro_I                  ,   1 },    //  unsigned char Gyro_I;                 // Wert : 0-250
    { param_Gyro_D                  ,   1 },    //  unsigned char Gyro_D;                 // Wert : 0-250
    { param_Gyro_Gier_P             ,   1 },    //  unsigned char Gyro_Gier_P;            // Wert : 10-250
    { param_Gyro_Gier_I             ,   1 },    //  unsigned char Gyro_Gier_I;            // Wert : 0-250
    { param_Gyro_Stability          ,   1 },    //  unsigned char Gyro_Stability;         // Wert : 0-16
    { param_UnterspannungsWarnung   ,   1 },    //  unsigned char UnterspannungsWarnung;  // Wert : 0-250
    { param_NotGas                  ,   1 },    //  unsigned char NotGas;                 // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit              ,   1 },    //  unsigned char NotGasZeit;             // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                ,   1 },    //  unsigned char Receiver;               // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                ,   1 },    //  unsigned char I_Faktor;               // Wert : 0-250
    { param_UserParam1              ,   1 },    //  unsigned char UserParam1;             // Wert : 0-250
    { param_UserParam2              ,   1 },    //  unsigned char UserParam2;             // Wert : 0-250
    { param_UserParam3              ,   1 },    //  unsigned char UserParam3;             // Wert : 0-250
    { param_UserParam4              ,   1 },    //  unsigned char UserParam4;             // Wert : 0-250
    { param_ServoNickControl        ,   1 },    //  unsigned char ServoNickControl;       // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp           ,   1 },    //  unsigned char ServoNickComp;          // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin            ,   1 },    //  unsigned char ServoNickMin;           // Wert : 0-250     // Anschlag
    { param_ServoNickMax            ,   1 },    //  unsigned char ServoNickMax;           // Wert : 0-250     // Anschlag
    { param_ServoRollControl        ,   1 },    //  unsigned char ServoRollControl;       // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp           ,   1 },    //  unsigned char ServoRollComp;          // Wert : 0-250
    { param_ServoRollMin            ,   1 },    //  unsigned char ServoRollMin;           // Wert : 0-250
    { param_ServoRollMax            ,   1 },    //  unsigned char ServoRollMax;           // Wert : 0-250
    { param_ServoNickRefresh        ,   1 },    //  unsigned char ServoNickRefresh;       // Speed of the Servo
    { param_ServoManualControlSpeed ,   1 },    //  unsigned char ServoManualControlSpeed;//
    { param_CamOrientation          ,   1 },    //  unsigned char CamOrientation;         //
    { param_Servo3                  ,   1 },    //  unsigned char Servo3;                 // Value or mapping of the Servo Output
    { param_Servo4                  ,   1 },    //  unsigned char Servo4;                    // Value or mapping of the Servo Output
    { param_Servo5                  ,   1 },    //  unsigned char Servo5;                    // Value or mapping of the Servo Output
    { param_LoopGasLimit            ,   1 },    //  unsigned char LoopGasLimit;           // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold           ,   1 },    //  unsigned char LoopThreshold;          // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese           ,   1 },    //  unsigned char LoopHysterese;          // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1           ,   1 },    //  unsigned char AchsKopplung1;          // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2           ,   1 },    //  unsigned char AchsKopplung2;          // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection   ,   1 },    //  unsigned char CouplingYawCorrection;  // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick      ,   1 },    //  unsigned char WinkelUmschlagNick;     // Wert: 0-250  180° Punkt
    { param_WinkelUmschlagRoll      ,   1 },    //  unsigned char WinkelUmschlagRoll;     // Wert: 0-250  180° Punkt
    { param_GyroAccAbgleich         ,   1 },    //  unsigned char GyroAccAbgleich;        // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp               ,   1 },    //  unsigned char Driftkomp;
    { param_DynamicStability        ,   1 },    //  unsigned char DynamicStability;
    { param_UserParam5              ,   1 },    //  unsigned char UserParam5;             // Wert : 0-250
    { param_UserParam6              ,   1 },    //  unsigned char UserParam6;             // Wert : 0-250
    { param_UserParam7              ,   1 },    //  unsigned char UserParam7;             // Wert : 0-250
    { param_UserParam8              ,   1 },    //  unsigned char UserParam8;             // Wert : 0-250
    { param_J16Bitmask              ,   1 },    //  unsigned char J16Bitmask;             // for the J16 Output
    { param_J16Timing               ,   1 },    //  unsigned char J16Timing;              // for the J16 Output
    { param_J17Bitmask              ,   1 },    //  unsigned char J17Bitmask;             // for the J17 Output
    { param_J17Timing               ,   1 },    //  unsigned char J17Timing;              // for the J17 Output
    { param_WARN_J16_Bitmask        ,   1 },    //  unsigned char WARN_J16_Bitmask;       // for the J16 Output
    { param_WARN_J17_Bitmask        ,   1 },    //  unsigned char WARN_J17_Bitmask;       // for the J17 Output
    { param_NaviOut1Parameter       ,   1 },    //  unsigned char NaviOut1Parameter;      // for the J16 Output
    { param_NaviGpsModeChannel      ,   1 },    //  unsigned char NaviGpsModeChannel;     // Parameters for the Naviboard
    { param_NaviGpsGain             ,   1 },    //  unsigned char NaviGpsGain;
    { param_NaviGpsP                ,   1 },    //  unsigned char NaviGpsP;
    { param_NaviGpsI                ,   1 },    //  unsigned char NaviGpsI;
    { param_NaviGpsD                ,   1 },    //  unsigned char NaviGpsD;
    { param_NaviGpsPLimit           ,   1 },    //  unsigned char NaviGpsPLimit;
    { param_NaviGpsILimit           ,   1 },    //  unsigned char NaviGpsILimit;
    { param_NaviGpsDLimit           ,   1 },    //  unsigned char NaviGpsDLimit;
    { param_NaviGpsA                ,   1 },    //  unsigned char NaviGpsA;
    { param_NaviGpsMinSat           ,   1 },    //  unsigned char NaviGpsMinSat;
    { param_NaviStickThreshold      ,   1 },    //  unsigned char NaviStickThreshold;
    { param_NaviWindCorrection      ,   1 },    //  unsigned char NaviWindCorrection;
    { param_NaviAccCompensation     ,   1 },    //  unsigned char NaviAccCompensation;    // New since 0.86 -> was: SpeedCompensation
    { param_NaviOperatingRadius     ,   1 },    //  unsigned char NaviOperatingRadius;
    { param_NaviAngleLimitation     ,   1 },    //  unsigned char NaviAngleLimitation;
    { param_NaviPH_LoginTime        ,   1 },    //  unsigned char NaviPH_LoginTime;
    { param_ExternalControl         ,   1 },    //  unsigned char ExternalControl;        // for serial Control
    { param_OrientationAngle        ,   1 },    //  unsigned char OrientationAngle;       // Where is the front-direction?
    { param_CareFreeChannel         ,   1 },    //  unsigned char CareFreeChannel;        // switch for CareFree
    { param_MotorSafetySwitch       ,   1 },    //  unsigned char MotorSafetySwitch;
    { param_MotorSmooth             ,   1 },    //  unsigned char MotorSmooth;
    { param_ComingHomeAltitude      ,   1 },    //  unsigned char ComingHomeAltitude;
    { param_FailSafeTime            ,   1 },    //  unsigned char FailSafeTime;
    { param_MaxAltitude             ,   1 },    //  unsigned char MaxAltitude;
    { param_FailsafeChannel         ,   1 },    //  unsigned char FailsafeChannel;        // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick         ,   1 },    //  unsigned char ServoFilterNick;
    { param_ServoFilterRoll         ,   1 },    //  unsigned char ServoFilterRoll;
    { param_StartLandChannel        ,   1 },    //  unsigned char StartLandChannel;
    { param_LandingSpeed            ,   1 },    //  unsigned char LandingSpeed;
    { param_BitConfig               ,   1 },    //  unsigned char BitConfig;              // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert         ,   1 },    //  unsigned char ServoCompInvert;        // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig             ,   1 },    //  unsigned char ExtraConfig;            // bitcodiert
    { param_GlobalConfig3           ,   1 },    //  unsigned char GlobalConfig3;          // bitcodiert
    { param_Name                    ,  12 },    //  char Name[12];
    { param_crc                     ,   1 },    //  unsigned char crc;                    // must be the last byte!

    { param_EOF                     ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_095[]




//-----------------------------------------------
// FC-Parameter Revision: 96
//
// ab FC-Version: ab ??? bis ???
// gefunden in: ???
//
// STRUKTUR-DIFF zu 095:
// - KEINE AENDERUNG der INTERNEN DATEN-STRUKTUR!
//
// DEFINE-DIFF zu 095:
//   GlobalConfig3
//   - add: #define CFG3_SERVO_NICK_COMP_OFF 0x80
//-----------------------------------------------



//-----------------------------------------------
// FC-Parameter Revision: 97
//
// ab FC-Version: ab ??? bis ???
// gefunden in: 2.02b
//
// STRUKTUR-DIFF zu 096:
// - add: param_CompassOffset
// - add: param_AutoLandingVoltage
//
// DEFINE-DIFF zu 096:
// - keine Aenderung
//-----------------------------------------------
paramRevItem_t const paramset_097[] PROGMEM =
{
    { param_Revision                ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung           ,  12 },    //  unsigned char Kanalbelegung[12];      // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig            ,   1 },    //  unsigned char GlobalConfig;           // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas            ,   1 },    //  unsigned char Hoehe_MinGas;           // Wert : 0-100
    { param_Luftdruck_D             ,   1 },    //  unsigned char Luftdruck_D;            // Wert : 0-250
    { param_HoeheChannel            ,   1 },    //  unsigned char HoeheChannel;           // Wert : 0-32
    { param_Hoehe_P                 ,   1 },    //  unsigned char Hoehe_P;                // Wert : 0-32
    { param_Hoehe_Verstaerkung      ,   1 },    //  unsigned char Hoehe_Verstaerkung;     // Wert : 0-50
    { param_Hoehe_ACC_Wirkung       ,   1 },    //  unsigned char Hoehe_ACC_Wirkung;      // Wert : 0-250
    { param_Hoehe_HoverBand         ,   1 },    //  unsigned char Hoehe_HoverBand;        // Wert : 0-250
    { param_Hoehe_GPS_Z             ,   1 },    //  unsigned char Hoehe_GPS_Z;            // Wert : 0-250
    { param_Hoehe_StickNeutralPoint ,   1 },    //  unsigned char Hoehe_StickNeutralPoint;// Wert : 0-250
    { param_Stick_P                 ,   1 },    //  unsigned char Stick_P;                // Wert : 1-6
    { param_Stick_D                 ,   1 },    //  unsigned char Stick_D;                // Wert : 0-64
    { param_StickGier_P             ,   1 },    //  unsigned char StickGier_P;            // Wert : 1-20
    { param_Gas_Min                 ,   1 },    //  unsigned char Gas_Min;                // Wert : 0-32
    { param_Gas_Max                 ,   1 },    //  unsigned char Gas_Max;                // Wert : 33-250
    { param_GyroAccFaktor           ,   1 },    //  unsigned char GyroAccFaktor;          // Wert : 1-64
    { param_KompassWirkung          ,   1 },    //  unsigned char KompassWirkung;         // Wert : 0-32
    { param_Gyro_P                  ,   1 },    //  unsigned char Gyro_P;                 // Wert : 10-250
    { param_Gyro_I                  ,   1 },    //  unsigned char Gyro_I;                 // Wert : 0-250
    { param_Gyro_D                  ,   1 },    //  unsigned char Gyro_D;                 // Wert : 0-250
    { param_Gyro_Gier_P             ,   1 },    //  unsigned char Gyro_Gier_P;            // Wert : 10-250
    { param_Gyro_Gier_I             ,   1 },    //  unsigned char Gyro_Gier_I;            // Wert : 0-250
    { param_Gyro_Stability          ,   1 },    //  unsigned char Gyro_Stability;         // Wert : 0-16
    { param_UnterspannungsWarnung   ,   1 },    //  unsigned char UnterspannungsWarnung;  // Wert : 0-250
    { param_NotGas                  ,   1 },    //  unsigned char NotGas;                 // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit              ,   1 },    //  unsigned char NotGasZeit;             // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                ,   1 },    //  unsigned char Receiver;               // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                ,   1 },    //  unsigned char I_Faktor;               // Wert : 0-250
    { param_UserParam1              ,   1 },    //  unsigned char UserParam1;             // Wert : 0-250
    { param_UserParam2              ,   1 },    //  unsigned char UserParam2;             // Wert : 0-250
    { param_UserParam3              ,   1 },    //  unsigned char UserParam3;             // Wert : 0-250
    { param_UserParam4              ,   1 },    //  unsigned char UserParam4;             // Wert : 0-250
    { param_ServoNickControl        ,   1 },    //  unsigned char ServoNickControl;       // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp           ,   1 },    //  unsigned char ServoNickComp;          // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin            ,   1 },    //  unsigned char ServoNickMin;           // Wert : 0-250     // Anschlag
    { param_ServoNickMax            ,   1 },    //  unsigned char ServoNickMax;           // Wert : 0-250     // Anschlag
    { param_ServoRollControl        ,   1 },    //  unsigned char ServoRollControl;       // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp           ,   1 },    //  unsigned char ServoRollComp;          // Wert : 0-250
    { param_ServoRollMin            ,   1 },    //  unsigned char ServoRollMin;           // Wert : 0-250
    { param_ServoRollMax            ,   1 },    //  unsigned char ServoRollMax;           // Wert : 0-250
    { param_ServoNickRefresh        ,   1 },    //  unsigned char ServoNickRefresh;       // Speed of the Servo
    { param_ServoManualControlSpeed ,   1 },    //  unsigned char ServoManualControlSpeed;//
    { param_CamOrientation          ,   1 },    //  unsigned char CamOrientation;         //
    { param_Servo3                  ,   1 },    //  unsigned char Servo3;                 // Value or mapping of the Servo Output
    { param_Servo4                  ,   1 },    //  unsigned char Servo4;                 // Value or mapping of the Servo Output
    { param_Servo5                  ,   1 },    //  unsigned char Servo5;                 // Value or mapping of the Servo Output
    { param_LoopGasLimit            ,   1 },    //  unsigned char LoopGasLimit;           // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold           ,   1 },    //  unsigned char LoopThreshold;          // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese           ,   1 },    //  unsigned char LoopHysterese;          // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1           ,   1 },    //  unsigned char AchsKopplung1;          // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2           ,   1 },    //  unsigned char AchsKopplung2;          // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection   ,   1 },    //  unsigned char CouplingYawCorrection;  // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick      ,   1 },    //  unsigned char WinkelUmschlagNick;     // Wert: 0-250  180°-Punkt
    { param_WinkelUmschlagRoll      ,   1 },    //  unsigned char WinkelUmschlagRoll;     // Wert: 0-250  180°-Punkt
    { param_GyroAccAbgleich         ,   1 },    //  unsigned char GyroAccAbgleich;        // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp               ,   1 },    //  unsigned char Driftkomp;
    { param_DynamicStability        ,   1 },    //  unsigned char DynamicStability;
    { param_UserParam5              ,   1 },    //  unsigned char UserParam5;             // Wert : 0-250
    { param_UserParam6              ,   1 },    //  unsigned char UserParam6;             // Wert : 0-250
    { param_UserParam7              ,   1 },    //  unsigned char UserParam7;             // Wert : 0-250
    { param_UserParam8              ,   1 },    //  unsigned char UserParam8;             // Wert : 0-250
    { param_J16Bitmask              ,   1 },    //  unsigned char J16Bitmask;             // for the J16 Output
    { param_J16Timing               ,   1 },    //  unsigned char J16Timing;              // for the J16 Output
    { param_J17Bitmask              ,   1 },    //  unsigned char J17Bitmask;             // for the J17 Output
    { param_J17Timing               ,   1 },    //  unsigned char J17Timing;              // for the J17 Output
    { param_WARN_J16_Bitmask        ,   1 },    //  unsigned char WARN_J16_Bitmask;       // for the J16 Output
    { param_WARN_J17_Bitmask        ,   1 },    //  unsigned char WARN_J17_Bitmask;       // for the J17 Output
    { param_NaviOut1Parameter       ,   1 },    //  unsigned char NaviOut1Parameter;      // for the J16 Output
    { param_NaviGpsModeChannel      ,   1 },    //  unsigned char NaviGpsModeChannel;     // Parameters for the Naviboard
    { param_NaviGpsGain             ,   1 },    //  unsigned char NaviGpsGain;
    { param_NaviGpsP                ,   1 },    //  unsigned char NaviGpsP;
    { param_NaviGpsI                ,   1 },    //  unsigned char NaviGpsI;
    { param_NaviGpsD                ,   1 },    //  unsigned char NaviGpsD;
    { param_NaviGpsPLimit           ,   1 },    //  unsigned char NaviGpsPLimit;
    { param_NaviGpsILimit           ,   1 },    //  unsigned char NaviGpsILimit;
    { param_NaviGpsDLimit           ,   1 },    //  unsigned char NaviGpsDLimit;
    { param_NaviGpsA                ,   1 },    //  unsigned char NaviGpsA;
    { param_NaviGpsMinSat           ,   1 },    //  unsigned char NaviGpsMinSat;
    { param_NaviStickThreshold      ,   1 },    //  unsigned char NaviStickThreshold;
    { param_NaviWindCorrection      ,   1 },    //  unsigned char NaviWindCorrection;
    { param_NaviAccCompensation     ,   1 },    //  unsigned char NaviAccCompensation;    // New since 0.86 -> was: SpeedCompensation
    { param_NaviOperatingRadius     ,   1 },    //  unsigned char NaviOperatingRadius;
    { param_NaviAngleLimitation     ,   1 },    //  unsigned char NaviAngleLimitation;
    { param_NaviPH_LoginTime        ,   1 },    //  unsigned char NaviPH_LoginTime;
    { param_ExternalControl         ,   1 },    //  unsigned char ExternalControl;        // for serial Control
    { param_OrientationAngle        ,   1 },    //  unsigned char OrientationAngle;       // Where is the front-direction?
    { param_CareFreeChannel         ,   1 },    //  unsigned char CareFreeChannel;        // switch for CareFree
    { param_MotorSafetySwitch       ,   1 },    //  unsigned char MotorSafetySwitch;
    { param_MotorSmooth             ,   1 },    //  unsigned char MotorSmooth;
    { param_ComingHomeAltitude      ,   1 },    //  unsigned char ComingHomeAltitude;
    { param_FailSafeTime            ,   1 },    //  unsigned char FailSafeTime;
    { param_MaxAltitude             ,   1 },    //  unsigned char MaxAltitude;
    { param_FailsafeChannel         ,   1 },    //  unsigned char FailsafeChannel;        // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick         ,   1 },    //  unsigned char ServoFilterNick;
    { param_ServoFilterRoll         ,   1 },    //  unsigned char ServoFilterRoll;
    { param_StartLandChannel        ,   1 },    //  unsigned char StartLandChannel;
    { param_LandingSpeed            ,   1 },    //  unsigned char LandingSpeed;
    { param_CompassOffset           ,   1 },    //  unsigned char CompassOffset;
    { param_AutoLandingVoltage      ,   1 },    //  unsigned char AutoLandingVoltage;     // in 0,1V  0 -> disabled
    { param_BitConfig               ,   1 },    //  unsigned char BitConfig;              // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert         ,   1 },    //  unsigned char ServoCompInvert;        // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig             ,   1 },    //  unsigned char ExtraConfig;            // bitcodiert
    { param_GlobalConfig3           ,   1 },    //  unsigned char GlobalConfig3;          // bitcodiert
    { param_Name                    ,  12 },    //  char Name[12];
    { param_crc                     ,   1 },    //  unsigned char crc;                    // must be the last byte!

    { param_EOF                     ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_097[]




//-----------------------------------------------
// FC-Parameter Revision: 98
//
// ab FC-Version: ab ??? bis ???
// gefunden in: 2.03d
//
// STRUKTUR-DIFF zu 097:
// - add: param_ComingHomeVoltage
//
// DEFINE-DIFF zu 097:
// - keine Aenderung
//-----------------------------------------------
paramRevItem_t const paramset_098[] PROGMEM =
{
    { param_Revision                ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung           ,  12 },    //  unsigned char Kanalbelegung[12];      // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig            ,   1 },    //  unsigned char GlobalConfig;           // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas            ,   1 },    //  unsigned char Hoehe_MinGas;           // Wert : 0-100
    { param_Luftdruck_D             ,   1 },    //  unsigned char Luftdruck_D;            // Wert : 0-250
    { param_HoeheChannel            ,   1 },    //  unsigned char HoeheChannel;           // Wert : 0-32
    { param_Hoehe_P                 ,   1 },    //  unsigned char Hoehe_P;                // Wert : 0-32
    { param_Hoehe_Verstaerkung      ,   1 },    //  unsigned char Hoehe_Verstaerkung;     // Wert : 0-50
    { param_Hoehe_ACC_Wirkung       ,   1 },    //  unsigned char Hoehe_ACC_Wirkung;      // Wert : 0-250
    { param_Hoehe_HoverBand         ,   1 },    //  unsigned char Hoehe_HoverBand;        // Wert : 0-250
    { param_Hoehe_GPS_Z             ,   1 },    //  unsigned char Hoehe_GPS_Z;            // Wert : 0-250
    { param_Hoehe_StickNeutralPoint ,   1 },    //  unsigned char Hoehe_StickNeutralPoint;// Wert : 0-250
    { param_Stick_P                 ,   1 },    //  unsigned char Stick_P;                // Wert : 1-6
    { param_Stick_D                 ,   1 },    //  unsigned char Stick_D;                // Wert : 0-64
    { param_StickGier_P             ,   1 },    //  unsigned char StickGier_P;            // Wert : 1-20
    { param_Gas_Min                 ,   1 },    //  unsigned char Gas_Min;                // Wert : 0-32
    { param_Gas_Max                 ,   1 },    //  unsigned char Gas_Max;                // Wert : 33-250
    { param_GyroAccFaktor           ,   1 },    //  unsigned char GyroAccFaktor;          // Wert : 1-64
    { param_KompassWirkung          ,   1 },    //  unsigned char KompassWirkung;         // Wert : 0-32
    { param_Gyro_P                  ,   1 },    //  unsigned char Gyro_P;                 // Wert : 10-250
    { param_Gyro_I                  ,   1 },    //  unsigned char Gyro_I;                 // Wert : 0-250
    { param_Gyro_D                  ,   1 },    //  unsigned char Gyro_D;                 // Wert : 0-250
    { param_Gyro_Gier_P             ,   1 },    //  unsigned char Gyro_Gier_P;            // Wert : 10-250
    { param_Gyro_Gier_I             ,   1 },    //  unsigned char Gyro_Gier_I;            // Wert : 0-250
    { param_Gyro_Stability          ,   1 },    //  unsigned char Gyro_Stability;         // Wert : 0-16
    { param_UnterspannungsWarnung   ,   1 },    //  unsigned char UnterspannungsWarnung;  // Wert : 0-250
    { param_NotGas                  ,   1 },    //  unsigned char NotGas;                 // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit              ,   1 },    //  unsigned char NotGasZeit;             // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                ,   1 },    //  unsigned char Receiver;               // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                ,   1 },    //  unsigned char I_Faktor;               // Wert : 0-250
    { param_UserParam1              ,   1 },    //  unsigned char UserParam1;             // Wert : 0-250
    { param_UserParam2              ,   1 },    //  unsigned char UserParam2;             // Wert : 0-250
    { param_UserParam3              ,   1 },    //  unsigned char UserParam3;             // Wert : 0-250
    { param_UserParam4              ,   1 },    //  unsigned char UserParam4;             // Wert : 0-250
    { param_ServoNickControl        ,   1 },    //  unsigned char ServoNickControl;       // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp           ,   1 },    //  unsigned char ServoNickComp;          // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin            ,   1 },    //  unsigned char ServoNickMin;           // Wert : 0-250     // Anschlag
    { param_ServoNickMax            ,   1 },    //  unsigned char ServoNickMax;           // Wert : 0-250     // Anschlag
    { param_ServoRollControl        ,   1 },    //  unsigned char ServoRollControl;       // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp           ,   1 },    //  unsigned char ServoRollComp;          // Wert : 0-250
    { param_ServoRollMin            ,   1 },    //  unsigned char ServoRollMin;           // Wert : 0-250
    { param_ServoRollMax            ,   1 },    //  unsigned char ServoRollMax;           // Wert : 0-250
    { param_ServoNickRefresh        ,   1 },    //  unsigned char ServoNickRefresh;       // Speed of the Servo
    { param_ServoManualControlSpeed ,   1 },    //  unsigned char ServoManualControlSpeed;//
    { param_CamOrientation          ,   1 },    //  unsigned char CamOrientation;         //
    { param_Servo3                  ,   1 },    //  unsigned char Servo3;                 // Value or mapping of the Servo Output
    { param_Servo4                  ,   1 },    //  unsigned char Servo4;                 // Value or mapping of the Servo Output
    { param_Servo5                  ,   1 },    //  unsigned char Servo5;                 // Value or mapping of the Servo Output
    { param_LoopGasLimit            ,   1 },    //  unsigned char LoopGasLimit;           // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold           ,   1 },    //  unsigned char LoopThreshold;          // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese           ,   1 },    //  unsigned char LoopHysterese;          // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1           ,   1 },    //  unsigned char AchsKopplung1;          // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2           ,   1 },    //  unsigned char AchsKopplung2;          // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection   ,   1 },    //  unsigned char CouplingYawCorrection;  // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick      ,   1 },    //  unsigned char WinkelUmschlagNick;     // Wert: 0-250  180°-Punkt
    { param_WinkelUmschlagRoll      ,   1 },    //  unsigned char WinkelUmschlagRoll;     // Wert: 0-250  180°-Punkt
    { param_GyroAccAbgleich         ,   1 },    //  unsigned char GyroAccAbgleich;        // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp               ,   1 },    //  unsigned char Driftkomp;
    { param_DynamicStability        ,   1 },    //  unsigned char DynamicStability;
    { param_UserParam5              ,   1 },    //  unsigned char UserParam5;             // Wert : 0-250
    { param_UserParam6              ,   1 },    //  unsigned char UserParam6;             // Wert : 0-250
    { param_UserParam7              ,   1 },    //  unsigned char UserParam7;             // Wert : 0-250
    { param_UserParam8              ,   1 },    //  unsigned char UserParam8;             // Wert : 0-250
    { param_J16Bitmask              ,   1 },    //  unsigned char J16Bitmask;             // for the J16 Output
    { param_J16Timing               ,   1 },    //  unsigned char J16Timing;              // for the J16 Output
    { param_J17Bitmask              ,   1 },    //  unsigned char J17Bitmask;             // for the J17 Output
    { param_J17Timing               ,   1 },    //  unsigned char J17Timing;              // for the J17 Output
    { param_WARN_J16_Bitmask        ,   1 },    //  unsigned char WARN_J16_Bitmask;       // for the J16 Output
    { param_WARN_J17_Bitmask        ,   1 },    //  unsigned char WARN_J17_Bitmask;       // for the J17 Output
    { param_NaviOut1Parameter       ,   1 },    //  unsigned char NaviOut1Parameter;      // for the J16 Output
    { param_NaviGpsModeChannel      ,   1 },    //  unsigned char NaviGpsModeChannel;     // Parameters for the Naviboard
    { param_NaviGpsGain             ,   1 },    //  unsigned char NaviGpsGain;
    { param_NaviGpsP                ,   1 },    //  unsigned char NaviGpsP;
    { param_NaviGpsI                ,   1 },    //  unsigned char NaviGpsI;
    { param_NaviGpsD                ,   1 },    //  unsigned char NaviGpsD;
    { param_NaviGpsPLimit           ,   1 },    //  unsigned char NaviGpsPLimit;
    { param_NaviGpsILimit           ,   1 },    //  unsigned char NaviGpsILimit;
    { param_NaviGpsDLimit           ,   1 },    //  unsigned char NaviGpsDLimit;
    { param_NaviGpsA                ,   1 },    //  unsigned char NaviGpsA;
    { param_NaviGpsMinSat           ,   1 },    //  unsigned char NaviGpsMinSat;
    { param_NaviStickThreshold      ,   1 },    //  unsigned char NaviStickThreshold;
    { param_NaviWindCorrection      ,   1 },    //  unsigned char NaviWindCorrection;
    { param_NaviAccCompensation     ,   1 },    //  unsigned char NaviAccCompensation;    // New since 0.86 -> was: SpeedCompensation
    { param_NaviOperatingRadius     ,   1 },    //  unsigned char NaviOperatingRadius;
    { param_NaviAngleLimitation     ,   1 },    //  unsigned char NaviAngleLimitation;
    { param_NaviPH_LoginTime        ,   1 },    //  unsigned char NaviPH_LoginTime;
    { param_ExternalControl         ,   1 },    //  unsigned char ExternalControl;        // for serial Control
    { param_OrientationAngle        ,   1 },    //  unsigned char OrientationAngle;       // Where is the front-direction?
    { param_CareFreeChannel         ,   1 },    //  unsigned char CareFreeChannel;        // switch for CareFree
    { param_MotorSafetySwitch       ,   1 },    //  unsigned char MotorSafetySwitch;
    { param_MotorSmooth             ,   1 },    //  unsigned char MotorSmooth;
    { param_ComingHomeAltitude      ,   1 },    //  unsigned char ComingHomeAltitude;
    { param_FailSafeTime            ,   1 },    //  unsigned char FailSafeTime;
    { param_MaxAltitude             ,   1 },    //  unsigned char MaxAltitude;
    { param_FailsafeChannel         ,   1 },    //  unsigned char FailsafeChannel;        // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick         ,   1 },    //  unsigned char ServoFilterNick;
    { param_ServoFilterRoll         ,   1 },    //  unsigned char ServoFilterRoll;
    { param_StartLandChannel        ,   1 },    //  unsigned char StartLandChannel;
    { param_LandingSpeed            ,   1 },    //  unsigned char LandingSpeed;
    { param_CompassOffset           ,   1 },    //  unsigned char CompassOffset;
    { param_AutoLandingVoltage      ,   1 },    //  unsigned char AutoLandingVoltage;     // in 0,1V  0 -> disabled
    { param_ComingHomeVoltage       ,   1 },    //  unsigned char ComingHomeVoltage;      // in 0,1V  0 -> disabled
    { param_BitConfig               ,   1 },    //  unsigned char BitConfig;              // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert         ,   1 },    //  unsigned char ServoCompInvert;        // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig             ,   1 },    //  unsigned char ExtraConfig;            // bitcodiert
    { param_GlobalConfig3           ,   1 },    //  unsigned char GlobalConfig3;          // bitcodiert
    { param_Name                    ,  12 },    //  char Name[12];
    { param_crc                     ,   1 },    //  unsigned char crc;                    // must be the last byte!

    { param_EOF                     ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_098[]



//-----------------------------------------------
// FC-Parameter Revision: 99
// keine Inormationen zu Rev. 99 vorhanden.
//
// Kommentar dazu von Holger Buss:
// "Die Revision 0.99 kannst Du ignorieren."
//-----------------------------------------------



//-----------------------------------------------
// FC-Parameter Revision: 100
//
// ab FC-Version: ab ??? bis 2.05e
// gefunden in: 2.05e
//
// STRUKTUR-DIFF zu 098:
// - del: NaviOut1Parameter
// - add: AutoPhotoDistance  (ersetzt NaviOut1Parameter)
// - add: AutoPhotoAtitudes
// - add: SingleWpSpeed
//
// DEFINE-DIFF zu 098:
// - keine Aenderung
//
// ANMERKUNG OG 06.04.2014:
// Die Tabelle kann ggf. demnaechst geloescht werden
// da sie nur fuer eine kurze Zeit in Betaversionen
// vorhanden war!
//-----------------------------------------------
paramRevItem_t const paramset_100[] PROGMEM =
{
    { param_Revision                   ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung              ,  12 },    //  unsigned char char Kanalbelegung[12];        // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig               ,   1 },    //  unsigned char char GlobalConfig;             // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas               ,   1 },    //  unsigned char char Hoehe_MinGas;             // Wert : 0-100
    { param_Luftdruck_D                ,   1 },    //  unsigned char char Luftdruck_D;              // Wert : 0-250
    { param_HoeheChannel               ,   1 },    //  unsigned char char HoeheChannel;             // Wert : 0-32
    { param_Hoehe_P                    ,   1 },    //  unsigned char char Hoehe_P;                  // Wert : 0-32
    { param_Hoehe_Verstaerkung         ,   1 },    //  unsigned char char Hoehe_Verstaerkung;       // Wert : 0-50
    { param_Hoehe_ACC_Wirkung          ,   1 },    //  unsigned char char Hoehe_ACC_Wirkung;        // Wert : 0-250
    { param_Hoehe_HoverBand            ,   1 },    //  unsigned char char Hoehe_HoverBand;          // Wert : 0-250
    { param_Hoehe_GPS_Z                ,   1 },    //  unsigned char char Hoehe_GPS_Z;              // Wert : 0-250
    { param_Hoehe_StickNeutralPoint    ,   1 },    //  unsigned char char Hoehe_StickNeutralPoint;  // Wert : 0-250
    { param_Stick_P                    ,   1 },    //  unsigned char char Stick_P;                  // Wert : 1-6
    { param_Stick_D                    ,   1 },    //  unsigned char char Stick_D;                  // Wert : 0-64
    { param_StickGier_P                ,   1 },    //  unsigned char char StickGier_P;              // Wert : 1-20
    { param_Gas_Min                    ,   1 },    //  unsigned char char Gas_Min;                  // Wert : 0-32
    { param_Gas_Max                    ,   1 },    //  unsigned char char Gas_Max;                  // Wert : 33-250
    { param_GyroAccFaktor              ,   1 },    //  unsigned char char GyroAccFaktor;            // Wert : 1-64
    { param_KompassWirkung             ,   1 },    //  unsigned char char KompassWirkung;           // Wert : 0-32
    { param_Gyro_P                     ,   1 },    //  unsigned char char Gyro_P;                   // Wert : 10-250
    { param_Gyro_I                     ,   1 },    //  unsigned char char Gyro_I;                   // Wert : 0-250
    { param_Gyro_D                     ,   1 },    //  unsigned char char Gyro_D;                   // Wert : 0-250
    { param_Gyro_Gier_P                ,   1 },    //  unsigned char char Gyro_Gier_P;              // Wert : 10-250
    { param_Gyro_Gier_I                ,   1 },    //  unsigned char char Gyro_Gier_I;              // Wert : 0-250
    { param_Gyro_Stability             ,   1 },    //  unsigned char char Gyro_Stability;           // Wert : 0-16
    { param_UnterspannungsWarnung      ,   1 },    //  unsigned char char UnterspannungsWarnung;    // Wert : 0-250
    { param_NotGas                     ,   1 },    //  unsigned char char NotGas;                   // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit                 ,   1 },    //  unsigned char char NotGasZeit;               // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                   ,   1 },    //  unsigned char char Receiver;                 // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                   ,   1 },    //  unsigned char char I_Faktor;                 // Wert : 0-250
    { param_UserParam1                 ,   1 },    //  unsigned char char UserParam1;               // Wert : 0-250
    { param_UserParam2                 ,   1 },    //  unsigned char char UserParam2;               // Wert : 0-250
    { param_UserParam3                 ,   1 },    //  unsigned char char UserParam3;               // Wert : 0-250
    { param_UserParam4                 ,   1 },    //  unsigned char char UserParam4;               // Wert : 0-250
    { param_ServoNickControl           ,   1 },    //  unsigned char char ServoNickControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp              ,   1 },    //  unsigned char char ServoNickComp;            // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin               ,   1 },    //  unsigned char char ServoNickMin;             // Wert : 0-250     // Anschlag
    { param_ServoNickMax               ,   1 },    //  unsigned char char ServoNickMax;             // Wert : 0-250     // Anschlag
    { param_ServoRollControl           ,   1 },    //  unsigned char char ServoRollControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp              ,   1 },    //  unsigned char char ServoRollComp;            // Wert : 0-250
    { param_ServoRollMin               ,   1 },    //  unsigned char char ServoRollMin;             // Wert : 0-250
    { param_ServoRollMax               ,   1 },    //  unsigned char char ServoRollMax;             // Wert : 0-250
    { param_ServoNickRefresh           ,   1 },    //  unsigned char char ServoNickRefresh;         // Speed of the Servo
    { param_ServoManualControlSpeed    ,   1 },    //  unsigned char char ServoManualControlSpeed;  //
    { param_CamOrientation             ,   1 },    //  unsigned char char CamOrientation;           //
    { param_Servo3                     ,   1 },    //  unsigned char char Servo3;                   // Value or mapping of the Servo Output
    { param_Servo4                     ,   1 },    //  unsigned char char Servo4;                   // Value or mapping of the Servo Output
    { param_Servo5                     ,   1 },    //  unsigned char char Servo5;                   // Value or mapping of the Servo Output
    { param_LoopGasLimit               ,   1 },    //  unsigned char char LoopGasLimit;             // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold              ,   1 },    //  unsigned char char LoopThreshold;            // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese              ,   1 },    //  unsigned char char LoopHysterese;            // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1              ,   1 },    //  unsigned char char AchsKopplung1;            // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2              ,   1 },    //  unsigned char char AchsKopplung2;            // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection      ,   1 },    //  unsigned char char CouplingYawCorrection;    // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick         ,   1 },    //  unsigned char char WinkelUmschlagNick;       // Wert: 0-250  180°-Punkt
    { param_WinkelUmschlagRoll         ,   1 },    //  unsigned char char WinkelUmschlagRoll;       // Wert: 0-250  180°-Punkt
    { param_GyroAccAbgleich            ,   1 },    //  unsigned char char GyroAccAbgleich;          // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp                  ,   1 },    //  unsigned char char Driftkomp;
    { param_DynamicStability           ,   1 },    //  unsigned char char DynamicStability;
    { param_UserParam5                 ,   1 },    //  unsigned char char UserParam5;               // Wert : 0-250
    { param_UserParam6                 ,   1 },    //  unsigned char char UserParam6;               // Wert : 0-250
    { param_UserParam7                 ,   1 },    //  unsigned char char UserParam7;               // Wert : 0-250
    { param_UserParam8                 ,   1 },    //  unsigned char char UserParam8;               // Wert : 0-250
    { param_J16Bitmask                 ,   1 },    //  unsigned char char J16Bitmask;               // for the J16 Output
    { param_J16Timing                  ,   1 },    //  unsigned char char J16Timing;                // for the J16 Output
    { param_J17Bitmask                 ,   1 },    //  unsigned char char J17Bitmask;               // for the J17 Output
    { param_J17Timing                  ,   1 },    //  unsigned char char J17Timing;                // for the J17 Output
    { param_WARN_J16_Bitmask           ,   1 },    //  unsigned char char WARN_J16_Bitmask;         // for the J16 Output
    { param_WARN_J17_Bitmask           ,   1 },    //  unsigned char char WARN_J17_Bitmask;         // for the J17 Output
    { param_AutoPhotoDistance          ,   1 },    //  unsigned char char AutoPhotoDistance;        // Auto Photo   // ab Rev. 100 (ehemals NaviOut1Parameter)
    { param_NaviGpsModeChannel         ,   1 },    //  unsigned char char NaviGpsModeChannel;       // Parameters for the Naviboard
    { param_NaviGpsGain                ,   1 },    //  unsigned char char NaviGpsGain;
    { param_NaviGpsP                   ,   1 },    //  unsigned char char NaviGpsP;
    { param_NaviGpsI                   ,   1 },    //  unsigned char char NaviGpsI;
    { param_NaviGpsD                   ,   1 },    //  unsigned char char NaviGpsD;
    { param_NaviGpsPLimit              ,   1 },    //  unsigned char char NaviGpsPLimit;
    { param_NaviGpsILimit              ,   1 },    //  unsigned char char NaviGpsILimit;
    { param_NaviGpsDLimit              ,   1 },    //  unsigned char char NaviGpsDLimit;
    { param_NaviGpsA                   ,   1 },    //  unsigned char char NaviGpsA;
    { param_NaviGpsMinSat              ,   1 },    //  unsigned char char NaviGpsMinSat;
    { param_NaviStickThreshold         ,   1 },    //  unsigned char char NaviStickThreshold;
    { param_NaviWindCorrection         ,   1 },    //  unsigned char char NaviWindCorrection;
    { param_NaviAccCompensation        ,   1 },    //  unsigned char char NaviAccCompensation;      // New since 0.86 -> was: SpeedCompensation
    { param_NaviOperatingRadius        ,   1 },    //  unsigned char char NaviOperatingRadius;
    { param_NaviAngleLimitation        ,   1 },    //  unsigned char char NaviAngleLimitation;
    { param_NaviPH_LoginTime           ,   1 },    //  unsigned char char NaviPH_LoginTime;
    { param_ExternalControl            ,   1 },    //  unsigned char char ExternalControl;          // for serial Control
    { param_OrientationAngle           ,   1 },    //  unsigned char char OrientationAngle;         // Where is the front-direction?
    { param_CareFreeChannel            ,   1 },    //  unsigned char char CareFreeChannel;          // switch for CareFree
    { param_MotorSafetySwitch          ,   1 },    //  unsigned char char MotorSafetySwitch;
    { param_MotorSmooth                ,   1 },    //  unsigned char char MotorSmooth;
    { param_ComingHomeAltitude         ,   1 },    //  unsigned char char ComingHomeAltitude;
    { param_FailSafeTime               ,   1 },    //  unsigned char char FailSafeTime;
    { param_MaxAltitude                ,   1 },    //  unsigned char char MaxAltitude;
    { param_FailsafeChannel            ,   1 },    //  unsigned char char FailsafeChannel;          // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick            ,   1 },    //  unsigned char char ServoFilterNick;
    { param_ServoFilterRoll            ,   1 },    //  unsigned char char ServoFilterRoll;
    { param_StartLandChannel           ,   1 },    //  unsigned char char StartLandChannel;
    { param_LandingSpeed               ,   1 },    //  unsigned char char LandingSpeed;
    { param_CompassOffset              ,   1 },    //  unsigned char char CompassOffset;
    { param_AutoLandingVoltage         ,   1 },    //  unsigned char char AutoLandingVoltage;       // in 0,1V  0 -> disabled
    { param_ComingHomeVoltage          ,   1 },    //  unsigned char char ComingHomeVoltage;        // in 0,1V  0 -> disabled
    { param_AutoPhotoAtitudes          ,   1 },    //  unsigned char char AutoPhotoAtitudes;        // ab Rev. 100
    { param_SingleWpSpeed              ,   1 },    //  unsigned char char SingleWpSpeed;            // ab Rev. 100
    { param_BitConfig                  ,   1 },    //  unsigned char char BitConfig;                // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert            ,   1 },    //  unsigned char char ServoCompInvert;          // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig                ,   1 },    //  unsigned char char ExtraConfig;              // bitcodiert
    { param_GlobalConfig3              ,   1 },    //  unsigned char char GlobalConfig3;            // bitcodiert
    { param_Name                       ,  12 },    //  char               Name[12];
    { param_crc                        ,   1 },    //  unsigned char char crc;                      // must be the last byte!

    { param_EOF                        ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_100[]




//-----------------------------------------------
// FC-Parameter Revision: 101
//
// ab FC-Version: ab 2.05f bis 2.05f
// gefunden in: 2.05f
//
// STRUKTUR-DIFF zu 100:
// - add: Servo3OnValue
// - add: Servo3OffValue
// - add: Servo4OnValue
// - add: Servo4OffValue
//
// DEFINE-DIFF zu 100:
// - keine Aenderung
//
// ANMERKUNG OG 06.04.2014:
// Die Tabelle kann ggf. demnaechst geloescht werden
// da sie nur fuer eine einzige Betaversion
// vorhanden war!
//-----------------------------------------------
paramRevItem_t const paramset_101[] PROGMEM =
{
    { param_DUMMY                      , 123 },    //  n-Bytes Fueller... keine Unterstuetzung fuer paramEdit
    { param_Name                       ,  12 },    //  char Name[12];
    { param_DUMMY                      ,   1 },    //  n-Bytes Fueller... keine Unterstuetzung fuer paramEdit

    { param_EOF                        ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_101[]





//-----------------------------------------------
// FC-Parameter Revision: 102
//
// ab FC-Version: ab 2.05g bis ??? (min. 2.06b)
// gefunden in: 2.05g, 2.06a
//
// STRUKTUR-DIFF zu 101:
// - add: NaviMaxFlyingRange    (ersetzt NaviOperatingRadius)
// - add: NaviDescendRange
// - del: NaviOperatingRadius
//
// DEFINE-DIFF zu 101:
// - del: CFG3_DPH_MAX_RADIUS
//-----------------------------------------------
paramRevItem_t const paramset_102[] PROGMEM =
{
    { param_Revision                   ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung              ,  12 },    //  unsigned char char Kanalbelegung[12];        // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig               ,   1 },    //  unsigned char char GlobalConfig;             // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas               ,   1 },    //  unsigned char char Hoehe_MinGas;             // Wert : 0-100
    { param_Luftdruck_D                ,   1 },    //  unsigned char char Luftdruck_D;              // Wert : 0-250
    { param_HoeheChannel               ,   1 },    //  unsigned char char HoeheChannel;             // Wert : 0-32
    { param_Hoehe_P                    ,   1 },    //  unsigned char char Hoehe_P;                  // Wert : 0-32
    { param_Hoehe_Verstaerkung         ,   1 },    //  unsigned char char Hoehe_Verstaerkung;       // Wert : 0-50
    { param_Hoehe_ACC_Wirkung          ,   1 },    //  unsigned char char Hoehe_ACC_Wirkung;        // Wert : 0-250
    { param_Hoehe_HoverBand            ,   1 },    //  unsigned char char Hoehe_HoverBand;          // Wert : 0-250
    { param_Hoehe_GPS_Z                ,   1 },    //  unsigned char char Hoehe_GPS_Z;              // Wert : 0-250
    { param_Hoehe_StickNeutralPoint    ,   1 },    //  unsigned char char Hoehe_StickNeutralPoint;  // Wert : 0-250
    { param_Stick_P                    ,   1 },    //  unsigned char char Stick_P;                  // Wert : 1-6
    { param_Stick_D                    ,   1 },    //  unsigned char char Stick_D;                  // Wert : 0-64
    { param_StickGier_P                ,   1 },    //  unsigned char char StickGier_P;              // Wert : 1-20
    { param_Gas_Min                    ,   1 },    //  unsigned char char Gas_Min;                  // Wert : 0-32
    { param_Gas_Max                    ,   1 },    //  unsigned char char Gas_Max;                  // Wert : 33-250
    { param_GyroAccFaktor              ,   1 },    //  unsigned char char GyroAccFaktor;            // Wert : 1-64
    { param_KompassWirkung             ,   1 },    //  unsigned char char KompassWirkung;           // Wert : 0-32
    { param_Gyro_P                     ,   1 },    //  unsigned char char Gyro_P;                   // Wert : 10-250
    { param_Gyro_I                     ,   1 },    //  unsigned char char Gyro_I;                   // Wert : 0-250
    { param_Gyro_D                     ,   1 },    //  unsigned char char Gyro_D;                   // Wert : 0-250
    { param_Gyro_Gier_P                ,   1 },    //  unsigned char char Gyro_Gier_P;              // Wert : 10-250
    { param_Gyro_Gier_I                ,   1 },    //  unsigned char char Gyro_Gier_I;              // Wert : 0-250
    { param_Gyro_Stability             ,   1 },    //  unsigned char char Gyro_Stability;           // Wert : 0-16
    { param_UnterspannungsWarnung      ,   1 },    //  unsigned char char UnterspannungsWarnung;    // Wert : 0-250
    { param_NotGas                     ,   1 },    //  unsigned char char NotGas;                   // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit                 ,   1 },    //  unsigned char char NotGasZeit;               // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                   ,   1 },    //  unsigned char char Receiver;                 // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                   ,   1 },    //  unsigned char char I_Faktor;                 // Wert : 0-250
    { param_UserParam1                 ,   1 },    //  unsigned char char UserParam1;               // Wert : 0-250
    { param_UserParam2                 ,   1 },    //  unsigned char char UserParam2;               // Wert : 0-250
    { param_UserParam3                 ,   1 },    //  unsigned char char UserParam3;               // Wert : 0-250
    { param_UserParam4                 ,   1 },    //  unsigned char char UserParam4;               // Wert : 0-250
    { param_ServoNickControl           ,   1 },    //  unsigned char char ServoNickControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp              ,   1 },    //  unsigned char char ServoNickComp;            // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin               ,   1 },    //  unsigned char char ServoNickMin;             // Wert : 0-250     // Anschlag
    { param_ServoNickMax               ,   1 },    //  unsigned char char ServoNickMax;             // Wert : 0-250     // Anschlag
    { param_ServoRollControl           ,   1 },    //  unsigned char char ServoRollControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp              ,   1 },    //  unsigned char char ServoRollComp;            // Wert : 0-250
    { param_ServoRollMin               ,   1 },    //  unsigned char char ServoRollMin;             // Wert : 0-250
    { param_ServoRollMax               ,   1 },    //  unsigned char char ServoRollMax;             // Wert : 0-250
    { param_ServoNickRefresh           ,   1 },    //  unsigned char char ServoNickRefresh;         // Speed of the Servo
    { param_ServoManualControlSpeed    ,   1 },    //  unsigned char char ServoManualControlSpeed;  //
    { param_CamOrientation             ,   1 },    //  unsigned char char CamOrientation;           //
    { param_Servo3                     ,   1 },    //  unsigned char char Servo3;                   // Value or mapping of the Servo Output
    { param_Servo4                     ,   1 },    //  unsigned char char Servo4;                   // Value or mapping of the Servo Output
    { param_Servo5                     ,   1 },    //  unsigned char char Servo5;                   // Value or mapping of the Servo Output
    { param_LoopGasLimit               ,   1 },    //  unsigned char char LoopGasLimit;             // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold              ,   1 },    //  unsigned char char LoopThreshold;            // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese              ,   1 },    //  unsigned char char LoopHysterese;            // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1              ,   1 },    //  unsigned char char AchsKopplung1;            // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2              ,   1 },    //  unsigned char char AchsKopplung2;            // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection      ,   1 },    //  unsigned char char CouplingYawCorrection;    // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick         ,   1 },    //  unsigned char char WinkelUmschlagNick;       // Wert: 0-250  180°-Punkt
    { param_WinkelUmschlagRoll         ,   1 },    //  unsigned char char WinkelUmschlagRoll;       // Wert: 0-250  180°-Punkt
    { param_GyroAccAbgleich            ,   1 },    //  unsigned char char GyroAccAbgleich;          // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp                  ,   1 },    //  unsigned char char Driftkomp;
    { param_DynamicStability           ,   1 },    //  unsigned char char DynamicStability;
    { param_UserParam5                 ,   1 },    //  unsigned char char UserParam5;               // Wert : 0-250
    { param_UserParam6                 ,   1 },    //  unsigned char char UserParam6;               // Wert : 0-250
    { param_UserParam7                 ,   1 },    //  unsigned char char UserParam7;               // Wert : 0-250
    { param_UserParam8                 ,   1 },    //  unsigned char char UserParam8;               // Wert : 0-250
    { param_J16Bitmask                 ,   1 },    //  unsigned char char J16Bitmask;               // for the J16 Output
    { param_J16Timing                  ,   1 },    //  unsigned char char J16Timing;                // for the J16 Output
    { param_J17Bitmask                 ,   1 },    //  unsigned char char J17Bitmask;               // for the J17 Output
    { param_J17Timing                  ,   1 },    //  unsigned char char J17Timing;                // for the J17 Output
    { param_WARN_J16_Bitmask           ,   1 },    //  unsigned char char WARN_J16_Bitmask;         // for the J16 Output
    { param_WARN_J17_Bitmask           ,   1 },    //  unsigned char char WARN_J17_Bitmask;         // for the J17 Output
    { param_AutoPhotoDistance          ,   1 },    //  unsigned char char AutoPhotoDistance;        // Auto Photo   // ab Rev. 100 (ehemals NaviOut1Parameter)
    { param_NaviGpsModeChannel         ,   1 },    //  unsigned char char NaviGpsModeChannel;       // Parameters for the Naviboard
    { param_NaviGpsGain                ,   1 },    //  unsigned char char NaviGpsGain;
    { param_NaviGpsP                   ,   1 },    //  unsigned char char NaviGpsP;
    { param_NaviGpsI                   ,   1 },    //  unsigned char char NaviGpsI;
    { param_NaviGpsD                   ,   1 },    //  unsigned char char NaviGpsD;
    { param_NaviGpsPLimit              ,   1 },    //  unsigned char char NaviGpsPLimit;
    { param_NaviGpsILimit              ,   1 },    //  unsigned char char NaviGpsILimit;
    { param_NaviGpsDLimit              ,   1 },    //  unsigned char char NaviGpsDLimit;
    { param_NaviGpsA                   ,   1 },    //  unsigned char char NaviGpsA;
    { param_NaviGpsMinSat              ,   1 },    //  unsigned char char NaviGpsMinSat;
    { param_NaviStickThreshold         ,   1 },    //  unsigned char char NaviStickThreshold;
    { param_NaviWindCorrection         ,   1 },    //  unsigned char char NaviWindCorrection;
    { param_NaviAccCompensation        ,   1 },    //  unsigned char char NaviAccCompensation;      // New since 0.86 -> was: SpeedCompensation
    { param_NaviMaxFlyingRange         ,   1 },    //  unsigned char char NaviMaxFlyingRange;       // in 10m
    { param_NaviAngleLimitation        ,   1 },    //  unsigned char char NaviAngleLimitation;
    { param_NaviPH_LoginTime           ,   1 },    //  unsigned char char NaviPH_LoginTime;
    { param_NaviDescendRange           ,   1 },    //  unsigned char char NaviDescendRange;
    { param_ExternalControl            ,   1 },    //  unsigned char char ExternalControl;          // for serial Control
    { param_OrientationAngle           ,   1 },    //  unsigned char char OrientationAngle;         // Where is the front-direction?
    { param_CareFreeChannel            ,   1 },    //  unsigned char char CareFreeChannel;          // switch for CareFree
    { param_MotorSafetySwitch          ,   1 },    //  unsigned char char MotorSafetySwitch;
    { param_MotorSmooth                ,   1 },    //  unsigned char char MotorSmooth;
    { param_ComingHomeAltitude         ,   1 },    //  unsigned char char ComingHomeAltitude;
    { param_FailSafeTime               ,   1 },    //  unsigned char char FailSafeTime;
    { param_MaxAltitude                ,   1 },    //  unsigned char char MaxAltitude;
    { param_FailsafeChannel            ,   1 },    //  unsigned char char FailsafeChannel;          // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick            ,   1 },    //  unsigned char char ServoFilterNick;
    { param_ServoFilterRoll            ,   1 },    //  unsigned char char ServoFilterRoll;
    { param_Servo3OnValue              ,   1 },    //  unsigned char char Servo3OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo3OffValue             ,   1 },    //  unsigned char char Servo3OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_Servo4OnValue              ,   1 },    //  unsigned char char Servo4OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo4OffValue             ,   1 },    //  unsigned char char Servo4OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_StartLandChannel           ,   1 },    //  unsigned char char StartLandChannel;
    { param_LandingSpeed               ,   1 },    //  unsigned char char LandingSpeed;
    { param_CompassOffset              ,   1 },    //  unsigned char char CompassOffset;
    { param_AutoLandingVoltage         ,   1 },    //  unsigned char char AutoLandingVoltage;       // in 0,1V  0 -> disabled
    { param_ComingHomeVoltage          ,   1 },    //  unsigned char char ComingHomeVoltage;        // in 0,1V  0 -> disabled
    { param_AutoPhotoAtitudes          ,   1 },    //  unsigned char char AutoPhotoAtitudes;        // ab Rev. 100
    { param_SingleWpSpeed              ,   1 },    //  unsigned char char SingleWpSpeed;            // ab Rev. 100
    { param_BitConfig                  ,   1 },    //  unsigned char char BitConfig;                // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert            ,   1 },    //  unsigned char char ServoCompInvert;          // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig                ,   1 },    //  unsigned char char ExtraConfig;              // bitcodiert
    { param_GlobalConfig3              ,   1 },    //  unsigned char char GlobalConfig3;            // bitcodiert
    { param_Name                       ,  12 },    //  char               Name[12];
    { param_crc                        ,   1 },    //  unsigned char char crc;                      // must be the last byte!

    { param_EOF                        ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_102[]






//-----------------------------------------------
// FC-Parameter Revision: 103
//
// ab FC-Version: ab 2.07f bis ???
// gefunden in: 2.07f
//
// STRUKTUR-DIFF zu 102:
// keine
//
// DEFINE-DIFF zu 102:
// - chg: Hoehe_GPS_Z zu Hoehe_TiltCompensation
//-----------------------------------------------
paramRevItem_t const paramset_103[] PROGMEM =
{
    { param_Revision                   ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung              ,  12 },    //  unsigned char char Kanalbelegung[12];        // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig               ,   1 },    //  unsigned char char GlobalConfig;             // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas               ,   1 },    //  unsigned char char Hoehe_MinGas;             // Wert : 0-100
    { param_Luftdruck_D                ,   1 },    //  unsigned char char Luftdruck_D;              // Wert : 0-250
    { param_HoeheChannel               ,   1 },    //  unsigned char char HoeheChannel;             // Wert : 0-32
    { param_Hoehe_P                    ,   1 },    //  unsigned char char Hoehe_P;                  // Wert : 0-32
    { param_Hoehe_Verstaerkung         ,   1 },    //  unsigned char char Hoehe_Verstaerkung;       // Wert : 0-50
    { param_Hoehe_ACC_Wirkung          ,   1 },    //  unsigned char char Hoehe_ACC_Wirkung;        // Wert : 0-250
    { param_Hoehe_HoverBand            ,   1 },    //  unsigned char char Hoehe_HoverBand;          // Wert : 0-250
    { param_Hoehe_TiltCompensation     ,   1 },    //  unsigned char char Hoehe_TiltCompensation;   // Wert : 0-250
    { param_Hoehe_StickNeutralPoint    ,   1 },    //  unsigned char char Hoehe_StickNeutralPoint;  // Wert : 0-250
    { param_Stick_P                    ,   1 },    //  unsigned char char Stick_P;                  // Wert : 1-6
    { param_Stick_D                    ,   1 },    //  unsigned char char Stick_D;                  // Wert : 0-64
    { param_StickGier_P                ,   1 },    //  unsigned char char StickGier_P;              // Wert : 1-20
    { param_Gas_Min                    ,   1 },    //  unsigned char char Gas_Min;                  // Wert : 0-32
    { param_Gas_Max                    ,   1 },    //  unsigned char char Gas_Max;                  // Wert : 33-250
    { param_GyroAccFaktor              ,   1 },    //  unsigned char char GyroAccFaktor;            // Wert : 1-64
    { param_KompassWirkung             ,   1 },    //  unsigned char char KompassWirkung;           // Wert : 0-32
    { param_Gyro_P                     ,   1 },    //  unsigned char char Gyro_P;                   // Wert : 10-250
    { param_Gyro_I                     ,   1 },    //  unsigned char char Gyro_I;                   // Wert : 0-250
    { param_Gyro_D                     ,   1 },    //  unsigned char char Gyro_D;                   // Wert : 0-250
    { param_Gyro_Gier_P                ,   1 },    //  unsigned char char Gyro_Gier_P;              // Wert : 10-250
    { param_Gyro_Gier_I                ,   1 },    //  unsigned char char Gyro_Gier_I;              // Wert : 0-250
    { param_Gyro_Stability             ,   1 },    //  unsigned char char Gyro_Stability;           // Wert : 0-16
    { param_UnterspannungsWarnung      ,   1 },    //  unsigned char char UnterspannungsWarnung;    // Wert : 0-250
    { param_NotGas                     ,   1 },    //  unsigned char char NotGas;                   // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit                 ,   1 },    //  unsigned char char NotGasZeit;               // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                   ,   1 },    //  unsigned char char Receiver;                 // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                   ,   1 },    //  unsigned char char I_Faktor;                 // Wert : 0-250
    { param_UserParam1                 ,   1 },    //  unsigned char char UserParam1;               // Wert : 0-250
    { param_UserParam2                 ,   1 },    //  unsigned char char UserParam2;               // Wert : 0-250
    { param_UserParam3                 ,   1 },    //  unsigned char char UserParam3;               // Wert : 0-250
    { param_UserParam4                 ,   1 },    //  unsigned char char UserParam4;               // Wert : 0-250
    { param_ServoNickControl           ,   1 },    //  unsigned char char ServoNickControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp              ,   1 },    //  unsigned char char ServoNickComp;            // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin               ,   1 },    //  unsigned char char ServoNickMin;             // Wert : 0-250     // Anschlag
    { param_ServoNickMax               ,   1 },    //  unsigned char char ServoNickMax;             // Wert : 0-250     // Anschlag
    { param_ServoRollControl           ,   1 },    //  unsigned char char ServoRollControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp              ,   1 },    //  unsigned char char ServoRollComp;            // Wert : 0-250
    { param_ServoRollMin               ,   1 },    //  unsigned char char ServoRollMin;             // Wert : 0-250
    { param_ServoRollMax               ,   1 },    //  unsigned char char ServoRollMax;             // Wert : 0-250
    { param_ServoNickRefresh           ,   1 },    //  unsigned char char ServoNickRefresh;         // Speed of the Servo
    { param_ServoManualControlSpeed    ,   1 },    //  unsigned char char ServoManualControlSpeed;  //
    { param_CamOrientation             ,   1 },    //  unsigned char char CamOrientation;           //
    { param_Servo3                     ,   1 },    //  unsigned char char Servo3;                   // Value or mapping of the Servo Output
    { param_Servo4                     ,   1 },    //  unsigned char char Servo4;                   // Value or mapping of the Servo Output
    { param_Servo5                     ,   1 },    //  unsigned char char Servo5;                   // Value or mapping of the Servo Output
    { param_LoopGasLimit               ,   1 },    //  unsigned char char LoopGasLimit;             // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold              ,   1 },    //  unsigned char char LoopThreshold;            // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese              ,   1 },    //  unsigned char char LoopHysterese;            // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1              ,   1 },    //  unsigned char char AchsKopplung1;            // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2              ,   1 },    //  unsigned char char AchsKopplung2;            // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection      ,   1 },    //  unsigned char char CouplingYawCorrection;    // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick         ,   1 },    //  unsigned char char WinkelUmschlagNick;       // Wert: 0-250  180°-Punkt
    { param_WinkelUmschlagRoll         ,   1 },    //  unsigned char char WinkelUmschlagRoll;       // Wert: 0-250  180°-Punkt
    { param_GyroAccAbgleich            ,   1 },    //  unsigned char char GyroAccAbgleich;          // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp                  ,   1 },    //  unsigned char char Driftkomp;
    { param_DynamicStability           ,   1 },    //  unsigned char char DynamicStability;
    { param_UserParam5                 ,   1 },    //  unsigned char char UserParam5;               // Wert : 0-250
    { param_UserParam6                 ,   1 },    //  unsigned char char UserParam6;               // Wert : 0-250
    { param_UserParam7                 ,   1 },    //  unsigned char char UserParam7;               // Wert : 0-250
    { param_UserParam8                 ,   1 },    //  unsigned char char UserParam8;               // Wert : 0-250
    { param_J16Bitmask                 ,   1 },    //  unsigned char char J16Bitmask;               // for the J16 Output
    { param_J16Timing                  ,   1 },    //  unsigned char char J16Timing;                // for the J16 Output
    { param_J17Bitmask                 ,   1 },    //  unsigned char char J17Bitmask;               // for the J17 Output
    { param_J17Timing                  ,   1 },    //  unsigned char char J17Timing;                // for the J17 Output
    { param_WARN_J16_Bitmask           ,   1 },    //  unsigned char char WARN_J16_Bitmask;         // for the J16 Output
    { param_WARN_J17_Bitmask           ,   1 },    //  unsigned char char WARN_J17_Bitmask;         // for the J17 Output
    { param_AutoPhotoDistance          ,   1 },    //  unsigned char char AutoPhotoDistance;        // Auto Photo   // ab Rev. 100 (ehemals NaviOut1Parameter)
    { param_NaviGpsModeChannel         ,   1 },    //  unsigned char char NaviGpsModeChannel;       // Parameters for the Naviboard
    { param_NaviGpsGain                ,   1 },    //  unsigned char char NaviGpsGain;
    { param_NaviGpsP                   ,   1 },    //  unsigned char char NaviGpsP;
    { param_NaviGpsI                   ,   1 },    //  unsigned char char NaviGpsI;
    { param_NaviGpsD                   ,   1 },    //  unsigned char char NaviGpsD;
    { param_NaviGpsPLimit              ,   1 },    //  unsigned char char NaviGpsPLimit;
    { param_NaviGpsILimit              ,   1 },    //  unsigned char char NaviGpsILimit;
    { param_NaviGpsDLimit              ,   1 },    //  unsigned char char NaviGpsDLimit;
    { param_NaviGpsA                   ,   1 },    //  unsigned char char NaviGpsA;
    { param_NaviGpsMinSat              ,   1 },    //  unsigned char char NaviGpsMinSat;
    { param_NaviStickThreshold         ,   1 },    //  unsigned char char NaviStickThreshold;
    { param_NaviWindCorrection         ,   1 },    //  unsigned char char NaviWindCorrection;
    { param_NaviAccCompensation        ,   1 },    //  unsigned char char NaviAccCompensation;      // New since 0.86 -> was: SpeedCompensation
    { param_NaviMaxFlyingRange         ,   1 },    //  unsigned char char NaviMaxFlyingRange;       // in 10m
    { param_NaviAngleLimitation        ,   1 },    //  unsigned char char NaviAngleLimitation;
    { param_NaviPH_LoginTime           ,   1 },    //  unsigned char char NaviPH_LoginTime;
    { param_NaviDescendRange           ,   1 },    //  unsigned char char NaviDescendRange;
    { param_ExternalControl            ,   1 },    //  unsigned char char ExternalControl;          // for serial Control
    { param_OrientationAngle           ,   1 },    //  unsigned char char OrientationAngle;         // Where is the front-direction?
    { param_CareFreeChannel            ,   1 },    //  unsigned char char CareFreeChannel;          // switch for CareFree
    { param_MotorSafetySwitch          ,   1 },    //  unsigned char char MotorSafetySwitch;
    { param_MotorSmooth                ,   1 },    //  unsigned char char MotorSmooth;
    { param_ComingHomeAltitude         ,   1 },    //  unsigned char char ComingHomeAltitude;
    { param_FailSafeTime               ,   1 },    //  unsigned char char FailSafeTime;
    { param_MaxAltitude                ,   1 },    //  unsigned char char MaxAltitude;
    { param_FailsafeChannel            ,   1 },    //  unsigned char char FailsafeChannel;          // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick            ,   1 },    //  unsigned char char ServoFilterNick;
    { param_ServoFilterRoll            ,   1 },    //  unsigned char char ServoFilterRoll;
    { param_Servo3OnValue              ,   1 },    //  unsigned char char Servo3OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo3OffValue             ,   1 },    //  unsigned char char Servo3OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_Servo4OnValue              ,   1 },    //  unsigned char char Servo4OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo4OffValue             ,   1 },    //  unsigned char char Servo4OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_StartLandChannel           ,   1 },    //  unsigned char char StartLandChannel;
    { param_LandingSpeed               ,   1 },    //  unsigned char char LandingSpeed;
    { param_CompassOffset              ,   1 },    //  unsigned char char CompassOffset;
    { param_AutoLandingVoltage         ,   1 },    //  unsigned char char AutoLandingVoltage;       // in 0,1V  0 -> disabled
    { param_ComingHomeVoltage          ,   1 },    //  unsigned char char ComingHomeVoltage;        // in 0,1V  0 -> disabled
    { param_AutoPhotoAtitudes          ,   1 },    //  unsigned char char AutoPhotoAtitudes;        // ab Rev. 100
    { param_SingleWpSpeed              ,   1 },    //  unsigned char char SingleWpSpeed;            // ab Rev. 100
    { param_BitConfig                  ,   1 },    //  unsigned char char BitConfig;                // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert            ,   1 },    //  unsigned char char ServoCompInvert;          // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig                ,   1 },    //  unsigned char char ExtraConfig;              // bitcodiert
    { param_GlobalConfig3              ,   1 },    //  unsigned char char GlobalConfig3;            // bitcodiert
    { param_Name                       ,  12 },    //  char               Name[12];
    { param_crc                        ,   1 },    //  unsigned char char crc;                      // must be the last byte!

    { param_EOF                        ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_103[]



//-----------------------------------------------
// FC-Parameter Revision: 104
//
// ab FC-Version: ab 2.09d bis ???
// gefunden in: 2.09d
//
// STRUKTUR-DIFF zu 103:
// add: LandingPulse
//

//-----------------------------------------------
paramRevItem_t const paramset_104[] PROGMEM =
{
    { param_Revision                   ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung              ,  12 },    //  unsigned char char Kanalbelegung[12];        // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig               ,   1 },    //  unsigned char char GlobalConfig;             // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas               ,   1 },    //  unsigned char char Hoehe_MinGas;             // Wert : 0-100
    { param_Luftdruck_D                ,   1 },    //  unsigned char char Luftdruck_D;              // Wert : 0-250
    { param_HoeheChannel               ,   1 },    //  unsigned char char HoeheChannel;             // Wert : 0-32
    { param_Hoehe_P                    ,   1 },    //  unsigned char char Hoehe_P;                  // Wert : 0-32
    { param_Hoehe_Verstaerkung         ,   1 },    //  unsigned char char Hoehe_Verstaerkung;       // Wert : 0-50
    { param_Hoehe_ACC_Wirkung          ,   1 },    //  unsigned char char Hoehe_ACC_Wirkung;        // Wert : 0-250
    { param_Hoehe_HoverBand            ,   1 },    //  unsigned char char Hoehe_HoverBand;          // Wert : 0-250
    { param_Hoehe_TiltCompensation     ,   1 },    //  unsigned char char Hoehe_TiltCompensation;   // Wert : 0-250
    { param_Hoehe_StickNeutralPoint    ,   1 },    //  unsigned char char Hoehe_StickNeutralPoint;  // Wert : 0-250
    { param_Stick_P                    ,   1 },    //  unsigned char char Stick_P;                  // Wert : 1-6
    { param_Stick_D                    ,   1 },    //  unsigned char char Stick_D;                  // Wert : 0-64
    { param_StickGier_P                ,   1 },    //  unsigned char char StickGier_P;              // Wert : 1-20
    { param_Gas_Min                    ,   1 },    //  unsigned char char Gas_Min;                  // Wert : 0-32
    { param_Gas_Max                    ,   1 },    //  unsigned char char Gas_Max;                  // Wert : 33-250
    { param_GyroAccFaktor              ,   1 },    //  unsigned char char GyroAccFaktor;            // Wert : 1-64
    { param_KompassWirkung             ,   1 },    //  unsigned char char KompassWirkung;           // Wert : 0-32
    { param_Gyro_P                     ,   1 },    //  unsigned char char Gyro_P;                   // Wert : 10-250
    { param_Gyro_I                     ,   1 },    //  unsigned char char Gyro_I;                   // Wert : 0-250
    { param_Gyro_D                     ,   1 },    //  unsigned char char Gyro_D;                   // Wert : 0-250
    { param_Gyro_Gier_P                ,   1 },    //  unsigned char char Gyro_Gier_P;              // Wert : 10-250
    { param_Gyro_Gier_I                ,   1 },    //  unsigned char char Gyro_Gier_I;              // Wert : 0-250
    { param_Gyro_Stability             ,   1 },    //  unsigned char char Gyro_Stability;           // Wert : 0-16
    { param_UnterspannungsWarnung      ,   1 },    //  unsigned char char UnterspannungsWarnung;    // Wert : 0-250
    { param_NotGas                     ,   1 },    //  unsigned char char NotGas;                   // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit                 ,   1 },    //  unsigned char char NotGasZeit;               // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                   ,   1 },    //  unsigned char char Receiver;                 // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                   ,   1 },    //  unsigned char char I_Faktor;                 // Wert : 0-250
    { param_UserParam1                 ,   1 },    //  unsigned char char UserParam1;               // Wert : 0-250
    { param_UserParam2                 ,   1 },    //  unsigned char char UserParam2;               // Wert : 0-250
    { param_UserParam3                 ,   1 },    //  unsigned char char UserParam3;               // Wert : 0-250
    { param_UserParam4                 ,   1 },    //  unsigned char char UserParam4;               // Wert : 0-250
    { param_ServoNickControl           ,   1 },    //  unsigned char char ServoNickControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp              ,   1 },    //  unsigned char char ServoNickComp;            // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin               ,   1 },    //  unsigned char char ServoNickMin;             // Wert : 0-250     // Anschlag
    { param_ServoNickMax               ,   1 },    //  unsigned char char ServoNickMax;             // Wert : 0-250     // Anschlag
    { param_ServoRollControl           ,   1 },    //  unsigned char char ServoRollControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp              ,   1 },    //  unsigned char char ServoRollComp;            // Wert : 0-250
    { param_ServoRollMin               ,   1 },    //  unsigned char char ServoRollMin;             // Wert : 0-250
    { param_ServoRollMax               ,   1 },    //  unsigned char char ServoRollMax;             // Wert : 0-250
    { param_ServoNickRefresh           ,   1 },    //  unsigned char char ServoNickRefresh;         // Speed of the Servo
    { param_ServoManualControlSpeed    ,   1 },    //  unsigned char char ServoManualControlSpeed;  //
    { param_CamOrientation             ,   1 },    //  unsigned char char CamOrientation;           //
    { param_Servo3                     ,   1 },    //  unsigned char char Servo3;                   // Value or mapping of the Servo Output
    { param_Servo4                     ,   1 },    //  unsigned char char Servo4;                   // Value or mapping of the Servo Output
    { param_Servo5                     ,   1 },    //  unsigned char char Servo5;                   // Value or mapping of the Servo Output
    { param_LoopGasLimit               ,   1 },    //  unsigned char char LoopGasLimit;             // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold              ,   1 },    //  unsigned char char LoopThreshold;            // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese              ,   1 },    //  unsigned char char LoopHysterese;            // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1              ,   1 },    //  unsigned char char AchsKopplung1;            // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2              ,   1 },    //  unsigned char char AchsKopplung2;            // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection      ,   1 },    //  unsigned char char CouplingYawCorrection;    // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick         ,   1 },    //  unsigned char char WinkelUmschlagNick;       // Wert: 0-250  180°-Punkt
    { param_WinkelUmschlagRoll         ,   1 },    //  unsigned char char WinkelUmschlagRoll;       // Wert: 0-250  180°-Punkt
    { param_GyroAccAbgleich            ,   1 },    //  unsigned char char GyroAccAbgleich;          // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp                  ,   1 },    //  unsigned char char Driftkomp;
    { param_DynamicStability           ,   1 },    //  unsigned char char DynamicStability;
    { param_UserParam5                 ,   1 },    //  unsigned char char UserParam5;               // Wert : 0-250
    { param_UserParam6                 ,   1 },    //  unsigned char char UserParam6;               // Wert : 0-250
    { param_UserParam7                 ,   1 },    //  unsigned char char UserParam7;               // Wert : 0-250
    { param_UserParam8                 ,   1 },    //  unsigned char char UserParam8;               // Wert : 0-250
    { param_J16Bitmask                 ,   1 },    //  unsigned char char J16Bitmask;               // for the J16 Output
    { param_J16Timing                  ,   1 },    //  unsigned char char J16Timing;                // for the J16 Output
    { param_J17Bitmask                 ,   1 },    //  unsigned char char J17Bitmask;               // for the J17 Output
    { param_J17Timing                  ,   1 },    //  unsigned char char J17Timing;                // for the J17 Output
    { param_WARN_J16_Bitmask           ,   1 },    //  unsigned char char WARN_J16_Bitmask;         // for the J16 Output
    { param_WARN_J17_Bitmask           ,   1 },    //  unsigned char char WARN_J17_Bitmask;         // for the J17 Output
    { param_AutoPhotoDistance          ,   1 },    //  unsigned char char AutoPhotoDistance;        // Auto Photo   // ab Rev. 100 (ehemals NaviOut1Parameter)
    { param_NaviGpsModeChannel         ,   1 },    //  unsigned char char NaviGpsModeChannel;       // Parameters for the Naviboard
    { param_NaviGpsGain                ,   1 },    //  unsigned char char NaviGpsGain;
    { param_NaviGpsP                   ,   1 },    //  unsigned char char NaviGpsP;
    { param_NaviGpsI                   ,   1 },    //  unsigned char char NaviGpsI;
    { param_NaviGpsD                   ,   1 },    //  unsigned char char NaviGpsD;
    { param_NaviGpsPLimit              ,   1 },    //  unsigned char char NaviGpsPLimit;
    { param_NaviGpsILimit              ,   1 },    //  unsigned char char NaviGpsILimit;
    { param_NaviGpsDLimit              ,   1 },    //  unsigned char char NaviGpsDLimit;
    { param_NaviGpsA                   ,   1 },    //  unsigned char char NaviGpsA;
    { param_NaviGpsMinSat              ,   1 },    //  unsigned char char NaviGpsMinSat;
    { param_NaviStickThreshold         ,   1 },    //  unsigned char char NaviStickThreshold;
    { param_NaviWindCorrection         ,   1 },    //  unsigned char char NaviWindCorrection;
    { param_NaviAccCompensation        ,   1 },    //  unsigned char char NaviAccCompensation;      // New since 0.86 -> was: SpeedCompensation
    { param_NaviMaxFlyingRange         ,   1 },    //  unsigned char char NaviMaxFlyingRange;       // in 10m
    { param_NaviAngleLimitation        ,   1 },    //  unsigned char char NaviAngleLimitation;
    { param_NaviPH_LoginTime           ,   1 },    //  unsigned char char NaviPH_LoginTime;
    { param_NaviDescendRange           ,   1 },    //  unsigned char char NaviDescendRange;
    { param_ExternalControl            ,   1 },    //  unsigned char char ExternalControl;          // for serial Control
    { param_OrientationAngle           ,   1 },    //  unsigned char char OrientationAngle;         // Where is the front-direction?
    { param_CareFreeChannel            ,   1 },    //  unsigned char char CareFreeChannel;          // switch for CareFree
    { param_MotorSafetySwitch          ,   1 },    //  unsigned char char MotorSafetySwitch;
    { param_MotorSmooth                ,   1 },    //  unsigned char char MotorSmooth;
    { param_ComingHomeAltitude         ,   1 },    //  unsigned char char ComingHomeAltitude;
    { param_FailSafeTime               ,   1 },    //  unsigned char char FailSafeTime;
    { param_MaxAltitude                ,   1 },    //  unsigned char char MaxAltitude;
    { param_FailsafeChannel            ,   1 },    //  unsigned char char FailsafeChannel;          // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick            ,   1 },    //  unsigned char char ServoFilterNick;
    { param_ServoFilterRoll            ,   1 },    //  unsigned char char ServoFilterRoll;
    { param_Servo3OnValue              ,   1 },    //  unsigned char char Servo3OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo3OffValue             ,   1 },    //  unsigned char char Servo3OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_Servo4OnValue              ,   1 },    //  unsigned char char Servo4OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo4OffValue             ,   1 },    //  unsigned char char Servo4OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_StartLandChannel           ,   1 },    //  unsigned char char StartLandChannel;
    { param_LandingSpeed               ,   1 },    //  unsigned char char LandingSpeed;
    { param_CompassOffset              ,   1 },    //  unsigned char char CompassOffset;
    { param_AutoLandingVoltage         ,   1 },    //  unsigned char char AutoLandingVoltage;       // in 0,1V  0 -> disabled
    { param_ComingHomeVoltage          ,   1 },    //  unsigned char char ComingHomeVoltage;        // in 0,1V  0 -> disabled
    { param_AutoPhotoAtitudes          ,   1 },    //  unsigned char char AutoPhotoAtitudes;        // ab Rev. 100
    { param_SingleWpSpeed              ,   1 },    //  unsigned char char SingleWpSpeed;            // ab Rev. 100
    { param_LandingPulse               ,   1 },    //  unsigned char char LandingPulse;             // ab Rev. 104 (FC 2.09d)
    { param_BitConfig                  ,   1 },    //  unsigned char char BitConfig;                // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert            ,   1 },    //  unsigned char char ServoCompInvert;          // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig                ,   1 },    //  unsigned char char ExtraConfig;              // bitcodiert
    { param_GlobalConfig3              ,   1 },    //  unsigned char char GlobalConfig3;            // bitcodiert
    { param_Name                       ,  12 },    //  char               Name[12];
    { param_crc                        ,   1 },    //  unsigned char char crc;                      // must be the last byte!

    { param_EOF                        ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_104[]

//-----------------------------------------------
// FC-Parameter Revision: 105
//
// ab FC-Version: ab 2.09j bis ???
// gefunden in: 2.09j
//
// STRUKTUR-DIFF zu 104:
// add: unsigned char ServoFS_Pos[5];
//

//-----------------------------------------------
paramRevItem_t const paramset_105[] PROGMEM =
{
    { param_Revision                   ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung              ,  12 },    //  unsigned char char Kanalbelegung[12];        // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig               ,   1 },    //  unsigned char char GlobalConfig;             // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas               ,   1 },    //  unsigned char char Hoehe_MinGas;             // Wert : 0-100
    { param_Luftdruck_D                ,   1 },    //  unsigned char char Luftdruck_D;              // Wert : 0-250
    { param_HoeheChannel               ,   1 },    //  unsigned char char HoeheChannel;             // Wert : 0-32
    { param_Hoehe_P                    ,   1 },    //  unsigned char char Hoehe_P;                  // Wert : 0-32
    { param_Hoehe_Verstaerkung         ,   1 },    //  unsigned char char Hoehe_Verstaerkung;       // Wert : 0-50
    { param_Hoehe_ACC_Wirkung          ,   1 },    //  unsigned char char Hoehe_ACC_Wirkung;        // Wert : 0-250
    { param_Hoehe_HoverBand            ,   1 },    //  unsigned char char Hoehe_HoverBand;          // Wert : 0-250
    { param_Hoehe_TiltCompensation     ,   1 },    //  unsigned char char Hoehe_TiltCompensation;   // Wert : 0-250
    { param_Hoehe_StickNeutralPoint    ,   1 },    //  unsigned char char Hoehe_StickNeutralPoint;  // Wert : 0-250
    { param_Stick_P                    ,   1 },    //  unsigned char char Stick_P;                  // Wert : 1-6
    { param_Stick_D                    ,   1 },    //  unsigned char char Stick_D;                  // Wert : 0-64
    { param_StickGier_P                ,   1 },    //  unsigned char char StickGier_P;              // Wert : 1-20
    { param_Gas_Min                    ,   1 },    //  unsigned char char Gas_Min;                  // Wert : 0-32
    { param_Gas_Max                    ,   1 },    //  unsigned char char Gas_Max;                  // Wert : 33-250
    { param_GyroAccFaktor              ,   1 },    //  unsigned char char GyroAccFaktor;            // Wert : 1-64
    { param_KompassWirkung             ,   1 },    //  unsigned char char KompassWirkung;           // Wert : 0-32
    { param_Gyro_P                     ,   1 },    //  unsigned char char Gyro_P;                   // Wert : 10-250
    { param_Gyro_I                     ,   1 },    //  unsigned char char Gyro_I;                   // Wert : 0-250
    { param_Gyro_D                     ,   1 },    //  unsigned char char Gyro_D;                   // Wert : 0-250
    { param_Gyro_Gier_P                ,   1 },    //  unsigned char char Gyro_Gier_P;              // Wert : 10-250
    { param_Gyro_Gier_I                ,   1 },    //  unsigned char char Gyro_Gier_I;              // Wert : 0-250
    { param_Gyro_Stability             ,   1 },    //  unsigned char char Gyro_Stability;           // Wert : 0-16
    { param_UnterspannungsWarnung      ,   1 },    //  unsigned char char UnterspannungsWarnung;    // Wert : 0-250
    { param_NotGas                     ,   1 },    //  unsigned char char NotGas;                   // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit                 ,   1 },    //  unsigned char char NotGasZeit;               // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                   ,   1 },    //  unsigned char char Receiver;                 // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                   ,   1 },    //  unsigned char char I_Faktor;                 // Wert : 0-250
    { param_UserParam1                 ,   1 },    //  unsigned char char UserParam1;               // Wert : 0-250
    { param_UserParam2                 ,   1 },    //  unsigned char char UserParam2;               // Wert : 0-250
    { param_UserParam3                 ,   1 },    //  unsigned char char UserParam3;               // Wert : 0-250
    { param_UserParam4                 ,   1 },    //  unsigned char char UserParam4;               // Wert : 0-250
    { param_ServoNickControl           ,   1 },    //  unsigned char char ServoNickControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp              ,   1 },    //  unsigned char char ServoNickComp;            // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin               ,   1 },    //  unsigned char char ServoNickMin;             // Wert : 0-250     // Anschlag
    { param_ServoNickMax               ,   1 },    //  unsigned char char ServoNickMax;             // Wert : 0-250     // Anschlag
    { param_ServoRollControl           ,   1 },    //  unsigned char char ServoRollControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp              ,   1 },    //  unsigned char char ServoRollComp;            // Wert : 0-250
    { param_ServoRollMin               ,   1 },    //  unsigned char char ServoRollMin;             // Wert : 0-250
    { param_ServoRollMax               ,   1 },    //  unsigned char char ServoRollMax;             // Wert : 0-250
    { param_ServoNickRefresh           ,   1 },    //  unsigned char char ServoNickRefresh;         // Speed of the Servo
    { param_ServoManualControlSpeed    ,   1 },    //  unsigned char char ServoManualControlSpeed;  //
    { param_CamOrientation             ,   1 },    //  unsigned char char CamOrientation;           //
    { param_Servo3                     ,   1 },    //  unsigned char char Servo3;                   // Value or mapping of the Servo Output
    { param_Servo4                     ,   1 },    //  unsigned char char Servo4;                   // Value or mapping of the Servo Output
    { param_Servo5                     ,   1 },    //  unsigned char char Servo5;                   // Value or mapping of the Servo Output
    { param_LoopGasLimit               ,   1 },    //  unsigned char char LoopGasLimit;             // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold              ,   1 },    //  unsigned char char LoopThreshold;            // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese              ,   1 },    //  unsigned char char LoopHysterese;            // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1              ,   1 },    //  unsigned char char AchsKopplung1;            // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2              ,   1 },    //  unsigned char char AchsKopplung2;            // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection      ,   1 },    //  unsigned char char CouplingYawCorrection;    // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick         ,   1 },    //  unsigned char char WinkelUmschlagNick;       // Wert: 0-250  180°-Punkt
    { param_WinkelUmschlagRoll         ,   1 },    //  unsigned char char WinkelUmschlagRoll;       // Wert: 0-250  180°-Punkt
    { param_GyroAccAbgleich            ,   1 },    //  unsigned char char GyroAccAbgleich;          // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp                  ,   1 },    //  unsigned char char Driftkomp;
    { param_DynamicStability           ,   1 },    //  unsigned char char DynamicStability;
    { param_UserParam5                 ,   1 },    //  unsigned char char UserParam5;               // Wert : 0-250
    { param_UserParam6                 ,   1 },    //  unsigned char char UserParam6;               // Wert : 0-250
    { param_UserParam7                 ,   1 },    //  unsigned char char UserParam7;               // Wert : 0-250
    { param_UserParam8                 ,   1 },    //  unsigned char char UserParam8;               // Wert : 0-250
    { param_J16Bitmask                 ,   1 },    //  unsigned char char J16Bitmask;               // for the J16 Output
    { param_J16Timing                  ,   1 },    //  unsigned char char J16Timing;                // for the J16 Output
    { param_J17Bitmask                 ,   1 },    //  unsigned char char J17Bitmask;               // for the J17 Output
    { param_J17Timing                  ,   1 },    //  unsigned char char J17Timing;                // for the J17 Output
    { param_WARN_J16_Bitmask           ,   1 },    //  unsigned char char WARN_J16_Bitmask;         // for the J16 Output
    { param_WARN_J17_Bitmask           ,   1 },    //  unsigned char char WARN_J17_Bitmask;         // for the J17 Output
    { param_AutoPhotoDistance          ,   1 },    //  unsigned char char AutoPhotoDistance;        // Auto Photo   // ab Rev. 100 (ehemals NaviOut1Parameter)
    { param_NaviGpsModeChannel         ,   1 },    //  unsigned char char NaviGpsModeChannel;       // Parameters for the Naviboard
    { param_NaviGpsGain                ,   1 },    //  unsigned char char NaviGpsGain;
    { param_NaviGpsP                   ,   1 },    //  unsigned char char NaviGpsP;
    { param_NaviGpsI                   ,   1 },    //  unsigned char char NaviGpsI;
    { param_NaviGpsD                   ,   1 },    //  unsigned char char NaviGpsD;
    { param_NaviGpsPLimit              ,   1 },    //  unsigned char char NaviGpsPLimit;
    { param_NaviGpsILimit              ,   1 },    //  unsigned char char NaviGpsILimit;
    { param_NaviGpsDLimit              ,   1 },    //  unsigned char char NaviGpsDLimit;
    { param_NaviGpsA                   ,   1 },    //  unsigned char char NaviGpsA;
    { param_NaviGpsMinSat              ,   1 },    //  unsigned char char NaviGpsMinSat;
    { param_NaviStickThreshold         ,   1 },    //  unsigned char char NaviStickThreshold;
    { param_NaviWindCorrection         ,   1 },    //  unsigned char char NaviWindCorrection;
    { param_NaviAccCompensation        ,   1 },    //  unsigned char char NaviAccCompensation;      // New since 0.86 -> was: SpeedCompensation
    { param_NaviMaxFlyingRange         ,   1 },    //  unsigned char char NaviMaxFlyingRange;       // in 10m
    { param_NaviAngleLimitation        ,   1 },    //  unsigned char char NaviAngleLimitation;
    { param_NaviPH_LoginTime           ,   1 },    //  unsigned char char NaviPH_LoginTime;
    { param_NaviDescendRange           ,   1 },    //  unsigned char char NaviDescendRange;
    { param_ExternalControl            ,   1 },    //  unsigned char char ExternalControl;          // for serial Control
    { param_OrientationAngle           ,   1 },    //  unsigned char char OrientationAngle;         // Where is the front-direction?
    { param_CareFreeChannel            ,   1 },    //  unsigned char char CareFreeChannel;          // switch for CareFree
    { param_MotorSafetySwitch          ,   1 },    //  unsigned char char MotorSafetySwitch;
    { param_MotorSmooth                ,   1 },    //  unsigned char char MotorSmooth;
    { param_ComingHomeAltitude         ,   1 },    //  unsigned char char ComingHomeAltitude;
    { param_FailSafeTime               ,   1 },    //  unsigned char char FailSafeTime;
    { param_MaxAltitude                ,   1 },    //  unsigned char char MaxAltitude;
    { param_FailsafeChannel            ,   1 },    //  unsigned char char FailsafeChannel;          // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick            ,   1 },    //  unsigned char char ServoFilterNick;
    { param_ServoFilterRoll            ,   1 },    //  unsigned char char ServoFilterRoll;
    { param_Servo3OnValue              ,   1 },    //  unsigned char char Servo3OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo3OffValue             ,   1 },    //  unsigned char char Servo3OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_Servo4OnValue              ,   1 },    //  unsigned char char Servo4OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo4OffValue             ,   1 },    //  unsigned char char Servo4OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_ServoFS_Pos                ,   5 },    //  unsigned char ServoFS_Pos[5];                // ab Rev. 105 (FC 2.09i)
    { param_StartLandChannel           ,   1 },    //  unsigned char char StartLandChannel;
    { param_LandingSpeed               ,   1 },    //  unsigned char char LandingSpeed;
    { param_CompassOffset              ,   1 },    //  unsigned char char CompassOffset;
    { param_AutoLandingVoltage         ,   1 },    //  unsigned char char AutoLandingVoltage;       // in 0,1V  0 -> disabled
    { param_ComingHomeVoltage          ,   1 },    //  unsigned char char ComingHomeVoltage;        // in 0,1V  0 -> disabled
    { param_AutoPhotoAtitudes          ,   1 },    //  unsigned char char AutoPhotoAtitudes;        // ab Rev. 100
    { param_SingleWpSpeed              ,   1 },    //  unsigned char char SingleWpSpeed;            // ab Rev. 100
    { param_LandingPulse               ,   1 },    //  unsigned char char LandingPulse;             // ab Rev. 104 (FC 2.09d)
    { param_BitConfig                  ,   1 },    //  unsigned char char BitConfig;                // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert            ,   1 },    //  unsigned char char ServoCompInvert;          // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig                ,   1 },    //  unsigned char char ExtraConfig;              // bitcodiert
    { param_GlobalConfig3              ,   1 },    //  unsigned char char GlobalConfig3;            // bitcodiert
    { param_Name                       ,  12 },    //  char               Name[12];
    { param_crc                        ,   1 },    //  unsigned char char crc;                      // must be the last byte!

    { param_EOF                        ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_105[]


//-----------------------------------------------
// FC-Parameter Revision: 106
//
// ab FC-Version: ab 2.11a bis ???
// gefunden in: 2.11a
//
// STRUKTUR-DIFF zu 105:
// add: unsigned char SingleWpControlChannel;
//      unsigned char MenuKeyChannel;

//

//-----------------------------------------------
paramRevItem_t const paramset_106[] PROGMEM =
{
    { param_Revision                   ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung              ,  12 },    //  unsigned char char Kanalbelegung[12];        // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig               ,   1 },    //  unsigned char char GlobalConfig;             // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas               ,   1 },    //  unsigned char char Hoehe_MinGas;             // Wert : 0-100
    { param_Luftdruck_D                ,   1 },    //  unsigned char char Luftdruck_D;              // Wert : 0-250
    { param_HoeheChannel               ,   1 },    //  unsigned char char HoeheChannel;             // Wert : 0-32
    { param_Hoehe_P                    ,   1 },    //  unsigned char char Hoehe_P;                  // Wert : 0-32
    { param_Hoehe_Verstaerkung         ,   1 },    //  unsigned char char Hoehe_Verstaerkung;       // Wert : 0-50
    { param_Hoehe_ACC_Wirkung          ,   1 },    //  unsigned char char Hoehe_ACC_Wirkung;        // Wert : 0-250
    { param_Hoehe_HoverBand            ,   1 },    //  unsigned char char Hoehe_HoverBand;          // Wert : 0-250
    { param_Hoehe_TiltCompensation     ,   1 },    //  unsigned char char Hoehe_TiltCompensation;   // Wert : 0-250
    { param_Hoehe_StickNeutralPoint    ,   1 },    //  unsigned char char Hoehe_StickNeutralPoint;  // Wert : 0-250
    { param_Stick_P                    ,   1 },    //  unsigned char char Stick_P;                  // Wert : 1-6
    { param_Stick_D                    ,   1 },    //  unsigned char char Stick_D;                  // Wert : 0-64
    { param_StickGier_P                ,   1 },    //  unsigned char char StickGier_P;              // Wert : 1-20
    { param_Gas_Min                    ,   1 },    //  unsigned char char Gas_Min;                  // Wert : 0-32
    { param_Gas_Max                    ,   1 },    //  unsigned char char Gas_Max;                  // Wert : 33-250
    { param_GyroAccFaktor              ,   1 },    //  unsigned char char GyroAccFaktor;            // Wert : 1-64
    { param_KompassWirkung             ,   1 },    //  unsigned char char KompassWirkung;           // Wert : 0-32
    { param_Gyro_P                     ,   1 },    //  unsigned char char Gyro_P;                   // Wert : 10-250
    { param_Gyro_I                     ,   1 },    //  unsigned char char Gyro_I;                   // Wert : 0-250
    { param_Gyro_D                     ,   1 },    //  unsigned char char Gyro_D;                   // Wert : 0-250
    { param_Gyro_Gier_P                ,   1 },    //  unsigned char char Gyro_Gier_P;              // Wert : 10-250
    { param_Gyro_Gier_I                ,   1 },    //  unsigned char char Gyro_Gier_I;              // Wert : 0-250
    { param_Gyro_Stability             ,   1 },    //  unsigned char char Gyro_Stability;           // Wert : 0-16
    { param_UnterspannungsWarnung      ,   1 },    //  unsigned char char UnterspannungsWarnung;    // Wert : 0-250
    { param_NotGas                     ,   1 },    //  unsigned char char NotGas;                   // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit                 ,   1 },    //  unsigned char char NotGasZeit;               // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                   ,   1 },    //  unsigned char char Receiver;                 // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                   ,   1 },    //  unsigned char char I_Faktor;                 // Wert : 0-250
    { param_UserParam1                 ,   1 },    //  unsigned char char UserParam1;               // Wert : 0-250
    { param_UserParam2                 ,   1 },    //  unsigned char char UserParam2;               // Wert : 0-250
    { param_UserParam3                 ,   1 },    //  unsigned char char UserParam3;               // Wert : 0-250
    { param_UserParam4                 ,   1 },    //  unsigned char char UserParam4;               // Wert : 0-250
    { param_ServoNickControl           ,   1 },    //  unsigned char char ServoNickControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp              ,   1 },    //  unsigned char char ServoNickComp;            // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin               ,   1 },    //  unsigned char char ServoNickMin;             // Wert : 0-250     // Anschlag
    { param_ServoNickMax               ,   1 },    //  unsigned char char ServoNickMax;             // Wert : 0-250     // Anschlag
    { param_ServoRollControl           ,   1 },    //  unsigned char char ServoRollControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp              ,   1 },    //  unsigned char char ServoRollComp;            // Wert : 0-250
    { param_ServoRollMin               ,   1 },    //  unsigned char char ServoRollMin;             // Wert : 0-250
    { param_ServoRollMax               ,   1 },    //  unsigned char char ServoRollMax;             // Wert : 0-250
    { param_ServoNickRefresh           ,   1 },    //  unsigned char char ServoNickRefresh;         // Speed of the Servo
    { param_ServoManualControlSpeed    ,   1 },    //  unsigned char char ServoManualControlSpeed;  //
    { param_CamOrientation             ,   1 },    //  unsigned char char CamOrientation;           //
    { param_Servo3                     ,   1 },    //  unsigned char char Servo3;                   // Value or mapping of the Servo Output
    { param_Servo4                     ,   1 },    //  unsigned char char Servo4;                   // Value or mapping of the Servo Output
    { param_Servo5                     ,   1 },    //  unsigned char char Servo5;                   // Value or mapping of the Servo Output
    { param_LoopGasLimit               ,   1 },    //  unsigned char char LoopGasLimit;             // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold              ,   1 },    //  unsigned char char LoopThreshold;            // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese              ,   1 },    //  unsigned char char LoopHysterese;            // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1              ,   1 },    //  unsigned char char AchsKopplung1;            // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2              ,   1 },    //  unsigned char char AchsKopplung2;            // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection      ,   1 },    //  unsigned char char CouplingYawCorrection;    // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick         ,   1 },    //  unsigned char char WinkelUmschlagNick;       // Wert: 0-250  180°-Punkt
    { param_WinkelUmschlagRoll         ,   1 },    //  unsigned char char WinkelUmschlagRoll;       // Wert: 0-250  180°-Punkt
    { param_GyroAccAbgleich            ,   1 },    //  unsigned char char GyroAccAbgleich;          // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp                  ,   1 },    //  unsigned char char Driftkomp;
    { param_DynamicStability           ,   1 },    //  unsigned char char DynamicStability;
    { param_UserParam5                 ,   1 },    //  unsigned char char UserParam5;               // Wert : 0-250
    { param_UserParam6                 ,   1 },    //  unsigned char char UserParam6;               // Wert : 0-250
    { param_UserParam7                 ,   1 },    //  unsigned char char UserParam7;               // Wert : 0-250
    { param_UserParam8                 ,   1 },    //  unsigned char char UserParam8;               // Wert : 0-250
    { param_J16Bitmask                 ,   1 },    //  unsigned char char J16Bitmask;               // for the J16 Output
    { param_J16Timing                  ,   1 },    //  unsigned char char J16Timing;                // for the J16 Output
    { param_J17Bitmask                 ,   1 },    //  unsigned char char J17Bitmask;               // for the J17 Output
    { param_J17Timing                  ,   1 },    //  unsigned char char J17Timing;                // for the J17 Output
    { param_WARN_J16_Bitmask           ,   1 },    //  unsigned char char WARN_J16_Bitmask;         // for the J16 Output
    { param_WARN_J17_Bitmask           ,   1 },    //  unsigned char char WARN_J17_Bitmask;         // for the J17 Output
    { param_AutoPhotoDistance          ,   1 },    //  unsigned char char AutoPhotoDistance;        // Auto Photo   // ab Rev. 100 (ehemals NaviOut1Parameter)
    { param_NaviGpsModeChannel         ,   1 },    //  unsigned char char NaviGpsModeChannel;       // Parameters for the Naviboard
    { param_NaviGpsGain                ,   1 },    //  unsigned char char NaviGpsGain;
    { param_NaviGpsP                   ,   1 },    //  unsigned char char NaviGpsP;
    { param_NaviGpsI                   ,   1 },    //  unsigned char char NaviGpsI;
    { param_NaviGpsD                   ,   1 },    //  unsigned char char NaviGpsD;
    { param_NaviGpsPLimit              ,   1 },    //  unsigned char char NaviGpsPLimit;
    { param_NaviGpsILimit              ,   1 },    //  unsigned char char NaviGpsILimit;
    { param_NaviGpsDLimit              ,   1 },    //  unsigned char char NaviGpsDLimit;
    { param_NaviGpsA                   ,   1 },    //  unsigned char char NaviGpsA;
    { param_NaviGpsMinSat              ,   1 },    //  unsigned char char NaviGpsMinSat;
    { param_NaviStickThreshold         ,   1 },    //  unsigned char char NaviStickThreshold;
    { param_NaviWindCorrection         ,   1 },    //  unsigned char char NaviWindCorrection;
    { param_NaviAccCompensation        ,   1 },    //  unsigned char char NaviAccCompensation;      // New since 0.86 -> was: SpeedCompensation
    { param_NaviMaxFlyingRange         ,   1 },    //  unsigned char char NaviMaxFlyingRange;       // in 10m
    { param_NaviAngleLimitation        ,   1 },    //  unsigned char char NaviAngleLimitation;
    { param_NaviPH_LoginTime           ,   1 },    //  unsigned char char NaviPH_LoginTime;
    { param_NaviDescendRange           ,   1 },    //  unsigned char char NaviDescendRange;
    { param_ExternalControl            ,   1 },    //  unsigned char char ExternalControl;          // for serial Control
    { param_OrientationAngle           ,   1 },    //  unsigned char char OrientationAngle;         // Where is the front-direction?
    { param_CareFreeChannel            ,   1 },    //  unsigned char char CareFreeChannel;          // switch for CareFree
    { param_MotorSafetySwitch          ,   1 },    //  unsigned char char MotorSafetySwitch;
    { param_MotorSmooth                ,   1 },    //  unsigned char char MotorSmooth;
    { param_ComingHomeAltitude         ,   1 },    //  unsigned char char ComingHomeAltitude;
    { param_FailSafeTime               ,   1 },    //  unsigned char char FailSafeTime;
    { param_MaxAltitude                ,   1 },    //  unsigned char char MaxAltitude;
    { param_FailsafeChannel            ,   1 },    //  unsigned char char FailsafeChannel;          // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick            ,   1 },    //  unsigned char char ServoFilterNick;
    { param_ServoFilterRoll            ,   1 },    //  unsigned char char ServoFilterRoll;
    { param_Servo3OnValue              ,   1 },    //  unsigned char char Servo3OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo3OffValue             ,   1 },    //  unsigned char char Servo3OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_Servo4OnValue              ,   1 },    //  unsigned char char Servo4OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo4OffValue             ,   1 },    //  unsigned char char Servo4OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_ServoFS_Pos                ,   5 },    //  unsigned char ServoFS_Pos[5];                // ab Rev. 105 (FC 2.09i)
    { param_StartLandChannel           ,   1 },    //  unsigned char char StartLandChannel;
    { param_LandingSpeed               ,   1 },    //  unsigned char char LandingSpeed;
    { param_CompassOffset              ,   1 },    //  unsigned char char CompassOffset;
    { param_AutoLandingVoltage         ,   1 },    //  unsigned char char AutoLandingVoltage;       // in 0,1V  0 -> disabled
    { param_ComingHomeVoltage          ,   1 },    //  unsigned char char ComingHomeVoltage;        // in 0,1V  0 -> disabled
    { param_AutoPhotoAtitudes          ,   1 },    //  unsigned char char AutoPhotoAtitudes;        // ab Rev. 100
    { param_SingleWpSpeed              ,   1 },    //  unsigned char char SingleWpSpeed;            // ab Rev. 100
    { param_LandingPulse               ,   1 },    //  unsigned char char LandingPulse;             // ab Rev. 104 (FC 2.09d)
    { param_SingleWpControlChannel     ,   1 },    //  unsigned char SingleWpControlChannel;        // ab Rev. 106 (FC2.11a)
    { param_MenuKeyChannel             ,   1 },    //  unsigned char MenuKeyChannel;                // ab Rev. 106 (FC2.11a)
    { param_BitConfig                  ,   1 },    //  unsigned char char BitConfig;                // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert            ,   1 },    //  unsigned char char ServoCompInvert;          // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig                ,   1 },    //  unsigned char char ExtraConfig;              // bitcodiert
    { param_GlobalConfig3              ,   1 },    //  unsigned char char GlobalConfig3;            // bitcodiert
    { param_Name                       ,  12 },    //  char               Name[12];
    { param_crc                        ,   1 },    //  unsigned char char crc;                      // must be the last byte!

    { param_EOF                        ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_106[]

//-----------------------------------------------
// FC-Parameter Revision: 107
//
// ab FC-Version: ab 2.13a bis ???
// gefunden in: 2.13a
//
// STRUKTUR-DIFF zu 106:
// add: unsigned char SingleWpControlChannel;
//      unsigned char MenuKeyChannel;

//

//-----------------------------------------------
paramRevItem_t const paramset_107[] PROGMEM =
{
    { param_Revision                   ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung              ,  12 },    //  unsigned char char Kanalbelegung[12];        // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig               ,   1 },    //  unsigned char char GlobalConfig;             // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas               ,   1 },    //  unsigned char char Hoehe_MinGas;             // Wert : 0-100
    { param_Luftdruck_D                ,   1 },    //  unsigned char char Luftdruck_D;              // Wert : 0-250
    { param_HoeheChannel               ,   1 },    //  unsigned char char HoeheChannel;             // Wert : 0-32
    { param_Hoehe_P                    ,   1 },    //  unsigned char char Hoehe_P;                  // Wert : 0-32
    { param_Hoehe_Verstaerkung         ,   1 },    //  unsigned char char Hoehe_Verstaerkung;       // Wert : 0-50
    { param_Hoehe_ACC_Wirkung          ,   1 },    //  unsigned char char Hoehe_ACC_Wirkung;        // Wert : 0-250
    { param_Hoehe_HoverBand            ,   1 },    //  unsigned char char Hoehe_HoverBand;          // Wert : 0-250
    { param_Hoehe_TiltCompensation     ,   1 },    //  unsigned char char Hoehe_TiltCompensation;   // Wert : 0-250
    { param_Hoehe_StickNeutralPoint    ,   1 },    //  unsigned char char Hoehe_StickNeutralPoint;  // Wert : 0-250
    { param_Stick_P                    ,   1 },    //  unsigned char char Stick_P;                  // Wert : 1-6
    { param_Stick_D                    ,   1 },    //  unsigned char char Stick_D;                  // Wert : 0-64
    { param_StickGier_P                ,   1 },    //  unsigned char char StickGier_P;              // Wert : 1-20
    { param_Gas_Min                    ,   1 },    //  unsigned char char Gas_Min;                  // Wert : 0-32
    { param_Gas_Max                    ,   1 },    //  unsigned char char Gas_Max;                  // Wert : 33-250
    { param_GyroAccFaktor              ,   1 },    //  unsigned char char GyroAccFaktor;            // Wert : 1-64
    { param_KompassWirkung             ,   1 },    //  unsigned char char KompassWirkung;           // Wert : 0-32
    { param_Gyro_P                     ,   1 },    //  unsigned char char Gyro_P;                   // Wert : 10-250
    { param_Gyro_I                     ,   1 },    //  unsigned char char Gyro_I;                   // Wert : 0-250
    { param_Gyro_D                     ,   1 },    //  unsigned char char Gyro_D;                   // Wert : 0-250
    { param_Gyro_Gier_P                ,   1 },    //  unsigned char char Gyro_Gier_P;              // Wert : 10-250
    { param_Gyro_Gier_I                ,   1 },    //  unsigned char char Gyro_Gier_I;              // Wert : 0-250
    { param_Gyro_Stability             ,   1 },    //  unsigned char char Gyro_Stability;           // Wert : 0-16
    { param_UnterspannungsWarnung      ,   1 },    //  unsigned char char UnterspannungsWarnung;    // Wert : 0-250
    { param_NotGas                     ,   1 },    //  unsigned char char NotGas;                   // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit                 ,   1 },    //  unsigned char char NotGasZeit;               // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                   ,   1 },    //  unsigned char char Receiver;                 // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                   ,   1 },    //  unsigned char char I_Faktor;                 // Wert : 0-250
    { param_UserParam1                 ,   1 },    //  unsigned char char UserParam1;               // Wert : 0-250
    { param_UserParam2                 ,   1 },    //  unsigned char char UserParam2;               // Wert : 0-250
    { param_UserParam3                 ,   1 },    //  unsigned char char UserParam3;               // Wert : 0-250
    { param_UserParam4                 ,   1 },    //  unsigned char char UserParam4;               // Wert : 0-250
    { param_ServoNickControl           ,   1 },    //  unsigned char char ServoNickControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp              ,   1 },    //  unsigned char char ServoNickComp;            // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin               ,   1 },    //  unsigned char char ServoNickMin;             // Wert : 0-250     // Anschlag
    { param_ServoNickMax               ,   1 },    //  unsigned char char ServoNickMax;             // Wert : 0-250     // Anschlag
    { param_ServoRollControl           ,   1 },    //  unsigned char char ServoRollControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp              ,   1 },    //  unsigned char char ServoRollComp;            // Wert : 0-250
    { param_ServoRollMin               ,   1 },    //  unsigned char char ServoRollMin;             // Wert : 0-250
    { param_ServoRollMax               ,   1 },    //  unsigned char char ServoRollMax;             // Wert : 0-250
    { param_ServoNickRefresh           ,   1 },    //  unsigned char char ServoNickRefresh;         // Speed of the Servo
    { param_ServoManualControlSpeed    ,   1 },    //  unsigned char char ServoManualControlSpeed;  //
    { param_CamOrientation             ,   1 },    //  unsigned char char CamOrientation;           //
    { param_Servo3                     ,   1 },    //  unsigned char char Servo3;                   // Value or mapping of the Servo Output
    { param_Servo4                     ,   1 },    //  unsigned char char Servo4;                   // Value or mapping of the Servo Output
    { param_Servo5                     ,   1 },    //  unsigned char char Servo5;                   // Value or mapping of the Servo Output
    { param_LoopGasLimit               ,   1 },    //  unsigned char char LoopGasLimit;             // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold              ,   1 },    //  unsigned char char LoopThreshold;            // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese              ,   1 },    //  unsigned char char LoopHysterese;            // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1              ,   1 },    //  unsigned char char AchsKopplung1;            // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2              ,   1 },    //  unsigned char char AchsKopplung2;            // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection      ,   1 },    //  unsigned char char CouplingYawCorrection;    // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick         ,   1 },    //  unsigned char char WinkelUmschlagNick;       // Wert: 0-250  180°-Punkt
    { param_WinkelUmschlagRoll         ,   1 },    //  unsigned char char WinkelUmschlagRoll;       // Wert: 0-250  180°-Punkt
    { param_GyroAccAbgleich            ,   1 },    //  unsigned char char GyroAccAbgleich;          // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp                  ,   1 },    //  unsigned char char Driftkomp;
    { param_DynamicStability           ,   1 },    //  unsigned char char DynamicStability;
    { param_UserParam5                 ,   1 },    //  unsigned char char UserParam5;               // Wert : 0-250
    { param_UserParam6                 ,   1 },    //  unsigned char char UserParam6;               // Wert : 0-250
    { param_UserParam7                 ,   1 },    //  unsigned char char UserParam7;               // Wert : 0-250
    { param_UserParam8                 ,   1 },    //  unsigned char char UserParam8;               // Wert : 0-250
    { param_J16Bitmask                 ,   1 },    //  unsigned char char J16Bitmask;               // for the J16 Output
    { param_J16Timing                  ,   1 },    //  unsigned char char J16Timing;                // for the J16 Output
    { param_J17Bitmask                 ,   1 },    //  unsigned char char J17Bitmask;               // for the J17 Output
    { param_J17Timing                  ,   1 },    //  unsigned char char J17Timing;                // for the J17 Output
    { param_WARN_J16_Bitmask           ,   1 },    //  unsigned char char WARN_J16_Bitmask;         // for the J16 Output
    { param_WARN_J17_Bitmask           ,   1 },    //  unsigned char char WARN_J17_Bitmask;         // for the J17 Output
    { param_AutoPhotoDistance          ,   1 },    //  unsigned char char AutoPhotoDistance;        // Auto Photo   // ab Rev. 100 (ehemals NaviOut1Parameter)
    { param_NaviGpsModeChannel         ,   1 },    //  unsigned char char NaviGpsModeChannel;       // Parameters for the Naviboard
    { param_NaviGpsGain                ,   1 },    //  unsigned char char NaviGpsGain;
    { param_NaviGpsP                   ,   1 },    //  unsigned char char NaviGpsP;
    { param_NaviGpsI                   ,   1 },    //  unsigned char char NaviGpsI;
    { param_NaviGpsD                   ,   1 },    //  unsigned char char NaviGpsD;
    { param_NaviGpsPLimit              ,   1 },    //  unsigned char char NaviGpsPLimit;
    { param_NaviGpsILimit              ,   1 },    //  unsigned char char NaviGpsILimit;
    { param_NaviGpsDLimit              ,   1 },    //  unsigned char char NaviGpsDLimit;
    { param_NaviGpsA                   ,   1 },    //  unsigned char char NaviGpsA;
    { param_NaviGpsMinSat              ,   1 },    //  unsigned char char NaviGpsMinSat;
    { param_NaviStickThreshold         ,   1 },    //  unsigned char char NaviStickThreshold;
    { param_NaviWindCorrection         ,   1 },    //  unsigned char char NaviWindCorrection;
    { param_NaviAccCompensation        ,   1 },    //  unsigned char char NaviAccCompensation;      // New since 0.86 -> was: SpeedCompensation
    { param_NaviMaxFlyingRange         ,   1 },    //  unsigned char char NaviMaxFlyingRange;       // in 10m
    { param_NaviAngleLimitation        ,   1 },    //  unsigned char char NaviAngleLimitation;
    { param_NaviPH_LoginTime           ,   1 },    //  unsigned char char NaviPH_LoginTime;
    { param_NaviDescendRange           ,   1 },    //  unsigned char char NaviDescendRange;
    { param_ExternalControl            ,   1 },    //  unsigned char char ExternalControl;          // for serial Control
    { param_OrientationAngle           ,   1 },    //  unsigned char char OrientationAngle;         // Where is the front-direction?
    { param_CareFreeChannel            ,   1 },    //  unsigned char char CareFreeChannel;          // switch for CareFree
    { param_MotorSafetySwitch          ,   1 },    //  unsigned char char MotorSafetySwitch;
    { param_MotorSmooth                ,   1 },    //  unsigned char char MotorSmooth;
    { param_ComingHomeAltitude         ,   1 },    //  unsigned char char ComingHomeAltitude;
    { param_FailSafeTime               ,   1 },    //  unsigned char char FailSafeTime;
    { param_MaxAltitude                ,   1 },    //  unsigned char char MaxAltitude;
    { param_FailsafeChannel            ,   1 },    //  unsigned char char FailsafeChannel;          // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick            ,   1 },    //  unsigned char char ServoFilterNick;
    { param_ServoFilterRoll            ,   1 },    //  unsigned char char ServoFilterRoll;
    { param_Servo3OnValue              ,   1 },    //  unsigned char char Servo3OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo3OffValue             ,   1 },    //  unsigned char char Servo3OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_Servo4OnValue              ,   1 },    //  unsigned char char Servo4OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo4OffValue             ,   1 },    //  unsigned char char Servo4OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_ServoFS_Pos                ,   5 },    //  unsigned char ServoFS_Pos[5];                // ab Rev. 105 (FC 2.09i)
    { param_StartLandChannel           ,   1 },    //  unsigned char char StartLandChannel;
    { param_LandingSpeed               ,   1 },    //  unsigned char char LandingSpeed;
    { param_CompassOffset              ,   1 },    //  unsigned char char CompassOffset;
    { param_AutoLandingVoltage         ,   1 },    //  unsigned char char AutoLandingVoltage;       // in 0,1V  0 -> disabled
    { param_ComingHomeVoltage          ,   1 },    //  unsigned char char ComingHomeVoltage;        // in 0,1V  0 -> disabled
    { param_AutoPhotoAtitudes          ,   1 },    //  unsigned char char AutoPhotoAtitudes;        // ab Rev. 100
    { param_SingleWpSpeed              ,   1 },    //  unsigned char char SingleWpSpeed;            // ab Rev. 100
    { param_LandingPulse               ,   1 },    //  unsigned char char LandingPulse;             // ab Rev. 104 (FC 2.09d)
    { param_SingleWpControlChannel     ,   1 },    //  unsigned char SingleWpControlChannel;        // ab Rev. 106 (FC2.11a)
    { param_MenuKeyChannel             ,   1 },    //  unsigned char MenuKeyChannel;                // ab Rev. 106 (FC2.11a)
    { param_CamCtrlModeChannel         ,   1 },    //  unsigned char CamCtrlModeChannel;            // ab Rev. 107 (FC2.13f)
    { param_CamCtrlZoomChannel         ,   1 },    //  unsigned char CamCtrlZoomChannel;            // ab Rev. 107 (FC2.13f)
    { param_BitConfig                  ,   1 },    //  unsigned char char BitConfig;                // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert            ,   1 },    //  unsigned char char ServoCompInvert;          // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig                ,   1 },    //  unsigned char char ExtraConfig;              // bitcodiert
    { param_GlobalConfig3              ,   1 },    //  unsigned char char GlobalConfig3;            // bitcodiert
    { param_Name                       ,  12 },    //  char               Name[12];
    { param_crc                        ,   1 },    //  unsigned char char crc;                      // must be the last byte!

    { param_EOF                        ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_107[]




//-----------------------------------------------
// FC-Parameter Revision: 108
//
// ab FC-Version: ab 2.13a bis 2.13?
// gefunden in: 2.13a
//
// STRUKTUR-DIFF zu 107:
// add: unsigned char reserved[32];       // for later use
//

//

//-----------------------------------------------
paramRevItem_t const paramset_108[] PROGMEM =
{
    { param_Revision                   ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung              ,  12 },    //  unsigned char char Kanalbelegung[12];        // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig               ,   1 },    //  unsigned char char GlobalConfig;             // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas               ,   1 },    //  unsigned char char Hoehe_MinGas;             // Wert : 0-100
    { param_Luftdruck_D                ,   1 },    //  unsigned char char Luftdruck_D;              // Wert : 0-250
    { param_HoeheChannel               ,   1 },    //  unsigned char char HoeheChannel;             // Wert : 0-32
    { param_Hoehe_P                    ,   1 },    //  unsigned char char Hoehe_P;                  // Wert : 0-32
    { param_Hoehe_Verstaerkung         ,   1 },    //  unsigned char char Hoehe_Verstaerkung;       // Wert : 0-50
    { param_Hoehe_ACC_Wirkung          ,   1 },    //  unsigned char char Hoehe_ACC_Wirkung;        // Wert : 0-250
    { param_Hoehe_HoverBand            ,   1 },    //  unsigned char char Hoehe_HoverBand;          // Wert : 0-250
    { param_Hoehe_TiltCompensation     ,   1 },    //  unsigned char char Hoehe_TiltCompensation;   // Wert : 0-250
    { param_Hoehe_StickNeutralPoint    ,   1 },    //  unsigned char char Hoehe_StickNeutralPoint;  // Wert : 0-250
    { param_Stick_P                    ,   1 },    //  unsigned char char Stick_P;                  // Wert : 1-6
    { param_Stick_D                    ,   1 },    //  unsigned char char Stick_D;                  // Wert : 0-64
    { param_StickGier_P                ,   1 },    //  unsigned char char StickGier_P;              // Wert : 1-20
    { param_Gas_Min                    ,   1 },    //  unsigned char char Gas_Min;                  // Wert : 0-32
    { param_Gas_Max                    ,   1 },    //  unsigned char char Gas_Max;                  // Wert : 33-250
    { param_GyroAccFaktor              ,   1 },    //  unsigned char char GyroAccFaktor;            // Wert : 1-64
    { param_KompassWirkung             ,   1 },    //  unsigned char char KompassWirkung;           // Wert : 0-32
    { param_Gyro_P                     ,   1 },    //  unsigned char char Gyro_P;                   // Wert : 10-250
    { param_Gyro_I                     ,   1 },    //  unsigned char char Gyro_I;                   // Wert : 0-250
    { param_Gyro_D                     ,   1 },    //  unsigned char char Gyro_D;                   // Wert : 0-250
    { param_Gyro_Gier_P                ,   1 },    //  unsigned char char Gyro_Gier_P;              // Wert : 10-250
    { param_Gyro_Gier_I                ,   1 },    //  unsigned char char Gyro_Gier_I;              // Wert : 0-250
    { param_Gyro_Stability             ,   1 },    //  unsigned char char Gyro_Stability;           // Wert : 0-16
    { param_UnterspannungsWarnung      ,   1 },    //  unsigned char char UnterspannungsWarnung;    // Wert : 0-250
    { param_NotGas                     ,   1 },    //  unsigned char char NotGas;                   // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit                 ,   1 },    //  unsigned char char NotGasZeit;               // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                   ,   1 },    //  unsigned char char Receiver;                 // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                   ,   1 },    //  unsigned char char I_Faktor;                 // Wert : 0-250
    { param_UserParam1                 ,   1 },    //  unsigned char char UserParam1;               // Wert : 0-250
    { param_UserParam2                 ,   1 },    //  unsigned char char UserParam2;               // Wert : 0-250
    { param_UserParam3                 ,   1 },    //  unsigned char char UserParam3;               // Wert : 0-250
    { param_UserParam4                 ,   1 },    //  unsigned char char UserParam4;               // Wert : 0-250
    { param_ServoNickControl           ,   1 },    //  unsigned char char ServoNickControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp              ,   1 },    //  unsigned char char ServoNickComp;            // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin               ,   1 },    //  unsigned char char ServoNickMin;             // Wert : 0-250     // Anschlag
    { param_ServoNickMax               ,   1 },    //  unsigned char char ServoNickMax;             // Wert : 0-250     // Anschlag
    { param_ServoRollControl           ,   1 },    //  unsigned char char ServoRollControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp              ,   1 },    //  unsigned char char ServoRollComp;            // Wert : 0-250
    { param_ServoRollMin               ,   1 },    //  unsigned char char ServoRollMin;             // Wert : 0-250
    { param_ServoRollMax               ,   1 },    //  unsigned char char ServoRollMax;             // Wert : 0-250
    { param_ServoNickRefresh           ,   1 },    //  unsigned char char ServoNickRefresh;         // Speed of the Servo
    { param_ServoManualControlSpeed    ,   1 },    //  unsigned char char ServoManualControlSpeed;  //
    { param_CamOrientation             ,   1 },    //  unsigned char char CamOrientation;           //
    { param_Servo3                     ,   1 },    //  unsigned char char Servo3;                   // Value or mapping of the Servo Output
    { param_Servo4                     ,   1 },    //  unsigned char char Servo4;                   // Value or mapping of the Servo Output
    { param_Servo5                     ,   1 },    //  unsigned char char Servo5;                   // Value or mapping of the Servo Output
    { param_LoopGasLimit               ,   1 },    //  unsigned char char LoopGasLimit;             // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold              ,   1 },    //  unsigned char char LoopThreshold;            // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese              ,   1 },    //  unsigned char char LoopHysterese;            // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1              ,   1 },    //  unsigned char char AchsKopplung1;            // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2              ,   1 },    //  unsigned char char AchsKopplung2;            // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection      ,   1 },    //  unsigned char char CouplingYawCorrection;    // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick         ,   1 },    //  unsigned char char WinkelUmschlagNick;       // Wert: 0-250  180°-Punkt
    { param_WinkelUmschlagRoll         ,   1 },    //  unsigned char char WinkelUmschlagRoll;       // Wert: 0-250  180°-Punkt
    { param_GyroAccAbgleich            ,   1 },    //  unsigned char char GyroAccAbgleich;          // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp                  ,   1 },    //  unsigned char char Driftkomp;
    { param_DynamicStability           ,   1 },    //  unsigned char char DynamicStability;
    { param_UserParam5                 ,   1 },    //  unsigned char char UserParam5;               // Wert : 0-250
    { param_UserParam6                 ,   1 },    //  unsigned char char UserParam6;               // Wert : 0-250
    { param_UserParam7                 ,   1 },    //  unsigned char char UserParam7;               // Wert : 0-250
    { param_UserParam8                 ,   1 },    //  unsigned char char UserParam8;               // Wert : 0-250
    { param_J16Bitmask                 ,   1 },    //  unsigned char char J16Bitmask;               // for the J16 Output
    { param_J16Timing                  ,   1 },    //  unsigned char char J16Timing;                // for the J16 Output
    { param_J17Bitmask                 ,   1 },    //  unsigned char char J17Bitmask;               // for the J17 Output
    { param_J17Timing                  ,   1 },    //  unsigned char char J17Timing;                // for the J17 Output
    { param_WARN_J16_Bitmask           ,   1 },    //  unsigned char char WARN_J16_Bitmask;         // for the J16 Output
    { param_WARN_J17_Bitmask           ,   1 },    //  unsigned char char WARN_J17_Bitmask;         // for the J17 Output
    { param_AutoPhotoDistance          ,   1 },    //  unsigned char char AutoPhotoDistance;        // Auto Photo   // ab Rev. 100 (ehemals NaviOut1Parameter)
    { param_NaviGpsModeChannel         ,   1 },    //  unsigned char char NaviGpsModeChannel;       // Parameters for the Naviboard
    { param_NaviGpsGain                ,   1 },    //  unsigned char char NaviGpsGain;
    { param_NaviGpsP                   ,   1 },    //  unsigned char char NaviGpsP;
    { param_NaviGpsI                   ,   1 },    //  unsigned char char NaviGpsI;
    { param_NaviGpsD                   ,   1 },    //  unsigned char char NaviGpsD;
    { param_NaviGpsPLimit              ,   1 },    //  unsigned char char NaviGpsPLimit;
    { param_NaviGpsILimit              ,   1 },    //  unsigned char char NaviGpsILimit;
    { param_NaviGpsDLimit              ,   1 },    //  unsigned char char NaviGpsDLimit;
    { param_NaviGpsA                   ,   1 },    //  unsigned char char NaviGpsA;
    { param_NaviGpsMinSat              ,   1 },    //  unsigned char char NaviGpsMinSat;
    { param_NaviStickThreshold         ,   1 },    //  unsigned char char NaviStickThreshold;
    { param_NaviWindCorrection         ,   1 },    //  unsigned char char NaviWindCorrection;
    { param_NaviAccCompensation        ,   1 },    //  unsigned char char NaviAccCompensation;      // New since 0.86 -> was: SpeedCompensation
    { param_NaviMaxFlyingRange         ,   1 },    //  unsigned char char NaviMaxFlyingRange;       // in 10m
    { param_NaviAngleLimitation        ,   1 },    //  unsigned char char NaviAngleLimitation;
    { param_NaviPH_LoginTime           ,   1 },    //  unsigned char char NaviPH_LoginTime;
    { param_NaviDescendRange           ,   1 },    //  unsigned char char NaviDescendRange;
    { param_ExternalControl            ,   1 },    //  unsigned char char ExternalControl;          // for serial Control
    { param_OrientationAngle           ,   1 },    //  unsigned char char OrientationAngle;         // Where is the front-direction?
    { param_CareFreeChannel            ,   1 },    //  unsigned char char CareFreeChannel;          // switch for CareFree
    { param_MotorSafetySwitch          ,   1 },    //  unsigned char char MotorSafetySwitch;
    { param_MotorSmooth                ,   1 },    //  unsigned char char MotorSmooth;
    { param_ComingHomeAltitude         ,   1 },    //  unsigned char char ComingHomeAltitude;
    { param_FailSafeTime               ,   1 },    //  unsigned char char FailSafeTime;
    { param_MaxAltitude                ,   1 },    //  unsigned char char MaxAltitude;
    { param_FailsafeChannel            ,   1 },    //  unsigned char char FailsafeChannel;          // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick            ,   1 },    //  unsigned char char ServoFilterNick;
    { param_ServoFilterRoll            ,   1 },    //  unsigned char char ServoFilterRoll;
    { param_Servo3OnValue              ,   1 },    //  unsigned char char Servo3OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo3OffValue             ,   1 },    //  unsigned char char Servo3OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_Servo4OnValue              ,   1 },    //  unsigned char char Servo4OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo4OffValue             ,   1 },    //  unsigned char char Servo4OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_ServoFS_Pos                ,   5 },    //  unsigned char ServoFS_Pos[5];                // ab Rev. 105 (FC 2.09i)
    { param_StartLandChannel           ,   1 },    //  unsigned char char StartLandChannel;
    { param_LandingSpeed               ,   1 },    //  unsigned char char LandingSpeed;
    { param_CompassOffset              ,   1 },    //  unsigned char char CompassOffset;
    { param_AutoLandingVoltage         ,   1 },    //  unsigned char char AutoLandingVoltage;       // in 0,1V  0 -> disabled
    { param_ComingHomeVoltage          ,   1 },    //  unsigned char char ComingHomeVoltage;        // in 0,1V  0 -> disabled
    { param_AutoPhotoAtitudes          ,   1 },    //  unsigned char char AutoPhotoAtitudes;        // ab Rev. 100
    { param_SingleWpSpeed              ,   1 },    //  unsigned char char SingleWpSpeed;            // ab Rev. 100
    { param_LandingPulse               ,   1 },    //  unsigned char char LandingPulse;             // ab Rev. 104 (FC 2.09d)
    { param_SingleWpControlChannel     ,   1 },    //  unsigned char SingleWpControlChannel;        // ab Rev. 106 (FC2.11a)
    { param_MenuKeyChannel             ,   1 },    //  unsigned char MenuKeyChannel;                // ab Rev. 106 (FC2.11a)
    { param_CamCtrlModeChannel         ,   1 },    //  unsigned char CamCtrlModeChannel;            // ab Rev. 107 (FC2.13f)
    { param_CamCtrlZoomChannel         ,   1 },    //  unsigned char CamCtrlZoomChannel;            // ab Rev. 107 (FC2.13f)
	{ param_reserved				   ,  32 },	   //  unsigned char reserved[32];       			// for later use ab Rev. 108 (FC2.14a)
    { param_BitConfig                  ,   1 },    //  unsigned char char BitConfig;                // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert            ,   1 },    //  unsigned char char ServoCompInvert;          // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig                ,   1 },    //  unsigned char char ExtraConfig;              // bitcodiert
    { param_GlobalConfig3              ,   1 },    //  unsigned char char GlobalConfig3;            // bitcodiert
    { param_Name                       ,  12 },    //  char               Name[12];
    { param_crc                        ,   1 },    //  unsigned char char crc;                      // must be the last byte!

    { param_EOF                        ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_108[]

//-----------------------------------------------
// FC-Parameter Revision: 109
//
// ab FC-Version: ab 2.16 bis ?
// gefunden in: 2.16
//
// STRUKTUR-DIFF zu 108:
//
//

//

//-----------------------------------------------
paramRevItem_t const paramset_109[] PROGMEM =
{
    { param_Revision                   ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung              ,  12 },    //  unsigned char char Kanalbelegung[12];        // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig               ,   1 },    //  unsigned char char GlobalConfig;             // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas               ,   1 },    //  unsigned char char Hoehe_MinGas;             // Wert : 0-100
    { param_Luftdruck_D                ,   1 },    //  unsigned char char Luftdruck_D;              // Wert : 0-250
    { param_HoeheChannel               ,   1 },    //  unsigned char char HoeheChannel;             // Wert : 0-32
    { param_Hoehe_P                    ,   1 },    //  unsigned char char Hoehe_P;                  // Wert : 0-32
    { param_Hoehe_Verstaerkung         ,   1 },    //  unsigned char char Hoehe_Verstaerkung;       // Wert : 0-50
    { param_Hoehe_ACC_Wirkung          ,   1 },    //  unsigned char char Hoehe_ACC_Wirkung;        // Wert : 0-250
    { param_Hoehe_HoverBand            ,   1 },    //  unsigned char char Hoehe_HoverBand;          // Wert : 0-250
    { param_Hoehe_TiltCompensation     ,   1 },    //  unsigned char char Hoehe_TiltCompensation;   // Wert : 0-250
    { param_Hoehe_StickNeutralPoint    ,   1 },    //  unsigned char char Hoehe_StickNeutralPoint;  // Wert : 0-250
    { param_Stick_P                    ,   1 },    //  unsigned char char Stick_P;                  // Wert : 1-6
    { param_Stick_D                    ,   1 },    //  unsigned char char Stick_D;                  // Wert : 0-64
    { param_StickGier_P                ,   1 },    //  unsigned char char StickGier_P;              // Wert : 1-20
    { param_Gas_Min                    ,   1 },    //  unsigned char char Gas_Min;                  // Wert : 0-32
    { param_Gas_Max                    ,   1 },    //  unsigned char char Gas_Max;                  // Wert : 33-250
    { param_GyroAccFaktor              ,   1 },    //  unsigned char char GyroAccFaktor;            // Wert : 1-64
    { param_KompassWirkung             ,   1 },    //  unsigned char char KompassWirkung;           // Wert : 0-32
    { param_Gyro_P                     ,   1 },    //  unsigned char char Gyro_P;                   // Wert : 10-250
    { param_Gyro_I                     ,   1 },    //  unsigned char char Gyro_I;                   // Wert : 0-250
    { param_Gyro_D                     ,   1 },    //  unsigned char char Gyro_D;                   // Wert : 0-250
    { param_Gyro_Gier_P                ,   1 },    //  unsigned char char Gyro_Gier_P;              // Wert : 10-250
    { param_Gyro_Gier_I                ,   1 },    //  unsigned char char Gyro_Gier_I;              // Wert : 0-250
    { param_Gyro_Stability             ,   1 },    //  unsigned char char Gyro_Stability;           // Wert : 0-16
    { param_UnterspannungsWarnung      ,   1 },    //  unsigned char char UnterspannungsWarnung;    // Wert : 0-250
    { param_NotGas                     ,   1 },    //  unsigned char char NotGas;                   // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit                 ,   1 },    //  unsigned char char NotGasZeit;               // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                   ,   1 },    //  unsigned char char Receiver;                 // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                   ,   1 },    //  unsigned char char I_Faktor;                 // Wert : 0-250
    { param_UserParam1                 ,   1 },    //  unsigned char char UserParam1;               // Wert : 0-250
    { param_UserParam2                 ,   1 },    //  unsigned char char UserParam2;               // Wert : 0-250
    { param_UserParam3                 ,   1 },    //  unsigned char char UserParam3;               // Wert : 0-250
    { param_UserParam4                 ,   1 },    //  unsigned char char UserParam4;               // Wert : 0-250
    { param_ServoNickControl           ,   1 },    //  unsigned char char ServoNickControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp              ,   1 },    //  unsigned char char ServoNickComp;            // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin               ,   1 },    //  unsigned char char ServoNickMin;             // Wert : 0-250     // Anschlag
    { param_ServoNickMax               ,   1 },    //  unsigned char char ServoNickMax;             // Wert : 0-250     // Anschlag
    { param_ServoRollControl           ,   1 },    //  unsigned char char ServoRollControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp              ,   1 },    //  unsigned char char ServoRollComp;            // Wert : 0-250
    { param_ServoRollMin               ,   1 },    //  unsigned char char ServoRollMin;             // Wert : 0-250
    { param_ServoRollMax               ,   1 },    //  unsigned char char ServoRollMax;             // Wert : 0-250
    { param_ServoNickRefresh           ,   1 },    //  unsigned char char ServoNickRefresh;         // Speed of the Servo
    { param_ServoManualControlSpeed    ,   1 },    //  unsigned char char ServoManualControlSpeed;  //
    { param_CamOrientation             ,   1 },    //  unsigned char char CamOrientation;           //
    { param_Servo3                     ,   1 },    //  unsigned char char Servo3;                   // Value or mapping of the Servo Output
    { param_Servo4                     ,   1 },    //  unsigned char char Servo4;                   // Value or mapping of the Servo Output
    { param_Servo5                     ,   1 },    //  unsigned char char Servo5;                   // Value or mapping of the Servo Output
    { param_LoopGasLimit               ,   1 },    //  unsigned char char LoopGasLimit;             // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold              ,   1 },    //  unsigned char char LoopThreshold;            // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese              ,   1 },    //  unsigned char char LoopHysterese;            // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1              ,   1 },    //  unsigned char char AchsKopplung1;            // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2              ,   1 },    //  unsigned char char AchsKopplung2;            // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection      ,   1 },    //  unsigned char char CouplingYawCorrection;    // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick         ,   1 },    //  unsigned char char WinkelUmschlagNick;       // Wert: 0-250  180°-Punkt
    { param_WinkelUmschlagRoll         ,   1 },    //  unsigned char char WinkelUmschlagRoll;       // Wert: 0-250  180°-Punkt
    { param_GyroAccAbgleich            ,   1 },    //  unsigned char char GyroAccAbgleich;          // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp                  ,   1 },    //  unsigned char char Driftkomp;
    { param_DynamicStability           ,   1 },    //  unsigned char char DynamicStability;
    { param_UserParam5                 ,   1 },    //  unsigned char char UserParam5;               // Wert : 0-250
    { param_UserParam6                 ,   1 },    //  unsigned char char UserParam6;               // Wert : 0-250
    { param_UserParam7                 ,   1 },    //  unsigned char char UserParam7;               // Wert : 0-250
    { param_UserParam8                 ,   1 },    //  unsigned char char UserParam8;               // Wert : 0-250
    { param_J16Bitmask                 ,   1 },    //  unsigned char char J16Bitmask;               // for the J16 Output
    { param_J16Timing                  ,   1 },    //  unsigned char char J16Timing;                // for the J16 Output
    { param_J17Bitmask                 ,   1 },    //  unsigned char char J17Bitmask;               // for the J17 Output
    { param_J17Timing                  ,   1 },    //  unsigned char char J17Timing;                // for the J17 Output
    { param_WARN_J16_Bitmask           ,   1 },    //  unsigned char char WARN_J16_Bitmask;         // for the J16 Output
    { param_WARN_J17_Bitmask           ,   1 },    //  unsigned char char WARN_J17_Bitmask;         // for the J17 Output
    { param_AutoPhotoDistance          ,   1 },    //  unsigned char char AutoPhotoDistance;        // Auto Photo   // ab Rev. 100 (ehemals NaviOut1Parameter)
    { param_NaviGpsModeChannel         ,   1 },    //  unsigned char char NaviGpsModeChannel;       // Parameters for the Naviboard
    { param_NaviGpsGain                ,   1 },    //  unsigned char char NaviGpsGain;
    { param_NaviGpsP                   ,   1 },    //  unsigned char char NaviGpsP;
    { param_NaviGpsI                   ,   1 },    //  unsigned char char NaviGpsI;
    { param_NaviGpsD                   ,   1 },    //  unsigned char char NaviGpsD;
    { param_NaviGpsPLimit              ,   1 },    //  unsigned char char NaviGpsPLimit;
    { param_NaviGpsILimit              ,   1 },    //  unsigned char char NaviGpsILimit;
    { param_NaviGpsDLimit              ,   1 },    //  unsigned char char NaviGpsDLimit;
    { param_NaviGpsA                   ,   1 },    //  unsigned char char NaviGpsA;
    { param_NaviGpsMinSat              ,   1 },    //  unsigned char char NaviGpsMinSat;
    { param_NaviStickThreshold         ,   1 },    //  unsigned char char NaviStickThreshold;
    { param_NaviWindCorrection         ,   1 },    //  unsigned char char NaviWindCorrection;
    { param_NaviAccCompensation        ,   1 },    //  unsigned char char NaviAccCompensation;      // New since 0.86 -> was: SpeedCompensation
    { param_NaviMaxFlyingRange         ,   1 },    //  unsigned char char NaviMaxFlyingRange;       // in 10m
    { param_NaviAngleLimitation        ,   1 },    //  unsigned char char NaviAngleLimitation;
    { param_NaviPH_LoginTime           ,   1 },    //  unsigned char char NaviPH_LoginTime;
    { param_NaviDescendRange           ,   1 },    //  unsigned char char NaviDescendRange;
    { param_ExternalControl            ,   1 },    //  unsigned char char ExternalControl;          // for serial Control
    { param_OrientationAngle           ,   1 },    //  unsigned char char OrientationAngle;         // Where is the front-direction?
    { param_CareFreeChannel            ,   1 },    //  unsigned char char CareFreeChannel;          // switch for CareFree
    { param_MotorSafetySwitch          ,   1 },    //  unsigned char char MotorSafetySwitch;
    { param_MotorSmooth                ,   1 },    //  unsigned char char MotorSmooth;
    { param_ComingHomeAltitude         ,   1 },    //  unsigned char char ComingHomeAltitude;
    { param_FailSafeTime               ,   1 },    //  unsigned char char FailSafeTime;
    { param_MaxAltitude                ,   1 },    //  unsigned char char MaxAltitude;
    { param_FailsafeChannel            ,   1 },    //  unsigned char char FailsafeChannel;          // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick            ,   1 },    //  unsigned char char ServoFilterNick;
    { param_ServoFilterRoll            ,   1 },    //  unsigned char char ServoFilterRoll;
    { param_Servo3OnValue              ,   1 },    //  unsigned char char Servo3OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo3OffValue             ,   1 },    //  unsigned char char Servo3OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_Servo4OnValue              ,   1 },    //  unsigned char char Servo4OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo4OffValue             ,   1 },    //  unsigned char char Servo4OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_ServoFS_Pos                ,   5 },    //  unsigned char ServoFS_Pos[5];                // ab Rev. 105 (FC 2.09i)
    { param_StartLandChannel           ,   1 },    //  unsigned char char StartLandChannel;
    { param_LandingSpeed               ,   1 },    //  unsigned char char LandingSpeed;
    { param_CompassOffset              ,   1 },    //  unsigned char char CompassOffset;
    { param_AutoLandingVoltage         ,   1 },    //  unsigned char char AutoLandingVoltage;       // in 0,1V  0 -> disabled
    { param_ComingHomeVoltage          ,   1 },    //  unsigned char char ComingHomeVoltage;        // in 0,1V  0 -> disabled
    { param_AutoPhotoAtitudes          ,   1 },    //  unsigned char char AutoPhotoAtitudes;        // ab Rev. 100
    { param_SingleWpSpeed              ,   1 },    //  unsigned char char SingleWpSpeed;            // ab Rev. 100
    { param_LandingPulse               ,   1 },    //  unsigned char char LandingPulse;             // ab Rev. 104 (FC 2.09d)
    { param_SingleWpControlChannel     ,   1 },    //  unsigned char SingleWpControlChannel;        // ab Rev. 106 (FC2.11a)
    { param_MenuKeyChannel             ,   1 },    //  unsigned char MenuKeyChannel;                // ab Rev. 106 (FC2.11a)
    { param_CamCtrlModeChannel         ,   1 },    //  unsigned char CamCtrlModeChannel;            // ab Rev. 107 (FC2.13f)
    { param_CamCtrlZoomChannel         ,   1 },    //  unsigned char CamCtrlZoomChannel;            // ab Rev. 107 (FC2.13f)
	{ param_reserved				   ,  32 },	   //  unsigned char reserved[32];       			// for later use ab Rev. 108 (FC2.14a)
    { param_BitConfig                  ,   1 },    //  unsigned char char BitConfig;                // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert            ,   1 },    //  unsigned char char ServoCompInvert;          // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig                ,   1 },    //  unsigned char char ExtraConfig;              // bitcodiert
    { param_GlobalConfig3              ,   1 },    //  unsigned char char GlobalConfig3;            // bitcodiert
    { param_Name                       ,  12 },    //  char               Name[12];
    { param_crc                        ,   1 },    //  unsigned char char crc;                      // must be the last byte!

    { param_EOF                        ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_109[]


//-----------------------------------------------
// FC-Parameter Revision: 110
//
// ab FC-Version: ab 2.18 bis 2.1?
// gefunden in: 2.18
//
// STRUKTUR-DIFF zu 109:
//
//

//

//-----------------------------------------------
paramRevItem_t const paramset_110[] PROGMEM =
{
    { param_Revision                   ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung              ,  12 },    //  unsigned char char Kanalbelegung[12];        // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig               ,   1 },    //  unsigned char char GlobalConfig;             // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas               ,   1 },    //  unsigned char char Hoehe_MinGas;             // Wert : 0-100
    { param_Luftdruck_D                ,   1 },    //  unsigned char char Luftdruck_D;              // Wert : 0-250
    { param_HoeheChannel               ,   1 },    //  unsigned char char HoeheChannel;             // Wert : 0-32
    { param_Hoehe_P                    ,   1 },    //  unsigned char char Hoehe_P;                  // Wert : 0-32
    { param_Hoehe_Verstaerkung         ,   1 },    //  unsigned char char Hoehe_Verstaerkung;       // Wert : 0-50
    { param_Hoehe_ACC_Wirkung          ,   1 },    //  unsigned char char Hoehe_ACC_Wirkung;        // Wert : 0-250
    { param_Hoehe_HoverBand            ,   1 },    //  unsigned char char Hoehe_HoverBand;          // Wert : 0-250
    { param_Hoehe_TiltCompensation     ,   1 },    //  unsigned char char Hoehe_TiltCompensation;   // Wert : 0-250
    { param_Hoehe_StickNeutralPoint    ,   1 },    //  unsigned char char Hoehe_StickNeutralPoint;  // Wert : 0-250
    { param_Stick_P                    ,   1 },    //  unsigned char char Stick_P;                  // Wert : 1-6
    { param_Stick_D                    ,   1 },    //  unsigned char char Stick_D;                  // Wert : 0-64
    { param_StickGier_P                ,   1 },    //  unsigned char char StickGier_P;              // Wert : 1-20
    { param_Gas_Min                    ,   1 },    //  unsigned char char Gas_Min;                  // Wert : 0-32
    { param_Gas_Max                    ,   1 },    //  unsigned char char Gas_Max;                  // Wert : 33-250
    { param_GyroAccFaktor              ,   1 },    //  unsigned char char GyroAccFaktor;            // Wert : 1-64
    { param_KompassWirkung             ,   1 },    //  unsigned char char KompassWirkung;           // Wert : 0-32
    { param_Gyro_P                     ,   1 },    //  unsigned char char Gyro_P;                   // Wert : 10-250
    { param_Gyro_I                     ,   1 },    //  unsigned char char Gyro_I;                   // Wert : 0-250
    { param_Gyro_D                     ,   1 },    //  unsigned char char Gyro_D;                   // Wert : 0-250
    { param_Gyro_Gier_P                ,   1 },    //  unsigned char char Gyro_Gier_P;              // Wert : 10-250
    { param_Gyro_Gier_I                ,   1 },    //  unsigned char char Gyro_Gier_I;              // Wert : 0-250
    { param_Gyro_Stability             ,   1 },    //  unsigned char char Gyro_Stability;           // Wert : 0-16
    { param_UnterspannungsWarnung      ,   1 },    //  unsigned char char UnterspannungsWarnung;    // Wert : 0-250
    { param_NotGas                     ,   1 },    //  unsigned char char NotGas;                   // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit                 ,   1 },    //  unsigned char char NotGasZeit;               // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                   ,   1 },    //  unsigned char char Receiver;                 // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                   ,   1 },    //  unsigned char char I_Faktor;                 // Wert : 0-250
    { param_UserParam1                 ,   1 },    //  unsigned char char UserParam1;               // Wert : 0-250
    { param_UserParam2                 ,   1 },    //  unsigned char char UserParam2;               // Wert : 0-250
    { param_UserParam3                 ,   1 },    //  unsigned char char UserParam3;               // Wert : 0-250
    { param_UserParam4                 ,   1 },    //  unsigned char char UserParam4;               // Wert : 0-250
    { param_ServoNickControl           ,   1 },    //  unsigned char char ServoNickControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp              ,   1 },    //  unsigned char char ServoNickComp;            // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin               ,   1 },    //  unsigned char char ServoNickMin;             // Wert : 0-250     // Anschlag
    { param_ServoNickMax               ,   1 },    //  unsigned char char ServoNickMax;             // Wert : 0-250     // Anschlag
    { param_ServoRollControl           ,   1 },    //  unsigned char char ServoRollControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp              ,   1 },    //  unsigned char char ServoRollComp;            // Wert : 0-250
    { param_ServoRollMin               ,   1 },    //  unsigned char char ServoRollMin;             // Wert : 0-250
    { param_ServoRollMax               ,   1 },    //  unsigned char char ServoRollMax;             // Wert : 0-250
    { param_ServoNickRefresh           ,   1 },    //  unsigned char char ServoNickRefresh;         // Speed of the Servo
    { param_ServoManualControlSpeed    ,   1 },    //  unsigned char char ServoManualControlSpeed;  //
    { param_CamOrientation             ,   1 },    //  unsigned char char CamOrientation;           //
    { param_Servo3                     ,   1 },    //  unsigned char char Servo3;                   // Value or mapping of the Servo Output
    { param_Servo4                     ,   1 },    //  unsigned char char Servo4;                   // Value or mapping of the Servo Output
    { param_Servo5                     ,   1 },    //  unsigned char char Servo5;                   // Value or mapping of the Servo Output
    { param_LoopGasLimit               ,   1 },    //  unsigned char char LoopGasLimit;             // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold              ,   1 },    //  unsigned char char LoopThreshold;            // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese              ,   1 },    //  unsigned char char LoopHysterese;            // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1              ,   1 },    //  unsigned char char AchsKopplung1;            // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2              ,   1 },    //  unsigned char char AchsKopplung2;            // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection      ,   1 },    //  unsigned char char CouplingYawCorrection;    // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick         ,   1 },    //  unsigned char char WinkelUmschlagNick;       // Wert: 0-250  180°-Punkt
    { param_WinkelUmschlagRoll         ,   1 },    //  unsigned char char WinkelUmschlagRoll;       // Wert: 0-250  180°-Punkt
    { param_GyroAccAbgleich            ,   1 },    //  unsigned char char GyroAccAbgleich;          // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp                  ,   1 },    //  unsigned char char Driftkomp;
    { param_DynamicStability           ,   1 },    //  unsigned char char DynamicStability;
    { param_UserParam5                 ,   1 },    //  unsigned char char UserParam5;               // Wert : 0-250
    { param_UserParam6                 ,   1 },    //  unsigned char char UserParam6;               // Wert : 0-250
    { param_UserParam7                 ,   1 },    //  unsigned char char UserParam7;               // Wert : 0-250
    { param_UserParam8                 ,   1 },    //  unsigned char char UserParam8;               // Wert : 0-250
    { param_J16Bitmask                 ,   1 },    //  unsigned char char J16Bitmask;               // for the J16 Output
    { param_J16Timing                  ,   1 },    //  unsigned char char J16Timing;                // for the J16 Output
    { param_J17Bitmask                 ,   1 },    //  unsigned char char J17Bitmask;               // for the J17 Output
    { param_J17Timing                  ,   1 },    //  unsigned char char J17Timing;                // for the J17 Output
    { param_WARN_J16_Bitmask           ,   1 },    //  unsigned char char WARN_J16_Bitmask;         // for the J16 Output
    { param_WARN_J17_Bitmask           ,   1 },    //  unsigned char char WARN_J17_Bitmask;         // for the J17 Output
    { param_AutoPhotoDistance          ,   1 },    //  unsigned char char AutoPhotoDistance;        // Auto Photo   // ab Rev. 100 (ehemals NaviOut1Parameter)
    { param_NaviGpsModeChannel         ,   1 },    //  unsigned char char NaviGpsModeChannel;       // Parameters for the Naviboard
    { param_NaviGpsGain                ,   1 },    //  unsigned char char NaviGpsGain;
    { param_NaviGpsP                   ,   1 },    //  unsigned char char NaviGpsP;
    { param_NaviGpsI                   ,   1 },    //  unsigned char char NaviGpsI;
    { param_NaviGpsD                   ,   1 },    //  unsigned char char NaviGpsD;
    { param_NaviGpsPLimit              ,   1 },    //  unsigned char char NaviGpsPLimit;
    { param_NaviGpsILimit              ,   1 },    //  unsigned char char NaviGpsILimit;
    { param_NaviGpsDLimit              ,   1 },    //  unsigned char char NaviGpsDLimit;
    { param_NaviGpsA                   ,   1 },    //  unsigned char char NaviGpsA;
    { param_NaviGpsMinSat              ,   1 },    //  unsigned char char NaviGpsMinSat;
    { param_NaviStickThreshold         ,   1 },    //  unsigned char char NaviStickThreshold;
    { param_NaviWindCorrection         ,   1 },    //  unsigned char char NaviWindCorrection;
    { param_NaviAccCompensation        ,   1 },    //  unsigned char char NaviAccCompensation;      // New since 0.86 -> was: SpeedCompensation
    { param_NaviMaxFlyingRange         ,   1 },    //  unsigned char char NaviMaxFlyingRange;       // in 10m
    { param_NaviAngleLimitation        ,   1 },    //  unsigned char char NaviAngleLimitation;
    { param_NaviPH_LoginTime           ,   1 },    //  unsigned char char NaviPH_LoginTime;
    { param_NaviDescendRange           ,   1 },    //  unsigned char char NaviDescendRange;
    { param_ExternalControl            ,   1 },    //  unsigned char char ExternalControl;          // for serial Control
    { param_OrientationAngle           ,   1 },    //  unsigned char char OrientationAngle;         // Where is the front-direction?
    { param_CareFreeChannel            ,   1 },    //  unsigned char char CareFreeChannel;          // switch for CareFree
    { param_MotorSafetySwitch          ,   1 },    //  unsigned char char MotorSafetySwitch;
    { param_MotorSmooth                ,   1 },    //  unsigned char char MotorSmooth;
    { param_ComingHomeAltitude         ,   1 },    //  unsigned char char ComingHomeAltitude;
    { param_FailSafeTime               ,   1 },    //  unsigned char char FailSafeTime;
    { param_MaxAltitude                ,   1 },    //  unsigned char char MaxAltitude;
    { param_FailsafeChannel            ,   1 },    //  unsigned char char FailsafeChannel;          // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick            ,   1 },    //  unsigned char char ServoFilterNick;
    { param_ServoFilterRoll            ,   1 },    //  unsigned char char ServoFilterRoll;
    { param_Servo3OnValue              ,   1 },    //  unsigned char char Servo3OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo3OffValue             ,   1 },    //  unsigned char char Servo3OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_Servo4OnValue              ,   1 },    //  unsigned char char Servo4OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo4OffValue             ,   1 },    //  unsigned char char Servo4OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_ServoFS_Pos                ,   5 },    //  unsigned char ServoFS_Pos[5];                // ab Rev. 105 (FC 2.09i)
    { param_StartLandChannel           ,   1 },    //  unsigned char char StartLandChannel;
    { param_LandingSpeed               ,   1 },    //  unsigned char char LandingSpeed;
    { param_CompassOffset              ,   1 },    //  unsigned char char CompassOffset;
    { param_AutoLandingVoltage         ,   1 },    //  unsigned char char AutoLandingVoltage;       // in 0,1V  0 -> disabled
    { param_ComingHomeVoltage          ,   1 },    //  unsigned char char ComingHomeVoltage;        // in 0,1V  0 -> disabled
    { param_AutoPhotoAtitudes          ,   1 },    //  unsigned char char AutoPhotoAtitudes;        // ab Rev. 100
    { param_SingleWpSpeed              ,   1 },    //  unsigned char char SingleWpSpeed;            // ab Rev. 100
    { param_LandingPulse               ,   1 },    //  unsigned char char LandingPulse;             // ab Rev. 104 (FC 2.09d)
    { param_SingleWpControlChannel     ,   1 },    //  unsigned char SingleWpControlChannel;        // ab Rev. 106 (FC2.11a)
    { param_MenuKeyChannel             ,   1 },    //  unsigned char MenuKeyChannel;                // ab Rev. 106 (FC2.11a)
    { param_CamCtrlModeChannel         ,   1 },    //  unsigned char CamCtrlModeChannel;            // ab Rev. 107 (FC2.13f)
    { param_CamCtrlZoomChannel         ,   1 },    //  unsigned char CamCtrlZoomChannel;            // ab Rev. 107 (FC2.13f)
	{ param_reserved				   ,  32 },	   //  unsigned char reserved[32];       			// for later use ab Rev. 108 (FC2.14a)
    { param_BitConfig                  ,   1 },    //  unsigned char char BitConfig;                // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert            ,   1 },    //  unsigned char char ServoCompInvert;          // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig                ,   1 },    //  unsigned char char ExtraConfig;              // bitcodiert
    { param_GlobalConfig3              ,   1 },    //  unsigned char char GlobalConfig3;            // bitcodiert
    { param_Name                       ,  12 },    //  char               Name[12];
    { param_crc                        ,   1 },    //  unsigned char char crc;                      // must be the last byte!

    { param_EOF                        ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_110[]

//-----------------------------------------------
// FC-Parameter Revision: 112
//
// ab FC-Version: ab 2.19 bis 2.1?
// gefunden in: 2.19
//
// STRUKTUR-DIFF zu 110:
//
//

//

//-----------------------------------------------
paramRevItem_t const paramset_112[] PROGMEM =
{
    { param_Revision                   ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung              ,  12 },    //  unsigned char char Kanalbelegung[12];        // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig               ,   1 },    //  unsigned char char GlobalConfig;             // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas               ,   1 },    //  unsigned char char Hoehe_MinGas;             // Wert : 0-100
    { param_Luftdruck_D                ,   1 },    //  unsigned char char Luftdruck_D;              // Wert : 0-250
    { param_HoeheChannel               ,   1 },    //  unsigned char char HoeheChannel;             // Wert : 0-32
    { param_Hoehe_P                    ,   1 },    //  unsigned char char Hoehe_P;                  // Wert : 0-32
    { param_Hoehe_Verstaerkung         ,   1 },    //  unsigned char char Hoehe_Verstaerkung;       // Wert : 0-50
    { param_Hoehe_ACC_Wirkung          ,   1 },    //  unsigned char char Hoehe_ACC_Wirkung;        // Wert : 0-250
    { param_Hoehe_HoverBand            ,   1 },    //  unsigned char char Hoehe_HoverBand;          // Wert : 0-250
    { param_Hoehe_TiltCompensation     ,   1 },    //  unsigned char char Hoehe_TiltCompensation;   // Wert : 0-250
    { param_Hoehe_StickNeutralPoint    ,   1 },    //  unsigned char char Hoehe_StickNeutralPoint;  // Wert : 0-250
    { param_Stick_P                    ,   1 },    //  unsigned char char Stick_P;                  // Wert : 1-6
    { param_Stick_D                    ,   1 },    //  unsigned char char Stick_D;                  // Wert : 0-64
    { param_StickGier_P                ,   1 },    //  unsigned char char StickGier_P;              // Wert : 1-20
    { param_Gas_Min                    ,   1 },    //  unsigned char char Gas_Min;                  // Wert : 0-32
    { param_Gas_Max                    ,   1 },    //  unsigned char char Gas_Max;                  // Wert : 33-250
    { param_GyroAccFaktor              ,   1 },    //  unsigned char char GyroAccFaktor;            // Wert : 1-64
    { param_KompassWirkung             ,   1 },    //  unsigned char char KompassWirkung;           // Wert : 0-32
    { param_Gyro_P                     ,   1 },    //  unsigned char char Gyro_P;                   // Wert : 10-250
    { param_Gyro_I                     ,   1 },    //  unsigned char char Gyro_I;                   // Wert : 0-250
    { param_Gyro_D                     ,   1 },    //  unsigned char char Gyro_D;                   // Wert : 0-250
    { param_Gyro_Gier_P                ,   1 },    //  unsigned char char Gyro_Gier_P;              // Wert : 10-250
    { param_Gyro_Gier_I                ,   1 },    //  unsigned char char Gyro_Gier_I;              // Wert : 0-250
    { param_Gyro_Stability             ,   1 },    //  unsigned char char Gyro_Stability;           // Wert : 0-16
    { param_UnterspannungsWarnung      ,   1 },    //  unsigned char char UnterspannungsWarnung;    // Wert : 0-250
    { param_NotGas                     ,   1 },    //  unsigned char char NotGas;                   // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit                 ,   1 },    //  unsigned char char NotGasZeit;               // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                   ,   1 },    //  unsigned char char Receiver;                 // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                   ,   1 },    //  unsigned char char I_Faktor;                 // Wert : 0-250
    { param_UserParam1                 ,   1 },    //  unsigned char char UserParam1;               // Wert : 0-250
    { param_UserParam2                 ,   1 },    //  unsigned char char UserParam2;               // Wert : 0-250
    { param_UserParam3                 ,   1 },    //  unsigned char char UserParam3;               // Wert : 0-250
    { param_UserParam4                 ,   1 },    //  unsigned char char UserParam4;               // Wert : 0-250
    { param_ServoNickControl           ,   1 },    //  unsigned char char ServoNickControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp              ,   1 },    //  unsigned char char ServoNickComp;            // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin               ,   1 },    //  unsigned char char ServoNickMin;             // Wert : 0-250     // Anschlag
    { param_ServoNickMax               ,   1 },    //  unsigned char char ServoNickMax;             // Wert : 0-250     // Anschlag
    { param_ServoRollControl           ,   1 },    //  unsigned char char ServoRollControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp              ,   1 },    //  unsigned char char ServoRollComp;            // Wert : 0-250
    { param_ServoRollMin               ,   1 },    //  unsigned char char ServoRollMin;             // Wert : 0-250
    { param_ServoRollMax               ,   1 },    //  unsigned char char ServoRollMax;             // Wert : 0-250
    { param_ServoNickRefresh           ,   1 },    //  unsigned char char ServoNickRefresh;         // Speed of the Servo
    { param_ServoManualControlSpeed    ,   1 },    //  unsigned char char ServoManualControlSpeed;  //
    { param_CamOrientation             ,   1 },    //  unsigned char char CamOrientation;           //
    { param_Servo3                     ,   1 },    //  unsigned char char Servo3;                   // Value or mapping of the Servo Output
    { param_Servo4                     ,   1 },    //  unsigned char char Servo4;                   // Value or mapping of the Servo Output
    { param_Servo5                     ,   1 },    //  unsigned char char Servo5;                   // Value or mapping of the Servo Output
    { param_LoopGasLimit               ,   1 },    //  unsigned char char LoopGasLimit;             // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold              ,   1 },    //  unsigned char char LoopThreshold;            // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese              ,   1 },    //  unsigned char char LoopHysterese;            // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1              ,   1 },    //  unsigned char char AchsKopplung1;            // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2              ,   1 },    //  unsigned char char AchsKopplung2;            // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection      ,   1 },    //  unsigned char char CouplingYawCorrection;    // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick         ,   1 },    //  unsigned char char WinkelUmschlagNick;       // Wert: 0-250  180°-Punkt
    { param_WinkelUmschlagRoll         ,   1 },    //  unsigned char char WinkelUmschlagRoll;       // Wert: 0-250  180°-Punkt
    { param_GyroAccAbgleich            ,   1 },    //  unsigned char char GyroAccAbgleich;          // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp                  ,   1 },    //  unsigned char char Driftkomp;
    { param_DynamicStability           ,   1 },    //  unsigned char char DynamicStability;
    { param_UserParam5                 ,   1 },    //  unsigned char char UserParam5;               // Wert : 0-250
    { param_UserParam6                 ,   1 },    //  unsigned char char UserParam6;               // Wert : 0-250
    { param_UserParam7                 ,   1 },    //  unsigned char char UserParam7;               // Wert : 0-250
    { param_UserParam8                 ,   1 },    //  unsigned char char UserParam8;               // Wert : 0-250
    { param_J16Bitmask                 ,   1 },    //  unsigned char char J16Bitmask;               // for the J16 Output
    { param_J16Timing                  ,   1 },    //  unsigned char char J16Timing;                // for the J16 Output
    { param_J17Bitmask                 ,   1 },    //  unsigned char char J17Bitmask;               // for the J17 Output
    { param_J17Timing                  ,   1 },    //  unsigned char char J17Timing;                // for the J17 Output
    { param_WARN_J16_Bitmask           ,   1 },    //  unsigned char char WARN_J16_Bitmask;         // for the J16 Output
    { param_WARN_J17_Bitmask           ,   1 },    //  unsigned char char WARN_J17_Bitmask;         // for the J17 Output
    { param_AutoPhotoDistance          ,   1 },    //  unsigned char char AutoPhotoDistance;        // Auto Photo   // ab Rev. 100 (ehemals NaviOut1Parameter)
    { param_NaviGpsModeChannel         ,   1 },    //  unsigned char char NaviGpsModeChannel;       // Parameters for the Naviboard
    { param_NaviGpsGain                ,   1 },    //  unsigned char char NaviGpsGain;
    { param_NaviGpsP                   ,   1 },    //  unsigned char char NaviGpsP;
    { param_NaviGpsI                   ,   1 },    //  unsigned char char NaviGpsI;
    { param_NaviGpsD                   ,   1 },    //  unsigned char char NaviGpsD;
    { param_NaviGpsPLimit              ,   1 },    //  unsigned char char NaviGpsPLimit;
    { param_NaviGpsILimit              ,   1 },    //  unsigned char char NaviGpsILimit;
    { param_NaviGpsDLimit              ,   1 },    //  unsigned char char NaviGpsDLimit;
    { param_NaviGpsA                   ,   1 },    //  unsigned char char NaviGpsA;
    { param_NaviGpsMinSat              ,   1 },    //  unsigned char char NaviGpsMinSat;
    { param_NaviStickThreshold         ,   1 },    //  unsigned char char NaviStickThreshold;
    { param_NaviWindCorrection         ,   1 },    //  unsigned char char NaviWindCorrection;
    { param_NaviAccCompensation        ,   1 },    //  unsigned char char NaviAccCompensation;      // New since 0.86 -> was: SpeedCompensation
    { param_NaviMaxFlyingRange         ,   1 },    //  unsigned char char NaviMaxFlyingRange;       // in 10m
    { param_NaviAngleLimitation        ,   1 },    //  unsigned char char NaviAngleLimitation;
    { param_NaviPH_LoginTime           ,   1 },    //  unsigned char char NaviPH_LoginTime;
    { param_NaviDescendRange           ,   1 },    //  unsigned char char NaviDescendRange;
    { param_ExternalControl            ,   1 },    //  unsigned char char ExternalControl;          // for serial Control
    { param_OrientationAngle           ,   1 },    //  unsigned char char OrientationAngle;         // Where is the front-direction?
    { param_CareFreeChannel            ,   1 },    //  unsigned char char CareFreeChannel;          // switch for CareFree
    { param_MotorSafetySwitch          ,   1 },    //  unsigned char char MotorSafetySwitch;
    { param_MotorSmooth                ,   1 },    //  unsigned char char MotorSmooth;
    { param_ComingHomeAltitude         ,   1 },    //  unsigned char char ComingHomeAltitude;
    { param_FailSafeTime               ,   1 },    //  unsigned char char FailSafeTime;
    { param_MaxAltitude                ,   1 },    //  unsigned char char MaxAltitude;
    { param_FailsafeChannel            ,   1 },    //  unsigned char char FailsafeChannel;          // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick            ,   1 },    //  unsigned char char ServoFilterNick;
    { param_ServoFilterRoll            ,   1 },    //  unsigned char char ServoFilterRoll;
    { param_Servo3OnValue              ,   1 },    //  unsigned char char Servo3OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo3OffValue             ,   1 },    //  unsigned char char Servo3OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_Servo4OnValue              ,   1 },    //  unsigned char char Servo4OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo4OffValue             ,   1 },    //  unsigned char char Servo4OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_ServoFS_Pos                ,   5 },    //  unsigned char ServoFS_Pos[5];                // ab Rev. 105 (FC 2.09i)
    { param_StartLandChannel           ,   1 },    //  unsigned char char StartLandChannel;
    { param_LandingSpeed               ,   1 },    //  unsigned char char LandingSpeed;
    { param_CompassOffset              ,   1 },    //  unsigned char char CompassOffset;
    { param_AutoLandingVoltage         ,   1 },    //  unsigned char char AutoLandingVoltage;       // in 0,1V  0 -> disabled
    { param_ComingHomeVoltage          ,   1 },    //  unsigned char char ComingHomeVoltage;        // in 0,1V  0 -> disabled
    { param_AutoPhotoAtitudes          ,   1 },    //  unsigned char char AutoPhotoAtitudes;        // ab Rev. 100
    { param_SingleWpSpeed              ,   1 },    //  unsigned char char SingleWpSpeed;            // ab Rev. 100
    { param_LandingPulse               ,   1 },    //  unsigned char char LandingPulse;             // ab Rev. 104 (FC 2.09d)
    { param_SingleWpControlChannel     ,   1 },    //  unsigned char SingleWpControlChannel;        // ab Rev. 106 (FC2.11a)
    { param_MenuKeyChannel             ,   1 },    //  unsigned char MenuKeyChannel;                // ab Rev. 106 (FC2.11a)
    { param_CamCtrlModeChannel         ,   1 },    //  unsigned char CamCtrlModeChannel;            // ab Rev. 107 (FC2.13f)
    { param_CamCtrlZoomChannel         ,   1 },    //  unsigned char CamCtrlZoomChannel;            // ab Rev. 107 (FC2.13f)
	{ param_FailsafeAltitude		   ,   1 },	   //  unsigned char FailsSafeAltitude;    		    // ab Rev. 111 (FC2.19)
	{ param_GimbalYawChannel		   ,   1 },    //  unsigned char GimbalYawChannel			    // ab Rev. 112 )FC2.19)
	{ param_reserved				   ,  30 },	   //  unsigned char reserved[32];       			// for later use ab Rev. 108 (FC2.14a)
    { param_BitConfig                  ,   1 },    //  unsigned char char BitConfig;                // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert            ,   1 },    //  unsigned char char ServoCompInvert;          // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig                ,   1 },    //  unsigned char char ExtraConfig;              // bitcodiert
    { param_GlobalConfig3              ,   1 },    //  unsigned char char GlobalConfig3;            // bitcodiert
    { param_Name                       ,  12 },    //  char               Name[12];
    { param_crc                        ,   1 },    //  unsigned char char crc;                      // must be the last byte!

    { param_EOF                        ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_112[]

//-----------------------------------------------
// FC-Parameter Revision: 112
//
// ab FC-Version: ab 2.19 bis 2.1?
// gefunden in: 2.19
//
// STRUKTUR-DIFF zu 110:
//
//

//

//-----------------------------------------------
paramRevItem_t const paramset_113[] PROGMEM =
{
    { param_Revision                   ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung              ,  12 },    //  unsigned char char Kanalbelegung[12];        // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig               ,   1 },    //  unsigned char char GlobalConfig;             // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas               ,   1 },    //  unsigned char char Hoehe_MinGas;             // Wert : 0-100
    { param_Luftdruck_D                ,   1 },    //  unsigned char char Luftdruck_D;              // Wert : 0-250
    { param_HoeheChannel               ,   1 },    //  unsigned char char HoeheChannel;             // Wert : 0-32
    { param_Hoehe_P                    ,   1 },    //  unsigned char char Hoehe_P;                  // Wert : 0-32
    { param_Hoehe_Verstaerkung         ,   1 },    //  unsigned char char Hoehe_Verstaerkung;       // Wert : 0-50
    { param_Hoehe_ACC_Wirkung          ,   1 },    //  unsigned char char Hoehe_ACC_Wirkung;        // Wert : 0-250
    { param_Hoehe_HoverBand            ,   1 },    //  unsigned char char Hoehe_HoverBand;          // Wert : 0-250
    { param_Hoehe_TiltCompensation     ,   1 },    //  unsigned char char Hoehe_TiltCompensation;   // Wert : 0-250
    { param_Hoehe_StickNeutralPoint    ,   1 },    //  unsigned char char Hoehe_StickNeutralPoint;  // Wert : 0-250
    { param_Stick_P                    ,   1 },    //  unsigned char char Stick_P;                  // Wert : 1-6
    { param_Stick_D                    ,   1 },    //  unsigned char char Stick_D;                  // Wert : 0-64
    { param_StickGier_P                ,   1 },    //  unsigned char char StickGier_P;              // Wert : 1-20
    { param_Gas_Min                    ,   1 },    //  unsigned char char Gas_Min;                  // Wert : 0-32
    { param_Gas_Max                    ,   1 },    //  unsigned char char Gas_Max;                  // Wert : 33-250
    { param_GyroAccFaktor              ,   1 },    //  unsigned char char GyroAccFaktor;            // Wert : 1-64
    { param_KompassWirkung             ,   1 },    //  unsigned char char KompassWirkung;           // Wert : 0-32
    { param_Gyro_P                     ,   1 },    //  unsigned char char Gyro_P;                   // Wert : 10-250
    { param_Gyro_I                     ,   1 },    //  unsigned char char Gyro_I;                   // Wert : 0-250
    { param_Gyro_D                     ,   1 },    //  unsigned char char Gyro_D;                   // Wert : 0-250
    { param_Gyro_Gier_P                ,   1 },    //  unsigned char char Gyro_Gier_P;              // Wert : 10-250
    { param_Gyro_Gier_I                ,   1 },    //  unsigned char char Gyro_Gier_I;              // Wert : 0-250
    { param_Gyro_Stability             ,   1 },    //  unsigned char char Gyro_Stability;           // Wert : 0-16
    { param_UnterspannungsWarnung      ,   1 },    //  unsigned char char UnterspannungsWarnung;    // Wert : 0-250
    { param_NotGas                     ,   1 },    //  unsigned char char NotGas;                   // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit                 ,   1 },    //  unsigned char char NotGasZeit;               // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                   ,   1 },    //  unsigned char char Receiver;                 // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                   ,   1 },    //  unsigned char char I_Faktor;                 // Wert : 0-250
    { param_UserParam1                 ,   1 },    //  unsigned char char UserParam1;               // Wert : 0-250
    { param_UserParam2                 ,   1 },    //  unsigned char char UserParam2;               // Wert : 0-250
    { param_UserParam3                 ,   1 },    //  unsigned char char UserParam3;               // Wert : 0-250
    { param_UserParam4                 ,   1 },    //  unsigned char char UserParam4;               // Wert : 0-250
    { param_ServoNickControl           ,   1 },    //  unsigned char char ServoNickControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp              ,   1 },    //  unsigned char char ServoNickComp;            // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin               ,   1 },    //  unsigned char char ServoNickMin;             // Wert : 0-250     // Anschlag
    { param_ServoNickMax               ,   1 },    //  unsigned char char ServoNickMax;             // Wert : 0-250     // Anschlag
    { param_ServoRollControl           ,   1 },    //  unsigned char char ServoRollControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp              ,   1 },    //  unsigned char char ServoRollComp;            // Wert : 0-250
    { param_ServoRollMin               ,   1 },    //  unsigned char char ServoRollMin;             // Wert : 0-250
    { param_ServoRollMax               ,   1 },    //  unsigned char char ServoRollMax;             // Wert : 0-250
    { param_ServoNickRefresh           ,   1 },    //  unsigned char char ServoNickRefresh;         // Speed of the Servo
    { param_ServoManualControlSpeed    ,   1 },    //  unsigned char char ServoManualControlSpeed;  //
    { param_CamOrientation             ,   1 },    //  unsigned char char CamOrientation;           //
    { param_Servo3                     ,   1 },    //  unsigned char char Servo3;                   // Value or mapping of the Servo Output
    { param_Servo4                     ,   1 },    //  unsigned char char Servo4;                   // Value or mapping of the Servo Output
    { param_Servo5                     ,   1 },    //  unsigned char char Servo5;                   // Value or mapping of the Servo Output
    { param_LoopGasLimit               ,   1 },    //  unsigned char char LoopGasLimit;             // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold              ,   1 },    //  unsigned char char LoopThreshold;            // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese              ,   1 },    //  unsigned char char LoopHysterese;            // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1              ,   1 },    //  unsigned char char AchsKopplung1;            // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2              ,   1 },    //  unsigned char char AchsKopplung2;            // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection      ,   1 },    //  unsigned char char CouplingYawCorrection;    // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick         ,   1 },    //  unsigned char char WinkelUmschlagNick;       // Wert: 0-250  180°-Punkt
    { param_WinkelUmschlagRoll         ,   1 },    //  unsigned char char WinkelUmschlagRoll;       // Wert: 0-250  180°-Punkt
    { param_GyroAccAbgleich            ,   1 },    //  unsigned char char GyroAccAbgleich;          // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp                  ,   1 },    //  unsigned char char Driftkomp;
    { param_DynamicStability           ,   1 },    //  unsigned char char DynamicStability;
    { param_UserParam5                 ,   1 },    //  unsigned char char UserParam5;               // Wert : 0-250
    { param_UserParam6                 ,   1 },    //  unsigned char char UserParam6;               // Wert : 0-250
    { param_UserParam7                 ,   1 },    //  unsigned char char UserParam7;               // Wert : 0-250
    { param_UserParam8                 ,   1 },    //  unsigned char char UserParam8;               // Wert : 0-250
    { param_J16Bitmask                 ,   1 },    //  unsigned char char J16Bitmask;               // for the J16 Output
    { param_J16Timing                  ,   1 },    //  unsigned char char J16Timing;                // for the J16 Output
    { param_J17Bitmask                 ,   1 },    //  unsigned char char J17Bitmask;               // for the J17 Output
    { param_J17Timing                  ,   1 },    //  unsigned char char J17Timing;                // for the J17 Output
    { param_WARN_J16_Bitmask           ,   1 },    //  unsigned char char WARN_J16_Bitmask;         // for the J16 Output
    { param_WARN_J17_Bitmask           ,   1 },    //  unsigned char char WARN_J17_Bitmask;         // for the J17 Output
    { param_AutoPhotoDistance          ,   1 },    //  unsigned char char AutoPhotoDistance;        // Auto Photo   // ab Rev. 100 (ehemals NaviOut1Parameter)
    { param_NaviGpsModeChannel         ,   1 },    //  unsigned char char NaviGpsModeChannel;       // Parameters for the Naviboard
    { param_NaviGpsGain                ,   1 },    //  unsigned char char NaviGpsGain;
    { param_NaviGpsP                   ,   1 },    //  unsigned char char NaviGpsP;
    { param_NaviGpsI                   ,   1 },    //  unsigned char char NaviGpsI;
    { param_NaviGpsD                   ,   1 },    //  unsigned char char NaviGpsD;
    { param_NaviGpsPLimit              ,   1 },    //  unsigned char char NaviGpsPLimit;
    { param_NaviGpsILimit              ,   1 },    //  unsigned char char NaviGpsILimit;
    { param_NaviGpsDLimit              ,   1 },    //  unsigned char char NaviGpsDLimit;
    { param_NaviGpsA                   ,   1 },    //  unsigned char char NaviGpsA;
    { param_NaviGpsMinSat              ,   1 },    //  unsigned char char NaviGpsMinSat;
    { param_NaviStickThreshold         ,   1 },    //  unsigned char char NaviStickThreshold;
    { param_NaviWindCorrection         ,   1 },    //  unsigned char char NaviWindCorrection;
    { param_NaviAccCompensation        ,   1 },    //  unsigned char char NaviAccCompensation;      // New since 0.86 -> was: SpeedCompensation
    { param_NaviMaxFlyingRange         ,   1 },    //  unsigned char char NaviMaxFlyingRange;       // in 10m
    { param_NaviAngleLimitation        ,   1 },    //  unsigned char char NaviAngleLimitation;
    { param_NaviPH_LoginTime           ,   1 },    //  unsigned char char NaviPH_LoginTime;
    { param_NaviDescendRange           ,   1 },    //  unsigned char char NaviDescendRange;
    { param_ExternalControl            ,   1 },    //  unsigned char char ExternalControl;          // for serial Control
    { param_OrientationAngle           ,   1 },    //  unsigned char char OrientationAngle;         // Where is the front-direction?
    { param_CareFreeChannel            ,   1 },    //  unsigned char char CareFreeChannel;          // switch for CareFree
    { param_MotorSafetySwitch          ,   1 },    //  unsigned char char MotorSafetySwitch;
    { param_MotorSmooth                ,   1 },    //  unsigned char char MotorSmooth;
    { param_ComingHomeAltitude         ,   1 },    //  unsigned char char ComingHomeAltitude;
    { param_FailSafeTime               ,   1 },    //  unsigned char char FailSafeTime;
    { param_MaxAltitude                ,   1 },    //  unsigned char char MaxAltitude;
    { param_FailsafeChannel            ,   1 },    //  unsigned char char FailsafeChannel;          // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick            ,   1 },    //  unsigned char char ServoFilterNick;
    { param_ServoFilterRoll            ,   1 },    //  unsigned char char ServoFilterRoll;
    { param_Servo3OnValue              ,   1 },    //  unsigned char char Servo3OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo3OffValue             ,   1 },    //  unsigned char char Servo3OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_Servo4OnValue              ,   1 },    //  unsigned char char Servo4OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo4OffValue             ,   1 },    //  unsigned char char Servo4OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_ServoFS_Pos                ,   5 },    //  unsigned char ServoFS_Pos[5];                // ab Rev. 105 (FC 2.09i)
    { param_StartLandChannel           ,   1 },    //  unsigned char char StartLandChannel;
    { param_LandingSpeed               ,   1 },    //  unsigned char char LandingSpeed;
    { param_CompassOffset              ,   1 },    //  unsigned char char CompassOffset;
    { param_AutoLandingVoltage         ,   1 },    //  unsigned char char AutoLandingVoltage;       // in 0,1V  0 -> disabled
    { param_ComingHomeVoltage          ,   1 },    //  unsigned char char ComingHomeVoltage;        // in 0,1V  0 -> disabled
    { param_AutoPhotoAtitudes          ,   1 },    //  unsigned char char AutoPhotoAtitudes;        // ab Rev. 100
    { param_SingleWpSpeed              ,   1 },    //  unsigned char char SingleWpSpeed;            // ab Rev. 100
    { param_LandingPulse               ,   1 },    //  unsigned char char LandingPulse;             // ab Rev. 104 (FC 2.09d)
    { param_SingleWpControlChannel     ,   1 },    //  unsigned char SingleWpControlChannel;        // ab Rev. 106 (FC2.11a)
    { param_MenuKeyChannel             ,   1 },    //  unsigned char MenuKeyChannel;                // ab Rev. 106 (FC2.11a)
    { param_CamCtrlModeChannel         ,   1 },    //  unsigned char CamCtrlModeChannel;            // ab Rev. 107 (FC2.13f)
    { param_CamCtrlZoomChannel         ,   1 },    //  unsigned char CamCtrlZoomChannel;            // ab Rev. 107 (FC2.13f)
	{ param_FailsafeAltitude		   ,   1 },	   //  unsigned char FailsSafeAltitude;    		    // ab Rev. 111 (FC2.19)
	{ param_GimbalYawChannel		   ,   1 },    //  unsigned char GimbalYawChannel			    // ab Rev. 112 )FC2.19)
	{ param_GimbalOut1Channel		   ,   1 },    //  unsigned char GimbalOut1Channel;				// ab Rev. 113 (FC2.20)
	{ param_GimbalOut2Channel		   ,   1 },    //  unsigned char GimbalOut2Channel;				// ab Rev. 113 (FC2.20)
	{ param_reserved				   ,  28 },	   //  unsigned char reserved[32];       			// for later use ab Rev. 108 (FC2.14a)
    { param_BitConfig                  ,   1 },    //  unsigned char char BitConfig;                // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert            ,   1 },    //  unsigned char char ServoCompInvert;          // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig                ,   1 },    //  unsigned char char ExtraConfig;              // bitcodiert
    { param_GlobalConfig3              ,   1 },    //  unsigned char char GlobalConfig3;            // bitcodiert
    { param_Name                       ,  12 },    //  char               Name[12];
    { param_crc                        ,   1 },    //  unsigned char char crc;                      // must be the last byte!

    { param_EOF                        ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_113[]

//-----------------------------------------------
paramRevItem_t const paramset_114[] PROGMEM =
{
    { param_Revision                   ,   1 },    //  unsigned char Revision;
    { param_Kanalbelegung              ,  12 },    //  unsigned char char Kanalbelegung[12];        // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
    { param_GlobalConfig               ,   1 },    //  unsigned char char GlobalConfig;             // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
    { param_Hoehe_MinGas               ,   1 },    //  unsigned char char Hoehe_MinGas;             // Wert : 0-100
    { param_Luftdruck_D                ,   1 },    //  unsigned char char Luftdruck_D;              // Wert : 0-250
    { param_HoeheChannel               ,   1 },    //  unsigned char char HoeheChannel;             // Wert : 0-32
    { param_Hoehe_P                    ,   1 },    //  unsigned char char Hoehe_P;                  // Wert : 0-32
    { param_Hoehe_Verstaerkung         ,   1 },    //  unsigned char char Hoehe_Verstaerkung;       // Wert : 0-50
    { param_Hoehe_ACC_Wirkung          ,   1 },    //  unsigned char char Hoehe_ACC_Wirkung;        // Wert : 0-250
    { param_Hoehe_HoverBand            ,   1 },    //  unsigned char char Hoehe_HoverBand;          // Wert : 0-250
    { param_Hoehe_TiltCompensation     ,   1 },    //  unsigned char char Hoehe_TiltCompensation;   // Wert : 0-250
    { param_Hoehe_StickNeutralPoint    ,   1 },    //  unsigned char char Hoehe_StickNeutralPoint;  // Wert : 0-250
    { param_Stick_P                    ,   1 },    //  unsigned char char Stick_P;                  // Wert : 1-6
    { param_Stick_D                    ,   1 },    //  unsigned char char Stick_D;                  // Wert : 0-64
    { param_StickGier_P                ,   1 },    //  unsigned char char StickGier_P;              // Wert : 1-20
    { param_Gas_Min                    ,   1 },    //  unsigned char char Gas_Min;                  // Wert : 0-32
    { param_Gas_Max                    ,   1 },    //  unsigned char char Gas_Max;                  // Wert : 33-250
    { param_GyroAccFaktor              ,   1 },    //  unsigned char char GyroAccFaktor;            // Wert : 1-64
    { param_KompassWirkung             ,   1 },    //  unsigned char char KompassWirkung;           // Wert : 0-32
    { param_Gyro_P                     ,   1 },    //  unsigned char char Gyro_P;                   // Wert : 10-250
    { param_Gyro_I                     ,   1 },    //  unsigned char char Gyro_I;                   // Wert : 0-250
    { param_Gyro_D                     ,   1 },    //  unsigned char char Gyro_D;                   // Wert : 0-250
    { param_Gyro_Gier_P                ,   1 },    //  unsigned char char Gyro_Gier_P;              // Wert : 10-250
    { param_Gyro_Gier_I                ,   1 },    //  unsigned char char Gyro_Gier_I;              // Wert : 0-250
    { param_Gyro_Stability             ,   1 },    //  unsigned char char Gyro_Stability;           // Wert : 0-16
    { param_UnterspannungsWarnung      ,   1 },    //  unsigned char char UnterspannungsWarnung;    // Wert : 0-250
    { param_NotGas                     ,   1 },    //  unsigned char char NotGas;                   // Wert : 0-250     //Gaswert bei Empfangsverlust
    { param_NotGasZeit                 ,   1 },    //  unsigned char char NotGasZeit;               // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
    { param_Receiver                   ,   1 },    //  unsigned char char Receiver;                 // 0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
    { param_I_Faktor                   ,   1 },    //  unsigned char char I_Faktor;                 // Wert : 0-250
    { param_UserParam1                 ,   1 },    //  unsigned char char UserParam1;               // Wert : 0-250
    { param_UserParam2                 ,   1 },    //  unsigned char char UserParam2;               // Wert : 0-250
    { param_UserParam3                 ,   1 },    //  unsigned char char UserParam3;               // Wert : 0-250
    { param_UserParam4                 ,   1 },    //  unsigned char char UserParam4;               // Wert : 0-250
    { param_ServoNickControl           ,   1 },    //  unsigned char char ServoNickControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoNickComp              ,   1 },    //  unsigned char char ServoNickComp;            // Wert : 0-250     // Einfluss Gyro/Servo
    { param_ServoNickMin               ,   1 },    //  unsigned char char ServoNickMin;             // Wert : 0-250     // Anschlag
    { param_ServoNickMax               ,   1 },    //  unsigned char char ServoNickMax;             // Wert : 0-250     // Anschlag
    { param_ServoRollControl           ,   1 },    //  unsigned char char ServoRollControl;         // Wert : 0-250     // Stellung des Servos
    { param_ServoRollComp              ,   1 },    //  unsigned char char ServoRollComp;            // Wert : 0-250
    { param_ServoRollMin               ,   1 },    //  unsigned char char ServoRollMin;             // Wert : 0-250
    { param_ServoRollMax               ,   1 },    //  unsigned char char ServoRollMax;             // Wert : 0-250
    { param_ServoNickRefresh           ,   1 },    //  unsigned char char ServoNickRefresh;         // Speed of the Servo
    { param_ServoManualControlSpeed    ,   1 },    //  unsigned char char ServoManualControlSpeed;  //
    { param_CamOrientation             ,   1 },    //  unsigned char char CamOrientation;           //
    { param_Servo3                     ,   1 },    //  unsigned char char Servo3;                   // Value or mapping of the Servo Output
    { param_Servo4                     ,   1 },    //  unsigned char char Servo4;                   // Value or mapping of the Servo Output
    { param_Servo5                     ,   1 },    //  unsigned char char Servo5;                   // Value or mapping of the Servo Output
    { param_LoopGasLimit               ,   1 },    //  unsigned char char LoopGasLimit;             // Wert: 0-250  max. Gas während Looping
    { param_LoopThreshold              ,   1 },    //  unsigned char char LoopThreshold;            // Wert: 0-250  Schwelle für Stickausschlag
    { param_LoopHysterese              ,   1 },    //  unsigned char char LoopHysterese;            // Wert: 0-250  Hysterese für Stickausschlag
    { param_AchsKopplung1              ,   1 },    //  unsigned char char AchsKopplung1;            // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
    { param_AchsKopplung2              ,   1 },    //  unsigned char char AchsKopplung2;            // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_CouplingYawCorrection      ,   1 },    //  unsigned char char CouplingYawCorrection;    // Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
    { param_WinkelUmschlagNick         ,   1 },    //  unsigned char char WinkelUmschlagNick;       // Wert: 0-250  180°-Punkt
    { param_WinkelUmschlagRoll         ,   1 },    //  unsigned char char WinkelUmschlagRoll;       // Wert: 0-250  180°-Punkt
    { param_GyroAccAbgleich            ,   1 },    //  unsigned char char GyroAccAbgleich;          // 1/k  (Koppel_ACC_Wirkung)
    { param_Driftkomp                  ,   1 },    //  unsigned char char Driftkomp;
    { param_DynamicStability           ,   1 },    //  unsigned char char DynamicStability;
    { param_UserParam5                 ,   1 },    //  unsigned char char UserParam5;               // Wert : 0-250
    { param_UserParam6                 ,   1 },    //  unsigned char char UserParam6;               // Wert : 0-250
    { param_UserParam7                 ,   1 },    //  unsigned char char UserParam7;               // Wert : 0-250
    { param_UserParam8                 ,   1 },    //  unsigned char char UserParam8;               // Wert : 0-250
    { param_J16Bitmask                 ,   1 },    //  unsigned char char J16Bitmask;               // for the J16 Output
    { param_J16Timing                  ,   1 },    //  unsigned char char J16Timing;                // for the J16 Output
    { param_J17Bitmask                 ,   1 },    //  unsigned char char J17Bitmask;               // for the J17 Output
    { param_J17Timing                  ,   1 },    //  unsigned char char J17Timing;                // for the J17 Output
    { param_WARN_J16_Bitmask           ,   1 },    //  unsigned char char WARN_J16_Bitmask;         // for the J16 Output
    { param_WARN_J17_Bitmask           ,   1 },    //  unsigned char char WARN_J17_Bitmask;         // for the J17 Output
    { param_AutoPhotoDistance          ,   1 },    //  unsigned char char AutoPhotoDistance;        // Auto Photo   // ab Rev. 100 (ehemals NaviOut1Parameter)
    { param_NaviGpsModeChannel         ,   1 },    //  unsigned char char NaviGpsModeChannel;       // Parameters for the Naviboard
    { param_NaviGpsGain                ,   1 },    //  unsigned char char NaviGpsGain;
    { param_NaviGpsP                   ,   1 },    //  unsigned char char NaviGpsP;
    { param_NaviGpsI                   ,   1 },    //  unsigned char char NaviGpsI;
    { param_NaviGpsD                   ,   1 },    //  unsigned char char NaviGpsD;
    { param_NaviGpsPLimit              ,   1 },    //  unsigned char char NaviGpsPLimit;
    { param_NaviGpsILimit              ,   1 },    //  unsigned char char NaviGpsILimit;
    { param_NaviGpsDLimit              ,   1 },    //  unsigned char char NaviGpsDLimit;
    { param_NaviGpsA                   ,   1 },    //  unsigned char char NaviGpsA;
    { param_NaviGpsMinSat              ,   1 },    //  unsigned char char NaviGpsMinSat;
    { param_NaviStickThreshold         ,   1 },    //  unsigned char char NaviStickThreshold;
    { param_NaviWindCorrection         ,   1 },    //  unsigned char char NaviWindCorrection;
    { param_NaviAccCompensation        ,   1 },    //  unsigned char char NaviAccCompensation;      // New since 0.86 -> was: SpeedCompensation
    { param_NaviMaxFlyingRange         ,   1 },    //  unsigned char char NaviMaxFlyingRange;       // in 10m
    { param_NaviAngleLimitation        ,   1 },    //  unsigned char char NaviAngleLimitation;
    { param_NaviPH_LoginTime           ,   1 },    //  unsigned char char NaviPH_LoginTime;
    { param_NaviDescendRange           ,   1 },    //  unsigned char char NaviDescendRange;
    { param_ExternalControl            ,   1 },    //  unsigned char char ExternalControl;          // for serial Control
    { param_OrientationAngle           ,   1 },    //  unsigned char char OrientationAngle;         // Where is the front-direction?
    { param_CareFreeChannel            ,   1 },    //  unsigned char char CareFreeChannel;          // switch for CareFree
    { param_MotorSafetySwitch          ,   1 },    //  unsigned char char MotorSafetySwitch;
    { param_MotorSmooth                ,   1 },    //  unsigned char char MotorSmooth;
    { param_ComingHomeAltitude         ,   1 },    //  unsigned char char ComingHomeAltitude;
    { param_FailSafeTime               ,   1 },    //  unsigned char char FailSafeTime;
    { param_MaxAltitude                ,   1 },    //  unsigned char char MaxAltitude;
    { param_FailsafeChannel            ,   1 },    //  unsigned char char FailsafeChannel;          // if the value of this channel is > 100, the MK reports "RC-Lost"
    { param_ServoFilterNick            ,   1 },    //  unsigned char char ServoFilterNick;
    { param_ServoFilterRoll            ,   1 },    //  unsigned char char ServoFilterRoll;
    { param_Servo3OnValue              ,   1 },    //  unsigned char char Servo3OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo3OffValue             ,   1 },    //  unsigned char char Servo3OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_Servo4OnValue              ,   1 },    //  unsigned char char Servo4OnValue;            // ab Rev. 101 (FC 2.05f)
    { param_Servo4OffValue             ,   1 },    //  unsigned char char Servo4OffValue;           // ab Rev. 101 (FC 2.05f)
    { param_ServoFS_Pos                ,   5 },    //  unsigned char ServoFS_Pos[5];                // ab Rev. 105 (FC 2.09i)
    { param_StartLandChannel           ,   1 },    //  unsigned char char StartLandChannel;
    { param_LandingSpeed               ,   1 },    //  unsigned char char LandingSpeed;
    { param_CompassOffset              ,   1 },    //  unsigned char char CompassOffset;
    { param_AutoLandingVoltage         ,   1 },    //  unsigned char char AutoLandingVoltage;       // in 0,1V  0 -> disabled
    { param_ComingHomeVoltage          ,   1 },    //  unsigned char char ComingHomeVoltage;        // in 0,1V  0 -> disabled
    { param_AutoPhotoAtitudes          ,   1 },    //  unsigned char char AutoPhotoAtitudes;        // ab Rev. 100
    { param_SingleWpSpeed              ,   1 },    //  unsigned char char SingleWpSpeed;            // ab Rev. 100
    { param_LandingPulse               ,   1 },    //  unsigned char char LandingPulse;             // ab Rev. 104 (FC 2.09d)
    { param_SingleWpControlChannel     ,   1 },    //  unsigned char SingleWpControlChannel;        // ab Rev. 106 (FC2.11a)
    { param_MenuKeyChannel             ,   1 },    //  unsigned char MenuKeyChannel;                // ab Rev. 106 (FC2.11a)
    { param_CamCtrlModeChannel         ,   1 },    //  unsigned char CamCtrlModeChannel;            // ab Rev. 107 (FC2.13f)
    { param_CamCtrlZoomChannel         ,   1 },    //  unsigned char CamCtrlZoomChannel;            // ab Rev. 107 (FC2.13f)
	{ param_FailsafeAltitude		   ,   1 },	   //  unsigned char FailsSafeAltitude;    		    // ab Rev. 111 (FC2.19)
	{ param_GimbalYawChannel		   ,   1 },    //  unsigned char GimbalYawChannel			    // ab Rev. 112 )FC2.19)
	{ param_GimbalOut1Channel		   ,   1 },    //  unsigned char GimbalOut1Channel;				// ab Rev. 113 (FC2.20)
	{ param_GimbalOut2Channel		   ,   1 },    //  unsigned char GimbalOut2Channel;				// ab Rev. 113 (FC2.20)
	{ param_ExternalGpsChannel		   ,   1 },    //  unsigned char ExternalGpsChannel;			// ab Rev. 114 (FC2.21)
	{ param_reserved				   ,  27 },	   //  unsigned char reserved[32];       			// for later use ab Rev. 108 (FC2.14a)
    { param_BitConfig                  ,   1 },    //  unsigned char char BitConfig;                // (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    { param_ServoCompInvert            ,   1 },    //  unsigned char char ServoCompInvert;          // 0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
    { param_ExtraConfig                ,   1 },    //  unsigned char char ExtraConfig;              // bitcodiert
    { param_GlobalConfig3              ,   1 },    //  unsigned char char GlobalConfig3;            // bitcodiert
    { param_Name                       ,  12 },    //  char               Name[12];
    { param_crc                        ,   1 },    //  unsigned char char crc;                      // must be the last byte!

    { param_EOF                        ,   0 }     // END OF PARAM-LIST - MUST BE THE LAST!!

};  // end: paramset_114[]

//#############################################################################################
//# 1b. Zuweisungstabelle: Revision -> paramset_xxx
//#############################################################################################

typedef struct
{
    unsigned char Revision;
    const paramRevItem_t *RevisionTable;
} paramsetRevMap_t;

#define MAPEOF      255


//----------------------------------------------------------------
// Mappingtabelle zwischen FC-Revision und Paramset-Tabelle
//
// Anmerkung bzgl. Groesse:
//  Angenommen man wuerde die Rev 97 bis 90 weglassen, dann
//  wuerde man ca. 1 KByte sparen
//----------------------------------------------------------------
paramsetRevMap_t paramsetRevMap[] =
{
  {  114   , paramset_114 },    // 2.21c
  {  113   , paramset_113 },    // 2.20
  {  112   , paramset_112 },    // 2.19
  {  110   , paramset_110 },    // 2.18
  {  109   , paramset_109 },    // 2.16
  {  108   , paramset_108 },    // 2.14a
  {  107   , paramset_107 },    // 2.13a
  {  106   , paramset_106 },    // 2.11a
  {  105   , paramset_105 },    // 2.09j
  {  104   , paramset_104 },    // 2.09d
  {  103   , paramset_103 },    // 2.07f
  {  102   , paramset_102 },    // 2.05g (bis min. 2.06b)
  {  101   , paramset_101 },    // 2.05f (eingeschraenkte Unterstuetzung - ) nur in einer einzigen Betaversion vorhanden!)
  {  100   , paramset_100 },    // 2.05e (Anmerkung OG 06.04.2014: ggf. spaeter loeschen um Platz zu sparen da diese Version nur in einigen wenigen Betaversionen vorhanden ist)
  // Rev. 99: nicht vorhanden; ignorieren
  {  98    , paramset_098 },    // 2.03d
  {  97    , paramset_097 },    // 2.02b
  {  96    , paramset_095 },    // ???                  (keine Struktur-Aenderung zu 095)
  {  95    , paramset_095 },    // 0.90L, 2.00a, 2.00e
  {  94    , paramset_094 },    // eingeschraenkte Unterstuetzung; 0.91a
  {  93    , paramset_093 },    // eingeschraenkte Unterstuetzung; 0.90e, 0.90g, 0.90j
  {  92    , paramset_093 },    // eingeschraenkte Unterstuetzung; 0.90d                (keine Struktur-Aenderung zu 093)
  {  91    , paramset_091 },    // eingeschraenkte Unterstuetzung; 0.88m, 0.88n
  {  90    , paramset_091 },    // eingeschraenkte Unterstuetzung; 0.88e                (keine Struktur-Aenderung zu 091)

  {  MAPEOF, 0            }     // END OF LIST - MUST BE THE LAST!!
};




//#############################################################################################
//# 2. Konfigurationstabelle der paramSubID's (Bit- und Bytefelder)
//#############################################################################################

//---------------------------------------
// struct: einzelnes Parameter-SubItem
//---------------------------------------
typedef struct
{
    unsigned char paramSubID;       // groesser/gleich PARAMSUBITEMS
    unsigned char paramID;          // -> mapping auf entsprechende paramID
    unsigned char subType;          // SUBTYPE_BIT  oder  SUBTYPE_BYTE
    unsigned char subIndex;         // bei SUBTYPE_BIT: Bitmask  z.b. 0x02
                                    // bei SUBTYPE_BYTE: index des Bytes (siehe param_Kanalbelegung)
    unsigned char von_Revision;     // ab welcher FC-Revision ist das SubItem vorhanden (0=immer)
    unsigned char bis_Revision;     // bis zu welcher FC-Revision ist das SubItem vorhanden (0=immer)
} paramSubItem_t;


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                                                                     +- von_Revision
//                                                                                                                     |
//  +- paramSubID                             +- gehoert zu?         +- Bit/Byte?   +- Bit/Byte der paramID            |  +- bis_Revision
//  |                                         |                      |              |                                  |  |
//--|-----------------------------------------|----------------------|--------------|----------------------------------|--|----------------------------------------------
paramSubItem_t const paramSubItem[] PROGMEM =
{
  { param_ServoCompInvert_SERVO_NICK_INV    , param_ServoCompInvert, SUBTYPE_BIT,   SERVO_NICK_INV                  ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: SVNick                        0x01
  { param_ServoCompInvert_SERVO_ROLL_INV    , param_ServoCompInvert, SUBTYPE_BIT,   SERVO_ROLL_INV                  ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: SVRoll                        0x02
  { param_ServoCompInvert_SERVO_RELATIVE    , param_ServoCompInvert, SUBTYPE_BIT,   SERVO_RELATIVE                  ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: SVRelMov                      0x04

  { param_ExtraConfig_HeightLimit           , param_ExtraConfig,     SUBTYPE_BIT,   CFG2_HEIGHT_LIMIT               ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG2_HEIGHT_LIMIT             0x01
  { param_ExtraConfig_HeightVario           , param_ExtraConfig,     SUBTYPE_BITN,  CFG2_HEIGHT_LIMIT               ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG2_HEIGHT_LIMIT             0x01 negiert zu param_ExtraConfig_HeightLimit
  { param_ExtraConfig_VarioBeep             , param_ExtraConfig,     SUBTYPE_BIT,   CFG2_VARIO_BEEP                 ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG2_VARIO_BEEP               0x02
  { param_ExtraConfig_SensitiveRc           , param_ExtraConfig,     SUBTYPE_BIT,   CFG_SENSITIVE_RC                ,  0,109},    // SUBTYPE_BIT - mk-data-structs.h: CFG_SENSITIVE_RC              0x04
  { param_ExtraConfig_FlyZone_Required      , param_ExtraConfig,     SUBTYPE_BIT,   CFG_FLYZONE_REQUIRED            ,110,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_FLYZONE_REQUIRED         0x04

  { param_ExtraConfig_33vReference          , param_ExtraConfig,     SUBTYPE_BIT,   CFG_3_3V_REFERENCE              ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_3_3V_REFERENCE            0x08
  { param_ExtraConfig_NoRcOffBeeping        , param_ExtraConfig,     SUBTYPE_BIT,   CFG_NO_RCOFF_BEEPING            ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_NO_RCOFF_BEEPING          0x10
  { param_ExtraConfig_GpsAid                , param_ExtraConfig,     SUBTYPE_BIT,   CFG_GPS_AID                     ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_GPS_AID                   0x20
  { param_ExtraConfig_LearnableCarefree     , param_ExtraConfig,     SUBTYPE_BIT,   CFG_LEARNABLE_CAREFREE          ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_LEARNABLE_CAREFREE        0x40
  { param_ExtraConfig_IgnoreMagErrAtStartup , param_ExtraConfig,     SUBTYPE_BIT,   CFG_IGNORE_MAG_ERR_AT_STARTUP   ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_IGNORE_MAG_ERR_AT_STARTUP 0x80

  { param_BitConfig_LoopOben                , param_BitConfig,       SUBTYPE_BIT,   CFG_LOOP_OBEN                   ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_LOOP_OBEN                 0x01
  { param_BitConfig_LoopUnten               , param_BitConfig,       SUBTYPE_BIT,   CFG_LOOP_UNTEN                  ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_LOOP_UNTEN                0x02
  { param_BitConfig_LoopLinks               , param_BitConfig,       SUBTYPE_BIT,   CFG_LOOP_LINKS                  ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_LOOP_LINKS                0x04
  { param_BitConfig_LoopRechts              , param_BitConfig,       SUBTYPE_BIT,   CFG_LOOP_RECHTS                 ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_LOOP_RECHTS               0x08
  { param_BitConfig_Led1_MotorRun           , param_BitConfig,       SUBTYPE_BIT,   CFG_LED1_MOTOR_RUN              ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_LED1_MOTOR_RUN              0x10
  { param_BitConfig_MotorOffLed1            , param_BitConfig,       SUBTYPE_BIT,   CFG_MOTOR_OFF_LED1              ,  0,101},    // SUBTYPE_BIT - mk-data-structs.h: CFG_MOTOR_OFF_LED1            0x20
  { param_BitConfig_MotorOffLed2            , param_BitConfig,       SUBTYPE_BIT,   CFG_MOTOR_OFF_LED2              ,  0,101},    // SUBTYPE_BIT - mk-data-structs.h: CFG_MOTOR_OFF_LED2            0x40
  { param_BitConfig_Led2_MotorRun           , param_BitConfig,       SUBTYPE_BIT,   CFG_LED2_MOTOR_RUN              ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_LED2_MOTOR_RUN              0x80

  { param_GlobalConfig3_NoSdCardNoStart     , param_GlobalConfig3,   SUBTYPE_BIT,   CFG3_NO_SDCARD_NO_START         ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG3_NO_SDCARD_NO_START       0x01
  { param_GlobalConfig3_DphMaxRadius        , param_GlobalConfig3,   SUBTYPE_BIT,   CFG3_DPH_MAX_RADIUS             ,  0,100},    // SUBTYPE_BIT - mk-data-structs.h: CFG3_DPH_MAX_RADIUS           0x02
  { param_GlobalConfig3_RiseFirstWP         , param_GlobalConfig3,   SUBTYPE_BIT,   CFG3_RISE_FIRST_WAYPOINT        ,109,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG3_RISE_FIRST_WAYPOINT           0x02
  { param_GlobalConfig3_VarioFailsafe       , param_GlobalConfig3,   SUBTYPE_BIT,   CFG3_VARIO_FAILSAFE             ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG3_VARIO_FAILSAFE           0x04
  { param_GlobalConfig3_MotorSwitchMode     , param_GlobalConfig3,   SUBTYPE_BIT,   CFG3_MOTOR_SWITCH_MODE          ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG3_MOTOR_SWITCH_MODE        0x08
  { param_GlobalConfig3_NoGpsFixNoStart     , param_GlobalConfig3,   SUBTYPE_BIT,   CFG3_NO_GPSFIX_NO_START         ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG3_NO_GPSFIX_NO_START       0x10
  { param_GlobalConfig3_UseNcForOut1        , param_GlobalConfig3,   SUBTYPE_BIT,   CFG3_USE_NC_FOR_OUT1            ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG3_USE_NC_FOR_OUT1          0x20
  { param_GlobalConfig3_SpeakAll            , param_GlobalConfig3,   SUBTYPE_BIT,   CFG3_SPEAK_ALL                  ,  0,110},    // SUBTYPE_BIT - mk-data-structs.h: CFG3_SPEAK_ALL                0x40 6.11.2017 bis EEprom 110
  { param_GlobalConfig3_ServoNickCompOff    , param_GlobalConfig3,   SUBTYPE_BIT,   CFG3_SERVO_NICK_COMP_OFF        , 96,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG3_SERVO_NICK_COMP_OFF      0x80

  { param_GlobalConfig_HoehenRegelung       , param_GlobalConfig,    SUBTYPE_BIT,   CFG_HOEHENREGELUNG              ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_HOEHENREGELUNG            0x01
  { param_GlobalConfig_HoehenSchalter       , param_GlobalConfig,    SUBTYPE_BIT,   CFG_HOEHEN_SCHALTER             ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_HOEHEN_SCHALTER           0x02
  { param_GlobalConfig_HeadingHold          , param_GlobalConfig,    SUBTYPE_BIT,   CFG_HEADING_HOLD                ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_HEADING_HOLD              0x04
  { param_GlobalConfig_KompassAktiv         , param_GlobalConfig,    SUBTYPE_BIT,   CFG_KOMPASS_AKTIV               ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_KOMPASS_AKTIV             0x08
  { param_GlobalConfig_KompassFix           , param_GlobalConfig,    SUBTYPE_BIT,   CFG_KOMPASS_FIX                 ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_KOMPASS_FIX               0x10
  { param_GlobalConfig_GpsAktiv             , param_GlobalConfig,    SUBTYPE_BIT,   CFG_GPS_AKTIV                   ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_GPS_AKTIV                 0x20
  { param_GlobalConfig_AchsenkopplungAktiv  , param_GlobalConfig,    SUBTYPE_BIT,   CFG_ACHSENKOPPLUNG_AKTIV        ,  0,108},    // SUBTYPE_BIT - mk-data-structs.h: CFG_ACHSENKOPPLUNG_AKTIV      0x40
  { param_GlobalConfig_LandOutsideFlyzone   , param_GlobalConfig,    SUBTYPE_BIT,   CFG_LAND_OUTSIDE_FLYZONE        ,110,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_LAND_OUTSIDE_FLYZONE      0x40
  { param_GlobalConfig_DrehratenBegrenzer   , param_GlobalConfig,    SUBTYPE_BIT,   CFG_DREHRATEN_BEGRENZER         ,  0,  0},    // SUBTYPE_BIT - mk-data-structs.h: CFG_DREHRATEN_BEGRENZER       0x80

  { param_Kanalbelegung_Nick                , param_Kanalbelegung,   SUBTYPE_BYTE,   0                              ,  0,  0},    // -> Kanalbelegung[12]
  { param_Kanalbelegung_Roll                , param_Kanalbelegung,   SUBTYPE_BYTE,   1                              ,  0,  0},
  { param_Kanalbelegung_Gas                 , param_Kanalbelegung,   SUBTYPE_BYTE,   2                              ,  0,  0},
  { param_Kanalbelegung_Gear                , param_Kanalbelegung,   SUBTYPE_BYTE,   3                              ,  0,  0},
  { param_Kanalbelegung_Poti1               , param_Kanalbelegung,   SUBTYPE_BYTE,   4                              ,  0,  0},
  { param_Kanalbelegung_Poti2               , param_Kanalbelegung,   SUBTYPE_BYTE,   5                              ,  0,  0},
  { param_Kanalbelegung_Poti3               , param_Kanalbelegung,   SUBTYPE_BYTE,   6                              ,  0,  0},
  { param_Kanalbelegung_Poti4               , param_Kanalbelegung,   SUBTYPE_BYTE,   7                              ,  0,  0},
  { param_Kanalbelegung_Poti5               , param_Kanalbelegung,   SUBTYPE_BYTE,   8                              ,  0,  0},
  { param_Kanalbelegung_Poti6               , param_Kanalbelegung,   SUBTYPE_BYTE,   9                              ,  0,  0},
  { param_Kanalbelegung_Poti7               , param_Kanalbelegung,   SUBTYPE_BYTE,  10                              ,  0,  0},
  { param_Kanalbelegung_Poti8               , param_Kanalbelegung,   SUBTYPE_BYTE,  11                              ,  0,  0},

  { param_CompassOffset_DisableDeclCalc     , param_CompassOffset,   SUBTYPE_BYTE,   0                              ,  0,  0},    // ist in Bit 8 und 7 von CompassOffset kodiert
  { param_ComingHomeOrientation             , param_ServoCompInvert, SUBTYPE_BYTE,   0                              ,  0,  0},    // ist in Bit 5 und 4 von CervoCompInvert

  { param_ServoNickFailsave                 , param_ServoFS_Pos,     SUBTYPE_BYTE,   0                              ,  0,  0},
  { param_ServoRollFailsave                 , param_ServoFS_Pos,     SUBTYPE_BYTE,   1                              ,  0,  0},
  { param_Servo3Failsave                    , param_ServoFS_Pos,     SUBTYPE_BYTE,   2                              ,  0,  0},
  { param_Servo4Failsave                    , param_ServoFS_Pos,     SUBTYPE_BYTE,   3                              ,  0,  0},
  { param_Servo5Failsave                    , param_ServoFS_Pos,     SUBTYPE_BYTE,   4                              ,  0,  0},

  { param_EOF , 0,0,0,0,0 }     // END - MUST BE THE LAST!!
};  // end: paramSubItem[]



//#############################################################################################
//# 3a. Transformations-Tabelle
//#############################################################################################

//---------------------------------------
// struct: einzelnes Parameter-SubItem
//---------------------------------------
typedef struct
{
    unsigned char paramID;          // paramID oder paramSubID
    unsigned char (*transformfunc)( uint8_t cmd, unsigned char value, unsigned char newvalue );   // Edit-Funktion (z.B. editGeneric()); cmd = CMD_EDIT oder CMD_SHORTVALUE
} paramTransform_t;



//---------------------------------------
// forward Deklarationen fuer transform
//---------------------------------------
unsigned char transform_CompassOffset_DisableDeclCalc( uint8_t cmd, unsigned char value, unsigned char newvalue );
unsigned char transform_ComingHomeOrientation( uint8_t cmd, unsigned char value, unsigned char newvalue );
//unsigned char transform_ValueACCZ( uint16_t cmd, uint16_t value, uint16_t newvalue );

//---------------------------------------
//---------------------------------------
paramTransform_t const paramTransform[] =
{
  { param_CompassOffset_DisableDeclCalc , &transform_CompassOffset_DisableDeclCalc },
  { param_ComingHomeOrientation , &transform_ComingHomeOrientation },
//  { param_LandingPulse, &transform_ValueACCZ},

  { param_EOF , NULL }     // END - MUST BE THE LAST!!
};




//#############################################################################################
//# 3b. Transformations-Funktionen
//#############################################################################################

//---------------------------------------
// transform_CompassOffset_DisableDeclCalc()
//
// PARAMETER:
//   cmd     : GETVALUE || SETVALUE
//   value   :
//   newvalue: nur wenn cmd == SETVALUE
//
// Der true/false (Ja/Nein) Wert von param_CompassOffset_DisableDeclCalc
// ist in dem Byte von param_CompassOffset in den Bits 7 & 8 einkodiert.
//---------------------------------------
unsigned char transform_CompassOffset_DisableDeclCalc( uint8_t cmd, unsigned char value, unsigned char newvalue )
{
    uint8_t  bit7;
    uint8_t  bit8;

    bit7 = ((value & 0x40) ? true : false);
    bit8 = ((value & 0x80) ? true : false);

    if( cmd == GETVALUE )
    {
        // Bit 8 == Bit 7: Disable dec. Calc  AUS
        // Bit 8 != Bit 7: Disable dec. Calc  AN
        value = ((bit8 == bit7) ? 0 : 1);
    }

    if( cmd == SETVALUE )
    {
        if( newvalue )  bit8 = !bit7;                   // Bit 8 != Bit 7: Disable dec. Calc  AN
        else            bit8 = bit7;                    // Bit 8 == Bit 7: Disable dec. Calc  AUS

        if( bit8 )  value = (value | 0x80);             // Bit 8 setzen
        else        value = (value & (0x80 ^ 0xff));    // Bit 8 loeschen
    }


    //lcdx_printf_at_P( 0, 7, MINVERS, 0,0 , PSTR(" %d => %d "), value, newvalue );

    return value;
}

//---------------------------------------
// transform_ComingHomeOrientation()
//
// PARAMETER:
//   cmd     : GETVALUE || SETVALUE
//   value   :
//   newvalue: nur wenn cmd == SETVALUE
//
//
// ist in dem Byte von param_ServoCompInvert in den Bits 4 & 5 einkodiert,  Bits 1-3 bleiben unverändert
//---------------------------------------

uint8_t  save =0; // globale Variable zum Sichern des alten Byte ServoCompInvert Wertes

unsigned char transform_ComingHomeOrientation( uint8_t cmd, unsigned char value, unsigned char newvalue )
{

    if( cmd == GETVALUE )
    {
        save = value;                              // altes "gemeinsames" Byte ServoCompInvert sichern
        value =((value & 0x18) >> 3);              // CominghomeOrientation Bits nach rechts verschieben
    }

    if( cmd == SETVALUE )
    {
          value = ((newvalue << 3)|(save & 0x07)); // CominghomeOrientation Bits nach links verschieben und "alte" Bits 1-3 wieder dazufï¿½gen
    }


    //lcdx_printf_at_P( 0, 7, MINVERS, 0,0 , PSTR(" %d => %d "), value, newvalue );

    return value;
}

//
//
////---------------------------------------
//// transform_ValueACCZ()
////
//// PARAMETER:
////   cmd     : GETVALUE || SETVALUE
////   value   :
////   newvalue: nur wenn cmd == SETVALUE
////
////
////  ACC Z Landing Pulse Wert für die Anzeige x 4
////---------------------------------------
//
//
//unsigned char transform_ValueACCZ( uint16_t cmd, uint16_t value, unint16_t newvalue )
//
//uint16_t  save16 =0; // globale Variable zum Sichern des alten Byte ServoCompInvert Wertes
//
//{
//    if( cmd == GETVALUE )
//    {
//        save16 = value;                              // altes Byte ACC Z Landing Pulse sichern
//        value = (value * 4);                       // Wert x 4
//    }
//
//    if( cmd == SETVALUE )
//    {
//          value = (newvalue / 4);                  // Neuer Wert ACC Z Landingpulse /4
//    }
//
//
//    return value;
//}

//#############################################################################################
//# 4a. interne Zugriffsfunktionen
//#############################################################################################

//--------------------------------------------------------------
// INTERN
//
// Parameter:
//  paramSubID: 'echter' Parameter zum Suchen in paramSubItem[] zum
//              nachfolgeden Parameter geben das Suchergebnis zurueck
//              und sind alle 0 wenn nichts gefunden wurde
//
// Rueckgabe:
//    true/false
//--------------------------------------------------------------
unsigned char _paramset_getsubitemid( unsigned char paramSubID,      unsigned char *paramID, unsigned char *subType, unsigned char *subIndex )
{
    unsigned char   id;
    unsigned char   *p;
    unsigned char   von_Revision;
    unsigned char   bis_Revision;

    *paramID  = 0;
    *subType  = 0;
    *subIndex = 0;

    p = (unsigned char *) paramSubItem;

    while( true )
    {
        id = pgm_read_byte(p+0);                        // paramSubItem[..].paramSubID - die aktuelle paramSubID

        if( (id == paramSubID) )                        // gefunden?
        {
            von_Revision = pgm_read_byte(p+4);          // paramSubItem[..].von_Revision;
            bis_Revision = pgm_read_byte(p+5);          // paramSubItem[..].bis_Revision;

            // in aktueller FC-Revision vorhanden?
            if( von_Revision && (MKVersion.paramsetRevision < von_Revision) )   return false;   // passt nicht zur aktuellen FC-Revision -> exit
            if( bis_Revision && (MKVersion.paramsetRevision > bis_Revision) )   return false;   // passt nicht zur aktuellen FC-Revision -> exit

            *paramID    = pgm_read_byte(p+1);           // paramSubItem[..].paramID;
            *subType    = pgm_read_byte(p+2);           // paramSubItem[..].subType;
            *subIndex   = pgm_read_byte(p+3);           // paramSubItem[..].subIndex;
            return true;
        }

        if( id == param_EOF )   break;

        p += sizeof( paramSubItem_t );
    }

    return false;
}




//--------------------------------------------------------------
// INTERN
//
// Parameter:
//  retItemSize: false = offset zurueckgeben (normaler Modus)
//               true  = size von paramID zurueckgeben (fuer paramSize())
//--------------------------------------------------------------
unsigned char _paramset_getoffsetX( unsigned char paramID, unsigned char retItemSize,      unsigned char *subType, unsigned char *subIndex )
{
    unsigned char   *p;
    unsigned char   offset;
    unsigned char   size;
    unsigned char   this_paramID;
    unsigned char   paramSubID;

    *subType  = SUBTYPE_NO;  // =0
    *subIndex = 0;


    size   = 0;
    offset = param_NOTFOUND;

    if( MKVersion.paramsetOK )
    {
        //-----------------------
        // eine paramSubID wurde uebergeben
        // -> ermittle zugehoerige paramID!
        //-----------------------
        if( (paramID >= param_SUBITEM) && (paramID != param_EOF) )
        {
            paramSubID = paramID;
            if( !_paramset_getsubitemid( paramSubID,   &paramID, subType, subIndex ) )
            {
                // keine paramID gefunden...
                if( retItemSize )   return size;        // size == 0
                else                return offset;      // offset == param_NOTFOUND
            }
        }


        //-----------------------
        // paramID suchen
        //-----------------------
        offset = 0;

        p = (unsigned char *) paramsetRevTable;

        while( true )
        {
            this_paramID = pgm_read_byte(p);     // die aktuelle paramID
            size         = pgm_read_byte(p+1);   // size von paramID

            // gefunden oder Ende
            if( (this_paramID == paramID) || (this_paramID == param_EOF) )
            {
                break;
            }

            offset += size;
            p += sizeof( paramRevItem_t );
        }


        // paramID nicht gefunden?
        if( (this_paramID == param_EOF) && (paramID != param_EOF) )
        {
            offset = param_NOTFOUND;
            size   = 0;
        }

    }

    if( retItemSize )   return size;
    else                return offset;
}



//--------------------------------------------------------------
// INTERN
//
// Parameter:
//  retItemSize == false: offset zurueckgeben (normaler Modus)
//  retItemSize == true : size von paramID zurueckgeben
//                        --> fuer paramSize()
//--------------------------------------------------------------
unsigned char _paramset_getoffset( unsigned char paramID, unsigned char retItemSize )
{
    unsigned char   subType;
    unsigned char   subIndex;

    return _paramset_getoffsetX( paramID, retItemSize,   &subType, &subIndex );
}



//--------------------------------------------------------------
// INTERN
//
// sucht/prueft ob eine Transform-Funktion vorhanden ist
//
// RUECKGABE:
//    Index: (Num 0..n) auf paramTransform[]
//           => param_EOF (=255) wenn keine transform-Funktion vorhanden
//--------------------------------------------------------------
unsigned char _paramset_gettransformIndex( unsigned char paramID )
{
    unsigned char i;

    i = 0;
    while( paramTransform[i].paramID != param_EOF )
    {
        if( paramTransform[i].paramID == paramID )      return i;
        if( paramTransform[i].paramID == param_EOF )    return param_EOF;
        i++;
    }

    return param_EOF;
}



//#############################################################################################
//# 4b. oeffentliche Zugriffsfunktionen fuer paramID / paramSubID  Elemente
//#
//# Hier sind die Getter/Setter um auf die einzelnen Werte der paramset-Struktur zuzugreifen.
//# Dazu muss vorher das richtige paramset initialisert worden sein!
//#
//# ACHTUNG!
//# Fuer die Daten wird direkt auf den RX-Buffer vom PKT zugegriffen!
//# ==> der Buffer darf NICHT durch andere Datenpakete bereits wieder ueberschrieben worden sein!
//#
//# Anmerkung:
//# Das obige 'Achtung' koennte man aendern durch eine Kopie des RX-Buffers. Das verbraucht
//# zusaetzlichen RAM-Speicher -> probieren wir es erstmal so...
//#############################################################################################

//--------------------------------------------------------------
// size = paramSize( paramID )
//--------------------------------------------------------------
unsigned char paramSize( unsigned char paramID )
{
    return( _paramset_getoffset(paramID,true) );   // true = size (in Bytes) der paramID zurueckgeben (nicht offset)
}



//--------------------------------------------------------------
// exist = paramExist( paramID )
//
// Rueckgabe:
//   true  : die paramID existiert im aktuellen paramset
//   false : die paramID existiert nicht
//--------------------------------------------------------------
unsigned char paramExist( unsigned char paramID )
{
    return( (_paramset_getoffset(paramID,true) != 0) );   // true = size (in Bytes) der paramID zurueckgeben (nicht offset)
}



//--------------------------------------------------------------
// pointer = paramGet_p( paramID )
//
// ACHTUNG! Nicht faehig fuer Transform!
//
// RUECKGABE:
//   pointer direkt auf~ein Byte im Parameterset
//   (paramSubID nicht moeglich!)
//--------------------------------------------------------------
unsigned char *paramGet_p( unsigned char paramID )
{
    unsigned char offset;

    offset =  _paramset_getoffset( paramID, false );  // false = offset zurueckgeben

    if( offset != param_NOTFOUND )
    {
        return( (unsigned char *) (mkparamset + offset) );  // Rueckgabe: Pointer auf die Daten
    }
    return 0;
}



//--------------------------------------------------------------
// value = paramGet( paramID )
//
// holt den Wert von paramID. paramSubID's werden dabei
// unterstuetzt (Bit- und Bytefelder).
//
// Bei Bit-Feldern (z.B. GlobalConfig3) wird eine enstprechende
// Bit-Maskierung automatisch durchgefuehrt
// Ergebnis: true (=1) oder false (=0)false (=0)
//
// Bei Byte-Feldern (z.B. Kanalbelegung) wird der Wert des
// entsprechenden Byte's zurueckgegeben
//
// Hinweis: anhand des Rueckgabewertes kann nicht ueberprueft
// werden ob die paramID gefunden wurde bzw. existiert!
// -> ggf. erst mit paramExist() pruefen ob der Wert existiert!
//--------------------------------------------------------------
unsigned char paramGet( unsigned char paramID )
{
    unsigned char offset;
    unsigned char subType;
    unsigned char subIndex;
    unsigned char value;
    unsigned char transformIdx;

    offset = _paramset_getoffsetX( paramID, false, &subType, &subIndex );       // false = offset zurueckgeben

    if( offset == param_NOTFOUND )  return false;                               // paramID nicht gefunden / nicht unterstuetzt

    value = (unsigned char) *(mkparamset + offset);                             // offset Inhalt lesen (paramID)

    //-------------------
    // subIndex: Bit
    //-------------------
    if( subType == SUBTYPE_BIT )
    {
        value = ((value & subIndex) ? true : false);                            // Rueckgabe: true/false des gewaehlten Bit's
    }

    //-------------------
    // subIndex: Bit negiert
    //-------------------
    if( subType == SUBTYPE_BITN )
    {
        value = ((value & subIndex) ? false : true);                            // Rueckgabe: true/false des gewaehlten Bit's (negiert)
    }

    //-------------------
    // subIndex: Byte
    //-------------------
    if( subType == SUBTYPE_BYTE )
    {
        value = (unsigned char) *(mkparamset + (offset+subIndex));              // subIndex Inhalt lesen
    }


    //-------------------
    // Transform
    //-------------------
    transformIdx = _paramset_gettransformIndex( paramID );
    if( transformIdx != param_EOF )
    {
        value = paramTransform[transformIdx].transformfunc( GETVALUE, value, value);
    }

    return value;       // ubyte zurueckgeben
}



//--------------------------------------------------------------
//--------------------------------------------------------------
unsigned char paramSet( unsigned char paramID, unsigned char newvalue )
{
    unsigned char offset;
    unsigned char subOffset = 0;
    unsigned char subType;
    unsigned char subIndex;
    unsigned char byte;
    unsigned char value;
    unsigned char transformIdx;

    value = newvalue;

    offset = _paramset_getoffsetX( paramID, false, &subType, &subIndex );   // false = offset zurueckgeben

    if( offset == param_NOTFOUND )  return false;                           // paramID nicht gefunden / nicht unterstuetzt

    //-------------------
    // subIndex: Bit
    //-------------------
    if( (subType == SUBTYPE_BIT) || (subType == SUBTYPE_BITN) )
    {
        byte = (unsigned char) *(mkparamset + offset);                      // offset Inhalt lesen

        if( subType == SUBTYPE_BITN )                                       // SUBTYPE_BITN = negiertes BIT
            value = (value ? false : true);                                 // negieren

        if(value)   byte = byte | subIndex;                                 // Bit setzen
        else        byte = byte & (subIndex ^ 0xff);                        // Bit loeschen

        value = byte;                                                       // neues Value fuer das gesamte Byte (mit eingerechnetem BIT)
    }

    //-------------------
    // subIndex: Byte
    //-------------------
    if( subType == SUBTYPE_BYTE )
    {
        subOffset = subIndex;                                               // das 'verschobene' Byte (hier nur den Offset verschieben)
    }


    //-------------------
    // Transform
    //-------------------
    transformIdx = _paramset_gettransformIndex( paramID );
    if( transformIdx != param_EOF )
    {
        byte  = (unsigned char) *(mkparamset + offset + subOffset);         // offset Inhalt lesen
        value = paramTransform[transformIdx].transformfunc( SETVALUE, byte, newvalue);
    }


    //-------------------
    // Byte speichern
    //-------------------
    *(mkparamset + offset + subOffset) = value;
    return true;                                                            // OK
}




//#############################################################################################
//# 4c. oeffentliche Zugriffsfunktionen fuer das gesamte Paramset (Int usw.)
//#
//# Init und Abfrage der Groesse in Bytes ddes gesamten Paramset's
//#############################################################################################


//--------------------------------------------------------------
// ok = paramsetInit( *pData )
//
// Ermittelt zu einer empfangenen Revision die passende paramset-Tabelle
// und setzt entsprechende Werte in MKVersion
//
// Wird von MK_load_setting() aufgerufen nach dem Einlesen der
// MK-Parameter (Setting-Request 'q')
//
// Parameter:
//   *pData : Zeiger direkt auf den RX-Buffer des PKT
//
// Rueckgabe:
//   true  : OK - Paramset ist initialisert; die aktuelle FC-Revision
//                steht in MKVersion.paramsetRevision
//   false : Fehler - keine passende paramset-Tabelle gefunden
//--------------------------------------------------------------
unsigned char paramsetInit( unsigned char *pData )
{
    unsigned char i;
    unsigned char *p;

    paramsetRevTable            = 0;
    mkparamset                  = 0;
    MKVersion.paramsetOK        = false;
    MKVersion.paramsetRevision  = 0;
    MKVersion.mksetting         = 0;
    MKVersion.mksettingName[0]  = 0;

    //--------------------------------------------------
    // pData == 0 -> nur 'reset' der Daten
    //--------------------------------------------------
    if( pData == 0 )
    {
        return 0;
    }

    //--------------------------------------------------
    // diese beiden Werte koennen in jedem Fall gesetzt werden
    // unabhaengig ob das gefundene Parameterset vom PKT
    // unterstuetzt wird oder nicht
    //--------------------------------------------------
    MKVersion.mksetting        = (unsigned char) *pData;
    MKVersion.paramsetRevision = (unsigned char) *(pData + 1);


    //--------------------------------------------------
    // FC-Revision in paramsetRevMap suchen
    //--------------------------------------------------
    i = 0;
    while( true )
    {
        if( (paramsetRevMap[i].Revision == MKVersion.paramsetRevision) || (paramsetRevMap[i].Revision == MAPEOF) )
            break;
        i++;
    }


    //--------------------------------------
    // Revision nicht gefunden -> return 0
    //--------------------------------------
    if( paramsetRevMap[i].Revision == MAPEOF )
    {
        return 0;
    }

    //--------------------------------------
    // Revision gefunden
    //--------------------------------------
    mkparamset              = (unsigned char *) (pData + 1);
    paramsetRevTable        = (paramRevItem_t *) paramsetRevMap[i].RevisionTable;
    MKVersion.paramsetOK    = true;


    //--------------------------------------
    // den aktuellen Setting-Namen kopieren
    // wird u.a. vom OSD verwendet
    //--------------------------------------
    p = paramGet_p( param_Name );
    if( p ) memcpy( MKVersion.mksettingName, p, 12 );

    return MKVersion.paramsetOK;
}



//--------------------------------------------------------------
// size = paramsetSize()
//
// Gibt die Groesse des gesamten aktuellen Parameterset's
// zurueck. Fuer Revision 098 z.B. 130 = 130 Bytes.
//
// Rueckgabe:
//   =0 : Fehler; nicht unterstuetzte FC-Revision oder nicht
//        initialisiert
//--------------------------------------------------------------
unsigned char paramsetSize( void )
{
    if( MKVersion.paramsetOK )
        return _paramset_getoffset( param_EOF, false );   // false = offset zurueckgeben; der offset von param_EOF entspricht der Groesse des paramSet's!
    else
        return 0;
}





//#############################################################################################
//# x. TEST / DEBUG
//#############################################################################################

//--------------------------------------------------------------
// TEST / DEBUG
//--------------------------------------------------------------
#ifdef DEBUG_PARAMSET
void paramsetDEBUG( void )
{
    lcd_cls();

    //lcdx_printp_at( 0, 0, PSTR("NEW PARAM TEST..."), MNORMAL,  0,0);
    lcd_printp_at(12, 7, strGet(ENDE), MNORMAL);                    // Keyline

    //memcpy( &MKVersion.NCVersion, (Version_t *) (pRxData), sizeof(MKVersion_t) );

/*

typedef struct
{
    Version_t       NCVersion;          //
    Version_t       FCVersion;          //
    unsigned char   paramsetRevision;   // von der FC
    unsigned char   paramsetOk;         // true wenn Revision in paramset.c vorhanden
} MKVersion_t;


//-------------------------------------
// zur Orientierung: Version_t
//-------------------------------------
//typedef struct
//{
//    unsigned char SWMajor;
//    unsigned char SWMinor;
//    unsigned char ProtoMajor;
//    unsigned char ProtoMinor;
//    unsigned char SWPatch;
//    unsigned char HardwareError[5];
//} __attribute__((packed)) Version_t;
*/

    //setting = MK_Setting_load( 0xff, 20);                               // aktuelles MK Setting ermitteln
    MK_Setting_load( 0xff, 20);                               // aktuelles MK Setting ermitteln

    //----------------------------------
    // Anzeige: FC/NC Version
    //----------------------------------
    lcdx_printp_at(      0, 0, PSTR("FC:"), MNORMAL, 0,0);
    MKVersion_print_at(  3, 0, FC, MNORMAL, 0,0);

    lcdx_printp_at(     13, 0, PSTR("NC:"), MNORMAL, 0,0);
    MKVersion_print_at( 16, 0, NC, MNORMAL, 0,0);


    //----------------------------------
    // Anzeige: FC Revision
    //----------------------------------
    lcd_printf_at_P( 0, 1, MNORMAL, PSTR("Rev: %03u"), MKVersion.paramsetRevision );
    if( MKVersion.paramsetOK )
        lcdx_printp_at( 8, 1, PSTR(" ok!"), MINVERS, 0,0);
    else
        lcdx_printp_at( 8, 1, PSTR(" err"), MINVERS, 0,0);

    // Size von paramset
    lcd_printf_at_P( 13, 1, MNORMAL, PSTR("Size=%3u"), paramsetSize() );


    //----------------------------------
    // Anzeige: aktuelles MK Setting
    //----------------------------------
    //lcd_printf_at_P( 0, 2, MNORMAL, PSTR("Set:%2u %s"), MKVersion.mksetting, MKVersion.mksettingName );

/*
    unsigned char *p;

    p = paramGet_p( param_Name );
    if( p )
    {
        lcd_printf_at_P( 0, 3, MNORMAL, PSTR("%c"), *p );
        p++;
        lcd_printf_at_P( 1, 3, MNORMAL, PSTR("%c"), *p );
        p++;
        lcd_printf_at_P( 2, 3, MNORMAL, PSTR("%c"), *p );
    }
*/

    //----------------------------------
    //----------------------------------

/*
    lcd_printf_at_P( 0, 3, MNORMAL, PSTR("Size Name:%3u"), paramSize(param_Name) );
    lcd_printf_at_P( 0, 5, MNORMAL, PSTR("%c"), paramGet_UByte( param_Name ) );
    paramSet_UByte( param_Name, 'x' );
    lcd_printf_at_P( 3, 5, MNORMAL, PSTR("%c"), paramGet_UByte( param_Name ) );
*/

/*
    uint8_t v;
    v = MKVersion_Cmp( MKVersion.FCVer,  2,0,'f' );

    lcd_printf_at_P( 0, 4, MNORMAL, PSTR("v:%3u"), v );
*/


/*
    unsigned char   paramID = 1;
    unsigned char   paramSubType = 1;
    unsigned char   paramSubIndex =1;

    _paramset_getsubitemid( param_GlobalConfig_DrehratenBegrenzer,  &paramID, &paramSubType, &paramSubIndex );

    //lcd_printf_at_P( 0, 4, MNORMAL, PSTR("ID:%u T:%u I:%u"), paramID, paramSubType, paramSubIndex );
*/

//#define param_ExtraConfig_NoRcOffBeeping      204      // SUBTYPE_BIT - mk-data-structs.h: CFG_NO_RCOFF_BEEPING          0x10
//#define param_ExtraConfig_GpsAid              205      // SUBTYPE_BIT - mk-data-structs.h: CFG_GPS_AID                   0x20
//#define param_ExtraConfig_LearnableCarefree     206      // SUBTYPE_BIT - mk-data-structs.h: CFG_LEARNABLE_CAREFREE        0x40
//#define param_ExtraConfig_IgnoreMagErrAtStartup 207      // SUBTYPE_BIT - mk-data-structs.h: CFG_IGNORE_MAG_ERR_AT_STARTUP 0x80

//#define param_GlobalConfig3_NoSdCardNoStart     220      // SUBTYPE_BIT - mk-data-structs.h: CFG3_NO_SDCARD_NO_START       0x01
//#define param_GlobalConfig3_NoGpsFixNoStart     224      // SUBTYPE_BIT - mk-data-structs.h: CFG3_NO_GPSFIX_NO_START       0x10

//paramGet( unsigned char paramID );

    //lcd_printf_at_P( 0, 5, MNORMAL, PSTR("%u %u %u"), paramGet(param_ExtraConfig_NoRcOffBeeping), paramGet(param_GlobalConfig3_NoSdCardNoStart), paramGet(param_GlobalConfig3_NoGpsFixNoStart) );

//#define param_Kanalbelegung_Gas               242      // SUBTYPE_BYTE - Kanalbelegung [2] (unsigned char)
//#define param_Kanalbelegung_Gear              243      // SUBTYPE_BYTE - Kanalbelegung [3] (unsigned char)
//#define param_Kanalbelegung_Nick              240      // SUBTYPE_BYTE - Kanalbelegung [0] (unsigned char)
//#define param_Kanalbelegung_Roll              241      // SUBTYPE_BYTE - Kanalbelegung [1] (unsigned char)

//    lcd_printf_at_P( 0, 4, MNORMAL, PSTR("%u %u %u %u"), paramGet(param_Kanalbelegung_Gas), paramGet(param_Kanalbelegung_Gear), paramGet(param_Kanalbelegung_Nick), paramGet(param_Kanalbelegung_Roll) );

//    lcd_printf_at_P( 0, 5, MNORMAL, PSTR("%u"), paramGet(param_Kanalbelegung) );


/*
    lcd_printf_at_P( 0, 4, MNORMAL, PSTR("%u %u %u"), paramGet(param_ExtraConfig_NoRcOffBeeping), paramGet(param_GlobalConfig3_NoSdCardNoStart), paramGet(param_GlobalConfig3_NoGpsFixNoStart) );

    unsigned char v;

    v = paramGet(param_GlobalConfig3_NoSdCardNoStart);
    v = (v ? false : true);
    paramSet(param_GlobalConfig3_NoSdCardNoStart, v);

    lcd_printf_at_P( 0, 5, MNORMAL, PSTR("%u %u %u"), paramGet(param_ExtraConfig_NoRcOffBeeping), paramGet(param_GlobalConfig3_NoSdCardNoStart), paramGet(param_GlobalConfig3_NoGpsFixNoStart) );

    v = paramGet(param_GlobalConfig3_NoSdCardNoStart);
    v = (v ? false : true);
    paramSet(param_GlobalConfig3_NoSdCardNoStart, v);

    lcd_printf_at_P( 0, 6, MNORMAL, PSTR("%u %u %u"), paramGet(param_ExtraConfig_NoRcOffBeeping), paramGet(param_GlobalConfig3_NoSdCardNoStart), paramGet(param_GlobalConfig3_NoGpsFixNoStart) );
*/

/*
    lcd_printf_at_P( 0, 3, MNORMAL, PSTR("%u %u"),
         paramGet( param_Kanalbelegung_Gas  ),
         paramGet( param_Kanalbelegung_Gear )
       );

    lcd_printf_at_P( 0, 4, MNORMAL, PSTR("%u %u %u %u"),
         paramGet( param_Kanalbelegung_Poti1  ),
         paramGet( param_Kanalbelegung_Poti2  ),
         paramGet( param_Kanalbelegung_Poti3  ),
         paramGet( param_Kanalbelegung_Poti4  )
       );

    lcd_printf_at_P( 0, 5, MNORMAL, PSTR("%u %u %u %u"),
         paramGet( param_Kanalbelegung_Poti5  ),
         paramGet( param_Kanalbelegung_Poti6  ),
         paramGet( param_Kanalbelegung_Poti7  ),
         paramGet( param_Kanalbelegung_Poti8  )
       );
*/

//    lcd_printf_at_P( 0, 3, MNORMAL, PSTR("HR:%u  H-Limit:%u"),
//         paramGet( param_GlobalConfig_HoehenRegelung  ),
//         paramGet( param_ExtraConfig_HeightLimit  )
//       );

//    lcd_printf_at_P( 0, 5, MNORMAL, PSTR("Auto-S-L:%u"),
//         paramGet( param_StartLandChannel    )
//       );

    lcd_printf_at_P( 0, 4, MNORMAL, PSTR("CH Orientation:%x"), paramGet( param_ServoCompInvert));

    lcd_printf_at_P( 0, 5, MNORMAL, PSTR("ACC Land pulse:%x"), paramGet( param_LandingPulse));


       ;

    while( true )
    {
        PKT_CtrlHook();
        if (get_key_press (1 << KEY_ESC))
        {
            break;
        }
    }
}
#endif // #ifdef DEBUG_PARAMSET

