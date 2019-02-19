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

#ifndef RadioHelper_h
#define RadioHelper_h

#include "OpenOceanBuddyLocatorConfig.h"
#include "OpenOceanBuddyLocator.h"
#include "DisplayHelper.h"
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

typedef enum { role_ping_out = 1, role_pong_back } role_e; 

union{
  double fval;
  byte bval[sizeof(double)];
}doubleByteArrayUnion;

class RadioHelper{
  public:
    RadioHelper(DisplayHelper displayHelper);
    void initialize();
    bool update(double latitude, double longitude);
    double getLatitudeFromReadPayload();
    double getLongitudeFromReadPayload();
  private:
    RF24* radio;
    DisplayHelper display;
    role_e role; 
    byte radioNumber;
    byte addresses[][6];
    byte radioWritePayload[2 * sizeof(double)];
    byte radioReadPayload[2 * sizeof(double)];
    void setRadioWritePayload(double latitude, double longitude);
    void setRadioWritePayloadValue(double val, int startIndex);
    bool pingOut();
    bool pongBack();
    double getFloatFromReadPayload(int startIndex);
    double getLatitudeFromWritePayload();
    double getLongitudeFromWritePayload();
    double getFloatFromWritePayload(int startIndex);
    
};

#endif
