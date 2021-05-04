/**
 * source for the Bluetooth driver
 * @file bluetooth.c
 * @author Linus Lotz<lotz@in.tum.de>
 * @author Salomon Sickert
 */


//############################################################################
//# HISTORY  bluetooth.c
//#
//#
//# 02.08.2015 CB
//# - chg: bt_showsettings(), angepasst und vollendet, zur Zeit nur im Mastermodus des BTM222 nutzbar
//#
//# 30.06.2014 OG
//# - chg: bt_discover() wieder auf (berechnete) Progressbar umgestellt
//#
//# 26.06.2014 OG
//# - del: bt_receiveNMEA() - jetzt in gps/nmea.c
//# - chg: bt_discover(), bt_searchDevice() - stark geaendert; Timeout geht jetzt
//#        nach Zeit und Abbruch durch Benutzer moeglich
//# - fix: BT-Devicesuche - wenn bei einer Suche Devices gefunden wurden und bei
//#        einer nachfolgenden Suche weniger (z.B. weil ein Device ausgeschaltet wurde)
//#        dann wurden zuviele Devices angezeigt; fixed
//#
//# 25.06.2014 OG
//# - chg: bt_discover(), bt_searchDevice()
//#
//# 22.06.2014 OG
//# - chg: Code-Formattierung
//#
//# 21.06.2014 CB
//# - fix: Fehler in "copy_DevName", zwei Leerzeichen am Anfang beendeten sofort den Namen, Namen war dann leer
//# - fix: Umbau der Timeoutfunktion bei der Devicesuche, wenn kein BT Device in Reichweite wurde die Suche nicht beendet
//# - add: Progressbar bei BT Device Suche
//#
//# 19.06.2014 OG
//# - chg: Code-Formattierung
//#
//# 16.06.2014 OG
//# - etliche Aenderungen und Funktionsumbenennungen - vorallem rund um den
//#   Bereich BT-Init
//# - viel Code-Layout (noch nicht abgeschlossen)
//#
//# 08.06.2014 OG
//# - del: BT_New_Baudrate
//#
//# 30.01.2014 OG
//# - add: Unterstuetzung fuer USE_BLUETOOTH
//#
//# 2013 Cebra
//# - Erweiterung um BT Local-ID Abfrage
//#
//# 26.06.2013 Cebra
//# - chg: Modulumschaltung fuer WiFlypatch geaendert
//#
//# 24.06.2013 OG
//# - add: bt_fixname() - korrigiert ggf. fehlehaften BT-Namen in Config.bt_name
//#
//# 24.06.2013 OG
//# - chg: bt_init(): Code-Layout, Code-Struktur, ggf. Debug-Ausgaben
//# - chg: bt_searchmodul(): Code-Layout und Ausgabe
//# - chg: send_cmd(): Handling von BT_SET_NAME & Code-Layout
//# - add: define DEBUG_BT
//#
//# 23.05.2013 OG
//# - chg: Timeout reduziert in bt_disconnect() (trennen der BT-Verbindung)
//# - fix: copy_DevName() Range angepasst und 0 Terminierung
//#
//# 24.04.2013 OG
//# - chg: Auskommentiert: #define __DELAY_BACKWARD_COMPATIBLE__
//#        das wird bereits vom Compiler-Symbol dieses PKT-Projektes definiert
//#        und erzeugt nur zusaetzliche Warnings
//#
//# 03.04.2013 Cebra
//# - chg: Init Routinen BT überarbeitet, es kam zu falschen Baudrateneinstellungen
//#        beim BTM-222 ohne Hinweis, mehr Displayinformationen beim Init
//############################################################################


#include <string.h>
#include "../cpu.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

#include "bluetooth.h"
#include "../main.h"


//----------------------------------------------------------------------------

#ifdef USE_BLUETOOTH        // 30.01.2014 OG
                            // brutal hier den Source ausklammern, damit man an anderen Stellen des PKT
                            // den BT-Code durch den Compiler entdeckt
                            // ansonsten kann es passieren, dass einiges an BT-Code unenddeckt einkompiliert wird
                            // -> bei den ersten Test's waren das immerhin ca. 10 KByte durch unenddeckten Code...

//----------------------------------------------------------------------------

//#define DEBUG_BT
//#ifdef HWVERSION3_9

#include "../uart/uart1.h"
#include "../uart/usart.h"
#include "../timer/timer.h"
#include "fifo.h"
#include "error.h"
#include "../lcd/lcd.h"
#include "../eeprom/eeprom.h"
#include "../setup/setup.h"
#include "bluetooth.h"
#include "../tracking/tracking.h"
#include "../utils/xutils.h"

#include "../messages.h"
#include "../pkt/pkt.h"
#include "../lipo/lipo.h"
#include "../utils/menuctrl.h"
#include "../utils/scrollbox.h"

//############################################################################
//############################################################################


//#define BT_CMD_TIMEOUT_MS 2000
#define BT_CMD_TIMEOUT_MS   500         // 15.06.2014 OG: Timeout reduziert von 2000 auf 500


static const char STR_BTM222INIT[]    PROGMEM = "BTM222 Init";


//#define SaveMem

//
// Baudrate for the UART-connection to the BTM-222 on SQUIRREL
//

#define SQUIRREL

#ifdef SQUIRREL
#define UART_BAUD_RATE      19200
#endif

#ifdef NUT
#define UART_BAUD_RATE      19200
#endif


typedef enum {
    BT_RAW,
    BT_DATA,
    BT_CMD,
    BT_NOECHO,
    BT_NOANSWER
} communication_mode_t;



