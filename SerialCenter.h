#ifndef SerialCenter_h
#define SerialCenter_h

#include "Arduino.h"
#include <stream.h>

class  SerialCenter
{
  private:
    Stream& port;
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
    SerialCenter(Stream& streamPort):port(streamPort){}// Constructor
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
