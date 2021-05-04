/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//############################################################################
//# HISTORY  gpsmouse.c
//#
//# 30.07.2015 CB
//# - add: Anzeige von CRC Fehlern im NMEA Datensatz in GPSMouse_ShowData
//#
//# 28.07.2015 CB
//# - fix: Überprüfung von führenden 00 in der MAC-Adresse der BT Maus abgeschaltet
//#
//# 28.06.2014 OG
//# - fix: GPSMouse_ShowData() - wenn BT-Datenverlust dann immer GPSMouse_Disconnect()
//#        auch wenn waitsatfix Modus aktiv ist
//# - fix: GPSMouse_ShowData() - Rueckgabe von 0 wenn Verbindung bereits vorhanden war
//#        und dabei aber ein Verbindungsverlust zur BT GPS-Maus auftrat
//#
//# 27.06.2014 OG
//# - chg: GPSMouse_Disconnect() - Anzeige von BT-Datenverlust optimiert
//# - chg: GPSMouse_ShowData() - nach GPSMouse_Connect ein clear_key_all() eingefuegt
//#
//# 24.06.2014 OG
//# - add: GPSMouse_Connect(), GPSMouse_Disconnect()
//#
//# 23.06.2014 OG
//# - chg: GPSMouse_ShowData() - neue Parameter und Return-Codes fuer
//#        hoehere Flexibilitaet
//#
//# 22.06.2014 OG - NEU
//# ehemals BT_ShowGPSData() in setup.c
//############################################################################

#include "../cpu.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <util/delay.h>

#include "../main.h"
#include "../lcd/lcd.h"
#include "../setup/setup.h"
#include "../eeprom/eeprom.h"
#include "../lipo/lipo.h"
#include "../timer/timer.h"
#include "../bluetooth/fifo.h"
#include "../bluetooth/bluetooth.h"
#include "../uart/uart1.h"
#include "../utils/menuctrl.h"
#include "../utils/xutils.h"
#include "../pkt/pkt.h"
#include "../messages.h"
//#include "../gps/gps_nmea.h"
#include "../avr-nmea-gps-library/nmea.h"
#include "../gps/gpsmouse.h"


static const char STR_FIX0[]     PROGMEM = "  no";
static const char STR_FIX1[]     PROGMEM = " GPS";
static const char STR_FIX2[]     PROGMEM = "DGPS";
static const char STR_FIX6[]     PROGMEM = "esti";
static const char STR_FIX[]      PROGMEM = " ---";





//--------------------------------------------------------------
// retcode = GPSMouse_Connect()
//
// RUECKGABE:
//  -2: kein BTM222 eingebaut
//  -1: keine GPS Maus konfiguriert
//   0: Fehler bei connect, kein connect durchgeführt (evtl. GPS Maus nicht eingeschaltet)
//   1: verbunden
//--------------------------------------------------------------
int8_t GPSMouse_Connect( void )
{
    uint8_t status;

    if( !BT_CheckBTM222() )
    {
        // PKT_Message_P( text, error, timeout, beep, clearscreen)
        PKT_Message_P( strGet(STR_NOBTM222), true, 1000, true, true);           // 1000 = max. 10 Sekunden anzeigen; "kein BTM222 vorh."
        return -2;                                                              // -2: kein BTM222 eingebaut
    }

//      deaktiviert, MAC Adressen von Handys haben keine 00 in der MAC-Adresse
//    if( (Config.gps_UsedMac[5] == '0') || (Config.gps_UsedMac[6] == '0') )
//    {
        // PKT_Message_P( text, error, timeout, beep, clearscreen)
//        PKT_Message_P( strGet(STR_NOGPSMOUSE), true, 1000, true, true);         // 1000 = max. 10 Sekunden anzeigen; "keine GPS-Maus!"
//        return -1;                                                              // -1: keine GPS Maus konfiguriert
//    }


    //--------------------
    // verbinde BT GPS-Maus
    //--------------------
    PKT_Title( Config.gps_UsedDevName, true, true);                             // PKT_Title( text, lShowLipo, clearscreen )

    lcdx_printp_center( 2, strGet(STR_GPSMOUSECONNECT), MNORMAL, 0,1);          // "suche GPS-Maus"

    PKT_Gauge_Begin( 0 );                                                       // Gauge: 0 = Default fuer y verwenden

    set_BTOn();

    if( Config.BTIsSlave )
    {
        bt_downlink_init();
    }

    status = bt_connect( Config.gps_UsedMac );

    uart1_flush();
    fifo_clear(&in_fifo);

    UART1_receiveNMEA = true;                                                   // Uartflag setzen zur Erkennung der NMEA Datensätze

    PKT_Gauge_End();                                                            // Gauge: Ende

    if( !status )                                                               // keine Verbindung zum BT-Device
    {
        set_BTOff();

        // PKT_Message_P( text, error, timeout, beep, clearscreen)
        PKT_Message_P( strGet(STR_GPSMOUSECONNECTION), true, 1000, true, true); // 1000 = max. 10 Sekunden anzeigen; "GPS-Maus Verbindung"
        return 0;                                                               // 0: Fehler bei connect, kein connect durchgefÃ¼hrt (evtl. GPS Maus nicht eingeschaltet)
    }

    return 1;                                                                   // 1 = ok, Verbindung steht
}



