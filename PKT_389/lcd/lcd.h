/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
 *   Copyright (C) 2009 Peter "woggle" Mack, mac@denich.net                  *
 *   - original LCD control by Thomas "thkais" Kaiser                        *
 *   - special number formating routines taken from C-OSD                    *
 *      from Claas Anders "CaScAdE" Rathje                                   *
 *   - some extension, ellipse and circ_line by Peter "woggle" Mack          *
 * Thanks to Oliver Schwaneberg for adding several functions to this library!*
 *                                                                           *
 * Author:  Jan Michel (jan at mueschelsoft dot de)                          *
 * License: GNU General Public License, version 3                            *
 * Version: v0.93 September 2010                                             *
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
 *****************************************************************************/

//############################################################################
//# HISTORY  lcd.h
//#
//# 23.10.2015 cebra
//# - add: set_lcd_orientation , Funktion wieder eingebaut
//#
//# 25.09.2015 Cebra
//# - add: #define PRINTF_BUFFER_SIZE in Headfile verschoben
//# - add: #define MCRLF, zuÃ¤tzlicher Mode fÃ¼r serielle Ausgabe
//#
//# 27.06.2014 OG
//# - add: define MINVERSX, MNORMALX
//#
//# 11.06.2014 OG
//# - add: lcd_set_contrast()
//#
//# 04.06.2014 OG
//# - add: lcdx_cls_rowwidth()
//#
//# 02.05.2014 OG
//# - add: Popup_Draw() (ehemals in osd.c)
//#
//# 13.04.2014 OG
//# - add: lcd_print_LF()
//#
//# 11.04.2014 OG
//# - add: lcdx_cls_row()
//#
//# 08.04.2014 OG
//# - add: lcdx_printf_center(), lcdx_printf_center_P()
//#
//# 07.04.2014 OG
//# - add: lcd_setpos(), lcd_print_char()
//#
//# 04.04.2014 OG
//# - add: ShowTitle_P()
//#
//# 28.02.2014 OG
//# - del: show_baudrate()
//#
//# 16.02.2014 OG
//# - add: lcdx_printp_center(), lcdx_print_center()
//#
//# 13.02.2014 OG
//# - add: R# define's fuer lcd_rect_round()
//#
//# 12.02.2014 OG
//# - add: lcd_frect_round()
//# - add: lcd_rect_round()
//#
//# 03.02.2014 OG
//# - fix: bei writex_ndigit_number_u_100th() fehlte der Parameter 'mode'
//#
//# 07.07.2013 OG
//# - add: SYMBOL_CHECK
//#
//# 11.06.2013 OG
//# - add: SYMBOL_AVG, SYMBOL_MIN, SYMBOL_MAX fuer OSDDATA Anzeige
//# - del: Antennen-Symbol von OSD_General (wird wieder gezeichnet)
//#
//# 15.05.2013 OG
//# - add: define SYMBOL_SMALLDEGREE, SYMBOL_RCQUALITY
//#
//# 03.05.2013 OG
//# - fix: writex_gpspos() - Anzeige negativer Koordinaten
//# - fix: Anzeigefehler writex_datetime_time()
//# - chg: writex_datetime_date() & writex_datetime_time() Parameter
//#        'timeoffset' entfernt da das durch PKT-Config geregelt werden soll
//#
//# 28.04.2013 OG
//# - add: lcdx_printf_at(), lcdx_printf_at_P()
//#        lcd_printf_at(), lcd_printf_at_P()
//# - del: write_gps_pos()
//#
//# 22.03.2013 OG
//# - siehe lcd.c
//#
//# 11.03.2013 OG
//# - siehe lcd.c
//#
//# 07.03.2013 OG
//# - siehe lcd.c
//#
//# 06.03.2013 OG
//#  - add: extended Funktionen lcdx_... / writex...
//############################################################################


#ifndef _LCD_H
#define _LCD_H

