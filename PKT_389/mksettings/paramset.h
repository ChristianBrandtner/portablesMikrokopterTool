/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
 *   Copyright (C) 2008 Thomas Kaiser, thomas@ft-fanpage.de                  *
 *   Copyright (C) 2009 Peter "woggle" Mack, mac@denich.net                  *
 *   Copyright (C) 2012 Christian "Cebra" Brandtner, brandtner@brandtner.net *
 *   Copyright (C) 2012 Harald Bongartz                                      *
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
//# HISTORY  paramset.h
//#
//# 16.07.2018 Cebra
//# - chg : param_BitConfig_MotorBlink1  zu param_BitConfig_Led1_MotorRun
//# - chg : param_BitConfig_MotorBlink2  zu param_BitConfig_Led2_MotorRun
//#
//# 08.07.2018 Cebra
//#	- add: paramset_113 (FC2.20)
//# - add: param_GimbalOut1Channel, param_GimbalOut2Channel
//#
//# 31.10.2017 Cebra
//# - add: param_ExtraConfig_FlyZone_Required ab FC Version 2.18
//# - chg: ParamID neu numeriert, wegen neuer Parameter ID's
//#
//# 13.10.2017 Cebra
//# - add: unsigned char FailsafeAltitude, neuer Parameter ab Rev. 111
//# - add: #define param_GlobalConfig3_RiseFirstWP	        , ab Rev.109
//# - chg: unsigned char reserved[31] , geändert von 32 auf 31
//# - add: #define param_GlobalConfig_Landoutside_Flyzone
//#
//# 18.01.2016 Cebra (PKT385a)
//# - add: #define param_reserved (FC2.14a)
//#
//# 18.01.2016 Cebra (PKT385a)
//# - add: #define param_CamCtrlModeChannel (FC2.13f)
//#        #define param_CamCtrlZoomChannel (FC2.13f)
//#
//# 16.07.2015 Cebra (PKT385a)
//# - add: #define param_SingleWpControlChannel (FC2.11a)
//#        #define param_MenuKeyChannel (FC2.11a)
//#
//# 09.04.2015 Cebra
//# - add:   #define param_ServoNickFailsave, #define param_ServoRollFailsave, #define param_Servo3Failsave
//#          #define param_Servo4Failsave, #define param_Servo5Failsave
//#
//# 26.01.2015 Cebra
//# - add:   #define param_ComingHomeOrientation  241  ab FC 209a im Wert ServoCompInvert, Bit4 + Bit5
//#
//# 26.09.2014 Cebra
//# - add: paramID fuer Rev.103  (FC 2.07f)
//#             -> param_Hoehe_TiltCompensation
//#
//# 08.04.2014 OG
//# - add: paramID's fuer Rev.102  (FC 2.05g)
//#		-> param_NaviMaxFlyingRange, param_NaviDescendRange
//#
//# 06.04.2014 OG
//# - add: paramID's fuer Rev.101
//#        -> param_Servo3OnValue, param_Servo3OffValue	
//#        -> param_Servo4OnValue, param_Servo4OffValue
//#
//# 28.03.2014 OG
//# - add: paramID's fuer Rev.100
//#        -> param_AutoPhotoDistance, param_AutoPhotoAtitudes
//#        -> param_SingleWpSpeed
//#
//# 26.03.2014 OG
//# - add: param_CompassOffset_DisableDeclCalc (Sub-Item)
//#
//# 27.02.2014 OG
//# - del: paramID's fuer FC-Revisionen < 95 entfernt
//#
//# 27.02.2014 OG
//# - chg: paramGet(), paramSet() - Unterstuetzen auch Bit- und Bytefelder
//#
//# 26.02.2014 OG
//# - add: defines fuer die Unterstuetzung von paramSubID's ergaenzt
//#        paramSubItems bieten direkten Zugriff auf Bit- und Bytefelder
//#        von paramID's - die paramSubItems wurde in die defines der
//#        paramID's integriert indem sie am Ende gelistet werden (>=160)
//# - chg: paramsetTest() umbenannt zu paramsetDEBUG()
//#
//# 25.02.2014 OG
//# - add: defines zu param_Kanalbelegung_xyz (240 bis 251)
//#
//# 14.02.2014 OG
//# - add: diverse Zugriffsfunktionen fuer paramID's und paramSet's 
//#
//# 05.02.2014 OG - NEU
//############################################################################

