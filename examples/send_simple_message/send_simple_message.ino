#include <SerialCenter.h>

SerialCenter mySerial;

enum SerialResult
    {
     OK,
     ERROR,
     NO_RESPONSE,
    };

 
void setup() {
  Serial.begin(9600);

}

unsigned long lastMessageTime = 0;
void loop() {
  
  if(millis() - lastMessageTime >= 3000)
  {
    mySerial.sendMessage("HOLA");

    if(mySerial.tryGetACK(100) == OK)
    {
      lastMessageTime = millis();
    }
    //Si no es OK, se reenviará el mensaje en la próxima iteración
  }

}
