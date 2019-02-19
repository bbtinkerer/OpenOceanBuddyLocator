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

#include "GpsHelper.h"

GpsHelper::GpsHelper(){
  latitude = NON_VALID_COORDINATE;
  longitude = NON_VALID_COORDINATE;
}

void GpsHelper::initialize(){
  int error_flag = 0;
  /* Turn on all LED:Setup start. */
  ledOn(PIN_LED0);
  ledOn(PIN_LED1);
  ledOn(PIN_LED2);
  ledOn(PIN_LED3);

  /* Set Debug mode to Info */
  Gnss.setDebugMode(PrintInfo);

  int result;

  /* Activate GNSS device */
  result = Gnss.begin();

  if (result != 0)
  {
    Serial.println("Gnss begin error!!");
    error_flag = 1;
  }
  else
  {
    /* Setup GNSS */
    Gnss.select(GPS);  // Michibiki complement
    Gnss.select(GLONASS);   // Michibiki augmentation(Valid only in Japan)

    /* Start positioning */
    result = Gnss.start(COLD_START);
    if (result != 0)
    {
      Serial.println("Gnss start error!!");
      error_flag = 1;
    }
    else
    {
      Serial.println("Gnss setup OK");
    }
  }

  /* Turn off all LED:Setup done. */
  ledOff(PIN_LED0);
  ledOff(PIN_LED1);
  ledOff(PIN_LED2);
  ledOff(PIN_LED3);

  /* Set error LED. */
  if (error_flag == 1)
  {
    Led_isError(true);
    exit(0);
  }
}

void GpsHelper::update()
{
  /* put your main code here, to run repeatedly: */

  static int LoopCount = 0;
  static int LastPrintMin = 0;

  /* Blink LED. */
  Led_isActive();

  /* Check update. */
  if (Gnss.waitUpdate(-1))
  {
    /* Get NaviData. */
    SpNavData NavData;
    Gnss.getNavData(&NavData);

    /* Set posfix LED. */
    bool LedSet = (NavData.posDataExist && (NavData.posFixMode != FixInvalid));
    Led_isPosfix(LedSet);

    /* Print satellite information every minute. */
    if (NavData.time.minute != LastPrintMin)
    {
      print_condition(&NavData);
      LastPrintMin = NavData.time.minute;
    }

    /* Print position information. */
    print_pos(&NavData);
  }
  else
  {
    /* Not update. */
    Serial.println("data not update");
  }
}

double GpsHelper::getLatitude(){
  return latitude;
}
double GpsHelper::getLongitude(){
  return longitude;  
}

void GpsHelper::print_condition(SpNavData *pNavData)
{
  unsigned long cnt;

  /* Print satellite count. */
  snprintf(stringBuffer, sizeof(stringBuffer), "numSatellites:%2d\n", pNavData->numSatellites);
  Serial.print(stringBuffer);

  for (cnt = 0; cnt < pNavData->numSatellites; cnt++)
  {
    const char *pType = "---";
    SpSatelliteType sattype = pNavData->getSatelliteType(cnt);

    /* Get satellite type. */
    /* Keep it to three letters. */
    switch (sattype)
    {
      case GPS:
        pType = "GPS";
        break;

      case GLONASS:
        pType = "GLN";
        break;

      case QZ_L1CA:
        pType = "QCA";
        break;

      case SBAS:
        pType = "SBA";
        break;

      case QZ_L1S:
        pType = "Q1S";
        break;

      default:
        pType = "UKN";
        break;
    }

    /* Get print conditions. */
    unsigned long Id  = pNavData->getSatelliteId(cnt);
    unsigned long Elv = pNavData->getSatelliteElevation(cnt);
    unsigned long Azm = pNavData->getSatelliteAzimuth(cnt);
    float sigLevel = pNavData->getSatelliteSignalLevel(cnt);

    /* Print satellite condition. */
    snprintf(stringBuffer, sizeof(stringBuffer), "[%2d] Type:%s, Id:%2d, Elv:%2d, Azm:%3d, CN0:", cnt, pType, Id, Elv, Azm );
    Serial.print(stringBuffer);
    Serial.println(sigLevel, 6);
  }
}

void GpsHelper::print_pos(SpNavData *pNavData)
{
  //char StringBuffer[STRING_BUFFER_SIZE];

  /* print time */
  snprintf(stringBuffer, sizeof(stringBuffer), "%04d/%02d/%02d ", pNavData->time.year, pNavData->time.month, pNavData->time.day);
  Serial.print(stringBuffer);

  snprintf(stringBuffer, sizeof(stringBuffer), "%02d:%02d:%02d.%06d, ", pNavData->time.hour, pNavData->time.minute, pNavData->time.sec, pNavData->time.usec);
  Serial.print(stringBuffer);

  /* print satellites count */
  snprintf(stringBuffer, sizeof(stringBuffer), "numSat:%2d, ", pNavData->numSatellites);
  Serial.print(stringBuffer);

  /* print position data */
  if (pNavData->posFixMode == FixInvalid)
  {
    Serial.print("No-Fix, ");
  }
  else
  {
    Serial.print("Fix, ");
  }
  if (pNavData->posDataExist == 0)
  {
    Serial.print("No Position");
  }
  else
  {
    latitude = pNavData->latitude;
    longitude = pNavData->longitude;
    Serial.print("Lat=");
    Serial.print(pNavData->latitude, 6);
    Serial.print(", Lon=");
    Serial.print(pNavData->longitude, 6);
  }

  Serial.println("");
}

/**
 * @brief Turn on / off the LED0 for CPU active notification.
 */
void GpsHelper::Led_isActive()
{
  static int state = 1;
  if (state == 1)
  {
    ledOn(PIN_LED0);
    state = 0;
  }
  else
  {
    ledOff(PIN_LED0);
    state = 1;
  }
}

/**
 * @brief Turn on / off the LED1 for positioning state notification.
 *
 * @param [in] state Positioning state
 */
void GpsHelper::Led_isPosfix(bool state)
{
  if (state)
  {
    ledOn(PIN_LED1);
  }
  else
  {
    ledOff(PIN_LED1);
  }
}

/**
 * @brief Turn on / off the LED3 for error notification.
 *
 * @param [in] state Error state
 */
void GpsHelper::Led_isError(bool state)
{
  if (state)
  {
    ledOn(PIN_LED3);
  }
  else
  {
    ledOff(PIN_LED3);
  }
}
