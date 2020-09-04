#include <ArduinoUnit.h>
#include "SerialCenter.h"
SerialCenter serialPort(Serial);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Test::run();
}

test(read)
{
    byte array[5];
    array[0] = 'a';
    array[1] = 'b';
    array[2] = 'c';
    serialPort.sendMessage(array, 3, 3, 200);
}