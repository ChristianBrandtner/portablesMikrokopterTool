/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*-
 * Copyright (c) 2012,2013 Darran Hunt (darran [at] hunt dot net dot nz)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Release history
 *
 * Version  Date         Description
 * 0.1      25-Mar-2012  First release.
 * 0.2      09-Apr-2012  Added features to support http servers.
 *                       - added an httpserver.ino example.
 *                       - added sendChunk() and sendChunkln() to send chunked HTTP bodies.
 *                       - added terminal() method for simple terminal access via debug stream
 *                       - replaced getFreeMemory() with simpler version that works with 0 bytes
 *                       - turned peek buffer into a circular buffer to fix bug with detecting
 *                         *CLOS* and *OPEN* after a partial match.
 *                       - Added new TCP connection detection via *OPEN* match from available().
 *                         isConnected() can now be polled until a client connects.
 *                       - made the match() function public, handy for matching text in a stream.
 *                       - Added a getProtocol() function to get current set of protocols.
 * 0.3      21-Apr-2012  Added createAdhocNetwork() to create an Ad Hoc WiFi network.
 *           Optimised the setopt() and getopt() function so they handle
 *           integer conversions and refactored all of the set and get functions.
 *           Added a multMatch_P() function to match serial data against multiple
 *           progmem strings.
 *           Added failure detection to the join() function to quickly detect
 *           a failure rather than relying on a timeout.
 *           Added setJoin() and getJoin() function for access to the wlan join parameter.
 *           Refactored getres() to use the new multiMatch_P() function.
 *
 */

/**
 * @mainpage WiFlyHQ WiFly RN-XV Arduino library
 *
 * This library provides functions for setting up and managing the WiFly module,
 * sending UDP packets, opening TCP connections and sending and receiving data
 * over the TCP connection.
 *
 * @author Harlequin-Tech
 */

//############################################################################
//# HISTORY  PKT_WiFlyHQ.h
//#
//# 13.04.2014 OG
//# - FAST KOMPLETT NEUER CODE
//#
//# 02.04.2014 OG
//# - add: Versionshistorie ergaenzt
//###########################################################################


#ifndef _WIFLYHQ_H_
#define _WIFLYHQ_H_

// Todo: Strings verfuegbar machen für connect.c
//extern const char WIFLY_IP_ADDRESS[];
//extern const char WIFLY_IP_PORT[];


uint8_t wifly_update( uint8_t wiflymode );
uint8_t wifly_reset(void);
uint8_t wifly_version_show( void );
uint8_t wifly_init( uint8_t wiflymode );

uint8_t wifly_test( void );

#endif
