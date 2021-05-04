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
//# HISTORY  mk_data-stucts.h
//#
//# 16.07.2018 Cebra
//# - chg : CFG_MOTOR_BLINK1 zu CFG_LED1_MOTOR_RUN
//# - chg : CFG_MOTOR_BLINK2 zu CFG_LED2_MOTOR_RUN
//#
//# 13.10.2017 Cebra
//# - add : CFG3_RISE_FIRST_WAYPOINT 0x02, EEpromversion 109
//# - add : CFG_LAND_OUTSIDE_FLYZONE, CFG_FLYZONE_REQUIRED  , EEProm Version 110
//#
//# 20.01.2016 Cebra
//# - add: neue Datastrukturen NC 2.13, External Control und struct NaviData_Tiny_t
//#
//# 16.07.2015 Cebra
//# - chg: Receiver Selection
//#        RECEIVER_USER  8  -> RECEIVER_MLINK 8
//# - add: Receiver Selection
//#        RECEIVER_USER  9
//#
//# 26.01.2015 Cebra
//# - chg: #defines für ServoCompInvert an aktuelle Syntax angepasst
//#        SVNick   -> SERVO_NICK_INV
//#        SVRoll   -> SERVO_ROLL_INV
//#        SVRelMov -> SERVO_RELATIVE
//# - add: #define CH_DIRECTION_1 0x08  für ComingHome Ausrichtung
//#        #define CH_DIRECTION_2 0x10
//#
//# 19.06.2014 OG
//# - chg: Aktualisiert: struct Point_t (aus Quelle: NaviCtrl V2.06f/waypoints.h - Revision 546)
//#
//# 12.02.2014 OG
//# - del: die alten defines bzgl. MKVERSIONnnn und mk_param_struct_t entfernt
//#
//# 02.12.2013 Cebra
//# - add: #define MKVERSION201f, Erweiterung für FC201f
//#
//# 21.10.2013 Cebra
//# - add: #define MKVERSION201a, Erweiterung für FC201a
//#
//# 26.06.2013 OG
//# - chg: "0.91f" -> "0.91f/l"
//#
//# 13.06.2013 OG
//# - chg: "0.90j" -> "0.90h/j"
//# - chg: Code-Layout
//#
//# 05.06.2013 Cebra
//# - chg: Anpassungen an FC091c
//#
//# 03.06.2013 Cebra
//# - chg: #define Fehler beseitigt
//#
//# 30.05.2013 Cebra
//# - add: #define MKVERSION091a, Erweiterung für FC091a
//#
//# 13.05.2013 Cebra
//# - add: #define MKVERSION090h    // wegen GPS-Zeitausgabe NC
//#
//# 11.05.2013 OG
//# - add: DateTime_t
//#
//# 20.03.2013 OG
//# - add: BLData_t (http://www.mikrokopter.de/ucwiki/en/SerialCommands/BLDataStruct)
//############################################################################


#ifndef _MK_DATA_STRUCTS_H
#define _MK_DATA_STRUCTS_H

#include "main.h"

//##################################################################################

//##################################################################################


#define u8 uint8_t
#define s8 int8_t
#define u16 uint16_t
#define s16 int16_t
#define u32 uint32_t
#define s32 int32_t

#define NUMBER_OF_DEBUG_DATAS 32
#define ANALOG_NAME_LENGTH 16


// Version of supported serial protocol
#define MIN_VERSION 7
#define MAX_VERSION 10


// Setting index
#define SETTING_1   1
#define SETTING_2   2
#define SETTING_3   3
#define SETTING_4   4
#define SETTING_5   5
#define SETTING_CURRENT 0xff


// MikroKopter defines
// taken from
// FC Software eeprom.h
//

//-----------------------GlobalConfig3 aus FC/eeprom.h-------------------------------
#define CFG3_NO_SDCARD_NO_START                 0x01
#define CFG3_DPH_MAX_RADIUS                     0x02    // bis EEprom Version 108
#define CFG3_RISE_FIRST_WAYPOINT 				0x02	// ab Eeprom Version 109
#define CFG3_VARIO_FAILSAFE                     0x04
#define CFG3_MOTOR_SWITCH_MODE                  0x08    //FC0.88L 7.5.12
#define CFG3_NO_GPSFIX_NO_START                 0x10    //FC0.88L 7.5.12
#define CFG3_USE_NC_FOR_OUT1                    0x20
#define CFG3_SPEAK_ALL                          0x40
#define CFG3_SERVO_NICK_COMP_OFF                0x80