#include <stdarg.h>             // Notwendig! (OG)
#include "../timer/timer.h"

#define PRINTF_BUFFER_SIZE  40                  // max. 40 Chars fuer den Buffer fuer lcdx_printf_at() / lcdx_printf_at_P()
char printf_buffer[PRINTF_BUFFER_SIZE];

#define MNORMAL     0   // Zeichendarstellung: Normal
#define MINVERS     2   // Zeichendarstellung: Invers
#define MBIG        3   // OBSOLETE! - Zeichendarstellung: grosser Zeichensatz (8x8) Normal
#define MBIGINVERS  4   // OBSOLETE! - Zeichendarstellung: grosser Zeichensatz (8x8) Invers
#define MNORMALX    5   // Zeichendarstellung: Normal - oben und Links wird eine 1 Pixel Linie gezogen
#define MINVERSX    6   // Zeichendarstellung: Invers - oben und Links wird eine 1 Pixel Linie gezogen
#define MCRLF       7   // für serialdebug, CR,LF am Ende einfügen

// fuer lcd_rect_round()
#define R0          0   // Radius 0
#define R1          1   // Radius 1
#define R2          2   // Radius 2


#define SYMBOL_AVG          10
#define SYMBOL_MAX          13
#define SYMBOL_MIN          16
#define SYMBOL_SMALLDEGREE  11
#define SYMBOL_BIGDEGREE    30
#define SYMBOL_CHECK        31

//------------------------------------------------------------------------------------
void lcdx_printf_at( uint8_t x, uint8_t y, uint8_t mode, int8_t xoffs, int8_t yoffs, const char *format, ... );
void lcdx_printf_at_P( uint8_t x, uint8_t y, uint8_t mode, int8_t xoffs, int8_t yoffs, const char *format, ... );
void lcd_printf_at( uint8_t x, uint8_t y, uint8_t mode, const char *format, ... );
void lcd_printf_at_P( uint8_t x, uint8_t y, uint8_t mode, const char *format, ... );
void lcdx_printf_center( uint8_t y, uint8_t mode, int8_t xoffs, int8_t yoffs, const char *format, ... );
void lcdx_printf_center_P( uint8_t y, uint8_t mode, int8_t xoffs, int8_t yoffs, const char *format, ... );


//------------------------------------------------------------------------------------
// X-tended
void lcdx_putc( uint8_t x, uint8_t y, uint8_t c, uint8_t mode, int8_t xoffs, int8_t yoffs );
void lcdx_print (uint8_t *text, uint8_t mode, int8_t xoffs, int8_t yoffs);
void lcdx_print_at (uint8_t x, uint8_t y, uint8_t *text, uint8_t mode, int8_t xoffs, int8_t yoffs);
void lcdx_printp (const char *text, uint8_t mode, int8_t xoffs, int8_t yoffs);
void lcdx_printp_at (uint8_t x, uint8_t y, const char *text, uint8_t mode, int8_t xoffs, int8_t yoffs);

void writex_ndigit_number_s (uint8_t x, uint8_t y, int16_t number, int16_t length, uint8_t pad, uint8_t mode, int8_t xoffs, int8_t yoffs);
void writex_ndigit_number_s_10th (uint8_t x, uint8_t y, int16_t number, int16_t length, uint8_t pad, uint8_t mode, int8_t xoffs, int8_t yoffs);

void writex_ndigit_number_u (uint8_t x, uint8_t y, uint16_t number, int16_t length, uint8_t pad, uint8_t mode, int8_t xoffs, int8_t yoffs);
void writex_ndigit_number_u_10th (uint8_t x, uint8_t y, uint16_t number, int16_t length, uint8_t pad, uint8_t mode, int8_t xoffs, int8_t yoffs);
void writex_ndigit_number_u_100th( uint8_t x, uint8_t y, uint16_t number, int16_t length, uint8_t pad, uint8_t mode, int8_t xoffs, int8_t yoffs);

