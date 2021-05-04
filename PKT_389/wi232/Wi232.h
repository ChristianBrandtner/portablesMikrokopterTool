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
//# HISTORY  Wi232.h
//#
//# 17.11.2015 Cebra
//# - chg: Funktion Wi232_Initalize um RÃ¼ckgabewert erweitert, wird im Setup benÃ¶tigt
//#
//# 10.06.2014 OG
//# - add: Wi232_Initalize()
//# - add: Wi232_ConfigPC()
//# - del: InitWi232()
//#
//# 08.06.2014 OG
//# - chg: InitWi232() - Parameteraenderung
//# - del: Wi232_New_Baudrate
//# - add: Source-Historie ergaenzt
//############################################################################

#ifndef WI232_H_
#define WI232_H_


// Non-volatile Registers
//      Name            Address Description Default
#define regNVTXCHANNEL  0x00    // Transmit channel setting         ## 0 ##
#define regNVRXCHANNEL  0x01    // Receive channel setting          ## 0 ##
#define regNVPWRMODE    0x02    // Operating mode settings          ## +13 dBm widebandmode ##
#define regNVDATARATE   0x03    // UART data rate                   ## 2400bps ##
#define regNVNETMODE    0x04    // Network mode (Normal/Slave)      ## Normal ##
#define regNVTXTO       0x05    // Transmit wait timeout            ## ~16ms ##
#define regNVNETGRP     0x06    // Network group ID                 ## 0x00 ##
#define regNVUSECRC     0x08    // Enable/Disable CRC               ## Enabled ##
#define regNVUARTMTU    0x09    // Minimum transmission unit.       ## 64 bytes ##
#define regNVSHOWVER    0x0A    // Enable/Disable start-up message  ## Enabled ##
#define regNVCSMAMODE   0x0B    // Enable/Disable CSMA              ## Enabled ##
#define regNVSLPMODE    0x0D    // Power state of module            ## Awake ##
#define regNVACKONWAKE  0x0E    // Send ACK character to host on wake


// Non-volatile Read Only Registers
//      Name            Address Description
#define regMAC0         0x22    // These registers form the unique 48-bit MAC address.
#define regMAC1         0x23    // MAC
#define regMAC2         0x24    // MAC
#define regOUI0         0x25    // MAC
#define regOUI1         0x26    // MAC
#define regOUI2         0x27    // MAC

#define regDiscover     0x78    // Versionsregister


// Volatile Read/Write Registers
//      Name            Address Description
#define regTXCHANNEL    0x4B    // Transmit channel setting
#define regRXCHANNEL    0x4C    // Receive channel setting
#define regPWRMODE      0x4D    // Operating mode settings
#define regDATARATE     0x4E    // UART data rate
#define regNETMODE      0x4F    // Network mode (Normal or Slave)
#define regTXTO         0x50    // Transmit wait timeout
#define regNETGRP       0x51    // Network group ID
#define regUSECRC       0x53    // Enable/Disable CRC
#define regUARTMTU      0x54    // Minimum transmission unit.
#define regSHOWVER      0x55    // Enable/Disable start-up message
#define regCSMAMODE     0x56    // Enable/disable CSMA
#define regSLPMODE      0x58    // Power state of module
#define regACKONWAKE    0x59    // Send ACK character to host on wake


// Wideband Channels
// regNVTXCHAN (0x00) regTXCHAN (0x4B)
//      Channel         Number  Frequency
#define wChan0          0x00    // 868.300 MHz
#define wChan1          0x01    // 868.95 MHz   ## MK ##

// Narrowband Channels
// regNVRXCHAN (0x01) regRXCHAN (0x4C)
//      Channel         Number  Frequency
#define nChan0          0x00    // 868.225 MHz
#define nChan1          0x01    // 868.375 MHz   ## MK ##
#define nChan2          0x02    // 868.850 MHz
#define nChan3          0x03    // 869.050 MHz
#define nChan4          0x04    // 869.525 MHz
#define nChan5          0x05    // 869.850 MHz

