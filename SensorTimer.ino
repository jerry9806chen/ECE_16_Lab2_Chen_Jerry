#include "CurieTimerOne.h"

//Global variables for current sampling number from the analogPin, the analog pin being sampled from, and the times of the current and previous samplings.
int sampNum;
int analogPin;
unsigned long prevTime;
unsigned long currTime;

//ISR for sampling and printing out sampNum, analogPin, and time since previous sampling. Triggered by CurieTimerOne
void sample() {
  sampNum += 1;
  Serial.print(sampNum);
  Serial.print("\t\t\t");
  Serial.print(analogRead(analogPin));
  Serial.print("\t\t\t");
  currTime = micros();
  Serial.println(currTime - prevTime);
  prevTime = currTime;
}

//Set serial, analogPin, sampNum, the starting time of the program, start the timer and attach the sample interrupt and print the headers.
void setup() {
  Serial.begin(115200);
  analogPin = 0;
  pinMode(analogPin, INPUT);
  while(!Serial);
  sampNum = 0;
  prevTime = micros();
  CurieTimerOne.start(10000, &sample);
  Serial.println("Sample Number\t\tSensor Reading\t\tTime Difference");
}

void loop() {
  //Do nothing.
}