void lcdx_puts_at(uint8_t x, uint8_t y, const char *s, uint8_t mode, int8_t xoffs, int8_t yoffs);
void writex_time (uint8_t x, uint8_t y, uint16_t seconds, uint8_t mode, int8_t xoffs, int8_t yoffs);
//void writex_gpspos( uint8_t x, uint8_t y, uint32_t GPSpos, uint8_t mode, int8_t xoffs, int8_t yoffs );
void writex_gpspos( uint8_t x, uint8_t y, int32_t GPSpos, uint8_t mode, int8_t xoffs, int8_t yoffs );

void writex_datetime_time( uint8_t x, uint8_t y, PKTdatetime_t datetime, uint8_t mode, int8_t xoffs, int8_t yoffs);
void writex_datetime_date( uint8_t x, uint8_t y, PKTdatetime_t datetime, uint8_t mode, int8_t xoffs, int8_t yoffs);

void Show_PKTError_NoRAM(void);
void ShowTitle_P( const char *title, uint8_t clearscreen );


//------------------------------------------------------------------------------------
void lcd_rect_round( uint8_t x1, uint8_t y1, uint8_t widthx, uint8_t widthy, uint8_t mode, uint8_t r);
void lcd_frect_round( uint8_t x1, uint8_t y1, uint8_t widthx, uint8_t widthy, uint8_t mode, uint8_t r);

void lcdx_print_center( uint8_t y, uint8_t *text, uint8_t mode, int8_t xoffs, int8_t yoffs);
void lcdx_printp_center( uint8_t y, const char *text, uint8_t mode, int8_t xoffs, int8_t yoffs);

void lcdx_cls_row( uint8_t y, uint8_t mode, int8_t yoffs );
void lcdx_cls_rowwidth( uint8_t y, uint8_t width, uint8_t mode, int8_t xoffs, int8_t yoffs );

void lcd_setpos( uint8_t x, uint8_t y );
void lcd_print_char( uint8_t c, uint8_t mode );
void lcd_print_LF( void );

void Popup_Draw( uint8_t heigthC );

void lcd_set_orientation( uint8_t value );


