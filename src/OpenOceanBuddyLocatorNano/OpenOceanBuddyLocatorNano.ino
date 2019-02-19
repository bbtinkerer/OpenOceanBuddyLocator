/*
* Open Ocean Buddy Locator
* Copyright (c) 2019 bbtinkerer
* 
* Device to give the direction of your buddy in the open ocean for 
* diving, fishing, kayaking, etc. Know the location of your buddy 
* without having to radio him in and give you directions.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      16

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second
/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

int currentPixelIndex = 0;
int previousPixelIndex = 0;
byte currentLocatorIndex = -1;
byte previousLocatorIndex = -1;

#define SOFTWARE_SERIAL_RX_PIN 2
#define SOFTWARE_SERIAL_TX_PIN 3

SoftwareSerial mySerial (SOFTWARE_SERIAL_RX_PIN, SOFTWARE_SERIAL_TX_PIN);
byte serialReadByte = -1;

void setup(void){
  Serial.begin(115200);
  mySerial.begin(9600);
  
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library. 
  /* Initialise the sensor */
  if(!mag.begin()){
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
}

void loop(void){
  sensors_event_t event; 
  mag.getEvent(&event);
  
  float Pi = 3.14159;
  
  // Calculate the angle of the vector y,x
  float heading = (atan2(event.magnetic.y,event.magnetic.x) * 180) / Pi;
  
  // Normalize to 0-360
  if (heading < 0){
    heading = 360 + heading;
  }
  
  previousPixelIndex = currentPixelIndex;
  currentPixelIndex = (int)((heading/360.0)*NUMPIXELS);

  previousLocatorIndex = currentLocatorIndex;

  if(mySerial.available()){
    serialReadByte = mySerial.read();
    Serial.print("Soft byte received: ");
    Serial.println(serialReadByte);
  } else {
    Serial.println("No Software Serial In");
  }

  if(0 <= serialReadByte && serialReadByte < NUMPIXELS){
    currentLocatorIndex = (currentPixelIndex + serialReadByte) % NUMPIXELS;
  } else {
    currentLocatorIndex = -1;
  }

  if(previousLocatorIndex >= 0){
    pixels.setPixelColor(previousLocatorIndex, pixels.Color(0,0,0)); // blank out previous
  }
  if(currentLocatorIndex >= 0){
    pixels.setPixelColor(currentLocatorIndex, pixels.Color(0,0,255)); // Moderately bright green color.
  }

  pixels.setPixelColor(previousPixelIndex, pixels.Color(0,0,0)); // blank out previous
  
  if(currentLocatorIndex != currentPixelIndex){
    pixels.setPixelColor(currentPixelIndex, pixels.Color(255,0,0)); 
  } else {
    pixels.setPixelColor(currentPixelIndex, pixels.Color(255,0,255)); 
  }

  pixels.show();
  delay(250);
}
