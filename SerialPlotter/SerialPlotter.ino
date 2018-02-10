#include "CurieIMU.h"
#include "CurieTimerOne.h"

//Sample from the gyroscope and print the magnitude
void sampleGyro() {
  float gx, gy, gz;
  CurieIMU.readGyroScaled(gx, gy, gz);
  Serial.println(sqrt(sq(gx) + sq(gy) + sq(gz)));
}

//Sample from the accelerometer and print the magnitude
void sampleAccel() {
  float ax, ay, az;
  CurieIMU.readAccelerometerScaled(ax, ay, az);
  Serial.println(sqrt(sq(ax) + sq(ay) + sq(az)));
}

void setup() {
  //Setup Serial and the CurieIMU and its gyroscope and accelerometer.
  Serial.begin(115200);
  CurieIMU.begin();
  CurieIMU.setGyroRange(250);
  CurieIMU.setAccelerometerRange(5);
  while(!Serial);

  //Request user input to select which source to acquire data sampling from.
  Serial.println("Board ready!\nWhich sensor would you like to sample?");
  Serial.println("\t(A: Accelerometer, G: Gyroscope)");
  while(!Serial.available());
  String sampleSource = Serial.readString();

  //Indicates further progression of setup to user.
  Serial.println("Initializing IMU device...");

  //Determine whether to check input from gyroscope or accelerometer.
  int indexG = sampleSource.indexOf('G');
  int indexA = sampleSource.indexOf('A');
  int index_g = sampleSource.indexOf('g');
  int index_a = sampleSource.indexOf('a');
  if(index_g >= 0 && index_g < indexG)
    indexG = index_g;
  if(index_a >= 0 && index_a < indexA)
    indexA = index_a;

  //If user entered G first, we will check the gyroscope every 5000 milliseconds and print/plot it.
  if(indexG >= 0 && (indexG < indexA || indexA < 0)) {
    CurieTimerOne.start(5000, &sampleGyro);
  }
  //If user entered A first, we will check the accelerometer every 5000 milliseconds and print/plot it.
  else if(indexA >= 0 && (indexA < indexG || indexG < 0)){
    CurieTimerOne.start(5000, &sampleAccel);
  }
  //If user selected neither, send an error message.
  else {
    Serial.println("Error. Invalid entry.");
  }
}

void loop() {
  //Do nothing
}