//------------------------GlobalConfig------------------------------------------------
#define CFG_HOEHENREGELUNG                      0x01
#define CFG_HOEHEN_SCHALTER                     0x02
#define CFG_HEADING_HOLD                        0x04
#define CFG_KOMPASS_AKTIV                       0x08
#define CFG_KOMPASS_FIX                         0x10
#define CFG_GPS_AKTIV                           0x20
#define CFG_ACHSENKOPPLUNG_AKTIV                0x40	// bis EEpromVersion 108
#define CFG_LAND_OUTSIDE_FLYZONE 				0x40	// ab EEpromversion 110
#define CFG_DREHRATEN_BEGRENZER                 0x80

//------------------------Bitconfig MAsk---------------------------------------------
#define CFG_LOOP_OBEN                           0x01
#define CFG_LOOP_UNTEN                          0x02
#define CFG_LOOP_LINKS                          0x04
#define CFG_LOOP_RECHTS                         0x08

//#define CFG_MOTOR_BLINK1                        0x10 // bis EEpromversion 113

#define CFG_LED1_MOTOR_RUN       				0x10  // LED Only active if MotorRun ab EEpromversion 114
#define CFG_MOTOR_OFF_LED1                      0x20	// Idle State if !MotorRun
#define CFG_MOTOR_OFF_LED2                      0x40	// Idle State if !MotorRun

//#define CFG_MOTOR_BLINK2                        0x80 // bis EEpromversion 113

#define CFG_LED2_MOTOR_RUN       				0x80  // LED Only active if MotorRun ab EEpromversion 114



// ServoCompInvert, FC0.89 entfällt und geändert s.u.

//#define SVNick                                  0x01
//#define SVRoll                                  0x02
//#define SVRelMov                                0x04


//ab FC 209a (26.1.2015 cebra)

// bitcoding for EE_Parameter.ServoCompInvert
#define SERVO_NICK_INV 0x01
#define SERVO_ROLL_INV 0x02
#define SERVO_RELATIVE 0x04                     //  direct poti control or relative moving of the servo value
#define CH_DIRECTION_1 0x08
#define CH_DIRECTION_2 0x10

//CH Orientation     ServoBits          0x10    0x08
// --> no change                        0       0
// --> front to starting point          0       1
// --> rear to to starting point        1       0
//-> start orientation                  1       1


//-----------------------------ExtraConfig----------------------------------------------------
#define CFG2_HEIGHT_LIMIT                       0x01
#define CFG2_VARIO_BEEP                         0x02
#define CFG_SENSITIVE_RC                        0x04  // 26.1.2015 entfällt ab FC 209a, cebra
#define CFG_FLYZONE_REQUIRED   		    		0x04  // ab EEprom Version 110
#define CFG_3_3V_REFERENCE                      0x08
#define CFG_NO_RCOFF_BEEPING                    0x10
#define CFG_GPS_AID                             0x20
#define CFG_LEARNABLE_CAREFREE                  0x40
#define CFG_IGNORE_MAG_ERR_AT_STARTUP           0x80


// bit mask for ParamSet.Config0
#define CFG0_AIRPRESS_SENSOR                    0x01
#define CFG0_HEIGHT_SWITCH                      0x02
#define CFG0_HEADING_HOLD                       0x04
#define CFG0_COMPASS_ACTIVE                     0x08
#define CFG0_COMPASS_FIX                        0x10
#define CFG0_GPS_ACTIVE                         0x20
#define CFG0_AXIS_COUPLING_ACTIVE               0x40
#define CFG0_ROTARY_RATE_LIMITER                0x80

// defines for the receiver selection
#define RECEIVER_PPM                            0
#define RECEIVER_SPEKTRUM                       1
#define RECEIVER_SPEKTRUM_HI_RES                2
#define RECEIVER_SPEKTRUM_LOW_RES               3
#define RECEIVER_JETI                           4
#define RECEIVER_ACT_DSL                        5
#define RECEIVER_HOTT                           6
#define RECEIVER_SBUS                           7
#define RECEIVER_MLINK                          8       //FC2.11 16.7.2015 CB
#define RECEIVER_USER                           9       //FC2.11 16.7.2015 CB
#define RECEIVER_UNKNOWN                        0xFF

