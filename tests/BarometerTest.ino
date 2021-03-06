/*
 * This program is designed to test a barometer, as well as test writing data to a SPI flash chip and an SD card.
 */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <SPIMemory.h>
#include <SPI.h>
#include <SD.h>

Adafruit_BMP280 bmp;
unsigned long time;
SPIFlash flash;
File flightData;
struct Data {
    unsigned long time;
    float temperature;
    float pressure;
    float altitude;
};
Data thisData;
int address = 0;
int maxAddress = 0;
String filename;
void saveToSD();

void setup() {
    Serial.begin(9600); 
    Serial.println(F("BMP280 test"));
    flash.begin();
    SD.begin(4);
    if (!bmp.begin()) {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
        while (1);
    }
    Serial.println(F("BMP280 test"));

    filename = String();

    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
    // Time since launch
    Serial.print(F("Time since launch: "));
    Serial.println(millis());
    // Print this all to the serial monitor for testing purposes
    // What's the temp?
    Serial.print(F("Temperature = "));
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
    thisData = {millis(), bmp.readTemperature(), bmp.readPressure(), bmp.readAltitude(1007.45)};
    flash.writeAnything(address, thisData); // write the current struct to the address defined by the previous struct
    address += sizeof(thisData);
    delay(100); //this will execute 10 times per second
    if(millis() > 20000) {
        maxAddress = address;
        saveToSD();
    }
}
void saveToSD() {
    int counter = 1;
    address = 0;
    char filename[12];
    sprintf(filename, "data%d.csv", counter);
    while(SD.exists(filename) ) {
        counter++;
        sprintf(filename, "data%d.csv", counter);
    }
    
    flightData = SD.open(filename);
    while( address < maxAddress ) {
        flash.readAnything(address, thisData);
        flightData.print(thisData.time); flightData.print(",");
        flightData.print(thisData.temperature); flightData.print(",");
        flightData.print(thisData.pressure); flightData.print(",");
        flightData.println(thisData.altitude);
        address += sizeof(thisData);
    }
}
