
void setup() {
  Serial.begin(9600);
  Serial.println("Light sensor Test Done!"); 

}

void loop() {
   // reads the input on analog pin A1 (value between 0 and 1023)
  int lightValue = analogRead(A1);
  Serial.print("Light sensor data ");
  Serial.print(lightValue);   // the raw analog reading
  Serial.print(" w/m2"); 
  
  // A few threshholds, qualitatively determined
  if (lightValue < 10) {
    Serial.println(" - Dark");
  } else if (lightValue < 200) {
    Serial.println(" - Dim");
  } else if (lightValue < 500) {
    Serial.println(" - Light");
  } else if (lightValue < 800) {
    Serial.println(" - Bright");
  } else {
    Serial.println(" - Very bright");
  }
  delay(2000); //delay between measurements
}
