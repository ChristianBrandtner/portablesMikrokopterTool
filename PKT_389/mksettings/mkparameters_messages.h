/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
 *****************************************************************************/

//############################################################################
//# HISTORY  mkparameters_messages.h
//#
//#
//# 31.10.2017 Cebra
//# - add: Texte für FC-Versionen 2.16 bis 2.18 Hinzugefügt
//#
//# 18.01.2016 Cebra
//# - add: Texte für neue Parameter in FC 2.13f inkl. der ID's
//#        const char param_CamCtrlModeChannel_de
//#        const char param_CamCtrlZoomChannel_de
//#
//# 16.07.2015 Cebra
//# - add: Texte für neue Parameter in FC 2.11a inkl. der ID's
//#        const char param_singleWpControlChannel_de
//#        const char param_MenuKeyChannel_de
//#
//# 09.04.2015 Cebra
//# - add: Texte für neue Parameter in FC 2.09j
//#        const char param_ServoNickFails_de[],const char param_ServoRollFails_de[], const char param_Servo3Fails_de[],
//#        const char param_Servo4Fails_de[], const char param_Servo5Fails_de[]
//#
//# 26.09.2014 Cebra
//# - add: Text Höhe Tilt Compensation (FC207f)
//#
//# 04.06.2014 OG
//# - chg: Text von ID_MENU_FAVORITEN etwas gekuerzt
//#
//# 07.05.2014 OG
//# - chg: ID_MENU_FAVORITEN_en
//#
//# 18.04.2014 OG
//# - chg: Textanpassung: param_NaviGpsMinSat_de, param_NaviStickThreshold_de
//#
//# 17.04.2014 OG
//# - add: param_Servo3OnValue, param_Servo3OffValue, param_Servo4OnValue
//#        param_Servo4OffValue
//# - add: param_NaviMaxFlyingRange, param_NaviDescendRange
//# - chg: Textanpassung: param_WARN_J16_Bitmask_de, param_WARN_J17_Bitmask_de
//# - chg: Textanpassung: param_J16Bitmask, param_J16Timing
//# - chg: Textanpassung: param_J17Bitmask, param_J17Timing
//# - chg: Textanpassung: param_SingleWpSpeed_de
//# - chg: Textanpassung: param_ServoNickRefresh_de, param_ServoManualControlSpeed_de
//# - chg: Textanpassung: param_ServoRollControl_de
//#
//# 07.04.2014 OG
//# - chg: kleine Aenderungen an englischen Texten
//#
//# 06.04.2014 CB
//# - add: englische Menütexte ergänzt
//#
//# 30.03.2014 OG
//# - chg: Sprache Hollaendisch vollstaendig entfernt
//#
//# 28.03.2014 OG
//# - add: paramID-Texte fuer Rev. 100
//#        (param_AutoPhotoDistance, param_AutoPhotoAtitudes, param_SingleWpSpeed)
//#
//# 16.03.2014 OG
//# erste groesstenteils fertige Version
//#
//# 23.02.2014 OG - NEU
//############################################################################

#ifndef _MKPARAMETERS_MESSAGES_H
#define _MKPARAMETERS_MESSAGES_H


//#############################################################################################
//# Texte: Menues von mkparameters
//#############################################################################################

static const char ID_MENU_TEST_de[]             PROGMEM = "TEST/DEBUG";
#define           ID_MENU_TEST_en               ID_MENU_TEST_de

static const char ID_MENU_FAVORITEN_de[]        PROGMEM = "Favoriten";
static const char ID_MENU_FAVORITEN_en[]        PROGMEM = "Favorites";

static const char ID_MENU_KANAELE_de[]          PROGMEM = "Kanäle";
static const char ID_MENU_KANAELE_en[]          PROGMEM = "Channel";

static const char ID_MENU_KONFIGURATION_de[]    PROGMEM = "Konfiguration";
static const char ID_MENU_KONFIGURATION_en[]    PROGMEM = "Configuration";

static const char ID_MENU_STICK_de[]            PROGMEM = "Stick";
#define           ID_MENU_STICK_en              ID_MENU_STICK_de

static const char ID_MENU_LOOPING_de[]          PROGMEM = "Looping";
#define           ID_MENU_LOOPING_en            ID_MENU_LOOPING_de

static const char ID_MENU_HOEHE_de[]            PROGMEM = "Höhe";
static const char ID_MENU_HOEHE_en[]            PROGMEM = "Altitude";

static const char ID_MENU_KAMERA_de[]           PROGMEM = "Kamera";
static const char ID_MENU_KAMERA_en[]           PROGMEM = "Camera";

static const char ID_MENU_CAMCTRL_de[]          PROGMEM = "CamCtrl";
#define           ID_MENU_CAMCTRL_en            ID_MENU_CAMCTRL_de

static const char ID_MENU_NAVICTRL_de[]         PROGMEM = "Navi-Ctrl";
#define           ID_MENU_NAVICTRL_en           ID_MENU_NAVICTRL_de

static const char ID_MENU_AUSGAENGE_de[]        PROGMEM = "Ausgänge";
static const char ID_MENU_AUSGAENGE_en[]        PROGMEM = "Outputs";

