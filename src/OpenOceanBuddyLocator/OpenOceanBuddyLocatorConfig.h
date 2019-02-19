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

#ifndef OpenOceanBuddyLocatorConfig_h
#define OpenOceanBuddyLocatorConfig_h

// OLED display configurations
#define DISPLAY_I2C_ADDRESS 0x3C
#define DISPLAY_RESET_PIN      4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define DISPLAY_WIDTH        128
#define DISPLAY_HEIGHT        64
#define DISPLAY_TEXT_SIZE      2


// nRF24L01 configurations
#define RADIO_CHIP_ENABLE_PIN 19
#define RADIO_CHIP_SELECT_PIN 18
#define RADIO_ROLE_CONFIG_PIN 25
#define RADIO_CHANNEL        108 // 2.508 Ghz - Above most Wifi Channels
#define RADIO_POWER RF24_PA_HIGH
#define RADIO_SPEED RF24_250KBPS

#define NEOPIXEL_COUNT 16

// Spresense configurations
#define SENSOR_READ_INTERVAL             1000
#define BATTERY_ANALOG_PIN                 A3
#define BATTERY_VOLTAGE_DISPLAY_INTERVAL 3000
#define BATTERY_VOLTAGE_DISPLAY_END      6000
#define BATTERY_ANALOG_OFFSET               0
#define DISTANCE_LIMIT                   1000

#endif
