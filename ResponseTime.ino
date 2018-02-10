#include "CurieIMU.h"
#include "CurieTimerOne.h"

//The time in milliseconds at which the sketch checks for a response from the user (disrupt accelerometer and gyroscope).
unsigned long startTime;

//Random number of seconds to trigger the LED to get use to trigger accelerometer/gyroscope, and boolean for indicating whether to check the gyroscope or accelerometer.
int randNum;
boolean checkGyro;

//The starting and current position and orientation of the accelerometer/gyroscope.
float startX, startY, startZ;
float nextX, nextY, nextZ;

//Checks for movement from either the gyroscope or accelerometer (depends on user input) and returns the result.
boolean detectMovement() {
  boolean movementDetected = false;
  if(checkGyro) {
    CurieIMU.readGyroScaled(nextX, nextY, nextZ);
    movementDetected = (abs((nextX - startX) > 2.0 || abs((nextY - startY) > 2.0)) || abs((nextZ - startZ) > 2.0));
  }
  else {
    CurieIMU.readAccelerometerScaled(nextX, nextY, nextZ);
    movementDetected = (abs((nextX - startX) > 0.1 || abs((nextY - startY) > 0.1)) || abs((nextZ - startZ) > 0.1));
  }
  return movementDetected;
}

//If the user moves the Arduino board and disrupts the accelerometer or gyroscope, the board prints out the response time.
void checkResponse() {
  if(detectMovement()) {
    Serial.print("Response time: ");
    Serial.print((millis() - startTime) / 1000.0);
    Serial.println(" seconds");
    CurieTimerOne.pause();
    digitalWrite(LED_BUILTIN, LOW);
    startTime = millis();
  }
}

void setup() {
  //Sets the LED to light up, starts the Serial and sets the seed for random numbers by reading from the analog pin 0.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  randomSeed(analogRead(0));

  //Sets up CurieIMU and the accelerometer and gyroscope.
  CurieIMU.begin();
  CurieIMU.setAccelerometerRange(3);
  CurieIMU.setGyroRange(250);

  //Wait for Serial to get ready.
  while(!Serial);

  //User selects which source to sample from.
  Serial.println("Which sensor would you like to sample (G/A)?");
  while(!Serial.available());
  String sampleSource = Serial.readString();

  //The random number of seconds is set.
  randNum = random(1, 10 + 1);

  //Start the first countdown till the board checks for user response.
  startTime = millis();

  //Blinks the LED twice to indicate that the countdown has begun.
  delay(250);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);

  //Determine whether to check input from gyroscope or accelerometer and record in checkGyro, or print an error message.
  int indexG = sampleSource.indexOf('G');
  int indexA = sampleSource.indexOf('A');
  int index_g = sampleSource.indexOf('g');
  int index_a = sampleSource.indexOf('a');
  if(index_g >= 0 && index_g < indexG)
    indexG = index_g;
  if(index_a >= 0 && index_a < indexA)
    indexA = index_a;
  if(indexA >= 0 || indexG >= 0)
    checkGyro = (indexG >= 0 && (indexG < indexA || indexA < 0));
  else
    Serial.println("Error!");

  //Read starting positions from either the gyroscope or accelerometer, depending on checkGyro.
  if(checkGyro)
    CurieIMU.readGyroScaled(startX, startY, startZ);
  else
    CurieIMU.readAccelerometerScaled(startX, startY, startZ);

  //Wait until the countdown is done. If there is any premature movement, the countdown resets.
  while(millis() - startTime < randNum * 1000) {
    if(detectMovement())
      startTime = millis();
  }

  //Start the timer to periodically check for user response every 100 microseconds and attach checkResponse as an ISR.
  CurieTimerOne.start(100, &checkResponse);

  //Turn LED on to indicate that the board is waiting for a response from the user and record the initial time of response checking.
  digitalWrite(LED_BUILTIN, HIGH);
  startTime = millis();
}

void loop() {
//Do nothing
}
