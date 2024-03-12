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
