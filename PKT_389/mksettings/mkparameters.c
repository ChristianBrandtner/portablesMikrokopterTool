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
//# HISTORY  mkparameters.c
//#
//# 06.11.2017 Cebra
//# - add: param_FailsafeAltitude eingefügt in Parameter Verschiedenses
//#
//# 31.11.2017 Cebra
//# - add:   { param_GlobalConfig3_RiseFirstWP, &editGeneric, GENERIC_NoYes ,109,  0, param_GlobalConfig3_RiseFirstWP_de,   param_GlobalConfig3_RiseFirstWP_en  },
//# - chg:  Menüreihenfolge der MK Parameter an Mikrokoptertool angepasst
//# - chg:  Loopingmenüpunkt deaktiviert
//#
//# 18.01.2016 Cebra
//# - add: Erweiterung CamCtrlModeChannel; (FC2.13a)
//#                    CamCtrlZoomChannel; (FC2.13a)
//#                    MainMenü um MenÃ¼punkt ID_MENU_CAMCTRL erweitert
//#
//# 16.07.2015 Cebra
//# - add: Erweiterung SingleWpControlChannel; (FC2.11a)
//#        MenuKeyChannel; (FC2.11a)
//#
//# 09.04.2015 Cebra
//# - add: Erweiterung paramEditItemTable und ID_MENU_KAMERA_Items[] um neue Parameter (FC2.09j)
//#        param_ServoNickFailsave, param_ServoRollFailsave, param_Servo3Failsave, param_Servo4Failsave, param_Servo5Failsave
//#
//# 26.01.2015 Cebra
//# - add: Comming Home Ausrichtung hinzugefügt,
//#        neue Einstellungen ab FC 209a im Wert ServoCompInvert, Bit4 + Bit5
//#
//# 26.09.2014 Cebra
//# - add: im Menü Höhe -> param_Hoehe_TiltCompensation, FC207d
//#
//# 04.06.2014 OG
//# - chg: MK_Parameters_MenuMain() eine Menue-Trennlinie hinter Favoriten eingefuegt
//#
//# 14.05.2014 OG
//# - chg: include "mkbase.h" geaendert auf "../mk/mkbase.h"
//#
//# 11.05.2014 OG
//# - chg: Menu_Favoriten() umgestellt auf MenuCtrl_SetTitleFromParentItem()
//# - chg: Menu_EditCategory() umgestellt auf MenuCtrl_SetTitleFromParentItem()
//#
//# 10.05.2014 OG
//# - del: editDisableDeclCalc() - wurde ersetzt durch eine Transform-Funktion
//#        in paramset.c und wird jetzt von editGeneric() bearbeitet
//#
//# 07.05.2014 OG
//# - chg: Menu_Favoriten() - uebernimmt den gegebenen Menuetitel vom
//#        uebergeordneten Eintrag aus mkparameters_messages.h (also Multilanguage)
//# - chg: fav_add() - erweitert um Menue-Separatoren (Trennlinien) aufzunehmen
//# - chg: Menu_EditCategory() umgestellt auf MenuCtrl_PushSeparatorID()
//#
//# 06.05.2014 OG
//# - add: Favoriten-Verwaltung implementiert (Aenderungen an verschiedenen Funktionen)
//#
//# 18.04.2014 OG
//# - fix: im ID_MENU_NAVICTRL fehlten param_NaviStickThreshold ("GPS Stick-Schwelle")
//#        und param_NaviGpsMinSat ("Min. Sat")
//#
//# 17.04.2014 OG
//# - add: param_Servo3OnValue, param_Servo3OffValue, param_Servo4OnValue
//#        param_Servo4OffValue
//# - add: param_NaviMaxFlyingRange, param_NaviDescendRange
//# - chg: Menu_EditCategory() blendet ggf. doppelt aufeinanderfolgenden
//#        Menue-Separatoren aus
//#
//# 30.03.2014 OG
//# - chg: Sprache Hollaendisch vollstaendig entfernt
//# - chg: MenuCtrl_PushML_P() umgestellt auf MenuCtrl_PushML2_P()
//#
//# 29.03.2014 OG
//# - chg: versch. Funktioionen del: MenuCtrl_SetShowBatt() wegen Aenderung Default auf true
//# - add: Unterstuetzung fuer Rev. 100
//#        (param_AutoPhotoDistance, param_AutoPhotoAtitudes, param_SingleWpSpeed)
//#
//# 27.03.2014 OG
//# kompletter neuer Code fuer ein erstes Release
//#
//# 23.02.2014 OG
//# - chg: MK_Parameters_Menu() umbenannt zu MK_Parameters()
//#
//# 20.02.2014 OG
//# - chg: MK_Parameters_Menu() meldet "nicht verfügbar"
//#
//# 12.02.2014 OG - NEU
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

//#include "../lipo/lipo.h"
#include "../main.h"
#include "../lipo/lipo.h"
#include "../lcd/lcd.h"
#include "../uart/usart.h"
#include "../utils/menuctrl.h"
#include "../utils/xutils.h"

#include "../uart/uart1.h"
#include "../mk-data-structs.h"
//#include "../menu.h"
#include "../timer/timer.h"
#include "../eeprom/eeprom.h"
#include "../messages.h"
#include "../pkt/pkt.h"
#include "../mk/mkbase.h"
#include "paramset.h"
#include "mkparameters.h"
#include "mkparameters_messages.h"


//#############################################################################################
//# Strukturen; Forward-Deklarationen
//#############################################################################################

#define DEBUG_PARAMEDIT         // schaltet zusaetzliche Debug-Ausgaben ein

#define EOF             255     // End Of File (bzw. end of Table...)

#define SEPARATOR       254     // ID fuer einen Separator in einem Menue (Trennlinie)


#define SCREEN_REFRESH  1       // neuzeichnen/refresh der Anzeige
#define SCREEN_REDRAW   2       // neuzeichnen/refresh der Anzeige


//--------------------------------------------
// editGenericCode_t
// deklariert ein einzelnes Code-Zeichen fuer Edit-Generic
//--------------------------------------------
typedef struct
{
    unsigned char   code;                       // z.B. '0', '1', 'v', 'P', 'C' ... (einzelnes Zeichen aus paramEditItem_t.format )
    unsigned char   min;
    unsigned char   max;
    const char      *shortText_de;              //
    const char      *shortText_en;
    const char      *longText_de;
    const char      *longText_en;
} editGenericCode_t;



//---------------------------------------------
//----- globale Modulvariablen
//---------------------------------------------

editGenericCode_t   genericCode;                        // Daten eines einzelnen Code-Zeichen (im RAM)
paramEditItem_t     paramEditItem;                      // RAM Buffer: fuer ein Element von paramEditDef
char                paramEditFormat[MKPARAM_STRBUFFER_LEN];     // RAM Buffer: fuer 'str' von paramEdit (Format; editGeneric)
char                mkparam_strValueBuffer[MKPARAM_STRBUFFER_LEN];      // Anzeige eines Values als Klartext; Kurz (fuer das Menue) oder Lang (in der Edit-Funktion)



//#############################################################################################
//#
//#############################################################################################

static const char GENERIC_SHORT_VALUE_de[]          PROGMEM = "%3u";            // nur Wert anzeigen - min/max wird uebergeordnet definiert
#define           GENERIC_SHORT_VALUE_en            GENERIC_SHORT_VALUE_de
#define           GENERIC_LONG_VALUE_de             GENERIC_SHORT_VALUE_de
#define           GENERIC_LONG_VALUE_en             GENERIC_SHORT_VALUE_de
//
//static const char GENERIC_SHORT_VALUE_ACCZ_de[]     PROGMEM = "%4u";            // nur Wert anzeigen - min/max wird uebergeordnet definiert
//#define           GENERIC_SHORT_VALUE_ACCZ_en       GENERIC_SHORT_VALUE_ACCZ_de
//#define           GENERIC_LONG_VALUE_ACCZ_de        GENERIC_SHORT_VALUE_ACCZ_de
//#define           GENERIC_LONG_VALUE_ACCZ_en        GENERIC_SHORT_VALUE_ACCZ_de


static const char GENERIC_SHORT_NO_de[]             PROGMEM = "  N";
#define           GENERIC_SHORT_NO_en               GENERIC_SHORT_NO_de
static const char GENERIC_LONG_NO_de[]              PROGMEM = "Nein";
static const char GENERIC_LONG_NO_en[]              PROGMEM = "No";

static const char GENERIC_SHORT_YES_de[]            PROGMEM = "  J";
static const char GENERIC_SHORT_YES_en[]            PROGMEM = "  Y";
static const char GENERIC_LONG_YES_de[]             PROGMEM = "Ja";
static const char GENERIC_LONG_YES_en[]             PROGMEM = "Yes";

static const char GENERIC_SHORT_POTI_de[]           PROGMEM = " P%u";
#define           GENERIC_SHORT_POTI_en             GENERIC_SHORT_POTI_de
static const char GENERIC_LONG_POTI_de[]            PROGMEM = "Poti %u";
#define           GENERIC_LONG_POTI_en              GENERIC_LONG_POTI_de

static const char GENERIC_SHORT_CHANNEL_de[]        PROGMEM = "C%2u";
#define           GENERIC_SHORT_CHANNEL_en          GENERIC_SHORT_CHANNEL_de
static const char GENERIC_LONG_CHANNEL_de[]         PROGMEM = "Kanal %u";
static const char GENERIC_LONG_CHANNEL_en[]         PROGMEM = "Channel %u";

static const char GENERIC_SHORT_SERCHANNEL_de[]     PROGMEM = "S%2u";
#define           GENERIC_SHORT_SERCHANNEL_en       GENERIC_SHORT_SERCHANNEL_de
static const char GENERIC_LONG_SERCHANNEL_de[]      PROGMEM = "Ser. Kanal %u";
static const char GENERIC_LONG_SERCHANNEL_en[]      PROGMEM = "Ser. Channel %u";

static const char GENERIC_SHORT_AUS_de[]            PROGMEM = "Aus";
static const char GENERIC_SHORT_AUS_en[]            PROGMEM = "Off";
#define           GENERIC_LONG_AUS_de               GENERIC_SHORT_AUS_de
#define           GENERIC_LONG_AUS_en               GENERIC_SHORT_AUS_en

static const char GENERIC_SHORT_INACTIV_de[]        PROGMEM = "Ina";
#define           GENERIC_SHORT_INACTIV_en          GENERIC_SHORT_INACTIV_de
static const char GENERIC_LONG_INACTIV_de[]         PROGMEM = "Inaktiv";
#define           GENERIC_LONG_INACTIV_en           GENERIC_LONG_INACTIV_de

static const char GENERIC_SHORT_WPEVENT_de[]        PROGMEM = "WPE";
#define           GENERIC_SHORT_WPEVENT_en          GENERIC_SHORT_WPEVENT_de
static const char GENERIC_LONG_WPEVENT_de[]         PROGMEM = "WP-Event";
#define           GENERIC_LONG_WPEVENT_en           GENERIC_LONG_WPEVENT_de

static const char GENERIC_SHORT_MINIMUM_de[]        PROGMEM = "Min";
#define           GENERIC_SHORT_MINIMUM_en          GENERIC_SHORT_MINIMUM_de
static const char GENERIC_LONG_MINIMUM_de[]         PROGMEM = "Minimum";
#define           GENERIC_LONG_MINIMUM_en           GENERIC_LONG_MINIMUM_de

static const char GENERIC_SHORT_MIDDLE_de[]         PROGMEM = "Mid";
#define           GENERIC_SHORT_MIDDLE_en           GENERIC_SHORT_MIDDLE_de
static const char GENERIC_LONG_MIDDLE_de[]          PROGMEM = "Mitte";
static const char GENERIC_LONG_MIDDLE_en[]          PROGMEM = "Middle";

static const char GENERIC_SHORT_MAXIMUM_de[]        PROGMEM = "Max";
#define           GENERIC_SHORT_MAXIMUM_en          GENERIC_SHORT_MAXIMUM_de
static const char GENERIC_LONG_MAXIMUM_de[]         PROGMEM = "Maximum";
#define           GENERIC_LONG_MAXIMUM_en           GENERIC_LONG_MAXIMUM_de

static const char GENERIC_SHORT_AN_de[]             PROGMEM = " An";
static const char GENERIC_SHORT_AN_en[]             PROGMEM = " On";
#define           GENERIC_LONG_AN_de                GENERIC_SHORT_AN_de
#define           GENERIC_LONG_AN_en                GENERIC_SHORT_AN_en

static const char GENERIC_SHORT_FREE_de[]           PROGMEM = "Fre";                    // z.b. "GPS Modus Steuerung"
#define           GENERIC_SHORT_FREE_en             GENERIC_SHORT_FREE_de
static const char GENERIC_LONG_FREE_de[]            PROGMEM = "Free";
#define           GENERIC_LONG_FREE_en              GENERIC_LONG_FREE_de

static const char GENERIC_SHORT_CH_de[]             PROGMEM = " CH";                    // z.b. "GPS Modus Steuerung"
#define           GENERIC_SHORT_CH_en               GENERIC_SHORT_CH_de
static const char GENERIC_LONG_CH_de[]              PROGMEM = "Coming Home (CH)";
#define           GENERIC_LONG_CH_en                GENERIC_LONG_CH_de

static const char GENERIC_SHORT_PH_de[]             PROGMEM = " PH";                    // z.b. "GPS Modus Steuerung"
#define           GENERIC_SHORT_PH_en               GENERIC_SHORT_PH_de
static const char GENERIC_LONG_PH_de[]              PROGMEM = "Position Hold (PH)";
#define           GENERIC_LONG_PH_en                GENERIC_LONG_PH_de

static const char GENERIC_SHORT_DISABLED_de[]       PROGMEM = "Dis";                    // z.b. "Auto Start / Land"
#define           GENERIC_SHORT_DISABLED_en         GENERIC_SHORT_DISABLED_de
static const char GENERIC_LONG_DISABLED_de[]        PROGMEM = "Disabled";
#define           GENERIC_LONG_DISABLED_en          GENERIC_LONG_DISABLED_de

static const char GENERIC_SHORT_OUT_de[]            PROGMEM = "Ou%1u";                  // z.B. Servo 3 und Servo 4
#define           GENERIC_SHORT_OUT_en              GENERIC_SHORT_OUT_de
static const char GENERIC_LONG_OUT_de[]             PROGMEM = "Out %1u";
#define           GENERIC_LONG_OUT_en               GENERIC_LONG_OUT_de

static const char GENERIC_SHORT_CH_OR_NC_de[]       PROGMEM = "kÄn";
static const char GENERIC_SHORT_CH_OR_NC_en[]       PROGMEM = "nCh";
static const char GENERIC_LONG_CH_OR_NC_de[]        PROGMEM = "Keine Änderung";
static const char GENERIC_LONG_CH_OR_NC_en[]        PROGMEM = "no change";

static const char GENERIC_SHORT_CH_OR_FH_de[]       PROGMEM = "FzH";
static const char GENERIC_SHORT_CH_OR_FH_en[]       PROGMEM = "FtH";
static const char GENERIC_LONG_CH_OR_FH_de[]        PROGMEM = "Front zu Home";
static const char GENERIC_LONG_CH_OR_FH_en[]        PROGMEM = "front to home";

static const char GENERIC_SHORT_CH_OR_RH_de[]       PROGMEM = "HzH";
static const char GENERIC_SHORT_CH_OR_RH_en[]       PROGMEM = "RtH";
static const char GENERIC_LONG_CH_OR_RH_de[]        PROGMEM = "Heck zu Home";
static const char GENERIC_LONG_CH_OR_RH_en[]        PROGMEM = "rear to home";

static const char GENERIC_SHORT_CH_OR_SO_de[]       PROGMEM = "wSt";
static const char GENERIC_SHORT_CH_OR_SO_en[]       PROGMEM = "sOr";
static const char GENERIC_LONG_CH_OR_SO_de[]        PROGMEM = "wie beim Start";
static const char GENERIC_LONG_CH_OR_SO_en[]        PROGMEM = "same as start";



//------------------------------------------------



/*************************************************
//-------------
// ZUR INFO
//-------------
typedef struct
{
    unsigned char   code;       // '0', '1', 'v', 'P', 'C' ...
    unsigned char   min;
    unsigned char   max;
    const char      *shortText_de;
    const char      *shortText_en;
    const char      *longText_de;
    const char      *longText_en;
} editGenericCode_t;
*************************************************/

// Anmerkung: kann bei Bedarf evtl. zu PROGMEM umschreiben
//editGenericCode_t const editGenericCode[] PROGMEM =

