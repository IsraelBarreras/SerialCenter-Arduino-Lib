#ifndef SerialCenter_h
#define SerialCenter_h

#include "Arduino.h"
#include <SoftwareSerial.h>

class SerialCenter
{
  private:
    boolean port;
    SoftwareSerial *softwarePort;
    const char STX = '\x002';
    const char ETX = '\x003';
    const char ACK = '\x006';
    const char NAK = '\x015';
    const int TimeOut = 100;
    int MAX_STRING_BYTES = 64;
    enum SerialResult
    {
     OK,
     ERROR,
     NO_RESPONSE,
    };
    
  public:
    SerialCenter(); // Constructor 1 para hardware serial
    SerialCenter(SoftwareSerial *softPort);// Constructor para software serial
    String readNextMessage();//Lee caracteres desde STX hasta ETX y revisa el checksum
    int available();//Bytes disponibles en el puerto
    void sendMessage(String message); //Envía un array de caracteres entre STX y ETX
    uint16_t ChecksumFletcher16(const byte *data, int dataLength);
    int tryGetACK(int timeOut);
    void setMaxString(int MAX);
    
};

#endif
