#include "Arduino.h"
#include "SerialCenter.h"
#include <SoftwareSerial.h>

  SerialCenter::SerialCenter()
  {
   port = false;
   
  }

  SerialCenter::SerialCenter(SoftwareSerial *softPort)
  {
   port = true;
   softwarePort = softPort;
  }

  void SerialCenter::sendMessage(String message)
 {
  int arrayLength = message.length()+1;
  byte arrayPointer[arrayLength];
  message.toCharArray(arrayPointer, arrayLength);
  if(!port)
  {
   Serial.write(STX);
   Serial.write(arrayPointer, arrayLength);
   uint16_t checksum = this->ChecksumFletcher16(arrayPointer, arrayLength);
   Serial.write((byte)checksum);
   Serial.write((byte)checksum >> 8);
   Serial.write(ETX);
  }
  else
  {
   softwarePort->write(STX);
   softwarePort->write(arrayPointer, arrayLength);
   uint16_t checksum = this->ChecksumFletcher16(arrayPointer, arrayLength);
   softwarePort->write((byte)checksum);
   byte checksumbyte = checksum >> 8;
   softwarePort->write(checksumbyte);
   softwarePort->write(ETX);
  }
 }

  uint16_t SerialCenter::ChecksumFletcher16(const byte *data, int dataLength)
 {
  uint8_t sum1 = 0;
  uint8_t sum2 = 0;
 
  for (int index = 0; index < dataLength; ++index)
  {
    sum1 = sum1 + data[index];
    sum2 = sum2 + sum1;
  }
  
  return (sum2 << 8) | sum1;
 }

   int SerialCenter::tryGetACK(int timeOut)
  {
   if(!port)
   {
    unsigned long startTime = millis();
    while (!Serial.available() && (millis() - startTime) < timeOut)
    {
    }
  
    if (Serial.available())
    {
      if(Serial.read() == ACK) {return OK;}
      else {return ERROR;}
    }
    return NO_RESPONSE;
   }
   else
   {
    unsigned long startTime = millis();
    while (!softwarePort->available() && (millis() - startTime) < timeOut)
    {
    }
  
    if (softwarePort->available())
    {
      if (softwarePort->read() == ACK) {return OK;}
      else{return ERROR;}
    }
    return NO_RESPONSE;
   }
  }

  String SerialCenter::readNextMessage()
 {
  if(Serial.read() == STX && !port || softwarePort->read() == STX && port)
  {
    int i = 0;
    byte data[MAX_STRING_BYTES];
    //int capacidad = 8;
    do
    {
     /*if(i > capacidad * 2)
     {
      capacidad = capacidad * 2;
      byte* newData = new byte[capacidad];
      memmove(newData, data, (i - 1) * sizeof(byte));
      delete[] data;
      data = newData;
     }*/
     delay(2);
     if(!port)
     {
      if(!Serial.available()){i++; break;}
      data[i] = Serial.read();
     }
     else
     {
      if(!softwarePort->available()){i++; break;}
      data[i] = softwarePort->read();
     }
     i++;
    }while(data[i-1] != ETX);
    int dataLength = i - 3;
    if(data[i-1] == ETX && data[i-3] | data[i-2] << 8 == ChecksumFletcher16(data, dataLength))
    {
     String message = "";
     if(!port)
     {
      Serial.print(ACK);
     }
     else
     {
      softwarePort->print(ACK);
     }
     for(i = 0; i < dataLength; i++)
     {
      message += (char)data[i];
     }
     return message; 
    }
    else
    {
      if(!port)
     {
      Serial.print(NAK);
     }
     else
     {
      softwarePort->print(NAK);
     }
      return F("ERROR");
    }
    
  }
 }

  int SerialCenter::available()
  {
   if(!port)
   {
     return Serial.available();
   }
   else
   {
     return softwarePort->available();
   }
  } 

  void SerialCenter::setMaxString(int MAX)
  {
    MAX_STRING_BYTES = MAX;
  }
