# pinIO

IO.h = Low level IO to things like i2c expanders, the arduino's pins & shift registers etc.
       there should be 1 function each for digital read, write & mode that calls pinMode(), digitalWrite etc. or the 
       appropriate functions for i2c etc that works with all the IO based on a byte pin value
       uses only arduino libs.

O.h = Higher level  output. e.g. output: relays for lights
       1: Keeps track of relay state and how to turn a relay on/off etc.
       2: Sends an mqtt message if a relay is updated
       uses: mqtt

I.h and O.h = Higher level input and output. e.g. input: switches, e.g. output: relays for lights
       1: Keeps track of current state. On, off etc.
       2: make sure any change in state is not checked again for a bit to debounce switches etc.
       3: keep track of quick switches, slower than debounce but faster than a second or 2 for extra functionality. 
          Turning all light on for example.
       4: handle keeping track of what the switch will do when switched.
       5: group handling.

## refactoring
1. IO.h will be split into a different library for each type. i2c expanders arduino's pins & shift registers etc.
2. I.h and O.h will still stay more or less the same but should allow different low level IO libs to be added. And if for example no wire.h is needed not require it.
3. 