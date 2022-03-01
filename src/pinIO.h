/**
 * @file pinIO.h
 * @author your name (you@domain.com)
 * @brief Local to the MCU pin IO to include Pin expanders, shift registers, Expanders etc. 
 * 
 * But not the logic linking a switch to a relay or stuff like MQTT, Web, Network(ethernet, CAN, rs432, Slow bit bang etc.)
 * 
 * This should handle:
 *  1: Pin index to pin number maping.
 *  2: Debounce.
 *  3: 
 * 
 * Suported so far:
 *   1: Local MCU gpio pins with flick count support.
 *   2: i2c MCP23017 todo: input, output only so far
 *  
 * 
 * @version 0.1
 * @date 2022-02-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _pinIO_h
#define _pinIO_h

/**
 * @brief map for local IO pin, expanders id to command
 * 
 * mcu gpio indexes first.
 * expanders next then shift registers 
 * 
 */
extern byte inputIOMap[];// this should probably be storage/eeprom.




#include <Wire.h>
#include <pasStr.h>

#endif