editGenericCode_t editGenericCode[] =
{
  {  'v',    0,  0, GENERIC_SHORT_VALUE_de,         GENERIC_SHORT_VALUE_en,         GENERIC_LONG_VALUE_de,          GENERIC_LONG_VALUE_en       },      // nur Wert anzeigen - min/max wird uebergeordnet definiert
//  {  'z',    0,  0, GENERIC_SHORT_VALUE_ACCZ_de,    GENERIC_SHORT_VALUE_ACCZ_en,    GENERIC_LONG_VALUE_ACCZ_de,     GENERIC_LONG_VALUE_ACCZ_en  },      // nur Wert anzeigen x 4- min/max wird uebergeordnet definiert
  {  '0',    0,  0, GENERIC_SHORT_NO_de,            GENERIC_SHORT_NO_en,            GENERIC_LONG_NO_de,             GENERIC_LONG_NO_en          },
  {  '1',    1,  1, GENERIC_SHORT_YES_de,           GENERIC_SHORT_YES_en,           GENERIC_LONG_YES_de,            GENERIC_LONG_YES_en         },
  {  'P',  255,248, GENERIC_SHORT_POTI_de,          GENERIC_SHORT_POTI_en,          GENERIC_LONG_POTI_de,           GENERIC_LONG_POTI_en        },
  {  'C',    1, 16, GENERIC_SHORT_CHANNEL_de,       GENERIC_SHORT_CHANNEL_en,       GENERIC_LONG_CHANNEL_de,        GENERIC_LONG_CHANNEL_en     },
  {  'S',   17, 28, GENERIC_SHORT_SERCHANNEL_de,    GENERIC_SHORT_SERCHANNEL_en,    GENERIC_LONG_SERCHANNEL_de,     GENERIC_LONG_SERCHANNEL_en  },
  {  'A',    0,  0, GENERIC_SHORT_AUS_de,           GENERIC_SHORT_AUS_en,           GENERIC_LONG_AUS_de,            GENERIC_LONG_AUS_en         },
  {  'I',    0,  0, GENERIC_SHORT_INACTIV_de,       GENERIC_SHORT_INACTIV_en,       GENERIC_LONG_INACTIV_de,        GENERIC_LONG_INACTIV_en     },
  {  'W',   29, 29, GENERIC_SHORT_WPEVENT_de,       GENERIC_SHORT_WPEVENT_en,       GENERIC_LONG_WPEVENT_de,        GENERIC_LONG_WPEVENT_en     },
  {  'U',   30, 30, GENERIC_SHORT_MINIMUM_de,       GENERIC_SHORT_MINIMUM_en,       GENERIC_LONG_MINIMUM_de,        GENERIC_LONG_MINIMUM_en     },
  {  'M',   31, 31, GENERIC_SHORT_MIDDLE_de,        GENERIC_SHORT_MIDDLE_en,        GENERIC_LONG_MIDDLE_de,         GENERIC_LONG_MIDDLE_en      },
  {  'X',   32, 32, GENERIC_SHORT_MAXIMUM_de,       GENERIC_SHORT_MAXIMUM_en,       GENERIC_LONG_MAXIMUM_de,        GENERIC_LONG_MAXIMUM_en     },
  {  'O',   30, 30, GENERIC_SHORT_AUS_de,           GENERIC_SHORT_AUS_en,           GENERIC_LONG_AUS_de,            GENERIC_LONG_AUS_en         },      // ein weiteres "Aus"; z.b. "Motors-Sicherheitsschalter"
  {  'N',   31, 31, GENERIC_SHORT_AN_de,            GENERIC_SHORT_AN_en,            GENERIC_LONG_AN_de,             GENERIC_LONG_AN_en          },      // z.b. "Carefree"
  {  'F',   30, 30, GENERIC_SHORT_FREE_de,          GENERIC_SHORT_FREE_en,          GENERIC_LONG_FREE_de,           GENERIC_LONG_FREE_en        },      // Free - z.b. "Carefree"
  {  'H',   31, 31, GENERIC_SHORT_CH_de,            GENERIC_SHORT_CH_en,            GENERIC_LONG_CH_de,             GENERIC_LONG_CH_en          },      // Coming Home - z.b. "Carefree"
  {  'Q',   32, 32, GENERIC_SHORT_PH_de,            GENERIC_SHORT_PH_en,            GENERIC_LONG_PH_de,             GENERIC_LONG_PH_en          },      // Positiosn Hold - z.b. "Carefree"
  {  'D',    0,  0, GENERIC_SHORT_DISABLED_de,      GENERIC_SHORT_DISABLED_en,      GENERIC_LONG_DISABLED_de,       GENERIC_LONG_DISABLED_en    },      // Inaktiv - z.b. "Auto Start / Land"
  {  'T',  247,246, GENERIC_SHORT_OUT_de,           GENERIC_SHORT_OUT_en,           GENERIC_LONG_OUT_de,            GENERIC_LONG_OUT_en         },
  {  'K',    0,  0, GENERIC_SHORT_CH_OR_NC_de,      GENERIC_SHORT_CH_OR_NC_en,      GENERIC_LONG_CH_OR_NC_de,       GENERIC_LONG_CH_OR_NC_en    },      // CommingHome Orientation,
  {  'V',    1,  1, GENERIC_SHORT_CH_OR_FH_de,      GENERIC_SHORT_CH_OR_FH_en,      GENERIC_LONG_CH_OR_FH_de,       GENERIC_LONG_CH_OR_FH_en    },      // CommingHome Orientation,
  {  'R',    2,  2, GENERIC_SHORT_CH_OR_RH_de,      GENERIC_SHORT_CH_OR_RH_en,      GENERIC_LONG_CH_OR_RH_de,       GENERIC_LONG_CH_OR_RH_en    },      // CommingHome Orientation,
  {  'G',    3,  3, GENERIC_SHORT_CH_OR_SO_de,      GENERIC_SHORT_CH_OR_SO_en,      GENERIC_LONG_CH_OR_SO_de,       GENERIC_LONG_CH_OR_SO_en    },      // CommingHome Orientation,

  {  EOF,    0,  0,  0,0,0,0 }     // END OF LIST - MUST BE THE LAST!!
};







//#############################################################################################
//#
//#############################################################################################


static const char GENERIC_NoYes[]               PROGMEM = "01";         // Nein, Ja                                                         (z.B. (fast) alle Checkboxen)
static const char GENERIC_Value[]               PROGMEM = "v";          // value mit min,max aus paramEditItem (in paramEditItemTable)      (z.B. Nick/Roll P (0..20) / Min. Gas (0..247) )
static const char GENERIC_ValueACCZ[]           PROGMEM = "D";          // value mit min,max aus paramEditItem (in paramEditItemTable) x 4  (ACC Z Landing pulse )
static const char GENERIC_ValuePoti[]           PROGMEM = "vP";         // value, Poti                                                      (z.B. Gyro Gier P)
static const char GENERIC_ValueOutPoti[]        PROGMEM = "vTP";        // value, Out, Poti                                                 (z.B. Servo 3 & 4)
static const char GENERIC_Cannel[]              PROGMEM = "C";          // Channel                                                          (z.B. Gas / Gier / Nick / Roll)
static const char GENERIC_AusChSerWpeMMM[]      PROGMEM = "ACSWUMX";    // Aus, Channel, Ser. Channel, WP Event, Minimum, Mitte, Maximum    (z.B. Poti 1..8)
static const char GENERIC_DisChSerWpeOff[]      PROGMEM = "DCSWO";      // Disabled, Channel, Ser. Channel, WP Event, Off                   (z.B. Motors-Sicherheitsschalter)
static const char GENERIC_DisChSerWpeOffOn[]    PROGMEM = "DCSWON";     // Disabled, Channel, Ser. Channel, WP Event, Off, On               (z.B. Carefree)
static const char GENERIC_DisChSerWpeFreeCHPH[] PROGMEM = "DCSWFHQ";    // Disabled, Channel, Ser. Channel, WP Event, Free, CH, PH          (z.B. GPS Modus Steuerung)
static const char GENERIC_DisChSer[]            PROGMEM = "DCS";        // Disabled, Channel, Ser. Channel                                  (z.B. Auto Start / Land)
static const char GENERIC_CH_Orientation[]      PROGMEM = "KVRG";       // Keine Änderung, vorne Home, Heck Home, gleiche Richtung          (Comming Home Orientation
static const char GENERIC_InaChSerWpeMMM[]      PROGMEM = "ICSWUMX";    // Inactiv, Channel, Ser. Channel, WP Event, Minimum, Mitte, Maximum(z.B. Menu Key Channel
static const char GENERIC_InaChSerCh[]          PROGMEM = "ICS";        // Inactiv, Channel, Ser. Channel                                   ( z.B. CamCtrl Menü



/*************************************************
//-------------
// ZUR INFO
//-------------
typedef struct
{
    unsigned char paramID;                                  // paramID aus paramset.h
    void (*editfunc)(unsigned char paramID, uint8_t cmd);   // Edit-Funktion - z.B. editGeneric(); cmd = MKPARAM_EDIT oder MKPARAM_SHORTVALUE
    const char    *format;                                  // Parameter: String (PROGMEM) (vorallem fuer editGeneric() )
    unsigned char  min;                                     // Parameter: min (P1)
    unsigned char  max;                                     // Parameter: max (P2)
    const char    *title_de;                                // Text in PROGMEM - Menuetext und Beschreibung im Edit-Screen
    const char    *title_en;                                // Text in PROGMEM
} paramEditItem_t;
*************************************************/

