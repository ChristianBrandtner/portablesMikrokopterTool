/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

#ifndef _TRACKING_H_
#define _TRACKING_H_

#define TRACKING_RSSI		1
#define TRACKING_GPS		2
#define TRACKING_MKCOCKPIT	3
#define TRACKING_NMEA		4

#define DLEFT				0
#define DRIGHT				1
#define AltFaktor			22.5

#define PAN_SERVO_CORRECT	3.125
#define TILT_SERVO_CORRECT	3.125

typedef void (*Displ_Fnct_t)( uint16_t );


//typedef struct {
//	int32_t distance;
//	int16_t bearing;
//}geo_t;

//typedef struct {
//	double				Home_Lon;		// in degrees
//	double				Home_Lat;		// in degrees
//	int32_t				Home_Lon7;		// in 1E-7 degrees
//	int32_t				Home_Lat7;		// in 1E-7 degrees
//	int32_t				Home_Alt;		// in mm
//	// ermittelte Konstante aus Mittelposition Antenne geo.bearing - navi_data.CompassHeading
//	int16_t				direction;
//}__attribute__((packed)) HomePos_t;

#define INTERNAT_STEPS_255                                                      "255 "
#define INTERNAT_STEPS_1023                                                     "1023"


// kapeschi
void Setup_ServoTracking( void );
void Tracking_GPS(void);
void Tracking_NMEA(void);
void Tracking_RSSI(void);
void setNMEAdir(void);

#endif 
