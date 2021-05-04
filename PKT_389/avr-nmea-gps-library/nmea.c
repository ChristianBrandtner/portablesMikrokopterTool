/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//############################################################################
//# HISTORY  nmea.c
//#
//# 08.08.2015 CB
//# - add: Einführung eines neuen Interruptbasierten NMEA Parsers
//#        cpp-Code auf c geändert und an PKT angepasst
//#
//############################################################################

/* URSPRUNG:
    File:       nmea.cpp
    Version:    0.1.0
    Date:       Feb. 23, 2013
	License:	GPL v2
    
	NMEA GPS content parser
    
    ****************************************************************************
    Copyright (C) 2013 Radu Motisan  <radu.motisan@gmail.com>
	
	http://www.pocketmagic.net

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    ****************************************************************************
 */

#include "nmea.h"
#include "../timer/timer.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <util/delay.h>
#include  <stdbool.h>


  #define MAX_POWER   10
  #define getPower(x) (int32_t)pgm_read_dword(&powers[x])
  static const int32_t    powers[MAX_POWER] PROGMEM = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

  uint8_t m_bFlagRead,                                      // flag used by the parser, when a valid sentence has begun
          m_bFlagDataReady = false;                         // valid GPS fix and data available, user can call reader functions
  volatile uint32_t res_nNMEAcounter = 0;                   // NMEA Datensatzzähler
  volatile uint32_t res_nCRCerror = 0;                      // zählt die CRC Errors beim dekodieren
  uint8_t receiveNMEA = false;
  char tmp_words[20][15],                                   //      hold parsed words for one given NMEA sentence
       tmp_szChecksum[15];                                  //      hold the received checksum for one given NMEA sentence

  // will be set to true for characters between $ and * only
  bool m_bFlagComputedCks ;                                 // used to compute checksum and indicate valid checksum interval (between $ and * in a given sentence)
  int  m_nChecksum ;                                        // numeric checksum, computed for a given sentence
  bool m_bFlagReceivedCks ;                                 // after getting  * we start cuttings the received checksum
  int  index_received_checksum ;                            // used to parse received checksum

  // word cutting variables
  int  m_nWordIdx ,                                         // the current word in a sentence
       m_nPrevIdx,                                          // last character index where we did a cut
       m_nNowIdx ;                                          // current character index

  // globals to store parser results
  int32_t res_fLongitude;                                   // GPRMC and GPGGA
  int32_t res_fLatitude;                                    // GPRMC and GPGGA
  unsigned char   res_nUTCHour, res_nUTCMin, res_nUTCSec,   // GPRMC and GPGGA
                  res_nUTCDay, res_nUTCMonth, res_nUTCYear; // GPRMC
  char     Time[9];                                         // GPRMC and GPGGA
  uint8_t res_nSatellitesUsed;                              // GPGGA
  uint8_t res_nGPSfix;                                      // GPGGA
  int16_t res_nHDOP;                                        // GPGGA
  int16_t res_fAltitude;                                    // GPGGA
  float res_fSpeed;                                         // GPRMC
  int16_t res_fBearing;                                       // GPRMC

  NMEA_GPGGA_t    NMEA;                                     // Variable mit der Struktur für die NMEA Daten für PKT

  // the parser, currently handling GPRMC and GPGGA, but easy to add any new sentences
  //              void parsedata();
  // aux functions
  int digit2dec(char hexdigit);
  float string2float(char* s);
  int   mstrcmp(const char *s1, const char *s2);




/*
 * The serial data is assembled on the fly, without using any redundant buffers. 
 * When a sentence is complete (one that starts with $, ending in EOL), all processing is done on 
 * this temporary buffer that we've built: checksum computation, extracting sentence "words" (the CSV values), 
 * and so on.
 * When a new sentence is fully assembled using the fusedata function, the code calls parsedata. 
 * This function in turn, splits the sentences and interprets the data. Here is part of the parser function, 
 * handling both the $GPRMC NMEA sentence:
 */