//--------------------------------------------------------------
// GPSMouse_Disconnect()
//--------------------------------------------------------------
void GPSMouse_Disconnect( void )
{
    PKT_Title( Config.gps_UsedDevName, true, true);                             //PKT_Title( text, lShowLipo, clearscreen )

    //---------------------
    // GPS beenden
    //---------------------
    UART1_receiveNMEA = false;
    if( !receiveNMEA )                                                          // ggf. anders implementieren, z.B. via Parameter; aber erstmal geht's auch so
    {
      //lcdx_cls_row( y, mode, yoffs )
        lcdx_cls_row( 7, MINVERSX, 0 );
        lcdx_printp_center( 7, strGet(STR_BT_LOSTDATA), MINVERSX, 0,0);         // "BT Datenverlust"
        set_beep( 600, 0xffff, BeepNormal);
    }

    lcdx_printp_center( 2, strGet(STR_GPSMOUSEDISCONNECT), MNORMAL, 0,1);       // "trenne GPS-Maus"

    PKT_Gauge_Begin( 0 );                                                       // Gauge: 0 = Default fuer y verwenden

    receiveNMEA = false;                                                        // Uartflag zurücksetzen, es kommen keine NMEA Datensätze mehr

    if (!bt_disconnect());
    {
//        lcdx_printp_center( 2, PSTR("Error on disconnect"), MNORMAL, 0,1);       // Fehler beim Disconncet aufgetreten
//        _delay_ms(2000);                                                         // 09.08.2015 cebra, muss noch untersucht werden, ist aber jetzt kein Problem
    }

    set_BTOff();

    PKT_Gauge_End();                                                            // Gauge: Ende
}



