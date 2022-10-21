/************************ SOIL MOISTURE SENSOR *********************/
  #define sensorPin A0
  int soil = analogRead(A0); 
  const int dry = 678; // value for dry sensor
  const int wet = 235; // value for wet sensor
  
/************************ HUMIDITY/TEMP SENSOR ***************/
 #include "DHT.h"
 #define DHTPIN 2 
 #define DHTTYPE DHT22
 DHT dht(DHTPIN, DHTTYPE);

/************************ LIGHT SENSOR ***************/
  #define sensorPin A1
  int light = analogRead(A1);
  
/************************ LCD *********************************/
  #include <LiquidCrystal.h>
  LiquidCrystal lcd(12, 11, 6, 5, 4, 3);

/************************ BUTTON SOLENOID CONTROL ON/OFF *********************************/
  int switch_pin = 10;
  //int relay = 7;

//--------------------------------------------------------------------------//
// SETUP      SETUP     SETUP     SETUP     SETUP     SETUP     SETUP       //
//--------------------------------------------------------------------------// 
void setup()
{
/************************ SOIL MOISTURE SENSOR ***************/
  Serial.begin(9600);   

/************************ HUMIDITY/TEMP SENSOR ***************/
  dht.begin();
  
/********************8***** LIGHT SENSOR **********************/  
 //Serial.begin(9600);
  
/************************ LCD *********************************/  
  //Serial.begin(9600);     //  opens serial port, sets data rate to 9600 bps
  lcd.begin(16, 2);       //  set up the 16/2 LCD's number of columns and rows:
  
/************************ BUTTON SOLENOID CONTROL ON/OFF *********************************/
  pinMode(switch_pin, INPUT);
  pinMode(7, OUTPUT); 
}

//--------------------------------------------------------------------------//
// LOOP     LOOP      LOOP      LOOP      LOOP      LOOP      LOOP          //
//--------------------------------------------------------------------------// 
void loop()
{
/************************ SOIL MOISTURE SENSOR *********************/
 int soil = analogRead(A0); 
 int percentageSoilHumidity = map(soil, wet, dry, 100, 0);
 Serial.println("Moisture sensor data "); 
 Serial.print("   Soil Moisture: "); 
 Serial.println(soil); 
 Serial.print("   Soil Moisture percentage: "); 
 Serial.print(percentageSoilHumidity);
 Serial.println(" %");

/************************ HUMIDITY/TEMP SENSOR *********************/
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
   Serial.println (h);
   //Serial.print (" %\t");
   Serial.print ("  Temperature: ");
   Serial.print (t);
   Serial.print (" *C   ");
   Serial.print (f);
   Serial.println (" *F\t");
   Serial.print ("  Heat index: ");
   Serial.print (hic);
   Serial.print (" *C   ");
   Serial.print (hif);
   Serial.println (" *F");


/*********************** LIGHT SENSOR **************************************/ 
  
 // reads the input on analog pin A1 (value between 0 and 1023)
   int light = analogRead(A1);
  Serial.println("Ligth sensor data ");
  Serial.print("  Ligth sensor: ");
  Serial.print(light);   // the raw analog reading
  
  // We'll have a few threshholds, qualitatively determined
  if (light < 10) {
    Serial.println(" - Dark");
  } else if (light < 200) {
    Serial.println(" - Dim");
  } else if (light < 500) {
    Serial.println(" - Light");
  } else if (light < 800) {
    Serial.println(" - Bright");
  } else {
    Serial.println(" - Very bright");
  }
  delay(500);
  

/************************ LCD *********************************/  
//Conversion formula

//   int analog_value = analogRead(A0);
//    temp = (analog_value * 5.0) / 1024.0; //[(Vref *analog V/)(Resolution 2^10)]
//   input_voltage = temp / (r2/(r1+r2));
//   
//   if (input_voltage < 0.1)
//   {
//     input_voltage=0.0;
//   }
//    Serial.print("v= ");
//    Serial.println(input_voltage);


    lcd.print("SMART IRR SYSTEM");
    lcd.setCursor(0, 1);
    lcd.print("Soil = ");
    //lcd.print(soil);
    if(soil < 300)
      lcd.print("High");
    else
      lcd.print("Low");  
    delay(5000);
    lcd.clear();

    lcd.print("SMART IRR SYSTEM");
    lcd.setCursor(0, 1);
    lcd.print("Humidity = ");
    lcd.print(h);
    delay(5000);
    lcd.clear();

    lcd.print("SMART IRR SYSTEM");
    lcd.setCursor(0, 1);
    lcd.print("Temp C = ");
    lcd.print(t);
    delay(5000);
    lcd.clear();

    lcd.print("SMART IRR SYSTEM");
    lcd.setCursor(0, 1);
    lcd.print("Temp F = ");
    lcd.print(f);
    delay(5000);
    lcd.clear();

    lcd.print("SMART IRR SYSTEM");
    lcd.setCursor(0, 1);
    lcd.print("Light - ");
   // lcd.print(light);
  if (light < 10) {
    lcd.print("Dark");
  } else if (light < 200) {
    lcd.print("Dim");
  } else if (light < 500) {
    lcd.print("Moderate");
  } else if (light < 800) {
    lcd.print("Bright");
  } else {
    lcd.print("Very bright");
  }
    delay(5000);
    lcd.clear();
/************************ BUTTON SOLENOID CONTROL ON/OFF *********************************/
    if(digitalRead(switch_pin) == HIGH){
      digitalWrite(7, HIGH);
    }
    if(digitalRead(switch_pin) == LOW){
      digitalWrite(7, LOW);
    }
//    
//   digitalWrite(7,HIGH);
//   delay(3000);
//   digitalWrite(7,LOW);
//   delay(5000);
   
}
