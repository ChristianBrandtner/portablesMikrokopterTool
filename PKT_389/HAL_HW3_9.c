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
//# HISTORY  HAL_HW3_9.c
//#
//#
//# 10.11.2014 cebra
//# - chg: Displaybeleuchtung aktiv einschalten beim Start
//#
//# 14.06.2014 OG
//# - chg: InitHWPorts() - umgestellt auf PKT_TitlePKTlipo(false) da eine
//#        richtige LiPo-Anzeige im Startbildschirm zu diesem Zeitpunkt noch
//#        nicht moeglich ist
//# - chg: InitHWPorts() - bt_init() umgestellt auf BTM222_Initalize()
//#
//# 11.06.2014 OG
//# - fix: InitHWPorts() einstellen der Sprache geht jetzt wieder
//#
//# 10.06.2014 OG
//# - chg: InitHWPorts() - umgestellt auf Wi232_Initalize()
//#
//# 08.06.2014 OG
//# - chg: InitHWPorts() - Aufruf von InitWi232() geaendert
//#
//# 28.05.2014 OG
//# - chg: InitHWPorts() - Bearbeiten der Sprache auskommentiert weil sich
//#        Edit_generic() geaendert hat und notwendige Texte aus einem Menue
//#        bezieht welches hier in der HAL nicht vorhanden ist
//#
//# 01.04.2014 OG
//# - chg: PCB_WiFlyPatch umbenannt zu PCB_SV2RxTxPatch
//#
//# 24.03.2014 OG
//# - chg: InitHWPorts(): den Begriff "WiFly" bzw. "WiFly Patch" geaendert
//#        zu "SV2 Patch"
//#
//# 21.02.2014 OG
//# - chg: InitHWPorts() auf PKT_TitlePKTVersion() angepasst
//#
//# 20.02.2014 OG
//# - chg: InitHWPorts() Anzeige vom Startscreen ueberarbeitet
//# - chg: Code-Formatierung
//#
//# 30.01.2014 OG
//# - add: Unterstuetzung fuer USE_BLUETOOTH
//#
//# 29.01.2014 OG
//# - del: #include "display.h"
//#
//# 25.08.2013 Cebra
//# - add: #ifdef USE_TRACKING vor Servo_init
//#
//# 07.07.2013 OG
//# - add: DEBUG_SV2_EXTENSION in InitHWPorts() bzgl. Anzeige von "A:.. B:.." usw.
//#
//# 26.06.2013 OG
//# - del: Code zu USE_PKT_STARTINFO
//#
//# 26.06.2013 Cebra
//# - add: Modulumschaltung für WiFlypatch umgebaut
//# - chg: Wartezeit nach Einschalten von BT auf 3 Sekunden verlängert
//#
//# 22.06.2013 cebra
//# - chg: Fehler in CheckPCB führte zu fehlerhafter Lipomessung, alle Pins von Port A waren auf Eingang
//#
//# 13.06.2013 cebra
//# - chg: Displaybeleuchtung ohne Helligkeitssteuerung
//#
//# 15.05.2013 OG
//# - chg: define USE_PKT_STARTINFO ergaenzt (siehe main.h)
//#
//# 28.04.2013 OG
//# - add: Unterstuetzung von define USE_FONT_BIG (Layoutanpassung)
//#
//# 12.04.2013 Cebra
//# - chg: Fehler bei der Umschaltung Wi232/Kabelverbindung beim Start des PKT
//###########################################################################


#ifndef HAL_HW3_9_C_
#define HAL_HW3_9_C_

#include "cpu.h"
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdlib.h>

#include "main.h"

//#if defined HWVERSION3_9
#include "eeprom/eeprom.h"
#include "messages.h"
#include "lcd/lcd.h"
#include "uart/usart.h"
#include "uart/uart1.h"
#include "timer/timer.h"

#include "wi232/Wi232.h"
//#include "i2cmaster.h"
#include "bluetooth/bluetooth.h"
#include "bluetooth/error.h"
#include "connect.h"
#include "lipo/lipo.h"
#include "pkt/pkt.h"
#include "setup/setup.h"
#include "osd/osd.h"
#include "sound/pwmsine8bit.h"
#include "tracking/ng_servo.h"