//--------------------------------------------------------------
// retcode = GPSMouse_ShowData( mode, waitsatfix )
//
// zeigt GPS-Daten der aktuellen BT-Maus an
//
// PARAMETER:
//  show:
//     was soll in der 2. Zeile angezeigt werden
//     GPSMOUSE_SHOW_TIME || GPSMOUSE_SHOW_WAITSATFIX
//
//  waitsatifix:
//      0 = Verbindungsaufbau; Daten anzeigen und beenden der
//          BT-Verbindung (fuer setup.c)
//      n = Zeit (via timer) die nach einem erfolgreichen Satfix gewartet wird bis
//          GPSMouse_ShowData() beendet wird - die Verbindung zur GPS-Maus bleibt erhalten
//          und kann weiterverwendet werden! (z.B. in followme.c)
//
// RUECKGABE:
//  -2: kein BTM222 eingebaut
//  -1: keine GPS Maus konfiguriert
//   0: Fehler bei connect, kein connect durchgefÃ¼hrt (evtl. GPS Maus nicht eingeschaltet)
//   1: connect zur GPS Maus ok, Benutzer hat Anzeige beenden
//   2: connect zur GPS Maus ok, Satix hat Anzeige beendet
//
// zusammengefasst:
//  >0 : Ok
//  <=0: Fehler
//--------------------------------------------------------------
int8_t GPSMouse_ShowData( uint8_t show, uint16_t waitsatfix )
{
    uint8_t redraw = true;
    uint8_t state_posfix = 0;
    const char *pStr;
    int8_t yoffs;
    int8_t retcode;



    //--------------------
    // BT GPS-Maus verbinden
    //--------------------
    retcode = GPSMouse_Connect();
    if( retcode <= 0 )
    {
        return retcode;
    }
    timer_nmea_timeout = NMEA_TIMEOUT;                      // Timeout Timer setzen
    clear_key_all();



    //---------------------
    // zeige GPS-Daten
    //---------------------

    receiveNMEA  = true;


    do
    {
        //-------------------------
        // Screen redraw
        //-------------------------
        if( redraw )
        {
            PKT_Title( Config.gps_UsedDevName, true, true);                             //PKT_Title( text, lShowLipo, clearscreen )

            if( show == GPSMOUSE_SHOW_TIME )                                            // Zeile 2: GPS-Zeit
            {
                lcdx_printp_at( 6, 1, PSTR("["), MNORMAL, -1,2);                            // Mitte: GPS Time
                lcdx_printp_at(15, 1, PSTR("]"), MNORMAL, -2,2);                            // ..
                lcd_line( 0, 13,  34, 13, 1);                                               // ..
                lcd_line(92, 13, 127, 13, 1);                                               // ..
            }

            if( show == GPSMOUSE_SHOW_WAITSATFIX )                                      // Zeile 2: "warte auf Satfix..."
            {
                yoffs = 1;
                lcdx_cls_row( 1, MINVERS, yoffs);
                lcdx_printp_center( 1, strGet(STR_WAITSATFIX), MINVERS, 0,yoffs);           // "warte auf Satfix..."
            }

//            yoffs = 1;
//            lcdx_cls_row( 1, MINVERS, yoffs);
//            lcdx_printp_center( 1,strGet(STR_WAITNMEA), MINVERS, 0,yoffs);           // "warte auf NMEA..."

            lcdx_printp_at( 0, 2, PSTR("Fix:") , MNORMAL, 0,3);                         // Links; Fix
            lcdx_printp_at( 0, 3, PSTR("Sat:") , MNORMAL, 0,3);                         // Links; Sat
            lcdx_printp_at( 0, 4, PSTR("Alt:") , MNORMAL, 0,3);                         // Links; Alt

            lcdx_printp_at(11, 2, PSTR("RCnt:"), MNORMAL, 0,3);                         // Rechts: RCnt - Zaehler empfangener GPGGA Datenpakete
            lcdx_printp_at(11, 3, PSTR("CErr:"), MNORMAL, 0,3);                         // Rechts: RErr - Zaehler fehlerhafte NMEA Pakete
            lcdx_printp_at(11, 4, PSTR("HDOP:"), MNORMAL, 0,3);                         // Rechts: HDOP

            lcdx_cls_row( 5, MINVERS, 4);                                               // Hintergrund invers
            lcdx_printp_at( 1, 5, strGet(START_LASTPOS1), MINVERS, 0,4);                //   "Breitengr.  Längengr."

            lcd_rect( 0, (4*8)+11, 127, (2*8)+4, 1);                                    // Rahmen

            redraw = false;
        }


        //-------------------------
        // PKT-LiPo anzeigen
        //-------------------------
        show_Lipo();


        //-------------------------
        // PKT-Update geht hier nicht...
        // wegen der Hardware-Verbindung
        //-------------------------
        //if( PKT_CtrlHook() )
        //{
        //    redraw = true;
        //}

        //-------------------------
        //-------------------------

       if(NMEA_isdataready())                                                                        // NMEA Daten vorhanden?
        {

          if( show == GPSMOUSE_SHOW_TIME )
          {
              lcdx_print_at(7, 1, (uint8_t *)NMEA.Time, MNORMAL, -2,2);                          // Mitte: GPS Time
          }

          //--------------
          // Sat-Fix
          //--------------
          //writex_ndigit_number_u( 6, 2, NMEA.SatFix , 2,0, MNORMAL, 0,3);                       // Links: Fix (als Zahl)
          switch( NMEA.SatFix )
          {
              case 0:  pStr = STR_FIX0;   break;                                                  // kein Sat-Fix!
              case 1:  pStr = STR_FIX1;   break;                                                  // GPS - Fix: OK
              case 2:  pStr = STR_FIX2;   break;                                                  // DGPS - Fix: OK
              case 6:  pStr = STR_FIX6;   break;                                                  // Estimated - wird hier als nicht Ok angesehen weil nicht klar was es bedeutet
              default: pStr = 0;                                                                  // Sat-Fix Code unbekannt!
                       lcdx_printf_at_P( 4, 2, MNORMAL, 1,3, PSTR(" ? %d"), NMEA.SatFix);         //  -> Zahl wird angezeigt - ggf. recherchieren was da bedeutet (Fix ok oder nicht)
                       break;
          }

          if( pStr )
          {
              lcdx_printf_at_P( 4, 2, MNORMAL, 1,3, PSTR("%4S"), pStr);                           // GPS Fix als String ausgeben
          }

          if( NMEA.SatFix==1 || NMEA.SatFix==2 )                                                  // Sat-Fix OK  =>  einen Haken anzeigen
              lcdx_putc( 8, 2, SYMBOL_CHECK, MNORMAL, 2,3 );
          else
              lcdx_putc( 8, 2, ' ',          MNORMAL, 2,3 );


          //--------------
          // Sat-Anzahl
          //--------------
          writex_ndigit_number_u( 6, 3, NMEA.SatsInUse, 2,0, MNORMAL, 1,3);                       // Links: Sat-Anzahl
          if( NMEA.SatsInUse >= GPSMOUSE_MINSAT )                                                 //   Haken angezeigt wenn MINSAT ok
              lcdx_putc( 8, 3, SYMBOL_CHECK, MNORMAL, 2,3 );
          else
              lcdx_putc( 8, 3, ' ',          MNORMAL, 2,3 );


          //--------------
          // Altitude
          //--------------
          lcdx_printf_at_P( 4, 4, MNORMAL, 1,3, PSTR("%4.1d"), NMEA.Altitude);                    // Links: Altitude


          //--------------
          // empfangene GPGGA Datenpakete
          //--------------
          lcdx_printf_at_P(16, 2, MNORMAL, 0,3, PSTR("%5ld"), NMEA.Counter );                     // Rechts: Anzahl empfangener NMEA GPGGA-Pakete


          //--------------
          // NMEA CRC Fehler
          //--------------
          lcdx_printf_at_P(16, 3, MNORMAL, 0,3, PSTR("%5ld"), NMEA_getCRCerror() );              // Rechts: Anzahl CRC Fehler im NMEA GPGGA-Pakete
//          lcdx_printf_at( 15, 3, mode, 0, 3, "%3.5ld", (uint32_t)NMEA_getBearing());
//          writex_ndigit_number_u_10th(15, 3, NMEA_getBearing(), 4,0, MNORMAL, 0,3);

          //--------------
          // HDOP
          //--------------
          if( NMEA.HDOP == 0 )                                                                    // Rechts:: HDOP
            lcdx_printp_at(16, 4, PSTR("  ---"), MNORMAL, 0,3);
          else
            writex_ndigit_number_u_10th(16, 4, NMEA.HDOP, 4,0, MNORMAL, 0,3);


          //--------------
          // Lat / Lon
          //--------------
          writex_gpspos( 1, 6, NMEA.Latitude , MNORMAL, 0,6 );                                     // unten links: Latitude
          writex_gpspos(12, 6, NMEA.Longitude, MNORMAL, 0,6 );                                     // unten rechts: Longitude


          //--------------
          //--------------
          if( waitsatfix > 0 )
          {
              if( (NMEA.SatFix==1 || NMEA.SatFix==2) && (state_posfix==0) )
              {
                  state_posfix = 1;
                  timer = waitsatfix;                                                             // Verzoegerung bzgl. Exit (z.B. 600 = 6 Sekunden)
              }

              if( (NMEA.SatFix!=1 && NMEA.SatFix!=2) && (state_posfix==1) && (timer!=0) )
              {
                  state_posfix = 0;
              }

              if( (NMEA.SatFix==1 || NMEA.SatFix==2) && (state_posfix==1) && (timer==0) )
              {
                  clear_key_all();
                  return 2;                                                                       // 2: connect zur GPS Maus ok, Satix hat Anzeige beendet
              }
          }

      }

       if (timer_nmea_timeout == 0) receiveNMEA = false;

    } while( !get_key_press(1 << KEY_ESC) && receiveNMEA );



    //---------------------
    // GPS beenden
    //---------------------
    if( (waitsatfix == 0) || !receiveNMEA )
    {

       GPSMouse_Disconnect();
    }

    clear_key_all();
    return (receiveNMEA ? 1 : 0);                                                               // 1: connect zur GPS Maus ok, Benutzer hat Anzeige beenden
}


