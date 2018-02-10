#include "CurieTimerOne.h"
#include "CurieIMU.h"

//The last recorded orientation
int lastOrientation;

void checkOrient() {
  int orientation = - 1;   // the board's current orientation, set to -1 because it has yet to be actually recorded. 
  String orientationString; // the string for printing the description of orientation
  /*
    The orientations of the board:
    0: flat, processor facing up
    1: flat, processor facing down
    2: Landscape-1, analog pins down
    3: Landscape-2, analog pins up
    4: Portrait-1, USB connector up
    5: Portrait-2, USB connector down
  */
  // read accelerometer:
  int x = CurieIMU.readAccelerometer(X_AXIS);
  int y = CurieIMU.readAccelerometer(Y_AXIS);
  int z = CurieIMU.readAccelerometer(Z_AXIS);
  
  // calculate the absolute values, to determine the largest
  int absX = abs(x);
  int absY = abs(y);
  int absZ = abs(z);

  //Base orientation on z; if the accelerometer reading (gravity) magnitude is highest on the z axis, it is the vertical axis.
  if ( (absZ > absX) && (absZ > absY)) {
    //accelerometer points to the bottom side. Board is facing up
    if (z > 0) {
      orientationString = "up";
      orientation = 0;  
    } 
    //accelerometer points to the top side. Board is facing down
    else {
      orientationString = "down";
      orientation = 1;
    }
  } 

  //Base orientation on y; if the accelerometer reading (gravity) magnitude is highest on the y axis, it is the vertical axis.
  else if ( (absY > absX) && (absY > absZ)) {
    //accelerometer points to the analog pins. Board is in Landscape-1
    if (y > 0) {
      orientationString = "Landscape-1";
      orientation = 2;
    } 
    //accelerometer points to the digital pins. Board is in Landscape-2
    else {
      orientationString = "Landscape-2";
      orientation = 3;
    }
  } 

  //Base orientation on x; if the accelerometer reading (gravity) magnitude is highest on the x axis, it is the vertical axis.
  else {
    //accelerometer points away from the USB connector. Board is in Potrait-1
    if (x < 0) {
      orientationString = "Portrait-1";
      orientation = 4;
    } 
    //accelerometer points to the USB connector. Board is in Potrait-2
    else {
      orientationString = "Portrait-2";
      orientation = 5;
    }
  }
  
  // if the orientation has changed, print out a description and flag the new orientation:
  if (orientation != lastOrientation) {
    Serial.println("Orientation has changed to ");
    Serial.println(orientationString);
    lastOrientation = orientation;
  }
}

//Setup the CurieIMU, Serial, and accelerometer. Set lastOrientation to -1 (not recorded yet). Set clock to trigger every 100000 microseconds the ISR checkOrient.
void setup() {
  CurieIMU.begin();
  CurieIMU.setAccelerometerRange(2);

  Serial.begin(115200);
  while(!Serial);

  lastOrientation = -1;

  CurieTimerOne.start(100000, &checkOrient);
}

void loop() {
  //Do nothing
}
