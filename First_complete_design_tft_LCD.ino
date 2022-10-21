/********************************************************************************************************/
/************************ SMART IRRIGATION SYSTEM WITH INTEGRATED AI FUNCTIONALITY *********************/
/******************************************************************************************************/
#include <SPI.h>
#include <SD.h>
#include <DHT.h>
#include <stdint.h>
#include <Wire.h>
#include "TouchScreen.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "Adafruit_STMPE610.h"

// Variable where all sensor data will be stored
File myFile;
File myFile_values_moisture;
File myFile_values_temperature;
File myFile_values_light;
File myFile_values_humidity;

/************************ LIGHT SENSOR *********************/
#define SensorPin A1
float lightValue = analogRead(A1);

/************************ SOIL MOISTURE SENSOR *********************/
#define SensorPin A0 
float soilMoisture = analogRead(A0); 
const int dry = 700; // value for dry sensor
const int wet = 300; // value for wet sensor

/************************ SOIL HUMIDITY/TEMP SENSOR ***************/
#define DHTPIN 2 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

/*********************** TFT-LCD **************************************/
#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

/*************************** TOUCH PEN ******************************/
//#include <XPT2046_Touchscreen.h>
////#include <SPI.h>
//
//#define CS_PIN  7
//// MOSI=11, MISO=12, SCK=13
//#define TIRQ_PIN  2 // not necessary for now, maybe if updates are included
//XPT2046_Touchscreen ts(CS_PIN);  // Param 2 - NULL - No interrupts

/*********************** BUTTONS *************************************/
#define YP A2  // must be an analog pin
#define XM A3  // must be an analog pin
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin
#define PRESSURE_THRESHOLD 10
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); // 300 = resistance between X+ and X- pins

int colors[3] = {ILI9341_DARKCYAN, ILI9341_ORANGE, ILI9341_OLIVE};
int font_colors[3] = {ILI9341_WHITE, ILI9341_BLACK, ILI9341_WHITE};
int active_colors = 0;

unsigned b1[4] = {15, 175, 135, 50};
unsigned b2[4] = {170, 175, 135, 50};

bool screenRedrawNecessary = false;

// Function prototypes
void setup();
void loop(void);
bool touchedWithin(TSPoint touch, unsigned x, unsigned y, unsigned w, unsigned h);
void resetAndClearScreen(void);
void drawIntroText(void);
void drawTouchButton(unsigned x, unsigned y, unsigned width, unsigned height, String text);

/*********************** SD CARD *************************************/
const int sd = 7; // connected to digital pin cs 7