static const char ID_MENU_VERSCHIEDENES_de[]    PROGMEM = "Verschiedenes";
static const char ID_MENU_VERSCHIEDENES_en[]    PROGMEM = "Miscellaneous";

static const char ID_MENU_GYRO_de[]             PROGMEM = "Gyro";
#define           ID_MENU_GYRO_en               ID_MENU_GYRO_de

static const char ID_MENU_BENUTZER_de[]         PROGMEM = "Benutzer";
static const char ID_MENU_BENUTZER_en[]         PROGMEM = "User";

static const char ID_MENU_ACHSKOPPLUNG_de[]     PROGMEM = "Achskopplung";
static const char ID_MENU_ACHSKOPPLUNG_en[]     PROGMEM = "Coupl Axes";

/*
static const char ID_MENU_MIXERSETUP_de[]       PROGMEM = "Mixer-Setup";
static const char ID_MENU_MIXERSETUP_en[]       PROGMEM = "Config Mix";
*/

static const char ID_MENU_EASYSETUP_de[]        PROGMEM = "Easy Setup";
#define           ID_MENU_EASYSETUP_en          ID_MENU_EASYSETUP_de



//#############################################################################################
//# Bezeichnungen der paramID's
//#############################################################################################

// Hier ist erstmal alles deklariert was auch in paramset.h vorhanden ist.
// Wenn der Compiler richtig eingestellt ist, dann wird alles nicht benoetigte wegoptimiert!

static const char param_Hoehe_MinGas_de[]                       PROGMEM = "Min. Gas";
static const char param_Hoehe_MinGas_en[]                       PROGMEM = "min. throttle";

static const char param_Luftdruck_D_de[]                        PROGMEM = "Luftdruck D";
static const char param_Luftdruck_D_en[]                        PROGMEM = "barometric D";

static const char param_HoeheChannel_de[]                       PROGMEM = "Höhe Sollwert";
static const char param_HoeheChannel_en[]                       PROGMEM = "hight setpoint";

static const char param_Hoehe_P_de[]                            PROGMEM = "Höhe P";
static const char param_Hoehe_P_en[]                            PROGMEM = "altitude P";

static const char param_Hoehe_Verstaerkung_de[]                 PROGMEM = "Verstärk./Rate";
static const char param_Hoehe_Verstaerkung_en[]                 PROGMEM = "gain rate";

static const char param_Hoehe_ACC_Wirkung_de[]                  PROGMEM = "Z-ACC";
#define           param_Hoehe_ACC_Wirkung_en                    param_Hoehe_ACC_Wirkung_de

static const char param_Hoehe_HoverBand_de[]                    PROGMEM = "Schwebe-Gas";
static const char param_Hoehe_HoverBand_en[]                    PROGMEM = "hoover throttle";

static const char param_Hoehe_GPS_Z_de[]                        PROGMEM = "GPS Z";
#define           param_Hoehe_GPS_Z_en                          param_Hoehe_GPS_Z_de

static const char param_Hoehe_Tilt_Comp_de[]                    PROGMEM = "Tilt Compensation";
#define           param_Hoehe_Tilt_Comp_en                      param_Hoehe_Tilt_Comp_de

static const char param_Hoehe_StickNeutralPoint_de[]            PROGMEM = "Stick Neutr.Punkt";
static const char param_Hoehe_StickNeutralPoint_en[]            PROGMEM = "stick neutr.point";

static const char param_Stick_P_de[]                            PROGMEM = "Nick/Roll P";
static const char param_Stick_D_de[]                            PROGMEM = "Nick/Roll D";
static const char param_StickGier_P_de[]                        PROGMEM = "Gier P";
#define           param_Stick_P_en                              param_Stick_P_de
#define           param_Stick_D_en                              param_Stick_D_de
#define           param_StickGier_P_en                          param_StickGier_P_de


static const char param_Gas_Min_de[]                            PROGMEM = "Min. Gas";
static const char param_Gas_Min_en[]                            PROGMEM = "min. throttle";

static const char param_Gas_Max_de[]                            PROGMEM = "Max. Gas";
static const char param_Gas_Max_en[]                            PROGMEM = "max. throttle";

static const char param_GyroAccFaktor_de[]                      PROGMEM = "ACC/Gyro Faktor";
#define           param_GyroAccFaktor_en                        param_GyroAccFaktor_de

static const char param_KompassWirkung_de[]                     PROGMEM = "Kompasswirkung";
static const char param_KompassWirkung_en[]                     PROGMEM = "compass effect";

static const char param_Gyro_P_de[]                             PROGMEM = "Gyro P";
static const char param_Gyro_I_de[]                             PROGMEM = "Gyro I";
static const char param_Gyro_D_de[]                             PROGMEM = "Gyro D";
static const char param_Gyro_Gier_P_de[]                        PROGMEM = "Gier P";
static const char param_Gyro_Gier_I_de[]                        PROGMEM = "Gier I";
static const char param_Gyro_Stability_de[]                     PROGMEM = "Gyro stab.";
#define           param_Gyro_P_en                               param_Gyro_P_de
#define           param_Gyro_I_en                               param_Gyro_I_de
#define           param_Gyro_D_en                               param_Gyro_D_de
#define           param_Gyro_Gier_P_en                          param_Gyro_Gier_P_de
#define           param_Gyro_Gier_I_en                          param_Gyro_Gier_I_de
#define           param_Gyro_Stability_en                       param_Gyro_Stability_de

