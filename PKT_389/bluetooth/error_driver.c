/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

#include <avr/pgmspace.h>
#include <stdbool.h>
//#include "cpu.h"
#include "error_driver.h"
#include "../main.h"

#ifdef DEBUG

#include "../uart/usart.h"
#include "../uart/uart1.h"


void errordriver_write_c(char c)


	{
		USART_putc(c);
	}

     void error_driver_Init(void)
	{
//		USART_Init(UART_BAUD_SELECT(USART_BAUD,F_CPU));
	}

#endif