volatile uint8_t USBBT;
volatile uint8_t U02SV2;
volatile uint8_t PCB_Version;
volatile uint8_t PCB_SV2RxTxPatch;


uint8_t PortA;
uint8_t PortB;
uint8_t PortC;
uint8_t PortD;



//--------------------------------------------------------------
//--------------------------------------------------------------
void CheckPCB(void)
{
//
  DDRA = 0x00;
  DDRB = 0x00;
  DDRC = 0x00;
  DDRD = 0x00;
  PortA = PINA;
  PortB = PINB;
  PortC = PINC;
  PortD = PIND;


  DDRC = 0x00;
  PORTC &= ~(1<<PORTC7);                // Pullup setzen
  if (PINC & (1<<PINC7) ) PCB_Version = PKT39m; else PCB_Version = PKT39x;
  PORTC &= ~(1<<PORTC0);                // Pullup setzen
  if (PINC & (1<<PINC0) ) PCB_SV2RxTxPatch = false; else PCB_SV2RxTxPatch = true;
  PORTC |= (1<<PORTC0);                 // Pullup aus

}



//--------------------------------------------------------------
//--------------------------------------------------------------
void InitHWPorts( void )  // Initialisierung der Hardware für die jeweilige Leiterplattenversion
{
    int8_t yoffs;

    CheckPCB();                                                     // Version der Leiterplatte prüfen

    if( PCB_SV2RxTxPatch )
    {
        DDRC |= (1<<DDC0)|(1<<DDC1);                                // Ausgang: C0 Reset BT, C1 Reset WiFly
        PORTC &= ~((1<<BT_Reset)|(1<<WiFly_Reset)|(1<<USB_Reset));  // Erst alle Module ausschalten
    }

    PORTA |= (1<<PORTA4)|(1<<PORTA5)|(1<<PORTA6)|(1<<PORTA7);       // Enable Pull Up for the 4 keys
    DDRA &= ~(1<<DDA4);                                             // Eingang: A4 auf Low setzen (Power On)

    if( Config.Lipomessung == true )
    {
        DDRA &= ~(1<<DDA1);                                         // Eingang: PKT Lipo Messung
        //PORTA &= ~(1<<PORTA1);
    }

    DDRB = 0xFF;                                                    // alles Ausgaenge

    PORTC |= (1<<PORTC4)|(1<<PORTC7);                               // Enable Pull Up for LBO + Summer
    DDRC |= (1<<DDC2)|(1<<DDC3)|(1<<DDC5)|(1<<DDC6|(1<<DDC7));      // Ausgang: Led2,Rs232Switch,Summer
    DDRC &= ~(1<<DDC4);                                             // Eingang: LowBat LTC1308

    if( PCB_SV2RxTxPatch )  clr_USBOn();
    else                    _BTOn();                                // Erstmal USB dektivieren, damit beim versehentlichen Einschalten USB im PC ruhig bleibt


    DDRD |= (1<<DDD4)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7);                // Ausgang: PiepserTest, Servo, Wi232-CMD und Beleuchtung
    PORTD |= (1<<PORTD6);                                           // Wi232-CMD auf High schalten
    set_D_LIGHT();                                                  // Displaybeleuchtung an


    set_V_On();                                                     // Spannung mit T3 halten
    Timer0_Init();                                                  // system

    Display_on = 1;
    USART_Init (UART_BAUD_SELECT(USART_BAUD,F_CPU));
    uart1_init (UART_BAUD_SELECT(USART_BAUD,F_CPU));                // USB
