#include <SerialCenter.h>
#include <SoftwareSerial.h>

SoftwareSerial softSerial(2,3);
SerialCenter  mySoftSerial(&softSerial);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  softSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(mySoftSerial.available())
  {
    String newMessage = mySoftSerial.readNextMessage();
    Serial.println(newMessage);
  }
}