/*

//-----------------------------------------------------------------------------
// Command Codes
//-----------------------------------------------------------------------------
//1: Display on/off
#define LCD_DISPLAY_ON          0xAF  //switch display on
#define LCD_DISPLAY_OFF         0xAE  //switch display off

//2: display start line set (lower 6 bits select first line on lcd from 64 lines in memory)
#define LCD_START_LINE          0x40

//3: Page address set (lower 4 bits select one of 8 pages)
#define LCD_PAGE_ADDRESS        0xB0

//4: column address (lower 4 bits are upper / lower nibble of column address)
#define LCD_COL_ADDRESS_MSB     0x10
#define LCD_COL_ADDRESS_LSB     0x00  //second part of column address

//8: select orientation (black side of the display should be further away from viewer)
#define LCD_BOTTOMVIEW          0xA1  //6 o'clock view
#define LCD_TOPVIEW             0xA0  //12 o'clock view

//9: select normal (white background, black pixels) or reverse (black background, white pixels) mode
#define LCD_DISPLAY_POSITIVE    0xA6  //not inverted mode
#define LCD_DISPLAY_INVERTED    0xA7  //inverted display

//10: show memory content or switch all pixels on
#define LCD_SHOW_NORMAL         0xA4  //show dram content
#define LCD_SHOW_ALL_POINTS     0xA5  //show all points

//11: lcd bias set
#define LCD_BIAS_1_9            0xA2
#define LCD_BIAS_1_7            0xA3

//14: Reset Controller
#define LCD_RESET_CMD           0xE2

//15: output mode select (turns display upside-down)
#define LCD_SCAN_DIR_NORMAL     0xC0  //normal scan direction
#define LCD_SCAN_DIR_REVERSE    0xC8  //reversed scan direction

//16: power control set (lower 3 bits select operating mode)
//Bit 0: Voltage follower on/off - Bit 1: Voltage regulator on/off - Bit 2: Booster circuit on/off
#define LCD_POWER_CONTROL       0x28  //base command
#define LCD_POWER_LOW_POWER     0x2F
#define LCD_POWER_WIDE_RANGE    0x2F
#define LCD_POWER_LOW_VOLTAGE   0x2B

//17: voltage regulator resistor ratio set (lower 3 bits select ratio)
//selects lcd voltage - 000 is low (~ -2V), 111 is high (~ - 10V), also depending on volume mode. Datasheet suggests 011
#define LCD_VOLTAGE             0x20

//18: Volume mode set (2-byte command, lower 6 bits in second word select value, datasheet suggests 0x1F)
#define LCD_VOLUME_MODE_1       0x81
#define LCD_VOLUME_MODE_2       0x00

//#if DISPLAY_TYPE == 128 || DISPLAY_TYPE == 132
//19: static indicator (2-byte command), first on/off, then blinking mode
#define LCD_INDICATOR_ON        0xAD  //static indicator on
#define LCD_INDICATOR_OFF       0xAC  //static indicator off
#define LCD_INDICATOR_MODE_OFF  0x00
#define LCD_INDICATOR_MODE_1HZ  0x01
#define LCD_INDICATOR_MODE_2HZ  0x10
#define LCD_INDICATOR_MODE_ON   0x11

//20: booster ratio set (2-byte command)
#define LCD_BOOSTER_SET         0xF8  //set booster ratio
#define LCD_BOOSTER_234         0x00  //2x-4x
#define LCD_BOOSTER_5           0x01  //5x
#define LCD_BOOSTER_6           0x03  //6x
//#endif

//22: NOP command
#define LCD_NOP                 0xE3

//#if DISPLAY_TYPE == 102
////25: advanced program control
//#define LCD_ADV_PROG_CTRL     0xFA
//#define LCD_ADV_PROG_CTRL2    0x10
//#endif

//-----------------------------------------------------------------------------
// Makros to execute commands
//-----------------------------------------------------------------------------

#define LCD_SWITCH_ON()                 lcd_command(LCD_DISPLAY_ON)
#define LCD_SWITCH_OFF()                lcd_command(LCD_DISPLAY_OFF)
#define LCD_SET_FIRST_LINE(i)           lcd_command(LCD_START_LINE | ((i) & 0x3F))
#define LCD_SET_PAGE_ADDR(i)            lcd_command(LCD_PAGE_ADDRESS | ((i) & 0x0F))
#define LCD_SET_COLUMN_ADDR(i)          lcd_command(LCD_COL_ADDRESS_MSB | ((i>>4) & 0x0F)); \
                                        lcd_command(LCD_COL_ADDRESS_LSB | ((i) & 0x0F))
#define LCD_GOTO_ADDRESS(page,col);     lcd_command(LCD_PAGE_ADDRESS | ((page) & 0x0F)); \
                                        lcd_command(LCD_COL_ADDRESS_MSB | ((col>>4) & 0x0F)); \
                                        lcd_command(LCD_COL_ADDRESS_LSB | ((col) & 0x0F));

#define LCD_SET_BOTTOM_VIEW()           lcd_command(LCD_BOTTOMVIEW)
#define LCD_SET_TOP_VIEW()              lcd_command(LCD_TOPVIEW)
#define LCD_SET_MODE_POSITIVE()         lcd_command(LCD_DISPLAY_POSITIVE)
#define LCD_SET_MODE_INVERTED()         lcd_command(LCD_DISPLAY_INVERTED)
#define LCD_SHOW_ALL_PIXELS_ON()        lcd_command(LCD_SHOW_ALL_POINTS)
#define LCD_SHOW_ALL_PIXELS_OFF()       lcd_command(LCD_SHOW_NORMAL)
#define LCD_SET_BIAS_RATIO_1_7()        lcd_command(LCD_BIAS_1_7)
#define LCD_SET_BIAS_RATIO_1_9()        lcd_command(LCD_BIAS_1_9)
#define LCD_SEND_RESET()                lcd_command(LCD_RESET_CMD)
#define LCD_ORIENTATION_NORMAL()        lcd_command(LCD_SCAN_DIR_NORMAL)
#define LCD_ORIENTATION_UPSIDEDOWN()    lcd_command(LCD_SCAN_DIR_REVERSE)
#define LCD_SET_POWER_CONTROL(i)        lcd_command(LCD_POWER_CONTROL | ((i) & 0x07))
#define LCD_SET_LOW_POWER()             lcd_command(LCD_POWER_LOW_POWER)
#define LCD_SET_WIDE_RANGE()            lcd_command(LCD_POWER_WIDE_RANGE)
#define LCD_SET_LOW_VOLTAGE()           lcd_command(LCD_POWER_LOW_VOLTAGE)
#define LCD_SET_BIAS_VOLTAGE(i)         lcd_command(LCD_VOLTAGE | ((i) & 0x07))
#define LCD_SET_VOLUME_MODE(i)          lcd_command(LCD_VOLUME_MODE_1); \
                                        lcd_command(LCD_VOLUME_MODE_2 | ((i) & 0x3F))

//#if DISPLAY_TYPE == 128 || DISPLAY_TYPE == 132
#define LCD_SET_INDICATOR_OFF()         lcd_command(LCD_INDICATOR_OFF); \
                                        lcd_command(LCD_INDICATOR_MODE_OFF)
#define LCD_SET_INDICATOR_STATIC()      lcd_command(LCD_INDICATOR_ON); \
                                        lcd_command(LCD_INDICATOR_MODE_ON)
#define LCD_SET_INDICATOR_1HZ()         lcd_command(LCD_INDICATOR_ON); \
                                        lcd_command(LCD_INDICATOR_MODE_1HZ)
#define LCD_SET_INDICATOR_2HZ()         lcd_command(LCD_INDICATOR_ON); \
                                        lcd_command(LCD_INDICATOR_MODE_2HZ)
#define LCD_SET_INDICATOR(i,j)          lcd_command(LCD_INDICATOR_OFF | ((i) & 1)); \
                                        lcd_command(((j) & 2))
#define LCD_SLEEP_MODE                  lcd_command(LCD_INDICATOR_OFF); \
                                        lcd_command(LCD_DISPLAY_OFF); \
                                        lcd_command(LCD_SHOW_ALL_POINTS)
//#endif

//#if DISPLAY_TYPE == 102
//#define LCD_TEMPCOMP_HIGH             0x80
//#define LCD_COLWRAP                   0x02
//#define LCD_PAGEWRAP                  0x01
//#define LCD_SET_ADV_PROG_CTRL(i)      lcd_command(LCD_ADV_PROG_CTRL);
//                                      lcd_command(LCD_ADV_PROG_CTRL2 & i)
//#endif

*/



