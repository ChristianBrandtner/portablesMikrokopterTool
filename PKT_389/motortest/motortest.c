
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
//# HISTORY  motortest.c
//#
//# 13.05.2014 OG
//# - chg: motor_test() - #ifdef USE_I2CMOTORTEST erweitert um unsigned int SerLoop;
//#        wegen Warning: variable set but not used
//#
//# 12.02.2014 OG
//# - chg: motor_test() Verschiebung von var 'buffer' wegen "unused variable 'buffer'"
//#
//# 13.05.2013 Cebra
//# - chg: #define USE_I2CMOTORTEST, I2C Funktionen schaltbar
//#
//############################################################################


#include "../cpu.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>
#include "../main.h"
#include "motortest.h"
#include "../lcd/lcd.h"
#include "../timer/timer.h"
#include "../motortest/twimaster.h"
//#include "menu.h"
#include "../uart/uart1.h"
#include "../uart/usart.h"
#include "../messages.h"




uint8_t m;
uint8_t mmode; // 0=Value 1=Motor
uint8_t v;

volatile uint8_t i2c_state;
volatile uint8_t motor_addr = 0;

#ifdef USE_I2CMOTORTEST
//--------------------------------------------------------------
// Senden der Motorwerte per I2C-Bus
//
void SendMotorData(uint8_t m,uint8_t v)
{
    if (m==0)

        for(m=0;m<MAX_MOTORS;m++)   // alle Motoren
        {
//          Motor[m].SetPoint = MotorTest[m];
            Motor[m].SetPoint = v;
            Motor[m].SetPointLowerBits = 0;

//          Motor[i].SetPoint = MotorTest[i] / 4;           // testing the high resolution
//          Motor[i].SetPointLowerBits = MotorTest[i] % 4;

        }
    else
    {
        Motor[m-1].SetPoint = v;
        Motor[m-1].SetPointLowerBits = 0;
    }



    if(I2C_TransferActive)
        I2C_TransferActive = 0; // enable for the next time
    else
    {
        motor_write = 0;
        I2C_Start(TWI_STATE_MOTOR_TX); //Start I2C Interrupt Mode
    }
}


//--------------------------------------------------------------
//
void Search_BL (void)
{
    uint8_t i = 0;
    unsigned int timer;
    lcd_cls ();
    MotorenEin =0;
    MotorTest[i] = 0;

    lcd_printp (PSTR("Suche BL-Ctrl"), 0);

    // Check connected BL-Ctrls
    BLFlags |= BLFLAG_READ_VERSION;
    motor_read = 0;  // read the first I2C-Data

    SendMotorData(0,0);
    timer = SetDelay(1);
    while(!(BLFlags & BLFLAG_TX_COMPLETE) && !CheckDelay(timer)); //wait for complete transfer

    timer = SetDelay(1);
    for(i=0; i < MAX_MOTORS; i++)
    {

        SendMotorData(i,0);


        while(!(BLFlags & BLFLAG_TX_COMPLETE)  && !CheckDelay(timer)); //wait for complete transfer


        if(Motor[i].State & MOTOR_STATE_PRESENT_MASK)
        {

            lcd_printp_at (0, 1, PSTR("Found BL-Ctrl:"), 0);
            lcd_print_hex_at (18,1,i,0);

            lcd_printp_at (0, 2, PSTR("Version:"), 0);
            lcd_print_hex_at (8,2,Motor[i].Version,0);
            lcd_printp_at (11, 2, PSTR("SetPoi:"), 0);
            lcd_print_hex_at (18,2,Motor[i].SetPoint,0);

            lcd_printp_at (0, 3, PSTR("SetPoiL:"), 0);
            lcd_print_hex_at (8,3,Motor[i].SetPointLowerBits,0);
            lcd_printp_at (11, 3, PSTR("State :"), 0);
            lcd_print_hex_at (18,3,Motor[i].State,0);

            lcd_printp_at (0, 4, PSTR("ReadMod:"), 0);
            lcd_print_hex_at (8,4,Motor[i].ReadMode,0);
            lcd_printp_at (11, 4, PSTR("Currnt:"), 0);
            lcd_print_hex_at (18,4,Motor[i].Current,0);

            lcd_printp_at (0, 5, PSTR("MaxPWM :"), 0);
            lcd_print_hex_at (8,5,Motor[i].MaxPWM,0);
            lcd_printp_at (11, 5, PSTR("Temp  :"), 0);
            write_ndigit_number_u (18,5,Motor[i].Temperature,3,1,0);
        }

    } //End For I
}
#endif


