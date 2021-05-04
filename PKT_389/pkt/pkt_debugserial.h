/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*
 * pkt_debugserial.h
 *
 *  Created on: 25.09.2015
 *      Author: cebra
 */

#ifndef PKT_PKT_DEBUGSERIAL_H_
#define PKT_PKT_DEBUGSERIAL_H_




void usbx_write_gpspos( int32_t GPSpos, uint8_t mode );
void usbx_print( uint8_t *text, uint8_t mode);
void usbx_printf( uint8_t mode, const char *format, ... );
void usbx_printp( const char *text, uint8_t mode);
void PKT_Debugfunktion(void);
#endif /* PKT_PKT_DEBUGSERIAL_H_ */
