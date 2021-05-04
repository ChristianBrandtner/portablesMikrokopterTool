/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*********************************************************************/
/*																																	 */
/*									 NG-Video 5,8GHz																 */
/*																																	 */
/*								Copyright (C) 2011 - gebad												 */
/*																																	 */
/*	This code is distributed under the GNU Public License						 */
/*	which can be found at http://www.gnu.org/licenses/gpl.txt				 */
/*																																	 */
/*	using 																													 */
/*! \file servo.c \brief Interrupt-driven RC Servo function library. */
/*																																	 */
/*File Name		: 'servo.c'																						 */
/*Title				: Interrupt-driven RC Servo function library					 */
/*Author			: Pascal Stang - Copyright (C) 2002										 */
/*Created			: 7/31/2002																						 */
/*Revised			: 8/02/2002																						 */
/*Version			: 1.0																									 */
/*Target MCU	: Atmel AVR Series																		 */
/*Editor Tabs	: 4																										 */
/*																																	 */
/*ingroup driver_sw																									 */
/*defgroup servo Interrupt-driven RC Servo Function Library (servo.c)*/
/*code #include "servo.h" \endcode																	 */
/*par Overview																											 */
/*																																	 */
/*This code is distributed under the GNU Public License							 */
/*which can be found at http://www.gnu.org/licenses/gpl.txt					 */
/*																																	 */
/*********************************************************************/

#ifndef SERVO_H
#define SERVO_H


/* Servo */
#define SERVO_PAN                                                               0
#define SERVO_TILT                                                      1
#define SERVO_NUM_CHANNELS                      2       // Anzahl der angeschlossen Servos max. 2!!!
/* Servokalibrierungen derzeit zu SERVO_STEPS = 255 skaliert */
//prescaler 256
#define SERVO_I0_RIGHT                          45     // default Wert, ca. 0,9ms
#define SERVO_I0_RIGHT_MIN                      0       // Servokalibrierung Grenze der linken Position
#define SERVO_I0_RIGHT_MAX                      100     // SERVO_MIN + SERVO_RIGHT

#define SERVO_I0_LEFT                           45     // default Wert, ca. 2,1ms
#define SERVO_I0_LEFT_MIN                       0       // Servokalibrierung Grenze der rechten Position
#define SERVO_I0_LEFT_MAX                       100     // SERVO_MAX - SERVO_LEFT

#define SERVO_I0_MIDDLE                         SERVO_STEPS/2
#define SERVO_I0_MIDDLE_MIN                     SERVO_STEPS/2 - 25
#define SERVO_I0_MIDDLE_MAX                     SERVO_STEPS/2 + 25
//prescaler 64
#define SERVO_I1_RIGHT                          180     // default Wert, ca. 0,9ms
#define SERVO_I1_RIGHT_MIN                      0       // Servokalibrierung Grenze der linken Position
#define SERVO_I1_RIGHT_MAX                      400     // SERVO_MIN + SERVO_RIGHT

#define SERVO_I1_LEFT                           180     // default Wert, ca. 2,1ms
#define SERVO_I1_LEFT_MIN                       0       // Servokalibrierung Grenze der rechten Position
#define SERVO_I1_LEFT_MAX                       400     // SERVO_MAX - SERVO_LEFT

//#define SERVO_I1_MIDDLE                               ((SERVO_STEPS + 1) * 4 - 1)/2
#define SERVO_I1_MIDDLE_MIN                     ((SERVO_STEPS + 1) * 4 - 1)/2 - 100
#define SERVO_I1_MIDDLE_MAX                     ((SERVO_STEPS + 1) * 4 - 1)/2 + 100

#define SERVO_REV                               0       // kein Reverse
/* Test Servo */
#define SERVO_PERIODE                           20     // default Angabe in ms
#define SERVO_PERIODE_MIN                       10     // 10ms
#define SERVO_PERIODE_MAX                       30     // 30ms
#define SINGLE_STEP                             0       // Einzelschritt aus
#define SINGLE_STEP_MIN                         0
#define SINGLE_STEP_MAX                         20     // bei prescaler 256, sonst * 4 (von links nach rechts in 9 Abschnitte)
                                                                                                                                        // zwischen den Schritten muss Pause > der Servoperiode sein, sonst keine Aktualisierung