//----------------------
// HINWEIS!
// Die unten stehende Aufgliederung in die verschiedenen Menue-Bereiche ist nur zur ORIENTIERUNG!
// Jede paramID wird nur EINMAL deklariert - die Zuordnung in die Menues erfolgt in den
// nachfolgenden ID_MENU_xyz_Items Strukturen!
//----------------------
paramEditItem_t const paramEditItemTable[] PROGMEM =
{
  //-----------------------------------
  // Menue: ID_MENU_KANAELE  (Kanäle)
  //-----------------------------------
  { param_Kanalbelegung_Gas                 , &editGeneric, GENERIC_Cannel          , 0,  0, param_Kanalbelegung_Gas_de,                param_Kanalbelegung_Gas_en              },
  { param_Kanalbelegung_Gear                , &editGeneric, GENERIC_Cannel          , 0,  0, param_Kanalbelegung_Gear_de,               param_Kanalbelegung_Gear_en             },
  { param_Kanalbelegung_Nick                , &editGeneric, GENERIC_Cannel          , 0,  0, param_Kanalbelegung_Nick_de,               param_Kanalbelegung_Nick_en             },
  { param_Kanalbelegung_Roll                , &editGeneric, GENERIC_Cannel          , 0,  0, param_Kanalbelegung_Roll_de,               param_Kanalbelegung_Roll_en             },
  { param_Kanalbelegung_Poti1               , &editGeneric, GENERIC_AusChSerWpeMMM  , 0,  0, param_Kanalbelegung_Poti1_de,              param_Kanalbelegung_Poti1_en            },
  { param_Kanalbelegung_Poti2               , &editGeneric, GENERIC_AusChSerWpeMMM  , 0,  0, param_Kanalbelegung_Poti2_de,              param_Kanalbelegung_Poti2_en            },
  { param_Kanalbelegung_Poti3               , &editGeneric, GENERIC_AusChSerWpeMMM  , 0,  0, param_Kanalbelegung_Poti3_de,              param_Kanalbelegung_Poti3_en            },
  { param_Kanalbelegung_Poti4               , &editGeneric, GENERIC_AusChSerWpeMMM  , 0,  0, param_Kanalbelegung_Poti4_de,              param_Kanalbelegung_Poti4_en            },
  { param_Kanalbelegung_Poti5               , &editGeneric, GENERIC_AusChSerWpeMMM  , 0,  0, param_Kanalbelegung_Poti5_de,              param_Kanalbelegung_Poti5_en            },
  { param_Kanalbelegung_Poti6               , &editGeneric, GENERIC_AusChSerWpeMMM  , 0,  0, param_Kanalbelegung_Poti6_de,              param_Kanalbelegung_Poti6_en            },
  { param_Kanalbelegung_Poti7               , &editGeneric, GENERIC_AusChSerWpeMMM  , 0,  0, param_Kanalbelegung_Poti7_de,              param_Kanalbelegung_Poti7_en            },
  { param_Kanalbelegung_Poti8               , &editGeneric, GENERIC_AusChSerWpeMMM  , 0,  0, param_Kanalbelegung_Poti8_de,              param_Kanalbelegung_Poti8_en            },
  { param_MotorSafetySwitch                 , &editGeneric, GENERIC_DisChSerWpeOff  , 0,  0, param_MotorSafetySwitch_de,                param_MotorSafetySwitch_en              },
  { param_GlobalConfig3_MotorSwitchMode     , &editGeneric, GENERIC_NoYes           , 0,  0, param_GlobalConfig3_MotorSwitchMode_de,    param_GlobalConfig3_MotorSwitchMode_en  },
  { param_ExtraConfig_SensitiveRc           , &editGeneric, GENERIC_NoYes           , 0,  0, param_ExtraConfig_SensitiveRc_de,          param_ExtraConfig_SensitiveRc_en        },
  { param_GlobalConfig3_SpeakAll            , &editGeneric, GENERIC_NoYes           , 0,  0, param_GlobalConfig3_SpeakAll_de,           param_GlobalConfig3_SpeakAll_en         },


  //-----------------------------------
  // Menue: ID_MENU_KONFIGURATION  (Konfiguration)
  //-----------------------------------
  { param_GlobalConfig_HoehenRegelung       , &editGeneric, GENERIC_NoYes           , 0,  0, param_GlobalConfig_HoehenRegelung_de,      param_GlobalConfig_HoehenRegelung_en        },
  { param_GlobalConfig_GpsAktiv             , &editGeneric, GENERIC_NoYes           , 0,  0, param_GlobalConfig_GpsAktiv_de,            param_GlobalConfig_GpsAktiv_en              },
  { param_GlobalConfig_KompassAktiv         , &editGeneric, GENERIC_NoYes           , 0,  0, param_GlobalConfig_KompassAktiv_de,        param_GlobalConfig_KompassAktiv_en          },
  { param_GlobalConfig_KompassFix           , &editGeneric, GENERIC_NoYes           , 0,  0, param_GlobalConfig_KompassFix_de,          param_GlobalConfig_KompassFix_en            },
 // param_ExtraConfig_SensitiveRc
  { param_GlobalConfig_AchsenkopplungAktiv  , &editGeneric, GENERIC_NoYes           , 0,  0, param_GlobalConfig_AchsenkopplungAktiv_de, param_GlobalConfig_AchsenkopplungAktiv_en   },
  { param_GlobalConfig_DrehratenBegrenzer   , &editGeneric, GENERIC_NoYes           , 0,  0, param_GlobalConfig_DrehratenBegrenzer_de,  param_GlobalConfig_DrehratenBegrenzer_en    },
  { param_GlobalConfig_HeadingHold          , &editGeneric, GENERIC_NoYes           , 0,  0, param_GlobalConfig_HeadingHold_de,         param_GlobalConfig_HeadingHold_en           },


  //-----------------------------------
  // Menue: ID_MENU_STICK  (Stick)
  //-----------------------------------
  { param_Stick_P                           , &editGeneric, GENERIC_Value           , 0,  20, param_Stick_P_de,                 param_Stick_P_en                },
  { param_Stick_D                           , &editGeneric, GENERIC_Value           , 0,  20, param_Stick_D_de,                 param_Stick_D_en                },
  { param_StickGier_P                       , &editGeneric, GENERIC_ValuePoti       , 0, 247, param_Gyro_Gier_P_de,             param_Gyro_Gier_P_en            },
  { param_ExternalControl                   , &editGeneric, GENERIC_ValuePoti       , 0, 247, param_ExternalControl_de,         param_ExternalControl_en        },


  //-----------------------------------
  // Menue: ID_MENU_LOOPING (Looping)
  //-----------------------------------
  { param_BitConfig_LoopOben                , &editGeneric, GENERIC_NoYes           , 0,   0, param_BitConfig_LoopOben_de,      param_BitConfig_LoopOben_en     },
  { param_BitConfig_LoopUnten               , &editGeneric, GENERIC_NoYes           , 0,   0, param_BitConfig_LoopUnten_de,     param_BitConfig_LoopUnten_en    },
  { param_BitConfig_LoopLinks               , &editGeneric, GENERIC_NoYes           , 0,   0, param_BitConfig_LoopLinks_de,     param_BitConfig_LoopLinks_en    },
  { param_BitConfig_LoopRechts              , &editGeneric, GENERIC_NoYes           , 0,   0, param_BitConfig_LoopRechts_de,    param_BitConfig_LoopRechts_en   },
  { param_LoopGasLimit                      , &editGeneric, GENERIC_ValuePoti       , 0, 247, param_LoopGasLimit_de,            param_LoopGasLimit_en           },
  { param_LoopThreshold                     , &editGeneric, GENERIC_Value           , 0, 247, param_LoopThreshold_de,           param_LoopThreshold_en          },
  { param_LoopHysterese                     , &editGeneric, GENERIC_Value           , 0, 247, param_LoopHysterese_de,           param_LoopHysterese_en          },
  { param_WinkelUmschlagNick                , &editGeneric, GENERIC_Value           , 0, 247, param_WinkelUmschlagNick_de,      param_WinkelUmschlagNick_en     },
  { param_WinkelUmschlagRoll                , &editGeneric, GENERIC_Value           , 0, 247, param_WinkelUmschlagRoll_de,      param_WinkelUmschlagRoll_en     },


  //-----------------------------------
  // Menue: ID_MENU_HOEHE (Höhe)
  //-----------------------------------
  { param_GlobalConfig_HoehenRegelung       , &editGeneric, GENERIC_NoYes           , 0,   0, param_GlobalConfig_HoehenRegelung_de, param_GlobalConfig_HoehenRegelung_en},
  { param_ExtraConfig_HeightLimit           , &editGeneric, GENERIC_NoYes           , 0,   0, param_ExtraConfig_HeightLimit_de,     param_ExtraConfig_HeightLimit_en    },
  { param_ExtraConfig_HeightVario           , &editGeneric, GENERIC_NoYes           , 0,   0, param_ExtraConfig_HeightVario_de,     param_ExtraConfig_HeightVario_en    },  // negiertes param_ExtraConfig_HeightLimit
  { param_GlobalConfig_HoehenSchalter       , &editGeneric, GENERIC_NoYes           , 0,   0, param_GlobalConfig_HoehenSchalter_de, param_GlobalConfig_HoehenSchalter_en},
  { param_ExtraConfig_VarioBeep             , &editGeneric, GENERIC_NoYes           , 0,   0, param_ExtraConfig_VarioBeep_de,       param_ExtraConfig_VarioBeep_en      },
  { param_HoeheChannel                      , &editGeneric, GENERIC_DisChSerWpeOffOn, 0,   0, param_HoeheChannel_de,                param_HoeheChannel_en               },
  { param_Hoehe_MinGas                      , &editGeneric, GENERIC_Value           , 0, 247, param_Hoehe_MinGas_de,                param_Hoehe_MinGas_en               },
  { param_Hoehe_P                           , &editGeneric, GENERIC_ValuePoti       , 0, 247, param_Hoehe_P_de,                     param_Hoehe_P_en                    },
  { param_Luftdruck_D                       , &editGeneric, GENERIC_ValuePoti       , 0, 247, param_Luftdruck_D_de,                 param_Luftdruck_D_en                },
  { param_Hoehe_ACC_Wirkung                 , &editGeneric, GENERIC_ValuePoti       , 0, 247, param_Hoehe_ACC_Wirkung_de,           param_Hoehe_ACC_Wirkung_en          },
  { param_MaxAltitude                       , &editGeneric, GENERIC_ValuePoti       , 0, 247, param_MaxAltitude_de,                 param_MaxAltitude_en                },
  { param_Hoehe_Verstaerkung                , &editGeneric, GENERIC_Value           , 0, 247, param_Hoehe_Verstaerkung_de,          param_Hoehe_Verstaerkung_en         },
  { param_Hoehe_HoverBand                   , &editGeneric, GENERIC_Value           , 0, 247, param_Hoehe_HoverBand_de,             param_Hoehe_HoverBand_en            },
  { param_Hoehe_GPS_Z                       , &editGeneric, GENERIC_ValuePoti       , 0, 247, param_Hoehe_GPS_Z_de,                 param_Hoehe_GPS_Z_en                },
  { param_Hoehe_TiltCompensation            , &editGeneric, GENERIC_ValuePoti       , 0, 247, param_Hoehe_Tilt_Comp_de,             param_Hoehe_Tilt_Comp_en            },
  { param_Hoehe_StickNeutralPoint           , &editGeneric, GENERIC_Value           , 0, 160, param_Hoehe_StickNeutralPoint_de,     param_Hoehe_StickNeutralPoint_en    },
  { param_StartLandChannel                  , &editGeneric, GENERIC_DisChSer        , 0,   0, param_StartLandChannel_de,            param_StartLandChannel_en           },
  { param_LandingSpeed                      , &editGeneric, GENERIC_Value           , 0, 247, param_LandingSpeed_de,                param_LandingSpeed_en               },
  { param_LandingPulse                      , &editACCZLandingPulse, GENERIC_ValueACCZ               , 0,   0, param_LandingPulse_de,                param_LandingPulse_en               },


  //-----------------------------------
  // Menue: ID_MENU_KAMERA (Kamera)
  //-----------------------------------
  { param_ServoNickControl                  , &editGeneric, GENERIC_ValuePoti       , 0, 247, param_ServoNickControl_de,                param_ServoNickControl_en               },
  { param_ServoNickComp                     , &editGeneric, GENERIC_ValuePoti       , 0, 247, param_ServoNickComp_de,                   param_ServoNickComp_en                  },
  { param_ServoNickFailsave                 , &editGeneric, GENERIC_Value           , 0, 247, param_ServoNickFails_de,                  param_ServoNickFails_en                 },
  { param_GlobalConfig3_ServoNickCompOff    , &editGeneric, GENERIC_NoYes           , 0,   0, param_GlobalConfig3_ServoNickCompOff_de,  param_GlobalConfig3_ServoNickCompOff_en }, // TODO: pruefen: ab welcher Version ist das drin ???
  { param_ServoCompInvert_SERVO_NICK_INV    , &editGeneric, GENERIC_NoYes           , 0,   0, param_ServoCompInvert_SVNick_de,          param_ServoCompInvert_SVNick_en         },
  { param_ServoCompInvert_SERVO_RELATIVE    , &editGeneric, GENERIC_NoYes           , 0,   0, param_ServoCompInvert_SVRelMov_de,        param_ServoCompInvert_SVRelMov_en       },
  { param_ServoNickMin                      , &editGeneric, GENERIC_Value           , 0, 247, param_ServoNickMin_de,                    param_ServoNickMin_en                   },
  { param_ServoNickMax                      , &editGeneric, GENERIC_Value           , 0, 247, param_ServoNickMax_de,                    param_ServoNickMax_en                   },
  { param_ServoFilterNick                   , &editGeneric, GENERIC_Value           , 0,  25, param_ServoFilterNick_de,                 param_ServoFilterNick_en                },
  { param_ServoRollControl                  , &editGeneric, GENERIC_ValuePoti       , 0, 247, param_ServoRollControl_de,                param_ServoRollControl_en               },
  { param_ServoRollComp                     , &editGeneric, GENERIC_ValuePoti       , 0, 247, param_ServoRollComp_de,                   param_ServoRollComp_en                  },
  { param_ServoRollFailsave                 , &editGeneric, GENERIC_Value           , 0, 247, param_ServoRollFails_de,                  param_ServoRollFails_en                 },
  { param_ServoCompInvert_SERVO_ROLL_INV    , &editGeneric, GENERIC_NoYes           , 0,   0, param_ServoCompInvert_SVRoll_de,          param_ServoCompInvert_SVRoll_en         },
  { param_ServoRollMin                      , &editGeneric, GENERIC_Value           , 0, 247, param_ServoRollMin_de,                    param_ServoRollMin_en                   },
  { param_ServoRollMax                      , &editGeneric, GENERIC_Value           , 0, 247, param_ServoRollMax_de,                    param_ServoRollMax_en                   },
  { param_ServoFilterRoll                   , &editGeneric, GENERIC_Value           , 0,  25, param_ServoFilterRoll_de,                 param_ServoFilterRoll_en                },
  { param_ServoNickRefresh                  , &editGeneric, GENERIC_Value           , 2,   8, param_ServoNickRefresh_de,                param_ServoNickRefresh_en               },
  { param_ServoManualControlSpeed           , &editGeneric, GENERIC_Value           , 0, 247, param_ServoManualControlSpeed_de,         param_ServoManualControlSpeed_en        },
  { param_Servo3                            , &editGeneric, GENERIC_ValueOutPoti    , 0, 245, param_Servo3_de,                          param_Servo3_en                         },
  { param_Servo3Failsave                    , &editGeneric, GENERIC_Value           , 0, 247, param_Servo3Fails_de,                     param_Servo3Fails_en                    },
  { param_Servo4                            , &editGeneric, GENERIC_ValueOutPoti    , 0, 245, param_Servo4_de,                          param_Servo4_en                         },
  { param_Servo4Failsave                    , &editGeneric, GENERIC_Value           , 0, 247, param_Servo4Fails_de,                     param_Servo4Fails_en                    },
  { param_Servo5                            , &editGeneric, GENERIC_ValuePoti       , 0, 247, param_Servo5_de,                          param_Servo5_en                         },
  { param_Servo5Failsave                    , &editGeneric, GENERIC_Value           , 0, 247, param_Servo5Fails_de,                     param_Servo5Fails_en                    },

  { param_Servo3OnValue                     , &editGeneric, GENERIC_Value           , 0, 247, param_Servo3OnValue_de,                   param_Servo3OnValue_en                  },
  { param_Servo3OffValue                    , &editGeneric, GENERIC_Value           , 0, 247, param_Servo3OffValue_de,                  param_Servo3OffValue_en                 },
  { param_Servo4OnValue                     , &editGeneric, GENERIC_Value           , 0, 247, param_Servo4OnValue_de,                   param_Servo4OnValue_en                  },
  { param_Servo4OffValue                    , &editGeneric, GENERIC_Value           , 0, 247, param_Servo4OffValue_de,                  param_Servo4OffValue_en                 },
  //{ param_CamOrientation                  , &editNA     , 0                       , 0,   0, param_CamOrientation_de,                  param_CamOrientation_en }, // wird evtl. nicht mehr unterstuetzt

  //-----------------------------------
  // Menue: ID_MENU_CAMCTRL (Kamera Steuerung)
  //-----------------------------------
  { param_CamCtrlModeChannel                , &editGeneric, GENERIC_InaChSerCh      , 0,   0, param_CamCtrlModeChannel_de,              param_CamCtrlModeChannel_en             }, //ab Rev.107
  { param_CamCtrlZoomChannel                , &editGeneric, GENERIC_InaChSerCh      , 0,   0, param_CamCtrlZoomChannel_de,              param_CamCtrlZoomChannel_en             }, //ab Rev.107
  { param_GimbalYawChannel					, &editGeneric, GENERIC_InaChSerCh      , 0,   0, param_GimbalYawChannel_de,			    param_GimbalYawChannel_en				}, //ab Rev.112 FC219 Beta
  { param_GimbalOut1Channel				    , &editGeneric, GENERIC_InaChSerCh		, 0,   0, param_GimbalOut1Channel_de,				param_GimbalOut1Channel_en				}, //ab Rev. FC2.20
  { param_GimbalOut2Channel				    , &editGeneric, GENERIC_InaChSerCh		, 0,   0, param_GimbalOut2Channel_de,				param_GimbalOut2Channel_en				}, //ab Rev. FC2.20

  //-----------------------------------
  // Menue: ID_MENU_NAVICTRL (NaviCtrl)
  //-----------------------------------
  { param_GlobalConfig_GpsAktiv             , &editGeneric, GENERIC_NoYes               , 0,   0, param_GlobalConfig_GpsAktiv_de,       param_GlobalConfig_GpsAktiv_en      },
  { param_NaviGpsModeChannel                , &editGeneric, GENERIC_DisChSerWpeFreeCHPH , 0,   0, param_NaviGpsModeChannel_de,          param_NaviGpsModeChannel_en         },
  { param_NaviGpsGain                       , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_NaviGpsGain_de,                 param_NaviGpsGain_en                },
  { param_NaviStickThreshold                , &editGeneric, GENERIC_Value               , 0, 247, param_NaviStickThreshold_de,          param_NaviStickThreshold_en         },
  { param_NaviGpsMinSat                     , &editGeneric, GENERIC_Value               , 0, 247, param_NaviGpsMinSat_de,               param_NaviGpsMinSat_en              },
  { param_NaviGpsP                          , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_NaviGpsP_de,                    param_NaviGpsP_en                   },
  { param_NaviGpsI                          , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_NaviGpsI_de,                    param_NaviGpsI_en                   },
  { param_NaviGpsD                          , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_NaviGpsD_de,                    param_NaviGpsD_en                   },
  { param_NaviGpsPLimit                     , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_NaviGpsPLimit_de,               param_NaviGpsPLimit_en              },
  { param_NaviGpsILimit                     , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_NaviGpsILimit_de,               param_NaviGpsILimit_en              },
  { param_NaviGpsDLimit                     , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_NaviGpsDLimit_de,               param_NaviGpsDLimit_en              },
  { param_NaviGpsA                          , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_NaviGpsA_de,                    param_NaviGpsA_en                   },
  { param_NaviWindCorrection                , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_NaviWindCorrection_de,          param_NaviWindCorrection_en         },
  { param_NaviAccCompensation               , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_NaviAccCompensation_de,         param_NaviAccCompensation_en        },
  { param_NaviMaxFlyingRange                , &editGeneric, GENERIC_Value               , 0, 250, param_NaviMaxFlyingRange_de,          param_NaviMaxFlyingRange_en         },
  { param_NaviOperatingRadius               , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_NaviOperatingRadius_de,         param_NaviOperatingRadius_en        },
  { param_NaviAngleLimitation               , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_NaviAngleLimitation_de,         param_NaviAngleLimitation_en        },
  { param_NaviPH_LoginTime                  , &editGeneric, GENERIC_Value               , 0, 247, param_NaviPH_LoginTime_de,            param_NaviPH_LoginTime_en           },
  { param_ExtraConfig_GpsAid                , &editGeneric, GENERIC_NoYes               , 0,   0, param_ExtraConfig_GpsAid_de,          param_ExtraConfig_GpsAid_en         },
  { param_GlobalConfig3_DphMaxRadius        , &editGeneric, GENERIC_NoYes               , 0,   0, param_GlobalConfig3_DphMaxRadius_de,  param_GlobalConfig3_DphMaxRadius_en },
  { param_GlobalConfig3_RiseFirstWP         , &editGeneric, GENERIC_NoYes               , 0,   0, param_GlobalConfig3_RiseFirstWP_de,   param_GlobalConfig3_RiseFirstWP_en  },
  { param_ComingHomeAltitude                , &editGeneric, GENERIC_Value               , 0, 247, param_ComingHomeAltitude_de,          param_ComingHomeAltitude_en         },
  { param_ComingHomeOrientation             , &editGeneric, GENERIC_CH_Orientation      , 0,   3, param_ComingHomeOrientation_de,       param_ComingHomeOrientation_en      },
  { param_SingleWpControlChannel            , &editGeneric, GENERIC_InaChSerWpeMMM      , 0,   0, param_SingleWpControlChannel_de,      param_SingleWpControlChannel_en     }, //ab Rev.106
  { param_MenuKeyChannel                    , &editGeneric, GENERIC_InaChSerWpeMMM      , 0,   0, param_MenuKeyChannel_de,              param_MenuKeyChannel_en             },
  { param_SingleWpSpeed                     , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_SingleWpSpeed_de,               param_SingleWpSpeed_en              }, // ab Rev. 100
  { param_NaviDescendRange                  , &editGeneric, GENERIC_Value               , 0, 250, param_NaviDescendRange_de,            param_NaviDescendRange_en           },


  //-----------------------------------
  // Menue: ID_MENU_AUSGAENGE (Ausgänge)
  //-----------------------------------
  { param_J16Bitmask                        , &editBitmask, 0                           , 0,   0, param_J16Bitmask_de,                  param_J16Bitmask_en                 },
  { param_J16Timing                         , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_J16Timing_de,                   param_J16Timing_en                  },
  { param_BitConfig_Led1_MotorRun             , &editGeneric, GENERIC_NoYes               , 0,   0, param_BitConfig_Led1_MotorRun_de,       param_BitConfig_Led1_MotorRun_en      }, // "nur nach Start der Motoren aktiv"
  { param_BitConfig_MotorOffLed1            , &editGeneric, GENERIC_NoYes               , 0,   0, param_BitConfig_MotorOffLed1_de,      param_BitConfig_MotorOffLed1_en     }, // nur wenn "nur nach Start der Motoren aktiv" = JA ist -> bestimmt ob LED's an oder aus sind wenn die Motoren aus sind
  { param_GlobalConfig3_UseNcForOut1        , &editGeneric, GENERIC_NoYes               , 0,   0, param_GlobalConfig3_UseNcForOut1_de,  param_GlobalConfig3_UseNcForOut1_en }, // "mit WP-Event verknï¿½pfen"
  { param_NaviOut1Parameter                 , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_NaviOut1Parameter_de,           param_NaviOut1Parameter_en          }, // "AutoTrigger alle...[meter]"           (bis Rev.98)
  { param_AutoPhotoDistance                 , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_AutoPhotoDistance_de,           param_AutoPhotoDistance_en          }, // "AutoTrigger alle [meter] in Distance" (ab Rev.100)
  { param_AutoPhotoAtitudes                 , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_AutoPhotoAtitudes_de,           param_AutoPhotoAtitudes_en          }, // "AutoTrigger alle [meter] in Altitude" (ab Rev.100)

  { param_J17Bitmask                        , &editBitmask, 0                           , 0,   0, param_J17Bitmask_de,                  param_J17Bitmask_en                 },
  { param_J17Timing                         , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_J17Timing_de,                   param_J17Timing_en                  },
  { param_BitConfig_Led2_MotorRun             , &editGeneric, GENERIC_NoYes               , 0,   0, param_BitConfig_Led2_MotorRun_de,       param_BitConfig_Led2_MotorRun_en      }, // "nur nach Start der Motoren aktiv"
  { param_BitConfig_MotorOffLed2            , &editGeneric, GENERIC_NoYes               , 0,   0, param_BitConfig_MotorOffLed2_de,      param_BitConfig_MotorOffLed2_en     }, // nur wenn "nur nach Start der Motoren aktiv" = JA ist -> bestimmt ob LED's an oder aus sind wenn die Motoren aus sind

  { param_WARN_J16_Bitmask                  , &editBitmask, 0                           , 0,   0, param_WARN_J16_Bitmask_de,            param_WARN_J16_Bitmask_en           }, // Bitmaske fuer Unterspannungswarnung
  { param_WARN_J17_Bitmask                  , &editBitmask, 0                           , 0,   0, param_WARN_J17_Bitmask_de,            param_WARN_J17_Bitmask_en           }, // Bitmaske fuer Unterspannungswarnung


  //-----------------------------------
  // Menue: ID_MENU_VERSCHIEDENES (Verschiedenes)
  //-----------------------------------
  { param_Gas_Min                           , &editGeneric, GENERIC_Value               , 0, 247, param_Gas_Min_de,                             param_Gas_Min_en                            },
  { param_Gas_Max                           , &editGeneric, GENERIC_Value               , 0, 247, param_Gas_Max_de,                             param_Gas_Max_en                            },
  { param_KompassWirkung                    , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_KompassWirkung_de,                      param_KompassWirkung_en                     },
//{ param_CompassOffset                     , &editGeneric, GENERIC_Value               , 0, 255, param_CompassOffset_de,                       param_CompassOffset_en                      }, // TODO: +/- Werte sind dort kodiert
  { param_CompassOffset                     , &editCompassOffset,   0                   , 0,   0, param_CompassOffset_de,                       param_CompassOffset_en                      }, // TODO: +/- Werte sind dort kodiert
  { param_CompassOffset_DisableDeclCalc     , &editGeneric, GENERIC_NoYes               , 0,   0, param_CompassOffset_DisableDeclCalc_de,       param_CompassOffset_DisableDeclCalc_en      }, // TODO: +/- Werte sind dort kodiert
  { param_CareFreeChannel                   , &editGeneric, GENERIC_DisChSerWpeOffOn    , 0,   0, param_CareFreeChannel_de,                     param_CareFreeChannel_en                    },
  { param_ExtraConfig_LearnableCarefree     , &editGeneric, GENERIC_NoYes               , 0,   0, param_ExtraConfig_LearnableCarefree_de,       param_ExtraConfig_LearnableCarefree_en      },

  { param_UnterspannungsWarnung             , &editGeneric, GENERIC_Value               , 0, 247, param_UnterspannungsWarnung_de,               param_UnterspannungsWarnung_en              },
  { param_ComingHomeVoltage                 , &editGeneric, GENERIC_Value               , 0, 247, param_ComingHomeVoltage_de,                   param_ComingHomeVoltage_en                  },
  { param_AutoLandingVoltage                , &editGeneric, GENERIC_Value               , 0, 247, param_AutoLandingVoltage_de,                  param_AutoLandingVoltage_en                 },

//{ param_ExtraConfig_33vReference          , &editGeneric, GENERIC_NoYes               , 0,   0, param_ExtraConfig_33vReference_de,            param_ExtraConfig_33vReference_en           }, // nicht mehr unterstuetzt!
  { param_NotGasZeit                        , &editGeneric, GENERIC_Value               , 0, 247, param_NotGasZeit_de,                          param_NotGasZeit_en                         },
  { param_GlobalConfig3_VarioFailsafe       , &editGeneric, GENERIC_NoYes               , 0,   0, param_GlobalConfig3_VarioFailsafe_de,         param_GlobalConfig3_VarioFailsafe_en        },
  { param_NotGas                            , &editGeneric, GENERIC_Value               , 0, 247, param_NotGas_de,                              param_NotGas_en                             },
  { param_FailSafeTime                      , &editGeneric, GENERIC_Value               , 0, 247, param_FailSafeTime_de,                        param_FailSafeTime_en                       },
  { param_FailsafeChannel                   , &editGeneric, GENERIC_Value               , 0,  16, param_FailsafeChannel_de,                     param_FailsafeChannel_en                    },
  { param_FailsafeAltitude 					, &editGeneric, GENERIC_Value               , 0, 247, param_FailsafeAltitude_de,					param_FailsafeAltitude_en					},
  { param_ExtraConfig_NoRcOffBeeping        , &editGeneric, GENERIC_NoYes               , 0,   0, param_ExtraConfig_NoRcOffBeeping_de,          param_ExtraConfig_NoRcOffBeeping_en         },
  { param_ExtraConfig_IgnoreMagErrAtStartup , &editGeneric, GENERIC_NoYes               , 0,   0, param_ExtraConfig_IgnoreMagErrAtStartup_de,   param_ExtraConfig_IgnoreMagErrAtStartup_en  },
  { param_GlobalConfig3_NoSdCardNoStart     , &editGeneric, GENERIC_NoYes               , 0,   0, param_GlobalConfig3_NoSdCardNoStart_de,       param_GlobalConfig3_NoSdCardNoStart_en      },
  { param_GlobalConfig3_NoGpsFixNoStart     , &editGeneric, GENERIC_NoYes               , 0,   0, param_GlobalConfig3_NoGpsFixNoStart_de,       param_GlobalConfig3_NoGpsFixNoStart_en      },
  { param_ExtraConfig_FlyZone_Required      , &editGeneric, GENERIC_NoYes               , 0,   0, param_ExtraConfig_FlyZoneRequired_de,         param_ExtraConfig_FlyZoneRequired_en       },
  { param_GlobalConfig_LandOutsideFlyzone   , &editGeneric, GENERIC_NoYes               , 0,   0, param_GlobalConfig_LandOutsideFlyzone_de,     param_GlobalConfig_LandOutsideFlyzone_en   },

  //-----------------------------------
  // Menue: ID_MENU_GYRO (Gyro)
  //-----------------------------------
  { param_Gyro_P                            , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_Gyro_P_de,                              param_Gyro_P_en                             },
  { param_Gyro_Gier_P                       , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_Gyro_Gier_P_de,                         param_Gyro_Gier_P_en                        },
  { param_Gyro_I                            , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_Gyro_I_de,                              param_Gyro_I_en                             },
  { param_Gyro_Gier_I                       , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_Gyro_Gier_I_de,                         param_Gyro_Gier_I_en                        },
  { param_Gyro_D                            , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_Gyro_D_de,                              param_Gyro_D_en                             },

  { param_DynamicStability                  , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_DynamicStability_de,                    param_DynamicStability_en                   },
  { param_GlobalConfig_DrehratenBegrenzer   , &editGeneric, GENERIC_NoYes               , 0,   0, param_GlobalConfig_DrehratenBegrenzer_de,     param_GlobalConfig_DrehratenBegrenzer_en    },

  { param_GyroAccFaktor                     , &editGeneric, GENERIC_Value               , 0, 247, param_GyroAccFaktor_de,                       param_GyroAccFaktor_en                      },
  { param_GyroAccAbgleich                   , &editGeneric, GENERIC_Value               , 0, 247, param_GyroAccAbgleich_de,                     param_GyroAccAbgleich_en                    },

  { param_I_Faktor                          , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_I_Faktor_de,                            param_I_Faktor_en                           },
  { param_Driftkomp                         , &editGeneric, GENERIC_Value               , 0, 247, param_Driftkomp_de,                           param_Driftkomp_en                          },
  { param_Gyro_Stability                    , &editGeneric, GENERIC_Value               , 0, 247, param_Gyro_Stability_de,                      param_Gyro_Stability_en                     },
  { param_MotorSmooth                       , &editGeneric, GENERIC_Value               , 0, 247, param_MotorSmooth_de,                         param_MotorSmooth_en                        },


  //-----------------------------------
  // Menue: ID_MENU_BENUTZER (Benutzer)
  //-----------------------------------
  { param_UserParam1                        , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_UserParam1_de,                          param_UserParam1_en                         },
  { param_UserParam2                        , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_UserParam2_de,                          param_UserParam2_en                         },
  { param_UserParam3                        , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_UserParam3_de,                          param_UserParam3_en                         },
  { param_UserParam4                        , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_UserParam4_de,                          param_UserParam4_en                         },
  { param_UserParam5                        , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_UserParam5_de,                          param_UserParam5_en                         },
  { param_UserParam6                        , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_UserParam6_de,                          param_UserParam6_en                         },
  { param_UserParam7                        , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_UserParam7_de,                          param_UserParam7_en                         },
  { param_UserParam8                        , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_UserParam8_de,                          param_UserParam8_en                         },


  //-----------------------------------
  // Menue: ID_MENU_ACHSKOPPLUNG (Achskopplung)
  //-----------------------------------
  { param_GlobalConfig_AchsenkopplungAktiv  , &editGeneric, GENERIC_NoYes               , 0,   0, param_GlobalConfig_AchsenkopplungAktiv_de,    param_GlobalConfig_AchsenkopplungAktiv_en       },
  { param_AchsKopplung1                     , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_AchsKopplung1_de,                       param_AchsKopplung1_en                          },
  { param_AchsKopplung2                     , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_AchsKopplung2_de,                       param_AchsKopplung2_en                          },
  { param_CouplingYawCorrection             , &editGeneric, GENERIC_ValuePoti           , 0, 247, param_CouplingYawCorrection_de,               param_CouplingYawCorrection_en                  },


  //-----------------------------------
  // ENDE
  //-----------------------------------
  { param_EOF, NOFUNC, 0,0,0,  0,0 }     // END OF LIST - MUST BE THE LAST!!
};


