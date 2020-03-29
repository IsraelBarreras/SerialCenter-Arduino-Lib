#ifndef serialCenter_h
#define serialCenter_h

#include "Arduino.h"
#include <SoftwareSerial.h>

class serialCenter
{
  private:
    boolean port;
    SoftwareSerial *softwarePort;
    const char STX = '\x002';
    const char ETX = '\x003';
    const char ACK = '\x006';
    const char NAK = '\x015';
    const int TimeOut = 100;
    
    enum SerialResult
    {
     OK,
     ERROR,
     NO_RESPONSE,
    };
    
  public:
    serialCenter(); // Constructor 1 para hardware serial
    serialCenter(SoftwareSerial *softPort);// Constructor para software serial
    int readNextMessage(byte *data);//Lee caracteres desde STX hasta ETX y revisa el checksum
    int available();//Bytes disponibles en el puerto
    boolean sendMessage(byte *arrayPointer, int arrayLength, int intentos_maximos, unsigned long timeOut); //Envía un array de caracteres entre STX y ETX
    void sendMessageAsString(String message);
    byte ChecksumBEE(const byte *data, int dataLength);
    int tryGetACK(int timeOut);
    void flush();
    char read();
    
};

#endif