#ifndef _PARAMSET_H
#define _PARAMSET_H


//---------------------------
// Typ einer paramSubID
//---------------------------
#define SUBTYPE_NO      0   // keine paramSubID
#define SUBTYPE_BIT     1   // Bit-Feld (z.B. param_GlobalConfig)
#define SUBTYPE_BITN    2   // Bit-Feld (z.B. param_GlobalConfig) negiert! 
#define SUBTYPE_BYTE    3   // Byte-Feld (z.B. Kanalbelegung)



//########################################################################################################################################################

//----------------------------------------------------------------------------------------
// Die Reihenfolge ist egal - aber(!) darauf achten das keine ID doppelt vergeben wird!
// Wenn neue Feleder hinzukommen an das Ende (vor EOF - keine ID 255!) hinzufuegen!
// Bitte die Datentypen im Kommentar notieren!
//
// -> paramID
//----------------------------------------------------------------------------------------
#define param_Revision                          1       // unsigned char 
#define param_Kanalbelegung                     2       // unsigned char [12]   GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
#define param_GlobalConfig                      3       // unsigned char        0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
#define param_Hoehe_MinGas                      4       // unsigned char        Wert : 0-100
#define param_Luftdruck_D                       5       // unsigned char        Wert : 0-250
#define param_HoeheChannel                      6       // unsigned char        Wert : 0-32
#define param_Hoehe_P                           7       // unsigned char        Wert : 0-32
#define param_Hoehe_Verstaerkung                8       // unsigned char        Wert : 0-50
#define param_Hoehe_ACC_Wirkung                 9       // unsigned char        Wert : 0-250
#define param_Hoehe_HoverBand                   10      // unsigned char        Wert : 0-250
#define param_Hoehe_GPS_Z                       11      // unsigned char        Wert : 0-250
#define param_Hoehe_StickNeutralPoint           12      // unsigned char        Wert : 0-250
#define param_Stick_P                           13      // unsigned char        WERT : 0..20
#define param_Stick_D                           14      // unsigned char        WERT : 0..20
#define param_StickGier_P                       15      // unsigned char        Wert :
#define param_Gas_Min                           16      // unsigned char        Wert : 0-32
#define param_Gas_Max                           17      // unsigned char        Wert : 33-250
#define param_GyroAccFaktor                     18      // unsigned char        Wert : 1-64
#define param_KompassWirkung                    19      // unsigned char        Wert : 0-32
#define param_Gyro_P                            20      // unsigned char        Wert : 10-250
#define param_Gyro_I                            21      // unsigned char        Wert : 0-250
#define param_Gyro_D                            22      // unsigned char        Wert : 0-250
#define param_Gyro_Gier_P                       23      // unsigned char        Wert : 10-250
#define param_Gyro_Gier_I                       24      // unsigned char        Wert : 0-250
#define param_Gyro_Stability                    25      // unsigned char        Wert : 0-16
#define param_UnterspannungsWarnung             26      // unsigned char        Wert : 0-250
#define param_NotGas                            27      // unsigned char        Wert : 0-250     //Gaswert bei Empfangsverlust
#define param_NotGasZeit                        28      // unsigned char        Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
#define param_Receiver                          29      // unsigned char        0= Summensignal, 1= Spektrum, 2 =Jeti, 3=ACT DSL, 4=ACT S3D
#define param_I_Faktor                          30      // unsigned char        Wert : 0-250
#define param_UserParam1                        31      // unsigned char        Wert : 0-250
#define param_UserParam2                        32      // unsigned char        Wert : 0-250
#define param_UserParam3                        33      // unsigned char        Wert : 0-250
#define param_UserParam4                        34      // unsigned char        Wert : 0-250
#define param_ServoNickControl                  35      // unsigned char        Wert : 0-250     // Stellung des Servos
#define param_ServoNickComp                     36      // unsigned char        Wert : 0-250     // Einfluss Gyro/Servo
#define param_ServoNickMin                      37      // unsigned char        Wert : 0-250     // Anschlag
#define param_ServoNickMax                      38      // unsigned char        Wert : 0-250     // Anschlag
#define param_ServoRollControl                  39      // unsigned char        Wert : 0-250     // Stellung des Servos
#define param_ServoRollComp                     40      // unsigned char        Wert : 0-250
#define param_ServoRollMin                      41      // unsigned char        Wert : 0-250
#define param_ServoRollMax                      42      // unsigned char        Wert : 0-250
#define param_ServoNickRefresh                  43      // unsigned char        Speed of the Servo
#define param_ServoManualControlSpeed           44      // unsigned char 
#define param_CamOrientation                    45      // unsigned char 
#define param_Servo3                            46      // unsigned char        Value or mapping of the Servo Output
#define param_Servo4                            47      // unsigned char        Value or mapping of the Servo Output
#define param_Servo5                            48      // unsigned char        Value or mapping of the Servo Output
#define param_LoopGasLimit                      49      // unsigned char        Wert: 0-250  max. Gas während Looping
#define param_LoopThreshold                     50      // unsigned char        Wert: 0-250  Schwelle für Stickausschlag
#define param_LoopHysterese                     51      // unsigned char        Wert: 0-250  Hysterese für Stickausschlag
#define param_AchsKopplung1                     52      // unsigned char        Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
#define param_AchsKopplung2                     53      // unsigned char        Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
#define param_CouplingYawCorrection             54      // unsigned char        Wert: 0-250  Faktor, mit dem Nick und Roll verkoppelt werden
#define param_WinkelUmschlagNick                55      // unsigned char        Wert: 0-250  180°-Punkt
#define param_WinkelUmschlagRoll                56      // unsigned char        Wert: 0-250  180°-Punkt
#define param_GyroAccAbgleich                   57      // unsigned char        1/k  (Koppel_ACC_Wirkung)
#define param_Driftkomp                         58      // unsigned char 
#define param_DynamicStability                  59      // unsigned char 
#define param_UserParam5                        60      // unsigned char        Wert : 0-250
#define param_UserParam6                        61      // unsigned char        Wert : 0-250
#define param_UserParam7                        62      // unsigned char        Wert : 0-250
#define param_UserParam8                        63      // unsigned char        Wert : 0-250
#define param_J16Bitmask                        64      // unsigned char        for the J16 Output
#define param_J16Timing                         65      // unsigned char        for the J16 Output
#define param_J17Bitmask                        66      // unsigned char        for the J17 Output
#define param_J17Timing                         67      // unsigned char        for the J17 Output
#define param_WARN_J16_Bitmask                  68      // unsigned char        for the J16 Output
#define param_WARN_J17_Bitmask                  69      // unsigned char        for the J17 Output
#define param_NaviOut1Parameter                 70      // unsigned char        for the J16 Output
#define param_NaviGpsModeChannel                71      // unsigned char        Parameters for the Naviboard
#define param_NaviGpsGain                       72      // unsigned char 
#define param_NaviGpsP                          73      // unsigned char 
#define param_NaviGpsI                          74      // unsigned char 
#define param_NaviGpsD                          75      // unsigned char 
#define param_NaviGpsPLimit                     76      // unsigned char 
#define param_NaviGpsILimit                     77      // unsigned char 
#define param_NaviGpsDLimit                     78      // unsigned char 
#define param_NaviGpsA                          79      // unsigned char 
#define param_NaviGpsMinSat                     80      // unsigned char 
#define param_NaviStickThreshold                81      // unsigned char 
#define param_NaviWindCorrection                82      // unsigned char 
#define param_NaviAccCompensation               83      // unsigned char        New since 0.86 -> was: SpeedCompensation
#define param_NaviOperatingRadius               84      // unsigned char 		bis Rev. 101
#define param_NaviAngleLimitation               85      // unsigned char 
#define param_NaviPH_LoginTime                  86      // unsigned char 
#define param_ExternalControl                   87      // unsigned char        for serial Control
#define param_OrientationAngle                  88      // unsigned char        Where is the front-direction?
#define param_CareFreeChannel                   89      // unsigned char        switch for CareFree
#define param_MotorSafetySwitch                 90      // unsigned char 
#define param_MotorSmooth                       91      // unsigned char 
#define param_ComingHomeAltitude                92      // unsigned char 
#define param_FailSafeTime                      93      // unsigned char 
#define param_MaxAltitude                       94      // unsigned char 
#define param_FailsafeChannel                   95      // unsigned char        if the value of this channel is > 100, the MK reports "RC-Lost"
#define param_ServoFilterNick                   96      // unsigned char 
#define param_ServoFilterRoll                   97      // unsigned char 
#define param_StartLandChannel                  98      // unsigned char 
#define param_LandingSpeed                      99      // unsigned char 
#define param_CompassOffset                     100     // unsigned char     
#define param_AutoLandingVoltage                101     // unsigned char        in 0,1V  0 -> disabled
#define param_ComingHomeVoltage                 102     // unsigned char        in 0,1V  0 -> disabled
#define param_BitConfig                         103     // unsigned char        (war Loop-Cfg) Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
#define param_ServoCompInvert                   104     // unsigned char        0x01 = Nick, 0x02 = Roll, 0x04 = relative moving // WICHTIG!!! am Ende lassen
#define param_ExtraConfig                       105     // unsigned char        bitcodiert
#define param_GlobalConfig3                     106     // unsigned char        bitcodiert
#define param_Name                              107     // char [12]            Name vom Parameterset
#define param_crc                               108     // unsigned char        
#define param_AutoPhotoDistance                 109     // unsigned char        ab Rev. 100 (ersetzt NaviOut1Parameter) // Auto Photo
#define param_AutoPhotoAtitudes                 110     // unsigned char        ab Rev. 100 
#define param_SingleWpSpeed                     111     // unsigned char        ab Rev. 100 
#define param_Servo3OnValue						112     // unsigned char        ab Rev. 101  (FC 2.05f)
#define param_Servo3OffValue					113     // unsigned char        ab Rev. 101  (FC 2.05f)
#define param_Servo4OnValue						114     // unsigned char        ab Rev. 101  (FC 2.05f)
#define param_Servo4OffValue					115     // unsigned char        ab Rev. 101  (FC 2.05f)
#define param_NaviMaxFlyingRange				116     // unsigned char        ab Rev. 102  (FC 2.05g)	 (ersetzt NaviOperatingRadius)		// in 10m
#define param_NaviDescendRange					117     // unsigned char        ab Rev. 102  (FC 2.05g)
#define param_Hoehe_TiltCompensation            118     // unsigned char        ab Rev. 103  (FC 2.07f)
#define param_LandingPulse                      119     // unsigned char        ab Rev. 104  (FC 2.09d)
#define param_ServoFS_Pos                       120     // unsigned char [5]    ServoNickFailsave[0],ServoRollFailsave[1],Servo3Failsave[2],Servo4Failsave[3],Servo5Failsave[4]  ab Rev. 105  (FC 2.09i)
#define param_SingleWpControlChannel            121     // unsigned char        ab Rev. 106  (FC 2.11a)
#define param_MenuKeyChannel                    122     // unsigned char        ab Rev. 106  (FC 2.11a)
#define param_CamCtrlModeChannel                123     // unsigned char        ab Rev. 107 (FC2.13a)
#define param_CamCtrlZoomChannel                124     // unsigned char        ab Rev. 107 (FC2.13a)
#define param_FailsafeAltitude					125		// unsigned char        ab Rev. 111 (FC2.19)
#define param_GimbalYawChannel					126		// unsigned char	    ab Rev. 112 (FC2.19)
#define param_GimbalOut1Channel					127		// unsigned char	    ab Rev. 113 (FC2.20)
#define param_GimbalOut2Channel					128		// unsigned char	    ab Rev. 113 (FC2.20)
#define param_ExternalGpsChannel				129		// unsigned char	    ab Rev. 113 (FC2.20)
#define param_reserved					 	    130     // unsigned char[27]    ab Rev. 114 (FC2.21c)