static const char param_UnterspannungsWarnung_de[]              PROGMEM = "Unterspannung  [0.1V]";
static const char param_UnterspannungsWarnung_en[]              PROGMEM = "undervoltage   [0.1V]";

static const char param_NotGas_de[]                             PROGMEM = "NOT-Gas";
static const char param_NotGas_en[]                             PROGMEM = "Emerg.Throttle";

static const char param_NotGasZeit_de[]                         PROGMEM = "NOT-Gas Zeit   [0.1s]";
static const char param_NotGasZeit_en[]                         PROGMEM = "Emerg.Thro.Time[0.1s]";

static const char param_Receiver_de[]                           PROGMEM = "Receiver";
#define           param_Receiver_en                             param_Receiver_de

static const char param_I_Faktor_de[]                           PROGMEM = "Hauptregler I";
static const char param_I_Faktor_en[]                           PROGMEM = "main I";

static const char param_UserParam1_de[]                         PROGMEM = "Parameter 1";
static const char param_UserParam2_de[]                         PROGMEM = "Parameter 2";
static const char param_UserParam3_de[]                         PROGMEM = "Parameter 3";
static const char param_UserParam4_de[]                         PROGMEM = "Parameter 4";
#define           param_UserParam1_en                           param_UserParam1_de
#define           param_UserParam2_en                           param_UserParam2_de
#define           param_UserParam3_en                           param_UserParam3_de
#define           param_UserParam4_en                           param_UserParam4_de

static const char param_ServoNickControl_de[]                   PROGMEM = "Nick Ansteuerung";
static const char param_ServoNickControl_en[]                   PROGMEM = "nick servo ctrl";

static const char param_ServoNickComp_de[]                      PROGMEM = "Nick Kompensation";
static const char param_ServoNickComp_en[]                      PROGMEM = "nick servo comp.";

static const char param_ServoNickFails_de[]                     PROGMEM = "Nick Failsave Wert";
static const char param_ServoNickFails_en[]                     PROGMEM = "nick failsave value";


static const char param_ServoNickMin_de[]                       PROGMEM = "Nick Servo Minimum";
static const char param_ServoNickMax_de[]                       PROGMEM = "Nick Servo Maximum";
#define           param_ServoNickMin_en                         param_ServoNickMin_de
#define           param_ServoNickMax_en                         param_ServoNickMax_de

static const char param_ServoRollControl_de[]                   PROGMEM = "Roll Ansteuerung";
static const char param_ServoRollControl_en[]                   PROGMEM = "roll servo ctrl.";

static const char param_ServoRollComp_de[]                      PROGMEM = "Roll Kompensation";
static const char param_ServoRollComp_en[]                      PROGMEM = "roll compensation";

static const char param_ServoRollFails_de[]                     PROGMEM = "Roll Failsave Wert";
static const char param_ServoRollFails_en[]                     PROGMEM = "roll failsave value";

static const char param_ServoRollMin_de[]                       PROGMEM = "Roll Minimum";
static const char param_ServoRollMax_de[]                       PROGMEM = "Roll Maximum";
#define           param_ServoRollMin_en                         param_ServoRollMin_de
#define           param_ServoRollMax_en                         param_ServoRollMax_de

//static const char param_ServoNickRefresh_de[]                   PROGMEM = "Anst. Geschw.";  // ALT
static const char param_ServoNickRefresh_de[]                   PROGMEM = "Anst. Geschwindigkeit";
static const char param_ServoNickRefresh_en[]                   PROGMEM = "servo refresh";

static const char param_ServoManualControlSpeed_de[]            PROGMEM = "Manu. Geschwindigkeit.";
static const char param_ServoManualControlSpeed_en[]            PROGMEM = "manuell Speed";

static const char param_CamOrientation_de[]                     PROGMEM = "Cam Richtung";
static const char param_CamOrientation_en[]                     PROGMEM = "cam orientation";

static const char param_Servo3_de[]                             PROGMEM = "Servo 3";
#define           param_Servo3_en                               param_Servo3_de

static const char param_Servo3Fails_de[]                        PROGMEM = "Servo 3 Failsave Wert";
static const char param_Servo3Fails_en[]                        PROGMEM = "Servo 3 failsave value";

static const char param_Servo4_de[]                             PROGMEM = "Servo 4";
#define           param_Servo4_en                               param_Servo4_de

static const char param_Servo4Fails_de[]                        PROGMEM = "Servo 4 Failsave Wert";
static const char param_Servo4Fails_en[]                        PROGMEM = "Servo 4 failsave value";

static const char param_Servo5_de[]                             PROGMEM = "Servo 5";
#define           param_Servo5_en                               param_Servo5_de

static const char param_Servo5Fails_de[]                        PROGMEM = "Servo 5 Failsave Wert";
static const char param_Servo5Fails_en[]                        PROGMEM = "Servo 5 failsave value";


