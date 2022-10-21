
#include "DHT.h"
#define DHTPIN 2 // what digital pin we're connected to
#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
void setup() {
   Serial.begin(9600);
   Serial.println("DHT22 Test!");
   dht.begin();
}
void loop() {
   delay(2000); // Wait a few seconds between measurements
   float h = dht.readHumidity();
   // Reading temperature or humidity takes about 250 milliseconds!
   float t = dht.readTemperature();
   // Read temperature as Celsius (the default)
   float f = dht.readTemperature(true);
   // Read temperature as Fahrenheit (isFahrenheit = true)
   // Check if any reads failed and exit early (to try again).
   if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
   }
   // Compute heat index in Fahrenheit (the default)
   float hif = dht.computeHeatIndex(f, h);
   // Compute heat index in Celsius (isFahreheit = false)
   float hic = dht.computeHeatIndex(t, h, false);
   Serial.println ("Humidity/Temp sensor data ");
   Serial.print ("  Humidity: ");
   Serial.print (h);
   Serial.println (" %");
   Serial.print ("  Temperature: ");
   Serial.print (t);
   Serial.print (" °C   ");
   Serial.print (f);
   Serial.println (" °F\t");
   Serial.print ("  Heat index: ");
   Serial.print (hic);
   Serial.print (" °C   ");
   Serial.print (hif);
   Serial.println (" °F");
}
