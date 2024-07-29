#include <Balboa32U4.h>
#include <Wire.h>
#include <LSM6.h>
#include "Controller.h"

// Instantiate objects for IMU, motors, and encoders
LSM6 imu;
Balboa32U4Motors motors;
Balboa32U4Encoders encoders;

void setup() {
    Serial.begin(9600);
    // Turn on the red LED during setup
    ledRed(true);
      
    // Initialize the controller setup
    setup_controller();

    // Turn off the red LED and turn on the green LED. Signifies that the robot is ready to start balancing.
    ledRed(false);
    ledGreen(true);
}


void loop() {
    // Wait until it is time to update balancing.
    delay(UPDATE_DELAY);

    update_controller();
}




