//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//############################################################################
//# HISTORY  gps.h
//#
//#
//#
//#
//#
//#
//#
//#
//# 22.09.2015 Starter
//# - PKT-Pos von lat lon auf latitude und longitude umbenannt
//#
//# 20.06.2014 OG - NEU
//############################################################################


#ifndef GPS_H_
#define GPS_H_

/**
 * Position data in fractional degrees or radians
 */
typedef struct _nmeaPOS
{
    int32_t latitude;          // Latitude in 1e-7 Grad
    int32_t longitude;         // Longitude in 1e-7 Grad

} nmeaPOS;



typedef struct _positionOffset
{
	int32_t latitude;			// latitude offset
	int32_t longitude;			// longitude offset
	uint8_t type;				// 0 = offset in mm
								// 1 = offset in 1e-7 Grad
}positionOffset;


GPS_PosDev_t gps_Deviation(
	GPS_Pos_t pos1,
	GPS_Pos_t pos2
	);


int nmea_move_horz(
    const nmeaPOS *start_pos,   /**< Start position in radians */
    nmeaPOS *end_pos,           /**< Result position in radians */
    double azimuth,             /**< Azimuth (degree) [0, 359] */
    double distance             /**< Distance (km) */
    );


int nmea_move_horz1(
    const nmeaPOS *start_pos,   /**< Start position in radians */
    nmeaPOS *end_pos,           /**< Result position in radians */
    double azimuth,             /**< Azimuth (degree) [0, 359] */
    double distance             /**< Distance (km) */
    );


uint8_t followme_add_offset(
    const nmeaPOS *pPktPos,             /**< Start position in radians */
    nmeaPOS *target_pos,                /**< Result position in radians */
    positionOffset *followMeOffset      /**< Position Offset in Millimeters */
    );

uint8_t followme_calculate_offset(
    int32_t radius,
    int16_t angle,
    positionOffset *followMeOffset
    );

int16_t my_abs(int16_t input);


#define LONG_DIV                    10000000
#define LAT_DIV                     LONG_DIV
#define FOLLOWME_M2DEG              111111
#define FOLLOWME_ROUND_100          100



#endif // #define GPS_H_
