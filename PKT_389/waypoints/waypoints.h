/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

#ifndef _WAYPOINTS_H
#define _WAYPOINTS_H

//#include "ubx.h"
#include "../mk-data-structs.h"
#define POINT_TYPE_INVALID 255
#define POINT_TYPE_WP	0
#define POINT_TYPE_POI	1
#define INVALID         0x00

//typedef struct
//{
//	int32_t Longitude;  // in 1E-7 deg
//	int32_t Latitude;	// in 1E-7 deg
//	int32_t Altitude;	// in mm
//	uint8_t Status;// validity of data
//} __attribute__((packed)) GPS_Pos_t;


//typedef struct
//{
// 	GPS_Pos_t Position;		// the gps position of the waypoint, see ubx.h for details
//	int16_t Heading;			// orientation, 0 no action, 1...360 fix heading, neg. = Index to POI in WP List
//	uint8_t  ToleranceRadius;	// in meters, if the MK is within that range around the target, then the next target is triggered
//	uint8_t  HoldTime;			// in seconds, if the was once in the tolerance area around a WP, this time defines the delay before the next WP is triggered
//	uint8_t  Event_Flag;			// future implementation
//	uint8_t  Index;              // to indentify different waypoints, workaround for bad communications PC <-> NC
//	uint8_t  Type;				// typeof Waypoint
//	uint8_t  WP_EventChannelValue;  //
//	uint8_t  AltitudeRate;		// rate to change the setpoint
//	uint8_t  reserve[8];		    // reserve
//} __attribute__((packed)) Point_t;

// Init List, return TRUE on success
uint8_t PointList_Init(void);
// Clear List, return TRUE on success
uint8_t PointList_Clear(void);
// Returns number of points in the list
uint8_t PointList_GetCount(void);
// return pointer to point at position
Point_t* PointList_GetAt(uint8_t index);
// set a point in the list at index, returns its index on success, else 0
uint8_t PointList_SetAt(Point_t* pPoint);
// goto the first WP in the list and return pointer to it
Point_t* PointList_WPBegin(void);
// goto the last WP in the list and return pointer to it
Point_t* PointList_WPEnd(void);
// goto next WP in the list and return pointer to it
Point_t* PointList_WPNext(void);
// enables/disables waypoint function
void PointList_WPActive(uint8_t set);
// returns pointer to actual POI
Point_t* PointList_GetPOI(void);


#endif // _WAYPOINTS_H