//    I2C_Init(1);
//    i2c_init();

    LCD_Init(0);                                                    // muss vor "ReadParameter" stehen
    ReadParameter();                                                // aktuelle Werte aus EEProm auslesen

    #ifdef USE_TRACKING
        servoInit(SERVO_PERIODE);
    #endif

    //servoSetDefaultPos();
    sei();

    Old_Baudrate = Config.PKT_Baudrate;
    SetBaudUart1( Config.PKT_Baudrate );
    SetBaudUart0( Config.PKT_Baudrate );

    // 10.11.2015 cebra
    // TODO cebra: diese Stelle muss bei HW_Sound überarbeitet werden
    //    if( (Config.HWSound==0) && (PCB_Version == PKT39m) )
    //    {
    //      Timer2_Init(); // Displaybeleuchtung oder Soundmodul
    //      OCR2A = Config.LCD_Helligkeit * 2.55;
    //    }




    #ifdef USE_SOUND
        if (Config.HWSound==1) InitSound();
    #endif


    LCD_Init(1);
    set_beep ( 400, 0x0080, BeepNormal);

    OSD_active = false;                                             // keine OSD Ausgabe

    ADC_Init();                                                     // ADC für Lipomessung// MartinR: verschoben


    //-------------------------
     // beim ersten Start (oder nach Reset) Sprache abfragen
    //-------------------------
    if( Config.DisplayLanguage > NUM_LANG )
    {
        Config.DisplayLanguage = 1;
      //Config.DisplayLanguage = Edit_generic( Config.DisplayLanguage,  0,  1, Language,1,   NULL,NULL);        // OG: ?? hierbei wird die falsche Sprache angezeigt - warum?
        Config.DisplayLanguage = Edit_generic( 2,  0,  1, Language,1,   NULL,NULL);
        WriteParameter();
    }


    //-------------------------
    // Anzeige Start-Screen
    //-------------------------
    PKT_TitlePKTlipo( false );                                      // Titel mit PKT-Version anzeigen (und clearscreen)

    yoffs = 11;

    if( PCB_SV2RxTxPatch )
    {
        lcdx_printp_center( 2, PSTR("SV2 Patch"), MNORMAL,  0,7);   // "SV2 Patch" (ehemals "Wifly Patch")
        yoffs = 5;
    }

    if( PCB_Version==PKT39m )
    {
        lcdx_printp_center( 1, PSTR("HW 3.9m (09.2011)"), MNORMAL,  0,yoffs);
    }

    if( PCB_Version==PKT39x )
    {
        lcdx_printp_center( 1, PSTR("HW 3.9x (11.2012)"), MNORMAL,  0,yoffs);
    }

    lcdx_printp_center( 5, strGet(BOOT1), MNORMAL,  0,2);           // "Taste 1 Sekunde"
    lcdx_printp_center( 6, strGet(BOOT2), MNORMAL,  0,4);           // "festhalten!"
    lcd_rect_round( 0, 5*8-2, 127, 2*8+8, 1, R2);                   // Rahmen


    //-------------------------
    //-------------------------

    #ifdef DEBUG_SV2_EXTENSION
        lcd_printpns_at (0, 7, PSTR("A:"),0);
             lcd_print_hex(PortA,0);
         lcd_printpns_at (5, 7, PSTR("B:"),0);
             lcd_print_hex(PortB,0);
         lcd_printpns_at (10, 7, PSTR("C:"),0);
             lcd_print_hex(PortC,0);
         lcd_printpns_at (15, 7, PSTR("D:"),0);
             lcd_print_hex(PortD,0);
    #endif


    //_delay_ms(800);
    _delay_ms(900);

    if (PINA & (1<<PINA7))    // Spannung eingeschaltet lassen
        clr_V_On();


    _delay_ms(100);

    set_beep ( 400, 0x0080, BeepNormal);
    get_key_press(KEY_ALL);

    #ifdef USE_SOUND
    if (Config.HWSound==1);
    {
        playTone(505,100,Config.Volume);
        playTone(515,100,Config.Volume);
        playTone(525,100,Config.Volume);
        playTone(535,100,Config.Volume);
        playTone(525,100,Config.Volume);
        playTone(515,100,Config.Volume);
        playTone(505,100,Config.Volume);
    }
    #endif

    //lcd_cls();  // 20.02.2014 OG


    //  servoSetPosition(1,0);
    //  _delay_ms(250);
    //        servoSetPosition(0,0);
    //        _delay_ms(250);
    //  servoSetPosition(1,400);
    //        _delay_ms(250);
    //        servoSetPosition(0,400);
    //        _delay_ms(250);
    //        servoSetPosition(1,0);
    //        _delay_ms(250);
    //        servoSetPosition(0,0);
    //        _delay_ms(250);
    //        servoSetPosition(1,400);
    //        _delay_ms(250);
    //        servoSetPosition(0,400);


    set_BTOff();
    //set_Modul_On(USB);

    if( (Config.UseWi == true) && (Config.WiIsSet == false) )
    {
        Wi232_Initalize();                          // wenn Wi232 nicht initialisiert ist, dann jetzt tun
    }


    #ifdef USE_BLUETOOTH
    if ((Config.UseBT == true) && (Config.BTIsSet == false))    // BT ausschalten
    {
        //lcd_cls();
        BTM222_Initalize();
        // set_USBOn();
    }
    bt_start();
    #endif  // end: #ifdef USE_BLUETOOTH


    if( (Config.UseWi == true) && (Config.U02SV2 == 0) )
    {
        Change_Output(Uart02Wi);    // Verbindung zu Wi232 herstellen
    }
    else
    {
        Change_Output(Uart02FC);    // Verbindung zu SV" (Kabel) herstellen
    }

}  // InitHWPorts()