uint8_t fusedata(char c)

         {
	
	if (c == '$') {
		m_bFlagRead = true;
		// init parser vars
		m_bFlagComputedCks = false;
		m_nChecksum = 0;
		// after getting  * we start cuttings the received m_nChecksum
		m_bFlagReceivedCks = false;
		index_received_checksum = 0;
		// word cutting variables
		m_nWordIdx = 0; m_nPrevIdx = 0; m_nNowIdx = 0;
		timer_nmea_timeout = NMEA_TIMEOUT;                      // Timeout Timer setzen
	}
	
	if (m_bFlagRead) {
		// check ending
		if (c == '\r' || c== '\n') {
			// catch last ending item too
			tmp_words[m_nWordIdx][m_nNowIdx - m_nPrevIdx] = 0;
			m_nWordIdx++;
			// cut received m_nChecksum
			tmp_szChecksum[index_received_checksum] = 0;
			// sentence complete, read done
			m_bFlagRead = false;
			// parse
			parsedata();
		} else {
			// computed m_nChecksum logic: count all chars between $ and * exclusively
			if (m_bFlagComputedCks && c == '*') m_bFlagComputedCks = false;
			if (m_bFlagComputedCks) m_nChecksum ^= c;
			if (c == '$') m_bFlagComputedCks = true;
			// received m_nChecksum
			if (m_bFlagReceivedCks)  {
				tmp_szChecksum[index_received_checksum] = c;
				index_received_checksum++;
			}
			if (c == '*') m_bFlagReceivedCks = true;
			// build a word
			tmp_words[m_nWordIdx][m_nNowIdx - m_nPrevIdx] = c;
			if (c == ',') {
				tmp_words[m_nWordIdx][m_nNowIdx - m_nPrevIdx] = 0;
				m_nWordIdx++;
				m_nPrevIdx = m_nNowIdx;
			}
			else m_nNowIdx++;
		}				
	}
	return m_nWordIdx;
}


/*
 * parse internal tmp_ structures, fused by pushdata, and set the data flag when done
 */
