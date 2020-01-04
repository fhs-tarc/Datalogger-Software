#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <SPIMemory.h>
Adafruit_BMP280 bmp;
unsigned long time;
SPIFlash flash; 
struct Data {
  float temperature;
  int pressure;
  float altitude;
};
int address = 0;
void setup() {
  Serial.begin(9600); 
  Serial.println(F("BMP280 test"));
  flash.begin();
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
  Serial.println(F("BMP280 test"));
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}
void loop() {
  // Time since launch
  Serial.print(F("Time since launch: "));
  time = millis();
  Serial.println(time);
  // Print this all to the serial monitor for testing purposes
  // What's the temp?
  Serial.print(F("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
  // What's the pressure?
  Serial.print(F("Pressure = "));
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");
  // What's the altitude?
  Serial.print(F("Altitude = "));
  Serial.print(bmp.readAltitude(1007.45));
  Serial.println(" m");
  // Save this information to the struct
  Data thisData = {bmp.readTemperature(), bmp.readPressure(); bmp.readAltitude(1007.45)};
  writeAnything(address, thisData); // write the current struct to the address defined by the previous struct
  address += sizeOf(thisData);
  delay(100); //this will execute 10 times per second
}
