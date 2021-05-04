/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*
      ___                         ___           ___           ___          _____
     /  /\                       /__/\         /  /\         /__/\        /  /::\
    /  /::\                     |  |::\       /  /::\        \  \:\      /  /:/\:\
   /  /:/\:\    ___     ___     |  |:|:\     /  /:/\:\        \  \:\    /  /:/  \:\
  /  /:/~/::\  /__/\   /  /\  __|__|:|\:\   /  /:/  \:\   _____\__\:\  /__/:/ \__\:|
 /__/:/ /:/\:\ \  \:\ /  /:/ /__/::::| \:\ /__/:/ \__\:\ /__/::::::::\ \  \:\ /  /:/
 \  \:\/:/__\/  \  \:\  /:/  \  \:\~~\__\/ \  \:\ /  /:/ \  \:\~~\~~\/  \  \:\  /:/
  \  \::/        \  \:\/:/    \  \:\        \  \:\  /:/   \  \:\  ~~~    \  \:\/:/
   \  \:\         \  \::/      \  \:\        \  \:\/:/     \  \:\         \  \::/
    \  \:\         \__\/        \  \:\        \  \::/       \  \:\         \__\/
     \__\/                       \__\/         \__\/         \__\/
      ___           ___           ___           ___           ___           ___
     /  /\         /  /\         /__/\         /__/\         /  /\         /__/\
    /  /:/        /  /::\       |  |::\       |  |::\       /  /::\        \  \:\
   /  /:/        /  /:/\:\      |  |:|:\      |  |:|:\     /  /:/\:\        \  \:\
  /  /:/  ___   /  /:/  \:\   __|__|:|\:\   __|__|:|\:\   /  /:/  \:\   _____\__\:\
 /__/:/  /  /\ /__/:/ \__\:\ /__/::::| \:\ /__/::::| \:\ /__/:/ \__\:\ /__/::::::::\
 \  \:\ /  /:/ \  \:\ /  /:/ \  \:\~~\__\/ \  \:\~~\__\/ \  \:\ /  /:/ \  \:\~~\~~\/
  \  \:\  /:/   \  \:\  /:/   \  \:\        \  \:\        \  \:\  /:/   \  \:\  ~~~
   \  \:\/:/     \  \:\/:/     \  \:\        \  \:\        \  \:\/:/     \  \:\
    \  \::/       \  \::/       \  \:\        \  \:\        \  \::/       \  \:\
     \__\/         \__\/         \__\/         \__\/         \__\/         \__\/


 **
 * Error handling functions
 */

//############################################################################
//# HISTORY  error.c
//#
//# 16.04.2013 Cebra
//# - chg: PROGMEM angepasst auf neue avr-libc und GCC, prog_char depreciated
//#
//##########################################################################



#include <stdbool.h>
#include <avr/pgmspace.h>
#include "error_driver.h"
#include "../main.h"
#ifdef DEBUG
//--------------------------------------------------------------
inline void _send_msg(const char *msg)
{
	for (uint8_t i=0; i<255 && msg[i]!='\0'; i++)
	{
		errordriver_write_c(msg[i]);
	}
	errordriver_write_c('\n');
}


//--------------------------------------------------------------
void send_pgm(const char *msg)
{
	uint8_t  myByte;
	myByte = pgm_read_byte(msg);
	for(int i = 1; myByte != '\0'; i++)
	{
		errordriver_write_c(myByte);
		myByte = pgm_read_byte(msg+i);
	}
}




//--------------------------------------------------------------
void error_init(void)
{
	error_driver_Init();
}


//--------------------------------------------------------------
void error_putc(const char c)
{
	errordriver_write_c(c);
}


//--------------------------------------------------------------
void assert (bool condition, const char *msg)
{
	if (!condition)
	{
		send_pgm(PSTR("ASS:"));
		_send_msg(msg);
	}
}


//--------------------------------------------------------------
void info (const char *msg)
{
	send_pgm(PSTR("INF:"));
	_send_msg(msg);
}


//--------------------------------------------------------------
void warn (const char *msg)
{
	send_pgm(PSTR("WARN:"));
	_send_msg(msg);
}


//--------------------------------------------------------------
void debug (const char *msg)
{
	send_pgm(PSTR("DBG:"));
	_send_msg(msg);
}


//--------------------------------------------------------------
void Error (const char *msg)
{
	send_pgm(PSTR("ERR:"));
	_send_msg(msg);
}
#endif

#ifdef DEBUG

//--------------------------------------------------------------
void assert_pgm(bool condition, const prog_char *msg)
{
	if (condition) {
		send_pgm(PSTR("ASS:"));
		send_pgm(msg);
		errordriver_write_c('\n');
	}
}


//--------------------------------------------------------------
void info_pgm(const prog_char *msg)
{
	send_pgm(PSTR("INF:"));
	send_pgm(msg);
	errordriver_write_c('\n');
}


//--------------------------------------------------------------
void warn_pgm(const prog_char *msg)
{
	send_pgm(PSTR("WARN:"));
	send_pgm(msg);
	errordriver_write_c('\n');
        errordriver_write_c('\r');
}


//--------------------------------------------------------------
void error_pgm(const prog_char *msg)
{
	send_pgm(PSTR("ERR:"));
	send_pgm(msg);
	errordriver_write_c('\n');
        errordriver_write_c('\r');
}


//--------------------------------------------------------------
void debug_pgm(const prog_char *msg)
{
	send_pgm(PSTR("DBG:"));
	send_pgm(msg);
	errordriver_write_c('\n');
        errordriver_write_c('\r');
}


////--------------------------------------------------------------
//void print_hex(uint8_t num)
//{
//	if (num<10)
//		error_putc(num+48);
//	else
//	{
//		switch (num)
//		{
//		case 10:
//			error_putc('A'); break;
//		case 11:
//			error_putc('B'); break;
//		case 12:
//			error_putc('C'); break;
//		case 13:
//			error_putc('D'); break;
//		case 14:
//			error_putc('E'); break;
//		case 15:
//			error_putc('F'); break;
//		default:
//			error_putc('#'); break;
//		}
//	}
//}
//
//
////--------------------------------------------------------------
//void byte_to_hex(uint8_t byte)
//{
//	uint8_t b2 = (byte & 0x0F);
//	uint8_t b1 = ((byte & 0xF0)>>4);
//	print_hex(b1);
//	print_hex(b2);
//}

#endif

