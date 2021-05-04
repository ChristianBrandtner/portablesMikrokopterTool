/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/****************************************************************/
/*																															*/
/*									 NG-Video 5,8GHz										 				*/
/*																															*/
/*							Copyright (C) 2011 - gebad									 		*/
/*																															*/
/*	This code is distributed under the GNU Public License				*/
/*	which can be found at http://www.gnu.org/licenses/gpl.txt		*/
/*																															*/
/****************************************************************/

//############################################################################
//# HISTORY  mymath.c
//#
//# 24.04.2013 OG
//# - chg: 'uint16_t atantab[T] PROGMEM' nach 'const uint16_t atantab[T] PROGMEM'
//#        wegen Compile-Error in AtmelStudio 6
//############################################################################


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <avr/pgmspace.h>

#include "mymath.h"

// http://www.mikrocontroller.net/articles/AVR_Arithmetik#avr-gcc_Implementierung_.2832_Bit.29
unsigned int sqrt32(uint32_t q)
{	uint16_t r, mask;
	uint16_t i = 8*sizeof (r) -1;
	
	r = mask = 1 << i;
	for (; i > 0; i--) {
		mask >>= 1;
        
		if (q < (uint32_t) r*r)
			r -= mask;
		else
			r += mask;
	}
    
	if (q < (uint32_t) r*r)
		r -= 1;
    
	return r;
}

// aus Orginal MK source code
// sinus with argument in degree at an angular resolution of 1 degree and a discretisation of 13 bit.
const int16_t sinlookup[91] = {    0,  143,  286,  429,  571,  714,  856,  998, 1140, 1282, 1423, 1563, 1703,     \
                                1843, 1982, 2120, 2258, 2395, 2531, 2667, 2802, 2936, 3069, 3201, 3332, \
                                3462, 3591, 3719, 3846, 3972, 4096, 4219, 4341, 4462, 4581, 4699, 4815, \
                                4930, 5043, 5155, 5266, 5374, 5482, 5587, 5691, 5793, 5893, 5991, 6088, \
                                6183, 6275, 6366, 6455, 6542, 6627, 6710, 6791, 6870, 6947, 7022, 7094, \
                                7165, 7233, 7299, 7363, 7424, 7484, 7541, 7595, 7648, 7698, 7746, 7791, \
                                7834, 7875, 7913, 7949, 7982, 8013, 8041, 8068, 8091, 8112, 8131, 8147, \
                                8161, 8172, 8181, 8187, 8191, 8192};


int16_t c_cos_8192(int16_t angle)
{
	return (c_sin_8192(90 - angle));
}



int16_t c_sin_8192(int16_t angle)
{
	int8_t m, n;
	int16_t sinus;

	// avoid negative angles
	if (angle < 0)
	{
			m = -1;
			angle = -angle;
	}
	else m = +1;

	// fold angle to interval 0 to 359
	angle %= 360;

	// check quadrant
	if (angle <= 90) n = 1; // first quadrant
	else if ((angle > 90) && (angle <= 180)) {angle = 180 - angle; n = 1;} // second quadrant
	else if ((angle > 180) && (angle <= 270)) {angle = angle - 180; n = -1;} // third quadrant
	else {angle = 360 - angle; n = -1;}     //fourth quadrant
	// get lookup value
	sinus = sinlookup[angle];
	// calculate sinus value
	return (sinus * m * n);
}

// ----------------------------------------------------------------------------------------------


const int16_t arccos64[65] = {90,89,88,87,86, 85, 84, 83, 83, 82, 81, 80, 79, 78, 77, 76,
                          75, 74, 73, 72, 71, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62,
                          61, 60, 59, 58, 57, 56, 55, 54, 53, 51, 50, 49, 48, 47, 45,
                          44, 43, 41, 40, 39, 37, 36, 34, 32, 31, 29, 27, 25, 23, 20,
                          18, 14, 10, 0};



int16_t c_arccos2(int32_t a, int32_t b)
{
        if(a>b) return(0);
        return(arccos64[64 * a / b]);
}


