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
//# HISTORY  HAL_HW3_9.h
//# 
//# 01.04.2014 OG
//# - chg: PCB_WiFlyPatch umbenannt zu PCB_SV2RxTxPatch
//# 
//# 26.06.2013 Cebra
//# - add: defines für WyFlypatch Port C0 + C1
//#
//# 17.05.2013 OG
//# - add: defines: KEY_ENTER_LONG, KEY_ESC_LONG, KEY_PLUS_LONG, KEY_MINUS_LONG
//############################################################################


#ifndef HAL_HW3_9_H_
#define HAL_HW3_9_H_



//#define PIEPSER_NERVT  // Summer zu Testzwecken ganz Ausschalten

#define PKT39m  1
#define PKT39x  2

#define USB        1
#define Bluetooth  2
#define Wlan       3



// Hardware 3.9 Portbelegung
#define KEY_PIN             PINA // Port A als Input

#define Touch0              PA0  // Pin 37
#define Touch1              PA1  // Pin 36
#define Touch2              PA2  // Pin 35
#define Touch3              PA3  // Pin 34
#define KEY_EXT                         PINA3  // MartinR

#define Key1                PA4  // Pin 33
#define Key2                PA5  // Pin 32
#define Key3                PA6  // Pin 31
#define Key4                PA7  // Pin 30

#define USB2Wi              PB0  // Pin 40  aktiv low > IC5
#define VoltageHold         PB1  // Pin 41  High = Spannung T3 halten
#define Display_Reset               PB2  // Pin 42
#define Display_A0          PB3  // Pin 43
#define Display_CS          PB4  // Pin 44
#define Display_SI          PB5  // Pin  1
#define LED1                PB6  // Pin  2  Low = LED1 (nicht benutzbar wegen SCL
#define Display_SCL         PB7  // Pin  3

#define I2C_SCL             PC0  // Pin 19  SCL
#define I2C_CDA             PC1  // Pin 20  SDA

#define BT_Reset                        PC0  // Pin 19  Reseteingang BTM222 / WiyFlypatch
#define WiFly_Reset                     PC1  // Pin 20  Reseteingang WyFly  / WiyFlypatch

#define USB2FC              PC2  // Pin 21  aktiv low > IC5
#define USB_BT              PC3  // Pin 22  high = USB, Low = Bluetooth, LED2

#define USB_Reset                       PC3  // Pin 22  Reseteingang USB FT232 / WiFlypatch

#define LowBat              PC4  // Pin 23  Low Bat Warning Lipo PKT,Input
#define Uart02Wi            PC5  // Pin 24  aktiv Low > IC4
#define Uart02FC            PC6  // Pin 25  aktiv Low > IC4
#define Summer              PC7  // Pin 26  Low = Summer
#define DisplaybeleuchtungV2            PC7  // Pin 26  High = Display-LED PCB 3.9x

#define Uart0RxD            PD0  // Pin  9  über IC4 =Wi | SV2
#define Uart0TxD            PD1  // Pin 10  über IC4 =Wi | SV2
#define Uart1RxD            PD2  // Pin 11  direkt = USB, BTM, über IC5 = Wi | SV2
#define Uart1TxD            PD3  // Pin 12  direkt = USB, BTM, über IC5 = Wi | SV2
#define SERVO2                  PD4  // Pin 13  PWM Servo 2
#define SERVO1              PD5  // Pin 14  PWM Servo 1
#define Wi232_CMD           PD6  // Pin 15  aktiv Low = Wi232 CMD
#define Displaybeleuchtung          PD7  // Pin 16  High = Display-LED PCB 3.9m
#define SummerV2                        PD7  // Pin 16  Low = Summer, PCB 3.9x

#define KEY_ENTER           Key1
#define KEY_ESC             Key2
#define KEY_PLUS            Key3
#define KEY_MINUS           Key4

#define KEY_NONE            0                   // keine Taste (menuctrl)