//--------------------------------------------------------------
//
void motor (uint8_t m,uint8_t v)
{
    memset (buffer, 0, 16);

    if(m == 0)
    {
        memset (buffer, v, 16);
    }
    else
    {
        buffer[m-1] = v;
    }

    SendOutData('t', ADDRESS_FC, 1, buffer, 16);
}



//--------------------------------------------------------------
//
void motor_test (uint8_t MotorMode)
{

    lcd_cls ();
    mmode = 1; // 1=Motor
    m = 1;
    v = 0;

#ifdef USE_I2CMOTORTEST
    unsigned int SerLoop;
    SerLoop = 10;

    char buffer[7];

    if (MotorMode == I2C_Mode)
    {
        Search_BL();
        do
        {
            lcd_printp_at (11, 7, PSTR("Ende Check"), 0);
            if (get_key_press (1 << KEY_ESC))
            {
                get_key_press(KEY_ALL);
                return;
            }
        }
        while (!get_key_press (1 << KEY_ENTER));
    }
#endif
    lcd_cls();
    lcd_printp (PSTR(" BL-Ctrl Test        "), 2);
    lcd_printp_at (2, 2, PSTR("Motor:   1"), 0);
    lcd_printp_at (2, 3, PSTR("Value:   0"), 0);
    lcd_frect ((8*1), (8*5), (0 * (14*8)) / 255, 6, 1);
//  lcd_printp_at (0, 7, PSTR(KEY_LINE_3), 0);
    lcd_printp_at(0, 7, strGet(KEYLINE3), 0);
    lcd_printp_at (18, 7, PSTR("\x1a \x1b"), 0);
    lcd_printp_at (0, 2, PSTR("\x1d"), 0);


#ifdef USE_I2CMOTORTEST
    if (MotorMode == I2C_Mode)
        uart1_puts("Motor;Version;Setpoint high;Setpoint low;State;ReadMode;Current;MaxPWM;Temperature\r");
#endif

    if (MotorMode == FC_Mode)
    {
        if (hardware == NC && current_hardware == NC)
        {
            SwitchToFC();
        }
    }
    do
    {
        // mmode 0=Value 1=Motor

        if ((mmode == 0) && (get_key_press (1 << KEY_PLUS) || get_key_long_rpt_sp ((1 << KEY_PLUS), 3)) && (v < 254))
        {
            v++;
            write_ndigit_number_u (9, 3, v, 3, 0,0);
            if (MotorMode == FC_Mode)
                lcd_frect ((8*1), (8*5), (v * (14*8)) / 255, 6, 1);
        }

        if ((mmode == 0) && (get_key_press (1 << KEY_MINUS) || get_key_long_rpt_sp ((1 << KEY_MINUS), 3)) && (v > 0))
        {
            if (MotorMode == FC_Mode)
                lcd_frect (((v * (14*8) / 255) + 8), (8*5), ((14*8) / 255), 6, 0);

            v--;
            write_ndigit_number_u (9, 3, v, 3, 0,0);
            if (MotorMode == FC_Mode)
                lcd_frect ((8*1), (8*5), (v * (14*8)) / 255, 6, 1);
        }

        if ((mmode == 1) && (get_key_press (1 << KEY_PLUS) || get_key_long_rpt_sp ((1 << KEY_PLUS), 1)) && (m < 16))
        {
            m++;
            write_ndigit_number_u (9, 2, m, 3, 0,0);
        }

        if ((mmode == 1) && (get_key_press (1 << KEY_MINUS) || get_key_long_rpt_sp ((1 << KEY_MINUS), 1)) && (m > 0))
        {
            m--;
            if(m > 0)
                write_ndigit_number_u (9, 2, m, 3, 0,0);
            if(m == 0)
                lcd_printp_at (9, 2, PSTR("All"), 0);
        }

        if (get_key_press (1 << KEY_ENTER))
        {
#ifdef USE_I2CMOTORTEST
            if (MotorMode == I2C_Mode)
            {
                if (v > 0)
                {
                    m = 0;
                    v=0;
                    lcd_frect ((8*1), (8*5), (0 * (14*8)) / 255, 6, 1);
                    lcd_cls_line (0, 5, 21);
                    if(m > 0) write_ndigit_number_u (9, 2, m, 3, 0,0);
                    if(m == 0) lcd_printp_at (9, 2, PSTR("All"), 0);
                    write_ndigit_number_u (9, 3, v, 3, 0,0);
                    SendMotorData(m,v);
                    timer = SetDelay(1);
                    while(!(BLFlags & BLFLAG_TX_COMPLETE)  && !CheckDelay(timer)); //wait for complete transfer
                }
            }
#endif

            if(mmode == 0) // 0=Value
            {
                lcd_printp_at (0, 2, PSTR("\x1d"), 0);
                lcd_printp_at (0, 3, PSTR(" "), 0);
                mmode = 1; // 1=Motor
            }
            else
            {
                lcd_printp_at (0, 2, PSTR(" "), 0);
                lcd_printp_at (0, 3, PSTR("\x1d"), 0);
                mmode = 0; // 0=Value
            }
        }
        //if (get_key_press (1 << KEY_ENTER))//
#ifdef USE_I2CMOTORTEST
        if (MotorMode == I2C_Mode)
        {
            SendMotorData(m,v);
            timer = SetDelay(1);
            lcd_printp_at (0, 3, PSTR("SetPoint   :"), 0);

            write_ndigit_number_u (13,3,Motor[m-1].SetPoint,3,0,0);
            lcd_printp_at (0, 4, PSTR("Current    :"), 0);
            lcd_print_hex_at (13,4,Motor[m-1].Current,0);
            write_ndigit_number_u (13,4,Motor[m-1].Current,3,0,0);
            lcd_printp_at (0, 5, PSTR("Temperature:"), 0);
            write_ndigit_number_u (13,5,Motor[m-1].Temperature,3,0,0);
            lcd_printp_at (0, 6, PSTR("Version:"), 0);
            lcd_print_hex_at (8,6,Motor[m-1].Version,0);
            lcd_printp_at (11, 6, PSTR("State :"), 0);
            lcd_print_hex_at (18,6,Motor[m-1].State,0);


            if (Motor[m-1].SetPoint > 0)
            {
                if (SerLoop == 0)
                {
                    itoa( m-1, buffer, 10);     // convert interger into string (decimal format)
                    uart1_puts(buffer);         // and transmit string to UART
                    uart1_puts(";");
                    itoa( Motor[m-1].Version, buffer, 10);   //
                    uart1_puts(buffer);
                    uart1_puts(";");
                    itoa( Motor[m-1].SetPoint, buffer, 10);   //
                    uart1_puts(buffer);
                    uart1_puts(";");
                    itoa( Motor[m-1].SetPointLowerBits, buffer, 10);   //
                    uart1_puts(buffer);
                    uart1_puts(";");
                    itoa( Motor[m-1].State, buffer, 10);   //
                    uart1_puts(buffer);
                    uart1_puts(";");
                    itoa( Motor[m-1].ReadMode, buffer, 10);   //
                    uart1_puts(buffer);
                    uart1_puts(";");
                    itoa( Motor[m-1].Current, buffer, 10);   //
                    uart1_puts(buffer);
                    uart1_puts(";");
                    itoa( Motor[m-1].MaxPWM, buffer, 10);   //
                    uart1_puts(buffer);
                    uart1_puts(";");
                    itoa( Motor[m-1].Temperature, buffer, 10);   //
                    uart1_puts(buffer);
                    uart1_puts("\r");
                    uart1_puts("\n");
                    SerLoop =200;
                }
                else
                    SerLoop--;
            }

            while(!(BLFlags & BLFLAG_TX_COMPLETE)  && !CheckDelay(timer)); //wait for complete transfer
        }
        else
#endif
            motor (m,v); //if (MotorMode == I2C_Mode)//

    }
    while (!get_key_press (1 << KEY_ESC));

    get_key_press(KEY_ALL);

    if (MotorMode == FC_Mode)
    {
        motor(0,0); // switch all engines off at exit
    }

}

