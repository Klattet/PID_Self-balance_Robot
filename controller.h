#pragma once
#include <LSM6.h>
#include <Balboa32U4.h>


static double DISTANCE_WEIGHT = 0.005;
static double ANGLE_WEIGHT = 1;
static double TARGET_ANGLE = 20;
static double SPEED_LIMIT = 300;
static double START_BALANCING_ANGLE = 45;

static int CALIBRATION_ITERATIONS = 100;
static int UPDATE_DELAY = 10 // milliseconds

// I had to fix a bug in the PID controller at some point that had an impact on the PID constants.
// Since I no longer have access to a Balboa 32U4 robot, I cannot test the performance characteristics of these constants.
// If you use this code for your own balancing project, you will likely have to use different constants.
static double KP = 1.3;
static double KI = 0.2;
static double KD = 10;


extern double angle;
extern double angle_rate;
extern double motor_speed;


extern LSM6 imu;
extern Balboa32U4Motors motors;
extern Balboa32U4Encoders encoders;

void setup_controller();
void update_controller();