/************************************************************
NICHT MEHR UNTERSTUEZT:
param_Receiver   (Empfänger Typ: Hott, Jeti, Spektrum,...)
************************************************************/





//#############################################################################################
//# Menue's und Menuezuordnung
//#############################################################################################

//-----------------------------------
// Menue ID's: Parameters Hauptmenue
//-----------------------------------


#define ID_MENU_FAVORITEN       1   // fuer spaeter reserviert....
#define ID_MENU_KANAELE         2
#define ID_MENU_KONFIGURATION   3
#define ID_MENU_STICK           4
#define ID_MENU_LOOPING         5
#define ID_MENU_HOEHE           6
#define ID_MENU_KAMERA          7
#define ID_MENU_CAMCTRL         8
#define ID_MENU_NAVICTRL        9
#define ID_MENU_AUSGAENGE       10
#define ID_MENU_VERSCHIEDENES   11
#define ID_MENU_GYRO            12
#define ID_MENU_BENUTZER        13
#define ID_MENU_ACHSKOPPLUNG    14
#define ID_MENU_MIXERSETUP      15  // nicht verwendet / unterstuetzt
#define ID_MENU_EASYSETUP       16


#define ID_MENU_TEST            66  // TEST / DEBUG


//-----------------------------------
// Zuordnungen von paramID's zu den jeweiligen Menue's
//
// Eintrag "SEPARATOR" - damit wird eine Trennlinie
// im Menue dargestellt
//-----------------------------------

//-------------------------------
// Menue: ID_MENU_KANAELE
//-------------------------------
unsigned char const ID_MENU_KANAELE_Items[] =
{
    param_Kanalbelegung_Gas,
    param_Kanalbelegung_Gear,
    param_Kanalbelegung_Nick,
    param_Kanalbelegung_Roll,
    param_Kanalbelegung_Poti1,
    param_Kanalbelegung_Poti2,
    param_Kanalbelegung_Poti3,
    param_Kanalbelegung_Poti4,
    param_Kanalbelegung_Poti5,
    param_Kanalbelegung_Poti6,
    param_Kanalbelegung_Poti7,
    param_Kanalbelegung_Poti8,
       SEPARATOR,
    param_MotorSafetySwitch,
    param_GlobalConfig3_MotorSwitchMode,
       SEPARATOR,
    param_ExtraConfig_SensitiveRc,
    param_GlobalConfig3_SpeakAll,


    EOF // *** MUST BE THE LAST! ***
};


//-------------------------------
// Menue: ID_MENU_KONFIGURATION
//-------------------------------
unsigned char const ID_MENU_KONFIGURATION_Items[] =
{
    param_GlobalConfig_HoehenRegelung,
    param_GlobalConfig_GpsAktiv,
    param_GlobalConfig_KompassAktiv,
    param_GlobalConfig_KompassFix,
    param_ExtraConfig_SensitiveRc,
    param_GlobalConfig_AchsenkopplungAktiv,
    param_GlobalConfig_DrehratenBegrenzer,
    param_GlobalConfig_HeadingHold,

    EOF // *** MUST BE THE LAST! ***
};


//-------------------------------
// Menue: ID_MENU_STICK
//-------------------------------
unsigned char const ID_MENU_STICK_Items[] =
{
    param_Stick_P,
    param_Stick_D,
    param_StickGier_P,
       SEPARATOR,
    param_ExternalControl,

    EOF // *** MUST BE THE LAST! ***
};


//-------------------------------
// Menue: ID_MENU_LOOPING
//-------------------------------
unsigned char const ID_MENU_LOOPING_Items[] =
{
    param_BitConfig_LoopOben,
    param_BitConfig_LoopUnten,
    param_BitConfig_LoopLinks,
    param_BitConfig_LoopRechts,
       SEPARATOR,
    param_LoopGasLimit,
    param_LoopThreshold,
    param_LoopHysterese,
    param_WinkelUmschlagNick,
    param_WinkelUmschlagRoll,

    EOF // *** MUST BE THE LAST! ***
};


//-------------------------------
// Menue: ID_MENU_HOEHE
//-------------------------------
unsigned char const ID_MENU_HOEHE_Items[] =
{
    param_GlobalConfig_HoehenRegelung,
    param_ExtraConfig_HeightLimit,
    param_ExtraConfig_HeightVario,
    param_GlobalConfig_HoehenSchalter,
    param_ExtraConfig_VarioBeep,
       SEPARATOR,
    param_HoeheChannel,
    param_Hoehe_Verstaerkung,
    param_Hoehe_MinGas,
    param_Hoehe_HoverBand,
    param_Hoehe_P,
    param_Hoehe_GPS_Z,
    param_Hoehe_TiltCompensation,
    param_Luftdruck_D,
    param_Hoehe_StickNeutralPoint,
    param_Hoehe_ACC_Wirkung,
    param_MaxAltitude,
       SEPARATOR,
    param_StartLandChannel,
    param_LandingSpeed,
    param_LandingPulse,
    EOF // *** MUST BE THE LAST! ***
};



//-------------------------------
// Menue: ID_MENU_KAMERA
//-------------------------------
unsigned char const ID_MENU_KAMERA_Items[] =
{
    param_ServoNickControl,
    param_ServoNickComp,
    param_ServoNickFailsave,
    param_GlobalConfig3_ServoNickCompOff,       // erst ab FC-Rev 96
    param_ServoCompInvert_SERVO_NICK_INV,
    param_ServoCompInvert_SERVO_RELATIVE,
    param_ServoNickMin,
    param_ServoNickMax,
    param_ServoFilterNick,
       SEPARATOR,
    param_ServoRollControl,
    param_ServoRollComp,
    param_ServoRollFailsave,
    param_ServoCompInvert_SERVO_ROLL_INV,
    param_ServoRollMin,
    param_ServoRollMax,
    param_ServoFilterRoll,
       SEPARATOR,
    param_ServoNickRefresh,
    param_ServoManualControlSpeed,
    param_Servo3,
    param_Servo3Failsave,
    param_Servo4,
    param_Servo4Failsave,
    param_Servo5,
    param_Servo5Failsave,
       SEPARATOR,
    param_Servo3OnValue,
    param_Servo3OffValue,
    param_Servo4OnValue,
    param_Servo4OffValue,

    EOF // *** MUST BE THE LAST! ***
};


//-------------------------------
// Menue: ID_MENU_CAMCTRL
//-------------------------------
unsigned char const ID_MENU_CAMCTRL_Items[] =
{

    param_CamCtrlModeChannel,
    param_CamCtrlZoomChannel,
	param_GimbalYawChannel,
	param_GimbalOut1Channel,
	param_GimbalOut2Channel,
    EOF // *** MUST BE THE LAST! ***
};