// MikroKopter Flags
// taken from
// http://svn.mikrokopter.de/mikrowebsvn/filedetails.php?repname=FlightCtrl&path=%2Ftags%2FV0.73d%2Ffc.h
//alt 0.86
//#define FCFLAG_MOTOR_RUN    0x01
//#define FCFLAG_FLY          0x02
//#define FCFLAG_CALIBRATE    0x04
//#define FCFLAG_START        0x08
//#define FCFLAG_NOTLANDUNG   0x10
//#define FCFLAG_LOWBAT       0x20
//#define FCFLAG_SPI_RX_ERR   0x40
//#define FCFLAG_I2CERR       0x80
// FC_StatusFlags 0.88
#define FC_STATUS_MOTOR_RUN                     0x01
#define FC_STATUS_FLY                           0x02
#define FC_STATUS_CALIBRATE                     0x04
#define FC_STATUS_START                         0x08
#define FC_STATUS_EMERGENCY_LANDING             0x10
#define FC_STATUS_LOWBAT                        0x20
#define FC_STATUS_VARIO_TRIM_UP                 0x40
#define FC_STATUS_VARIO_TRIM_DOWN               0x80

// FC_StatusFlags2
#define FC_STATUS2_CAREFREE                     0x01
#define FC_STATUS2_ALTITUDE_CONTROL             0x02
#define FC_STATUS2_RC_FAILSAVE_ACTIVE           0x04
#define FC_STATUS2_OUT1_ACTIVE                  0x08
#define FC_STATUS2_OUT2_ACTIVE                  0x10



// NaviCtrl Flags
// taken from
// http://mikrocontroller.cco-ev.de/mikrowebsvn/filedetails.php?repname=NaviCtrl&path=%2Ftags%2FV0.15c%2Fuart1.h
#define NC_FLAG_FREE                            0x01
#define NC_FLAG_PH                              0x02
#define NC_FLAG_CH                              0x04
#define NC_FLAG_RANGE_LIMIT                     0x08
#define NC_FLAG_NOSERIALLINK                    0x10
#define NC_FLAG_TARGET_REACHED                  0x20
#define NC_FLAG_MANUAL_CONTROL                  0x40
#define NC_FLAG_GPS_OK                          0x80


typedef struct
{
    unsigned char SWMajor;
    unsigned char SWMinor;
    unsigned char ProtoMajor;
    unsigned char ProtoMinor;
    unsigned char SWPatch;
    unsigned char HardwareError[5];
} __attribute__((packed)) Version_t;



// FC Debug Struct
// portions taken and adapted from
// http://svn.mikrokopter.de/mikrowebsvn/filedetails.php?repname=FlightCtrl&path=%2Ftags%2FV0.72p%2Fuart.h
typedef struct // 0.86
{
        uint8_t Digital[2];
        // NC: unsigned; FC: signed !!!!
        int16_t Analog[32];     // Debugvalues
} __attribute__((packed)) DebugData_t;



//******************************************************************
// uart1.h NC 0.87, zur Zeit hier nicht verwendet 28.01.2012 CB
#define AMPEL_FC                0x01
#define AMPEL_BL                0x02
#define AMPEL_NC                0x04
#define AMPEL_COMPASS           0x08



typedef struct  //0.87
{
        u8 StatusGreen;
        u8 StatusRed;
        u16 Analog[32];    // Debugwerte
} __attribute__((packed)) DebugOut_t;


//******************************************************************
//external Control from Navictrl uart1.h


#define EC_VALID 			0x01 // only valid if this is 1
#define EC_GAS_ADD 			0x02 // if 1 -> use the GAS Value not as MAX
#define EC_USE_SWITCH 		0x20 // if 1 -> use the Switches for further control
#define EC_IGNORE_RC_STICK 	0x40 // direct control (do nor add to RC-Stick)
#define EC_IGNORE_RC_LOST	0x80 // if 1 -> for Flying without RC-Control

// defines for ExternalControl.Switches -> control GPS Modes etc. if(Config & EC_USE_SWITCH)
#define EC2_PH 				0x01 // GPS-Mode: PH
#define EC2_CH 				0x02 // GPS-Mode: CH
#define EC2_CAREFREE 		0x10 //
#define EC2_ALTITUDE 		0x20 //
#define EC2_AUTOSTART 		0x40 //
#define EC2_AUTOLAND 		0x80 //

typedef struct
{
 signed char Nick;
 signed char Roll;
 signed char Gier;
 signed char Gas;
 unsigned char Frame; // will return a confirm frame with this value
 unsigned char Config;
 unsigned char Switches;
 unsigned char Free1; // these two don't need capacity in the ASCII data string
 unsigned char Free2;
} __attribute__((packed)) ExternControl_t;




#define SERIAL_POTI_START 17
#define WP_EVENT_PPM_IN   29
#define PPM_IN_OFF        30
#define PPM_IN_MAX        31
#define PPM_IN_MID        32
typedef struct
{
 signed char   Ch[12];
} __attribute__((packed)) SerialChannel_t;