//------- ^^^ HIER NEUE paramID's einfuegen! ^^^


// ab hier sind Sub-Items zu einer paramID definiert
// - einige paramID's werden nochmals untergliedert in Bit- oder Bytefelder
// - die Zuordnung der Sub-Items zu paramID's erfolgt in paramset.c
#define param_SUBITEM                           160     // <- ab diesem Wert handelt es sich um ein paramSubID und nicht um eine paramID

#define param_Kanalbelegung_Nick                170      // SUBTYPE_BYTE - Kanalbelegung [0] (unsigned char) 
#define param_Kanalbelegung_Roll                171      // SUBTYPE_BYTE - Kanalbelegung [1] (unsigned char) 
#define param_Kanalbelegung_Gas                 172      // SUBTYPE_BYTE - Kanalbelegung [2] (unsigned char) 
#define param_Kanalbelegung_Gear                173      // SUBTYPE_BYTE - Kanalbelegung [3] (unsigned char) 
#define param_Kanalbelegung_Poti1               174      // SUBTYPE_BYTE - Kanalbelegung [4] (unsigned char) 
#define param_Kanalbelegung_Poti2               175      // SUBTYPE_BYTE - Kanalbelegung [5] (unsigned char) 
#define param_Kanalbelegung_Poti3               176      // SUBTYPE_BYTE - Kanalbelegung [6] (unsigned char) 
#define param_Kanalbelegung_Poti4               177      // SUBTYPE_BYTE - Kanalbelegung [7] (unsigned char) 
#define param_Kanalbelegung_Poti5               178      // SUBTYPE_BYTE - Kanalbelegung [8] (unsigned char) 
#define param_Kanalbelegung_Poti6               179      // SUBTYPE_BYTE - Kanalbelegung [9] (unsigned char) 
#define param_Kanalbelegung_Poti7               180      // SUBTYPE_BYTE - Kanalbelegung [10] (unsigned char) 
#define param_Kanalbelegung_Poti8               181      // SUBTYPE_BYTE - Kanalbelegung [11] (unsigned char) 

