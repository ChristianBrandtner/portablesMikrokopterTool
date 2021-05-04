/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/****************************************************************/
/*                                                              */
/*  NG-Video 5,8GHz                                             */
/*                                                              */
/*  Copyright (C) 2011 - gebad                                  */
/*                                                              */
/*  This code is distributed under the GNU Public License       */
/*  which can be found at http://www.gnu.org/licenses/gpl.txt   */
/*                                                              */
/****************************************************************/

//############################################################################
//# HISTORY  servo_setup.c
//#
//# 27.06.2014 OG
//# - del: #include "tools.h"
//# - del: #include "mymath.h"
//#
//# 30.05.2014 OG
//# - chg: etliche Edit-Anzeigen/Funktionen auf neues Layout umgeschrieben;
//#        einige Optimierungen an Aufruffunktionen (unnoetige Menue-
//#        Interfacefunktionen entfernt)
//#
//# 13.05.2014 OG
//# - chg: Menu_Test_Start() - Variable 'range' auskommentiert
//#        wegen Warning: variable set but not used
//# - chg: Displ_PulseWidth() - Variable 'me' auskommentiert
//#        wegen Warning: variable set but not used
//# - chg: Change_Value_plmi() - Variable 'tmp_val' entfernt
//#        wegen Warning: variable set but not used
//#
//# 11.05.2014 OG
//# - chg: einge Setup-Menues umgestellt auf MenuCtrl_SetTitleFromParentItem()
//#        -> die Menues 'erben' damit ihren Titel vom aufrufenden Menuepunkt
//#
//# 30.03.2014 OG
//# - chg: ein paar englische Texte auf DE gemappt weil der Unterschied unerheblich war
//# - chg: Sprache Hollaendisch vollstaendig entfernt
//# - chg: MenuCtrl_PushML_P() umgestellt auf MenuCtrl_PushML2_P()
//#
//# 25.08.2013 Cebra
//# - add: #ifdef USE_TRACKING
//#
//# 30.05.2013 cebra
//# - chg: doppelte Texte auf #define umgestellt
//#
//# 24.05.2013 OG
//# - chg: Aufrufe von MenuCtrl_Push() umgestellt auf MenuCtrl_PushML_P()
//#
//# 22.05.2013 OG
//# - umgestellt auf menuctrl: Setup_ServoTracking(), Setup_ServoTest(),
//#   Setup_ServoTestCont(), Setup_ServoAdjust()
//# - del: include utils/menuold.h
//#
//# 17.05.2013 OG
//# - chg: umgestellt auf utils/menuold.h
//#
//# 16.04.2013 Cebra
//# - chg: PROGMEM angepasst auf neue avr-libc und GCC, prog_char depreciated
//#
//# 04.04.2013 Cebra
//# - chg: Texttuning
//#
//# 03.04.2013 Cebra
//# - chg: Holländische Menütexte
//#
//# 27.03.2013 Cebra
//# - chg: Fehler bei der Menï¿½steuerung behoben
//############################################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../cpu.h"
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "../main.h"
#include "../timer/timer.h"
#include "servo_setup.h"
#include "tracking.h"
#include "../lcd/lcd.h"
#include "../timer/timer.h"
#include "../menu.h"
#include "../messages.h"
#include "../mk-data-structs.h"
#include "../uart/usart.h"
#include "../osd/osd.h"
#include "../eeprom/eeprom.h"
#include "../setup/setup.h"
#include "../utils/menuctrl.h"
#include "../pkt/pkt.h"


#ifdef USE_TRACKING


//#include "ng_usart.h"
//#include "ng_config.h"
//#include "servo.h"
//#include "tools.h"
//#include "mk.h"
//#include "keys.h"
//#include "mymath.h"
//

//GPS_Pos_t last5pos[7];
//uint8_t error1 = 0;
//NaviData_t *naviData;
//HomePos_t MK_pos;             // Home position of station
//GPS_Pos_t currentPos;         // Current position of flying object
//int8_t satsInUse;         // Number of satelites currently in use

//uint8_t tracking   = TRACKING_MIN;
//uint8_t track_hyst = TRACKING_HYSTERESE;
//uint8_t track_tx   = 0;

//geo_t geo;
//int16_t   anglePan, angleTilt; // Servo Winkel
//uint8_t   coldstart   = 1;

uint8_t servo_nr;                        // zwischen Servo 1 und 2 wird nur mit global servo_nr unterschieden
//uint8_t   FCStatusFlags;


//############################################################################

//-----------------------------
// Setup_ServoTracking()
//-----------------------------
#define MSERVO_STEPS      1
#define MSERVO_SERVO1     2
#define MSERVO_SERVO2     3
#define MSERVO_SERVOTEST  4

static const char MSERVO_STEPS_de[]         PROGMEM = "Servoschritte";
static const char MSERVO_STEPS_en[]         PROGMEM = "servo steps";

static const char MSERVO_SERVO1_de[]        PROGMEM = "Servo 1";
#define           MSERVO_SERVO1_en          MSERVO_SERVO1_de

static const char MSERVO_SERVO2_de[]        PROGMEM = "Servo 2";
#define           MSERVO_SERVO2_en          MSERVO_SERVO2_de

static const char MSERVO_SERVOTEST_de[]     PROGMEM = "Servotest";
#define           MSERVO_SERVOTEST_en       MSERVO_SERVOTEST_de


//############################################################################

//-----------------------------
// Setup_ServoTest()
//-----------------------------
#define MSERVOT_PULS      1
#define MSERVOT_CONT      2
#define MSERVOT_SERVO     3
#define MSERVOT_FRAME     4

static const char MSERVOT_PULS_de[]         PROGMEM = "Test Pulslänge";
static const char MSERVOT_PULS_en[]         PROGMEM = "test puls width";

static const char MSERVOT_CONT_de[]         PROGMEM = "Test Fortlfd.";
static const char MSERVOT_CONT_en[]         PROGMEM = "test cont.";

static const char MSERVOT_SERVO_de[]        PROGMEM = "Servo";
#define           MSERVOT_SERVO_en          MSERVOT_SERVO_de

static const char MSERVOT_FRAME_de[]        PROGMEM = "Periode";
static const char MSERVOT_FRAME_en[]        PROGMEM = "frame";

//############################################################################

//-----------------------------
// Setup_ServoTestCont()
//-----------------------------
#define MSERVOTC_START      1
#define MSERVOTC_SINGLE     2
#define MSERVOTC_COUNT      3
#define MSERVOTC_PAUSEEND   4
#define MSERVOTC_PAUSEINC   5

static const char MSERVOTC_START_de[]       PROGMEM = "Start Test";
static const char MSERVOTC_START_en[]       PROGMEM = "start test";

static const char MSERVOTC_SINGLE_de[]      PROGMEM = "Einzelschritt";
static const char MSERVOTC_SINGLE_en[]      PROGMEM = "single step";

static const char MSERVOTC_COUNT_de[]       PROGMEM = "Anzahl Test";
static const char MSERVOTC_COUNT_en[]       PROGMEM = "number of test";

static const char MSERVOTC_PAUSEEND_de[]    PROGMEM = "Pause Endpos.";
static const char MSERVOTC_PAUSEEND_en[]    PROGMEM = "pause end pos";

static const char MSERVOTC_PAUSEINC_de[]    PROGMEM = "Pause pro Inc.";
static const char MSERVOTC_PAUSEINC_en[]    PROGMEM = "pause proc inc.";

