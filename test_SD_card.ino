 ** MOSI - pin 11   ** MISO - pin 12   ** CLK - pin 13   ** CS - pin 7  */
#include <SPI.h>
#include <SD.h>
File myFile;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect
  }
  Serial.println("Initializing SD card...");

  if (!SD.begin(7)) {
    Serial.println("initialization Failed!");
    while (1);
  }
  Serial.println("initialization Done.");

  // Open the file. Only one file can be open at a time, so we have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened is okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("Done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");
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

void loop() {
  
}