#define param_ServoCompInvert_SERVO_NICK_INV    190      // SUBTYPE_BIT - mk-data-structs.h: SVNick                        0x01
#define param_ServoCompInvert_SERVO_ROLL_INV    191      // SUBTYPE_BIT - mk-data-structs.h: SVRoll                        0x02
#define param_ServoCompInvert_SERVO_RELATIVE    192      // SUBTYPE_BIT - mk-data-structs.h: SVRelMov                      0x04


#define param_ExtraConfig_HeightLimit           193      // SUBTYPE_BIT - mk-data-structs.h: CFG2_HEIGHT_LIMIT             0x01
#define param_ExtraConfig_HeightVario           194      // SUBTYPE_BIT - mk-data-structs.h: CFG2_HEIGHT_LIMIT             0x01  ist negiert zu param_ExtraConfig_HeightLimit
#define param_ExtraConfig_VarioBeep             195      // SUBTYPE_BIT - mk-data-structs.h: CFG2_VARIO_BEEP               0x02
#define param_ExtraConfig_SensitiveRc           196      // SUBTYPE_BIT - mk-data-structs.h: CFG_SENSITIVE_RC              0x04  // bis EEprom Version 109
#define param_ExtraConfig_FlyZone_Required      197      // SUBTYPE_BIT - mk-data-structs.h: CFG_FLYZONE_REQUIRED          0x04  // ab EEprom Version 110
#define param_ExtraConfig_33vReference          198      // SUBTYPE_BIT - mk-data-structs.h: CFG_3_3V_REFERENCE            0x08
#define param_ExtraConfig_NoRcOffBeeping        199      // SUBTYPE_BIT - mk-data-structs.h: CFG_NO_RCOFF_BEEPING          0x10
#define param_ExtraConfig_GpsAid                200      // SUBTYPE_BIT - mk-data-structs.h: CFG_GPS_AID                   0x20
#define param_ExtraConfig_LearnableCarefree     201      // SUBTYPE_BIT - mk-data-structs.h: CFG_LEARNABLE_CAREFREE        0x40
#define param_ExtraConfig_IgnoreMagErrAtStartup 202      // SUBTYPE_BIT - mk-data-structs.h: CFG_IGNORE_MAG_ERR_AT_STARTUP 0x80


