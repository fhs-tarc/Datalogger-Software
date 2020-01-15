#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include "SPI.h"

LSM6DS3 imu;
SPIFlash flash;
struct Data {
  unsigned long time;
  float aSubX;
  float aSubY;
  float aSubZ;
  float omegaSubX;
  float omegaSubY;
  float omegaSubZ;
}
Data thisData;
int address = 0;
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
  // Time since launch
  Serial.print(F("Time since launch: "));
  Serial.println(millis());
  // Get all parameters from accelerometer.
  Serial.print(F("\nAccelerometer:\n"));
  Serial.print(F(" X = "));
  Serial.println(myIMU.readFloatAccelX(), 4);
  Serial.print(F(" Y = "));
  Serial.println(myIMU.readFloatAccelY(), 4);
  Serial.print(F(" Z = "));
  Serial.println(myIMU.readFloatAccelZ(), 4);
  // Get all parameters from gyro.
  Serial.print("\nGyroscope:\n");
  Serial.print(F(" X = "));
  Serial.println(myIMU.readFloatGyroX(), 4);
  Serial.print(F(" Y = "));
  Serial.println(myIMU.readFloatGyroY(), 4);
  Serial.print(F(" Z = "));
  Serial.println(myIMU.readFloatGyroZ(), 4);
  // Write values to struct, transfer to flash.
  thisData = {millis(), myIMU.readFloatAccelX(), myIMU.readFloatAccelY(), myIMU.readFloatAccelZ()
                  myIMU.readFloatGyroX(), myIMU.readFloatGyroY(), myIMU.readFloatGyroZ()};
  writeAnything(address, thisData);
  address += sizeOf(thisData);
  delay(100); //this will execute 10 times per second
}
/* Using read anything from the flash library, we can write the data we collected line by line
   to an csv file on the SD card. We'll increment the address by sizeOf everytime and write by one line at a time
   to the SD card using write() in the SD library*/
