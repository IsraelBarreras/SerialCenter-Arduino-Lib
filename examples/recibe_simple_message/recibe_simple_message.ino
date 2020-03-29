#include <SerialCenter.h>
#include <SoftwareSerial.h>

SoftwareSerial softSerial(2,3); //Pin TX, PIN RX
SerialCenter  mySoftSerial(&softSerial);


void setup() {
  //Inicializamos los puertos seriales, Recibiremos los mensajes através del softwareSerial y lo imprimireemos en el puerto Serie
  Serial.begin(9600);
  softSerial.begin(9600);
}

void loop() {
  if(mySoftSerial.available())
  {
    byte arrayMessage[64]
    int messageLength = mySoftSerial.readMessage(arrayMessage);

    for(int i = 0; i < messageLength; i++)
    {
      Serial.print((char)arrayMessage[i]);
    }
    Serial.println();
  }
}
