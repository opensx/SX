/*
 * sx_interface.ino
 *
 *  Created on: 10.11.2013
 *  Changed on:  
 *  
 *  Author: Michael Blank
 *  
 *  Example program for the Selectrix(TM) Library
 *  sends SX data to serial port (only when data change)
 *  format:    V  80  16   (data bit 5 set on sx channel 80)
 *  and reads commands from the serial port, interprets
 *  them and send them on SX Bus.
 *  format:    S <ch> <data>   (or SET .. ..)
 *             S  80  255   sets all bits for channel 80      
 *  
 */


#include <SX.h>   // this is the Selectrix library
#include <SXCommand.h>   // this is the Selectrix Command library

#define LED_PIN  13   // on most Arduinos there is an LED at pin 13

#define LED_CMD  8
#define LED_97_1 9    // read sx channel 97 and switch on/off LEDs 
#define LED_97_2 10   // on pins 9 and 10

SX sx;                // library
SXCommand sxcmd;      // holds command data

static int ledState = LOW;
static byte oldSx[MAX_CHANNEL_NUMBER];

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
boolean cmdState = false;


void printSXValue(int i,int data) {
    // send data for 1 SX Channel on serial port
    Serial.print("V ");
    Serial.print(i);
    Serial.print(" ");
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
    inputString.reserve(80); 
    pinMode(LED_PIN,OUTPUT);
    pinMode(LED_CMD,OUTPUT);
    pinMode(LED_97_1,OUTPUT);
    pinMode(LED_97_2,OUTPUT);
    pinMode(SCOPE,OUTPUT);
    
    digitalWrite(LED_PIN, ledState);
    Serial.begin(115200);      // open the serial port
    for (int i=0; i<112;i++) {
        oldSx[i]=0;   // initialize to zero
        printSXValue(i,0);
    }

    // initialize interrupt routine
    sx.init(97);   // scope triggered at channel 97

    // RISING slope on INT0 triggers the interrupt routine sxisr (see above)
    attachInterrupt(0, sxisr, RISING); 
} 

/*
 SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
 
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      if (cmdState == false) {
        digitalWrite(LED_CMD,HIGH);
        cmdState = true;
      } else {
        digitalWrite(LED_CMD,LOW);
        cmdState = false;
      }
	  if ( (inputString[0] == 's') ||
               (inputString[0] == 'S') ) {
	     sxcmd.decode(inputString);
		 if (sxcmd.err == COMMAND_OK) {
		    Serial.println("OK ");
			do {
				delay(10);
			} while (sx.set(sxcmd.channel,sxcmd.data));
		} else {
		    Serial.println("ERR");
		}
	  } else {
		    Serial.println("ERR");
	  }
      //Serial.print("read=");
      //Serial.println(inputString);
      inputString="";
      stringComplete = false;
    } 
  }
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
           digitalWrite(LED_97_1,bitRead(d,1));
           digitalWrite(LED_97_2,bitRead(d,2));
        }
    }
    
}




