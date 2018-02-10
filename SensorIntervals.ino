#include "CurieIMU.h"

//The analog pin being read and the times at the start and end of each data sampling.
int analogPin;
unsigned long startTime;
unsigned long endTime;

//Setup CurieIMU accelerometer and gyroscope, start serial, and set the analoPin to record from.
void setup() {
  CurieIMU.begin();

  CurieIMU.setAccelerometerRange(2);
  CurieIMU.setGyroRange(250);

  Serial.begin(115200);

  analogPin = 0;
  pinMode(analogPin, INPUT);
  while(!Serial);
}

//Continually reads from accelerometer, analog pin, and gyroscope to the local variables and measures and prints the start and end time for each read.
void loop() {
  int analogVal;
  float ax, ay, az, gx, gy, gz;

  Serial.println("Microseconds for Accel read");
  startTime = micros();
  CurieIMU.readAccelerometerScaled(ax, ay, az);
  endTime = micros();
  Serial.println(endTime - startTime);
  
  Serial.println("Microseconds for analogRead");
  startTime = micros();
  analogVal = analogRead(analogPin);
  endTime = micros();
  Serial.println(endTime - startTime);

  Serial.println("Microseconds for Gyro read");
  startTime = micros();
  CurieIMU.readGyroScaled(gx, gy, gz);
  endTime = micros();
  Serial.println(endTime - startTime);
}
