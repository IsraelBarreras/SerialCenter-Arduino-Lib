#include "Arduino.h"
#include "SerialCenter.h"
#include <SoftwareSerial.h>
//#define serialDev

  SerialCenter::SerialCenter()
  {
   port = false;
  }

  SerialCenter::SerialCenter(SoftwareSerial *softPort)
  {
   port = true;
   softwarePort = softPort;
  }

 boolean SerialCenter::sendMessage(byte *arrayPointer, int arrayLength, int intentos_maximos, unsigned long timeOut)
 {
  byte intentos = 0;
#ifdef serialDev
  Serial.print("Enviando: ");
  for(int i = 0; i < arrayLength; i++)
  {
    Serial.print(arrayPointer[i]);
    Serial.print("-");
  }
  Serial.println();
#endif
  do
  {
   if(!port)
   {
    Serial.write(STX);
    Serial.write(arrayPointer, arrayLength);
    Serial.write(255 - this->ChecksumBEE(arrayPointer, arrayLength));
    Serial.write(ETX);
   }
   else
   {
    softwarePort->write(STX);
    softwarePort->write(arrayPointer, arrayLength);
    softwarePort->write(255 - this->ChecksumBEE(arrayPointer, arrayLength));
    softwarePort->write(ETX);
   }
   
   if(intentos >= intentos_maximos)
   {
    return false; 
   }
   intentos++;
  }while(this->tryGetACK(timeOut) != OK);
  return true;
 }

 void SerialCenter::sendMessageAsString(String message)
 {
  int arrayLength = message.length()+1;
  byte arrayPointer[arrayLength];
  message.toCharArray(arrayPointer, arrayLength);
  if(!port)
  {
   Serial.write(STX);
   Serial.write(arrayPointer, arrayLength);
   Serial.write(255 - this->ChecksumBEE(arrayPointer, arrayLength));
   Serial.write(ETX);
  }
  else
  {
   softwarePort->write(STX);
   softwarePort->write(arrayPointer, arrayLength);
   softwarePort->write(255 - this->ChecksumBEE(arrayPointer, arrayLength));
   softwarePort->write(ETX);
  }
 }

  byte SerialCenter::ChecksumBEE(const byte *data, int dataLength)
 {
  uint16_t sum = 0;
  for (int index = 0; index < dataLength; index++)
  {
    sum += (byte)data[index];
  }
  return ((sum & 255));
 }

   int SerialCenter::tryGetACK(int timeOut)
   {
   if(!port)
   {
    unsigned long startTime = millis();
    while ((millis() - startTime) < timeOut)
    {
     if (Serial.available())
     {
      if(Serial.read() == ACK) 
      {
        return OK;
      }
      else 
      {
        return ERROR;
      }
     }
    }
    return NO_RESPONSE;
   }
   else
   {
    unsigned long startTime = millis();
    while ((millis() - startTime) < timeOut)
    {
     if (softwarePort->available())
     {
      if (softwarePort->read() == ACK) 
      {
        return OK;
      }
      else
      {
        return ERROR;
      }
     }
    }
    return NO_RESPONSE;
   }
   return NO_RESPONSE;
  }

  

 int SerialCenter::readNextMessage(byte *data)
 {
  byte head;
  int dataLength = 0;
  
  if(!port)
  {
    head = Serial.read();
  }
  else
  {
    head = softwarePort->read();
  }
  
  if(head == STX)
  {  
    int i = 0;
     delay(2);
     if(!port)
     {
      while(Serial.available())
      {
        delay(2);
        data[i] = Serial.read();
        i++;
      }
     }
     else
     {
      while(softwarePort->available())
      {
        delay(2);
        data[i] = softwarePort->read();
        i++;
      }
     }

    dataLength = i;
#ifdef serialDev     
    for(int i = 0; i < dataLength; i++)
    {
      Serial.println(data[i]);
    }
    
    Serial.println("Evaluando");
    Serial.print("Array size: ");
    Serial.println(i);
    Serial.print("Fin de linea: ");
    Serial.println(data[dataLength-1]);
    Serial.print("Checksum: ");
    Serial.println(data[dataLength-2]);
#endif
    if(data[dataLength-1] == ETX && data[dataLength - 2] + ChecksumBEE(data, dataLength-2) == 255)
    {
#ifdef serialDev 
     Serial.println("OK");
#endif
     String message = "";
     if(!port)
     {
      Serial.print(ACK);
     }
     else
     {
      softwarePort->print(ACK);
     }
     
     return dataLength-2; 
    }
    else
    {
#ifdef serialDev 
      Serial.println("error");
#endif
      data[0] = 0;
      if(!port)
     {
      Serial.print(NAK);
     }
     else
     {
      softwarePort->print(NAK);
     }
      return dataLength;
    }
    
  }
  
  if(!port)
  {
   String desc = Serial.readString();
  }
  else
  {
   String desc = softwarePort->readString();
  }
  data[0] = 0;
  return dataLength;
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
   return 0;
  } 

  void SerialCenter::flush()
  {
   if(!port)
   {
    Serial.flush();
   }
   else
   {
    softwarePort->flush();
   }
  }

  char SerialCenter::read()
  {
   if(!port)
   {
    return Serial.read();
   }
   else
   {
    return softwarePort->read();
   }
   return NULL;
  }
