/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

//file pwmsine8bit.c
//generate 8 bit sinewave using wavetable lookup
//12khz sample rate, 62500 hz pwm on pd5 oc1a
//for ere co embmega32 16Mhz 38400

//May 3 07 Bob G initial edit 8 bit
//May 5 07 Bob G add traffic tones
//Jan 18 08 Bob G compile with 7.15

//#include <stdio.h>
//#include <stdlib.h>


#include <ctype.h>
#include <math.h>   //sin
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include <stdint.h>
#include "../timer/timer.h"
#include "../main.h"
#ifdef USE_SOUND


#define INTR_OFF() asm("CLI")
#define INTR_ON()  asm("SEI")


//------vars in bss-------

volatile unsigned char sindat[256]; //8 bit sine table
//unsigned int addat[8];    //internal 10 bit
unsigned int    idesfreq;
unsigned char   iattenfac; //0-255
unsigned char   dispon;
unsigned char   freqincipart,freqincfpart;
unsigned char   waveptipart,waveptfpart;
unsigned char   tics;
unsigned char   generate;

float basefreq;
float freqratio;
float sampfreq;  //12khz
float samppd;    //83.33usec
float fdesfreq;
float attendb;   //0-50
float attenfac;  //.999-0
const float pi=3.141592654;



#define MSB(w) ((char*) &w)[1]
#define LSB(w) ((char*) &w)[0]

#define TONE_BUFFER_SIZE 64
#define TONE_BUFFER_MASK ( TONE_BUFFER_SIZE - 1)

static volatile unsigned char ToneBuf[TONE_BUFFER_SIZE];
static volatile unsigned char ToneBufHead;
static volatile unsigned char ToneBufTail;


  
//-------------------------------
void cvtfreq2frac(void){
//calc freq ratio, separate into integer and fractional parts
	
  fdesfreq=idesfreq;
  freqratio=fdesfreq/basefreq;               //calc freq ratio from basefreq
  freqincipart=(int)freqratio;               //calc integer part
  freqincfpart=(freqratio-freqincipart)*256; //calc fractional part
//  cprintf("ipart %02x  fpart %02x\n",freqincipart,freqincfpart);  
}

  
//-------------------------------
void cvtdb2fac(void){
//cvt db to attenuation factor

  attenfac=pow(10.0,-attendb/20.0); //10^0=1
  iattenfac=(unsigned char)(attenfac*255.0);
//  cprintf("attenfac %#7.5f\n",attenfac);
//  cprintf("iattenfac %02x\n",iattenfac);
}

//-------------------------
void initsindat(void){
//init 8 bit signed sin table 0-ff in ram 36dB s/n   46Hz basefreq
unsigned int n;

  for(n=0; n < 256; n++){
    sindat[n]=(signed char)127.0*sin(2.0*pi*n/256.0); //256 sin points
  }
}


//-------------------------
void initvars(void){
//init vars

//  sampfreq=12000.0;    //hz
  sampfreq=19530.0;    //hz
//  printf("samp freq %#7.1f hz\n\r",sampfreq);
//  printf("samp freq %#7.1f hz\n\r",1234567.1);
  samppd=1.0/sampfreq; //83.33usec
//  printf("samppd %#7.1f usec \n\r",samppd*1e6);
  basefreq=sampfreq/256.0; //46.875hz
//  printf("basefreq %#7.5f hz\n\r",basefreq);
  idesfreq=400;
  cvtfreq2frac();
  cvtdb2fac(); //0db =.9999= 0xff
}


//---------------------




void tone1(int hz)
//tone at hz for ms

{

  uint8_t volume = 0;
//  TCNT2=0;
//  sound_timer = ms/5;
//  soundpause_timer = soundpause/5;

  idesfreq=hz;
  cvtfreq2frac();
  attendb=volume;
  cvtdb2fac();
  if (hz ==0) generate=0;
   else { generate = 1;
//          TIMSK2 |= _BV(TOIE2);          // Interrupt freigeben
        }

}



//-------------------------------
void tone_handler(void)  // wird aus Timerinterrupt aufgerufen

{
  uint16_t f_Hz=0;
  uint16_t dur_ms=0;
  char volume=0;
  char tmp_high = 0;
  char tmp_low = 0;
  unsigned char tmptail;


    if ( ToneBufHead != ToneBufTail) {
        /* calculate and store new buffer index */
        tmptail = (ToneBufTail + 1) & TONE_BUFFER_MASK;
        ToneBufTail = tmptail;
        /* get one byte from buffer */
        tmp_low = ToneBuf[tmptail];  /* f_Hz low */
    }else{

        return;
    }
    if ( ToneBufHead != ToneBufTail) {
        /* calculate and store new buffer index */
        tmptail = (ToneBufTail + 1) & TONE_BUFFER_MASK;
        ToneBufTail = tmptail;
        /* get one byte from buffer */
        tmp_high = ToneBuf[tmptail];  /* f_Hz high */
    }else{
        return;
    }

      f_Hz = (((uint16_t) tmp_high) << 8) | tmp_low;

      if ( ToneBufHead != ToneBufTail) {
          /* calculate and store new buffer index */
          tmptail = (ToneBufTail + 1) & TONE_BUFFER_MASK;
          ToneBufTail = tmptail;
          /* get one byte from buffer*/
          volume = ToneBuf[tmptail];  /* volume */
      }else{
          return;
      }

      if ( ToneBufHead != ToneBufTail) {
          /* calculate and store new buffer index */
          tmptail = (ToneBufTail + 1) & TONE_BUFFER_MASK;
          ToneBufTail = tmptail;
          /* get one byte from buffer */
          tmp_low = ToneBuf[tmptail];  /* low Duratuion */
      }else{
          return;
      }
      if ( ToneBufHead != ToneBufTail) {
          /* calculate and store new buffer index */
          tmptail = (ToneBufTail + 1) & TONE_BUFFER_MASK;
          ToneBufTail = tmptail;
          /* get one byte from buffer*/
          tmp_high = ToneBuf[tmptail];  /* high Duration */
      }else{
          return;
      }

      dur_ms = (((uint16_t) tmp_high) << 8) | tmp_low;

      sound_timer = dur_ms/10;
//      TCNT2=0;
      idesfreq= f_Hz;
      cvtfreq2frac();
      attendb= volume;
      cvtdb2fac();

      if (f_Hz ==0) generate=0;
       else { generate = 1;
//              TIMSK2 |= _BV(TOIE2);          // Interrupt freigeben
       }



//      printf("tone_handler f_Hz: %i dur_ms: %i volume: %i\n\r", f_Hz,dur_ms,volume);

}


void tone_putc(unsigned char data)
{
    unsigned char tmphead;

    tmphead  = (ToneBufHead + 1) & TONE_BUFFER_MASK;

    while ( tmphead == ToneBufTail ){
        ;/* wait for free space in buffer */
    }

    ToneBuf[tmphead] = data;
    ToneBufHead = tmphead;

}/* tone_putc */


void playTone(uint16_t f_Hz, uint16_t dur_ms, uint8_t volume)

 {

//  printf("Playtone f_Hz: %i dur_ms: %i volume: %i\n\r", f_Hz,dur_ms,volume);

  tone_putc(LSB(f_Hz));
  tone_putc( MSB(f_Hz));
  tone_putc( volume);
  tone_putc(LSB(dur_ms));
  tone_putc(MSB(dur_ms));


 }


//-----------------
void InitSound(void){//main program



  ToneBufHead = 0;
  ToneBufTail = 0;

  initvars();
  initsindat();

  Timer2_Init();
  Timer3_Init();




}

#endif