//############################################################################

//-----------------------------
// Setup_ServoAdjust()
//-----------------------------
#define MSERVOADJ_REV      1
#define MSERVOADJ_LEFT     2
#define MSERVOADJ_RIGHT    3
#define MSERVOADJ_MID      4

static const char MSERVOADJ_REV_de[]        PROGMEM = "Reverse";
static const char MSERVOADJ_REV_en[]        PROGMEM = "reverse";

static const char MSERVOADJ_LEFT_de[]       PROGMEM = "Links";
static const char MSERVOADJ_LEFT_en[]       PROGMEM = "left";

static const char MSERVOADJ_RIGHT_de[]      PROGMEM = "Rechts";
static const char MSERVOADJ_RIGHT_en[]      PROGMEM = "right";

static const char MSERVOADJ_MID_de[]        PROGMEM = "Mitte";
static const char MSERVOADJ_MID_en[]        PROGMEM = "middle";

//############################################################################



//--------------------------------------------------------------
//--------------------------------------------------------------
void draw_input_screen( void )
{
    lcd_cls ();
    PKT_TitleFromMenuItem( true );
    lcdx_printf_at_P( 0, 2, MNORMAL, 0,0, PSTR("%S:"), MenuCtrl_GetItemText() );    // Menuetext muss im PGM sein! (aktuell keine Unterscheidung RAM/PGM)
    lcd_printp_at(0, 7, strGet(KEYLINE2), MNORMAL);
}



///************************************************************************************/
///*                                                                                                                                                                                                                                                                                                                                      */
///*      ï¿½ndern der Werte mit Tasten +,- und Anzeige                                                                                                                                                     */
///*      z.B. fï¿½r U-Offset, Batterie leer Eingabe ...                                                                                                                                            */
///*                                                                                                                                                                                                                                                                                                                                      */
///*      Parameter:                                                                                                                                                                                                                                                                                      */
///*      uint16_t                        val                                     :zu ï¿½ndernter Wert                                                                                                                                      */
///*      uint16_t min_val, max_val               :min, max Grenze Wert ï¿½ndern darf                                                                               */
///*      uint8_t posX, posY                                      :Darstellung Wert xPos, YPos auf LCD                                                            */
///*      Displ_Fnct_t    Displ_Fnct              :Index um variable Display Funktion aufzurufen                  */
///*      uint8_t                         cycle                                   :0 begrenzt Anzeige bei man_val, bzw. max_val                           */
///*                                                                                                                      :1 springt nach max_val auf min_val und umgedreht               */
///*      uint8_t                         vrepeat                         :beschleunigte Repeat-Funktion aus/ein                                                  */
///*      uint16_t Change_Value_plmi(...)         :Rï¿½ckgabe geï¿½nderter Wert                                                                                       */
///*                                                                                                                                                                                                                                                                                                                                      */
///************************************************************************************/

void Servo_tmp_Original(uint8_t track)
{
        servoSetDefaultPos();
//        tracking = track;                     // ursprï¿½nglicher Wert Tracking aus, RSSI oder GPS
//        NoTracking_ServosOff();              // Servos sind nur zum Tracking oder bei Kalibrierung eingeschaltet
//        Jump_Menu(pmenu);
}

uint8_t Servo_tmp_on(uint8_t servo_period)
{
//  uint8_t tmp_tracking = tracking;

//        tracking = 0;                                           // Servopositionierung durch tracking abschalten
//        if (tracking == TRACKING_MIN) servoInit(servo_period);  // falls aus, Servos einschalten
        servoInit(servo_period);
//        lcdGotoXY(0, 0);                                        // lcd Cursor vorpositionieren
//        return(tmp_tracking);
        return (0);
}


void Displ_Off_On(uint16_t val)
{
        if (val == 0) lcd_printp_at(17, 2, strGet(OFF), 0); else lcd_printp_at(17, 2, strGet(ON), 0);
}



uint16_t Change_Value_plmi(uint16_t val, uint16_t min_val, uint16_t max_val, uint8_t posX, uint8_t posY,Displ_Fnct_t Displ_Fnct)
{
  // >> Menueauswahl nach oben

          if (get_key_press (1 << KEY_PLUS) || get_key_long_rpt_sp ((1 << KEY_PLUS), 3))
          {
                if (val < max_val) {
                    edit = 1;
                    val++;

                }
                else
                  {
                   val = min_val;
                  }
                   Displ_Fnct(val);                                // geï¿½nderten Wert darstellen, je nach Menï¿½punkt


        }
        // >> Menueauswahl nach unten

          if (get_key_press (1 << KEY_MINUS) || get_key_long_rpt_sp ((1 << KEY_MINUS), 3))
          {
                if (val > min_val) {
                        val--;
                        edit = 1;

                }
                else
                  {
                  val = max_val;
                  }
                  Displ_Fnct(val);                                                        // geï¿½nderten Wert darstellen, je nach Menï¿½punkt


        }

        return(val);
}
//
///************************************************************************************/
///*                                                                                                              */
///*      ï¿½ndern der Werte mit Tasten +,- repetierend; (long)Entertaste und Anzeige                               */
///*                                                      z.B. fï¿½r U-Offset, Batterie leer Eingabe ...            */
///*                                                                                                              */
///*      Parameter:                                                                                              */
///*      uint16_t *val                   :zu ï¿½ndernter Wert                                                      */
///*      uint16_t min_val, max_val       :min, max Grenze Wert ï¿½ndern darf                                       */
///*      uint8_t fl_pos                  :Bit 7 beschleunigte Repeat-Funktion aus/ein                            */
///*                                       Bit 6 zyklische Werteï¿½nderung aus/ein                                  */
///*                                       Bit 4-5  z.Z. ohne Funktion                                            */
///*                                       Bit 0-3 Wert xPos auf LCD                                              */
///*      Displ_Fnct_t Displ_Fnct         :Index um variable Display Funktion aufzurufen                          */
///*      uint8_t Change_Value(...)       :Rï¿½ckgabe geï¿½ndert ergibt TRUE                                          */
///*                                                                                                              */
///************************************************************************************/
//// Bei Bedarf kï¿½nnte einfach innerhalp fl_pos auch noch pos_y (Bit 4-5) ï¿½bergeben werden
uint8_t Change_Value(uint16_t *val, uint16_t min_val, uint16_t max_val,Displ_Fnct_t Displ_Fnct)
{
    uint16_t tmp_val;

        tmp_val = *val;
        Displ_Fnct(tmp_val);               // initiale Wertdarstellung, je nach Menï¿½punkt
        while(!get_key_press(1<<KEY_ENTER) && !get_key_press(1<<KEY_ESC))
                *val = Change_Value_plmi(*val, min_val, max_val, 16,2, Displ_Fnct);


        if (*val == tmp_val) {
           edit = 0;
//            lcd_printp_at (0, 5, PSTR("Edit=0"), 0);
//            _delay_ms(500);
////            return (*val);
        }
//
        else
          {
           edit = 1;
//            lcd_printp_at (0, 5, PSTR("Edit=1"), 0);
//            _delay_ms(500);
          }

            return (tmp_val != *val);


}