static const char param_Servo3OnValue_de[]                      PROGMEM = "Servo 3 On     Out1/2";
static const char param_Servo3OffValue_de[]                     PROGMEM = "Servo 3 Off    Out1/2";
static const char param_Servo4OnValue_de[]                      PROGMEM = "Servo 4 On     Out1/2";
static const char param_Servo4OffValue_de[]                     PROGMEM = "Servo 4 Off    Out1/2";
#define           param_Servo3OnValue_en                        param_Servo3OnValue_de
#define           param_Servo3OffValue_en                       param_Servo3OffValue_de
#define           param_Servo4OnValue_en                        param_Servo4OnValue_de
#define           param_Servo4OffValue_en                       param_Servo4OffValue_de


static const char param_LoopGasLimit_de[]                       PROGMEM = "Looping Gas Limit";
static const char param_LoopGasLimit_en[]                       PROGMEM = "loop throttle limit";

static const char param_LoopThreshold_de[]                      PROGMEM = "Ansprechschwelle";
static const char param_LoopThreshold_en[]                      PROGMEM = "response threshold";

static const char param_LoopHysterese_de[]                      PROGMEM = "Hysterese";
static const char param_LoopHysterese_en[]                      PROGMEM = "hysteresis";

static const char param_AchsKopplung1_de[]                      PROGMEM = "Gier pos. Kopplung";
static const char param_AchsKopplung1_en[]                      PROGMEM = "gier pos. coupling";

static const char param_AchsKopplung2_de[]                      PROGMEM = "Nick/Roll Kopplung";
static const char param_AchsKopplung2_en[]                      PROGMEM = "nick/roll coupling";

static const char param_CouplingYawCorrection_de[]              PROGMEM = "Gier Korrektur";
static const char param_CouplingYawCorrection_en[]              PROGMEM = "gier correction";

static const char param_WinkelUmschlagNick_de[]                 PROGMEM = "Nick Umkehrpunkt";
static const char param_WinkelUmschlagNick_en[]                 PROGMEM = "nick turnover";

static const char param_WinkelUmschlagRoll_de[]                 PROGMEM = "Roll Umkehrpunkt";
static const char param_WinkelUmschlagRoll_en[]                 PROGMEM = "roll turnover";

static const char param_GyroAccAbgleich_de[]                    PROGMEM = "ACC/Gyro Komp.  [1/x]";
static const char param_GyroAccAbgleich_en[]                    PROGMEM = "ACC/Gyro Comp.  [1/x]";

static const char param_Driftkomp_de[]                          PROGMEM = "Drift-Kompensation";
static const char param_Driftkomp_en[]                          PROGMEM = "drift compensation";

static const char param_DynamicStability_de[]                   PROGMEM = "Dynamische Stabilität";
static const char param_DynamicStability_en[]                   PROGMEM = "dynamic stabiliy";

static const char param_UserParam5_de[]                         PROGMEM = "Parameter 5";
static const char param_UserParam6_de[]                         PROGMEM = "Parameter 6";
static const char param_UserParam7_de[]                         PROGMEM = "Parameter 7";
static const char param_UserParam8_de[]                         PROGMEM = "Parameter 8";
#define           param_UserParam5_en                           param_UserParam5_de
#define           param_UserParam6_en                           param_UserParam6_de
#define           param_UserParam7_en                           param_UserParam7_de
#define           param_UserParam8_en                           param_UserParam8_de


static const char param_J16Bitmask_de[]                         PROGMEM = "Out1: Bitmaske";
static const char param_J16Bitmask_en[]                         PROGMEM = "Out1: bitmask";

static const char param_J16Timing_de[]                          PROGMEM = "Out1: Timing   [10ms]";
#define           param_J16Timing_en                            param_J16Timing_de

static const char param_J17Bitmask_de[]                         PROGMEM = "Out2: Bitmaske";
static const char param_J17Bitmask_en[]                         PROGMEM = "Out2: bitmask";

static const char param_J17Timing_de[]                          PROGMEM = "Out2: Timing   [10ms]";
#define           param_J17Timing_en                            param_J17Timing_de

static const char param_WARN_J16_Bitmask_de[]                   PROGMEM = "Out1 Warn UBat";
static const char param_WARN_J16_Bitmask_en[]                   PROGMEM = "Out1 undervolt warn";

static const char param_WARN_J17_Bitmask_de[]                   PROGMEM = "Out2 Warn UBat";
static const char param_WARN_J17_Bitmask_en[]                   PROGMEM = "Out2 undervolt warn";

static const char param_AutoPhotoDistance_de[]                  PROGMEM = "Auto Trigger Dist.[m]";      // ab Rev. 100 (z.B. FC 2.05e)
#define           param_AutoPhotoDistance_en                    param_AutoPhotoDistance_de              // ab Rev. 100 (z.B. FC 2.05e)

static const char param_AutoPhotoAtitudes_de[]                  PROGMEM = "Auto Trigger Alt. [m]";      // ab Rev. 100 (z.B. FC 2.05e)
#define           param_AutoPhotoAtitudes_en                    param_AutoPhotoAtitudes_de              // ab Rev. 100 (z.B. FC 2.05e)

