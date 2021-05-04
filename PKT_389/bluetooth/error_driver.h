/*
* Functions to write error message to FTDI or USART
*/

#ifndef __ERROR_DRIVER__
#define __ERROR_DRIVER__

#include <avr/io.h>
#include "../main.h"



#ifdef DEBUG
extern void errordriver_write_c(char c);
extern void error_driver_Init(void);
#else
#define errordriver_write_c(c) {}
#define error_driver_init() {}
#endif


#endif //__ERROR_DRIVER__
