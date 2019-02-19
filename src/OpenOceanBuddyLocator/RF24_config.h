/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/*
* Open Ocean Buddy Locator
* Copyright (c) 2019 bbtinkerer
* 
* Device to give the direction of your buddy in the open ocean for 
* diving, fishing, kayaking, etc. Know the location of your buddy 
* without having to radio him in and give you directions.
*
* This program is a modified version to get working with the Spresense board.
*/

#ifndef __RF24_CONFIG_H__
#define __RF24_CONFIG_H__


  /*** USER DEFINES:  ***/  
  //#define FAILURE_HANDLING
  //#define SERIAL_DEBUG
  //#define MINIMAL
  //#define SPI_UART  // Requires library from https://github.com/TMRh20/Sketches/tree/master/SPI_UART
  //#define SOFTSPI   // Requires library from https://github.com/greiman/DigitalIO
  
  /**********************/
  #define rf24_max(a,b) (a>b?a:b)
  #define rf24_min(a,b) (a<b?a:b)

  #define RF24_SPI_TRANSACTIONS
 
 
//ATXMega
#if ( !defined (ARDUINO) ) // Any non-arduino device is handled via configure/Makefile

  // The configure script detects device and copies the correct includes.h file to /utility/includes.h
  // This behavior can be overridden by calling configure with respective parameters
  // The includes.h file defines either RF24_RPi, MRAA, LITTLEWIRE or RF24_SPIDEV and includes the correct RF24_arch_config.h file
  #include "utility/includes.h"
 
//Everything else
#else 

  #include <Arduino.h>
  
  // RF modules support 10 Mhz SPI bus speed
  const uint32_t RF24_SPI_SPEED = 1000000;  

#if defined (ARDUINO) && !defined (__arm__) && !defined (__ARDUINO_X86__)    
  #include <SPI.h>
  #define _SPI SPI5
#else
  // Define _BV for non-Arduino platforms and for Arduino DUE
  #include <stdint.h>
  #include <stdio.h>
  #include <string.h>


 #if defined(__arm__) || defined (__ARDUINO_X86__)
     #include <SPI.h>
     #define _SPI SPI5
 #elif !defined(__arm__) && !defined (__ARDUINO_X86__)
   extern HardwareSPI SPI;
 #endif
 
 #define _BV(x) (1<<(x))
  
#endif

  #ifdef SERIAL_DEBUG
	#define IF_SERIAL_DEBUG(x) ({x;})
  #else
	#define IF_SERIAL_DEBUG(x)
  #endif
  
#if  defined (__ARDUINO_X86__)
	#define printf_P printf
	#define _BV(bit) (1<<(bit))
#endif
  
// Progmem is Arduino-specific
// Arduino DUE is arm and does not include avr/pgmspace
#if defined (ARDUINO_ARCH_ESP8266)

  #include <pgmspace.h>
  #define PRIPSTR "%s"

#elif defined(ARDUINO) && !defined(ESP_PLATFORM) && ! defined(__arm__) && !defined (__ARDUINO_X86__) || defined(XMEGA)
	#include <avr/pgmspace.h>
	#define PRIPSTR "%S"
#else
  #if ! defined(ARDUINO) // This doesn't work on Arduino DUE
	typedef char const char;
  #else // Fill in pgm_read_byte that is used, but missing from DUE
	#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
  #endif


  typedef uint16_t prog_uint16_t;
  #define PSTR(x) (x)
  #define printf_P printf
  #define strlen_P strlen
  #define PROGMEM
  #define pgm_read_word(p) (*(p))

  #define PRIPSTR "%s"

#endif

#endif



#endif // __RF24_CONFIG_H__
