# Proceso detallado:

1. Visitamos la página Silicon Labs y descargamos los controladores CP210x USB to UART Bridge VCP
2. Seleccionamos el puerto de serie adecuado
3. Una vez seleccionado el puerto y verificar la correcta instalación de los drivers, se procede a instalar la tarjeta correspondiente dentro del IDE de Arduino.
4. Entramos a la página de MathWorks
5. Procedemos a hacer la conexión de la placa (ESP32) junto con el sensor de temperatura (Utilizamos el DHT11) en nuestra protoboard, conectamos el PIN 26 de la ESP32 al sensor, así como la entrada de 3.3v y la salida GND (tierra).
6. Una vez conectados todos los componentes anteriormente mencionados, se ingresa a la página de ThingSpeak y se crea el canal correspondiente para el monitoreo, junto con las etiquetas “Temperatura” y “Humedad”.
7. Cuando se crea el canal, nos proporciona la llave de la API correspondiente para realizar la conexión al canal, la cuál se ingresa en el código.
8. Una vez corroborada la funcionalidad de la placa, se coloca el siguiente código

```cpp
#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define DHTPIN 26 // The pin you connected the DHT's data pin to
#define DHTTYPE DHT11 // DHT 11
#define LED 32

// Replace with your WiFi credentials
const char* ssid = "INFINITUM9C9C_2.4";
const char* password = "9XE5GZJ8Sw";

// ThingSpeak Settings
unsigned long myChannelNumber = 2452531;
const char * myWriteAPIKey = "4G8EB5IOSQZCKCE4";

WiFiClient  client;
DHT dht(DHTPIN, DHTTYPE);

unsigned long startTime = millis();
unsigned long ledLastToggleTime = millis();
unsigned long period = 300000; // 5 minutes in milliseconds
unsigned long ledInterval = 5000; // 5 seconds in milliseconds
bool ledState = false;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  ThingSpeak.begin(client);  // Initialize ThingSpeak
  dht.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  // Correctly blink LED every 5 seconds
  if (currentMillis - ledLastToggleTime >= ledInterval) {
    // Toggle the LED state
    ledState = !ledState;
    digitalWrite(LED, ledState ? HIGH : LOW);
    ledLastToggleTime = currentMillis;
  }

  // Check if 5 minutes have passed to read temperature and humidity
  if (currentMillis - startTime >= period) {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.print(" %\t");
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" *C ");

      ThingSpeak.setField(1, temperature);
      ThingSpeak.setField(2, humidity);
      ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    }

    startTime = currentMillis; // Reset the timer for the next read
  }
}



