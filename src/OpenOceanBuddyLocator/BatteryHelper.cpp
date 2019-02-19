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

#include "BatteryHelper.h"

BatteryHelper::BatteryHelper(int pin){
  batteryAnalogPin = pin;
}

double BatteryHelper::getVoltage(){
  int analogReadValue = analogRead(batteryAnalogPin);  
  analogReadValue -= BATTERY_ANALOG_OFFSET;
  if(analogReadValue < 0 ){
    analogReadValue = 0;
  }
  double batteryVoltage = analogReadValue / 1024.0 * 5.0;
  return batteryVoltage;
}
