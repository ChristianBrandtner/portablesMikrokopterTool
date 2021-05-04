/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */


/********************************************************************/
/*																																	*/
/*									 NG-Video 5,8GHz																*/
/*																																	*/
/*								Copyright (C) 2011 - gebad												*/
/*																																	*/
/*	This code is distributed under the GNU Public License						*/
/*	which can be found at http://www.gnu.org/licenses/gpl.txt				*/
/*																																	*/
/*	using																														*/
/*! \file servo.c \brief Interrupt-driven RC Servo function library.*/
/*																																	*/
/*File Name		: 'servo.c'																						*/
/*Title				: Interrupt-driven RC Servo function library					*/
/*Author				: Pascal Stang - Copyright (C) 2002									*/
/*Created				: 7/31/2002																					*/
/*Revised				: 8/02/2002																					*/
/*Version				: 1.0																								*/
/*Target MCU		: Atmel AVR Series																	*/
/*Editor Tabs		: 2																									*/
/*																																	*/
/*This code is distributed under the GNU Public License							*/
/*				which can be found at http://www.gnu.org/licenses/gpl.txt	*/
/*																																	*/
/********************************************************************/

//############################################################################
//# HISTORY  ng_servo.c
//#
//# 25.08.2013 Cebra
//# - add: #ifdef USE_TRACKING
//#
//# 24.04.2013 OG
//# - chg: umbenannt servo_test(void) nach _servo_test(void)
//#        das verursachte in meinem PKT-Projekt Errors weil die Funktion 
//#        servo_test() ebenfalls ein servo.c vorhanden ist
//############################################################################


#include <avr/interrupt.h>
#include <avr/io.h>

#include "../tracking/ng_servo.h"
#include "../tracking/tracking.h"
#include "../HAL_HW3_9.h"
#include "../eeprom/eeprom.h"
//#include "config.h"
#ifdef USE_TRACKING
// Global variables
uint16_t ServoPosTics;
uint16_t ServoPeriodTics;

// servo channel registers
ServoChannelType ServoChannels[SERVO_NUM_CHANNELS];

const ServoConst_t ServoConst[2] = {{SERVO_MAX, SERVO_MIN, SERVO_STEPS, SERVO_PRESCALER},
                                    {SERVO_MAX * 4, SERVO_MIN * 4, (SERVO_STEPS + 1) * 4 - 1, SERVO_PRESCALER / 4}};

// Servo limits (depend on hardware)
const servo_limit_t servo_limit[2][3] = {{{SERVO_I0_RIGHT_MIN, SERVO_I0_RIGHT_MAX},
					 {SERVO_I0_LEFT_MIN, SERVO_I0_LEFT_MAX},
					 {SERVO_I0_MIDDLE_MIN, SERVO_I0_MIDDLE_MAX}},
					 {{SERVO_I1_RIGHT_MIN, SERVO_I1_RIGHT_MAX},
					 {SERVO_I1_LEFT_MIN, SERVO_I1_LEFT_MAX},
					 {SERVO_I1_MIDDLE_MIN, SERVO_I1_MIDDLE_MAX}}};

// Servopositionen normiert für 950µs, 2,05ms und 1,5ms ==> Ergebnis Schritte. Da Zeit in µs ist F_CPU*e-1
const steps_pw_t steps_pw[2] = {{(uint64_t)950*F_CPU*1e-6/SERVO_PRESCALER + 0.5, (uint64_t)2050*F_CPU*1e-6/SERVO_PRESCALER + 0.5,(uint64_t)1500*F_CPU*1e-6/SERVO_PRESCALER + 0.5},
                                {(uint64_t)950*4*F_CPU*1e-6/SERVO_PRESCALER + 0.5, (uint64_t)2050*4*F_CPU*1e-6/SERVO_PRESCALER + 0.5, (uint64_t)1500*4*F_CPU*1e-6/SERVO_PRESCALER + 0.5}};

// anzufahrende Servopositionen 0=MIN, 1=MID, 2=MAX
const uint8_t PosIdx[POSIDX_MAX] = {1, 0, 1, 2 };

// functions
void _servo_test(void)
{
  //Dummy
}
//! initializes software PWM system 16-bit Timer
// normaler Weise wird ein Serv-PWM Signal aller 20ms wiederholt
// Werte: rev, min, max, mid vorher über servoSet...() initialisieren und einmal servoSetPosition(...) ausführen!!!
void servoInit(uint8_t servo_period)
{ uint16_t OCValue; // set initial interrupt time




	// disble Timer/Counter1, Output Compare A Match Interrupt
	TIMSK1 &= ~(1<<OCIE1A); 
	// set the prescaler for timer1
	if (Config.sIdxSteps == STEPS_255) {
		TCCR1B &= ~((1<<CS11) | (1<<CS10));
		TCCR1B |= (1<<CS12); 							// prescaler 256, Servo-Schritte 185 bei 180 grd Winkel
	}
	else {
		TCCR1B &= ~(1<<CS12);
		TCCR1B |= (1<<CS11) | (1<<CS10);		// prescaler 64,	Servo-Schritte 740 bei 180 grd Winkel
	}
	// attach the software PWM service routine to timer1 output compare A
	// timerAttach(TIMER1OUTCOMPAREA_INT, servoService);
	// enable channels
	for(uint8_t channel=0; channel < SERVO_NUM_CHANNELS; channel++) {
		// set default pins assignments SERVO2 Pin 4; SERVO1 Pin 5
		ServoChannels[channel].pin = (1 << (SERVO2 + channel));
	}
	ServoPosTics = 0; // set PosTics
	// set PeriodTics
	ServoPeriodTics = F_CPU / ServoConst[Config.sIdxSteps].prescaler * servo_period * 1e-3;
	// read in current value of output compare register OCR1A
	OCValue =	OCR1AL;			// read low byte of OCR1A
	OCValue += (OCR1AH << 8);		// read high byte of OCR1A
	OCValue += ServoPeriodTics;		// increment OCR1A value by nextTics
	// set future output compare time to this new value
	OCR1AH = OCValue >> 8;			// write high byte
	OCR1AL = OCValue & 0x00FF;		// write low byte
	TIMSK1 |= (1<<OCIE1A);				// enable the timer1 output compare A interrupt
	coldstart = 1;
}