extern volatile uint8_t LCD_ORIENTATION;

//#define LCD_LINES               8
//#define LCD_COLS                21

extern uint8_t lcd_xpos;
extern uint8_t lcd_ypos;

void lcd_set_contrast( uint8_t value );
void lcd_command(uint8_t cmd);
void send_byte (uint8_t data);
void LCD_Init (uint8_t LCD_Mode);
void new_line (void);
void lcd_puts_at(uint8_t x, uint8_t y,const char *s, uint8_t mode );
void lcd_putc (uint8_t x, uint8_t y, uint8_t c, uint8_t mode);
void send_byte (uint8_t data);
void lcd_print (uint8_t *text, uint8_t mode);
void lcd_print_at (uint8_t x, uint8_t y, uint8_t *text, uint8_t mode);

void lcd_printp (const char *text, uint8_t mode);
void lcd_printp_at (uint8_t x, uint8_t y, const char *text, uint8_t mode);
void lcd_printpns (const char *text, uint8_t mode);
void lcd_printpns_at (uint8_t x, uint8_t y, const char *text, uint8_t mode);
void lcd_cls (void);
void lcd_cls_line (uint8_t x, uint8_t y, uint8_t w);

void print_display (uint8_t *text);
void print_display_at (uint8_t x, uint8_t y, uint8_t *text);
void copy_line (uint8_t y);
void paste_line (uint8_t y);

