#include "Arduino.h"
#include "SerialCenter.h"
#include <SoftwareSerial.h>


//#define serialDev

 /*****************************************************************
 * Función: sendMessage
 * Descripción: Envía un buffer de bytes por el puerto serie asignado en formato serialCenter,
 * recibe como parámetros el array de bytes a enviar (arrayPointer), El tamaño del array (arrayLength),
 * la cantidad de reintentos maximos en caso de error (intentos_maximos) y el 
 * tiempo maximo de espera (timeOut) en milisegundos
 *****************************************************************/
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
   port.write(STX);
   port.write(arrayPointer, arrayLength);
   port.write(255 - this->ChecksumBEE(arrayPointer, arrayLength));
   port.write(ETX);
   
   if(intentos >= intentos_maximos)
   {
    return false; 
   }
   intentos++;
  }while(this->tryGetACK(timeOut) != OK);
  return true;
 }


/*****************************************************************
 * Función: sendMessageAsString
 * Descripción: Envía un string por el puerto serie asignado en formato SerialCenter
 *****************************************************************/
 void SerialCenter::sendMessageAsString(String message)
 {
  int arrayLength = message.length();
  byte arrayPointer[arrayLength + 1];
  message.toCharArray(arrayPointer, arrayLength + 1);
 
   port.write(STX);
   port.write(arrayPointer, arrayLength);
   port.write(255 - this->ChecksumBEE(arrayPointer, arrayLength - 1));
   port.write(ETX);
   
 }

/*****************************************************************
 * Función: ChecksumBEE
 * Descripción: Calcúla un checksum simple de un array de bytes (data), 
 * y regresa el byte menos significativo
 *****************************************************************/
byte SerialCenter::ChecksumBEE(const byte *data, int dataLength)
{
  uint16_t sum = 0;
  for (int index = 0; index < dataLength; index++)
  {
    sum += (byte)data[index];
  }
  return ((sum & 255));
  
}


/*****************************************************************
 * Función: tryGetACK
 * Descripción: Espera a recibir una confirmación de de mensaje (ACK) 
 * por el puerto asignado, regrsa OK o ERROR o NO_RESPONSE si el timeOut es superado
 *****************************************************************/
int SerialCenter::tryGetACK(int timeOut)
{
 unsigned long startTime = millis();
 while ((millis() - startTime) < timeOut)
 {
  if (port.available())
  {
   if (port.read() == ACK) 
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

  
/*****************************************************************
 * Función: readNextMessage
 * Descripción: Revisa si hay nuevos mensajes en el puerto serie asignado,
 * al tener datos disponibles revisa si el mensaje viene con el formato serialCenter
 * y calcúla el checksum para compararlo con el mensaje, si esto se cumple envía ACK
 * como respuesta através del puerto serie y retorna el numero de bytes recibidos.
 * En caso contrario envía NACK por el puerto serie asignado y setea en primer byte
 * del array (data) en "0" (este mecanismo permite identificar errores en la recepción).
 * 
 * Este método debe ejecutarse al detectar bytes disponibles en el puerto con el método SerialCenter.available()
******************************************************************/
int SerialCenter::readNextMessage(byte *data)
{
 byte head;
 int dataLength = 0;
 
 head = port.read();
 
 if(head == STX)
 {  
   int i = 0;
    delay(2);
    while(port.available())
    {
      delay(2);
      data[i] = port.read();
      i++;
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
    port.print(ACK);       
    return dataLength-2; 
   }
   else
   {
#ifdef serialDev 
     Serial.println("error");
#endif
     data[0] = 0;
     port.print(NAK);
     return dataLength;
   }
   
 }
 
 while(port.available()) 
 {
     char c = port.read();
 }
 //descartar buffer de lectura

 data[0] = 0;
 return dataLength;
}


/*****************************************************************
 * Función: available
 * Descripción: Retorna el número de bytes disponibles para leer en
 * el puerto serie asignado. Si > 0 quiere decir que hay un nuevo mensaje
 * Este método se debe ejecutar constantemente en el loop() de nuestro código.
******************************************************************/
int SerialCenter::available()
{
  return port.available();
} 


/*****************************************************************
 * Función: flush
 * Descripción: ejecuta Serial.flush()
******************************************************************/
void SerialCenter::flush()
{
  port.flush();
}


/*****************************************************************
 * Función: read
 * Descripción: lee un byte del buffer serial asignado
******************************************************************/
char SerialCenter::read()
{
  return port.read();
}