#define           param_NaviOut1Parameter_de                    param_AutoPhotoDistance_de              // bis Rev. 98
#define           param_NaviOut1Parameter_en                    param_AutoPhotoDistance_de              // bis Rev. 98

static const char param_SingleWpSpeed_de[]                      PROGMEM = "SingleWP Speed 0.1m/s";      // ab Rev. 100 (z.B. FC 2.05e)
#define           param_SingleWpSpeed_en                        param_SingleWpSpeed_de                  // ab Rev. 100 (z.B. FC 2.05e)

static const char param_LandingPulse_de[]                      PROGMEM = "ACC Z Landing Pulse";        // ab Rev. 104 (FC 2.09d)
#define           param_LandingPulse_en                        param_LandingPulse_de                   // ab Rev. 104 (FC 2.09d)

static const char param_NaviGpsModeChannel_de[]                 PROGMEM = "GPS Modus Steuerung";
static const char param_NaviGpsModeChannel_en[]                 PROGMEM = "GPS mode control";

static const char param_NaviGpsGain_de[]                        PROGMEM = "GPS Verstärkung   [%]";
static const char param_NaviGpsGain_en[]                        PROGMEM = "GPS gain          [%]";

static const char param_NaviGpsP_de[]                           PROGMEM = "GPS-P";
static const char param_NaviGpsI_de[]                           PROGMEM = "GPS-I";
static const char param_NaviGpsD_de[]                           PROGMEM = "GPS-D";
static const char param_NaviGpsPLimit_de[]                      PROGMEM = "GPS-P Limit";
static const char param_NaviGpsILimit_de[]                      PROGMEM = "GPS-I Limit";
static const char param_NaviGpsDLimit_de[]                      PROGMEM = "GPS-D Limit";
static const char param_NaviGpsA_de[]                           PROGMEM = "GPS Acc";
#define           param_NaviGpsP_en                             param_NaviGpsP_de
#define           param_NaviGpsI_en                             param_NaviGpsI_de
#define           param_NaviGpsD_en                             param_NaviGpsD_de
#define           param_NaviGpsPLimit_en                        param_NaviGpsPLimit_de
#define           param_NaviGpsILimit_en                        param_NaviGpsILimit_de
#define           param_NaviGpsDLimit_en                        param_NaviGpsDLimit_de
#define           param_NaviGpsA_en                             param_NaviGpsA_de

static const char param_NaviGpsMinSat_de[]                      PROGMEM = "Min. Satelliten";
static const char param_NaviGpsMinSat_en[]                      PROGMEM = "minimum satelite";

static const char param_NaviStickThreshold_de[]                 PROGMEM = "GPS Stick-Schwelle";
static const char param_NaviStickThreshold_en[]                 PROGMEM = "GPS stick threshold";

static const char param_NaviWindCorrection_de[]                 PROGMEM = "GPS Windkorrektur [%]";
static const char param_NaviWindCorrection_en[]                 PROGMEM = "GPS wind correct. [%]";

static const char param_NaviAccCompensation_de[]                PROGMEM = "ACC Kompensation";
static const char param_NaviAccCompensation_en[]                PROGMEM = "ACC compensation";

static const char param_NaviOperatingRadius_de[]                PROGMEM = "max. Radius";
static const char param_NaviOperatingRadius_en[]                PROGMEM = "max. radius";

static const char param_NaviAngleLimitation_de[]                PROGMEM = "Winkel Limit";
static const char param_NaviAngleLimitation_en[]                PROGMEM = "angle limit";

static const char param_NaviPH_LoginTime_de[]                   PROGMEM = "PH Login Zeit     [s]";
static const char param_NaviPH_LoginTime_en[]                   PROGMEM = "PH login time     [s]";

static const char param_ExternalControl_de[]                    PROGMEM = "Ext. Kontrolle";
static const char param_ExternalControl_en[]                    PROGMEM = "ext. control";

static const char param_OrientationAngle_de[]                   PROGMEM = "OrientationAngle";
#define           param_OrientationAngle_en                     param_OrientationAngle_de

static const char param_CareFreeChannel_de[]                    PROGMEM = "CareFree Steuerung";
static const char param_CareFreeChannel_en[]                    PROGMEM = "careFree control";

static const char param_MotorSafetySwitch_de[]                  PROGMEM = "Motor Sicherh.Schalt.";
static const char param_MotorSafetySwitch_en[]                  PROGMEM = "motor safety switch";

static const char param_MotorSmooth_de[]                        PROGMEM = "Motor Glättung";
static const char param_MotorSmooth_en[]                        PROGMEM = "motor smooth";

static const char param_ComingHomeAltitude_de[]                 PROGMEM = "Com.Home Höhe     [m]";
static const char param_ComingHomeAltitude_en[]                 PROGMEM = "coming home alti. [m]";

static const char param_FailSafeTime_de[]                       PROGMEM = "Fails. CH Zeit    [s]";
static const char param_FailSafeTime_en[]                       PROGMEM = "fails. CH time    [s]";

static const char param_MaxAltitude_de[]                        PROGMEM = "Max. Höhe         [m]";
static const char param_MaxAltitude_en[]                        PROGMEM = "max. altitude     [m]";

