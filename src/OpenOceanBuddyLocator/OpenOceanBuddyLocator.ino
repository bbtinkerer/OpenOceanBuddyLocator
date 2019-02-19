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

#include "OpenOceanBuddyLocatorConfig.h"
#include "OpenOceanBuddyLocator.h"
#include "DisplayHelper.h"
#include "RadioHelper.h"
#include "GpsHelper.h"
#include "BatteryHelper.h"

#define LATITUDE_MAX 90.0
#define LATITUDE_MIN -90.0
#define LONGITUDE_MAX 180.0
#define LONGITUDE_MIN -180.0

char stringBuffer[128];
char displayLine[DISPLAY_LINES_COUNT][DISPLAY_LINE_BUFFER_SIZE];
DisplayHelper display;
RadioHelper radio(display);
GpsHelper gps;
BatteryHelper battery(BATTERY_ANALOG_PIN);

bool radiosConnected = false;
byte sendSerialByte = 0;
unsigned long lastReadTime = 0;
unsigned long lastBatteryDisplayTime = 0;
double latitude = NON_VALID_COORDINATE;
double longitude = NON_VALID_COORDINATE;
double readLatitude = NON_VALID_COORDINATE;
double readLongitude = NON_VALID_COORDINATE;
double distance = 0;
double bearing = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  
  display.initialize();
  radio.initialize();
  gps.initialize();
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long batteryTime = currentTime - lastBatteryDisplayTime;
  
  if((currentTime - lastReadTime) > SENSOR_READ_INTERVAL){

    latitude = gps.getLatitude();
    longitude = gps.getLongitude();

    if(batteryTime > BATTERY_VOLTAGE_DISPLAY_INTERVAL && batteryTime < BATTERY_VOLTAGE_DISPLAY_END){
      snprintf(displayLine[0], DISPLAY_LINE_BUFFER_SIZE, "Batt %.2fV", battery.getVoltage());
    } else {
      if(latitude > LATITUDE_MAX){
        snprintf(displayLine[0], DISPLAY_LINE_BUFFER_SIZE, "GPS Wait");
      } else {
        snprintf(displayLine[0], DISPLAY_LINE_BUFFER_SIZE, "GPS OK");
      }
  
      radiosConnected = radio.update(latitude, longitude);
      if(radiosConnected){
        snprintf(displayLine[1], DISPLAY_LINE_BUFFER_SIZE, "Com OK");
      } else {
        snprintf(displayLine[1], DISPLAY_LINE_BUFFER_SIZE, "Com Wait");
      }
    }

    if(batteryTime > BATTERY_VOLTAGE_DISPLAY_END){
      lastBatteryDisplayTime = currentTime;
    }


    readLatitude = radio.getLatitudeFromReadPayload();
    readLongitude = radio.getLongitudeFromReadPayload();

    if((LATITUDE_MIN < readLatitude && readLatitude < LATITUDE_MAX) && latitude > LATITUDE_MAX){
      snprintf(displayLine[2], DISPLAY_LINE_BUFFER_SIZE, "Buddy GPS");
      snprintf(displayLine[3], DISPLAY_LINE_BUFFER_SIZE, "Received");    
    } else if((LATITUDE_MIN < readLatitude && readLatitude < LATITUDE_MAX) && (LATITUDE_MIN < latitude && latitude < LATITUDE_MAX)){
      distance = calculateDistance(latitude, longitude, readLatitude, readLongitude);
      bearing = calculateBearing(latitude, longitude, readLatitude, readLongitude);

      // make sure distance is reasonable, sometimes noise gets read from the radio resulting in bad data
      if(distance < DISTANCE_LIMIT){
        sendNeoPixelPositionViaSerial2();
        snprintf(displayLine[2], DISPLAY_LINE_BUFFER_SIZE, "%.2fm", distance);
        snprintf(displayLine[3], DISPLAY_LINE_BUFFER_SIZE, "%.2f%c", bearing, DEGREE_CHAR);
      } else {
        displayWaitForBuddyGps();
      }
    } else {
      displayWaitForBuddyGps();
    }
    
    snprintf(stringBuffer, sizeof(stringBuffer), "Read Latitude: %.6f  Longitude: %.6f", readLatitude, readLongitude);
    Serial.println(stringBuffer);

    gps.update();

    lastReadTime = currentTime;
    
    display.print(displayLine[0], displayLine[1], displayLine[2], displayLine[3]);
  }
}

void displayWaitForBuddyGps(){
  snprintf(displayLine[2], DISPLAY_LINE_BUFFER_SIZE, "Wait for");
  snprintf(displayLine[3], DISPLAY_LINE_BUFFER_SIZE, "Buddy GPS");
}

void sendNeoPixelPositionViaSerial2(){
  sendSerialByte = (byte)(((360.0-bearing)/360.0)*NEOPIXEL_COUNT);
  sendSerialByte = (sendSerialByte) % NEOPIXEL_COUNT;
  Serial2.write(sendSerialByte);
}

//https://stackoverflow.com/questions/365826/calculate-distance-between-2-gps-coordinates
double degreesToRadians(double degrees){
  double radians = degrees * PI / 180.0;
  return radians;
}

double radiansToDegrees(double radians){
  double degrees = radians * 180.0 / PI;
  return degrees;
}

double calculateDistance(double lat1, double lon1, double lat2, double lon2){
  double earthRadiusMeters = 6371000;
  
  double dlat = degreesToRadians(lat2 - lat1);
  double dlon = degreesToRadians(lon2 - lon1);

  lat1 = degreesToRadians(lat1);
  lat2 = degreesToRadians(lat2);

  double a = sin(dlat/2) * sin(dlat /2) + 
            sin(dlon/2) * sin(dlon/2) *
            cos(lat1) * cos(lat2);

  double c = 2 * atan2(sqrt(a), sqrt(1-a));

  return earthRadiusMeters * c;
}


double calculateBearing(double lat1, double lon1, double lat2, double lon2){
  double dLon = degreesToRadians(lon2-lon1);

  lat1 = degreesToRadians(lat1);
  lat2 = degreesToRadians(lat2);
  
  double y = sin(dLon) * cos(lat2);
  double x = cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(dLon);
  double brng = atan2(y, x);
  brng = radiansToDegrees(brng);
  brng = fmod(brng+360.0, 360); // normalize
  return brng;
}