#define REPEAT                                  1
#define REPEAT_MIN                              1
#define REPEAT_MAX                              100
#define PAUSE                                   10
#define PAUSE_MIN                               4       // mindestens 400ms, da mechanischer Servo-Lauf zur Position berücksichtigt werden muss
#define PAUSE_MAX                               20     // Pause pro Links-, Mittel- und Rechtsposition 10*100ms
#define PAUSE_STEP                              0
#define PAUSE_STEP_MIN                          0       // Pause bei jeden Servoschritt in ms
#define PAUSE_STEP_MAX                          200
// The numbers below good for parallax servos at an F_CPU of 20.000MHz.
// Da einige Servo's auch eien Winkel von 180 grd zulassen, Wertebereich
// entgegen den sonst üblichen. Einschränkung mit default Kalibrierung
// auf 0,9ms (45) bis 2,1ms(45) gesetzt. Je nach Servo, entspricht einen
// Winkel von etwa 180grd
// Periode default 20ms

#define SERVO_MAX				211	// 2,7 ms bei prescaler 256, bei prescaler 64 SERVO_MAX * 4
#define SERVO_MIN				26	// 0,33ms bei prescaler 256, bei prescaler 64 SERVO_MIN * 4
#define SERVO_STEPS			        255	// Servo-Schritte von links nach rechts, Anschlagkalibrierung spielt keine Rolle
#define SERVO_PRESCALER	                        256	// bei prescaler 256, bei prescaler 64 SERVO_PRESCALER / 4
#define STEPS_255				0	// Servo-Schritte von links nach rechts, Anschlagkalibrierung spielt keine Rolle
#define STEPS_1023				1	// Servo-Schritte von links nach rechts, Anschlagkalibrierung spielt keine Rolle

typedef struct //Servo-Konstante je nach Prescaler
{
	uint16_t max;
	uint16_t min;
	uint16_t steps;
	uint16_t prescaler;
}ServoConst_t;

typedef struct //struct_ServoChannel
{
	uint8_t	 pin;					// hardware I/O port and pin for this channel
	uint16_t duty;					// PWM duty setting which corresponds to servo position
	uint8_t	 rev;					// Parameter, wie on/off; reverse; range
	uint16_t min;					// SERVO_MIN + Parameter min
	uint16_t max;					// SERVO_MAX - Parameter max
	uint16_t mid_scaled;		                // skalierte Servomitte
	int16_t	 mid;					// Servomitte = SERVO_STEPS/2 +/- x Schritte; bei Pescaler 256 wird nur uint8_t benötigt aber bei 64
}ServoChannelType;

//uint8_t		 sIdxSteps;			        // 0 für 255 und 1 für 1023 Schritte; Prescaler 256 oder 64

// Define servo limits (depend on hardware)
typedef struct {
	uint16_t min;
	uint16_t max;
}servo_limit_t;

extern const servo_limit_t servo_limit[2][3];

// Define servo positions (depend on hardware)
typedef struct {
	uint16_t min;
	uint16_t max;
	uint16_t mid;
}steps_pw_t;


typedef struct {
        uint8_t rev;
        uint16_t min;
        uint16_t max;
        uint16_t mid;
} servo_t;

// Servopositionen für 950µs, 2,05ms und 1,5ms ==> Ergebnis Schritte. Da Zeit in µs ist F_CPU*e-1
extern const steps_pw_t steps_pw[2];

#define RIGHT					0		// Servopostionen, welche vom Einbau abhängig sind
#define LEFT					1
#define MIDDLE				        2

#define POSIDX_MAX 		                4
extern const uint8_t PosIdx[POSIDX_MAX];		// anzufahrende Servopositionen 0=MIN, 1=MID, 2=MAX

// functions


void servoSetDefaultPos(void);

// initializes servo system
//				You must run this to begin servo control
void servoInit(uint8_t servo_period);

// turns off servo system
//				This stops controlling the servos and
//				returns control of the SERVOPORT to your code
void servoOff(void);

// set servo position on a given channel 
//				servoSetPosition() commands the servo on <channel> to the position you
//				desire.	The position input must lie between 0 and POSITION_MAX and
//				will be automatically scaled to raw positions between SERVO_MIN and
//				SERVO_MAX
void servoSetPosition(uint8_t channel, uint16_t position);

// set raw servo position on a given channel
//				Works like non-raw commands but position is not scaled.	Position must
//				be between SERVO_MIN and SERVO_MAX
void servoSetPositionRaw(uint8_t channel, uint16_t position);

// set servo to a specific angle
void servoSetAngle(uint8_t servo_nr, int16_t angle);

// vor servoInit(), oder vor sei() ServoWerte mit servoSet...() initialisieren, einschließlich servoSetPosition(...)!
void servoSet_rev(uint8_t channel, uint8_t val);
void servoSet_min(uint8_t channel, uint16_t min);
void servoSet_max(uint8_t channel, uint16_t max);
void servoSet_mid(uint8_t channel, uint16_t mid);
uint16_t pw_us(uint16_t Steps);	// gibt Zeit in µs bei x Servoschritte
uint16_t ServoSteps(void);			// gibt "Konstante" derzeitiger Servoschritte zürück

#endif /* SERVO_H */