#define param_BitConfig_LoopOben                203      // SUBTYPE_BIT - mk-data-structs.h: CFG_LOOP_OBEN                 0x01
#define param_BitConfig_LoopUnten               204      // SUBTYPE_BIT - mk-data-structs.h: CFG_LOOP_UNTEN                0x02
#define param_BitConfig_LoopLinks               205      // SUBTYPE_BIT - mk-data-structs.h: CFG_LOOP_LINKS                0x04
#define param_BitConfig_LoopRechts              206      // SUBTYPE_BIT - mk-data-structs.h: CFG_LOOP_RECHTS               0x08
#define param_BitConfig_Led1_MotorRun             207      // SUBTYPE_BIT - mk-data-structs.h: CFG_MOTOR_BLINK1              0x10
//#define param_BitConfig_MotorBlink1            207      // SUBTYPE_BIT - mk-data-structs.h: CFG_LED1_MOTOR_RUN            0x10
#define param_BitConfig_MotorOffLed1            208      // SUBTYPE_BIT - mk-data-structs.h: CFG_MOTOR_OFF_LED1            0x20
#define param_BitConfig_MotorOffLed2            209      // SUBTYPE_BIT - mk-data-structs.h: CFG_MOTOR_OFF_LED2            0x40
//#define param_BitConfig_MotorBlink2             210      // SUBTYPE_BIT - mk-data-structs.h: CFG_MOTOR_BLINK2              0x80
#define param_BitConfig_Led2_MotorRun           210      // SUBTYPE_BIT - mk-data-structs.h: CFG_LED1_MOTOR_RUN            0x80

