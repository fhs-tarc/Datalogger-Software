#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include "SPI.h"
#include "SPI.h"
#include "SD.h"

LSM6DS3 imu;
SPIFlash flash;
struct Data {
  unsigned long time;
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;
}
Data thisData;
File flightData;
int address = 0;
int maxAddress = 0;

void saveToSD();

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println(F("Processor is awake.\n"));
  // Start the Flash
  flash.begin();
  // Start the IMU
  imu.begin();
}

void loop() {
  // Write values to struct, transfer to flash.
  thisData = {millis(), myIMU.readFloatAccelX(), myIMU.readFloatAccelY(), myIMU.readFloatAccelZ()
              myIMU.readFloatGyroX(), myIMU.readFloatGyroY(), myIMU.readFloatGyroZ()};
  // Time since launch
  Serial.print(F("Time since launch: "));
  Serial.println(millis());
  // Get all parameters from accelerometer.
  Serial.print(F("\nAccelerometer:\n"));
  Serial.print(F(" X = "));
  Serial.println(thisData.ax, 4);
  Serial.print(F(" Y = "));
  Serial.println(thisData.ay, 4);
  Serial.print(F(" Z = "));
  Serial.println(thisData.az, 4);
  // Get all parameters from gyro.
  Serial.print("\nGyroscope:\n");
  Serial.print(F(" X = "));
  Serial.println(thisData.gx, 4);
  Serial.print(F(" Y = "));
  Serial.println(thisData.gy, 4);
  Serial.print(F(" Z = "));
  Serial.println(thisData.gz, 4);
  writeAnything(address, thisData);
  address += sizeOf(thisData);
  delay(100); //this will execute 10 times per second
}

void saveToSD() {
    int counter = 1;
    address = 0;
    while( SD.exists("data" + counter + ".csv") ) {
        counter++;
    }
    flightData = SD.open("data" + counter + ".csv");
    while( address < maxAddress ) {
        flash.readAnything(address, thisData);
        flightData.println(thisData.ax, thisData.ay, thisData.az, thisData.gx, thisData.gy, thisData.gz);
        address += sizeof(thisData);
    }
}
