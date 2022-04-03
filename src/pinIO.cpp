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

void setupPinIO()
{
    SetUpInputs();
}

void pinIO_SwitchesExe()
{
    SwitchesExe();
}

/**
 * @brief Print to console local IO Pin info, including expander, shift register etc.
 *
 */
void printPinIOinfo()
{
    byte i;
    // io_printHeading("Printing local IO setup");
    io_print("Number of local input(switch) pins: ");
    io_println_n(pinIO_Max_switches);
    if (pinIO_Max_switches > 0)
    {
        io_print_n(pinIO_pinsA_in[i]);
        if (pinIO_pinsA_in[i] >= A0)
        {
            Serial.print(F("(A"));
            Serial.print(pinIO_pinsA_in[i] - A0);
            Serial.print(")");
        }
    }
    for (i = 1; i < pinIO_Max_switches; i++)
    {
        io_print(", ");
        io_print_n(pinIO_pinsA_in[i]);
        if (pinIO_pinsA_in[i] >= A0)
        {
            Serial.print(F("(A"));
            Serial.print(pinIO_pinsA_in[i] - A0);
            Serial.print(")");
        }
    }
    io_ln;
    for (i = 0; i < pinIO_Max_switches; i++)
    {
        debugSwitch(i);
    }
}