/**
 * @file D_Serial_Debug.h
 *
 * @section description Description
 * A header file that allows for simple serial monitor debugging. 
 *
 * @section author Author
 * - Created by JTB on 20251103.
 * - Modified by by JTB on 20251103.
 */

#ifndef __D_SERIAL_DEBUG_H__
	#define __D_SERIAL_DEBUG_H__

#if DEBUG
  #define D_SerialBegin(...)       Serial.begin(__VA_ARGS__);
  #define D_SerialPrint(...)       Serial.print(__VA_ARGS__)
  #define D_SerialWrite(...)       Serial.write(__VA_ARGS__)
  #define D_SerialPrintln(...)     Serial.println(__VA_ARGS__)
  #define D_SerialAvailable()      Serial.available()
  #define D_SerialFlush()          Serial.flush()
  #define D_SERIAL_DELAY           1500
  #define D_SERIAL_BAUD_RATE       115200
#else
  #define D_SerialBegin(bauds)
  #define D_SerialPrint(...)
  #define D_SerialWrite(...)
  #define D_SerialPrintln(...)
  #define D_SerialAvailable()
  #define D_SerialFlush()
  #define D_SERIAL_DELAY
  #define D_SERIAL_BAUD_RATE
#endif

#endif // __D_SERIAL_DEBUG_H__