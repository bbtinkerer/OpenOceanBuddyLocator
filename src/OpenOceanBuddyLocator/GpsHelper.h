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

#ifndef GpsHelper_h
#define GpsHelper_h

#include "OpenOceanBuddyLocatorConfig.h"
#include "OpenOceanBuddyLocator.h"
#include <GNSS.h>
#include <Arduino.h>

class GpsHelper{
  public:
    GpsHelper();
    void initialize();
    void update();
    double getLatitude();
    double getLongitude();
  private:
    SpGnss Gnss;
    double latitude;
    double longitude;
    void Led_isError(bool state);
    void Led_isActive(void);
    void Led_isPosfix(bool state);
    void print_condition(SpNavData *pNavData);
    void print_pos(SpNavData *pNavData);
};





#endif