#define param_GlobalConfig3_NoSdCardNoStart     211      // SUBTYPE_BIT - mk-data-structs.h: CFG3_NO_SDCARD_NO_START       0x01
#define param_GlobalConfig3_DphMaxRadius        212      // SUBTYPE_BIT - mk-data-structs.h: CFG3_DPH_MAX_RADIUS           0x02	nur bis Rev 101
#define param_GlobalConfig3_RiseFirstWP	        213		 // SUBTYPE_BIT - mk-data-structs.h: CFG3_RISE_FIRST_WAYPOINT      0x02	ab Rev 109
#define param_GlobalConfig3_VarioFailsafe       214      // SUBTYPE_BIT - mk-data-structs.h: CFG3_VARIO_FAILSAFE           0x04
#define param_GlobalConfig3_MotorSwitchMode     215      // SUBTYPE_BIT - mk-data-structs.h: CFG3_MOTOR_SWITCH_MODE        0x08
#define param_GlobalConfig3_NoGpsFixNoStart     216      // SUBTYPE_BIT - mk-data-structs.h: CFG3_NO_GPSFIX_NO_START       0x10
#define param_GlobalConfig3_UseNcForOut1        217      // SUBTYPE_BIT - mk-data-structs.h: CFG3_USE_NC_FOR_OUT1          0x20
#define param_GlobalConfig3_SpeakAll            218      // SUBTYPE_BIT - mk-data-structs.h: CFG3_SPEAK_ALL                0x40
#define param_GlobalConfig3_ServoNickCompOff    219      // SUBTYPE_BIT - mk-data-structs.h: CFG3_SERVO_NICK_COMP_OFF      0x80

