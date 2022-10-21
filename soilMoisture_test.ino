/* 
 Soil Moisture Sensor   
*/
#define SensorPin A0 
float soilMoisture = analogRead(A0); 
const int dry = 700; // value for dry sensor
const int wet = 300; // value for wet sensor

void setup() { 
 Serial.begin(9600); 
} 

void loop() { 
 for (int i = 0; i < 100; i++) 
 { 
   soilMoisture = soilMoisture + analogRead(SensorPin); 
   delay(1); 
 } 
 soilMoisture = soilMoisture/100.0; 
 Serial.print("Soil Moisture sensor data: "); 
 Serial.print(soilMoisture);
 Serial.println(" g/m³"); 
 //dewpoint (°F or °C), grams of water per cubic meter (g/m³)
 int percentageSoilHumidity = map(soilMoisture, wet,dry, 100, 0);
 Serial.print("Soil Moisture percentage: "); 
 Serial.print(percentageSoilHumidity);
 Serial.println(" %");
 delay(3000); 
} 
