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
//# HISTORY  bluetooth.h
//#
//# 25.06.2014 OG
//# - chg: bt_discover() Parameter
//#
//# 16.06.2014 OG
//# - add: BTM222_Initalize()
//# - del: bt_init()
//#
//# 08.06.2014 OG
//# - del: BT_New_Baudrate
//#
//# 24.06.2013 OG
//# - add: bt_fixname()
//############################################################################


#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include <avr/io.h>
//#include <common.h>
#include "fifo.h"


#define SQUIRREL
#define NUTS_LIST           16
#define EXTENSIONS_LIST     16
#define RXD_BUFFER_SIZE     150
//void InitBT(void);

extern char bt_rx_buffer[RXD_BUFFER_SIZE];
extern volatile uint8_t bt_rx_len;
extern volatile uint8_t bt_rx_ready;


//extern char data_decode[RXD_BUFFER_SIZE];

extern uint8_t bt_rxerror;
extern uint8_t bt_frameerror;
extern uint8_t bt_overrun;
extern uint8_t bt_bufferoverflow;

extern fifo_t in_fifo;


typedef struct _device_info device_info;

// device info struct, holds mac , class and extensions + values of a device
struct _device_info
{
    char DevName[20];
    char mac[14];

//    uint8_t class;
//    uint8_t extension_types[EXTENSIONS_LIST];
//    uint16_t values_cache[EXTENSIONS_LIST];
};
extern uint8_t bt_devicecount;
extern device_info device_list[NUTS_LIST];
extern char localID[15];
#define valid(num) (num < NUTS_LIST && (device_list[num].mac[0] != 0 || device_list[num].mac[1] != 0 || device_list[num].mac[2] != 0 || device_list[num].mac[3] != 0 || device_list[num].mac[4] != 0 || device_list[num].mac[5] != 0 || device_list[num].mac[6] != 0 || device_list[num].mac[7] != 0 || device_list[num].mac[8] != 0 || device_list[num].mac[9] != 0 || device_list[num].mac[10] != 0 || device_list[num].mac[11] != 0))


extern void bt_start(void); // EchoAnswervariable setzen
void bt_set_EchoAnswer (uint8_t onoff);

extern uint8_t bt_getID (void);

//extern static communication_mode_t update_comm_mode(uint16_t timeout_ms);

// Bluetooth mode ENUM
typedef enum
{
    BLUETOOTH_MASTER, // < Master Mode (to create outgoinng connections).
    BLUETOOTH_SLAVE   // < Slave Mode (to wait for incoming connections).
} bt_mode_t;

// set baudrate bluetooth
// @return true = ok
//         false = error
//extern uint16_t bt_setbaud (uint8_t baudrate);
uint16_t bt_setbaud(uint8_t baudrate);

// init bluetooth driver
// @return always true
//
//extern uint16_t bt_init (void (*upate_percentage) (uint16_t));
//extern uint16_t bt_init (void);

// Set the Bluetooth mode
// @param mode bt_mode_t Bluetooth Mode ENUM (BLUETOOTH_MASTER or BLUETOOTH_SLAVE)
// @return true if mode change was succesful, false if not
//
extern uint8_t bt_set_mode (const bt_mode_t mode);

// recieve data over bluetooth
// @param data pointer to memory for data storage
// @param length value of length after call holds the actual recived data length
// @param timeout_ms timeout in ms after the recive function aborts and returns with false
// @return false if recived length > length parameter or it timeouted, true otherwise
//
extern uint16_t bt_receive (void * data, uint8_t length, uint16_t timeout_ms);

// send data over bluetooth
// @param data pointer to the data to send
// @param length length of the data to be send
// @return true if sendingn was successful, false otherwise
//
extern uint16_t bt_send (void * data, const uint8_t length);

// squirrelt only functions
#ifdef SQUIRREL

// open bluetoot connection (only one at a time possible)
// @param address connection is opened to this device mac address
// @return true if connection was established, false otherwise
//
extern uint16_t bt_connect (const char *address);

// closes bluetooth connection
// @return false if failed, true otherwise
//
extern uint16_t bt_disconnect (void);

// discover bluetooth devices
// @param result in a 2 dimensional array first index are devicecs (max 8) second is mac address string
// @param update_callback to inform of progress (in % from 0 to 100)
// @return true if successful, false if error occured
//
//extern uint16_t bt_discover(void);
//extern uint16_t bt_discover (char result[8][12], void (*update_callback)(const uint16_t progress));

extern void bt_downlink_init(void);        // Auf Master stellen für Devicesuche und GPS Empfang

extern void bt_searchDevice(void);              //Bluetoothgeräte suchen

#endif  // SQUIRREL


void uart_receive(void);
void bt_fixname(void);
void BTM222_Initalize( void );
uint8_t bt_showsettings( void );


#endif