#define param_GlobalConfig_HoehenRegelung       220      // SUBTYPE_BIT - mk-data-structs.h: CFG_HOEHENREGELUNG            0x01
#define param_GlobalConfig_HoehenSchalter       221      // SUBTYPE_BIT - mk-data-structs.h: CFG_HOEHEN_SCHALTER           0x02
#define param_GlobalConfig_HeadingHold          222      // SUBTYPE_BIT - mk-data-structs.h: CFG_HEADING_HOLD              0x04
#define param_GlobalConfig_KompassAktiv         223      // SUBTYPE_BIT - mk-data-structs.h: CFG_KOMPASS_AKTIV             0x08
#define param_GlobalConfig_KompassFix           224      // SUBTYPE_BIT - mk-data-structs.h: CFG_KOMPASS_FIX               0x10
#define param_GlobalConfig_GpsAktiv             225      // SUBTYPE_BIT - mk-data-structs.h: CFG_GPS_AKTIV                 0x20
#define param_GlobalConfig_AchsenkopplungAktiv  226      // SUBTYPE_BIT - mk-data-structs.h: CFG_ACHSENKOPPLUNG_AKTIV      0x40 bis EEprom 109
#define param_GlobalConfig_LandOutsideFlyzone   227      // SUBTYPE_BIT - mk-data-structs.h: CFG_LAND_OUTSIDE_FLYZONE      0x40 ab EEprom 110
#define param_GlobalConfig_DrehratenBegrenzer   228      // SUBTYPE_BIT - mk-data-structs.h: CFG_DREHRATEN_BEGRENZER       0x80
#define param_ServoNickFailsave                 229      // SUBTYPE_BYTE- mk-data-structs.h: ServoFS_Pos[0]                     ab FC 2.09j
#define param_ServoRollFailsave                 230      // SUBTYPE_BYTE- mk-data-structs.h: ServoFS_Pos[1]                     ab FC 2.09j
#define param_Servo3Failsave                    231      // SUBTYPE_BYTE- mk-data-structs.h: ServoFS_Pos[2]                     ab FC 2.09j
#define param_Servo4Failsave                    232      // SUBTYPE_BYTE- mk-data-structs.h: ServoFS_Pos[3]                     ab FC 2.09j
#define param_Servo5Failsave                    233      // SUBTYPE_BYTE- mk-data-structs.h: ServoFS_Pos[4]                     ab FC 2.09j




// hier: spezielle Subitems
#define param_CompassOffset_DisableDeclCalc     244      // wird gemappt auf: param_CompassOffset - der Wert ist dort in Bit 8 und 7 kodiert
#define param_ComingHomeOrientation             245      // unsigned char        0x08, 0x10, Bits im Feld ServoCompInvert





//---------------------------------------------------------------------------------------
// IMMER am Ende!!
#define param_DUMMY                             254      // DO NOT CHANGE! DUMMY        => spacer werte
#define param_EOF                               255      // DO NOT CHANGE! EOF          => markiert das Ende von paramID-Listen
#define param_NOTFOUND                          255      // DO NOT CHANGE! NOTFOUND     => kein paramID gefunden

//########################################################################################################################################################



//---------------------------
// struct: einzelnes Parameter-Item
//---------------------------
typedef struct
{   
    unsigned char paramID;
    unsigned char size;
} paramRevItem_t;




//---------------------------
// exportierte Funktionen
//---------------------------
unsigned char paramsetInit( unsigned char *pData );
unsigned char paramsetSize( void );

unsigned char paramExist( unsigned char paramID );
unsigned char paramSize( unsigned char paramID );
unsigned char paramGet( unsigned char paramID );
unsigned char paramSet( unsigned char paramID, unsigned char newvalue );
unsigned char *paramGet_p( unsigned char paramID );




// TEST / DEBUG
void paramsetDEBUG( void );

#endif  // _PARAMSET_H