void servoSetDefaultPos(void)
{
        servoSetPosition(SERVO_PAN, ServoSteps());            // Ausgangsstellung SERVO_PAN
        servoSetPosition(SERVO_TILT,ServoSteps());                        // Ausgangsstellung SERVO_TILT
}

uint16_t pw_us(uint16_t Steps)
{
	return(Steps * ServoConst[Config.sIdxSteps].prescaler/(F_CPU * 1e-6)	+ 0.5);	// Zeit pro Schritt (Wert * e-1) in µs
}

uint16_t ServoSteps(void)
{
	return(ServoConst[Config.sIdxSteps].steps);
}

void servoSet_rev(uint8_t channel, uint8_t val)
{
	ServoChannels[channel].rev = val & 0x01;
}

void servoSet_min(uint8_t channel, uint16_t min)
{
	ServoChannels[channel].min = ServoConst[Config.sIdxSteps].min + min;
}

void servoSet_max(uint8_t channel, uint16_t max)
{
	ServoChannels[channel].max = ServoConst[Config.sIdxSteps].max - max;
}

void servoSet_mid(uint8_t channel, uint16_t mid)
{ 
	ServoChannels[channel].mid = mid;
	// Faktor 16, bzw. 16/2 um mit einer Nachkommastelle zu Rechnen 
	ServoChannels[channel].mid_scaled = (8 * (ServoChannels[channel].max - ServoChannels[channel].min) + \
	                                    (16 * mid - 8 * ServoConst[Config.sIdxSteps].steps))/16 + ServoChannels[channel].min;
}

//! turns off software PWM system
void servoOff(void)
{
	// disable the timer1 output compare A interrupt
	TIMSK1 &= ~(1<<OCIE1A);
}

//! set servo position on channel (raw unscaled format)
void servoSetPositionRaw(uint8_t channel, uint16_t position)
{
	// bind to limits
	if (position < ServoChannels[channel].min) position = ServoChannels[channel].min;
	if (position > ServoChannels[channel].max) position = ServoChannels[channel].max;
	// set position
	ServoChannels[channel].duty = position;
}

//! set servo position on channel
// input should be between 0 and ServoSteps() (entspricht Maximalausschlag)
void servoSetPosition(uint8_t channel, uint16_t position)
{ uint16_t pos_scaled;
 
	// calculate scaled position
	if (ServoChannels[channel].rev != 0) position = ServoConst[Config.sIdxSteps].steps - position;
	if (position < ServoConst[Config.sIdxSteps].steps/2)
		//bei Position < Servomittelstellung	Position*(Mitte - Min)/(Servoschritte/2)
		pos_scaled = ServoChannels[channel].min + ((int32_t)position*2*(ServoChannels[channel].mid_scaled-ServoChannels[channel].min))/ \
								 ServoConst[Config.sIdxSteps].steps;
	else
		//bei Position >= Servomittelstellung
		pos_scaled = ServoChannels[channel].mid_scaled + (uint32_t)(position - ServoConst[Config.sIdxSteps].steps / 2) \
								 * 2 * (ServoChannels[channel].max-ServoChannels[channel].mid_scaled)/ServoConst[Config.sIdxSteps].steps;
	// set position
	servoSetPositionRaw(channel, pos_scaled);
}

// Umrechnung Winkel in Servoschritte
void servoSetAngle(uint8_t servo_nr, int16_t angle)
{
	servoSetPosition(servo_nr, (uint16_t)((uint32_t)angle * ServoConst[Config.sIdxSteps].steps / 180));
}

//Interruptroutine
ISR(TIMER1_COMPA_vect)
{ static uint8_t ServoChannel;
	uint16_t nextTics;
	uint16_t OCValue; // schedule next interrupt

	if(ServoChannel < SERVO_NUM_CHANNELS) {
		PORTD &= ~ServoChannels[ServoChannel].pin;		// turn off current channel
	}
	ServoChannel++;	// next channel
	if(ServoChannel != SERVO_NUM_CHANNELS) {
		// loop to channel 0 if needed
		if(ServoChannel > SERVO_NUM_CHANNELS)		ServoChannel = 0;
		// turn on new channel
		PORTD |= ServoChannels[ServoChannel].pin;
		// schedule turn off time
		nextTics = ServoChannels[ServoChannel].duty;
	}
	else {
		// ***we could save time by precalculating this
		// schedule end-of-period
		nextTics = ServoPeriodTics-ServoPosTics;
	}
	// read in current value of output compare register OCR1A
	OCValue =	OCR1AL;			// read low	byte of OCR1A
	OCValue += (OCR1AH <<8);		// read high byte of OCR1A
	OCValue += nextTics;			// increment OCR1A value by nextTics
	// set future output compare time to this new value
	OCR1AH = OCValue >> 8;			// write high byte
	OCR1AL = OCValue & 0x00FF;	        // write low	byte

	ServoPosTics += nextTics;		// set our new tic position
	if(ServoPosTics >= ServoPeriodTics) ServoPosTics = 0;
}
#endif