//******************************************************************


// NaviCtrl OSD Structs
// portions taken and adapted from
// http://svn.mikrokopter.de/mikrowebsvn/filedetails.php?repname=NaviCtrl&path=%2Ftags%2FV0.15c%2Fuart1.h
//

typedef struct  //NC uart1.h
{
        s16 AngleNick;  // in 0.1 deg
        s16 AngleRoll;   // in 0.1 deg
        s16 Heading;    // in 0.1 deg
        u8 StickNick;
        u8 StickRoll;
        u8 StickYaw;
        u8 StickGas;
        u8 reserve[4];
} __attribute__((packed)) Data3D_t;



typedef struct
{
    s32 Longitude;      // in 1E-7 deg
    s32 Latitude;       // in 1E-7 deg
    s32 Altitude;       // in mm
    u8 Status;          // validity of data
} __attribute__((packed)) GPS_Pos_t;



typedef struct
{
    u16 Distance;       // distance to target in cm
    s16 Bearing;        // course to target in deg
} __attribute__((packed)) GPS_PosDev_t;




//----------------------------
// aus NC waypoint.h
//
// AKTUALISIERT: 18.06.2014 OG
//
// Anmerkung 18.06.2014 OG
//   "Name" hinzugefuegt - das gibt es
//   seit dem 23.03.2012!
//----------------------------
typedef struct
{
        GPS_Pos_t Position;             // the gps position of the waypoint, see ubx.h for details
        s16 Heading;                    // orientation, 0 no action, 1...360 fix heading, neg. = Index to POI in WP List
        u8  ToleranceRadius;    		// in meters, if the MK is within that range around the target, then the next target is triggered
        u8  HoldTime;                   // in seconds, if the was once in the tolerance area around a WP, this time defines the delay before the next WP is triggered
        u8  Event_Flag;                 // future implementation
        u8  Index;              		// to indentify different waypoints, workaround for bad communications PC <-> NC
        u8  Type;                       // typeof Waypoint
        u8  WP_EventChannelValue;  		// Will be transferred to the FC and can be used as Poti value there
        u8  AltitudeRate;           	// rate to change the setpoint in steps of 0.1m/s
        u8  Speed;                      // rate to change the Position(0 = max) in steps of 0.1m/s
        u8  CamAngle;                   // Camera servo angle in degree (255 -> POI-Automatic)
        u8  Name[4];                	// Name of that point (ASCII)
    u8  reserve[2];                 	// reserve
} __attribute__((packed)) Point_t;




//----------------------------
// aus NC waypoint.h
// VERALTET! 18.06.2014 OG
//----------------------------
/*
typedef struct
{
        GPS_Pos_t Position;             // the gps position of the waypoint, see ubx.h for details
        s16 Heading;                    // orientation, 0 no action, 1...360 fix heading, neg. = Index to POI in WP List
        u8  ToleranceRadius;            // in meters, if the MK is within that range around the target, then the next target is triggered
        u8  HoldTime;                   // in seconds, if the was once in the tolerance area around a WP, this time defines the delay before the next WP is triggered
        u8  Event_Flag;                 // future implementation
        u8  Index;                      // to indentify different waypoints, workaround for bad communications PC <-> NC
        u8  Type;                       // typeof Waypoint
        u8  WP_EventChannelValue;       //
        u8  AltitudeRate;               // rate to change the setpoint
        u8  Speed;                      // rate to change the Position
        u8  CamAngle;                   // Camera servo angle
        u8  reserve[6];                 // reserve
} __attribute__((packed)) Point_t;
*/


// NaviCtrl struct
// taken from
// http://mikrocontroller.cco-ev.de/mikrowebsvn/filedetails.php?repname=NaviCtrl&path=%2Ftags%2FV0.15c%2Fuart1.h
//
#define NAVIDATA_VERSION 5