//#define set_BEEP()              (PORTC &= ~(1 << Summer))               // Summer
//#define clr_BEEP()              (PORTC |=  (1 << Summer))


//--------------------------------------------------------------
//--------------------------------------------------------------
void set_BEEP(void)
{
  if (PCB_Version == PKT39m)
    PORTC &= ~(1 << Summer);               // Summer
  if (PCB_Version == PKT39x)
    PORTD &= ~(1 << SummerV2);               // Summer
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void clr_BEEP(void)
{
  if (PCB_Version == PKT39m)
    PORTC |=  (1 << Summer);               // Summer
  if (PCB_Version == PKT39x)
    PORTD |=  (1 << SummerV2);               // Summer
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void set_D_LIGHT(void)  /* Displaybeleuchtung ein*/
        {
        if (PCB_Version == PKT39m)
          {
//           if (Config.HWSound==0)
//              {
//              // PWM einschalten
//              TCCR2A |= (1 << WGM21)  | (1 << WGM20)  | (1 << COM2A1);
//              TCCR2B |= (1 << CS20);
//              }
//            else
              clr_DISPLAYLIGHT();
            }

        if (PCB_Version == PKT39x)
          {
//            if (HWSound==0)
//              {
//              // PWM einschalten
//              TCCR2A |= (1 << WGM21)  | (1 << WGM20)  | (1 << COM2A1);
//              TCCR2B |= (1 << CS20);
//              }
//            else
              clr_DISPLAYLIGHTV2();
            }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void clr_D_LIGHT(void)  /* Displaybeleuchtung aus*/
   {
        if (PCB_Version == PKT39m)
          {
//            if (Config.HWSound==0)
//              {
//              // PWM ausschalten
//              TCCR2A = 0;
//              TCCR2B = 0;
//              }
//            else

              set_DISPLAYLIGHT();
          }
        if (PCB_Version == PKT39x)
          {
//            if (HWSound==0)
//              {
//              // PWM ausschalten
//              TCCR2A = 0;
//              TCCR2B = 0;
//              }
//            else
              set_DISPLAYLIGHTV2();
          }
   }
uint8_t BTIsOn=0;



//--------------------------------------------------------------
//--------------------------------------------------------------
void set_Modul_On(uint8_t Modul)   // Umschaltfunktionen für Leiterplatte 3.9x mit WiFlypatch
{

  if (PCB_SV2RxTxPatch) PORTC &= ~((1<<BT_Reset)|(1<<WiFly_Reset)|(1<<USB_Reset));  // Erst alle Module ausschalten

  switch (Modul)
  {
   case USB:       if (PCB_SV2RxTxPatch) set_USBOn();
                     else set_USBOn();
                   BTIsOn = 0;
                   break;

   case Bluetooth: if (PCB_SV2RxTxPatch) set_BTM222On();
                     else _BTOn();
                   if (BTIsOn == 0)
                       {
                        BTIsOn = 1;
                        _delay_ms(3000);  // wait for BTM222 initialisation
                       }
                   break;
   case Wlan:     set_WiFlyOn();
                   BTIsOn = 0;
                   break;
  }


}



//--------------------------------------------------------------
//--------------------------------------------------------------
void set_BTOn(void)
{
  if (BTIsOn == 0)
    {
     if (PCB_SV2RxTxPatch) set_Modul_On(Bluetooth);
       else
         {
           _BTOn();
           BTIsOn = 1;
           _delay_ms(3000);
         }
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void set_BTOff(void)
{
    if( PCB_SV2RxTxPatch )
        set_Modul_On(USB);
    else
    {
        set_USBOn();
        BTIsOn = 0;
    }
}



#endif
//#endif // HAL_HW3_9_C_