// Power Mode
// regNVPWRMODE (0x02) regPWRMODE (0x4D)
//                               PM1 PM1 PM0 Mode
#define NbModeN0        0x00    // 0   0   0    Narrowband Mode       0dBm power setting (typical)
#define WbModeP5        0x01    // 0   0   1    Wideband Mode        +5dBm power setting (typical)
#define WbModeP10       0x02    // 0   1   0    Wideband Mode       +10dBm power setting (typical)
#define WbModeP15       0x03    // 0   1   1    Wideband Mode       +15dBm power setting (typical)   ## MK ##
#define WbModeN0        0x04    // 1   0   0    Wideband Mode         0dBm power setting (typical)
#define NbModeP5        0x05    // 1   0   1    Narrowband Mode      +5dBm power setting (typical)
#define NbModeP10       0x06    // 1   1   0    Narrowband Mode     +10dBm power setting (typical)
#define NbModeP15       0x07    // 1   1   1    Narrowband Mode     +15dBm power setting (typical)

// Wi232 UART Baudrate
// regNVDATARATE (0x03) regDATARATE (0x4E)
//          Baud Rate           BR2 BR1 BR0
#define Wi232_2400      Baud_2400   // 0 0 0* (default 2400)
#define Wi232_9600      Baud_9600   // 0 0 1
#define Wi232_19200     Baud_19200  // 0 1 0
#define Wi232_38400     Baud_38400  // 0 1 1
#define Wi232_57600     Baud_57600  // 1 0 0   ## MK ##
#define Wi232_115200            Baud_115200 // 1 0 1
#define Wi232_10400     0x06    // 1 1 0
#define Wi232_31250     0x07    // 1 1 1

// NetworkMode
// regNVNETMODE (0x04) regNETMODE (0x4F)
#define NetMode_Slave   0x00    // Slavemode
#define NetMode_Normal  0x01    // Normalmode (default)

// Transmit Wait Timeout
// regNVTXTO (0x05) regTXTO (0x50)
#define TWaitTimeFull   0x00    // full Buffer required
#define TWaitTime16     0x10    // 16 ms Delay (default)

// Network Group
// regNVNETGRP (0x06) regNETGRP (0x51)
#define NetWorkGroup    66      // default = 0, valid 0-127   ## MK = 66 ##

// CRC Control
// regNVUSECRC (0x08) regUSECRC (0x53)
#define CRC_Disable     0x00    // no CRC check
#define CRC_Enable      0x01    // CRC check (default)

// UART minimum transmission unit
// regNVUARTMTU (0x09) regUARTMTU (0x54)
#define UartMTU64       64      // default=64, valid 1-144

// Verbose mode
// regNVSHOWVER (0x0A)
#define ShowVers_Dis    0x00    // do not show Startupmessage   ## MK = 66 ##
#define ShowVers_En     0x01    // show Startupmessage (default)

// CSMA enable
// regNVCSMAMODE (0x0B) regCSMAMODE (0x56)
#define CSMA_Dis        0x00    // disable CSMA Carrier-sense multiple access
#define CSMA_En         0x01    // enable  CSMA (default)

// Sleep control
// regNVSLPMODE (0x0D) regSLPMODE (0x58)
#define Sleep_Awake     0x00    // Sleepmode = Awake (default)
#define Sleep           0x01    // Sleepmode = Sleep
#define Sleep_Stby      0x02    // Sleepmode = Standby

// ACK on Wake
// regNVACKONWAKE (0x0D) regACKONWAKE (0x59)
#define ACKwake_Dis     0x00    // disable ACK on Wake
#define ACKwake_En      0x01    // enable  ACK on Wake (default)



//----------------------------------
// EXPORT
//----------------------------------
uint8_t Wi232_Initalize( void );
void Wi232_ConfigPC( void );


#endif // WI232_H_
