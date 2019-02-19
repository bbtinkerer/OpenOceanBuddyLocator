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

#include "DisplayHelper.h"

DisplayHelper::DisplayHelper(){
  display = new Adafruit_SSD1306(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, DISPLAY_RESET_PIN);  
}

void DisplayHelper::initialize(){
  if(!display->begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS)) { 
    for(;;); // Don't proceed, loop forever
  }
  display->setTextSize(DISPLAY_TEXT_SIZE);
  display->setTextColor(WHITE); 
  print("Display", "OK", "", "");
}

void DisplayHelper::print(char* line1, char* line2, char* line3, char* line4){
  display->clearDisplay();
  display->setCursor(0, 0); 
  display->println(line1);
  display->println(line2);
  display->println(line3);
  display->println(line4);
  display->display();
}