//-------------------------------
// Menue: ID_MENU_NAVICTRL
//-------------------------------
unsigned char const ID_MENU_NAVICTRL_Items[] =
{
    param_GlobalConfig_GpsAktiv,
    param_NaviGpsModeChannel,
    param_NaviGpsGain,
    param_NaviStickThreshold,
    param_NaviGpsMinSat,
    param_NaviGpsP,
    param_NaviGpsPLimit,
    param_NaviGpsI,
    param_NaviGpsILimit,
    param_NaviGpsD,
    param_NaviGpsDLimit,
    param_NaviGpsA,
       SEPARATOR,
    param_NaviWindCorrection,
    param_NaviAccCompensation,
    param_NaviMaxFlyingRange,
    param_NaviOperatingRadius,
    param_NaviAngleLimitation,
    param_NaviPH_LoginTime,
    param_ExtraConfig_GpsAid,
    param_GlobalConfig3_DphMaxRadius,
	   SEPARATOR,
    param_ComingHomeAltitude,
    param_ComingHomeOrientation,
    SEPARATOR,
	param_GlobalConfig3_RiseFirstWP,
    SEPARATOR,
    param_SingleWpControlChannel,
    param_MenuKeyChannel,
    param_SingleWpSpeed,
       SEPARATOR,
    param_NaviDescendRange,

    EOF // *** MUST BE THE LAST! ***
};


//-------------------------------
// Menue: ID_MENU_AUSGAENGE
//-------------------------------
unsigned char const ID_MENU_AUSGAENGE_Items[] =
{
    param_J16Bitmask,                   // LED 1 Bitmaske
    param_J16Timing,
    param_BitConfig_Led1_MotorRun,
    param_BitConfig_MotorOffLed1,
    param_GlobalConfig3_UseNcForOut1,
    param_NaviOut1Parameter,
    param_AutoPhotoDistance,
    param_AutoPhotoAtitudes,
       SEPARATOR,
    param_J17Bitmask,                   // LED 2 Bitmaske
    param_J17Timing,
    param_BitConfig_Led2_MotorRun,
    param_BitConfig_MotorOffLed2,
       SEPARATOR,
    param_WARN_J16_Bitmask,
    param_WARN_J17_Bitmask,

    EOF // *** MUST BE THE LAST! ***
};


//-------------------------------
// Menue: ID_MENU_VERSCHIEDENES
//-------------------------------
unsigned char const ID_MENU_VERSCHIEDENES_Items[] =
{
    param_Gas_Min,
    param_Gas_Max,
    param_KompassWirkung,
    param_CompassOffset,
    param_CompassOffset_DisableDeclCalc,
    param_CareFreeChannel,
    param_ExtraConfig_LearnableCarefree,
       SEPARATOR,
    param_UnterspannungsWarnung,
    param_ComingHomeVoltage,
    param_AutoLandingVoltage,
       SEPARATOR,
    param_NotGasZeit,
    param_GlobalConfig3_VarioFailsafe,          // Nutze "Vario-Höhe" für "Not-Gas" -> beeinflusst 'param_NotGas' bzgl. "%"
    param_NotGas,
    param_FailSafeTime,
    param_FailsafeChannel,
	param_FailsafeAltitude,
    param_ExtraConfig_NoRcOffBeeping,
       SEPARATOR,
    param_ExtraConfig_IgnoreMagErrAtStartup,
    param_GlobalConfig3_NoSdCardNoStart,
    param_GlobalConfig3_NoGpsFixNoStart,
	param_ExtraConfig_FlyZone_Required,
	param_GlobalConfig_LandOutsideFlyzone,

    EOF // *** MUST BE THE LAST! ***
};


//-------------------------------
// Menue: ID_MENU_GYRO
//-------------------------------
unsigned char const ID_MENU_GYRO_Items[] =
{
    param_Gyro_P,
    param_Gyro_I,
    param_Gyro_D,
    param_Gyro_Gier_P,
    param_Gyro_Gier_I,
       SEPARATOR,
    param_DynamicStability,
    param_GlobalConfig_DrehratenBegrenzer,
       SEPARATOR,
    param_GyroAccFaktor,
    param_GyroAccAbgleich,
       SEPARATOR,
    param_I_Faktor,
    param_Driftkomp,
    param_Gyro_Stability,
    param_MotorSmooth,

    EOF // *** MUST BE THE LAST! ***
};


//-------------------------------
// Menue: ID_MENU_BENUTZER
//-------------------------------
unsigned char const ID_MENU_BENUTZER_Items[] =
{
    param_UserParam1,
    param_UserParam2,
    param_UserParam3,
    param_UserParam4,
    param_UserParam5,
    param_UserParam6,
    param_UserParam7,
    param_UserParam8,

    EOF // *** MUST BE THE LAST! ***
};


//-------------------------------
// Menue: ID_MENU_ACHSKOPPLUNG
//-------------------------------
unsigned char const ID_MENU_ACHSKOPPLUNG_Items[] =
{
    param_GlobalConfig_AchsenkopplungAktiv,
    param_AchsKopplung1,
    param_AchsKopplung2,
    param_CouplingYawCorrection,

    EOF // *** MUST BE THE LAST! ***
};


//-------------------------------
// Menue: ID_MENU_EASYSETUP
//-------------------------------
unsigned char const ID_MENU_EASYSETUP_Items[] =
{
    param_GlobalConfig_HoehenRegelung,
    param_HoeheChannel,
    param_Hoehe_StickNeutralPoint,
    param_StartLandChannel,
       SEPARATOR,
    param_GlobalConfig_GpsAktiv,
    param_NaviGpsModeChannel,
    param_ExtraConfig_GpsAid,
    param_ComingHomeAltitude,
       SEPARATOR,
    param_CareFreeChannel,
    param_ExtraConfig_LearnableCarefree,

    EOF // *** MUST BE THE LAST! ***
};




//#############################################################################################
//# Error-Handling
//#############################################################################################
//# mittels des auf dem PKT angezeigten Errocodes (z.B. "E32") kann man hier schauen was bzw.
//# wo (in welcher Funktion) der Fehler aufgetreten ist.
//#
//# Im Menue wird der Fehlercode rechts bei den Values dargestellt (dabei ertönen immer wieder
//# Fehler-Beep's).
//# In der Vollbildanzeige wird zusaetzlich ein uebergebener Wert (value) angezeigt - im Code
//# kann man ermitteln was der Wert darstellt.
//#############################################################################################

#define E01_PARAMID_MENUEDITCATEGORY     1  // Menu_EditCategory() - FEHLER: paramEditItem (=paramID) nicht in Tabelle paramEditItemTable gefunden

#define E11_PARAMID_EDITGENERIC         11  // editGeneric() - FEHLER: paramEditItem (=paramID) nicht in Tabelle paramEditItemTable gefunden
#define E12_PARAMID_EDITBITMASK         12  // editBitmask() - FEHLER: paramEditItem (=paramID) nicht in Tabelle paramEditItemTable gefunden

#define E30_CODECHAR_NOT_FOUND          30  // find_genericCode() - FEHLER: Code-Zeichen nicht in Tabelle editGenericCode gefunden
#define E31_PARAMEDIT_ITEM_NOT_FOUND    31  // find_genericCodeByValue() - FEHLER: paramEditItem (=paramID) nicht in Tabelle paramEditItemTable gefunden
#define E32_CODECHAR_NOT_FOUND          32  // find_genericCodeByValue() - FEHLER: Code-Zeichen nicht in Tabelle editGenericCode gefunden
#define E33_VALUE_NOT_IN_FORMAT         33  // find_genericCodeByValue() - FEHLER: Ende des Format-Strings erreicht - fuer das gegebene value konnte kein passender Code im Format-String gefunden werden

#define E41_VALUE_MIN_MAX_RANGE         41  // strValueGeneric() - FEHLER: fuer die Range von 'v' (Value) wurde max < min angegeben bei Generic -> nicht unterstuetzt




//--------------------------------------------------------------
// _error( errorcode, value, showscreen)
//
// zeigt Fehlermeldungen an
//
// PARAMETER:
//   errorcode : siehe Error-defines
//   value     : zusaetzlicher Anzeigeparameter (nur in Screen-Anzeige)
//   showscreen: true = Fehlermeldung wird vollstaendig angezeigt.
//                      - er Screen wird dabei geloescht
//                      - Ende mit Taste 'Ende'
//                      - langer Fehler-Beep
//
//               false = es wird nur der Buffer mkparam_strValueBuffer
//                       mit dem Fehlercode befuellt
//                       - Anzeige im Menue ganz recht als "Enn"
//                       - kurzer Fehler-Beep der jedoch bei jeder
//                         Aktion im Menue zu hoeren ist
//                         => dann auf Anzeige "Enn" achten!
//--------------------------------------------------------------
void _error( uint8_t errorcode, uint8_t value, uint8_t showscreen)
{
    xsnprintf_P( mkparam_strValueBuffer, MKPARAM_STRBUFFER_LEN, PSTR("E%02u"), errorcode);      // in mkparam_strValueBuffer ausgeben fuer das Menue

    if( showscreen )
    {
        lcd_cls();

        lcdx_printp_center( 1, PSTR(" ERROR "), MINVERS, 0,-3);
        lcdx_printp_center( 2, PSTR("mkparameters.c"), MNORMAL, 0,0);

        lcd_printf_at_P( 0, 4, MNORMAL, PSTR("Code : E%02u"), errorcode );
        lcd_printf_at_P( 0, 5, MNORMAL, PSTR("Value: %3u"), value );

        lcd_printp_at(11, 7, strGet(ENDE), MNORMAL );

        set_beep( 500, 0x000f, BeepNormal);        // Beep Error

        while( !get_key_press(1 << KEY_ESC) )
        {
            //--------------------------------
            // Anmerkung: OG 25.03.2014
            //
            // Ohne diese Verzoegerung funkioniert die Update-Erkennung via PKT-UpdateTool
            // meist nicht - ist das PKT zu schnell?
            // Kann man evtl. das PKT-UpdateTool diesbezueglich mal ueberpruefen ob man
            // Timings/Timeout's anpassen kann?
            //
            // Hinweis dazu: PKT_CtrlHook() bzw. die dort enthaltene Update-Anforderungspruefung
            // ist in den letzten Wochen beschleunigt geworden da unnoetiger Code fuer nicht
            // benoetigte Bildschirmausgaben entfernt wurde - das ist grundsaetzlich fuer das PKT
            // besser.
            //
            // Um das nachzustellen:
            // Unten das Delay auskommentieren und einen Fehler in Menu_EditCategory() provozieren
            // indem "paramID = 0;" einkommentiert wird (ist dort beschrieben)
            // Dann den Fehlerscreen im PKT anzeigen lassen und ein Update via PKT-UpdateTool initiieren,
            //--------------------------------
            _delay_ms(150);

            PKT_CtrlHook();
        }
    }
    else
    {
        // kurzer Fehler-Beep - der kann wiederholt im Menue auftreten bei jedem Refresh des Menues
        set_beep( 160, 0x000f, BeepNormal);        // Beep Error
    }
}




//#############################################################################################
//# Edit-Funktionen
//#############################################################################################


//--------------------------------------------------------------
// ok = find_paramEditItem( paramID )
//
// sucht in paramEditItemTable (PROGMEM) eine paramID und
// kopiert den Eintrag in das RAM
//
// PARAMETER:
//   paramID
//
// RUECKGABE:
//   true  = paramID gefunden
//   false = nicht gefunden (Fehler)
//
// -- globale Modulvariablen --
// Ergebnisse in:
//
//  paramEditItem  : der gefundene Eintrag vom Typ paramEditItem_t (im RAM)
//  paramEditFormat: der Format-String paramEditItem.format im RAM
//--------------------------------------------------------------
unsigned char find_paramEditItem( unsigned char paramID )
{
    unsigned char   id;
    unsigned char   *p;

    p = (unsigned char *) paramEditItemTable;

    while( true )
    {
        id = pgm_read_byte(p);                      // die aktuelle paramID aus paramEditDef

        if( id == paramID || id == param_EOF )
            break;

        p += sizeof( paramEditItem_t );
    }

    //-----
    // wenn die gesuchte paramID nicht gefunden wurde steht
    // im RAM-Buffer paramEdit.paramID == param_EOF !!
    //-----
    memcpy_P ( &paramEditItem , p, sizeof(paramEditItem_t) );                   // die Struktur  -> in den RAM-Buffer kopieren (Zugriff: paramEditItem)
    strncpy_P( paramEditFormat, paramEditItem.format, MKPARAM_STRBUFFER_LEN);   // format String -> in den RAM-Buffer kopieren (Zugriff: paramEditFormat)

    return( id!=param_EOF );
}



//--------------------------------------------------------------
// ptrP = paramEditItemTitle()
//
// gibt einen Zeiger in PROGMEM auf den Titel/Bezeichnung eines
// paramID's in der richtigen Sprache zurueck
//
// ACHTUNG! paramEditItem muss vorher initialisert worden sein!
//--------------------------------------------------------------
/*
const char * paramEditItemTitle( void )
{
    if( Config.DisplayLanguage == 0 )
        return( paramEditItem.title_de );
    else
        return( paramEditItem.title_en );
}
*/



//--------------------------------------------------------------
// print_paramEditItemTitle()
//
// gibt Titel/Bezeichnung auf den Bildschirm bei x=0, y=2 aus
//
// ACHTUNG! paramEditItem muss vorher initialisert worden sein!
//--------------------------------------------------------------
void print_paramEditItemTitle( void )
{
    lcd_printp_at( 0, 2, (Config.DisplayLanguage == 0 ? paramEditItem.title_de : paramEditItem.title_en ), MNORMAL);    // Bezeichnung des paramID's
}



//--------------------------------------------------------------
// ok = find_genericCode( code )
//
// Ergebnis steht in: (globale Modulvariablen)
//   genericCode
//--------------------------------------------------------------
unsigned char find_genericCode( unsigned char code )
{
    unsigned char   codeIdx;

    codeIdx = 0;
    while( (editGenericCode[codeIdx].code != code ) && (editGenericCode[codeIdx].code != EOF) )
        codeIdx++;

    //-----------------------------
    // find_genericCode() - FEHLER: Code-Zeichen nicht in Tabelle editGenericCode gefunden
    //-----------------------------
    if( editGenericCode[codeIdx].code == EOF )
    {
        _error( E30_CODECHAR_NOT_FOUND, code, false);
        return false;
    }

    memcpy( &genericCode, &editGenericCode[codeIdx], sizeof(editGenericCode_t) );           // die gefundene Struktur in den RAM-Buffer kopieren

    return true;
}