// save, Anzeige der Daten im Format der neuen NMEA Routinen, nach Umbau auf NMEA Structur nicht mehr notwendig
//              {
//
//
//                if( show == GPSMOUSE_SHOW_TIME )
//                {
////                    lcdx_print_at(7, 1, (uint8_t *)NMEA.Time, MNORMAL, -2,2);                          // Mitte: GPS Time
//
//                    writex_ndigit_number_u( 7, 1, NMEA_getHour(), 2,0, MNORMAL, -2,2);
//                    lcdx_printp_at( 9, 1, PSTR(":") , MNORMAL, -2,2);
//                    writex_ndigit_number_u( 10, 1, NMEA_getMinute(), 2,0, MNORMAL, -2,2);
//                    lcdx_printp_at( 12, 1, PSTR(":") , MNORMAL, -2,2);
//                    writex_ndigit_number_u( 13, 1, NMEA_getSecond(), 2,0, MNORMAL, -2,2);
//
//                }
//
//                //--------------
//                // Sat-Fix
//                //--------------
//                //writex_ndigit_number_u( 6, 2, NMEA.SatFix , 2,0, MNORMAL, 0,3);                       // Links: Fix (als Zahl)
//                switch( NMEA_getGPSfix() )
//                {
//                    case 0:  pStr = STR_FIX0;   break;                                                  // kein Sat-Fix!
//                    case 1:  pStr = STR_FIX1;   break;                                                  // GPS - Fix: OK
//                    case 2:  pStr = STR_FIX2;   break;                                                  // DGPS - Fix: OK
//                    case 6:  pStr = STR_FIX6;   break;                                                  // Estimated - wird hier als nicht Ok angesehen weil nicht klar was es bedeutet
//                    default: pStr = 0;                                                                  // Sat-Fix Code unbekannt!
//                             lcdx_printf_at_P( 4, 2, MNORMAL, 1,3, PSTR(" ? %d"), NMEA_getGPSfix());         //  -> Zahl wird angezeigt - ggf. recherchieren was da bedeutet (Fix ok oder nicht)
//                             break;
//                }
//
//                if( pStr )
//                {
//                    lcdx_printf_at_P( 4, 2, MNORMAL, 1,3, PSTR("%4S"), pStr);                           // GPS Fix als String ausgeben
//                }
//
//                if( NMEA_getGPSfix()==1 || NMEA_getGPSfix()==2 )                                                  // Sat-Fix OK  =>  einen Haken anzeigen
//                    lcdx_putc( 8, 2, SYMBOL_CHECK, MNORMAL, 2,3 );
//                else
//                    lcdx_putc( 8, 2, ' ',          MNORMAL, 2,3 );
//
//
//                //--------------
//                // Sat-Anzahl
//                //--------------
//                writex_ndigit_number_u( 6, 3, NMEA_getSatellites(), 2,0, MNORMAL, 1,3);                       // Links: Sat-Anzahl
//                if( NMEA_getSatellites() >= GPSMOUSE_MINSAT )                                                 //   Haken angezeigt wenn MINSAT ok
//                    lcdx_putc( 8, 3, SYMBOL_CHECK, MNORMAL, 2,3 );
//                else
//                    lcdx_putc( 8, 3, ' ',          MNORMAL, 2,3 );
//
//
//                //--------------
//                // Altitude
//                //--------------
//                lcdx_printf_at_P( 4, 4, MNORMAL, 1,3, PSTR("%4.1d"), NMEA_getAltitude());                    // Links: Altitude
//
//
//                //--------------
//                // empfangene GPGGA Datenpakete
//                //--------------
//                lcdx_printf_at_P(16, 2, MNORMAL, 0,3, PSTR("%5ld"), NMEA_getNMEAcounter() );                     // Rechts: Anzahl empfangener NMEA GPGGA-Pakete
//
//
//                //--------------
//                // NMEA CRC Fehler
//                //--------------
//                lcdx_printf_at_P(16, 3, MNORMAL, 0,3, PSTR("%5ld"), NMEA_getCRCerror() );                     // Rechts: Anzahl empfangener NMEA GPGGA-Pakete
//
////                writex_ndigit_number_u(1, 5, UART1_GPGGA   , 4,0, MNORMAL, 0,3);
////                writex_ndigit_number_u(5, 5, bt_overrun   , 4,0, MNORMAL, 0,3);
////                writex_ndigit_number_u(10, 5, bt_bufferoverflow   , 4,0, MNORMAL, 0,3);
//
//
//                //--------------
//                // HDOP
//                //--------------
//                if(NMEA_getHDOP() == 0 )                                                                    // Rechts:: HDOP
//                  lcdx_printp_at(16, 4, PSTR("  ---"), MNORMAL, 0,3);
//                else
//                  writex_ndigit_number_u_10th(16, 4, NMEA_getHDOP(), 4,0, MNORMAL, 0,3);
//
//
//                //--------------
//                // Lat / Lon
//                //--------------
//                writex_gpspos( 1, 6, NMEA_getLatitude() , MNORMAL, 0,6 );                                     // unten links: Latitude
//                writex_gpspos(12, 6, NMEA_getLongitude(), MNORMAL, 0,6 );                                     // unten rechts: Longitude
//
//
//                //--------------
//                //--------------
//                if( waitsatfix > 0 )
//                {
//                    if( (NMEA_getGPSfix()==1 || NMEA_getGPSfix()==2) && (state_posfix==0) )
//                    {
//                        state_posfix = 1;
//                        timer = waitsatfix;                                                             // Verzoegerung bzgl. Exit (z.B. 600 = 6 Sekunden)
//                    }
//
//                    if( (NMEA_getGPSfix()!=1 && NMEA_getGPSfix()!=2) && (state_posfix==1) && (timer!=0) )
//                    {
//                        state_posfix = 0;
//                    }
//
//                    if( (NMEA_getGPSfix()==1 || NMEA_getGPSfix()==2) && (state_posfix==1) && (timer==0) )
//                    {
//                        clear_key_all();
//                        return 2;                                                                       // 2: connect zur GPS Maus ok, Satix hat Anzeige beendet
//                    }
//                }
//
//          }