void parsedata(void) {
	int received_cks = 16*digit2dec(tmp_szChecksum[0]) + digit2dec(tmp_szChecksum[1]);
	//uart1.Send("seq: [cc:%X][words:%d][rc:%s:%d]\r\n", m_nChecksum,m_nWordIdx, tmp_szChecksum, received_cks);
	// check checksum, and return if invalid!
	if (m_nChecksum != received_cks) {
	    m_bFlagDataReady = false;
	    res_nCRCerror = res_nCRCerror+1;
	    return;
	}
	/* $GPGGA
	 * $GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
	 * ex: $GPGGA,230600.501,4543.8895,N,02112.7238,E,1,03,3.3,96.7,M,39.0,M,,0000*6A,
	 *
	 * WORDS:
	 *  1    = UTC of Position
	 *  2    = Latitude
	 *  3    = N or S
	 *  4    = Longitude
	 *  5    = E or W
	 *  6    = GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
	 *  7    = Number of satellites in use [not those in view]
	 *  8    = Horizontal dilution of position
	 *  9    = Antenna altitude above/below mean sea level (geoid)
	 *  10   = Meters  (Antenna height unit)
	 *  11   = Geoidal separation (Diff. between WGS-84 earth ellipsoid and mean sea level.  
	 *      -geoid is below WGS-84 ellipsoid)
	 *  12   = Meters  (Units of geoidal separation)
	 *  13   = Age in seconds since last update from diff. reference station
	 *  14   = Diff. reference station ID#
	 *  15   = Checksum
	 */
	if (mstrcmp(tmp_words[0], "$GPGGA") == 0) {
		// Check GPS Fix: 0=no fix, 1=GPS fix, 2=Dif. GPS fix
	        res_nGPSfix = atoi(&tmp_words[6][0]);
	        NMEA.SatFix = res_nGPSfix;
		if (tmp_words[6][0] == '0') { 
			// clear data
			res_fLatitude = 0;
			res_fLongitude = 0;
//			m_bFlagDataReady = false;

//			return;
		}			
		// parse time
		res_nUTCHour = digit2dec(tmp_words[1][0]) * 10 + digit2dec(tmp_words[1][1]);
		res_nUTCMin = digit2dec(tmp_words[1][2]) * 10 + digit2dec(tmp_words[1][3]);
		res_nUTCSec = digit2dec(tmp_words[1][4]) * 10 + digit2dec(tmp_words[1][5]);
		NMEA_getTime (tmp_words[1]);

		// parse latitude and longitude in NMEA format
//		res_fLatitude = string2float(tmp_words[2]);
//		res_fLongitude = string2float(tmp_words[4]);

		// get decimal format
//		if (tmp_words[3][0] == 'S') res_fLatitude  *= -1.0;
//		if (tmp_words[5][0] == 'W') res_fLongitude *= -1.0;
//		float degrees = trunc(res_fLatitude / 100.0f);
//		float minutes = res_fLatitude - (degrees * 100.0f);
//		res_fLatitude = degrees + minutes / 60.0f;
//		degrees = trunc(res_fLongitude / 100.0f);
//		minutes = res_fLongitude - (degrees * 100.0f);
//		res_fLongitude = degrees + minutes / 60.0f;

		res_fLatitude = NMEA_calcLatitude(tmp_words[2],tmp_words[3]);
		res_fLongitude = NMEA_calcLongitude(tmp_words[4],tmp_words[5]);
		NMEA.Latitude = res_fLatitude;
		NMEA.Longitude = res_fLongitude;
		// parse number of satellites
		res_nSatellitesUsed = (int)string2float(tmp_words[7]);
		NMEA.SatsInUse = atoi(tmp_words[7]);
		// parse HDOP
                res_nHDOP = NMEA_floatStrToInt(tmp_words[8],1);
                NMEA.HDOP = res_nHDOP;

		// parse altitude
//		res_fAltitude = string2float(tmp_words[9]);
		res_fAltitude = NMEA_floatStrToInt( tmp_words[9], 1);
		NMEA.Altitude = res_fAltitude;
		// data ready
		m_bFlagDataReady = true;
		res_nNMEAcounter = res_nNMEAcounter +1;
		NMEA.Counter =  res_nNMEAcounter;

	}

// 8.8.2015 CB wird zur Zeit nicht benötigt
	/* $GPRMC
	 * note: a siRF chipset will not support magnetic headers.
	 * $GPRMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,ddmmyy,x.x,a*hh
	 * ex: $GPRMC,230558.501,A,4543.8901,N,02112.7219,E,1.50,181.47,230213,,,A*66,
	 *
	 * WORDS:
	 *  1	 = UTC of position fix
	 *  2    = Data status (V=navigation receiver warning)
	 *  3    = Latitude of fix
	 *  4    = N or S
	 *  5    = Longitude of fix
	 *  6    = E or W
	 *  7    = Speed over ground in knots
	 *  8    = Track made good in degrees True, Bearing This indicates the direction that the device is currently moving in,
	 *       from 0 to 360, measured in “azimuth”.
	 *  9    = UT date
	 *  10   = Magnetic variation degrees (Easterly var. subtracts from true course)
	 *  11   = E or W
	 *  12   = Checksum
	 */
//	if (mstrcmp(tmp_words[0], "$GPRMC") == 0) {
//		// Check data status: A-ok, V-invalid
//		if (tmp_words[2][0] == 'V') {
//			// clear data
//			res_fLatitude = 0;
//			res_fLongitude = 0;
////			m_bFlagDataReady = false;
//			return;
//		}
////		// parse time
////		res_nUTCHour = digit2dec(tmp_words[1][0]) * 10 + digit2dec(tmp_words[1][1]);
////		res_nUTCMin = digit2dec(tmp_words[1][2]) * 10 + digit2dec(tmp_words[1][3]);
////		res_nUTCSec = digit2dec(tmp_words[1][4]) * 10 + digit2dec(tmp_words[1][5]);
////		// parse latitude and longitude in NMEA format
////		res_fLatitude = string2float(tmp_words[3]);
////		res_fLongitude = string2float(tmp_words[5]);
////		// get decimal format
////		if (tmp_words[4][0] == 'S') res_fLatitude  *= -1.0;
////		if (tmp_words[6][0] == 'W') res_fLongitude *= -1.0;
////		float degrees = trunc(res_fLatitude / 100.0f);
////		float minutes = res_fLatitude - (degrees * 100.0f);
////		res_fLatitude = degrees + minutes / 60.0f;
////		degrees = trunc(res_fLongitude / 100.0f);
////		minutes = res_fLongitude - (degrees * 100.0f);
////		res_fLongitude = degrees + minutes / 60.0f;
//		//parse speed
//		// The knot (pronounced not) is a unit of speed equal to one nautical mile (1.852 km) per hour
//		res_fSpeed = NMEA_floatStrToInt(tmp_words[7],1);
//		res_fSpeed /= 1.852; // convert to km/h
//		// parse bearing
//		res_fBearing = NMEA_floatStrToInt(tmp_words[8],1);
////		// parse UTC date
////		res_nUTCDay = digit2dec(tmp_words[9][0]) * 10 + digit2dec(tmp_words[9][1]);
////		res_nUTCMonth = digit2dec(tmp_words[9][2]) * 10 + digit2dec(tmp_words[9][3]);
////		res_nUTCYear = digit2dec(tmp_words[9][4]) * 10 + digit2dec(tmp_words[9][5]);
//
//		// data ready
//                res_nNMEAcounter = res_nNMEAcounter +1;
//                NMEA.Counter =  res_nNMEAcounter;
//		m_bFlagDataReady = true;
//	}
}



//--------------------------------------------------------------
// NMEA latitudes are in the form ddmm.mmmmm, we want an integer in 1E-7 degree steps
//--------------------------------------------------------------
int32_t NMEA_calcLatitude( const char *s, const char *NS)
{
    int32_t deg;
    int32_t min;

    //lcdx_puts_at(0,5,NS,0,0,0);
    deg = (s[0] - '0') * 10 + s[1] - '0';                           // First 2 chars are full degrees
    min = NMEA_floatStrToInt( &s[2], 6) / 6;                         // Minutes * 1E5 * 100 / 60 = Minutes * 1E6 / 6 = 1E-7 degree steps

    deg = deg * 10000000 + min;

    if( *NS == 'S' )
    {
        deg = -deg;
    }

    return deg;
}



