/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//############################################################################
//# HISTORY  gps.c
//#
//#
//#
//#
//#
//#
//#
//#
//# 22.09.2015 Starter
//# - followme_add_offset(...) und followme_calculate_offset getestet mit PKT
//# - add my_abs(...)
//#
//# 20.09.2015 Starter
//# - add Routine um einen Offset in Meter zu den aktuellen Koordinaten dazurechnen
//#    followme_calculate_offset(...)
//#
//# 03.08.2015 cebra
//# - add: Routine um aus gegebenen Koordinaten mit Abstand und Winkel eine ZielKoordinate zu berechnen
//#    int nmea_move_horz(
//#    const nmeaPOS *start_pos,   /**< Start position in radians */
//#    nmeaPOS *end_pos,           /**< Result position in radians */
//#    double azimuth,             /**< Azimuth (degree) [0, 359] */
//#    double distance)             /**< Distance (km) */
//#
//# 27.06.2014 OG - NEU
//# - chg: auf #include "../gps/mymath.h" angepasst
//#
//# 20.06.2014 OG - NEU
//############################################################################


#include "../cpu.h"
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <math.h>
#include "../main.h"

#include "../mk-data-structs.h"
#include "../gps/mymath.h"
#include "gps.h"


/*
// definiert in: mk_data-stucts.h
typedef struct
{
    u16 Distance;       // distance to target in cm
    s16 Bearing;        // course to target in deg
} __attribute__((packed)) GPS_PosDev_t;
*/

/*
// definiert in: mk_data-stucts.h
typedef struct
{
    s32 Longitude;      // in 1E-7 deg
    s32 Latitude;       // in 1E-7 deg
    s32 Altitude;       // in mm
    u8 Status;          // validity of data
} __attribute__((packed)) GPS_Pos_t;
*/


//--------------------------------------------------------------

#define NMEA_PI                     (3.141592653589793)             /**< PI value */
#define NMEA_PI180                  (NMEA_PI / 180)                 /**< PI division by 180 */
#define NMEA_EARTHRADIUS_KM         (6378)                          /**< Earth's mean radius in km */
#define R                           (6371)
#define NMEA_EARTHRADIUS_M          (NMEA_EARTHRADIUS_KM * 1000)    /**< Earth's mean radius in m */
#define NMEA_EARTH_SEMIMAJORAXIS_M  (6378137.0)                     /**< Earth's semi-major axis in m according WGS84 */
#define NMEA_EARTH_SEMIMAJORAXIS_KM (NMEA_EARTHMAJORAXIS_KM / 1000) /**< Earth's semi-major axis in km according WGS 84 */
#define NMEA_EARTH_FLATTENING       (1 / 298.257223563)             /**< Earth's flattening according WGS 84 */
#define NMEA_DOP_FACTOR             (5)                             /**< Factor for translating DOP to meters */


// Definitonen für FollowMeStep2


# define NMEA_POSIX(x)  x



/**
 * \fn nmea_degree2radian
 * \brief Convert degree to radian
 */
double nmea_degree2radian(double val)
{ return (val * NMEA_PI180); }


//------------------------------------------------------------------------------------------
nmeaPOS NMEApos;
nmeaPOS NMEATarget;

/**
 * \brief Horizontal move of point position
 */
int nmea_move_horz(
    const nmeaPOS *start_pos,   /**< Start position in radians */
    nmeaPOS *end_pos,           /**< Result position in radians */
    double azimuth,             /**< Azimuth (degree) [0, 359] */
    double distance             /**< Distance (km) */
    )
{
    nmeaPOS p1 = *start_pos;
    int RetVal = 1;

    distance /= NMEA_EARTHRADIUS_KM; /* Angular distance covered on earth's surface */
    azimuth = nmea_degree2radian(azimuth);

    end_pos->latitude = asin( sin(p1.latitude) * cos(distance) + cos(p1.latitude) * sin(distance) * cos(azimuth));

    end_pos->longitude = p1.longitude + atan2( sin(azimuth) * sin(distance) * cos(p1.latitude), cos(distance) - sin(p1.latitude) * sin(end_pos->latitude));

    if(NMEA_POSIX(isnan)(end_pos->latitude) || NMEA_POSIX(isnan)(end_pos->longitude))
    {
        end_pos->latitude = 0; end_pos->longitude = 0;
        RetVal = 0;
    }

    return RetVal;
}





// Fügt der Startpostition einen Offset hinzu und gibt sie als Zielposition zurück
//
// Benutzt die c_cos_8192 der FC
// TODO: move to followme.c
// TODO: *8192 optimieren

uint8_t followme_add_offset(
    const nmeaPOS *pPktPos,     		/**< Start position in radians */
    nmeaPOS *target_pos,        		/**< Result position in radians */
	positionOffset *pFollowMeOffset		/**< Position Offset */
    )
{
	nmeaPOS pktPos = *pPktPos;
	positionOffset followMeOffset = * pFollowMeOffset;
	
	if( followMeOffset.type == 0 )
	{
		target_pos->latitude = pktPos.latitude + ( ( followMeOffset.latitude * ( LAT_DIV / FOLLOWME_M2DEG ) ) ) / 1000;
		target_pos->longitude = pktPos.longitude + ( ( followMeOffset.longitude * ( LONG_DIV / FOLLOWME_M2DEG ) * (8192/1000) ) / my_abs( c_cos_8192( (pktPos.latitude / LONG_DIV ) ) ) );
		return 1;
	}
	else
		if( followMeOffset.type == 1 )
		{
			target_pos->latitude = pktPos.latitude + followMeOffset.latitude;
			target_pos->longitude = pktPos.longitude + followMeOffset.longitude;
			return 1;
		}
	return 0;
}


// schlanke abs-Methode
// TODO: move to mymath.h
int16_t my_abs(int16_t input)
{
	if(input < 0)
		return -input;
	else
		return input;
}


// Rechnet einen Offset aus Radius und Winkel nach Lat/Long
// Benutzt die c_cos_8192 und c_sin_8192 der FC
// TODO: move to followme.c

uint8_t followme_calculate_offset(
		int32_t radius,				// in mm
		int16_t angle,				// in Grad °
		positionOffset *followMeOffset
		)
{
	angle %= 360;			// map angle to 0° - 360°

	followMeOffset->latitude = ( radius * c_cos_8192( angle ) ) / 8192;
	followMeOffset->longitude = ( radius * c_sin_8192( angle ) ) / 8192;

	return 1;
}


//###############################################################################################



//--------------------------------------------------------------
GPS_PosDev_t gps_Deviation( GPS_Pos_t pos1, GPS_Pos_t pos2 )
{
    int32_t      lat1, lon1, lat2, lon2;
    int32_t      d1, dlat;
    GPS_PosDev_t PosDev;

    lon1 = pos1.Longitude;
    lat1 = pos1.Latitude;

    lon2 = pos2.Longitude;
    lat2 = pos2.Latitude;

    d1   = (1359 * (int32_t)(c_cos_8192((lat1 + lat2) / 20000000)) * ((lon1 - lon2)/10))/ 10000000;
    dlat = (1113 * (lat1 - lat2) / 10000);

    PosDev.Bearing  = (my_atan2(d1, dlat) + 540) % 360;         // 360 +180 besserer Vergleich mit MkCockpit
    PosDev.Distance = sqrt32( d1 * d1 + dlat * dlat );          //

    return PosDev;
}



