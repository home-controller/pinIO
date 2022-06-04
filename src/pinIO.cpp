/**
 * @file pinIO.cpp
 * @author your name (you@domain.com)
 * @brief To encaptialate all local pinIO including expanders and shift registers.
 * @version 0.1
 * @date 2022-03-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <Arduino.h>
#include "pinIO.h"
#include "I.h"
#include <ui_io.h>

 /**
  * @brief Construct a new pinIO C::pinIO C object
  *
  * @param outPinsA byte array of GPIO pin numbers e.g. connected to a relay, LED etc.
  * @param numOut size of the above array.
  * @param inPinsA  byte array of GPIO pin numbers e.g. connected to a wall switch etc.
  * @param numIn
  */
pinIO_C::pinIO_C(byte outPinsA[ ], byte numOut, byte inPinsA[ ], byte numIn) {
    outGPIO_pinsA = outPinsA;
    numberOutPins = numOut;
    inGPIO_pinsA = inPinsA;
    numberInPins = numIn;

}

void pinIO_C::printOutputInfo() {
    byte i;
    io_print("Number GPIO output pins:");
    io_println_n(numberOutPins);
    for (i = 0; i < numberOutPins; i++) {
        io_print("i:"); io_print_n(i);
        //if (i > 0) io_print(", ");
        io_print(", MCU pin:"); vtGreen; io_print_n(outGPIO_pinsA[i]);
        if (outGPIO_pinsA[i] >= A0) {
            Serial.print(F("(A"));
            Serial.print(outGPIO_pinsA[i] - A0);
            Serial.print(")");
        }
        vtResetColour; io_ln;
    }

}

/**
 * @brief Print to console local GPIO input Pin info, including expander, shift register etc.
 *
 */
void pinIO_C::printInputInfo() {
    byte i;
    // io_printHeading("Printing local IO setup");
    io_print("GPIO input(switch) pins: ");
    io_println_n(numberInPins);
    for (i = 0; i < numberInPins; i++) {
        debugSwitch(i);
    }
}

void setupPinIO() {
    SetUpInputs();
}

void pinIO_SwitchesExe() {
    SwitchesExe();
}