typedef enum {
    BT_TEST,                // AT
    BT_CONNECT,             // ATA
    BT_DISCONNECT,          // ATH
    BT_CLEAR_ADDRESS,       // ATD0
    BT_SET_ADDRESS,         // ATD=_____
    BT_GET_LOCALID,         // ATB? Inquire the Local BD address
    BT_FIND_DEVICES,        // ATF?
    BT_DISABLE_AUTOCONNECT, // ATO1
    BT_ENABLE_AUTOCONNECT,  // ATO0
    BT_SET_MASTER,          // ATR0
    BT_SET_SLAVE,           // ATR1
    BT_SET_PIN,             // ATP=1234
    BT_SET_2400,            // ATL* Baudrate 2400
    BT_SET_4800,            // ATL0 Baudrate 4800
    BT_SET_9600,            // ATL1 Baudrate 9600
    BT_SET_19200,           // ATL2 Baudrate 19200
    BT_SET_38400,           // ATL3 Baudrate 38400
    BT_SET_57600,           // ATL4 Baudrate 57600
    BT_SET_115200,          // ATL5 Baudrate 115200
    BT_SET_NOANSWER,        // ATQ1 RÃ¼ckmeldungen aus
    BT_SET_NOECHO,          // ATE0 ECHO deaktivieren
    BT_SET_ANSWER,          // ATQ0 RÃ¼ckmeldungen
    BT_SET_ECHO,            // ATE1 ECHO aktivieren
    BT_SET_DEFAULT,         // Defaultwerte setzen
    BT_SET_NAME,            // Devicename
    BT_SET_DISPWRDOWN,      // disable auto Powerdown
    BT_SHOW_SETTINGS        // ATI1 Listing all setting value
} bt_cmd_t;


//TODO: FIFO Groesse
#define IN_FIFO_SIZE 512

char localID[15]="12345678901234";
static uint8_t bt_buffer[IN_FIFO_SIZE];
fifo_t in_fifo;

char bt_rx_buffer[RXD_BUFFER_SIZE];
volatile uint8_t bt_rx_len;
volatile uint8_t bt_rx_ready = 0;


uint8_t EchoAnswerOn=false;                   //Merkzelle
static bt_mode_t bt_mode = BLUETOOTH_SLAVE;
static communication_mode_t comm_mode = BT_CMD;

uint8_t i = 0;

uint8_t bt_devicecount = 0;

uint8_t bt_rxerror = 0;
uint8_t bt_frameerror = 0;
uint8_t bt_overrun = 0;
uint8_t bt_bufferoverflow = 0;


device_info device_list[NUTS_LIST];




// Set a timeout of Y ms and a Conditon X, which have to be true while timeout
#define while_timeout(X, Y) for(uint16_t __timeout = 0; __timeout++ <= Y && (X); Delay_MS(Y ? 1 : 0))


