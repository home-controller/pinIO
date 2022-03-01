
#include "I.h"
#include <defs.h>
#include <Arduino.h>
#include "O.h"
#include <avr/wdt.h>
//#include <EEPROM.h>
//#include "mqtt.h"

unsigned long lastMils = millis();

boolean ReadSwitch(byte i){ //digitalRead() don't works with A6 and A7. Also moved looking up the pin from switch No. index here.
  byte pin;
  pin = pinIO_pinsA_in[i];
  if ( (pin == A6) or (pin == A7) ){
    if ( analogRead(pin) > 600 ) {return HIGH; }
    return LOW;
  }
  return digitalRead(pin);
}

void SetUpInputs(){
  byte i;
  Serial.println(F("Todo: still only MCU pins input. No expanders") );//expanders etc. should be in different unit maybe.
  for(i=0; i<pinIO_Max_switches;i++){
    Serial.print(F("MCU pin '") ); Serial.print(pinIO_pinsA_in[i]); Serial.print(F("' set to input, Switch No. = ") ); Serial.println(i);
    pinMode(pinIO_pinsA_in[i], INPUT_PULLUP);      // sets the input pins as input. Pullup works on A0 to A5 but A6 & A7 still float
    //pinMode(pinsA_in[i], INPUT);      // sets the input pins as input. If not use pullup may be able to detect bad connection.
    if(ReadSwitch(i) ) {
      pinIO_switchState[i] = 0b11;
    } else {
      pinIO_switchState[i] = 0b0;
    }
  }
}

void debugSwitch(byte i){
  Serial.print(F("pin state: ") );  Serial.print(ReadSwitch(i) );  Serial.print(F(", i: ") );  Serial.print(i);  Serial.print(F(", pin: ") );
  if(pinIO_pinsA_in[i] >= A0){ Serial.print(pinIO_pinsA_in[i]); Serial.print('(');Serial.print('A'); Serial.print(pinIO_pinsA_in[i]- A0); Serial.print(')');}
  else Serial.print(pinIO_pinsA_in[i]);  
  Serial.print(F(", saved: b") );    Serial.print(pinIO_switchState[i] & 0b1, BIN);
  Serial.print(F(", New: b") );  Serial.print((pinIO_switchState[i] >>1) & 0b1, BIN);
  Serial.print(F(", Count: b") );  Serial.print((pinIO_switchState[i] >>2) & 0b11, BIN);
  Serial.print(F(", 1/8s: b") );  Serial.print((pinIO_switchState[i])>>4, BIN);
  Serial.print(F(", switchState[i]: ") ); Serial.print(pinIO_switchState[i], BIN);
  Serial.print(F(", analogRead: ") ); Serial.println(analogRead(pinIO_pinsA_in[i]));
  
}

void getInputStates(){
  //This updates the 'current state' bit in switchState array of switches for each switch.
  //This will overwrite the value without updating any other bits, eg. count.
  byte i;
  // If any pins are A6 or A7 on arduino digital read and INPUT_PULLUP don't work.
#ifdef _debug_switches
  //#define _debug_switches_gis
#endif
//#define _debug_switches_gis
#ifdef _debug_switches_gis
  Serial.println(F("entering getInputStates()") );
#endif
  
  for(i=0; i<pinIO_Max_switches;i++){
    if( ReadSwitch(i) ) {
      pinIO_switchState[i] |= 0b10; 
      //newSwitchState |= (1 << i);
    } else {            //76543210
      pinIO_switchState[i] &= 0b11111101;//(~(0b10));
      //newSwitchState &= ~(1 << i); // bit clear;
    }
/*    
  * switchState[i]: 
  *  state          : 0b0000 0001  Fully updated state, including updated mqtt etc. 0 = off 1 = on
  *  current state  : 0b0000 0010  State at last check. Current switch state but count may still be updating etc. 0 = off 1 = on 
  *  change count   : 0b0000 1100  Number of changes within 0.1 and 2 seconds betwean changes. max changes 8, maybe 7. Use difference of bit 1 and 2 as first bit
  *  time         : 0b1111 0000 time in 1/8 seconds since last change. Within approx 1/8 second
*/
#ifdef _debug_switches_gis 
 debugSwitch(i);
#endif
  }
#ifdef _debug_switches_gis
  wdt_disable();
  delay(10000);
  wdt_enable(WDTO_8S);
#endif
}

#define s1Msk B00000001
#define s2Msk B00000010
#define s3Msk B00000100

/** Check for changes no more than 1/8 of a second, If sending mqtt or web page may be longer?. Should be good for debounce?
  * If same switch changes again in less than 2 seconds add one to switch state to max of 8. 
  * stateVar      : 0b0000 0000 
  *  state          : 0b0000 0001  Fully updated state, including updated mqtt etc. 0 = off 1 = on
  *  current state  : 0b0000 0010  State at last check. Current switch state but count may still be updating etc. 0 = off 1 = on 
  *  change count   : 0b0000 1100  Number of changes within 0.1 and 2 seconds betwean changes. max changes 8, maybe 7. Use difference of bit 1 and 2 as first bit
  *  time         : 0b1111 0000 time in 1/8 seconds since last change. Within approx 1/8 second
**/




