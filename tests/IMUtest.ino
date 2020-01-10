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
  Serial.println("Processor is awake.\n");
  // Start the IMU
  imu.begin();
}
void loop() {
  // Time since launch
  Serial.print(F("Time since launch: "));
  time = millis();
  Serial.println(time);
  // Get all parameters from accelerometer.
  Serial.print("\nAccelerometer:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readFloatAccelX(), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.readFloatAccelY(), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.readFloatAccelZ(), 4);
  // Get all parameters from gyro.
  Serial.print("\nGyroscope:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readFloatGyroX(), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.readFloatGyroY(), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.readFloatGyroZ(), 4);
  // Write values to struct, transfer to flash.
  thisData = {myIMU.readFloatAccelX(), myIMU.readFloatAccelY(), myIMU.readFloatAccelZ()
                  myIMU.readFloatGyroX(), myIMU.readFloatGyroY(), myIMU.readFloatGyroZ()};
  address += sizeOf(thisData);
  delay(100); //this will execute 10 times per second
}