//--------------------------------------------------------------------------//
// SETUP      SETUP     SETUP     SETUP     SETUP     SETUP     SETUP       //
//--------------------------------------------------------------------------// 
void setup() {  

/************************ SOIL MOISTURE SENSOR *********************/
  Serial.begin(9600);  
  Serial.println("Soil sensor Test Done!"); 

/************************ HUMIDITY/TEMP SENSOR ***************/
  dht.begin();
  Serial.println("DHT22 (Temp/Hum) Test Done!");
  
/********************8***** LIGHT SENSOR *************************/  
  Serial.begin(9600);
  Serial.println("Light sensor Test Done!"); 

/***************************** SOLENOID **************************/
  pinMode(5, OUTPUT);  // added test case to check operation at the very bottom

/*********************** TFT-LCD *********************************/
  Serial.println("ILI9341 Test (TFT-LCD)!"); 
  // All tft tests peformed and pssed in additional file TFT-LCD_1, removed to save memory
  
  pinMode(TFT_CS, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(7, HIGH);

  tft.begin();
  // read diagnostics (help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("  Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("  MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("  Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("  Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("  Self Diagnostic: 0x"); Serial.println(x, HEX);
  Serial.println("TFT-LCD not connected"); 
  Serial.println(); 
  delay(10000);

/*************************** TOUCH PEN ******************************/
  Serial.begin(38400);
  // deselct all SPI devices
  
  pinMode(10, OUTPUT);    
  pinMode(7, OUTPUT);
  digitalWrite(10, HIGH);
  digitalWrite(7, HIGH);
  
  //ts.begin();
  //ts.setRotation(1); // flip to landscape mode

/*********************** BUTTONS *************************************/  
  Serial.begin(9600);
  tft.begin();
  screenRedrawNecessary = true;
  
/*************************** SD CARD ******************************/
  // Open serial communications and wait for port to open:
  
  while (!Serial) {
    // serial port connect for USB
  Serial.print("Initializing SD card...");

  if (!SD.begin(sd)) {
    Serial.println("Initialization failed!");
    while (1);  // return; will work too
  }
  Serial.println("initialization Done."); // if sd initialization is successful Decision tree can run

  // Open file, only one at a time. Close this one before opening another.
  myFile = SD.open("data.txt", FILE_WRITE);

  // if file opened okay, ready to write:
  if (myFile) {
    Serial.println("Writing to data.txt...");
    myFile.println("Printing measurements: ");
    // close the file:
    myFile.close();
    Serial.println("Done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("Error opening data.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("data.txt");
  if (myFile) {
    Serial.println("data.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("Error opening test.txt");
  }
  }
}
//--------------------------------------------------------------------------//
// LOOP     LOOP      LOOP      LOOP      LOOP      LOOP      LOOP          //
//--------------------------------------------------------------------------// 
void loop(void) {
  
/************************ SOIL MOISTURE SENSOR *********************/
 float soilMoisture = analogRead(A0); 
 Serial.print("Soil Moisture sensor data: "); 
 Serial.print(soilMoisture);
 Serial.println(" g/m³"); 
 //dewpoint (°F or °C), grams of water per cubic meter (g/m³)
 int percentageSoilHumidity = map(soilMoisture, wet, dry, 100, 0);
 Serial.print("Soil Moisture percentage: "); 
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


/*********************** LIGHT SENSOR **************************************/ 
 // reads the input on analog pin A1 (value between 0 and 1023)
  float lightValue = analogRead(A1);
  Serial.print("Light sensor data ");
  Serial.print(lightValue);   // the raw analog reading
  Serial.print(" w/m2"); 
  
  // A few threshholds, qualitatively determined
  if (lightValue < 10) {
    Serial.println(" - Dark");
    Serial.println();
  } else if (lightValue < 200) {
    Serial.println(" - Dim");
    Serial.println();
  } else if (lightValue < 500) {
    Serial.println(" - Light");
    Serial.println();
  } else if (lightValue < 800) {
    Serial.println(" - Bright");
    Serial.println();
  } else {
    Serial.println(" - Very bright");
    Serial.println();
  }
  delay(20000); //delay between measurements increased for testing purposes
  
/*********************** SOLENOID **************************************/
// test case for solenoid

//   digitalWrite(5,HIGH);
//   delay(3000);
//   digitalWrite(5,LOW);
//   delay(5000);

/*********************** TFT-LCD **************************************/
  for(uint8_t rotation=0; rotation<4; rotation++) {
    tft.setRotation(rotation);
   // testText(); call additional calibration functions ommited purposly saving memory
    delay(10000);
  }
  
/*********************** TOUCH PEN **************************************/
// THIS PART IS FOR TESTING PURPOSES ONLY
// comment when object ts for buttons is created 

//    if (ts.touched()) {
//      TS_Point p = ts.getPoint();
//      Serial.print("Pressure = ");
//      Serial.print(p.z);
//      Serial.print(", x = ");
//      Serial.print(p.x);
//      Serial.print(", y = ");
//      Serial.print(p.y);
//      delay(30);
//      Serial.println();
//  }
  
/*********************** BUTTONS *************************************/  
 // Determine whether the user touched the display
  TSPoint p = ts.getPoint();

  if (p.z > PRESSURE_THRESHOLD && !screenRedrawNecessary)
  { 
    if(touchedWithin(p, 165, 100, 320, 450))
    {
      active_colors -= 1;
      screenRedrawNecessary = true;
    }
    else if(touchedWithin(p, 175, 500, 330, 830))
    {
      active_colors += 1;
      screenRedrawNecessary = true;
    }

    if(active_colors < 0)
      active_colors = 2;
    if(active_colors >= 3)
      active_colors = 0;
  }

  if(screenRedrawNecessary)
  {
    resetAndClearScreen();
    drawIntroText();
    drawTouchButton(b1[0], b1[1], b1[2], b1[3], "AUTO ON/OFF");
    drawTouchButton(b2[0], b2[1], b2[2], b2[3], "Presets");
    
    screenRedrawNecessary = false;
  }

/*********************** SD CARD **************************************/
 //Open SD card for writing
   myFile = SD.open("data.txt", FILE_WRITE);
    // write data to SD card
    getMoisture(A0);
    myFile.println(soilMoisture); // soil moisture value
    getLight(A1);
    myFile.println(lightValue);   // light sensor value
    getHumidity();
    myFile.println(h);            // humidity value
    getTempF();
    myFile.println(t);            // temperature value in °C


    // close the file
    if(!myFile){
      myFile.close();
      Serial.println("Error opening file in loop.");
  }
  delay(86400000);            // once a day in milliseconds
  
// EVALUATING DATA IN SD AND MAKE A DECISION

double m = 0.0;
double l = 0.0;
double tt = 0.0;
double hh = 0.0;

for(int i = 0; i < 5; i++){                 //check system values 5 times to prevent discrepances
  
   if(soilMoisture <= 400){                 // if soil moisture is too low
    myFile_values_moisture.read();          // read all collected data and get an average for the period
      for (int i=0; i < myFile_values_moisture.size(); i++){  
        m = m + myFile_values_moisture.position();
      }
      m = m/myFile_values_moisture.size();
      if (m <= soilMoisture){               // if average is lower than mesured value run the solenoid
          digitalWrite(5,HIGH);             // run solenoid
          delay(3600000);                   // water for 1 hour
          digitalWrite(5,LOW);              // stop
      }
   }
      else
        continue;
        
    if(lightValue > 200 && lightValue <= 800){
      myFile_values_light.read();
        for (int i=0; i < myFile_values_light.size(); i++){  
          l = l + myFile_values_light.position();
        }
        l = l/myFile_values_light.size();
        if (l <= getLight(A0)){           
          digitalWrite(5,HIGH);           
          delay(3600000);              
          digitalWrite(5,LOW);          
      }
    }
        else
          continue;
    
    if(getHumidity() <= 40){
      myFile_values_humidity.read();
        for (int i=0; i < myFile_values_humidity.size(); i++){  
          hh = hh + myFile_values_humidity.position();
        }
        hh = hh/myFile_values_humidity.size();
        if (hh <= getHumidity()){           
          digitalWrite(5,HIGH);           
          delay(3600000);              
          digitalWrite(5,LOW);          
      }
      }else
        continue;

    if(dht.readTemperature() <= 5){
      myFile_values_temperature.read();
        for (int i=0; i < myFile_values_temperature.size(); i++){  
          tt = tt + myFile_values_temperature.position();
        }
        tt = tt/myFile_values_temperature.size();
        if (t <= dht.readTemperature()){           
          digitalWrite(5,HIGH);           
          delay(3600000);              
          digitalWrite(5,LOW);          
      }
    }else
      continue;
   }
    Serial.println("Solenoid off. Watering not necessary");
}   
/**********************************************************/
/**************** FUNCTION PROTOTYPS **********************/
/**********************************************************/
bool touchedWithin(TSPoint touch, unsigned x, unsigned y, unsigned w, unsigned h)
{
  return (touch.x > x) && (touch.x < (w)) && (touch.y > y) && (touch.y < (h));
}

void resetAndClearScreen(void)
{
  tft.setRotation(1);
  tft.fillScreen(colors[active_colors]);
  yield();
}

void drawIntroText(void)
{
  tft.setCursor(15, 15);
  tft.setTextColor(font_colors[active_colors]);
  tft.setTextSize(2);
  tft.println("Hello!");
  
  tft.setCursor(15, 55);
  tft.println("Please use the buttons");
  tft.setCursor(15, 75);
  tft.println("below to make your");
  tft.setCursor(15, 95);
  tft.println("selection!");
}

void drawTouchButton(unsigned x, unsigned y, unsigned width, unsigned height, String text)
{
  int x_text_pos = x + width / 2 - (text.length() / 2) * 11;
  int y_text_pos = y + height / 2 - 8;
  
  tft.drawRect(x, y, width, height, font_colors[active_colors]);
  tft.setTextSize(2);
  tft.setTextColor(font_colors[active_colors]);
  tft.setCursor(x_text_pos, y_text_pos);
  tft.print(text);
}

float getMoisture(int pin)
{
  return (analogRead(A0));
}

float getLight(int pin)
{
  return (analogRead(A1));
}

float getTempF()
{
  return (dht.readHumidity());
}

float getHumidity()
{
  return (dht.readHumidity());
}

float getTempC()
{
  return (dht.readTemperature());
}
