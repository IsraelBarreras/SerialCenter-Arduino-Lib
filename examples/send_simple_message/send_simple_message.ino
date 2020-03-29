#include <SerialCenter.h>

SerialCenter mySerial;
 
void setup() {
  Serial.begin(9600);

}

unsigned long lastMessageTime = 0;
void loop() {
  
  if(millis() - lastMessageTime >= 3000)
  {
    int arrayLength = 16;
    byte arrayMessage[arrayLength] = {116, 101, 99, 110, 111, 105, 110, 103, 101, 110, 105, 97, 46, 99, 111, 109};
    int intentos = 3;
    unsigned long timeOut = 1000;
    mySerial.sendMessage(arrayMessage, arrayLength, intentos, timeOut);
  }

}