//--------------------------------------------------------------
// NMEA longitudes are in the form dddmm.mmmmm, we want an integer in 1E-7 degree steps
//--------------------------------------------------------------
int32_t NMEA_calcLongitude( const char *s, const char *WE)
{
    int32_t deg;
    int32_t min;

    //lcdx_puts_at(10,5,WE,0,0,0);
    deg = ((s[0] - '0') * 10 + s[1] - '0') * 10 + s[2] - '0';       // First 3 chars are full degrees
    min = NMEA_floatStrToInt( &s[3], 6) / 6;                         // Minutes * 1E5 * 100 / 60 = Minutes * 1E6 / 6 = 1E-7 degree steps

    deg = deg * 10000000 + min;

    if( *WE == 'W' )
    {
        deg = -deg;
    }

    return deg;
}

//--------------------------------------------------------------
void NMEA_getTime( const char *s)
{
    uint8_t sem = 0;
    uint8_t i;

    for( i=0; i<6; i++ )
    {
        NMEA.Time[sem++] = s[i];

        if( i==1 || i==3)
            NMEA.Time[sem++] = ':';

    }
    NMEA.Time[sem] = '\0';
}

//--------------------------------------------------------------
/*
 * returns base-16 value of chars '0'-'9' and 'A'-'F';
 * does not trap invalid chars!
 */	
int digit2dec(char digit)
  {
	if ((int)digit >= 65)
		return ((int)digit - 55);
	else
         return ((int)digit - 48);
 }

//--------------------------------------------------------------
/* returns base-10 value of zero-terminated string
 * that contains only chars '+','-','0'-'9','.';
 * does not trap invalid strings! 
 */
float string2float(char* s) {
	long  integer_part = 0;
	float decimal_part = 0.0;
	float decimal_pivot = 0.1;
	bool isdecimal = false, isnegative = false;
	
	char c;
	while ( ( c = *s++) )  { 
		// skip special/sign chars
		if (c == '-') { isnegative = true; continue; }
		if (c == '+') continue;
		if (c == '.') { isdecimal = true; continue; }
		
		if (!isdecimal) {
			integer_part = (10 * integer_part) + (c - 48);
		}
		else {
			decimal_part += decimal_pivot * (float)(c - 48);
			decimal_pivot /= 10.0;
		}
	}
	// add integer part
	decimal_part += (float)integer_part;
	
	// check negative
	if (isnegative)  decimal_part = - decimal_part;

	return decimal_part;
}


//--------------------------------------------------------------
// Trying to avoid floating point maths here. Converts a floating point string to an integer with a smaller unit
// i.e. floatStrToInt("4.5", 2) = 4.5 * 1E2 = 450
//--------------------------------------------------------------
int32_t NMEA_floatStrToInt( const char *s, int32_t power1 )
{
    char    *endPtr;
    int32_t  v;

    v = strtol(s, &endPtr, 10);

    if( *endPtr == '.' )
    {
        for (s = endPtr + 1; *s && power1; s++)
        {
            v = v * 10 + (*s - '0');
            --power1;
        }
    }

    if( power1 )
    {
        // Table to avoid multiple multiplications
        v = v * getPower(power1);
    }

    return v;
}


//--------------------------------------------------------------
int mstrcmp(const char *s1, const char *s2)
{
	while((*s1 && *s2) && (*s1 == *s2))
	s1++,s2++;
	return *s1 - *s2;
}

//--------------------------------------------------------------
bool NMEA_isdataready() {
	return m_bFlagDataReady;
}

int NMEA_getHour() {
	return res_nUTCHour;
}	
int NMEA_getMinute() {
	return res_nUTCMin;
}
int NMEA_getSecond() {
	return res_nUTCSec;
}
int NMEA_getDay() {
	return res_nUTCDay;
}
int NMEA_getMonth() {
	return res_nUTCMonth;
}
int NMEA_getYear() {
	return res_nUTCYear;
}

int32_t NMEA_getLatitude() {
	return res_fLatitude;
}

int32_t NMEA_getLongitude() {
	return res_fLongitude;
}

uint8_t NMEA_getSatellites() {
	return res_nSatellitesUsed;
}

uint8_t NMEA_getGPSfix() {
        return res_nGPSfix;
}
int16_t NMEA_getHDOP() {
        return res_nHDOP;
}

int16_t  NMEA_getAltitude() {
	return res_fAltitude;
}

float NMEA_getSpeed() {
	return res_fSpeed;
}

int16_t NMEA_getBearing() {
	return res_fBearing;
}
uint32_t NMEA_getNMEAcounter() {
        return res_nNMEAcounter;
}

uint32_t NMEA_getCRCerror() {
        return res_nCRCerror;
}