static const char param_FailsafeChannel_de[]                    PROGMEM = "Fails. Channel  0=Aus";
static const char param_FailsafeChannel_en[]                    PROGMEM = "fails. channel  0=off";

static const char param_ServoFilterNick_de[]                    PROGMEM = "Nick Filter";
static const char param_ServoFilterNick_en[]                    PROGMEM = "nick filter";

static const char param_ServoFilterRoll_de[]                    PROGMEM = "Roll Filter";
static const char param_ServoFilterRoll_en[]                    PROGMEM = "roll filter";

static const char param_StartLandChannel_de[]                   PROGMEM = "Auto StartLand Kanal";
static const char param_StartLandChannel_en[]                   PROGMEM = "auto start/land chan.";

static const char param_LandingSpeed_de[]                       PROGMEM = "Landing Speed  0.1m/s";
#define           param_LandingSpeed_en                         param_LandingSpeed_de

static const char param_CompassOffset_de[]                      PROGMEM = "Compass Offset    [\x0B]";
#define           param_CompassOffset_en                        param_CompassOffset_de

static const char param_AutoLandingVoltage_de[]                 PROGMEM = "Autoland. Volt [0.1V]";
#define           param_AutoLandingVoltage_en                   param_AutoLandingVoltage_de

static const char param_ComingHomeVoltage_de[]                  PROGMEM = "Coming H. Volt [0.1V]";
#define           param_ComingHomeVoltage_en                    param_ComingHomeVoltage_de

static const char param_BitConfig_de[]                          PROGMEM = "BitConfig";
static const char param_ServoCompInvert_de[]                    PROGMEM = "ServoCompInvert";
static const char param_ExtraConfig_de[]                        PROGMEM = "ExtraConfig";
static const char param_GlobalConfig3_de[]                      PROGMEM = "GlobalConfig3";
static const char param_Name_de[]                               PROGMEM = "Setting Name";
#define           param_BitConfig_en                            param_BitConfig_de
#define           param_ServoCompInvert_en                      param_ServoCompInvert_de
#define           param_ExtraConfig_en                          param_ExtraConfig_de
#define           param_GlobalConfig3_en                        param_GlobalConfig3_de
#define           param_Name_en                                 param_Name_de


static const char param_ComingHomeOrientation_de[]              PROGMEM = "CH Ausrichtung";
static const char param_ComingHomeOrientation_en[]              PROGMEM = "CH orientation";

static const char param_SingleWpControlChannel_de[]             PROGMEM = "Single WP Ctrl Chan.";
#define           param_SingleWpControlChannel_en               param_SingleWpControlChannel_de

static const char param_MenuKeyChannel_de[]                     PROGMEM = "Next WP Channel";
#define           param_MenuKeyChannel_en                       param_MenuKeyChannel_de

static const char param_CamCtrlModeChannel_de[]                 PROGMEM = "CamCtrlModeChannel";
#define           param_CamCtrlModeChannel_en                   param_CamCtrlModeChannel_de

static const char param_CamCtrlZoomChannel_de[]                 PROGMEM = "CamCtrlZoomChannel";
#define           param_CamCtrlZoomChannel_en                   param_CamCtrlZoomChannel_de


// subitems  (Bit / Byte-Felder)
static const char param_ServoCompInvert_SVNick_de[]             PROGMEM = "Nick Umkehren";
static const char param_ServoCompInvert_SVNick_en[]             PROGMEM = "nick inv. direction";

static const char param_ServoCompInvert_SVRoll_de[]             PROGMEM = "Roll Umkehren";
static const char param_ServoCompInvert_SVRoll_en[]             PROGMEM = "Roll inv. direction";

static const char param_ServoCompInvert_SVRelMov_de[]           PROGMEM = "Nick Relativ";
static const char param_ServoCompInvert_SVRelMov_en[]           PROGMEM = "nick relativ";

static const char param_ExtraConfig_HeightLimit_de[]            PROGMEM = "Höhenbegrenzung";
static const char param_ExtraConfig_HeightLimit_en[]            PROGMEM = "heigth limitation";

static const char param_ExtraConfig_HeightVario_de[]            PROGMEM = "Vario-Höhe";         // negiert param_ExtraConfig_HeightLimit
static const char param_ExtraConfig_HeightVario_en[]            PROGMEM = "vario heigth";         // negiert param_ExtraConfig_HeightLimit

static const char param_ExtraConfig_VarioBeep_de[]              PROGMEM = "akust. Vario";
#define           param_ExtraConfig_VarioBeep_en                param_ExtraConfig_VarioBeep_de

static const char param_ExtraConfig_SensitiveRc_de[]            PROGMEM = "Erw. Empf.Sig.Prüfung";
static const char param_ExtraConfig_SensitiveRc_en[]            PROGMEM = "enh. rec. sign. check";

static const char param_ExtraConfig_33vReference_de[]           PROGMEM = "33vReference";
#define           param_ExtraConfig_33vReference_en             param_ExtraConfig_33vReference_de

static const char param_ExtraConfig_NoRcOffBeeping_de[]         PROGMEM = "k.Summer o.Sender";
static const char param_ExtraConfig_NoRcOffBeeping_en[]         PROGMEM = "no beep without TX";