void lcd_plot (uint8_t x, uint8_t y, uint8_t mode);
void lcd_line (unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, uint8_t mode);
void lcd_rect (uint8_t x1, uint8_t y1, uint8_t widthx, uint8_t widthy, uint8_t mode);
void lcd_frect (uint8_t x1, uint8_t y1, uint8_t widthx, uint8_t widthy, uint8_t mode);
void lcd_circle (int16_t x0, int16_t y0, int16_t radius, uint8_t mode);
void lcd_fcircle (int16_t x0, int16_t y0, int16_t radius, uint8_t mode);
void lcd_circ_line (uint8_t x, uint8_t y, uint8_t r, uint16_t deg, uint8_t mode);

void lcd_ellipse (int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint8_t mode);
void lcd_ellipse_line (uint8_t x, uint8_t y, uint8_t rx, uint8_t ry, uint16_t deg, uint8_t mode);

void lcd_ecircle (int16_t x0, int16_t y0, int16_t radius, uint8_t mode);
void lcd_ecirc_line (uint8_t x, uint8_t y, uint8_t r, uint16_t deg, uint8_t mode);

void lcd_view_font (uint8_t page);
void lcd_print_hex_at (uint8_t x, uint8_t y, uint8_t h, uint8_t mode);

void lcd_write_number_u (uint8_t number);
void lcd_write_number_u_at (uint8_t x, uint8_t y, uint8_t number);
void lcd_print_hex (uint8_t h, uint8_t mode);
/**
 * Write only some digits of a unsigned <number> at <x>/<y>
 * <length> represents the length to rightbound the number
 * <pad> = 1 will cause blank spaced to be filled up with zeros e.g. 007 instead of   7
 */
void write_ndigit_number_u (uint8_t x, uint8_t y, uint16_t number, int16_t length, uint8_t pad,uint8_t mode);

/**
 * Write only some digits of a signed <number> at <x>/<y>
 * <length> represents the length to rightbound the number
 * <pad> = 1 will cause blank spaced to be filled up with zeros e.g. 007 instead of   7
 */

void write_ndigit_number_s (uint8_t x, uint8_t y, int16_t number, int16_t length, uint8_t pad, uint8_t mode);

/**
 * Write only some digits of a unsigned <number> at <x>/<y> as /10th of the value
 * <length> represents the length to rightbound the number
 * <pad> = 1 will cause blank spaced to be filled up with zeros e.g. 00.7 instead of   .7
 */
void write_ndigit_number_u_10th (uint8_t x, uint8_t y, uint16_t number, int16_t length, uint8_t pad, uint8_t mode);

/**
 * Write only some digits of a unsigned <number> at <x>/<y> as /100th of the value
 * <length> represents the length to rightbound the number
 * <pad> = 1 will cause blank spaced to be filled up with zeros e.g. 00.7 instead of   .7
 */
void write_ndigit_number_u_100th (uint8_t x, uint8_t y, uint16_t number, int16_t length, uint8_t pad);

/**
 * Write only some digits of a signed <number> at <x>/<y> as /10th of the value
 * <length> represents the length to rightbound the number
 * <pad> = 1 will cause blank spaced to be filled up with zeros e.g. 00.7 instead of   .7
 */
void write_ndigit_number_s_10th (uint8_t x, uint8_t y, int16_t number, int16_t length, uint8_t pad, uint8_t mode);

/**
 *  write <seconds> as human readable time at <x>/<y>
 */
void write_time (uint8_t x, uint8_t y, uint16_t seconds);



#endif