//--------------------------------------------------------------
// ok = find_genericCode( paramID, &v, &min, &max, pFormat)
//
// PARAMETER:
//   paramID
//   *v
//   *min
//   *max
//   *pFormat
//
// Ergebnis steht in: (globale Modulvariablen)
//   paramEditItem
//   paramEditFormat: der zugehoerige Format-String
//   genericCode
//--------------------------------------------------------------
unsigned char find_genericCodeByValue( unsigned char paramID, unsigned char *v, unsigned char *min, unsigned char *max )
{
    unsigned char   codeIdx;
    char           *pFormat;


    //-------------------------------------------------
    // 1. finde paramID in der paramEditItem-Tabelle
    //
    // sollte der RAM-Buffer paramEditItem bereits das
    // gesuchte enthalten dann ok
    //-------------------------------------------------
    if( paramEditItem.paramID != paramID )
        find_paramEditItem( paramID );

    //---
    // FEHLER: paramEditItem nicht in Tabelle paramEditItemTable gefunden
    //---
    if( paramEditItem.paramID == param_EOF )
    {
        _error( E31_PARAMEDIT_ITEM_NOT_FOUND, paramID, false);
        return false;
    }

    pFormat = paramEditFormat;              // Zeiger auf das erste Zeichen vom Format-String (z.B. "DCSWFHQ")
    *v      = paramGet( paramID );          // der aktuelle Wert des paraID

    while( true )
    {
        //------
        // suche editGenericCode mittels aktuelles Zeichens *pFormat
        //------
        codeIdx = 0;
        while( (editGenericCode[codeIdx].code != *pFormat) && (editGenericCode[codeIdx].code != EOF) )
            codeIdx++;


        //------
        // FEHLER: Code-Zeichen nicht in Tabelle editGenericCode gefunden
        //------
        if( editGenericCode[codeIdx].code == EOF )
        {
            _error( E32_CODECHAR_NOT_FOUND, *pFormat, false);
            return false;
        }


        //------
        // suche den Value-Bereich (min/max) fuer das Code-Zeichen
        //------
        if( editGenericCode[codeIdx].code == 'v' )
        {
            // bei code 'v' kommt min/max aus paramEditItem
            *min = paramEditItem.min;
            *max = paramEditItem.max;
        }
        else
        {
            // ansonsten kommt min/max aus dem Code selber
            *min = editGenericCode[codeIdx].min;
            *max = editGenericCode[codeIdx].max;
        }

        // value gefunden (innerhalb von min/max)?
        // beruecksichtigt 'gedrehte' min/max (z.B. Poti1..8)
        if( (*min <= *max && *v >= *min && *v <= *max) || (*min > *max && *v >= *max && *v <= *min) )
        {
            // gefunden - Schleife verlassen
            break;
        }


        //------
        // naechstes Code-Zeichen aus der Format-Maske
        //------
        pFormat++;


        //------
        // FEHLER: Ende des Format-Strings erreicht - fuer das gegebene value konnte kein passender Code im Format-String gefunden werden
        //------
        if( *pFormat == 0 )         // Ende des Format-Strings erreicht - Fehler...
        {
            _error( E33_VALUE_NOT_IN_FORMAT, *v, false);
            return false;
        }
    }

    memcpy( &genericCode, &editGenericCode[codeIdx], sizeof(editGenericCode_t) );           // die gefundene Struktur in den RAM-Buffer kopieren

    genericCode.min = *min;     // ggf. min/max ueberschreiben durch min/max von paramEditItem
    genericCode.max = *max;

    return true;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
const char * get_genericCodeText( uint8_t cmd )
{
    if( cmd == MKPARAM_LONGVALUE )
    {
        if( Config.DisplayLanguage == 0 )   return genericCode.longText_de;
        else                                return genericCode.longText_en;
    }
    else
    {
        if( Config.DisplayLanguage == 0 )   return genericCode.shortText_de;
        else                                return genericCode.shortText_en;
    }
    return 0;
}



//--------------------------------------------------------------
// ok = strValueGeneric( paramID, cmd )
//
// erstellt die String Representation des Wertes von paramID
// im Buffer mkparam_strValueBuffer.
//
// Mit 'cmd' wird gesteuert ob es der Lang-Text oder der
// Kurztext (3 Zeichen) ist.
//
// PARAMETER:
//  paramID:
//  cmd: MKPARAM_SHORTVALUE oder MKPARAM_LONGVALUE
//--------------------------------------------------------------
uint8_t strValueGeneric( unsigned char paramID, uint8_t cmd )
{
    unsigned char  v;
    unsigned char  min;
    unsigned char  max;

    if( find_genericCodeByValue( paramID, &v, &min, &max) )
    {
        //-------------------
        // value gefunden!
        //-------------------
        if( genericCode.code == 'v' )       // Code 'v' (Value) - keine Umrechnung von min/max
        {
            if( max < min )
            {
                // max < min bei 'v' (Value) wird aktuell nicht unterstuezt da
                // kein Beispiel vorhanden ist was damit ggf. gemeint ist.
                // Wenn das irgendwann benoetigt wird muss das hier gecoded werden.
                // Bis dahin wird das als Fehler angesehen (ggf. vertippt beim coden).
                _error( E41_VALUE_MIN_MAX_RANGE, v, false);
                return false;
            }
        }
        else                                // alles andere ausser 'v' - der Wert wird in die Range von min/max umgerechnet
        {
            if( min <= max )
                v = v-min;
            else
                v = min-v;                  // OG Notiz: v = 248 = 8 # 248 = 8, 255 = 1  #   min 255 max = 248
            v++;                            // fuer Anzeige +1: statt '0' eine '1' anzeigen - Beispiel: statt "Poti0" zeige "Poti1"
        }


        //-------------------
        // ertstelle String
        //-------------------
        xsnprintf_P( mkparam_strValueBuffer, MKPARAM_STRBUFFER_LEN, get_genericCodeText(cmd), v);       // erzeuge den Ausgabestring in mkparam_strValueBuffer

        return true;
    }

    return false;
}




//--------------------------------------------------------------
// ok = getGenericNextValue( paramID, vInc )
//
// PARAMETER:
//--------------------------------------------------------------
uint8_t getGenericNextValue( unsigned char paramID, int8_t vInc )
{
    int           v;        // value von paramID - signed int: damit -1 und >255 ausgewertet werden kann
    unsigned char value;    // zum holen von v via find_genericCodeByValue()
    unsigned char min;
    unsigned char max;
    unsigned char codeIdx;
    char          *pFormat;


    if( find_genericCodeByValue( paramID, &value, &min, &max) )
    {
        v = (int)value;     // value vom paramID wird gecastet auf int um -1 und >255 fuer edit zu erkennen

        //------------------------------------------------------
        // Pointer auf das Zeichen im Format-String ermitteln
        //------------------------------------------------------
        pFormat = paramEditFormat;
        while( *pFormat != genericCode.code )   pFormat++;


        if( min > max )   // 'umgedrehte' min/max Werte wie bei Poti1..8
            vInc = vInc * -1;


        v = v + vInc;


        //------------------------------------------------------
        // min/max Grenzen des Code-Zeichens ueberschritten?
        //------------------------------------------------------
        if( (min <= max && v < min) || (min > max && v > min) )
        {
            if( pFormat == paramEditFormat )    // Anfang vom Code-String? -> dann gehe zum Ende (letztes Zeichen)
            {
                while( *(pFormat+1) )   pFormat++;
            }
            else
            {
                pFormat--;                      // vorheriges Format-Zeichen = vorheriger Code
            }


            codeIdx = 0;                        // code-Zeichen suchen in Tabelle editGenericCode
            while( (editGenericCode[codeIdx].code != *pFormat) && (editGenericCode[codeIdx].code != EOF) )
                codeIdx++;

            if( editGenericCode[codeIdx].code == EOF )      // Fehler? -> exit
            {
                return false;
            }

            if( editGenericCode[codeIdx].code == 'v' )
                v = paramEditItem.max;
            else
                v = editGenericCode[codeIdx].max;

        }
        else if( (min <= max && v > max) || (min > max && v < max) )
        {
            pFormat++;                          // naechstes Format-Zeichen = naechster Code
            if( *pFormat == 0 )                 // Ende des Format-Strings?
                pFormat = paramEditFormat;

            codeIdx = 0;                        // code-Zeichen suchen in Tabelle editGenericCode
            while( (editGenericCode[codeIdx].code != *pFormat) && (editGenericCode[codeIdx].code != EOF) )
                codeIdx++;

            if( editGenericCode[codeIdx].code == EOF )      // Fehler? -> exit
            {
                return false;
            }

            if( editGenericCode[codeIdx].code == 'v' )
                v = paramEditItem.min;
            else
                v = editGenericCode[codeIdx].min;
        }


        //------------------------------------------------------
        // neuen Wert setzen
        //------------------------------------------------------
        paramSet( paramID, (unsigned char)v );

        return true;

    }  // end: if( find_genericCode( paramID, &v, &min, &max) )


    return false;
}



//--------------------------------------------------------------
// editGeneric( paramID, cmd )
//
// PARAMETER:
//  cmd: MKPARAM_EDIT oder MKPARAM_SHORTVALUE oder MKPARAM_LONGVALUE
//--------------------------------------------------------------
void editGeneric( unsigned char paramID, uint8_t cmd )
{
    unsigned char v_org;
    uint8_t redraw;
    int8_t  vInc;

    if( cmd != MKPARAM_EDIT )
    {
        // wenn cmd = MKPARAM_SHORTVALUE oder MKPARAM_LONGVALUE
        strValueGeneric( paramID, cmd);                     // Ergebnis in: mkparam_strValueBuffer
        return;
    }

    find_paramEditItem( paramID );

    //------------------------
    // editGeneric() - FEHLER: paramEditItem (=paramID) nicht in Tabelle paramEditItemTable gefunden
    //------------------------
    if( paramEditItem.paramID == param_EOF )
    {
        _error( E11_PARAMID_EDITGENERIC, paramID, true);
        return;
    }

    lcd_cls();

    v_org   = paramGet( paramID );
    redraw  = SCREEN_REDRAW;
    vInc    = 0;        // 0, +1, -1  (inc/dec)

    while( true )
    {
        //-----------------------
        // Anzeige: Titel, usw..
        //-----------------------
        if( redraw == SCREEN_REDRAW )
        {
            lcd_frect( 0, 0, 127, 7, 1);                                            // Headline: Box fill
            lcdx_printp_at( 1, 0, PSTR("Ändern"), MINVERS,  0,0);                   // Titel
            lcd_printp_at( 0, 7, strGet(KEYLINE2), MNORMAL);                        // Keyline: <-  ->  Ende OK
            lcd_printp_at(11, 7, strGet(KEYCANCEL), MNORMAL);                       // Keyline: "Abbr." statt "Ende" einsetzen

            print_paramEditItemTitle();                                             // Bezeichnung des paramID's anzeigen

            redraw = SCREEN_REFRESH;
        }


        //---------------
        // LiPo Anzeigen
        //---------------
        show_Lipo();                                                                        // LiPo anzeigen


        //-----------------------
        // Anzeige: nur Wert
        //-----------------------
        if( redraw == SCREEN_REFRESH )
        {
            strValueGeneric( paramID, MKPARAM_LONGVALUE);                                   // Lang-Text des Values anzeigen
                                                                                            // nach strValueGeneric() ist auch genericCode initialisiert!
            lcd_printf_at_P( 0, 4, MNORMAL, PSTR("> %18s"), mkparam_strValueBuffer );


            // DEBUG...
            #ifdef DEBUG_PARAMEDIT
                //lcd_printf_at_P( 16, 1, MNORMAL, PSTR("%c %3u"), genericCode.code,    paramGet(paramID) );
            #endif

            //lcd_printf_at_P( 16, 4, MNORMAL, PSTR("= %03u"), v );


            if( abs(genericCode.max-genericCode.min) <= 20 )
              _delay_ms( 200 );                                                             // Verzoegerung

            vInc   = 0;
            redraw = 0;
        }


        //-----------------
        // TASTEN
        //-----------------
        if( get_key_press(1 << KEY_ESC) )                                                   // Key: Cancel
        {
            paramSet( paramID, v_org );             // org. Wert wieder herstellen
            break;
        }

        if( get_key_press(1 << KEY_ENTER) )                                                 // Key: OK
        {
            break;
        }

        if( get_key_press(1 << KEY_PLUS) || get_key_long_rpt_sp( (1 << KEY_PLUS),2) )       // Key: rechts / +1
        {
            vInc = +1;
        }


        if( get_key_press(1 << KEY_MINUS) || get_key_long_rpt_sp( (1 << KEY_MINUS),2) )     // Key: links / -1
        {
            vInc = -1;
        }

        if( vInc != 0 )
        {
            getGenericNextValue( paramID, vInc );
            redraw = SCREEN_REFRESH;
        }


        //------------------------------------------
        // Pruefe PKT-Update oder andere PKT-Aktion
        //------------------------------------------
        if( PKT_CtrlHook() )                                            // Update vom Updatetool angefordert?
        {
            lcd_cls();
            redraw = SCREEN_REDRAW;
        }

    }  // end: while( true )

}



//--------------------------------------------------------------
// editBitmask( paramID, cmd )
//
// PARAMETER:
//  paramID: die paramID
//  cmd:     MKPARAM_EDIT oder MKPARAM_SHORTVALUE oder MKPARAM_LONGVALUE
//--------------------------------------------------------------
void editBitmask( unsigned char paramID, uint8_t cmd )
{
    unsigned char v;
    uint8_t  redraw;
    uint8_t  i;
    int8_t   pos;

    find_paramEditItem( paramID );

    //-----------------------
    // editBitmask() - FEHLER: paramEditItem (=paramID) nicht in Tabelle paramEditItemTable gefunden
    //-----------------------
    if( paramEditItem.paramID == param_EOF )
    {
        _error( E12_PARAMID_EDITBITMASK, paramID, (cmd==MKPARAM_EDIT) );
        return;
    }


    //-----------------------
    // nur Rueckgabe des Wertes in mkparam_strValueBuffer ?
    //-----------------------
    if( cmd != MKPARAM_EDIT )
    {
        v = paramGet( paramID );
        xsnprintf_P( mkparam_strValueBuffer, MKPARAM_STRBUFFER_LEN, PSTR("%03u"), v);       // erzeuge den Ausgabestring in mkparam_strValueBuffer
        return;
    }


    lcd_cls();

    v       = paramGet( paramID );
    redraw  = SCREEN_REDRAW;
    pos     = 0;

    while( true )
    {
        //-----------------------
        // Anzeige: Titel, usw..
        //-----------------------
        if( redraw == SCREEN_REDRAW )
        {
            lcd_frect( 0, 0, 127, 7, 1);                                            // Headline: Box fill
            lcdx_printp_at( 1, 0, PSTR("Ändern"), MINVERS,  0,0);                   // Titel
            lcd_printp_at(   1, 7, PSTR("0/1   \x19           OK"), MNORMAL);       // Keyline: ->  0/1      OK
            lcd_printp_at(  11, 7, strGet(KEYCANCEL), MNORMAL);                     // Keyline: "Abbr." bzw "Cancel"
            lcdx_printp_at(  7, 6, PSTR("\x18"), MNORMAL, -2,2);                    // Keyline langer Tastendruck: ->  0/1      OK

            if( (paramID == param_J16Bitmask) || (paramID == param_J17Bitmask) )
            {
                lcdx_printp_at( 10, 5, PSTR("(Bit1=Idle)"), MNORMAL, 0,2);              //
            }

            print_paramEditItemTitle();                                             // Bezeichnung des paramID's (Stelle x=0, y=2)

            redraw = SCREEN_REFRESH;
        }


        //---------------
        // LiPo Anzeigen
        //---------------
        show_Lipo();                                                                // LiPo anzeigen


        //-----------------------
        // Anzeige: nur Wert
        //-----------------------
        if( redraw == SCREEN_REFRESH )
        {
            for(i = 0; i < 8; i++)
            {
                lcd_putc( 8-i, 4, ( (v & (1 << i)) ? '1' : '0'), MNORMAL);
            }

            lcd_frect( 0, (8*5), 125-(11*6), 7, 0);                                 // clear: Eingabemarkierung
            lcd_printp_at (pos+1, 5,PSTR("\x12"), MNORMAL);                         // Eingabemarkierung (Pfeil nach oben)

            lcd_printf_at_P( 15, 4, MNORMAL, PSTR("= %03u"), v );                   // Anzeige des aktuellen Wertes Dezimal

            //  _delay_ms( 200 );                                                   // Verzoegerung
            redraw = 0;
        }


        //-----------------
        // TASTEN
        //-----------------
        if( get_key_short(1 << KEY_ESC) )                                           // Key: Cancel
        {
            break;                                                                  // verlassen
        }

        if (get_key_short(1 << KEY_ENTER))                                          // Key: OK
        {
            paramSet( paramID, v );                                                 // Wert speichern
            break;                                                                  // und verlassen
        }

        if( get_key_short(1 << KEY_PLUS) )
        {
            if(pos == 7) pos = 0;
            else         pos++;
            redraw = SCREEN_REFRESH;
        }

        if( get_key_long(1 << KEY_PLUS) )
        {
            if(pos == 0) pos = 7;
            else         pos--;
            redraw = SCREEN_REFRESH;
        }


        if( get_key_short (1 << KEY_MINUS) )
        {
            v ^= (1<<(7-pos));
            redraw = SCREEN_REFRESH;
        }


        //------------------------------------------
        // evtl. weitere lange Tasten abfangen, da es
        // ansonsten ggf. Nebeneffekte bzgl. dem Menue
        // beim verlassen der Funktion gibt
        //------------------------------------------
        get_key_long_rpt_sp( KEY_ALL,2);


        //------------------------------------------
        // Pruefe PKT-Update oder andere PKT-Aktion
        //------------------------------------------
        if( PKT_CtrlHook() )                                            // Update vom Updatetool angefordert?
        {
            lcd_cls();
            redraw = SCREEN_REDRAW;
        }

    }  // end: while( true )
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void editCompassOffset( unsigned char paramID, uint8_t cmd )
{
    unsigned char vu;   // unsigned byte
    int      v;  // signed
    uint8_t  redraw;
    uint8_t  bit7;
    uint8_t  bit8;
    uint8_t  lDisDeclCalc;

    find_paramEditItem( paramID );

    //-----------------------
    // editCompassOffset() - FEHLER: paramEditItem (=paramID) nicht in Tabelle paramEditItemTable gefunden
    //-----------------------
    if( paramEditItem.paramID == param_EOF )
    {
        // TODO: Fehlercode anpassen!
        _error( E12_PARAMID_EDITBITMASK, paramID, (cmd==MKPARAM_EDIT) );
        return;
    }


    //-----------------------
    // Wert ermitteln (-60 bis 60)
    //-----------------------
    vu = paramGet( paramID );

    bit7 = ((vu & 0x40) ? true : false);            // Bit 7: 1 = negativ; 0 = positiv
    bit8 = ((vu & 0x80) ? true : false);            // Bit 8 in Kombination mit Bit 7 = "Disable dec. Calc"

    // Bit 8 == Bit 7: "Disable dec. Calc"  AUS
    // Bit 8 != Bit 7: "Disable dec. Calc"  AN
    lDisDeclCalc = ((bit8 == bit7) ? 0 : 1);        // merken um das spaeter wieder in den Wert einzubauen

    vu = (vu & (0x80 ^ 0xff));                      // Bit 8 loeschen

    if( bit7 )                                      // Wert negativ?
    {
        vu = vu - 1;                                // Umrechnung: 7-Bit Zweierkomplement
        vu = (vu ^ 0xff);                           // invertieren
        vu = (vu & (0x80 ^ 0xff));                  // Bit 8 loeschen
        v = (int)vu;
        v = v * -1;
    }
    else                                            // Wert ist positiv
    {
        v = (int)vu;
    }


    //-----------------------
    // nur Rueckgabe des Wertes in mkparam_strValueBuffer ?
    //-----------------------
    if( cmd != MKPARAM_EDIT )
    {
        xsnprintf_P( mkparam_strValueBuffer, MKPARAM_STRBUFFER_LEN, PSTR("%3d"), v );       // erzeuge den Ausgabestring in mkparam_strValueBuffer
        return;
    }


    //-----------------------
    // Wert Editieren
    //-----------------------
    lcd_cls();

    redraw  = SCREEN_REDRAW;

    while( true )
    {
        //-----------------------
        // Anzeige: Titel, usw..
        //-----------------------
        if( redraw == SCREEN_REDRAW )
        {
            lcd_frect( 0, 0, 127, 7, 1);                                            // Headline: Box fill
            lcdx_printp_at( 1, 0, PSTR("Ändern")   , MINVERS,  0,0);                // Titel
            lcd_printp_at(  0, 7, strGet(KEYLINE2) , MNORMAL);                      // Keyline: <-  ->  Ende OK
            lcd_printp_at( 11, 7, strGet(KEYCANCEL), MNORMAL);                      // Keyline: "Abbr." statt "Ende" einsetzen

            print_paramEditItemTitle();                                             // Bezeichnung des paramID's anzeigen

            redraw = SCREEN_REFRESH;
        }


        //---------------
        // LiPo Anzeigen
        //---------------
        show_Lipo();                                                                // LiPo anzeigen


        //-----------------------
        // Anzeige: Wert
        //-----------------------
        if( redraw == SCREEN_REFRESH )
        {
            lcd_printf_at_P( 0, 4, MNORMAL, PSTR("> %3d"), v );
            redraw = false;
        }


        //-----------------
        // TASTEN
        //-----------------
        if( get_key_short(1 << KEY_ESC) )                                           // Key: Cancel
        {
            break;                                                                  // verlassen
        }

        if( get_key_short(1 << KEY_ENTER) )                                         // Key: OK
        {
            //-------------------------------------
            // signed int in unsigned byte wandeln
            //-------------------------------------
            if( v < 0 )                                         // Wert negativ?
            {
                // Umwandeln in 7-Bit Zweierkomplement
                v  = v * -1;                                    // Wert positiv machen
                vu = (unsigned char)v;                          // in unsigned Byte speichern
                vu = (vu ^ 0xff);                               // invertieren
                vu = vu + 1;                                    // +1
            }
            else                                                // Wert ist positiv
            {
                vu = (unsigned char)v;                          // in unsigned Byte speichern
            }

            //-------------------------------------
            // "Disable declination calc" wieder einrechnen
            //-------------------------------------
            bit7 = ((vu & 0x40) ? true : false);                // Bit 7: 1 = negativ; 0 = positiv

            if( lDisDeclCalc )  bit8 = !bit7;                   // Bit 8 != Bit 7: "Disable dec. Calc"  AN
            else                bit8 = bit7;                    // Bit 8 == Bit 7: "Disable dec. Calc"  AUS

            if( bit8 )          vu = (vu | 0x80);               // Bit 8 setzen
            else                vu = (vu & (0x80 ^ 0xff));      // Bit 8 loeschen

            //-------------------------------------
            // Wert speichern
            //-------------------------------------
            paramSet( paramID, vu );                            // Wert speichern
            break;                                              // und verlassen
        } // end: KEY_ENTER


        if( get_key_short(1 << KEY_MINUS) || get_key_long_rpt_sp( (1 << KEY_MINUS),2) )
        {
            if( v <= -60 )  v = 60;
            else            v = v - 1;
            redraw = SCREEN_REFRESH;
        }


        if( get_key_short(1 << KEY_PLUS) || get_key_long_rpt_sp( (1 << KEY_PLUS),2) )
        {
            if( v >= 60 )   v = -60;
            else            v = v + 1;
            redraw = SCREEN_REFRESH;
        }

        //------------------------------------------
        // evtl. weitere lange Tasten abfangen, da es
        // ansonsten ggf. Nebeneffekte bzgl. dem Menue
        // beim verlassen der Funktion gibt
        //------------------------------------------
        get_key_long_rpt_sp( KEY_ALL,2);


        //------------------------------------------
        // Pruefe PKT-Update oder andere PKT-Aktion
        //------------------------------------------
        if( PKT_CtrlHook() )                                            // Update vom Updatetool angefordert?
        {
            lcd_cls();
            redraw = SCREEN_REDRAW;
        }

    }  // end: while( true )
}

//--------------------------------------------------------------
void editACCZLandingPulse( unsigned char paramID, uint8_t cmd )
{
    unsigned char vu;   // unsigned byte
//    int      v;  // signed
    uint8_t  redraw;
//    uint8_t  bit7;
//    uint8_t  bit8;
//    uint8_t  lDisDeclCalc;

    find_paramEditItem( paramID );

    //-----------------------
    // editACCZLandingPulse() - FEHLER: paramEditItem (=paramID) nicht in Tabelle paramEditItemTable gefunden
    //-----------------------
    if( paramEditItem.paramID == param_EOF )
    {
        // TODO: Fehlercode anpassen!
        _error( E12_PARAMID_EDITBITMASK, paramID, (cmd==MKPARAM_EDIT) );
        return;
    }

    vu = paramGet( paramID );

    //-----------------------
    // nur Rueckgabe des Wertes in mkparam_strValueBuffer ?
    //-----------------------
    if( cmd != MKPARAM_EDIT )
    {
        xsnprintf_P( mkparam_strValueBuffer, MKPARAM_STRBUFFER_LEN, PSTR("%4d"), vu*4 );       // erzeuge den Ausgabestring in mkparam_strValueBuffer
        return;
    }


    //-----------------------
    // Wert Editieren
    //-----------------------
    lcd_cls();

    redraw  = SCREEN_REDRAW;

    while( true )
    {
        //-----------------------
        // Anzeige: Titel, usw..
        //-----------------------
        if( redraw == SCREEN_REDRAW )
        {
            lcd_frect( 0, 0, 127, 7, 1);                                            // Headline: Box fill
            lcdx_printp_at( 1, 0, PSTR("Ändern")   , MINVERS,  0,0);                // Titel
            lcd_printp_at(  0, 7, strGet(KEYLINE2) , MNORMAL);                      // Keyline: <-  ->  Ende OK
            lcd_printp_at( 11, 7, strGet(KEYCANCEL), MNORMAL);                      // Keyline: "Abbr." statt "Ende" einsetzen

            print_paramEditItemTitle();                                             // Bezeichnung des paramID's anzeigen

            redraw = SCREEN_REFRESH;
        }


        //---------------
        // LiPo Anzeigen
        //---------------
        show_Lipo();                                                                // LiPo anzeigen


        //-----------------------
        // Anzeige: Wert
        //-----------------------
        if( redraw == SCREEN_REFRESH )
        {
            lcd_printf_at_P( 0, 4, MNORMAL, PSTR("> %4d"), vu *4 );
            redraw = false;
        }


        //-----------------
        // TASTEN
        //-----------------
        if( get_key_short(1 << KEY_ESC) )                                           // Key: Cancel
        {
            break;                                                                  // verlassen
        }

        if( get_key_short(1 << KEY_ENTER) )                                         // Key: OK
        {
//            //-------------------------------------
//            // signed int in unsigned byte wandeln
//            //-------------------------------------
//            if( v < 0 )                                         // Wert negativ?
//            {
//                // Umwandeln in 7-Bit Zweierkomplement
//                v  = v * -1;                                    // Wert positiv machen
//                vu = (unsigned char)v;                          // in unsigned Byte speichern
//                vu = (vu ^ 0xff);                               // invertieren
//                vu = vu + 1;                                    // +1
//            }
//            else                                                // Wert ist positiv
//            {
//                vu = (unsigned char)v;                          // in unsigned Byte speichern
//            }
//
//            //-------------------------------------
//            // "Disable declination calc" wieder einrechnen
//            //-------------------------------------
//            bit7 = ((vu & 0x40) ? true : false);                // Bit 7: 1 = negativ; 0 = positiv
//
//            if( lDisDeclCalc )  bit8 = !bit7;                   // Bit 8 != Bit 7: "Disable dec. Calc"  AN
//            else                bit8 = bit7;                    // Bit 8 == Bit 7: "Disable dec. Calc"  AUS
//
//            if( bit8 )          vu = (vu | 0x80);               // Bit 8 setzen
//            else                vu = (vu & (0x80 ^ 0xff));      // Bit 8 loeschen

            //-------------------------------------
            // Wert speichern
            //-------------------------------------
            paramSet( paramID, vu );                            // Wert speichern
            break;                                              // und verlassen
        } // end: KEY_ENTER


        if( get_key_short(1 << KEY_MINUS) || get_key_long_rpt_sp( (1 << KEY_MINUS),2) )
        {
//            if( v <= -60 )  v = 60;
//            else            v = v - 1;
//
            if( vu < 191 )  vu = 190;
            else             vu = vu -1;

            redraw = SCREEN_REFRESH;
        }


        if( get_key_short(1 << KEY_PLUS) || get_key_long_rpt_sp( (1 << KEY_PLUS),2) )
        {
//            if( v >= 60 )   v = -60;
//            else            v = v + 1;
            if( vu >= 255 )   vu = 255;
//            if( vu == 1 )   vu = 191;
            else              vu = vu +1;
            redraw = SCREEN_REFRESH;
        }

        //------------------------------------------
        // evtl. weitere lange Tasten abfangen, da es
        // ansonsten ggf. Nebeneffekte bzgl. dem Menue
        // beim verlassen der Funktion gibt
        //------------------------------------------
        get_key_long_rpt_sp( KEY_ALL,2);


        //------------------------------------------
        // Pruefe PKT-Update oder andere PKT-Aktion
        //------------------------------------------
        if( PKT_CtrlHook() )                                            // Update vom Updatetool angefordert?
        {
            lcd_cls();
            redraw = SCREEN_REDRAW;
        }

    }  // end: while( true )
}

//--------------------------------------------------------------


//--------------------------------------------------------------
// editNA( paramID, cmd )
//
// Hilfsfunktion die nur "nicht verfï¿½gbar" anzeigt wenn die
// paramID noch nicht implementiert ist
//
// PARAMETER:
//  paramID: die paramID
//  cmd:     MKPARAM_EDIT oder MKPARAM_SHORTVALUE oder MKPARAM_LONGVALUE
//--------------------------------------------------------------
void editNA( unsigned char paramID, uint8_t cmd )
{
    if( cmd != MKPARAM_EDIT )
    {
        strncpy_P( mkparam_strValueBuffer, PSTR("NA!"), MKPARAM_STRBUFFER_LEN);     // "NA!" bzw, "not available"
        return;
    }

    //PKT_Message_P( *text, error, timeout, beep, clearscreen )
    PKT_Message_P( PSTR("nicht verfügbar"), false, 2000, true, true );       // "nicht verfï¿½gbar"
}




//#############################################################################################
//# TEST / DEBUG
//#############################################################################################

//--------------------------------------------------------------
//--------------------------------------------------------------
void editGenericTEST( unsigned char paramID )
{
    lcd_cls();

    //lcdx_printp_at( 0, 0, PSTR("NEW PARAM TEST..."), MNORMAL,  0,0);
    lcd_printp_at(12, 7, strGet(ENDE), MNORMAL);                    // Keyline

    find_paramEditItem( paramID );      // Ergebnis in paramEditItem (RAM)

    if( paramEditItem.paramID == paramID )
    {
        lcd_printf_at_P( 0, 1, MNORMAL, PSTR("%S"),         paramEditItem.title_de );

        lcd_printf_at_P( 0, 2, MNORMAL, PSTR("format: %s"), paramEditFormat );

        lcd_printf_at_P( 0, 3, MNORMAL, PSTR("value:%u"),   paramGet(paramID)   );

        strValueGeneric( paramID, MKPARAM_LONGVALUE);
        lcd_printf_at_P( 0, 4, MNORMAL, PSTR("vstr: %s"), mkparam_strValueBuffer );

        strValueGeneric( paramID, MKPARAM_SHORTVALUE);
        lcd_printf_at_P( 0, 5, MNORMAL, PSTR("vstr: %s"), mkparam_strValueBuffer );
    }


    if( paramEditItem.paramID == param_EOF )
    {
        lcd_printf_at_P( 0, 5, MNORMAL, PSTR("! ERROR !") );
    }


    //-----------------------------------------
    // Tasten...
    while( true )
    {
        PKT_CtrlHook();
        if (get_key_press (1 << KEY_ESC))
        {
            break;
        }
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void TEST(void)
{
    //editGeneric( param_Gyro_Gier_P, MKPARAM_EDIT );
    editGeneric( param_Kanalbelegung_Gas, MKPARAM_EDIT );
}




//#############################################################################################
//# Menues & Favoriten
//#############################################################################################


//--------------------------------------------------------------
// fav_add()
//
// einen Favoriten hinzufuegen
//
// Aufruf durch: wird durch den Menu-Controller aufgerufen
//--------------------------------------------------------------
void fav_add( void )
{
    uint8_t paramID;
    uint8_t i;


    //------------------------------
    // welche paramID wurde gewaehlt?
    //------------------------------
    paramID = MenuCtrl_GetItemId();                             // gewaehlter Menuepunkt bzw. paramID   (0 = keine itemID)


    //------------------------------
    // ungueltige paramID?
    //------------------------------
    if( paramID==0 || paramID==EOF )
    {
        return;                                                 // keine gueltige paramID gewaehlt -> ZURUECK
    }


    //------------------------------
    // Duplikatsuche bzgl. paramID
    // (gilt nicht fuer SEPARATOR)
    //------------------------------
    if( paramID!=SEPARATOR )
    {
        //------------------------------
        // suche Duplikate fuer paramID
        //------------------------------
        for( i=0; (i<MAX_MKPARAM_FAVORITES) && (Config.MKParam_Favs[i]!=paramID); i++);

        //------------------------------
        // Favoriten bereits vorhanden?
        //  -> exit
        //------------------------------
        if( Config.MKParam_Favs[i] == paramID )
        {
            set_beep( 300, 0x000f, BeepNormal);                 // Error Beep kurz
            PKT_Popup_P( 400, strGet(STR_FAV_EXIST),0,0,0);     // "* Fav vorhanden *"  (ca. 4 sec max.)
            return;
        }
    } // end: if( paramID!=SEPARATOR )


    //------------------------------
    // suche freien Speicherplatz fuer paramID
    //------------------------------
    for( i=0; (i<MAX_MKPARAM_FAVORITES) && (Config.MKParam_Favs[i]!=0); i++);

    //------------------------------
    // Favoriten voll?
    //  -> exit
    //------------------------------
    if( i >= MAX_MKPARAM_FAVORITES )
    {
        set_beep( 500, 0x000f, BeepNormal);                     // Error Beep lang
        PKT_Popup_P( 400, strGet(STR_FAV_FULL),0,0,0);          // "* Fav ist voll *"  (ca. 4 sec max.)
        return;
    }

    //------------------------------
    // neuen Fav merken
    //------------------------------
    Config.MKParam_Favs[i] = paramID;
    set_beep( 25, 0xffff, BeepNormal );                         // kurzer Bestaetigungs-Beep
    PKT_Popup_P( 400, strGet(STR_FAV_ADD),0,0,0);               // "Fav hinzugefügt!"  (ca. 4 sec max.)
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Menu_Favoriten( void )
{
    uint8_t paramID;
    uint8_t i;

  //unsigned char   MKParam_Favs[MAX_MKPARAM_FAVORITES];                        // Array von MK-Parameter Favoriten des Benutzers

    if( Config.MKParam_Favs[0] == 0 )                                           // Favoriten vorhanden
    {
      //PKT_Message_P( *text, error, timeout, beep, clearscreen )
        PKT_Message_P( strGet(STR_FAV_NOTEXIST), false, 500, true, true );      // Anzeige "nicht verfügbar" (max. 2 Sekunden anzeigen)
        return;
    }


    //---------------
    // Create
    //---------------
    MenuCtrl_Create();
    MenuCtrl_SetTitleFromParentItem();                                          // uebernimmt den Titel vom uebergeordneten Menuepunkt
    MenuCtrl_ShowLevel( false );
    MenuCtrl_SetDelete( true  );                                                // Menueeintraege loeschen      (langer Druck auf "OK")
    MenuCtrl_SetMove( true );                                                   // Menueeintraege verschieben   (langer Druck auf hoch/runter)
  //MenuCtrl_SetShowBatt( true );
  //MenuCtrl_SetCycle( false );
  //MenuCtrl_SetBeep( true );


    //---------------
    // Menu-Items
    //---------------
    i = 0;
    while( (i<MAX_MKPARAM_FAVORITES) && (Config.MKParam_Favs[i]!=0) )
    {
        paramID = Config.MKParam_Favs[i];

        if( paramID == SEPARATOR )
        {
            MenuCtrl_PushSeparatorID( SEPARATOR );                              // Trennlinie im Menue hinzufuegen
        }
        else
        {
            find_paramEditItem( paramID );

            //--------
            // FEHLER: paramEditItem (=paramID) nicht in Tabelle paramEditItemTable gefunden - Menu_EditCategory()
            //--------
            if( paramEditItem.paramID == param_EOF )
            {
                _error( E01_PARAMID_MENUEDITCATEGORY, paramID, true);
                MenuCtrl_Destroy();
                return;
            }

            MenuCtrl_PushParamEdit( paramID );

            // existiert die paramID in der Firmware des Kopters?
            if( !paramExist(paramID) )
            {
                MenuCtrl_ItemActive( paramID, false );                          // paramID deaktivieren
            }

            // DEBUG! (Simulation eines deaktivierten Fav's)
            // if( i==2 )   MenuCtrl_ItemActive( paramID, false );              // paramID deaktivieren

        } // end: else: if( paramID == SEPARATOR )

        i++;
    } // end: while(..)



    //---------------
    // Control
    //---------------
    while( true )
    {
        MenuCtrl_Control( MENUCTRL_EVENT );
        if( MenuCtrl_GetKey() == KEY_ESC )      break;      // KEY_ESC = Menue beenden

        paramID = MenuCtrl_GetItemId();                     // welcher Menu-Punkt (= paramID)

        find_paramEditItem( paramID );                      // Edit-Definitionen heraussuchen (initialisiert: paramEditItem)

        paramEditItem.editfunc( paramID, MKPARAM_EDIT );    // zugeordnete Edit-Funktion aufrufen
    }


    //---------------
    // ggf. neue Menuereihenfolge
    // in der Config speichern
    //---------------
    for( i=0; (i<MAX_MKPARAM_FAVORITES); i++)
    {
        Config.MKParam_Favs[i] = MenuCtrl_GetItemIdByIndex( i );
    }


    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Menu_EditCategory( const unsigned char *menuitems )
{
    unsigned char   paramID;
    unsigned char   lastmenuitem = 0;

    //---------------
    // Create
    //---------------
    MenuCtrl_Create();

    //---------------
    // Einstellungen
    //---------------
    MenuCtrl_SetTitleFromParentItem();      // uebernimmt den Titel vom uebergeordneten Menuepunkt
    MenuCtrl_ShowLevel( false );
  //MenuCtrl_SetShowBatt( true );
  //MenuCtrl_SetCycle( false );
  //MenuCtrl_SetBeep( true );

  //MenuCtrl_SetKey( uint8_t key, const char *keytext, void (*keyfunc)(void) )
    MenuCtrl_SetKey( KEY_ENTER_LONG, 0, &fav_add );


    //---------------
    // Menuitems
    //---------------
    while( *menuitems != EOF )
    {
        paramID = *menuitems;

        if( paramID == SEPARATOR )
        {
            // wenn der vorherige Menueeintrag bereits ein SEPARATOR war
            // dann nicht erneut einen Separator anzeigen!
            //
            // Das kann ggf. passieren wenn z.B. aufgrund der FC-Revision
            // alle Zwischeneintraege ausgeblendet wurden
            if( lastmenuitem != SEPARATOR )
            {
                MenuCtrl_PushSeparatorID( SEPARATOR );       // Trennlinie im Menue hinzufuegen
                lastmenuitem = paramID;
            }
        }
        else
        {
            //------------------------------------------------------
            // TEST / DEBUG fuer beschriebenes Problem in _error()
            //
            // Wenn man das dortige Problem nachstellen will kann
            // man hier einen Fehlerscreen provozieren!
            //------------------------------------------------------
            //paramID = 0;


            find_paramEditItem( paramID );

            //--------
            // FEHLER: paramEditItem (=paramID) nicht in Tabelle paramEditItemTable gefunden - Menu_EditCategory()
            //--------
            if( paramEditItem.paramID == param_EOF )
            {
                _error( E01_PARAMID_MENUEDITCATEGORY, paramID, true);
                MenuCtrl_Destroy();
                return;
            }

            // existiert die paramID in der Firmware des Kopters?
            if( paramExist(paramEditItem.paramID) )
            {
                MenuCtrl_PushParamEdit( paramEditItem.paramID );
                lastmenuitem = paramID;
            }
        }

        menuitems++;
    }


    //---------------
    // Control
    //---------------
    while( true )
    {
        MenuCtrl_Control( MENUCTRL_EVENT );
        if( MenuCtrl_GetKey() == KEY_ESC )      break;      // Menue beenden

        paramID = MenuCtrl_GetItemId();                     // welcher Menu-Punkt (= paramID)

        find_paramEditItem( paramID );                      // Edit-Definitionen heraussuchen (initialisiert: paramEditItem)

        paramEditItem.editfunc( paramID, MKPARAM_EDIT );    // zugeordnete Edit-Funktion aufrufen
    }

    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();
}




//--------------------------------------------------------------
//--------------------------------------------------------------
void MK_Parameters_MenuMain( uint8_t setting, char *settingname )
{
    uint8_t itemid;

    //---------------
    // Create
    //---------------
    MenuCtrl_Create();

    //---------------
    // Einstellungen
    //---------------
    MenuCtrl_SetTitle( settingname );
    MenuCtrl_ShowLevel( false );
    //MenuCtrl_SetShowBatt( true );
    //MenuCtrl_SetCycle( false );
    //MenuCtrl_SetBeep( true );

    //---------------
    // Menuitems
    //---------------
  //MenuCtrl_PushML2_P( ID_MENU_TEST         , MENU_ITEM, &TEST, ID_MENU_TEST_de    , ID_MENU_TEST_en     );

    MenuCtrl_PushML2_P( ID_MENU_FAVORITEN    , MENU_ITEM, NOFUNC, ID_MENU_FAVORITEN_de    , ID_MENU_FAVORITEN_en     ); // Favoriten: noch nicht implementiert...

    MenuCtrl_PushSeparator();   // MENUE TRENNLINIE
    MenuCtrl_PushML2_P( ID_MENU_EASYSETUP    , MENU_ITEM, NOFUNC, ID_MENU_EASYSETUP_de    , ID_MENU_EASYSETUP_en     );
    MenuCtrl_PushML2_P( ID_MENU_KANAELE      , MENU_ITEM, NOFUNC, ID_MENU_KANAELE_de      , ID_MENU_KANAELE_en       );
    MenuCtrl_PushML2_P( ID_MENU_KAMERA       , MENU_ITEM, NOFUNC, ID_MENU_KAMERA_de       , ID_MENU_KAMERA_en        );
    MenuCtrl_PushML2_P( ID_MENU_CAMCTRL      , MENU_ITEM, NOFUNC, ID_MENU_CAMCTRL_de      , ID_MENU_CAMCTRL_en       );
    MenuCtrl_PushML2_P( ID_MENU_AUSGAENGE    , MENU_ITEM, NOFUNC, ID_MENU_AUSGAENGE_de    , ID_MENU_AUSGAENGE_en     );
    MenuCtrl_PushML2_P( ID_MENU_KONFIGURATION, MENU_ITEM, NOFUNC, ID_MENU_KONFIGURATION_de, ID_MENU_KONFIGURATION_en );
    MenuCtrl_PushML2_P( ID_MENU_STICK        , MENU_ITEM, NOFUNC, ID_MENU_STICK_de        , ID_MENU_STICK_en         );
//    MenuCtrl_PushML2_P( ID_MENU_LOOPING      , MENU_ITEM, NOFUNC, ID_MENU_LOOPING_de      , ID_MENU_LOOPING_en       );
    MenuCtrl_PushML2_P( ID_MENU_HOEHE        , MENU_ITEM, NOFUNC, ID_MENU_HOEHE_de        , ID_MENU_HOEHE_en         );

    MenuCtrl_PushML2_P( ID_MENU_NAVICTRL     , MENU_ITEM, NOFUNC, ID_MENU_NAVICTRL_de     , ID_MENU_NAVICTRL_en      );

    MenuCtrl_PushML2_P( ID_MENU_VERSCHIEDENES, MENU_ITEM, NOFUNC, ID_MENU_VERSCHIEDENES_de, ID_MENU_VERSCHIEDENES_en );
    MenuCtrl_PushML2_P( ID_MENU_GYRO         , MENU_ITEM, NOFUNC, ID_MENU_GYRO_de         , ID_MENU_GYRO_en          );
    MenuCtrl_PushML2_P( ID_MENU_BENUTZER     , MENU_ITEM, NOFUNC, ID_MENU_BENUTZER_de     , ID_MENU_BENUTZER_en      );
    MenuCtrl_PushML2_P( ID_MENU_ACHSKOPPLUNG , MENU_ITEM, NOFUNC, ID_MENU_ACHSKOPPLUNG_de , ID_MENU_ACHSKOPPLUNG_en  );
  //MenuCtrl_PushML2_P( ID_MENU_MIXERSETUP   , MENU_ITEM, NOFUNC, ID_MENU_MIXERSETUP_de   , ID_MENU_MIXERSETUP_en    ); // nicht mehr unterstuetzt!

// Parametermenü bis PKT Version 3.89
//    MenuCtrl_PushML2_P( ID_MENU_KANAELE      , MENU_ITEM, NOFUNC, ID_MENU_KANAELE_de      , ID_MENU_KANAELE_en       );
//    MenuCtrl_PushML2_P( ID_MENU_KONFIGURATION, MENU_ITEM, NOFUNC, ID_MENU_KONFIGURATION_de, ID_MENU_KONFIGURATION_en );
//    MenuCtrl_PushML2_P( ID_MENU_STICK        , MENU_ITEM, NOFUNC, ID_MENU_STICK_de        , ID_MENU_STICK_en         );
//    MenuCtrl_PushML2_P( ID_MENU_LOOPING      , MENU_ITEM, NOFUNC, ID_MENU_LOOPING_de      , ID_MENU_LOOPING_en       );
//    MenuCtrl_PushML2_P( ID_MENU_HOEHE        , MENU_ITEM, NOFUNC, ID_MENU_HOEHE_de        , ID_MENU_HOEHE_en         );
//    MenuCtrl_PushML2_P( ID_MENU_KAMERA       , MENU_ITEM, NOFUNC, ID_MENU_KAMERA_de       , ID_MENU_KAMERA_en        );
//    MenuCtrl_PushML2_P( ID_MENU_CAMCTRL      , MENU_ITEM, NOFUNC, ID_MENU_CAMCTRL_de      , ID_MENU_CAMCTRL_en       );
//    MenuCtrl_PushML2_P( ID_MENU_NAVICTRL     , MENU_ITEM, NOFUNC, ID_MENU_NAVICTRL_de     , ID_MENU_NAVICTRL_en      );
//    MenuCtrl_PushML2_P( ID_MENU_AUSGAENGE    , MENU_ITEM, NOFUNC, ID_MENU_AUSGAENGE_de    , ID_MENU_AUSGAENGE_en     );
//    MenuCtrl_PushML2_P( ID_MENU_VERSCHIEDENES, MENU_ITEM, NOFUNC, ID_MENU_VERSCHIEDENES_de, ID_MENU_VERSCHIEDENES_en );
//    MenuCtrl_PushML2_P( ID_MENU_GYRO         , MENU_ITEM, NOFUNC, ID_MENU_GYRO_de         , ID_MENU_GYRO_en          );
//    MenuCtrl_PushML2_P( ID_MENU_BENUTZER     , MENU_ITEM, NOFUNC, ID_MENU_BENUTZER_de     , ID_MENU_BENUTZER_en      );
//    MenuCtrl_PushML2_P( ID_MENU_ACHSKOPPLUNG , MENU_ITEM, NOFUNC, ID_MENU_ACHSKOPPLUNG_de , ID_MENU_ACHSKOPPLUNG_en  );
//  //MenuCtrl_PushML2_P( ID_MENU_MIXERSETUP   , MENU_ITEM, NOFUNC, ID_MENU_MIXERSETUP_de   , ID_MENU_MIXERSETUP_en    ); // nicht mehr unterstuetzt!
//    MenuCtrl_PushML2_P( ID_MENU_EASYSETUP    , MENU_ITEM, NOFUNC, ID_MENU_EASYSETUP_de    , ID_MENU_EASYSETUP_en     );

    //---------------
    // Control
    //---------------
    while( true )
    {
        MenuCtrl_Control( MENUCTRL_EVENT );
        if( MenuCtrl_GetKey() == KEY_ESC )      break;

        itemid = MenuCtrl_GetItemId();          // welcher Menu-Punkt (ID)

        switch( itemid )
        {
            case ID_MENU_FAVORITEN:     Menu_Favoriten();  break;

            case ID_MENU_KANAELE:       Menu_EditCategory( ID_MENU_KANAELE_Items       );  break;
            case ID_MENU_KONFIGURATION: Menu_EditCategory( ID_MENU_KONFIGURATION_Items );  break;
            case ID_MENU_STICK:         Menu_EditCategory( ID_MENU_STICK_Items         );  break;
            case ID_MENU_LOOPING:       Menu_EditCategory( ID_MENU_LOOPING_Items       );  break;
            case ID_MENU_HOEHE:         Menu_EditCategory( ID_MENU_HOEHE_Items         );  break;
            case ID_MENU_KAMERA:        Menu_EditCategory( ID_MENU_KAMERA_Items        );  break;
            case ID_MENU_CAMCTRL:       Menu_EditCategory( ID_MENU_CAMCTRL_Items       );  break;
            case ID_MENU_NAVICTRL:      Menu_EditCategory( ID_MENU_NAVICTRL_Items      );  break;
            case ID_MENU_AUSGAENGE:     Menu_EditCategory( ID_MENU_AUSGAENGE_Items     );  break;
            case ID_MENU_VERSCHIEDENES: Menu_EditCategory( ID_MENU_VERSCHIEDENES_Items );  break;
            case ID_MENU_GYRO:          Menu_EditCategory( ID_MENU_GYRO_Items          );  break;
            case ID_MENU_BENUTZER:      Menu_EditCategory( ID_MENU_BENUTZER_Items      );  break;
            case ID_MENU_ACHSKOPPLUNG:  Menu_EditCategory( ID_MENU_ACHSKOPPLUNG_Items  );  break;
            case ID_MENU_EASYSETUP:     Menu_EditCategory( ID_MENU_EASYSETUP_Items     );  break;

            default:                  //PKT_Message_P( *text, error, timeout, beep, clearscreen )
                                        PKT_Message_P( PSTR("nicht verfügbar"), false, 200, true, true );       // Anzeige "nicht verfï¿½gbar" (max. 2 Sekunden anzeigen)
                                        break;
        }
    }


    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();
}





//#############################################################################################
//# MK_Parameters() - Main-Function
//#############################################################################################

//--------------------------------------------------------------
// changed = MK_Parameters( setting, settingname )
//--------------------------------------------------------------
uint8_t MK_Parameters( uint8_t setting, char *settingname )
{
    int changed;
    unsigned char *org_parameters;
    uint8_t size    = paramsetSize();

    //-----------------------------------------------------------------
    // Erkennung ob Aenderungen durch den Benutzer vorgenommen wurde
    // -> das aktuelle Paramset wird gespeichert um es spaeter mit
    //    der bearbeiteten Version via memcmp zu vergleichen
    //-----------------------------------------------------------------
    org_parameters = malloc( size+1 );                                  // +1 fuer das erste settings-byte

    if( !org_parameters )
    {
        //PKT_Message_P( *text, error, timeout, beep, clearscreen )
        PKT_Message_P( PSTR("NO RAM!"), true, 800, true, true );                    // FEHLER!  (NO RAM)
        return false;
    }
    memcpy( org_parameters, (unsigned char *)(pRxData), size+1 );           // memcpy( dst, src, size))


    //-----------------------------------------------------------------
    // MK-Parameter bearbeiten
    //-----------------------------------------------------------------
    MK_Parameters_MenuMain( setting, settingname );


    //-----------------------------------------------------------------
    // Vergleich: Orginal-Daten <-> ggf. geaenderte Daten
    //-----------------------------------------------------------------
    changed = memcmp( org_parameters, (unsigned char *)(pRxData), size+1 );

    free( org_parameters );

    return( changed!=0 );
}