static const char param_ExtraConfig_GpsAid_de[]                 PROGMEM = "Dynamic PH";
#define           param_ExtraConfig_GpsAid_en                   param_ExtraConfig_GpsAid_de

static const char param_ExtraConfig_LearnableCarefree_de[]      PROGMEM = "Teachable CF";
#define           param_ExtraConfig_LearnableCarefree_en        param_ExtraConfig_LearnableCarefree_de

static const char param_ExtraConfig_IgnoreMagErrAtStartup_de[]  PROGMEM = "Kompass Fehler ignor.";
static const char param_ExtraConfig_IgnoreMagErrAtStartup_en[]  PROGMEM = "ignore compass error";

static const char param_BitConfig_LoopOben_de[]                 PROGMEM = "Looping Oben";
static const char param_BitConfig_LoopOben_en[]                 PROGMEM = "looping up";

static const char param_BitConfig_LoopUnten_de[]                PROGMEM = "Looping Unten";
static const char param_BitConfig_LoopUnten_en[]                PROGMEM = "looping down";

static const char param_BitConfig_LoopLinks_de[]                PROGMEM = "Looping Links";
static const char param_BitConfig_LoopLinks_en[]                PROGMEM = "looping left";

static const char param_BitConfig_LoopRechts_de[]               PROGMEM = "Looping Rechts";
static const char param_BitConfig_LoopRechts_en[]               PROGMEM = "looping right";

static const char param_BitConfig_Led1_MotorRun_de[]              PROGMEM = "nur wenn Motor An";
static const char param_BitConfig_Led1_MotorRun_en[]              PROGMEM = "only with motor on";

static const char param_BitConfig_MotorOffLed1_de[]             PROGMEM = " \x19 sofort an";
static const char param_BitConfig_MotorOffLed1_en[]             PROGMEM = " \x19 immediately on";

#define           param_BitConfig_MotorOffLed2_de               param_BitConfig_MotorOffLed1_de
#define           param_BitConfig_Led2_MotorRun_de                param_BitConfig_Led1_MotorRun_de

#define           param_BitConfig_MotorOffLed2_en               param_BitConfig_MotorOffLed1_en
#define           param_BitConfig_Led2_MotorRun_en                param_BitConfig_Led1_MotorRun_en


static const char param_GlobalConfig3_NoSdCardNoStart_de[]      PROGMEM = "k.Start o.SD-Karte";
static const char param_GlobalConfig3_NoSdCardNoStart_en[]      PROGMEM = "no start w/o SD-card";

static const char param_GlobalConfig3_DphMaxRadius_de[]         PROGMEM = "Max.Radius dPH";
#define           param_GlobalConfig3_DphMaxRadius_en           param_GlobalConfig3_DphMaxRadius_de

static const char param_GlobalConfig3_VarioFailsafe_de[]        PROGMEM = "NOT-Gas Vario Höhe";
static const char param_GlobalConfig3_VarioFailsafe_en[]        PROGMEM = "Emerg.thr.vario h.";

static const char param_GlobalConfig3_MotorSwitchMode_de[]      PROGMEM = "Motor Swi.Mode";
#define           param_GlobalConfig3_MotorSwitchMode_en        param_GlobalConfig3_MotorSwitchMode_de

static const char param_GlobalConfig3_NoGpsFixNoStart_de[]      PROGMEM = "k.Start o.GPS Fix";
static const char param_GlobalConfig3_NoGpsFixNoStart_en[]      PROGMEM = "no start w/o GPS Fix";

static const char param_GlobalConfig3_UseNcForOut1_de[]         PROGMEM = "mit WP-Event  verkn.";
static const char param_GlobalConfig3_UseNcForOut1_en[]         PROGMEM = "combine with WP-Event";

static const char param_GlobalConfig3_SpeakAll_de[]             PROGMEM = "Alles Ansagen";
static const char param_GlobalConfig3_SpeakAll_en[]             PROGMEM = "speak all";

static const char param_GlobalConfig3_ServoNickCompOff_de[]     PROGMEM = "Nick Komp. Aus";
static const char param_GlobalConfig3_ServoNickCompOff_en[]     PROGMEM = "nick compensation off";

static const char param_GlobalConfig_HoehenRegelung_de[]        PROGMEM = "Höhenregelung";
static const char param_GlobalConfig_HoehenRegelung_en[]        PROGMEM = "height control";

static const char param_GlobalConfig_HoehenSchalter_de[]        PROGMEM = "Schalter Höhe";
static const char param_GlobalConfig_HoehenSchalter_en[]        PROGMEM = "height switch";

static const char param_GlobalConfig_HeadingHold_de[]           PROGMEM = "Heading Hold";
#define           param_GlobalConfig_HeadingHold_en             param_GlobalConfig_HeadingHold_de

static const char param_GlobalConfig_KompassAktiv_de[]          PROGMEM = "Kompass Aktiv";
static const char param_GlobalConfig_KompassAktiv_en[]          PROGMEM = "compass aktiv";

static const char param_GlobalConfig_KompassFix_de[]            PROGMEM = "Kompass Fix";
static const char param_GlobalConfig_KompassFix_en[]            PROGMEM = "compass fix";

