# SerialCenter
Librería dedicada a tener una comunicación por puerto serie efectiva, la comunicación está basada en el el envío y recepción de bytes por mensaje. Aplicando un Caracter de inicio de trama (**STX**), carga útil, un **checksum** básico, un final de trama (**ETX**), mensaje de comprobación automático (**ACK**) y número editable de reintentos. Soporta **softwareSerial**.



## ¿Cómo usar?
**Para declarar un objeto de la clase serialCenter:**

```C++
serialCenter serCenter; //Para Usar en el puerto del hardware serial.
```
----------------------------------------------------------------------------------
```C++
SoftwareSerial	softPort(2,3); //Pin TX, Pin RX
serialCenter	softSerialCenter(&softPort); //Para usar un puerto de SoftwareSerial
```

**Para enviar un mensaje:**

	int intentos = 3; número máximo de intentos de reenvío de mensajes
	unsigned long timeOut = 1000; //En milisegundos, tiempo de espera máximo por intento
	int arrayLength = 7;
	byte arrayPointer[arrayLength] = {0, 1, 86, 254, 41, 32, 88}
	serCenter.senMessage(arrayPointer, arrayLength, intentos, timeOut);

**Para recibir un nuevo mensaje:**

```C++
if(Serial.available())
{
	byte arrayMessage[64]; //Aquí se guardarán los datos del mensaje. Puedes definir el tamño máximo del mensaje.
	int messageLength = serCenter.readNextMessage(arrayMessage); //Regresa el tamaño en bytes del mensaje
}
```
