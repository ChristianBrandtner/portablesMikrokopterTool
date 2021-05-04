#ifndef _MYMATH_H_
#define _MYMATH_H_

#include <avr/io.h>

#define 	T		16
#define 	Q		28140
#define 	SCALED(X)	((int32_t)((X) * Q))
#define 	RAD_TO_DEG		57.2957795	// radians to degrees = 180 / PI

uint16_t sqrt32(uint32_t qzahl);
int16_t c_cos_8192(int16_t angle);
int16_t c_sin_8192(int16_t angle);
int16_t my_atan2(int32_t y, int32_t x);

#endif
