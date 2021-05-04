/*
 * tracking.h
 *
 *  Created on: 13.02.2012
 *      Author: cebra
 */

//############################################################################
//# HISTORY  tracking.h
//#
//# 27.06.2014 OG
//# - Entkernt: verschiedene Variablen und Funktionen geloescht
//#
//# 16.06.2014 OG
//# - add: NMEA_GPGGA_counter (zaehlt empfangene GPGGA-Pakete)
//# - add: Source-Historie
//############################################################################


#ifndef TRACKING_H_
#define TRACKING_H_


#define REPEAT                  1
#define REPEAT_MIN              1
#define REPEAT_MAX              100
#define PAUSE                   10
#define PAUSE_MIN               4       // mindestens 400ms, da mechanischer Servo-Lauf zur Position berücksichtigt werden muss
#define PAUSE_MAX               20      // Pause pro Links-, Mittel- und Rechtsposition 10*100ms
#define PAUSE_STEP              0
#define PAUSE_STEP_MIN          0       // Pause bei jeden Servoschritt in ms
#define PAUSE_STEP_MAX          200

// Antennen-Nachführung
#define TRACKING_MIN            0       // aus, TRACKING_RSSI, TRACKING_GPS, TRACKING_MKCOCKPIT, TRACKING_NMEA
#define TRACKING_MAX            4

// Antennen-Nachführung per RSSI
#define TRACKING_HYSTERESE      40      // Hysterese bevor Tracking bei Richtungswechsel anspricht
#define TRACKING_HYST_MIN       0
#define TRACKING_HYST_MAX       100

#define FC_FLAG_MOTOR_RUN       0x01
#define FC_FLAG_FLY             0x02
#define FC_FLAG_CALIBRATE       0x04
#define FC_FLAG_MOTOR_START     0x08



typedef struct {
        int32_t distance;
        int16_t bearing;
} geo_t;



typedef struct {
    int32_t   Home_Lon;          // in 1E-7 degrees
    int32_t   Home_Lat;          // in 1E-7 degrees
    int32_t   Home_Alt;          // in mm
    int16_t   direction;         // ermittelte Konstante aus Mittelposition Antenne geo.bearing - navi_data.CompassHeading
} __attribute__((packed)) HomePos_t;



extern uint8_t coldstart;                           // Flag erstmaliger MK-Start(Motore) nur nach GPS-Fix


#ifdef USE_TRACKING

void PKT_tracking(void);

#endif // #ifdef USE_TRACKING



#endif // TRACKING_H_
