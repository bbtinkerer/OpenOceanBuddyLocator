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

#include "RadioHelper.h"

RadioHelper::RadioHelper(DisplayHelper displayHelper){
  radio = new RF24(RADIO_CHIP_ENABLE_PIN, RADIO_CHIP_SELECT_PIN);
  
  pinMode(RADIO_ROLE_CONFIG_PIN, INPUT_PULLUP);
  if(digitalRead(RADIO_ROLE_CONFIG_PIN)){
    role = role_ping_out;
    radioNumber = 0;
  } else {
    role = role_pong_back;
    radioNumber = 1;
  }
  
  memcpy(addresses[0], "1Node", 6);
  memcpy(addresses[1], "2Node", 6);
  
  display = displayHelper;
}

void RadioHelper::initialize(){
  Serial.println("Radio initializing");
  while(!radio->begin()){
    // radio did not respond
    //display.print("Radio", "did not", "respond", "retrying");
    delay(500);
  }

  radio->setPALevel(RADIO_POWER);
  radio->setChannel(RADIO_CHANNEL); 
  radio->setDataRate(RADIO_SPEED);
  radio->enableAckPayload(); 
  radio->enableDynamicPayloads();

  if(radioNumber){
    radio->openWritingPipe(addresses[1]);        // Both radios listen on the same pipes by default, but opposite addresses
    radio->openReadingPipe(1,addresses[0]);      // Open a reading pipe on address 0, pipe 1
  }else{
    radio->openWritingPipe(addresses[0]);
    radio->openReadingPipe(1,addresses[1]);
  }
  radio->startListening(); 

  setRadioWritePayload(NON_VALID_COORDINATE, NON_VALID_COORDINATE);
  memcpy(radioReadPayload, radioWritePayload, sizeof(radioWritePayload));
  radio->writeAckPayload(1,&radioWritePayload,sizeof(radioWritePayload));          // Pre-load an ack-paylod into the FIFO buffer for pipe 1
  radio->printDetails();
}

bool RadioHelper::update(double latitude, double longitude){
  setRadioWritePayload(latitude, longitude);
  if(role == role_ping_out){
    return pingOut();
  } else {
    return pongBack();
  }
}

void RadioHelper::setRadioWritePayload(double latitude, double longitude){
  setRadioWritePayloadValue(latitude, 0);
  setRadioWritePayloadValue(longitude, sizeof(double));
}

void RadioHelper::setRadioWritePayloadValue(double val, int startIndex){
  doubleByteArrayUnion.fval = val;
  int index = 0;
  for(int i = 0; i < sizeof(double); i++){
    index = i + startIndex;
    radioWritePayload[index] = doubleByteArrayUnion.bval[i];
  }
}

bool RadioHelper::pingOut(){
  bool receivedResponse = false;
  double latitude = getLatitudeFromWritePayload();
  double longitude = getLongitudeFromWritePayload();
  radio->stopListening();                                  
  
  snprintf(stringBuffer, sizeof(stringBuffer), "Ping out Latitude: %.6f  Longitude: %.6f", latitude, longitude);
  Serial.println(stringBuffer);
  
  unsigned long time = micros();                          
  unsigned long elapsedTime = 0;                                                    
  if ( radio->write(&radioWritePayload, sizeof(radioWritePayload)) ){                         
    if(!radio->available()){                            
      elapsedTime = micros()-time;
      snprintf(stringBuffer, sizeof(stringBuffer), "Got blank response. Round-trip delay %d uS", elapsedTime);
      Serial.println(stringBuffer);
    }else{      
      while(radio->available() ){
        radio->read(&radioReadPayload, sizeof(radioReadPayload));
        receivedResponse = true;
        unsigned long timer = micros();
        elapsedTime = timer-time;
        snprintf(stringBuffer, sizeof(stringBuffer), "Got response. Round-trip delay %d uS", elapsedTime);
        Serial.println(stringBuffer);
      }
    }  
  } else {
    Serial.println("Sending failed."); 
  }
  return receivedResponse;
}

bool RadioHelper::pongBack(){
  bool receivedResponse = false;
  byte pipeNo;
  double latitude = getLatitudeFromWritePayload();
  double longitude = getLongitudeFromWritePayload(); 

  snprintf(stringBuffer, sizeof(stringBuffer), "Pong back Latitude: %.6f  Longitude: %.6f", latitude, longitude);
  Serial.println(stringBuffer);
  
  while( radio->available(&pipeNo)){
    radio->read( &radioReadPayload, sizeof(radioReadPayload));                   
    radio->writeAckPayload(pipeNo,&radioWritePayload, sizeof(radioWritePayload));
    receivedResponse = true;
    snprintf(stringBuffer, sizeof(stringBuffer), "Loaded next response Latitude: %.6f  Longitude: %.6f", latitude, longitude);
    Serial.println(stringBuffer);
    
    latitude = getLatitudeFromReadPayload();
    longitude = getLongitudeFromReadPayload(); 
    
    snprintf(stringBuffer, sizeof(stringBuffer), "received Latitude: %.6f  Longitude: %.6f", latitude, longitude);
    Serial.println(stringBuffer);
  }
  return receivedResponse;
}

double RadioHelper::getLatitudeFromReadPayload(){
  return getFloatFromReadPayload(0);
}

double RadioHelper::getLongitudeFromReadPayload(){
  return getFloatFromReadPayload(sizeof(double));
}

double RadioHelper::getFloatFromReadPayload(int startIndex){
  int index = 0;
  for(int i = 0; i < sizeof(double); i++){
    index = i + startIndex;
    doubleByteArrayUnion.bval[i] = radioReadPayload[index];
  }
  return doubleByteArrayUnion.fval;
}

double RadioHelper::getLatitudeFromWritePayload(){
  return getFloatFromWritePayload(0);
}

double RadioHelper::getLongitudeFromWritePayload(){
  return getFloatFromWritePayload(sizeof(double));
}

double RadioHelper::getFloatFromWritePayload(int startIndex){
  int index = 0;
  for(int i = 0; i < sizeof(double); i++){
    index = i + startIndex;
    doubleByteArrayUnion.bval[i] = radioWritePayload[index];
  }
  return doubleByteArrayUnion.fval;
}
