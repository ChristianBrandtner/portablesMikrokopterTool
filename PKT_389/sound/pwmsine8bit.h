/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*
 * pwmsine8bit.h
 *
 *  Created on: 01.09.2012
 *      Author: cebra
 */

#ifndef PWMSINE8BIT_H_
#define PWMSINE8BIT_H_

extern unsigned char   freqincipart,freqincfpart;
extern unsigned char   waveptipart,waveptfpart;
extern unsigned char   iattenfac; //0-255
extern unsigned char   generate;
extern volatile unsigned char sindat[256]; //8 bit sine table
void InitSound(void);
void tone(int hz,uint8_t volume, uint16_t ms, uint16_t soundpause);
void tone1(int hz);
void playTone(uint16_t f_Hz, uint16_t dur_ms, uint8_t volume);
void tone_handler(void);

#endif /* PWMSINE8BIT_H_ */
