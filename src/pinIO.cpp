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

#include "pinIO.h"
#include <ui_io.h>

/**
 * @brief Print to console local IO Pin info, including expander, shift register etc.
 * 
 */
void printPinIOinfo() {
    byte i;
    io_printHeading("Printing local IO setup");
    io_print("Number of local input(switch) pins: ");
    io_println_n(sizeof(pinsA_in));
    if (sizeof(pinsA_in) > 0) { io_print_n(pinsA_in[0]); }
    for (i = 0; i < sizeof(pinsA_in); i++) {
        io_print(", ");
        io_print_n(pinsA_in[i]);
    }


}