//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//############################################################################
//# HISTORY  gpsmouse.h
//#
//# 24.06.2014 OG
//# - chg: Parameteraenderung bei GPSMouse_ShowData()
//# - add: GPSMouse_Connect(), GPSMouse_Disconnect()
//#
//# 22.06.2014 OG
//# - add: GPSMouse_ShowData()
//# - add: #define GPSMOUSE_SHOWDATA, GPSMOUSE_WAITSATFIX
//#
//# 22.06.2014 OG - NEU
//############################################################################


#ifndef GPSMOUSE_H_
#define GPSMOUSE_H_


//----------------------------------
// GPSMouse_ShowData() - show
//----------------------------------
#define GPSMOUSE_SHOW_TIME          1
#define GPSMOUSE_SHOW_WAITSATFIX    2


//----------------------------------
// minimal akzeptierte Sat-Zahl fuer
// Steuerungsfunktionen des MK
//----------------------------------
#define GPSMOUSE_MINSAT             6


//----------------------------------
// Export
//----------------------------------
int8_t GPSMouse_Connect( void );
void   GPSMouse_Disconnect( void );
int8_t GPSMouse_ShowData( uint8_t show, uint16_t waitsatfix );



#endif // #define GPSMOUSE_H_
