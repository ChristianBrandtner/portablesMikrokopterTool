/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*
 * FollowMe.h
 *
 *  Created on: 18.05.2012
 *      Author: cebra
 */

#ifndef FOLLOWME_H_
#define FOLLOWME_H_

#include "../mk-data-structs.h"
#include <inttypes.h>

void FollowMe (void);
void Debug_GPS (void);
void sendFollowMeData(Point_t *tFollowMe, uint32_t *tsend_followme_counter, uint32_t  *tNMEA_GPGGA_counter_old);
void printFollowMeStatic(void);


#endif /* FOLLOWME_H_ */
