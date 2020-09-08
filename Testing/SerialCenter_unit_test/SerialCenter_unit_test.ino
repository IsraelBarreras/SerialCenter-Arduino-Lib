#include <ArduinoUnit.h>
#include "SerialCenter.h"
#include "StreamMOCK.h"

const char STX = '\x002';
const char ETX = '\x003';
const char ACK = '\x006';
const char NAK = '\x015';

enum SerialResult
    {
     OK,
     ERROR,
     NO_RESPONSE,
    };

StreamMOCK mockSerial;
SerialCenter serialPort(mockSerial);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Test::run();
}

test(sendMessage)
{
    mockSerial.clean();
    assertEqual(0, mockSerial.writeCalls());
    byte array[5];
    array[0] = 'a';
    array[1] = 'b';
    array[2] = 'c';
    serialPort.sendMessage(array, 3, 4, 200);

    assertEqual(STX, mockSerial.writeBuffer[0]);
    assertEqual('a', mockSerial.writeBuffer[1]);
    assertEqual('b', mockSerial.writeBuffer[2]);
    assertEqual('c', mockSerial.writeBuffer[3]);
    assertEqual(ETX, mockSerial.writeBuffer[5]);
    assertEqual(30, mockSerial.writeCalls());
}

test(sendMessageAsString)
{
    mockSerial.clean();
    String probeString = "1234";
    serialPort.sendMessageAsString(probeString);
 

    assertEqual(STX, mockSerial.writeBuffer[0]);
    assertEqual('1', mockSerial.writeBuffer[1]);
    assertEqual('2', mockSerial.writeBuffer[2]);
    assertEqual('3', mockSerial.writeBuffer[3]);
    assertEqual('4', mockSerial.writeBuffer[4]);
    assertEqual(ETX, mockSerial.writeBuffer[6]);
    assertEqual(7, mockSerial.writeCalls());

    
}

test(ChecksumBEE)
{
    byte array[4] = {10, 15, 20, 30};
    byte calculatedChecksum = serialPort.ChecksumBEE(array, 4);
    assertEqual(75, calculatedChecksum);

    byte array2[4] = {200, 200, 200, 200};
    calculatedChecksum = serialPort.ChecksumBEE(array2, 4);
    assertEqual(32, calculatedChecksum);
}

test(tryGetACK)
{
    mockSerial.clean();
    mockSerial.readBuffer[0] = ACK;
    mockSerial.availableBytes = 1;
    assertEqual(OK, serialPort.tryGetACK(200));

    mockSerial.clean();
    mockSerial.readBuffer[0] = NAK;
    mockSerial.availableBytes = 1;
    assertEqual(ERROR, serialPort.tryGetACK(200));

    mockSerial.clean();
    mockSerial.availableBytes = 0;
    assertEqual(NO_RESPONSE, serialPort.tryGetACK(200));
}

test(readNextMessage)
{
   /* Caso de lectura correcta */
   mockSerial.clean();
   mockSerial.readBuffer[0] = STX;
   mockSerial.readBuffer[1] = 'H';
   mockSerial.readBuffer[2] = 'O';
   mockSerial.readBuffer[3] = 'L';
   mockSerial.readBuffer[4] = 'A';
   mockSerial.readBuffer[5] = 219;
   mockSerial.readBuffer[6] = ETX;
   mockSerial.availableBytes = 7;
   
   byte array[64];
   assertEqual(4, serialPort.readNextMessage(array));
   assertEqual('H', array[0]);
   assertEqual('O', array[1]);
   assertEqual('L', array[2]);
   assertEqual('A', array[3]);
   assertEqual(ACK, mockSerial.writeBuffer[0]);

   /* Caso checksum Incorrecto */
   mockSerial.clean();
   mockSerial.readBuffer[0] = STX;
   mockSerial.readBuffer[1] = 'H';
   mockSerial.readBuffer[2] = 'O';
   mockSerial.readBuffer[3] = 'L';
   mockSerial.readBuffer[4] = 'A';
   mockSerial.readBuffer[5] = 255;
   mockSerial.readBuffer[6] = ETX;
   mockSerial.availableBytes = 7;
   
   assertEqual(6, serialPort.readNextMessage(array));
   assertEqual(0, array[0]);
   assertEqual(NAK, mockSerial.writeBuffer[0]);

   /* Caso cabecera y fin de mensaje Incorrectos */
   mockSerial.clean();
   mockSerial.readBuffer[0] = ETX;
   mockSerial.readBuffer[1] = 'H';
   mockSerial.readBuffer[2] = 'O';
   mockSerial.readBuffer[3] = 'L';
   mockSerial.readBuffer[4] = 'A';
   mockSerial.readBuffer[5] = 219;
   mockSerial.readBuffer[6] = ETX;
   mockSerial.availableBytes = 7;
   
   assertEqual(0, serialPort.readNextMessage(array));
   assertEqual(0, array[0]);
   assertEqual(0, mockSerial.availableBytes);

}

test(available)
{
    mockSerial.clean();

    serialPort.available();
    assertEqual(1,  mockSerial.availableCalls());
}

test(flush)
{
    mockSerial.clean();

    serialPort.flush();
    assertEqual(1,  mockSerial.flushCalls());
}


test(read)
{
    mockSerial.clean();

    serialPort.read();
    assertEqual(1,  mockSerial.readCalls());
}
