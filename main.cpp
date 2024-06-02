#include "DHT.h"
#include <WiFiNINA.h>
#include "ThingSpeak.h"

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials
char ssid[] = "WiFi SSID";  // Ensure the SSID is in quotes
char pass[] = "WiFi Password";    // Ensure the password is in quotes
WiFiClient client;

unsigned long myChannelNumber = 2566944;
const char * myWriteAPIKey = "GF6CYTGJPVUBMB0R";  // Ensure the API key is in quotes

void setup() {
  Serial.begin(9600);
  dht.begin();
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  ThingSpeak.begin(client);
}

void loop() {
  delay(60000);  // Wait 60 seconds between updates
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");

  // Update ThingSpeak
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
}