void Switched(byte sw_i, byte count, byte state){// Count 0 is quick on. sw = 0 for first switch
// some of this should be moved to elseware this lib should just keep track of what switch was switched and 
// maybe call a function pointer to handel any switching of lights etc.
  // byte i_index, g_index, i, opt;
  // word groupmask;
  // i = switches_eeprom_start + (switches_size * sw_i) + count;
  // EEPROM.get(i, i_index);
  // g_index = groups_eeprom_start + (i_index*group_size);
  // EEPROM.get(g_index, groupmask);
  // EEPROM.get(g_index+2, opt);
  // //#ifdef _debug_switches
  // #ifdef _term_v
  // Serial.print(F("Switch index: ") ); Serial.print(sw_i);  Serial.print(F(", count: ") ); Serial.print(count);  Serial.print(F(", Group index: ") ); Serial.print(i_index);  Serial.print(F(", groupmask: b") ); Serial.print(groupmask, BIN);Serial.print(F(", "));
  // #endif
  // i=1;
  // while (groupmask > 0){
  //   if(groupmask & 0b1){
  //     //todo Need to add toggle relay etc. Only does hard set to switch pos at min.
  //     if(count == 1){
  //       UpdateRelayState(i, (groupsSateA[i_index+1] & 0b1) );
  //     }
  //     else if(count == 0){
  //       UpdateRelayState(i,1,false);
  //     }
  //     else {
  //       if(opt == 0){ UpdateRelayState(i,0);}//UpdateRelayState() first relay = 1;}
  //       else if(opt == 1){ UpdateRelayState(i,1);}//UpdateRelayState() first relay = 1;}
  //       else if(opt == 4){ UpdateRelayState(i,state);}//UpdateRelayState() first relay = 1;}
  //       else if(opt == 5){
  //         if(count == 2){UpdateRelayState(i,1);}//UpdateRelayState() first relay = 1;}
  //         if(count == 3){UpdateRelayState(i,0);}//UpdateRelayState() first relay = 1;}
  //       }
  //     }
  //   }
  //   i++;
  //   groupmask =  groupmask >> 1;
  // }
  //     if(count = 1){
  //       groupsSateA[i_index+1] = groupsSateA[i_index+1] xor 0b1; // i starts at 1
  //     }
  //     else if(count > 1){
  //       groupsSateA[i_index+1] = state & 0b1;//groupsSateA[0] is array length - 1(not counting [0])
  //     }
  
  // if (mqtt_client.connected()) {
  //   MqttPushSwitchState(i, state);
  // }
}

void SwitchesExe(){
  byte i, state1,state2, count1, time1;
#ifdef _debug_switches
  //Serial.println(F("entering SwitchesExe()") );
#endif
  if( (millis() - lastMils ) < (125) ) return ; // 1/8th of a second. If called just under 1/8 of a second and again 1/8 later.
  // Could be 1/4 second betwean switch checks even when called at close to every 1/8th second.
  getInputStates();//update the curent switch state bit. Just that no turning on lights etc.
  for (i=0;i<pinIO_Max_switches;i++){
    if(pinIO_switchState[i] == 0 or pinIO_switchState[i] == 0b11) { continue;}// If everything is up to date then skip to check next switch;
    state1 = pinIO_switchState[i] & 0b1;
    state2 = (pinIO_switchState[i] >> 1) & 0b1;
    count1 = (pinIO_switchState[i] >> 2) & 0b11; //bits B00001100
    time1 = pinIO_switchState[i] >> 4;
    if(state1 ==  state2){// work out if switch changed when state(see above) are the same
      if(count1 & 0b1){ // if count is an odd number and states are the same then changed.
        // count odd and state1 = state2, hence switch has changed position
        count1++;
        //Serial.print(F("Switch ") ); Serial.print(i); Serial.print(F(" changed. line:") ); Serial.println(__LINE__);
        time1 = 0;
      }
      else {// No change.
        // count even and state1 = state2, hence no change
        if(count1 > 0) {time1++; }//  Add to timeout
      }
    } 
    else {// States are different.
      if( (count1 & 0b1) == 0){// check if switch state changed since last call.
        // count is even and state1 != state2, hence changed
        count1++;// as changed add to changed count
        time1 = 0;//reset time since last change
        //Serial.print(F("Switch ") ); Serial.print(i); Serial.print(F(" changed. line:") ); Serial.println(__LINE__);
        if(count1 == 0){
  #ifdef _debug_switches
          Serial.print(F("Call quick light on func: ") );
  #endif
          Switched(i, 0, 1);// Switch relays etc.
        }
      }
      else {// if not change then add to time since last switch
        // count is odd and state1 != state2, hence not changed
        time1++;//Serial.print(time1,HEX);
        
      }//
        
    }
    if (count1 >= 4 || time1 >= 0b1111){// reched max change count or time since last change > 2 seconds
#ifdef _debug_switches
      Serial.print(F("Switch state update ") ); Serial.print(F(", count1 = ") ); Serial.print(count1);  Serial.print(F(", timeout in 1/8 seconds = ") ); Serial.print(time1);
      Serial.print(F(", switch No. = ") ); Serial.println(i);
#endif
      //count1 = (count1  >> 1) + 1; 
      Switched(i, count1, state2);// Switch relays etc.
      //and update switchState[i] to curent state
      if (state2 > 0) pinIO_switchState[i] = 0b11;
      else pinIO_switchState[i] = 0;
    } else {
      pinIO_switchState[i] &= 0b11;
      pinIO_switchState[i] |= (count1 << 2);
      pinIO_switchState[i] |= (time1 << 4);
      
    }
    
  }
  lastMils = millis();
}