#define KEY_ENTER_LONG      KEY_ENTER+100       // for long press keys used in scrollbox & menuctrl
#define KEY_ESC_LONG        KEY_ESC+100
#define KEY_PLUS_LONG       KEY_PLUS+100
#define KEY_MINUS_LONG      KEY_MINUS+100


//    |=     schaltet Ausgang auf HIGH
//    &= ~   schaltet Ausgang auf LOW

#define set_reset()     (PORTB |=  (1 << Display_Reset))
#define clr_reset()     (PORTB &= ~(1 << Display_Reset))

#define set_A0()        (PORTB |=  (1 << Display_A0))
#define clr_A0()        (PORTB &= ~(1 << Display_A0))

#define set_cs()        (PORTB |=  (1 << Display_CS))
#define clr_cs()        (PORTB &= ~(1 << Display_CS))

#define set_si()        (PORTB |=  (1 << Display_SI))
#define clr_si()        (PORTB &= ~(1 << Display_SI))

#define set_scl()       (PORTB |=  (1 << Display_SCL))
#define clr_scl()       (PORTB &= ~(1 << Display_SCL))

#define _BTOn()             (PORTC &= ~(1 << USB_BT))       // Bluetooth ein

#define set_USBOn()     (PORTC |=  (1 << USB_Reset))        // USB ein
#define clr_USBOn()             (PORTC &=  ~(1 << USB_Reset))              // USB aus

#define set_BTM222On()          (PORTC |=  (1 << BT_Reset))              // BT ein
#define clr_BTM222On()          (PORTC &=  ~(1 << BT_Reset))             // BT aus

#define set_WiFlyOn()           (PORTC |=  (1 << WiFly_Reset))        // WiFly ein
#define clr_WiFlyOn()           (PORTC &=  ~(1 << WiFly_Reset))       // WiFly aus

#define clr_V_On()      (PORTB &= ~(1 << VoltageHold))  // Spannung mit T3 halten
#define set_V_On()      (PORTB |=  (1 << VoltageHold))

#define set_USB2FC()    (PORTC &= ~(1 << USB2FC))       // USB mit FC-Kabel verbinden
#define clr_USB2FC()    (PORTC |=  (1 << USB2FC))

#define set_USB2Wi()    (PORTB &= ~(1 << USB2Wi))       // USB mit Wi232 verbinden
#define clr_USB2Wi()    (PORTB |=  (1 << USB2Wi))

#define set_Uart02FC()  (PORTC &= ~(1 << Uart02FC))     // Uart0 mit FC-Kabel verbinden
#define clr_Uart02FC()  (PORTC |=  (1 << Uart02FC))

#define set_Uart02Wi()  (PORTC &= ~(1 << Uart02Wi))     // Uart0 mit Wi232 verbinden
#define clr_Uart02Wi()  (PORTC |=  (1 << Uart02Wi))


#define set_DISPLAYLIGHT()      (PORTD &= ~(1 << Displaybeleuchtung))               // Displaybeleuchtung PCB3.9m
#define clr_DISPLAYLIGHT()      (PORTD |=  (1 << Displaybeleuchtung))
#define set_DISPLAYLIGHTV2()    (PORTC &= ~(1 << DisplaybeleuchtungV2))             // Displaybeleuchtung PCB3.9x
#define clr_DISPLAYLIGHTV2()    (PORTC |=  (1 << DisplaybeleuchtungV2))

#define set_WI232CMD()  (PORTD &= ~(1 << Wi232_CMD))            // Wi232 Programmierpin
#define clr_WI232CMD()  (PORTD |=  (1 << Wi232_CMD))



void set_D_LIGHT(void);  /* Displaybeleuchtung ein*/
void clr_D_LIGHT(void);  /* Displaybeleuchtung aus */
void set_BEEP(void);     /* Beeper ein*/
void clr_BEEP(void);     /* Beeper aus*/
void InitHWPorts(void);
void set_BTOn(void);    /* Bluetooth einschalten*/
void set_BTOff(void);    /* Bluetooth einschalten*/
void set_Modul_On(uint8_t Modul);
extern volatile uint8_t PCB_Version;
extern volatile uint8_t PCB_SV2RxTxPatch;


#endif
