#include <Arduino.h>
#include "ui_io.h"
#include "../../pinIO/src/pinIO.h"

#define pinIO_no_of_switches 5          // setup the number of gpio's used
#define pinIO_inPins A7, A6, A0, A1, A2 // in sa main.h

#if pinIO_no_of_switches >= 1
byte pinIO_pinsA_in[pinIO_no_of_switches] = {pinIO_inPins};
// byte switchState[ pinIO_no_of_switches ];// 0 = off, 1 = on
byte pinIO_Max_switches = pinIO_no_of_switches; // as no easy way to pass #defines with -D in Arduino ide and needs to pas back to already compiled lib.
byte pinIO_switchState[pinIO_no_of_switches];
#endif

void setString(char r[], const char s[])
{
  byte i = 0;
  while (s[i] != 0 and (i < 15))
  {
    r[i] = s[i];
    i++;
  }
  r[i] = 0;
}


void setup()
{
  // put your setup code here, to run once:
  // pinIO_Max_switches = pinIO_no_of_switches
  Serial.begin(38400);
  setupPinIO();
  printPinIOinfo();
}

void loop()
{
  pinIO_SwitchesExe();
  // put your main code here, to run repeatedly:
}