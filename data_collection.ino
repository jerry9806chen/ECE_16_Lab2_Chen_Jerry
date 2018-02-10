#include "CurieTimerOne.h"
#include "CurieIMU.h"

//the source the user selects to sample data from (in integer form (1: accelerometer; 2: gyroscope; 3: analog pin)) 
int userIntSource;

//time since the sampling started and the current time.
unsigned long startTime;
unsigned long currTime;

//values of acceleration and gyroscopic rotation and the analog pin voltage
float ax, ay, az, gx, gy, gz;
float pinVal;

//String of printed information and flag for each time new data is read to trigger the conditional in loop.
char printString[50];
boolean newRead;

//Depending on userIntSource, samplingISR will sample from the accelerometer, gyroscope, or analog pin and flag the newly read data.
void samplingISR() {
  if(userIntSource == 1)
    CurieIMU.readAccelerometerScaled(ax, ay, az);
  else if(userIntSource == 2)
    CurieIMU.readGyroScaled(gx, gy, gz);
  else if(userIntSource == 3)
    pinVal = analogRead(0);
  newRead = true;
}

void setup() {  
  Serial.begin(115200);
  CurieIMU.begin();
  CurieIMU.setGyroRange(250);
  CurieIMU.setAccelerometerRange(5);
  while(!Serial);

  while(!Serial.available());
  userIntSource = Serial.readString().toInt();
  newRead = false;

  startTime = millis();

  CurieTimerOne.start(5000, &samplingISR);
}

//Print out the time each data is read and the data read from the sampling sources.
void loop() {
  currTime = millis();
  if(newRead) {
    Serial.print("Time since start: ");
    Serial.print(currTime - startTime);
    if(userIntSource == 1) {
      sprintf(printString, "ms\n-ax=%.2f, ay=%.2f, az=%.2f\n", ax, ay, az);
      Serial.print(printString);
    }
    else if(userIntSource == 2) {
      sprintf(printString, "ms\n-gx=%.2f, gy=%.2f, gz=%.2f\n", gx, gy, gz);
      Serial.print(printString);
    }
    else if(userIntSource == 3) {
      Serial.print("ms\nPin Voltage = ");
      Serial.print(pinVal/1000.0);
      Serial.println("V");
    }
    newRead = false;
  }
}
