# Pasos para la configuración del proyecto:

### Materiales necesarios:

*Placa de desarrollo ESP32
*Sensor de temperatura DTH11
*Cables de alambre
*Protoboard
*LED y resistencia de 100 ohms (opcional)
*IDE para programar la tarjeta

### Proceso: 

1. El primer paso es descargar el IDE adecuado para nuestro proyecto, en este caso se utiliza el Arduino IDE
2. Se descargan las dependencias, drivers y plugins necesarios para poder utilizar la placa de desarrollo ESP32 en el IDE (librerías de Espressif Systems, driver para conexión UART a Windows)
3. Una vez que se han descargado todas las dependencias necesarias, se procede a hacer la conexión física de los elementos; utilizando la protoboard, se conecta la placa ESP32, seguido del sensor de temperatura y de los cables (en este caso se utilizaron los pines GND para la conexión a tierra, 3V3 para la alimentación del sensor, y el pin GPIO26 para activar el sensor)
4. Se coloca el [código en el archivo .ino](Codigo_Dispositivo_IoT/parcial_iot_principal.ino) en el IDE y se presiona "Upload", para que el código se guarde dentro de la tarjeta.
5. Entrar al canal de ThingSpeak [asociado para visualizar los resultados](https://thingspeak.com/channels/2452531)
