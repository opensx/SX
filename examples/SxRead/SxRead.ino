/*
 * sx_read.ino
 *
 *  Created on: 02.01.2012
 *  Changed on: 13.01.2012
 *  
 *  Author: mblank
 *  
 *  Example program for the Selectrix(TM) Library
 *  sends SX data to serial port (only when data change)
 *  and uses the 2 LED on SX-shield to display the data
 *  of SX Channel 97-1 and 97-2
 *  
 */


#include <SX.h>   // this is the Selectrix library

#define LED_PIN  8   // on most Arduinos there is an LED at pin 13

#define LED_97_1 9    // read sx channel 97 and switch on/off LEDs 
#define LED_97_2 10   // on pins 9 and 10

SX sx;    // library

static int ledState = LOW;
static byte oldSx[MAX_CHANNEL_NUMBER];

void printSXValue(int i,int data) {
    // send data for 1 SX Channel on serial port
    Serial.print("SX ");
    Serial.print(i);
    Serial.print("=");
    Serial.println(data);
}

void toggleLed() {
    // to notify a change
	if (ledState == LOW)  {
		ledState = HIGH;
	} else {
		ledState = LOW;
	}
	digitalWrite(LED_PIN, ledState);
}

void sxisr(void) {
    // if you want to understand this, see:
    // http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1239522239   
    sx.isr();
}

void setup() {
    pinMode(LED_PIN,OUTPUT);
    pinMode(LED_97_1,OUTPUT);
    pinMode(LED_97_2,OUTPUT);
    
    digitalWrite(LED_PIN, ledState);
    Serial.begin(115200);      // open the serial port
    for (int i=0; i<112;i++) {
        oldSx[i]=0;   // initialize to zero
        printSXValue(i,0);
    }

    // initialize interrupt routine
    sx.init(97);

    // RISING slope on INT0 triggers the interrupt routine sxisr (see above)
    attachInterrupt(0, sxisr, RISING);   
}

void loop() {
	// check selectrix channels for changes
    for (int i=0; i<112; i++) {
        byte d=sx.get(i);
        if (oldSx[i] != d) {   // data have changed on SX bus
            oldSx[i] = d;
            printSXValue(i,d);   // send new value to serial port
            toggleLed();  // indicate change
        }
        if (i == 97) {
           digitalWrite(LED_97_1,bitRead(d,0));
           digitalWrite(LED_97_2,bitRead(d,1));
        }
    }
}




