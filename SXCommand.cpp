/*
 * SXCommand.cpp
 *  holds an SX command, composed of channel, data and error
 *  methode:
 *        decode() an ASCII String to get an SX channel number 
 *        and sx data.
 *
 *  Created on: 10.11.2013
 *  Changed on: 10.11.2013
 *  Version:    2.1
 *  Copyright:  Michael Blank
 *
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 */


#include <Arduino.h> 

#include "SXCommand.h"
#include "SX.h"


SXCommand::SXCommand() {
    err = COMMAND_ERROR;
}

/** converts a string to 2 integer numbers 
 *     first one in the range 0..111 (sx channel)
 *     second one in the range 0..255 (data)
 */
void SXCommand::decode(String s) {
    
	uint16_t rv = 0;
	channel = 0;
	data = 0;
	err = COMMAND_ERROR;
	
    uint8_t pos = 0;
	if (s[pos] == '\0')
		return;    // string empty => error

	/* skip till we find sxChannel digit */
	while (s[pos]) {
		if (s[pos] <= '9' && s[pos] >= '0')
			break;
		pos++;
	}

	while (s[pos] && s[pos] >= '0' && s[pos] <= '9') {
		rv = (rv * 10) + (s[pos] - '0');
		pos++;
	}
	if (rv >= MAX_CHANNEL_NUMBER) {
		return;   // number >111 => error
	}
	channel = (uint8_t) (rv & 0xff);

	if (s[pos] == '\0')
		return;   // no second number => error
	rv = 0;
	/* skip till we find sxChannel digit */
	while (s[pos]) {
		if (s[pos] <= '9' && s[pos] >= '0')
			break;
		pos++;
	}

	while (s[pos] && s[pos] >= '0' && s[pos] <= '9') {
		rv = (rv * 10) + (s[pos] - '0');
		pos++;
	}
	if (rv >= 256) {
		return;    // number >255 => error
	}
	data = (uint8_t) (rv & 0xff);
	err = COMMAND_OK;
	return;

}