static const char param_GlobalConfig_GpsAktiv_de[]              PROGMEM = "GPS Aktiv";
#define           param_GlobalConfig_GpsAktiv_en                param_GlobalConfig_GpsAktiv_de

static const char param_GlobalConfig_AchsenkopplungAktiv_de[]   PROGMEM = "Achs(ent)kopplung";
static const char param_GlobalConfig_AchsenkopplungAktiv_en[]   PROGMEM = "(De)Coupl Axes";

static const char param_GlobalConfig_DrehratenBegrenzer_de[]    PROGMEM = "Drehratenbegrenzung";
static const char param_GlobalConfig_DrehratenBegrenzer_en[]    PROGMEM = "rotary rate limit.";

static const char param_Kanalbelegung_Nick_de[]                 PROGMEM = "Nick";
static const char param_Kanalbelegung_Roll_de[]                 PROGMEM = "Roll";
#define           param_Kanalbelegung_Nick_en                   param_Kanalbelegung_Nick_de
#define           param_Kanalbelegung_Roll_en                   param_Kanalbelegung_Roll_de

static const char param_Kanalbelegung_Gas_de[]                  PROGMEM = "Gas";
static const char param_Kanalbelegung_Gas_en[]                  PROGMEM = "throttle";

static const char param_Kanalbelegung_Gear_de[]                 PROGMEM = "Gear";
#define           param_Kanalbelegung_Gear_en                   param_Kanalbelegung_Gear_de

static const char param_Kanalbelegung_Poti1_de[]                PROGMEM = "Poti 1";
static const char param_Kanalbelegung_Poti2_de[]                PROGMEM = "Poti 2";
static const char param_Kanalbelegung_Poti3_de[]                PROGMEM = "Poti 3";
static const char param_Kanalbelegung_Poti4_de[]                PROGMEM = "Poti 4";
static const char param_Kanalbelegung_Poti5_de[]                PROGMEM = "Poti 5";
static const char param_Kanalbelegung_Poti6_de[]                PROGMEM = "Poti 6";
static const char param_Kanalbelegung_Poti7_de[]                PROGMEM = "Poti 7";
static const char param_Kanalbelegung_Poti8_de[]                PROGMEM = "Poti 8";
#define           param_Kanalbelegung_Poti1_en                  param_Kanalbelegung_Poti1_de
#define           param_Kanalbelegung_Poti2_en                  param_Kanalbelegung_Poti2_de
#define           param_Kanalbelegung_Poti3_en                  param_Kanalbelegung_Poti3_de
#define           param_Kanalbelegung_Poti4_en                  param_Kanalbelegung_Poti4_de
#define           param_Kanalbelegung_Poti5_en                  param_Kanalbelegung_Poti5_de
#define           param_Kanalbelegung_Poti6_en                  param_Kanalbelegung_Poti6_de
#define           param_Kanalbelegung_Poti7_en                  param_Kanalbelegung_Poti7_de
#define           param_Kanalbelegung_Poti8_en                  param_Kanalbelegung_Poti8_de

static const char param_CompassOffset_DisableDeclCalc_de[]      PROGMEM = "Disable Decl. Calc";
#define           param_CompassOffset_DisableDeclCalc_en        param_CompassOffset_DisableDeclCalc_de


static const char param_NaviMaxFlyingRange_de[]                 PROGMEM = "Max. Flugradius [10m]";
static const char param_NaviMaxFlyingRange_en[]                 PROGMEM = "Max. Flightrad. [10m]";

static const char param_NaviDescendRange_de[]                   PROGMEM = "Fail SinkRadius [10m]";
#define           param_NaviDescendRange_en                     param_NaviDescendRange_de

static const char param_GlobalConfig3_RiseFirstWP_de[]          PROGMEM = "Höhe vor 1.WP";
static const char param_GlobalConfig3_RiseFirstWP_en[]          PROGMEM = "Rise before 1. WP";

static const char param_ExtraConfig_FlyZoneRequired_de[]		PROGMEM = "Flugzone erforderlich";
static const char param_ExtraConfig_FlyZoneRequired_en[]		PROGMEM = "Flyzone required";

static const char param_GlobalConfig_LandOutsideFlyzone_de[]	PROGMEM = "lande ausserh.Flugzon";
static const char param_GlobalConfig_LandOutsideFlyzone_en[]	PROGMEM = "land outside Flyzone";

static const char param_FailsafeAltitude_de[]			        PROGMEM = "Failsave Altitude";
#define param_FailsafeAltitude_en								param_FailsafeAltitude_de

static const char param_GimbalYawChannel_de[]					PROGMEM = "Yaw Channel";
#define param_GimbalYawChannel_en								param_GimbalYawChannel_de

static const char param_GimbalOut1Channel_de[]					PROGMEM = "Gimbal Out1";
#define param_GimbalOut1Channel_en								param_GimbalOut1Channel_de

static const char param_GimbalOut2Channel_de[]					PROGMEM = "Gimbal Out2";
#define param_GimbalOut2Channel_en								param_GimbalOut2Channel_de

#endif  // _MKPARAMETERS_H