uint16_t calc_range(int16_t PosProzent, int16_t min, int16_t max, int16_t mid)
{ uint16_t range;

        if (PosProzent < 0) {
                range = mid - min;
//                if (chrxs == CHRRS) {           // falls Richtung geï¿½ndert, anderen Zeichensatz laden
//                        chrxs = CHRLS;
////                        lcdWriteCGRAM_Array(lcdSpecialChrLs, 5);// LCD-Char mit Rahmensymbole vom Graph
//                }
        }
        else {
                range = max - mid;
//                if (chrxs == CHRLS) {           // falls Richtung geï¿½ndert, anderen Zeichensatz laden
////                        lcdWriteCGRAM_Array(lcdSpecialChrRs, 5);// LCD-Char mit Rahmensymbole vom Graph
//                        chrxs = CHRRS;
//                }
        }
        return(range);
}



/************************************************************************************/
/*      zeigt einen max. 3-stelligen Integerwert auf Display an                                                                                                 */
/*      Parameter:                                                                                                                                                                                                                                                                                      */
/*      uint16_t val                            :anzuzeigender Wert,                                                                                                                                                            */
/*                                                                                       uint16_t wegen Vereinheitlichung f. Funktionsaufrauf                           */
/*                                                                                                                                                                                                                                                                                                                                      */
/************************************************************************************/
void Displ_Format_Int( uint16_t val )
{
    lcdx_printf_at_P( 17, 2, MNORMAL, 0,0, PSTR("%3d"), val );
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Displ_PulseWidth( uint16_t val )
{
    int16_t PosProzent, range;

    servoSetPositionRaw( servo_nr, val);

    PosProzent = val - steps_pw[Config.sIdxSteps].mid;
    range      = calc_range(PosProzent, steps_pw[Config.sIdxSteps].min, steps_pw[Config.sIdxSteps].max, steps_pw[Config.sIdxSteps].mid);
    PosProzent = (int32_t)1000 * PosProzent / range;

    lcdx_printf_at_P( 15, 2, MNORMAL, 0,0, PSTR("%4.1d"), PosProzent );
}



/************************************************************************************/
/*      zeigt Pausenlï¿½nge der Links-, Mittel- und Rechtsposition auf Display an                                 */
/*      Parameter:                                                                                                                                                                                                                                                                                      */
/*      uint16_t val            : Zeit in 1ms * 100                                                                                                                                                                                     */
/*                                                                                                                                                                                                                                                                                                                                      */
/************************************************************************************/
void Displ_Pause(uint16_t val)
{
    lcdx_printf_at_P( 17, 2, MNORMAL, 0,0, PSTR("%1.1d"), val );
    //lcdPuts("s ");
}



/************************************************************************************/
/*      zeigt aus oder Integerwert auf Display an                                                                                                                                                               */
/*      Parameter:                                                                                                                                                                                                                                                                                      */
/*      uint16_t val            : val = 0 ==> aus, sont Integerwert                                                                                                                     */
/*                                                                                                                                                                                                                                                                                                                                      */
/************************************************************************************/
void Displ_Off_Format_Int( uint16_t val )
{
    if( val == 0 )
    {
        lcd_printp_at( 17, 2, strGet(OFF), MNORMAL);
    }
    else
    {
        Displ_Format_Int( val );
    }
}



/************************************************************************************/
/*      zeigt aus oder Pausenzeit zwischen 2 Servoschritte auf Display an                                                               */
/*      Parameter:                                                                                                                                                                                                                                                                                      */
/*      uint16_t val            : val = 0 ==> aus, sont Integerwert                                                                                                                     */
/*                                                                                                                                                                                                                                                                                                                                      */
/************************************************************************************/
void Displ_Pause_Step( uint16_t val )
{
    Displ_Off_Format_Int( val );
}



/************************************************************************************/
/*      zeigt zu testende Servonummer zur Auswahl auf Display an                                                                                                */
/*      Parameter:                                                                                                                                                                                                                                                                                      */
/*      uint16_t val                            :0 = Servo 1 oder 1 = Servo 2,                                                                                                                  */
/*                                                                                       uint16_t wegen Vereinheitlichung f. Funktionsaufrauf                           */
/*                                                                                                                                                                                                                                                                                                                                      */
/************************************************************************************/
void Displ_ServoNr( uint16_t val )
{
    Displ_Format_Int( val+1 );
}



/**************************/
/*                                                                                              */
/*                      Servos-Tests                    */
/*                                                                                              */
/**************************/
//void Menu_Servo_Test(void)
//{ uint8_t scr_sub_menu[SCROLL_MAX_6]    = {SCROLL_MAX_6 - 2, MSG_RETURN, MSG_PULSE_WIDTH, MSG_CONTINOUS, MSG_SERVO, MSG_FRAME};
//
//        Scroll_Menu(scr_sub_menu, m_pkt); // pmenu global
//        servo_nr = eeprom_read_byte(&ep_servo_nr);
//        Jump_Menu(pmenu);
//}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Menu_Test_Frame( void )
{
    uint16_t tmp_val;

    draw_input_screen();

    tmp_val = Config.servo_frame;
    if( Change_Value( &tmp_val, SERVO_PERIODE_MIN, SERVO_PERIODE_MAX, Displ_Format_Int) )
    {
        Config.servo_frame = tmp_val;
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Menu_Test_ServoNr( void )
{
    uint16_t tmp_val;

    draw_input_screen();

    tmp_val = servo_nr;

    if( Change_Value( &tmp_val, 0, 1, Displ_ServoNr) )
    {   // pmenu global; es gibt nur 0=Servo1, 1=Servo2
        servo_nr = tmp_val;
    }
}



//--------------------------------------------------------------
// Dieser Test im raw-Modus ohne Anschlagkalibrierung (normiert) z.B.: fï¿½r Modelleinstellungen ohne Empfï¿½nger
//--------------------------------------------------------------
void Menu_Test_PulseWidth( void )
{
    uint8_t tmp_tracking;
    uint16_t tmp_val;

    draw_input_screen();

    // 30.05.2014 OG: macht dieser Code Sinn?
    // Change_Value() -> 'tmp_val' und 'tmp_val' ist local - was soll mir das sagen?

    tmp_tracking = Servo_tmp_on(Config.servo_frame);
    tmp_val      = steps_pw[Config.sIdxSteps].mid;
    Change_Value( &tmp_val, steps_pw[Config.sIdxSteps].min, steps_pw[Config.sIdxSteps].max, Displ_PulseWidth); // pmenu global
    cli();
    servoInit( SERVO_PERIODE );
    sei();
    Servo_tmp_Original(tmp_tracking);
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Menu_Test_SingleStep(void)
{
    uint16_t tmp_val;

    draw_input_screen();

    tmp_val = Config.single_step;

    if( Change_Value( &tmp_val, SINGLE_STEP_MIN, SINGLE_STEP_MAX, Displ_Off_Format_Int) )
    { // pmenu global
        Config.single_step = tmp_val;
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Menu_Test_Repeat( void )
{
    uint16_t tmp_val;

    draw_input_screen();

    tmp_val = Config.repeat;

    if( Change_Value( &tmp_val, REPEAT_MIN, REPEAT_MAX, Displ_Format_Int) )
    {
        Config.repeat = tmp_val;
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Menu_Test_Pause( void )
{
    uint16_t tmp_val;

    draw_input_screen();

    tmp_val = Config.pause;

    if( Change_Value( &tmp_val, PAUSE_MIN, PAUSE_MAX, Displ_Pause) )
    {
        Config.pause = tmp_val;
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Menu_Test_Pause_Step( void )
{
    uint16_t tmp_val;

    draw_input_screen();

    tmp_val = Config.pause_step;

    if( Change_Value( &tmp_val, PAUSE_STEP_MIN, PAUSE_STEP_MAX, Displ_Pause_Step) )
    {
        Config.pause_step = tmp_val;
    }
}



int8_t calc_dir(uint8_t idx, int16_t *Position)
{ uint8_t nextIdx;
        int8_t nextDir = 1;

        nextIdx = idx;
        if ((idx + 1) < POSIDX_MAX)
                nextIdx++;
        else
                nextIdx = 0;
        if (Position[PosIdx[idx]] > Position[PosIdx[nextIdx]]) nextDir = -1;
        return(nextDir);
}



void Displ_LoopCounter( uint8_t val )
{
    lcdx_printf_at_P( 16, 2, MNORMAL, 0,0, PSTR("%4d"), val );
    //lcd_puts_at(16,2,my_itoa(val, 4, 0, 0),0);
}




// Test ï¿½ber Scalierung der Servos mit Anschlagkalibrierung
void Menu_Test_Start(void)
{
    uint8_t tmp_tracking, idx, rep;
        int8_t dir;
        int16_t sPos;
        int16_t Position[3];
        //int16_t range;

        tmp_tracking = Servo_tmp_on(Config.servo_frame);
//        lcdWriteCGRAM_Array(lcdSpecialChrLs, 8);                // LCD-Char mit Rahmensymbole vom Graph
//        chrxs = CHRLS;                                          // Flag, welche Kï¿½stchensymbole geladen
//        Displ_Title(MSG_CONTINOUS);

        lcd_cls ();
        PKT_TitleFromMenuItem( true );
        lcd_printp_at(0, 7, strGet(KEYLINE2), 0);

        Displ_LoopCounter(Config.repeat);
        Position[0] = 0;                                         // skalierte Servoposition aber unterschiedliche Schrittanzahl mï¿½glich
        Position[1] = ServoSteps()/2;
        Position[2] = ServoSteps();
        // init Einzelschritt
        idx = 0;
        dir = calc_dir(idx, Position);
        sPos = Position[PosIdx[idx]];
        idx++;
        rep = Config.repeat;

        // Test bis Ende der Wiederholungen oder irgendein Enter
        while(!get_key_press(1<<KEY_ENTER) && !get_key_press(1<<KEY_ESC)) {
                calc_range(sPos - Position[1], Position[0], Position[2], Position[1]);
                //range = calc_range(sPos - Position[1], Position[0], Position[2], Position[1]);
//                draw_bar(sPos - Position[1], range, 1); // eingerahmter Balkengraph auf 2. Display-Zeile
                servoSetPosition(servo_nr, sPos);

                if ( sPos != Position[PosIdx[idx]]) {           // Links-, Mittel- oder Rechtsposotion erreicht?
                        sPos += (Config.single_step * dir);            // variable Schrittweite subtrahieren oder addieren
                        if (((dir < 0) && (sPos < Position[PosIdx[idx]])) || ((dir > 0) && (sPos > Position[PosIdx[idx]])) || !(Config.single_step))
                                sPos = Position[PosIdx[idx]];   // ï¿½berlauf bei variabler Schrittweite berï¿½cksichtigen oder Einzelschritt
                        Delay_ms(Config.servo_frame + 1 + Config.pause_step);// Bei Schrittweite um 1 wï¿½rden welche ï¿½bersprungen, zusï¿½tzlich pro Servoschritt verzï¿½gert
                }
                else {
                        dir = calc_dir(idx, Position);            // Richtungsï¿½nderung
                        if (idx < (POSIDX_MAX - 1)) {
                                if (idx == 0) {
                                        rep--;                    // bei jeden vollen Durchlauf Wiederholzï¿½hler verringern
                                        Displ_LoopCounter(rep);
                                }
                                idx++;                          // Index fï¿½r nï¿½chsten Positionswert ==> Array PosIdx[] bestimmt Anschlagreihenfolge
                        }
                        else
                                idx = 0;

                        _delay_ms( Config.pause*100 );                    // variable Pause bei Links-, Mittel- und Rechtsposotion Mindestzeit 400ms (Servolauf)
                }
        }

//        lcdClear();
//        if (pmenu[0] == '\0')
//                Displ_Main_Disp();
//        else
//                return_m_pkt(strlen(pmenu));                     // um bei Rï¿½cksprung auf ursprï¿½nglichen Menï¿½punkt zeigen oder Displ_Main_Disp()
//        lcdWriteCGRAM_Array(lcdSpecialChr, 7);                  // LCD-Char fï¿½r Bargraph zurï¿½ckschreiben
        cli();
        servoInit(SERVO_PERIODE);
        sei();
        Servo_tmp_Original(tmp_tracking);
}





/********************************************************************************/
/*      zeigt Servo-Anschlagposition links auf Display an                       */
/*      mit sofortiger Wirkung auf Servo                                        */
/*      Parameter:                                                              */
/*      uint16_t val            :anzuzeigender Wert,                            */
/*      uint16_t wegen Vereinheitlichung f. Funktionsaufrauf                    */
/*                                                                              */
/********************************************************************************/
void Displ_Servo_Min(uint16_t val)
{
    uint16_t steps = 0;

    write_ndigit_number_s( 15, 2, val, 5, 0,0);
    servoSet_min(servo_nr, val);                                                    // Wert setzen damit nachfolgend die
    if (Config.servo[servo_nr].rev) steps = ServoSteps();
    servoSetPosition(servo_nr, steps);                                              // ï¿½nderung direkt am Servo sichtbar ist
}


/************************************************************************************/
/*      zeigt Servo-Anschlagposition rechts auf Display an                                                                                                                      */
/*      mit sofortiger Wirkung auf Servo                                                                                                                                                                                                */
/*      Parameter:                                                                                                                                                                                                                                                                                      */
/*      uint16_t val                            :anzuzeigender Wert,                                                                                                                                                            */
/*      uint16_t wegen Vereinheitlichung f. Funktionsaufrauf                           */
/*                                                                                                                                                                                                                                                                                                                          */
/************************************************************************************/
void Displ_Servo_Max(uint16_t val)
{
    uint16_t steps = ServoSteps();

    write_ndigit_number_u( 15, 2, val, 5, 0,0);
    servoSet_max(servo_nr, val);                                                    // Wert setzen damit nachfolgend die
    if (Config.servo[servo_nr].rev) steps = 0;
    servoSetPosition(servo_nr, steps);                                              // ï¿½nderung direkt am Servo sichtbar ist
}



/************************************************************************************/
/*      zeigt Servo-Anschlagposition Mitte auf Display an                                                                                                                               */
/*      mit sofortiger Wirkung auf Servo                                                                                                                                                                                                */
/*      Parameter:                                                                                                                                                                                                                                                                                      */
/*      uint16_t val                            :anzuzeigender Wert,                                                                                                                                                            */
/*      uint16_t wegen Vereinheitlichung f. Funktionsaufrauf                           */
/*                                                                                                                                                                                                                                                                                                                                      */
/************************************************************************************/
void Displ_Servo_Mid( uint16_t val )
{
    int16_t mid_val;

    mid_val = val - ServoSteps()/2;

    lcdx_printf_at_P( 17, 2, MNORMAL, 0,0, PSTR("%3d"), mid_val );

    servoSet_mid(servo_nr, val);                    // Wert setzen damit nachfolgend die
    servoSetPosition(servo_nr, ServoSteps()/2);     // ï¿½nderung direkt am Servo sichtbar ist
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Servo_rev( void )
{
    uint16_t tmp_val;
    uint8_t tmp_tracking;

    draw_input_screen();

    tmp_tracking = Servo_tmp_on(SERVO_PERIODE);
    tmp_val = Config.servo[servo_nr].rev;

    if( Change_Value(&tmp_val , 0, 1, Displ_Off_On) )
    {   //reverse gibt es nur 0=off, 1=on
        Config.servo[servo_nr].rev = tmp_val ;
        servoSet_rev(servo_nr, tmp_val );
    }
    Servo_tmp_Original(tmp_tracking);
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Servo_left( void )
{
    uint16_t tmp_val;
    uint8_t tmp_tracking;

    draw_input_screen();

    tmp_tracking = Servo_tmp_on(SERVO_PERIODE);
    servoSetPosition(servo_nr, ServoSteps());                               // Linkssanschlag um Kalibrierung am Servo zu sehen
    tmp_val = Config.servo[servo_nr].max;

    if( Change_Value(&tmp_val , servo_limit[Config.sIdxSteps][LEFT].min, servo_limit[Config.sIdxSteps][LEFT].max, Displ_Servo_Max) )
    {
        Config.servo[servo_nr].max = tmp_val;
        servoSet_mid(servo_nr, Config.servo[servo_nr].mid);            // Mittelposition muss sich bei Ausschlagsï¿½nderung verschieben
    }
    Servo_tmp_Original(tmp_tracking);
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Servo_right( void )
{
    uint16_t tmp_val;
    uint8_t tmp_tracking;

    draw_input_screen();

    tmp_tracking = Servo_tmp_on(SERVO_PERIODE);
    servoSetPosition(servo_nr, 0);                                                                          // Rechtsanschlag um Kalibrierung am Servo zu sehen
    tmp_val = Config.servo[servo_nr].min;

    if( Change_Value(&tmp_val , servo_limit[Config.sIdxSteps][RIGHT].min, servo_limit[Config.sIdxSteps][RIGHT].max, Displ_Servo_Min) )
    {
        Config.servo[servo_nr].min = tmp_val;
        servoSet_mid(servo_nr, Config.servo[servo_nr].mid);            // Mittelposition muss sich bei Ausschlagsï¿½nderung verschieben
    }
    Servo_tmp_Original(tmp_tracking);
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Servo_middle( void )
{
    uint16_t tmp_val;
    uint8_t tmp_tracking;

    draw_input_screen();

    tmp_tracking = Servo_tmp_on(SERVO_PERIODE);
    servoSetPosition(servo_nr, ServoSteps()/2);                     // Mittelposition um Kalibrierung am Servo zu sehen
    tmp_val = Config.servo[servo_nr].mid;

    if( Change_Value(&tmp_val , servo_limit[Config.sIdxSteps][MIDDLE].min, servo_limit[Config.sIdxSteps][MIDDLE].max, Displ_Servo_Mid) )
    {
        Config.servo[servo_nr].mid = tmp_val;
    }
    Servo_tmp_Original(tmp_tracking);
}



void Servo_NewValues(uint8_t idx_presc)
{
        for (uint8_t i = 0; i < SERVO_NUM_CHANNELS; i++) {
                if (idx_presc == STEPS_255) {                   // Werte umrechnen fï¿½r Prescaler = 256
                    Config.servo[i].min /= 4;
                    Config.servo[i].max /= 4;
                    Config.servo[i].mid /= 4;
                }
                else {                                                                                                          // Werte umrechnen fï¿½r Prescaler = 64
                    Config.servo[i].min *= 4;
                    Config.servo[i].max *= 4;
                    Config.servo[i].mid    = (Config.servo[i].mid + 1) * 4 - 1;
                }
                servoSet_min(i, Config.servo[i].min);
                servoSet_max(i, Config.servo[i].max);
                servoSet_mid(i, Config.servo[i].mid);
//                eeprom_write_block(&servo[i],&ep_servo[i],sizeof(servo_t));
        }
        // Vorberechnung von ServoChannels[channel].duty
        servoSetDefaultPos();                                                           // Ausgangsstellung beider Servos

}

/************************************************************************************/
/*      zeigt Servoschritte zur Auswahl auf Display an                                                                                                                                  */
/*      Parameter:                                                                                                                                                                                                                                                                                      */
/*      uint16_t val                            :0 = 255 oder 1 = 1023,                                                                                                                                                 */
/*                                                                                       uint16_t wegen Vereinheitlichung f. Funktionsaufrauf                           */
/*                                                                                                                                                                                                                                                                                                                                      */
/************************************************************************************/
void Displ_Servo_Steps(uint16_t val)
{
        if (val==0)
          lcd_puts_at(16,2,INTERNAT_STEPS_255,0 );
        else
          lcd_puts_at(16,2,INTERNAT_STEPS_1023,0 );

}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Menu_Servo_Steps(void)
{
    uint16_t tmp_val;

    draw_input_screen();

    tmp_val = Config.sIdxSteps;

    if( Change_Value( &tmp_val, STEPS_255, STEPS_1023,Displ_Servo_Steps) )
    {
        cli();
        Config.sIdxSteps = tmp_val;
        Servo_NewValues( Config.sIdxSteps );                             // hier ist der neue Index anzugeben!
        servoInit(SERVO_PERIODE);
        sei();


    }
}



//--------------------------------------------------------------
// Setup_ServoAdjust()
//--------------------------------------------------------------
void Setup_ServoAdjust( uint8_t servo )
{
    servo_nr = servo;

    //---------------
    // Create
    //---------------
    MenuCtrl_Create();
    MenuCtrl_SetTitleFromParentItem();      // "Servo 1" oder "Servo 1"
    //MenuCtrl_SetTitle_P( PSTR("Servo Adjust") );


    //---------------
    // Menuitems
    //---------------
    MenuCtrl_PushML2_P( MSERVOADJ_REV   , MENU_ITEM, &Servo_rev    , MSERVOADJ_REV_de   , MSERVOADJ_REV_en   );
    MenuCtrl_PushML2_P( MSERVOADJ_LEFT  , MENU_ITEM, &Servo_left   , MSERVOADJ_LEFT_de  , MSERVOADJ_LEFT_en  );
    MenuCtrl_PushML2_P( MSERVOADJ_RIGHT , MENU_ITEM, &Servo_right  , MSERVOADJ_RIGHT_de , MSERVOADJ_RIGHT_en );
    MenuCtrl_PushML2_P( MSERVOADJ_MID   , MENU_ITEM, &Servo_middle , MSERVOADJ_MID_de   , MSERVOADJ_MID_en   );

    //---------------
    // Control
    //---------------
    MenuCtrl_Control( MENUCTRL_EVENT );


    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();
}



//--------------------------------------------------------------
// Setup_ServoTestCont()
//--------------------------------------------------------------
void Setup_ServoTestCont( void )
{
    //---------------
    // Create
    //---------------
    MenuCtrl_Create();
    MenuCtrl_SetTitleFromParentItem();      // "Test fortlfd."
    //MenuCtrl_SetTitle_P( PSTR("Servotest Cont.") );

    //---------------
    // Menuitems
    //---------------
    MenuCtrl_PushML2_P( MSERVOTC_START    , MENU_ITEM, &Menu_Test_Start      , MSERVOTC_START_de    , MSERVOTC_START_en    );
    MenuCtrl_PushML2_P( MSERVOTC_SINGLE   , MENU_ITEM, &Menu_Test_SingleStep , MSERVOTC_SINGLE_de   , MSERVOTC_SINGLE_en   );
    MenuCtrl_PushML2_P( MSERVOTC_COUNT    , MENU_ITEM, &Menu_Test_Repeat     , MSERVOTC_COUNT_de    , MSERVOTC_COUNT_en    );
    MenuCtrl_PushML2_P( MSERVOTC_PAUSEEND , MENU_ITEM, &Menu_Test_Pause      , MSERVOTC_PAUSEEND_de , MSERVOTC_PAUSEEND_en );
    MenuCtrl_PushML2_P( MSERVOTC_PAUSEINC , MENU_ITEM, &Menu_Test_Pause_Step , MSERVOTC_PAUSEINC_de , MSERVOTC_PAUSEINC_en );

    //---------------
    // Control
    //---------------
    MenuCtrl_Control( MENUCTRL_EVENT );

    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();
}



//--------------------------------------------------------------
// Setup_ServoTest()
//--------------------------------------------------------------
void Setup_ServoTest( void )
{
    //---------------
    // Create
    //---------------
    MenuCtrl_Create();
    MenuCtrl_SetTitleFromParentItem();      // "Servotest"

    //---------------
    // Menuitems
    //---------------
    MenuCtrl_PushML2_P( MSERVOT_PULS  , MENU_ITEM, &Menu_Test_PulseWidth , MSERVOT_PULS_de  , MSERVOT_PULS_en  );
    MenuCtrl_PushML2_P( MSERVOT_CONT  , MENU_SUB , &Setup_ServoTestCont  , MSERVOT_CONT_de  , MSERVOT_CONT_en  );
    MenuCtrl_PushML2_P( MSERVOT_SERVO , MENU_ITEM, &Menu_Test_ServoNr    , MSERVOT_SERVO_de , MSERVOT_SERVO_en );
    MenuCtrl_PushML2_P( MSERVOT_FRAME , MENU_ITEM, &Menu_Test_Frame      , MSERVOT_FRAME_de , MSERVOT_FRAME_en );

    //---------------
    // Control
    //---------------
    MenuCtrl_Control( MENUCTRL_EVENT );

    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();
}



//--------------------------------------------------------------
// Setup_ServoTracking()
//--------------------------------------------------------------
void Setup_ServoTracking( void )
{
    uint8_t itemid;

    //---------------
    // Create
    //---------------
    MenuCtrl_Create();
    MenuCtrl_SetTitleFromParentItem();      // "Tracking"

    //---------------
    // Menuitems
    //---------------
    MenuCtrl_PushML2_P( MSERVO_STEPS     , MENU_ITEM, &Menu_Servo_Steps , MSERVO_STEPS_de     , MSERVO_STEPS_en     );
    MenuCtrl_PushML2_P( MSERVO_SERVO1    , MENU_SUB , NOFUNC            , MSERVO_SERVO1_de    , MSERVO_SERVO1_en    );
    MenuCtrl_PushML2_P( MSERVO_SERVO2    , MENU_SUB , NOFUNC            , MSERVO_SERVO2_de    , MSERVO_SERVO2_en    );
    MenuCtrl_PushML2_P( MSERVO_SERVOTEST , MENU_SUB , &Setup_ServoTest  , MSERVO_SERVOTEST_de , MSERVO_SERVOTEST_en );


    //---------------
    // Control
    //---------------
    while( true )
    {
        MenuCtrl_Control( MENUCTRL_EVENT );

        if( MenuCtrl_GetKey() == KEY_ESC )  break;      // ENDE

        itemid = MenuCtrl_GetItemId();                  // welcher Menu-Punkt (ID)

        if( itemid == MSERVO_SERVO1     ) { Setup_ServoAdjust(0); }
        if( itemid == MSERVO_SERVO2     ) { Setup_ServoAdjust(1); }
    }

    //---------------
    // Destroy
    //---------------
    MenuCtrl_Destroy();
}




//--------------------------------------------------------------

//void start_tracking(void)
//{
//  #define TIMEOUT 200 // 2 sec
//
//  uint16_t old_anglePan = 0;
//  uint16_t old_angleTilt = 0;
//
//  //uint16_t old_hh = 0;
//  uint8_t flag;
//  uint8_t tmp_dat;
//
//  lcd_cls ();
//  //lcd_printpns_at(0, 0, PSTR("start_tracking   "), 2);
//
//  //lcd_printpns_at(0, 1, PSTR("ab jetzt Tracking"), 0);
//
//  lcd_ecircle(22, 35, 16, 1);
//  lcd_ecircle(88, 35, 16, 1);
//  lcd_putc (10, 1, 0x1e, 0);  // degree symbol
//  lcd_putc (20, 1, 0x1e, 0);  // degree symbol
////    lcd_printp_at (0, 7, PSTR(KEY_LINE_1), 0);
//        lcd_puts_at(0, 7, strGet(KEYLINE1), 0);
//  SwitchToNC();
//
//  mode = 'O';
//
//  // disable debug...
//  //  RS232_request_mk_data (0, 'd', 0);
//  tmp_dat = 0;
//  SendOutData ('d', ADDRESS_ANY, 1, &tmp_dat, 1);
//
//  // request OSD Data from NC every 100ms
//  //  RS232_request_mk_data (1, 'o', 100);
//  tmp_dat = 10;
//  SendOutData ('o', ADDRESS_NC, 1, &tmp_dat, 1);
//
//  if (rxd_buffer_locked)
//      {
//          timer = TIMEOUT;
//          Decode64 ();
//          naviData = (NaviData_t *) pRxData;
//
//          if(error1 == 1)
//              lcd_cls();
//      }
//
//  GPS_Pos_t currpos;
//  currpos.Latitude = naviData->CurrentPosition.Latitude;
//  currpos.Longitude = naviData->CurrentPosition.Longitude;
//
//  flag = 0;
//  timer = TIMEOUT;
//  abo_timer = ABO_TIMEOUT;
//
//  coldstart = 1;
//
//  do
//  {
//      if (rxd_buffer_locked)
//      {
//          timer = TIMEOUT;
//          Decode64 ();
//          naviData = (NaviData_t *) pRxData;
//
//
////CB          uint8_t FCStatusFlag = naviData->FCFlags;
//                        uint8_t FCStatusFlag = naviData->FCStatusFlags;
//          //write_ndigit_number_u (0, 0, FCStatusFlag);
//
//          Tracking_GPS();
//
//          //uint16_t heading_home = (naviData->HomePositionDeviation.Bearing + 360 - naviData->CompassHeading) % 360;
//
//          // alte Linien lï¿½schen
//          //lcd_ecirc_line (22, 35, 15, old_hh, 0);
//          //old_hh = heading_home;
//          lcd_ecirc_line (22, 35, 15, old_anglePan, 0);
//          old_anglePan = anglePan;
//          lcd_ecirc_line (88, 35, 15, old_angleTilt, 0);
//          old_angleTilt = angleTilt;
//
//          lcd_ecirc_line (22, 35, 15, anglePan, 1);
//          write_ndigit_number_u (7, 1, anglePan, 3, 0,0);
//          lcd_ecirc_line (88, 35, 15, angleTilt, 1);
//          write_ndigit_number_u (17, 1, angleTilt, 3, 0,0);
//
//          rxd_buffer_locked = FALSE;
//
//          if (!abo_timer)
//          {   // renew abo every 3 sec
//              // request OSD Data from NC every 100ms
//              //  RS232_request_mk_data (1, 'o', 100);
//              tmp_dat = 10;
//              SendOutData ('o', ADDRESS_NC, 1, &tmp_dat, 1);
//
//              abo_timer = ABO_TIMEOUT;
//          }
//      }
//
//      if (!timer)
//      {
//          OSD_Timeout(flag);
//          flag = 0;
//      }
//  }
//  while(!get_key_press (1 << KEY_ESC));
//
//  //lcd_cls();
//  //return;
//}

//--------------------------------------------------------------
//
//void conect2at_unit(void)
//{
//  lcd_cls ();
//  lcd_printpns_at(0, 0, PSTR("conect2at_unit   "), 2);
//
//  lcd_printpns_at(0, 3, PSTR("work in progress ;)"), 2);
//
////    lcd_printp_at (0, 7, PSTR(KEY_LINE_1), 0);
//        lcd_puts_at(0, 7, strGet(KEYLINE1), 0);
//
//  while(!get_key_press (1 << KEY_ESC));
//
//  lcd_cls();
//  return;
//}
//
////--------------------------------------------------------------
//
//void conect2gps_ser (void)
//{
//  lcd_cls ();
//  lcd_printpns_at(0, 0, PSTR("conect2gps_ser   "), 2);
//
//  lcd_printpns_at(0, 3, PSTR("work in progress ;)"), 2);
//
////    lcd_printp_at (0, 7, PSTR(KEY_LINE_1), 0);
//        lcd_puts_at(0, 7, strGet(KEYLINE1), 0);
//  while(!get_key_press (1 << KEY_ESC));
//
//  lcd_cls();
//  return;
//}
//
////--------------------------------------------------------------
//
//void conect2gps_bt (void)
//{
//  lcd_cls ();
//  lcd_printpns_at(0, 0, PSTR("conect2gps_bt    "), 2);
//
//  lcd_printpns_at(0, 3, PSTR("work in progress ;)"), 2);
//
////    lcd_printp_at (0, 7, PSTR(KEY_LINE_1), 0);
//        lcd_puts_at(0, 7, strGet(KEYLINE1), 0);
//  while(!get_key_press (1 << KEY_ESC));
//
//  lcd_cls();
//  return;
//}

//--------------------------------------------------------------

//void conect2gps_menu(void)
//{
//  uint8_t ii = 0;
//  uint8_t Offset = 0;
//  uint8_t size = ITEMS_CONECT_GPS;
//  uint8_t dmode = 0;
//  uint8_t target_pos = 1;
//  uint8_t val = 0;
//
//  while(1)
//  {
//      lcd_cls ();
//      lcd_printpns_at(0, 0, PSTR("conect2gps_menu  "), 2);
////        lcd_printp_at (0, 7, PSTR(KEY_LINE_1), 0);
//                lcd_puts_at(0, 7, strGet(KEYLINE1), 0);
//      while(2)
//      {
//          ii = 0;
//          if(Offset > 0)
//          {
//              lcd_printp_at(1,1, PSTR("\x12"), 0);
//          }
//          for(ii = 0;ii < 6 ; ii++)
//          {
//              if((ii+Offset) < size)
//              {
//                  lcd_printp_at(3,ii+1,conect_gps_menuitems[ii+Offset], 0);
//              }
//              if((ii == 5)&&(ii+Offset < (size-1)))
//              {
//                  lcd_printp_at(1,6, PSTR("\x13"), 0);
//              }
//          }
//          if(dmode == 0)
//          {
//              if(Offset == 0)
//              {
//                  if(size > 6)
//                  {
//                      val = menu_choose2 (1, 5, target_pos,0,1);
//                  }
//                  else
//                  {
//                      val = menu_choose2 (1, size, target_pos,0,0);
//                  }
//              }
//              else
//              {
//                  val = menu_choose2 (2, 5, target_pos,1,1);
//              }
//          }
//          if(dmode == 1)
//          {
//              if(Offset+7 > size)
//              {
//                  val = menu_choose2 (2, 6, target_pos,1,0);
//              }
//              else
//              {
//                  val = menu_choose2 (2, 5, target_pos,1,1);
//              }
//          }
//          if(val == 254)
//          {
//              Offset++;
//              dmode = 1;
//              target_pos = 5;
//          }
//          else if(val == 253)
//          {
//              Offset--;
//              dmode = 0;
//              target_pos = 2;
//          }
//          else if(val == 255)
//          {
//              return;
//          }
//          else
//          {
//              break;
//          }
//      }
//      target_pos = val;
//
//      if((val+Offset) == 1 )
//          conect2gps_ser();
//      if((val+Offset) == 2 )
//          conect2gps_bt();
//  }
//}
//--------------------------------------------------------------
//void tracking_menu(void)
//{
//  uint8_t ii = 0;
//  uint8_t Offset = 0;
//  uint8_t size = ITEMS_AT;
//  uint8_t dmode = 0;
//  uint8_t target_pos = 1;
//  uint8_t val = 0;
//
//  while(1)
//  {
//      lcd_cls ();
//      lcd_printpns_at(1, 0, PSTR("Tracking Men\x06 V.01    "), 2);
////        lcd_printp_at (0, 7, PSTR(KEY_LINE_1), 0);
//                lcd_puts_at(0, 7, strGet(KEYLINE1), 0);
//      while(2)
//      {
//          ii = 0;
//          if(Offset > 0)
//          {
//              lcd_printp_at(1,1, PSTR("\x12"), 0);
//          }
//          for(ii = 0;ii < 6 ; ii++)
//          {
//              if((ii+Offset) < size)
//              {
//                  lcd_printp_at(3,ii+1,at_menuitems[ii+Offset], 0);
//              }
//              if((ii == 5)&&(ii+Offset < (size-1)))
//              {
//                  lcd_printp_at(1,6, PSTR("\x13"), 0);
//              }
//          }
//          if(dmode == 0)
//          {
//              if(Offset == 0)
//              {
//                  if(size > 6)
//                  {
//                      val = menu_choose2 (1, 5, target_pos,0,1);
//                  }
//                  else
//                  {
//                      val = menu_choose2 (1, size, target_pos,0,0);
//                  }
//              }
//              else
//              {
//                  val = menu_choose2 (2, 5, target_pos,1,1);
//              }
//          }
//          if(dmode == 1)
//          {
//              if(Offset+7 > size)
//              {
//                  val = menu_choose2 (2, 6, target_pos,1,0);
//              }
//              else
//              {
//                  val = menu_choose2 (2, 5, target_pos,1,1);
//              }
//          }
//          if(val == 254)
//          {
//              Offset++;
//              dmode = 1;
//              target_pos = 5;
//          }
//          else if(val == 253)
//          {
//              Offset--;
//              dmode = 0;
//              target_pos = 2;
//          }
//          else if(val == 255)
//          {
//              return;
//          }
//          else
//          {
//              break;
//          }
//      }
//      target_pos = val;
//
//      if((val+Offset) == 1 )
//          test_servo_menu();
//      if((val+Offset) == 2 )
//          adjust_servo_menu();
//      if((val+Offset) == 3 )
//          show_angle();
//      if((val+Offset) == 4 )
////TODO:           start_tracking();
//      if((val+Offset) == 5 )
//          conect2at_unit();
//      if((val+Offset) == 6 )
//          conect2gps_menu();
//  }
//}

//--------------------------------------------------------------
// kapeschi Ant.Treking Funktionen
//--------------------------------------------------------------

// Berechnung von Distanz und Winkel aus GPS-Daten home(MK eingeschaltet)
// zur aktuellen Position(nach Motorstart)
//geo_t calc_geo(HomePos_t *home, GPS_Pos_t *pos)
//{ int32_t lat1, lon1, lat2, lon2;
//  int32_t d1, dlat;
//  geo_t geo;
//
//  lon1 = MK_pos.Home_Lon;
//  lat1 = MK_pos.Home_Lat;
//  lon2 = pos->Longitude;
//  lat2 = pos->Latitude;
//
//  // Formel verwendet von http://www.kompf.de/gps/distcalc.html
//  // 111.3 km = Abstand zweier Breitenkreise und/oder zweier Lï¿½ngenkreise am ï¿½quator
//  // es wird jedoch in dm Meter weiter gerechnet
//  // (tlon1 - tlon2)/10) sonst uint32_t-ï¿½berlauf bei cos(0) gleich 1
//  d1   = (1359 * (int32_t)(c_cos_8192((lat1 + lat2) / 20000000)) * ((lon1 - lon2)/10))/ 10000000;
//  dlat = 1113 * (lat1 - lat2) / 10000;
//  geo.bearing = (my_atan2(d1, dlat) + 540) % 360; // 360 +180 besserer Vergleich mit MkCockpit
//  geo.distance = sqrt32(d1 * d1 + dlat * dlat);
//  return(geo);
//}

//void do_tracking(void)
//{ //static uint8_t hysteresis = 0;
//  // aus MkCockpit http://forum.mikrokopter.de/topic-post216136.html#post216136
//  // (4 * (........))/5 ==> Wichtung Luftdruck-Hï¿½he zu GPS
//  currentPos.Altitude = MK_pos.Home_Alt + (4000 * (int32_t)(naviData->Altimeter) / AltFaktor + currentPos.Altitude - MK_pos.Home_Alt) / 5;
//
//  geo = calc_geo(&MK_pos, &currentPos);
//  angleTilt = RAD_TO_DEG * (double)atan2((double)(currentPos.Altitude - MK_pos.Home_Alt) / 1000, geo.distance);
//  //if (geo.distance < 4 || (geo.distance < 6 && hysteresis)) {       // < 4m ==> Pan-Servo in Mittelstellung. Hysterese bis 6m, damit Servo im Grenzbereich nicht wild rumschlï¿½gt
//      //geo.bearing = MK_pos.direction;
//      //angleTilt = 0;
//      //hysteresis = 1;
//  //}
//  //else {
//      //hysteresis = 0;
//  //}
////
//  //// egal wo der ï¿½bergangspunkt 359, 360, 1grd ist => Winkelï¿½bergangspunkt auf 0 bzw. 180grd des Servos bringen
//  //// 360 grd negative Winkelwerte als positive
//  anglePan = (geo.bearing + 450 - MK_pos.direction) % 360; // 450 = 360 + 90
//
//  //if (angleTilt < 0) angleTilt = 0;
//  //if (angleTilt > 180) angleTilt = 180;
////
//  //if (anglePan >= 180) {                // zwecks 360grd-Abdeckung flipt Pan-/Tilt-Servo
//      //anglePan = anglePan - 180;
//      //angleTilt = 180 - angleTilt;
//      //
//  //}
////angleTilt = 180;
////angleTilt = 180;
//
////    servoSetAngle(0, anglePan);
////    servoSetAngle(1, angleTilt);
//}


/****************************************************************/
/*                                                              */
/*  MK GPS Tracking                                             */
/*                                                              */
/****************************************************************/

// MK OSD-Daten lesen und verifizieren
//uint8_t OSD_Data_valid(NaviData_t **navi_data)
//{ uint8_t ret = 0;
    //char *tx_osd = {"#co?]==EH\r"};
////    char interval[2] = {10, '\0'};
    //
    //if (rx_line_decode('O')) {                    // OSD-Datensatz prï¿½fen/dekodieren
        ////*navi_data = (NaviData_t*)data_decode;  // dekodierte Daten mit Struktur OSD-Daten versehen
        //if (rx_timeout < RX_TIME_OLD) {           // GPS-Daten nicht zu alt und ok.
            //currentPos = (*navi_data)->CurrentPosition;
            //if ((*navi_data)->NCFlags & NC_FLAG_GPS_OK)
                //ret = 1;
            //// aus MkCockpit http://forum.mikrokopter.de/topic-post216136.html#post216136
            //// (4 * (........))/5 ==> Wichtung Luftdruck-Hï¿½he zu GPS
            //currentPos.Altitude = MK_pos.Home_Alt + (4000 * (int32_t)((*navi_data)->Altimeter) / AltFaktor + currentPos.Altitude - MK_pos.Home_Alt) / 5;
            //satsInUse = (*navi_data)->SatsInUse;
        //}
    //}
    //// ca. 210ms keine OSD-Daten empfangen ==> sende neue Anforderung an MK
////    if ((track_tx) && (rx_timeout > RX_TIMEOUT)) tx_Mk(NC_ADDRESS, 'o', interval, 1); //    420 * 0.5ms interval
    //if ((track_tx) && (rx_timeout > RX_TIMEOUT)) SendOutData(tx_osd); // 420 * 0.5ms interval
    //return ret;
//}
//

// MK eingeschaltet und GPS-ok, danach Motoren gestartet ==> Berechnung horizontaler/vertikaler Servowinkel
// Hauptprogramm von GPS Antennen-Nachfï¿½hrung
//void Tracking_GPS(void)
//{ //NaviData_t *navidata;
//  static uint8_t track_running = 0;
//
//  if (!track_running)
//  {
//    //track_running = 1;  // verhindert doppelten Aufruf, wenn in Eingabeschleife Menu_MK_BatteryChangeNr() !!!
//      //if (OSD_Data_valid(&naviData)) {
//          if (coldstart)
//          {
//              //// erst nach Neustart NGVideo und beim Motorstart werden Daten vom MK ï¿½bernommen
//              //if (naviData->FCFlags & FC_FLAG_MOTOR_START)
//              //{
//                  MK_pos.Home_Lon  = (double)naviData->HomePosition.Longitude / 10000000.0;
//                  MK_pos.Home_Lat  = (double)naviData->HomePosition.Latitude   / 10000000.0;
//                  MK_pos.Home_Lon7 = naviData->HomePosition.Longitude;
//                  MK_pos.Home_Lat7 = naviData->HomePosition.Latitude;
//                  MK_pos.Home_Alt  = naviData->HomePosition.Altitude;
//                  MK_pos.direction = naviData->CompassHeading;
//                  coldstart        = 0;
//              //}
//          //}
//          //else {
//              //do_tracking();
//          }
//      //}
//      track_running = 0;
//  }
//  do_tracking();
//}
#endif