//--------------------------------------------------------------
// entfernt ungueltige Zeichen und Leerzeichen (rechts) aus dem
// BT-Namen die evtl. durch Default's reingekommen sind.
// Falls ungueltige Zeichen ('.') wird der BT-Name auf "PKT"
// gesetzt.
//--------------------------------------------------------------
void bt_fixname(void)
{
    char *p;

    p = Config.bt_name;
    while( *p!=0 && *p!='.' ) p++;                                  // suche '.'
    if( *p=='.' || Config.bt_name[0]==' ' || Config.bt_name[0]==0)  // wenn '.' gefunden oder Anfang mit ' ' oder kein String
        strcpy_P(Config.bt_name, PSTR("PKT"));                      //   -> dann BT-Name auf "PKT" setzen
    strrtrim( Config.bt_name );                                     // ggf. Leerzeichen rechts loeschen
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Delay_MS(int count)
{
    for (int i = 0; i < count; i++)
        _delay_ms(1);
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void bt_start(void)
{
    if( Config.BTIsSlave == true )  EchoAnswerOn = false;
    else                            EchoAnswerOn = true;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void uart_receive(void)
{
    unsigned int uart_data;

    while( !fifo_is_full(&in_fifo) )
    {
        uart_data = uart1_getc();

        //USART_puts(".");
        if (!(uart_data==UART_NO_DATA)) USART_putc(uart_data);

        switch( uart_data & 0xFF00 )
        {
            // Framing Error detected, i.e no stop bit detected
            case UART_FRAME_ERROR:
                    bt_rxerror++;
                    bt_frameerror++;
                    return;

            // Overrun, a character already presend in the UART UDR register was
            // not read by the interrupt handler before the next character arrived,
            // one or more received characters have been dropped
            //
            case UART_OVERRUN_ERROR:
                    bt_rxerror++;
                    bt_overrun++;
                    return;

            // We are not reading the receive buffer fast enough,
            // one or more received character have been dropped
            //
            case UART_BUFFER_OVERFLOW:
                    bt_rxerror++;
                    bt_bufferoverflow++;
                    return;

            // UART Inputbuffer empty, nothing to do
            case UART_NO_DATA:
                    return;

            default:
                    fifo_write( &in_fifo, uart_data);
        }
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
static void uart_send(const char *data, const uint8_t length)
{
    #ifdef SND_DEBUG
    debug_pgm(PSTR("bt_uart_send"));
    if (comm_mode == BT_CMD) debug_pgm(PSTR("bt_uart_send:BT_CMD")); else debug_pgm(PSTR("bt_uart_send:Wrong comm-mode"));
    if (EchoAnswerOn == true) debug_pgm(PSTR("bt_uart_send:EchoAnswer ON")); else debug_pgm(PSTR("bt_uart_send:EchoAnswer OFF"));
    #endif

    char echo;

//  lcd_printp_at (i++, 1, PSTR("."), 0);
    for (uint8_t i = 0; i < length; i++)
    {

    #ifdef SND_DEBUG
        USART_putc((data[i]));    //test
    #endif
//      debug_pgm(PSTR("bt_init_S"));

        if (uart1_putc(data[i]) == 0)
        {
    #ifdef SND_DEBUG
            warn_pgm(PSTR("UART: Remote not ready"));
    #endif
            return;
        }

        if (comm_mode == BT_RAW)
            _delay_ms(50);

        if (comm_mode == BT_DATA)
            _delay_ms(1);


        if ((comm_mode == BT_CMD) && (EchoAnswerOn == true))
        {
    #ifdef SND_DEBUG
                        warn_pgm(PSTR ("UARTsend: get Echo"));
    #endif
            uint8_t x = 0;
            for (; x < 3; x++)
            {

                while_timeout(fifo_is_empty(&in_fifo), 200)
//              for(uint16_t __timeout = 0; __timeout++ <= 200 && (fifo_is_empty(&in_fifo)); _delay_ms(200 ? 1 : 0))

                uart_receive();


                fifo_read(&in_fifo, &echo);

                if (echo != data[i]) {
                    if (uart1_putc(data[i]) == 0)
                    {
    #ifdef SND_DEBUG
                            warn_pgm(PSTR ("UART: Remote not ready"));
    #endif
                        return;
                    }
                }
                else
                    break;
            }
    #ifdef DEBUG
            if (x == 3)
            {

                error_putc(data[i]);

                error_pgm(PSTR("BT: WRONG ECHO"));
                                //_delay_ms(2000);

            }
    #endif
        }
        else
          {
                    {
                            uart_receive();
                            fifo_read(&in_fifo, &echo);

                    }

    #ifdef SND_DEBUG
                    warn_pgm(PSTR ("UARTsend: skip Echo"));
    #endif
          }
    }
}


//--------------------------------------------------------------
// send_cmd( command, *data)
//--------------------------------------------------------------
static uint16_t send_cmd( const bt_cmd_t command, const char *data)
{
    uint16_t CommandDelay;      // nach BTM222 Kommandos verschiedene Verzögerungszeiten bevor es weitergehen kann
    uint8_t i;

    char full_command[20];      // Maximum command size

    CommandDelay = 100;         // Default = 100ms

    switch( command )
    {
        case BT_SET_PIN:        strcpy_P( full_command, PSTR("ATP="));
                                for( i = 0; i < bt_pin_length; i++)
                                {
                                    full_command[i+4] = Config.bt_pin[i];
                                }
                                full_command[(bt_pin_length+4)] = 0;
                                break;

        case BT_SET_DEFAULT:    strcpy_P( full_command, PSTR("ATZ0"));
                                CommandDelay = 3000;
                                break;


        case BT_SET_2400:       strcpy_P( full_command, PSTR("ATL*"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATL*"));
                                #endif
                                break;

        case BT_SET_4800:       strcpy_P( full_command, PSTR("ATL0"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATL0"));
                                #endif
                                break;

        case BT_SET_9600:       strcpy_P( full_command, PSTR("ATL1"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATL1"));
                                #endif
                                break;

        case BT_SET_19200:      strcpy_P( full_command, PSTR("ATL2"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATL2"));
                                #endif
                                break;

        case BT_SET_38400:      strcpy_P( full_command, PSTR("ATL3"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATL3"));
                                #endif
                                break;

        case BT_SET_57600:      strcpy_P( full_command, PSTR("ATL4"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATL4"));
                                #endif
                                break;

        case BT_SET_115200:     strcpy_P( full_command, PSTR("ATL5"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATL5"));
                                #endif
                                break;

        case BT_SET_NOANSWER:   strcpy_P( full_command, PSTR("ATQ1"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATQ1"));
                                #endif
                                break;

        case BT_SET_NOECHO:     strcpy_P( full_command, PSTR("ATE0"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATE0"));
                                #endif
                                break;

        case BT_SET_ANSWER:     strcpy_P( full_command, PSTR("ATQ0"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATQ0"));
                                #endif
                                break;

        case BT_SET_ECHO:       strcpy_P( full_command, PSTR("ATE1"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATE1"));
                                #endif
                                break;

        case BT_TEST:           strcpy_P( full_command, PSTR("AT"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("AT"));
                                #endif
                                break;

        case BT_CONNECT:        strcpy_P( full_command, PSTR("ATA"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATA"));
                                #endif
                                break;

        case BT_DISCONNECT:     strcpy_P( full_command, PSTR("ATH"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATH"));
                                #endif
                                break;

        case BT_CLEAR_ADDRESS:  strcpy_P( full_command, PSTR("ATD0"));
                                break;

        case BT_SET_ADDRESS:    strcpy_P( full_command, PSTR("ATD="));
                                memcpy( (full_command + strlen(full_command)), data, 12);
                                full_command[16] = 0;
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATLD="));
                                #endif
                                break;

        case BT_FIND_DEVICES:   strcpy_P( full_command, PSTR("ATF?"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATF?"));
                                #endif
                                break;

        case BT_DISABLE_AUTOCONNECT:
                                strcpy_P( full_command, PSTR("ATO1"));
                                CommandDelay = 3500;
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATO1"));
                                #endif
                                break;

        case BT_ENABLE_AUTOCONNECT:
                                strcpy_P( full_command, PSTR("ATO0"));
                                CommandDelay = 3500;
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATO0"));
                                #endif
                                break;

        case BT_SET_MASTER:     strcpy_P( full_command, PSTR("ATR0"));
                                CommandDelay = 3000;
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATR0"));
                                #endif
                                break;

        case BT_SET_SLAVE:      strcpy_P( full_command, PSTR("ATR1"));
                                CommandDelay = 3000;
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATR1"));
                                #endif
                                break;

        case BT_SET_NAME:       strcpy_P( full_command, PSTR("ATN=") );
                                strcat( full_command, Config.bt_name);
                                //lcd_print_at( 0, 7, full_command, 0);     // DEBUG
                                //lcdx_printf_at( 17, 7, MNORMAL, 0,0, "%3d", strlen(full_command) );
                                //_delay_ms(6000);
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATN="));
                                #endif
                                break;

        case BT_SET_DISPWRDOWN: strcpy_P(full_command, PSTR("ATS1"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATS1"));
                                #endif
                                break;

        case BT_GET_LOCALID:    strcpy_P(full_command, PSTR("ATB?"));
                                #ifdef DEBUG
                                  debug_pgm(PSTR("ATB?"));
                                #endif
                                break;
        case BT_SHOW_SETTINGS:  strcpy_P(full_command, PSTR("ATI1"));
                                break;

        default:                warn_pgm(PSTR("CMD UNK"));
                                CommandDelay = 0;
                                return false;
    }

    strcat_P( full_command, PSTR("\r") );

    // throw away your television
    uart_receive();
    fifo_clear( &in_fifo );

    // send command
    uart_send( full_command, strlen(full_command));

    // get response
    #ifdef DEBUG
    debug_pgm(PSTR("send_cmd:get Response"));
    #endif


    if( !EchoAnswerOn )
    {
        _delay_ms(CommandDelay);
        uart1_flush();
        fifo_clear(&in_fifo);
        return true;
    }
    else
    {
        while_timeout(true, BT_CMD_TIMEOUT_MS)
        {
            uart_receive();

            if( command == BT_GET_LOCALID )
            {
                _delay_ms(CommandDelay);
                return bt_getID();
            }

            if( fifo_strstr_pgm( &in_fifo, PSTR("OK\r\n")) )
            {
                _delay_ms(CommandDelay);
                return true;
            }

            if( fifo_strstr_pgm(&in_fifo, PSTR("ERROR\r\n")) )
            {
                return false;
            }
        }  // end: while_timeout()
    }

    return false;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
static void clean_line(void)
{
    while_timeout(true, 50)
        uart_receive();

    fifo_strstr_pgm( &in_fifo, PSTR("\r\n") );
}



//--------------------------------------------------------------
//--------------------------------------------------------------
static communication_mode_t update_comm_mode( uint16_t timeout_ms )
{
    while_timeout(true, timeout_ms)
    {
        uart_receive();

        if( fifo_strstr_pgm(&in_fifo, PSTR("DISCONNECT")) )
        {
            clean_line();
            comm_mode = BT_CMD;
            send_cmd(BT_TEST, NULL);
            return comm_mode;
        }

        if( fifo_strstr_pgm(&in_fifo, PSTR("CONNECT")) )
        {
            _delay_ms(100);                                     // don't delete this, else there will be no success!!!!!!!!!
            comm_mode = BT_DATA;
            return comm_mode;
        }

        if( fifo_strstr_pgm (&in_fifo, PSTR("Time out,Fail to connect!")) )
        {
            clean_line();

            #ifdef DEBUG
            debug_pgm(PSTR("CONNECT FAILED\n"));
            #endif

            send_cmd(BT_TEST, NULL);
            comm_mode = BT_CMD;
            return comm_mode;
        }
    }

    return comm_mode;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void bt_set_EchoAnswer( uint8_t on )
{
    comm_mode = BT_CMD;     // Set comm_mode to CMD
    EchoAnswerOn = false;

    if( on )
    {
        send_cmd( BT_SET_ECHO   , NULL);
        send_cmd( BT_SET_ANSWER , NULL);
        send_cmd( BT_TEST       , NULL);
//        EchoAnswerOn = true;

        /*
        if( !send_cmd( BT_TEST  , NULL) )
        {
            //lcd_printp_at (0, 6, PSTR("Error set Echo"), MNORMAL);
            //_delay_ms(3000);
        }
        */
    }
    else
    {
        send_cmd( BT_SET_NOECHO  , NULL);
        send_cmd( BT_SET_NOANSWER, NULL);
//        EchoAnswerOn = false;
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint16_t BTM222_SetBaud( uint8_t baudrate )
{
    bt_cmd_t btcommand = BT_SET_57600;
    uint8_t  lOk       = true;


    comm_mode = BT_CMD;                 // Set comm_mode to CMD
    uart1_flush();
    fifo_clear( &in_fifo );

    bt_set_EchoAnswer(true);

    if( !send_cmd(BT_TEST, NULL) )      // Test mit PKT_Baudrate
    {
        return false;
    }
    else
    {
        switch( baudrate )
        {
            case Baud_2400:     btcommand = BT_SET_2400;    break;
            case Baud_4800:     btcommand = BT_SET_4800;    break;
            case Baud_9600:     btcommand = BT_SET_9600;    break;
            case Baud_19200:    btcommand = BT_SET_19200;   break;
            case Baud_38400:    btcommand = BT_SET_38400;   break;
            case Baud_57600:    btcommand = BT_SET_57600;   break;
            case Baud_115200:   btcommand = BT_SET_115200;  break;
        }

        if( !(send_cmd( btcommand, NULL)) )     lOk = false;        // Fehler

        SetBaudUart1( baudrate );
        uart1_flush();
        fifo_clear( &in_fifo );
        send_cmd( BT_TEST, NULL);

        if( !(send_cmd(BT_TEST, NULL)) )        lOk = false;        // Fehler

        bt_set_EchoAnswer(false);
    }

    return lOk;
}



//--------------------------------------------------------------
// foundbaud = BTM222_CheckBaud( Baudrate )
//
// suche BTM222 mit entsprechender Baudrate
//
// RUECKGABE:
//  =0: Error (BTM222 not found at given Baud)
//  >0: Baudrate
//
// INFO: Baudrate
//   Baud_9600       1
//   Baud_19200      2
//   Baud_38400      3
//   Baud_57600      4
//   Baud_115200     5
//   Baud_4800       6
//   Baud_2400       7
//--------------------------------------------------------------
uint8_t BTM222_CheckBaud( uint8_t Baudrate )
{
  //lcdx_cls_row( y, mode, yoffs )
    lcdx_cls_row( 2, MNORMAL, 1 );                          // Zeile loeschen
    lcdx_printf_center_P( 2, MNORMAL, 0,1, PSTR("%S %lu Baud"), strGet(STR_SEARCH), Baud_to_uint32(Baudrate) );   // "suche nnn Baud"

    comm_mode = BT_CMD;             // Set comm_mode to CMD
    SetBaudUart1( Baudrate );

    uart1_flush();
    fifo_clear( &in_fifo );

    EchoAnswerOn = false;

    send_cmd( BT_TEST      , NULL);
    send_cmd( BT_SET_ANSWER, NULL);
    send_cmd( BT_SET_ECHO  , NULL);

    EchoAnswerOn = true;


    if( send_cmd(BT_TEST, NULL) )
    {
        return Baudrate;            // BTM222 Baudrate gefunden
    }

    return 0;                       // 0 = nicht gefunden
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void BTM222_Initalize( void )
{
    uint8_t foundbaud  = 0;
    uint8_t nError     = 0;

    // ggf. rechte Leerzeichen oder ungueltige Zeichen
    // von bt_name entfernen
    bt_fixname();


    //----------------------------
    // 1. Frage: initialisieren?
    //----------------------------
    // Text1: "Kopter ausschalten!"
    // Text2: NULL
    // Head : "* ACHTUNG *"
    // Titel: "Wi.232 Init"

    if( !PKT_Ask_P( ASK_CANCEL_OK, PSTR("initialisieren?"), NULL, STR_BTM222INIT, STR_BTM222INIT) )
    {
        return;
    }



    //------------------------------------
    // 2. BTM222 suchen
    //------------------------------------
    PKT_Title_P( STR_BTM222INIT, true, true );                                                          // Titel - mit ShowLipo und clearscreen
    lcdx_printf_center_P( 2, MNORMAL, 0,1, PSTR("%S %lu Baud"), strGet(STR_SEARCH), Baud_to_uint32(Old_Baudrate) );   // "suche nnn Baud"

    set_Modul_On( Bluetooth );
    EchoAnswerOn = false;

    fifo_init( &in_fifo, bt_buffer, IN_FIFO_SIZE);

    if( !foundbaud )    foundbaud = BTM222_CheckBaud( Old_Baudrate );                                   // versuche zuerst mit der 'alten' Baudrate (hohe Trefferwahrscheinlichkeit)
    if( !foundbaud )    foundbaud = BTM222_CheckBaud( Baud_2400    );
    if( !foundbaud )    foundbaud = BTM222_CheckBaud( Baud_9600    );
    if( !foundbaud )    foundbaud = BTM222_CheckBaud( Baud_19200   );
    if( !foundbaud )    foundbaud = BTM222_CheckBaud( Baud_38400   );
    if( !foundbaud )    foundbaud = BTM222_CheckBaud( Baud_57600   );
    if( !foundbaud )    foundbaud = BTM222_CheckBaud( Baud_115200  );

    if( !foundbaud )
    {
      //PKT_Message_P( text, error, timeout, beep, clearscreen)
        PKT_Message_P( PSTR("BTM222 not found!"), true, 2000, true, true);                              // ERROR: max. 20 Sekunden anzeigen
        Config.UseBT = false;                                                                           // Modul in Konfig sperren, verhindert Init beim Start
    }


    //------------------------------------
    // 2. Initialisieren
    //------------------------------------
    if( foundbaud > 0 )
    {
        PKT_Title_P( STR_BTM222INIT, true, true );                                                      // Titel: mit ShowLipo und clearscreen
        lcdx_printf_center_P( 2, MNORMAL, 0,1, STR_BTM222INIT );                                        // Headline

      //PKT_Progress_Init( max, yoffs, width, height);
        PKT_Progress_Init( 7, 0,  0,0);                                                                 // 7 Progress Steps

        comm_mode = BT_CMD;                                                                             // Set comm_mode to CMD

        if( !nError && PKT_Progress_Next() && !send_cmd( BT_CLEAR_ADDRESS, NULL)   )    nError = 1;     // Clear remote address
        if( !nError && PKT_Progress_Next() && !send_cmd( BT_SET_SLAVE, NULL)       )    nError = 2;     // Set BTM to SLAVE
        if( !nError && PKT_Progress_Next() && !send_cmd( BT_SET_PIN, NULL)         )    nError = 3;     // Set BTM PIN
        if( !nError && PKT_Progress_Next() && !send_cmd( BT_SET_NAME, NULL)        )    nError = 4;     // Set BTM Name
        if( !nError && PKT_Progress_Next() && !send_cmd( BT_SET_DISPWRDOWN, NULL)  )    nError = 5;
        if( !nError && PKT_Progress_Next() && !send_cmd( BT_GET_LOCALID, NULL)     )    nError = 6;     // MAC ermitteln
      //if( !nError && PKT_Progress_Next() && !send_cmd( BT_SET_ANSWER, NULL)      )    nError = 7;

        if( !nError && PKT_Progress_Next() && !BTM222_SetBaud( Config.PKT_Baudrate))    nError = 8;     // Set BTM Baudrate
      //if( !nError && PKT_Progress_Next() && !BTM222_SetBaud( Baud_38400)) nError = 8;                 // * TEST/DEBUG * Set BTM Baudrate


        SetBaudUart1( Config.PKT_Baudrate );  // PKT wieder auf Original Baudrate setzen

        if( !nError )
        {
            bt_set_EchoAnswer( false );
            bt_mode = BLUETOOTH_SLAVE;

            WriteBTInitFlag();  // Init merken
            WriteBTSlaveFlag();

            bt_start();
            set_Modul_On( USB );

          //PKT_Message_P( text, error, timeout, beep, clearscreen)
            PKT_Message_P( PSTR("BTM222 Init OK!"), false, 1000, true, true);                           // OK: 1000 = max. 10 Sekunden anzeigen
            return;
        }
        else
        {
            set_Modul_On(USB);

          //PKT_Message( text, error, timeout, beep, clearscreen)
            PKT_Message( buffered_sprintf_P(PSTR("BTM Error: %d"),nError), true, 2000, true, true);     // ERROR: 2000 = max. 20 Sekunden anzeigen
            return;
        }
    }

    SetBaudUart1( Config.PKT_Baudrate );                                                                // die richtige UART Baudrate wiederherstellen !!
    set_Modul_On( USB );
    clear_key_all();

    return;
}



//######################################################################################
//######################################################################################



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t bt_set_mode( const bt_mode_t mode )
{
    #ifdef DEBUG
    debug_pgm(PSTR("bt_setmode: set Mode"));
    #endif

    //if (update_comm_mode(0) == BT_DATA) // 30.1.2012 CB
    //  return false;

    if( mode == bt_mode )
        return true;

    if( mode == BLUETOOTH_MASTER )
    {
        comm_mode = BT_CMD;
        bt_set_EchoAnswer(true);

        if( send_cmd(BT_SET_MASTER, NULL) )
        {
            bt_mode = BLUETOOTH_MASTER;
            send_cmd(BT_DISABLE_AUTOCONNECT, NULL);
            WriteBTMasterFlag();

            #ifdef DEBUG
            debug_pgm(PSTR("bt_setmode: Master is set"));
            #endif
        }
    }

    if( mode == BLUETOOTH_SLAVE )
    {
        comm_mode = BT_CMD;
        bt_set_EchoAnswer(true);

        if( send_cmd(BT_ENABLE_AUTOCONNECT, NULL) )
        {
            bt_mode = BLUETOOTH_SLAVE;
            send_cmd(BT_SET_SLAVE, NULL);
            WriteBTSlaveFlag();
            bt_set_EchoAnswer(false);
            comm_mode = BT_CMD;

            #ifdef DEBUG
            debug_pgm(PSTR("bt_setmode: Slave is set"));
            #endif
        }
    }

    //  if (bt_mode == BLUETOOTH_MASTER)  debug_pgm(PSTR("bt_mode:BLUETOOTH_MASTER "));
    //  if (bt_mode == BLUETOOTH_SLAVE)  debug_pgm(PSTR("bt_mode:BLUETOOTH_SLAVE"));
    //        if (mode == BLUETOOTH_MASTER)  debug_pgm(PSTR("mode:BLUETOOTH_MASTER "));
    //        if (mode == BLUETOOTH_SLAVE)  debug_pgm(PSTR("mode:BLUETOOTH_SLAVE"));

    return (mode == bt_mode);
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint16_t bt_receive( void *data, uint8_t length, uint16_t timeout_ms )
{
    uint8_t rec_length = 0;
    uint8_t i = 0;

    //while_timeout(true, timeout_ms);
    for( uint16_t __timeout = 0; __timeout++ <= true && (timeout_ms); _delay_ms(true ? 1 : 0))
    {
        if( i == length )
            return true;

        uart_receive();

        if( fifo_is_empty(&in_fifo) )
            continue;

        if( update_comm_mode(0) != BT_DATA)
        {
            #ifdef DEBUG
            debug_pgm(PSTR("not connected"));
            #endif

            return false;
        }


        if( timeout_ms == 0 )                            // We have a connection
            timeout_ms += 2000;

        if( fifo_is_empty(&in_fifo) )
            continue;


        if( !rec_length )                                // Find starting point of packet
        {
            fifo_read( &in_fifo, (char *)&rec_length);

            if( rec_length != length )
            {
                rec_length = 0;
            }
            else
            {
                timeout_ms += 2000;
            }
        }
        else
        {
            fifo_read( &in_fifo, (char *)data + i);
            i++;
        }
    }
    return false;
}




#ifndef SaveMem



//--------------------------------------------------------------
//--------------------------------------------------------------
uint16_t bt_send( void *data, const uint8_t length )
{
    if (update_comm_mode(0) == BT_CMD)
        return false;

    uart_send((const char *)&length, 1);
    uart_send((char *)data, length);
    return (update_comm_mode(0) == BT_DATA);
}


#ifdef SQUIRREL



//--------------------------------------------------------------
//--------------------------------------------------------------
uint16_t bt_connect( const char *address )
{
    fifo_init(&in_fifo, bt_buffer, IN_FIFO_SIZE);
    uart_receive();
    fifo_clear(&in_fifo);

    // Maybe we already disconnected???
    if (BT_DATA == update_comm_mode(0))
    {
        #ifdef DEBUG
        debug_pgm(PSTR("We are still connected..."));
        #endif
        return false;
    }

//    send_cmd(BT_TEST, NULL);


    /*
    if (!send_cmd(BT_DISABLE_AUTOCONNECT, address))
        return false;
    */

    send_cmd(BT_TEST, NULL);

    #ifdef DEBUG
    debug_pgm (PSTR ("SET_ADD"));
    #endif

    if (!send_cmd(BT_SET_ADDRESS, address))
        return false;

    send_cmd(BT_TEST, NULL);

    #ifdef DEBUG
    debug_pgm (PSTR ("CONNECT"));
    #endif

    if (!send_cmd(BT_CONNECT, NULL))
        return false;

    #ifdef DEBUG
    debug_pgm (PSTR ("WAIT FOR COMM"));
    #endif

    return (BT_DATA == update_comm_mode(20000));
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint16_t bt_disconnect(void)
{
uint8_t RetVal = false;

    for( uint8_t i = 0; i < 4; i++)             // Switch to online cmd mode
    {
        const char plus = '+';
        uart_send(&plus, 1);
        _delay_ms(1000);
    }

    comm_mode = BT_CMD;
    EchoAnswerOn = true;
    uart1_flush();                              // noch nicht verarbeitete Daten löschen
    fifo_clear(&in_fifo);


    if( !send_cmd(BT_DISCONNECT, NULL) )
        RetVal = false;

    if( BT_CMD == update_comm_mode(2000) )      // neue Position um Disconnect-Antwort zu finden
    {
        fifo_clear(&in_fifo);
        RetVal = true;
    }

    if( !send_cmd(BT_CLEAR_ADDRESS, NULL) )
        RetVal = false;


//    if( BT_CMD == update_comm_mode(2000) )      // 31.7.2015 CB ursprüngliche Position, wurde verschoben
//    {
//        fifo_clear(&in_fifo);
//        return true;
//    }

    #ifdef DEBUG
    debug_pgm(PSTR("Still in DATA??"));
    #endif

    return RetVal;
}



/*
BTM-222 Softwareversion 4.35
Inquiry Results:
           111111111222222222233333333334
01234567890123456789012345678901234567890

1  LE091452          0024-2C-BEB0CA
2  E71 c             0024-7C-3EC9B9

BTM-222 Softwareversion 6.26
Inquiry Results:
1  E71 c  0024-7C-3EC9B9 N.A.
2  LE091452  0024-2C-BEB0CA N.A.

*/


//--------------------------------------------------------------
//--------------------------------------------------------------
void copy_mac(const char *src, char *dst)
{
    uint8_t off = 0;

    for (uint8_t i = 0; i < 40; i++)
    {
        if (src[i] == '-')
            if (src[i+3] == '-')// MAC Adresse suchen
            {
                off = i-4;
                break;
            }
    }

    for (uint8_t i = 0; i < 14; i++)
    {
        if (src[i + off] == '-')
            off++;

        dst[i] = src[i + off];
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void copy_localID(const char *src, char *dst)
{
    uint8_t off = 0;

    //for (uint8_t i = 0; i < 40; i++)
    //{
    //    if (src[i] == '-') if (src[i+3] == '-')// MAC Adresse suchen
    //                        {
    //                          off = i-4;
    //                          break;
    //                        }
    //}

    for (uint8_t i = 0; i < 14; i++)
    {
        if (src[i + off] == '-')
            off++;

        dst[i] = src[i + off];
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void copy_DevName(const char *src, char *dst)
{
    uint8_t off = 0;
    uint8_t i = 0;

    for (i = 0; i < 18; i++)
    {
        if (src[i] == ' ' && src[i+1] == ' ')
        {
            dst[i] = 0;  //Stringende
            break; // nach zwei Leerzeichen ist der Name zuende
        }
        dst[i] = src[i + off];
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t bt_discover( void )
{
    char buffer[255];           //oversized, but who cares?
    char *bufferhead  = buffer;
    uint16_t devcount = 0;
    uint16_t Timeout  = 10000;
    uint8_t progress  = 0;
    uint8_t  keyesc   = false;


    #define BT_SEARCHTIME   7500        // = 75 Sekunden BT-Geraete suchen (60 werden vom BTM222 min. dafuer veranschlagt + 15 Sekunden Sicherheit)
    #define PROGRESSUPDATE   100        // = 1 Sekunde - jede Sekunde die Progressbar updaten bei BT-Inquiry


    //PKT_Progress_Init( max, yoffs,  width,height)
    PKT_Progress_Init( 4 + ((BT_SEARCHTIME/PROGRESSUPDATE)+1), 0,  0,0);    // Anzahl der gesamten Progress Steps


    PKT_Progress_Next();                                        // Progressbar: Extra 1

    //------------------------
    //------------------------
    if( !bt_set_mode( BLUETOOTH_MASTER) )
        return 0;

    PKT_Progress_Next();                                        // Progressbar: Extra 2

    send_cmd( BT_TEST, NULL);

    fifo_clear( &in_fifo );

    if( !send_cmd( BT_FIND_DEVICES, NULL) )
        return 0;

    PKT_Progress_Next();                                        // Progressbar: Extra 3

    //------------------------
    //------------------------
    do
    {
        uart_receive();
        Timeout--;

        _delay_ms(1);

        //write_ndigit_number_u( 0,7,fifo_getcount( &in_fifo)  ,5,0,0);

        if( fifo_is_full(&in_fifo) )
            break;

        if( get_key_short(1 << KEY_ESC) )       // 3. Taste
        {
            keyesc = true;
            break;
        }

    } while( (Timeout != 0) && (!fifo_strstr_pgm(&in_fifo, PSTR("Inquiry Results:\r\n"))) );


    fifo_clear( &in_fifo );


    //------------------------
    //------------------------
    if( Timeout!=0 && !keyesc )
    {
        // 25.06.2014 OG: umgestellt auf zeitliches Timeout anstatt einer Zaehlschleife

        timer3 = BT_SEARCHTIME;                                 // 75 Sekunden BT Device suchen

        while( timer3 && (!fifo_search( &in_fifo, PSTR("Inquiry End"))))
        {
            if( (timer3 / PROGRESSUPDATE) != progress )         // Progressbar: next step
            {
                progress = (timer3 / PROGRESSUPDATE);
                PKT_Progress_Next();
            }

            uart_receive();
            _delay_ms(2);                                       // 25.06.2014 OG: diese Verzoegerung muss aktuell sein - sonst werden scheinbar keine Geraete gefunden :-(

            if( get_key_short(1 << KEY_ESC) )                   // 3. Taste
            {
                //keyesc = true;
                break;
            }
        }
    }


    PKT_Progress_Next();                                        // Progressbar: Extra 4


    //set_beep( 300, 0xffff, BeepNormal );  _delay_ms(3000);    // Test um das Ende der Progressbar zu ueberpruefen


    //------------------------
    //------------------------
    while( !fifo_is_empty( &in_fifo) )
    {
        while( !fifo_cmp_pgm( &in_fifo, PSTR("\r\n")) )         // Get next line
        {
            fifo_read( &in_fifo, bufferhead);
            bufferhead++;
        }

        *bufferhead = 0;                                        // terminate string

        bufferhead = buffer;                                    // reset bufferhead

//        write_ndigit_number_u( 0,6,devcount ,5,0,0);
//        set_beep( 300, 0xffff, BeepNormal );
//        _delay_ms(1000);


        if( strlen(buffer) == 0 )                               // the empty line before end of inquiry
            continue;

        if( strstr_P( buffer, PSTR("Inquiry End")))
        {
            break;
        }

        copy_DevName( &buffer[3], device_list[devcount].DevName);
        copy_mac(     &buffer[3], device_list[devcount].mac);

        devcount++;


        if( devcount >= NUTS_LIST )
        {
            break;
        }

    } // end: while( !fifo_is_empty(&in_fifo) )

    clear_key_all();

    return devcount;
}


//--------------------------------------------------------------
uint8_t bt_showsettings( void )
{
    char buffer[512];           //oversized, but who cares?
    char *bufferhead  = buffer;
    uint16_t Timeout  = 100;
    bt_rxerror = 0;

    fifo_init( &in_fifo, bt_buffer, IN_FIFO_SIZE);
    uart1_flush();

    PKT_Progress_Init( 4 , 0,  0,0);    // Anzahl der gesamten Progress Steps
    PKT_Progress_Next();                                        // Progressbar: Extra 1

    if( !bt_set_mode( BLUETOOTH_MASTER) )
        return 0;

//    if (EchoAnswerOn) ScrollBox_Push_P( MNORMAL, PSTR("1 E A on") ); else ScrollBox_Push_P( MNORMAL, PSTR("1 E A off") );

    PKT_Progress_Next();                                        // Progressbar: Extra 2

    EchoAnswerOn = true;
    if( !send_cmd( BT_SHOW_SETTINGS, NULL) )
      ScrollBox_Push_P( MNORMAL, PSTR("Command Error") );

    PKT_Progress_Next();                                        // Progressbar: Extra 3

    do
    {
        uart_receive();
//        write_ndigit_number_u( 0,6,fifo_getcount( &in_fifo)  ,5,0,0);
        Timeout--;
        _delay_ms(2);
        if( fifo_is_full(&in_fifo) )
          {
            ScrollBox_Push_P( MNORMAL, PSTR("Fifo is full") );
            break;
          }

    } while(Timeout != 0 );
// } while( (Timeout != 0) && (!fifo_search(&in_fifo, PSTR("+++"))) );

    PKT_Progress_Next();                                        // Progressbar: Extra 4

    while( !fifo_is_empty( &in_fifo) )
    {
        while( !fifo_cmp_pgm( &in_fifo, PSTR("\r\n")) )         // Get next line
        {
            fifo_read( &in_fifo, bufferhead);
            bufferhead++;
        }
        *bufferhead = 0;                                        // terminate string
        bufferhead = buffer;                                    // reset bufferhead
        if( strlen(buffer) == 0 )                               // the empty line before end of inquiry
            continue;
        ScrollBox_Push( MNORMAL, buffer );

    } // end: while( !fifo_is_empty(&in_fifo) )
    clear_key_all();
//    if (EchoAnswerOn) ScrollBox_Push_P( MNORMAL, PSTR("2 E A on") ); else ScrollBox_Push_P( MNORMAL, PSTR("2 E A off") );

    return true;
}





//--------------------------------------------------------------
//--------------------------------------------------------------
/*
uint16_t bt_discover_OLD( char result[8][12] )
{
    char buffer[255];   //oversized, but who cares?
    char *bufferhead = buffer;
    uint16_t pos = 0;
    uint16_t Timeout = 10000;
    uint16_t posC = 0;
    uint8_t Result=false;

    PKT_Progress_Init( 25, 0,  0,0);                                    // 25 Progress Steps

    if (!bt_set_mode(BLUETOOTH_MASTER)) Result = false;
    PKT_Progress_Next();                                                // Progressbar

    send_cmd(BT_TEST, NULL);
    if( !send_cmd(BT_FIND_DEVICES, NULL)) Result = false;
    PKT_Progress_Next();                                                // Progressbar

    do
    {
        uart_receive();
        Timeout--;

        posC++;
        _delay_ms(1);
        write_ndigit_number_u(0,5,fifo_getcount(&in_fifo),5,0,0);
        if (posC ==1000)
        {
            PKT_Progress_Next();                                        // Progressbar
            posC = 0;
        }

        if (fifo_is_full(&in_fifo)) break;

    } while( (Timeout != 0) && (!fifo_strstr_pgm(&in_fifo, PSTR("Inquiry Results:\r\n"))) );


    if (Timeout==0)
    {
        Result = false;
    }
    else
    {
        for (uint16_t i = 0; i < 40000; i++)
        {
            if( (i % 2000) == 0 )
                PKT_Progress_Next();                                    // Progressbar
            uart_receive();
            write_ndigit_number_u( 0,5,fifo_getcount(&in_fifo),5,0,0);
            _delay_ms(1);
        }
    }


    while( !fifo_is_empty(&in_fifo) )
    {
        // Get next line
        while (!fifo_cmp_pgm(&in_fifo, PSTR("\r\n")))
        {
            fifo_read(&in_fifo, bufferhead);
            bufferhead++;
        }
        // terminate string
        *bufferhead = 0;

        //reset bufferhead
        bufferhead = buffer;

        if (strlen(buffer) == 0)
            continue;   //the empty line before end of inquiry

        if( strstr_P(buffer, PSTR("Inquiry End")))
        //if (searchend)
        {
            fifo_clear(&in_fifo);
            return true;
        }

        copy_DevName(&buffer[3],device_list[pos].DevName);
        copy_mac(&buffer[3], device_list[pos].mac);
        pos++;

    } // end: while( !fifo_is_empty(&in_fifo) )

    return Result;
}
*/






//--------------------------------------------------------------
// ermittelt die MAC vom BTM2222
//--------------------------------------------------------------
uint8_t bt_getID( void )
{
    //char buffer[255];         //oversized, but who cares?
    char buffer[80];        //oversized, but who cares?
    char *bufferhead = buffer;
    uint8_t i;


    while_timeout( !fifo_strstr_pgm(&in_fifo, PSTR("r\n")), 2000)
        uart_receive();


    while( !fifo_is_empty(&in_fifo) )
    {
        while( !fifo_cmp_pgm( &in_fifo, PSTR("\r\n")) )     // Get next line
        {
              fifo_read( &in_fifo, bufferhead);
              bufferhead++;
        }

        *bufferhead = 0;                                    // terminate string

        bufferhead = buffer;                                // reset bufferhead

        if( strlen(buffer) == 0 )
            continue;                                       // the empty line before end of inquiry

        copy_localID( &buffer[0], &localID[0] );

        for( i = 0; i < 13; i++)
        {
            //lcd_putc (i, 6, localID[i],0);
            Config.bt_Mac[i] = localID[i];
        }


        if( fifo_strstr_pgm(&in_fifo, PSTR("OK")) )
        {
            fifo_clear( &in_fifo );
            return true;
        }
        else
        {
            return false;
        }

    } // end: if( !fifo_is_empty(&in_fifo) )

    return false;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void bt_downlink_init(void)
{
    fifo_init( &in_fifo, bt_buffer, IN_FIFO_SIZE);
    _delay_ms(100);
    //      debug_pgm(PSTR("bt_downlink_init"));
    uart_receive();
    fifo_clear(&in_fifo);
    //        send_cmd(BT_TEST, NULL);

    #ifdef DEBUG
    debug_pgm(PSTR("downlink_init Start"));
    #endif

    if( Config.BTIsSlave == true )          // nur Init wenn BT ist Slave
    {
        #ifdef DEBUG
        debug_pgm(PSTR("downlink_init:try to set Master"));
        #endif

        //comm_mode = BT_NOECHO;
        //
        //if (!send_cmd (BT_SET_ECHO,NULL))
        //{
        //  #ifdef DEBUG
        //  debug_pgm(PSTR("downlink_init:Couldn't set Echo!"));
        //  #endif
        //}
        //
        //comm_mode = BT_NOANSWER;
        //if (!send_cmd(BT_SET_ANSWER,NULL))
        //{
        //  #ifdef DEBUG
        //  debug_pgm(PSTR("downlink_init:Couldn't set Answer!"));
        //  #endif
        //
        //}

        comm_mode = BT_CMD;
        //send_cmd(BT_TEST, NULL);

        bt_set_EchoAnswer(true);

        if( !bt_set_mode(BLUETOOTH_MASTER) )
        {
            #ifdef DEBUG
            debug_pgm(PSTR("downlink_init:Couldn't set master!"));
            #endif

            return;
        }

        #ifdef DEBUG
        debug_pgm(PSTR("downlink_init:master is set "));
        #endif

        //WriteBTMasterFlag();  // Master merken
        comm_mode = BT_CMD;
    }
    else
    {
        #ifdef DEBUG
        debug_pgm(PSTR("downlink_init:Master was set"));
        #endif

        comm_mode = BT_CMD;
    }
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void bt_searchDevice( void )                              //Bluetoothgeräte suchen
{
    bt_devicecount = bt_discover();
}




//
////--------------------------------------------------------------
//uint16_t bt_receiveNMEA2(void)
//{
//
//        char received;
//        static uint8_t line_flag = 1;
//        static char* ptr_write = bt_rx_buffer;
//        uint16_t timeout_ms=2000;
//
////        while_timeout(true, timeout_ms) {
//while(1){
//
//        uart_receive();
//           if (fifo_is_empty(&in_fifo))
//               continue;
//        fifo_read(&in_fifo, &received);
//        USART_putc(received);
////        _delay_ms(1);
//    }
////
//return true;
//}



#endif
#endif
//#endif


#endif  // end: #ifdef USE_BLUETOOTH

