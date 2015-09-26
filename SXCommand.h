/*
 * SXCommand.h
 *
 *  Created on: 10.11.2013
 *  Changed on: 10.11.2013
 *  Version:    2.1
 *  Copyright:  Michael Blank
 
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

#ifndef SXCOMMAND_H_
#define SXCOMMAND_H_
#include <WString.h>
#include <inttypes.h>

#define COMMAND_OK       0
#define COMMAND_ERROR    1


class SXCommand {
   
public:
	SXCommand();
	void decode(String);
	uint8_t channel;
    uint8_t data;
    uint8_t err; 

private:
	
};

#endif /* SXCOMMAND_H_ */
