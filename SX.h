/*
 * SX.h
 *
 *  Created on: 02.01.2012
 *  Changed on: 10.11.2013
 *  Version:    2.1
 *  Copyright:  Michael Blank

*  interface hardware needed ! see www.oscale.net/SX

 Read SX Signal - SX Clock must be connected to Pin2=INT0 and
 SX Data must be connected to Pin4. For triggering a scope, a signal 
 can be generated at Pin3 at a defined base address.
 Pin6 can be connected via a 150 ohm resistor to the write line 
 of the SX bus
 
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

#ifndef SX_H_
#define SX_H_

#include <inttypes.h>

// define arduino pins 
#define SX_CLK   2     // must be INT0 !!
#define SX_DATA  4
#define SCOPE    3
#define SX_OUT   6     // for writing


// defines for state machine
#define SYNC  0
#define ADDR  1
#define DATA  2

#define MAX_DATACOUNT    7    // 7 dataframes in 1 SYNC Channel
#define MAX_DATABITCOUNT 12   // 12 bits in 1 frame

#define MAX_CHANNEL_NUMBER 112   // SX channels

class SX {
public:
	SX();
	void init(void);
    void init(uint8_t);	
	uint8_t get(uint8_t);
	uint8_t set(uint8_t, uint8_t);
	void isr(void);

private:
	void switchAdr(void);
	void switchData(void);


	uint8_t _toggle;
	uint8_t _zeroCount;
	uint8_t _adrCount;
	uint8_t _state;
	uint8_t _dataBitCount;    // bit counting
	uint8_t _dataFrameCount;  // frame counting

	uint8_t _data;    // 1 data uint8_t
	uint8_t _baseAdr;   // base address
	uint8_t _triggerAdr;  // SX address to trigger scope
    uint8_t _scopeFlag;   // generate scope trigger signal if != 0

	uint8_t _sx_write_channel;   // stores channel number to write to
    uint8_t _sx_write_data;      // data to write
    volatile uint8_t _sx_write_busy;    // waiting to send data, cannot accept new data
    uint8_t _sx_writing;         // currently writing a byte
	
	uint8_t _bit;
	uint8_t _sx[MAX_CHANNEL_NUMBER];   // to store the SX data

	uint8_t _channel;   // channel from 0 to 15, B3..B0 in sync data
	// 0  0  0  1  X   1  B3  B2  1  B1  B0  1 == sync frame of 12 bits
	

	/* SX Timing
	 1   Bit             50 µs
	 1   Kanal          600 µs (= 12 Bit)
	 1   Grundrahmen    ca. 4,8 ms
	 1   Gesamtrahmen   ca.  80 ms (= 16 Grundrahmen)  */
};

#endif /* SX_H_ */
