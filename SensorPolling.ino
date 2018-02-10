//Global variables for current sampling number from the analogPin, the analog pin being sampled from, and the times of the current and previous samplings.
int sampNum;
int analogPin;
unsigned long prevTime;
unsigned long currTime;

/*Set the analogPin, start serial and sampNum and record startup time. Print headers*/
void setup() {
  analogPin = 0;
  Serial.begin(115200);
  while(!Serial);
  sampNum = 0;
  prevTime = micros();
  Serial.println("Sample Number\t\tSensor Reading\t\tTime Difference");
}

//Polls and prints out the sample number, analog pin reading, and time since last polling at a interval determined by each iteration of the loop.
void loop() {
  currTime = micros();
  if(currTime - prevTime >= 10000) {
    sampNum += 1;
    Serial.print(sampNum);
    Serial.print("\t\t\t");
    Serial.print(analogRead(analogPin));
    Serial.print("\t\t\t");
    Serial.println(currTime - prevTime);
    prevTime = currTime;
  }
}