/* ----------------------------------------------------------------------------------------------

  atan2.S

  Author:   Reiner Patommel
  
atan2.S uses CORDIC, an algorithm which was developed in 1956 by Jack Volder.
CORDIC can be used to calculate trigonometric, hyperbolic and linear
functions and is until today implemented in most pocket calculators.
The algorithm makes only use of simple integer arithmetic.

The CORDIC equations in vectoring mode for trigonometric functions are:
Xi+1 = Xi - Si * (Yi * 1 / 2^i)
Yi+1 = Yi + Si * (Xi * 1 / 2^i)
Zi+1 = Zi - Si *  atan(1 / 2^i)
where Si = +1 if Yi < 0 else Si = -1
The rotation angles are limited to -PI/2 and PI/2 i.e.
-90 degrees ... +90 degrees

For the calculation of atan2(x,y) we need a small pre-calculated table of
angles or radians with the values Tn = atan(1/2^i).
We rotate the vector(Xo,Yo) in steps to the x-axis i.e. we drive Y to 0 and
accumulate the rotated angles or radians in Z.  The direction of the rotation 
will be positive or negative depending on the sign of Y after the previous
rotation and the rotation angle will decrease from step to step. (The first
step is 45 degrees, the last step is 0.002036 degrees for n = 15).

After n rotations the variables will have the following values:
Yn	= ideally 0
Xn	= c*sqrt(Xo^2+Yo^2)			(magnitude of the vector)
Zn	= Zo+atan(Yo/Xo)			(accumulated rotation angles or radians)

c, the cordic gain, is the product Pn of sqrt(1+2^(-2i)) and amounts to
approx. 1.64676 for n = 15.

In order to represent X, Y and Z as integers we introduce a scaling factor Q
which is chosen as follows:
1.	We normalize Xo and Yo (starting values) to be less than or equal to 1*Q and 
	set Zo = 0 i.e. Xomax = 1*Q, Yomax = 1*Q, Zo = 0.
2.	With Xo = 1*Q and Yo = 1*Q, Xn will be Xnmax = Q*c*sqrt(2) = 2.329*Q
3.	In order to boost accuracy we only cover the rotation angles between 0 and PI/2
	i.e. X and Z are always > 0 and therefore can be unsigned.
	(We do the quadrant correction afterwards based on the initial signs of x and y)
4.	If X is an unsigned int, Xnmax = 65536, and Q = 65536/2.329 = 28140.
	i.e.
	Xnmax = 65536					--> unsigned int
	Ynmax = +/- 28140				--> signed int
	Znmax = PI/2 * 28140 = 44202	--> unsigned int
	The systematic error is 90/44202 = 0.002036 degrees or 0.0000355 rad.
	
Below is atan2 and atan in C: */

#define getAtanVal(x)			(uint16_t)pgm_read_word(&atantab[x])

const uint16_t atantab[T] PROGMEM = {22101,13047,6894,3499,1756,879,440,220,110,55,27,14,7,3,2,1};

int16_t my_atan2(int32_t y, int32_t x)
{	unsigned char i;
	uint32_t xQ;
	int32_t yQ;
	uint32_t angle = 0;
	uint32_t tmp;
	double x1, y1;

	x1 = abs(x);
	y1 = abs(y);

 	if (y1 == 0) {
		if (x < 0) 
			return (180);
		else 
			return 0;
	}
	
	if (x1 < y1) {
		x1 /= y1;
		y1 = 1;
	}
	else {
		y1 /= x1; 
		x1 = 1;
	}
	
	xQ = SCALED(x1);
	yQ = SCALED(y1);

	for (i = 0; i < T-1; i++) {
		tmp = xQ >> i;
		if (yQ < 0) {
			xQ -= yQ >> i;
			yQ += tmp;
			angle -= getAtanVal(i);
		}
		else {
			xQ += yQ >> i;
			yQ -= tmp;
			angle += getAtanVal(i);
		}
	}

	angle = RAD_TO_DEG * angle/Q;
	if (x <= 0)
		angle = 180 - angle;
	if (y <= 0) 
		return(-angle);
	return(angle);
}