typedef struct
{
        u8 Version;                                     // version of the data structure
        GPS_Pos_t CurrentPosition;                      // see ubx.h for details
        GPS_Pos_t TargetPosition;
        GPS_PosDev_t TargetPositionDeviation;
        GPS_Pos_t HomePosition;
        GPS_PosDev_t HomePositionDeviation;
        u8  WaypointIndex;                              // index of current waypoints running from 0 to WaypointNumber-1
        u8  WaypointNumber;                             // number of stored waypoints
        u8  SatsInUse;                                  // number of satellites used for position solution
        s16 Altimeter;                                  // hight according to air pressure
        s16 Variometer;                                 // climb(+) and sink(-) rate
        u16 FlyingTime;                                 // in seconds
        u8  UBat;                                       // Battery Voltage in 0.1 Volts
        u16 GroundSpeed;                                // speed over ground in cm/s (2D)
        s16 Heading;                                    // current flight direction in Â° as angle to north
        s16 CompassHeading;                             // current compass value in
        s8  AngleNick;                                  // current Nick angle in 1
        s8  AngleRoll;                                  // current Roll angle in 1
        u8  RC_Quality;                                 // RC_Quality
        u8  FCStatusFlags;                              // Flags from FC
        u8  NCFlags;                                    // Flags from NC
        u8  Errorcode;                                  // 0 --> okay
        u8  OperatingRadius;                            // current operation radius around the Home Position in m
        s16 TopSpeed;                                   // velocity in vertical direction in cm/s
        u8  TargetHoldTime;                             // time in s to stay at the given target, counts down to 0 if target has been reached
        u8  FCStatusFlags2;                             // StatusFlags2 (since version 5 added)
        s16 SetpointAltitude;                           // setpoint for altitude
        u8  Gas;                                        // for future use
        u16 Current;                                    // actual current in 0.1A steps
        u16 UsedCapacity;                               // used capacity in mAh
} __attribute__((packed)) NaviData_t;

typedef struct // 10 (15 Bytes belegen 27 ASCII-Zeichen)
{
	u8 Index;						// version of the data structure
	s32 ActualLongitude;			//
	s32 ActualLatitude;				//
	s16 Altimeter; 					// hight according to air pressure
	u8 GroundSpeed;					// speed over ground in 10cm/s (2D) (255 = 91km/h)
	u8 OSDStatusFlags;
	u8 CamCtrlChar;					// Status from a connected CamCtrl unit: 'R' = REC  'c' = Ready  '!' = Error  ...etc
	u8 reserve1;
} __attribute__((packed)) NaviData_Tiny_t;

typedef struct
{
        uint8_t Version;                        // the version of the BL (0 = old)
        uint8_t SetPoint;                       // written by attitude controller
        uint8_t SetPointLowerBits;      // for higher Resolution of new BLs
        uint8_t State;                          // 7 bit for I2C error counter, highest bit indicates if motor is present
        uint8_t ReadMode;                       // select data to read
        // the following bytes must be exactly in that order!
        uint8_t Current;                        // in 0.1 A steps, read back from BL
        uint8_t MaxPWM;                         // read back from BL -> is less than 255 if BL is in current limit, not running (250) or starting (40)
        int8_t  Temperature;                    // old BL-Ctrl will return a 255 here, the new version the temp. in ?C
} __attribute__((packed)) MotorData_t;



typedef struct
{
        uint8_t Revision;                       // must be BL_REVISION
        uint8_t SetMask;                        // settings mask
        uint8_t PwmScaling;                     // maximum value of control pwm, acts like a thrust limit
        uint8_t CurrentLimit;           // current limit in A
        uint8_t TempLimit;                      // in ?C
        uint8_t CurrentScaling;         // scaling factor for current measurement
        uint8_t BitConfig;                      // see defines above
        uint8_t crc;                            // checksum
}  __attribute__((packed)) BLConfig_t;



//##############################################################################
//# Datenstruktur von ehemals mk_param_struct_t jetzt via mk/paramset.c !
//##############################################################################


//-------------------------------------
// 20.03.2013 OG: BLData_t
// aus: http://www.mikrokopter.de/ucwiki/en/SerialCommands/BLDataStruct
//-------------------------------------
typedef struct
{
            unsigned char Index;            // address of BL-Ctrl 
            unsigned char Current;          // in 0.1 A steps, read back from BL
            unsigned char Temperature;      // old BL-Ctrl will return a 255 here, the new version (>= V2.0) the temp. in °C
            unsigned char MaxPWM;           // EVENTUELL: read back from BL -> is less than 255 if BL is in current limit, not running (250) or starting (40)
            unsigned char Status;           // EVENTUELL: 7 bit for I2C error counter, highest bit indicates if motor is present
}  BLData_t;



//-------------------------------------
// 11.05.2013 OG: DateTime_t
// aus: NC v0.30g timer1.h
//-------------------------------------
typedef struct{
    u16    Year;
    u8    Month; // @suppress("Type cannot be resolved")
    u8    Day;
    u8    Hour;
    u8    Min;
    u8    Sec;
    u16 mSec;
    u8  Valid;
}  DateTime_t;



#endif
