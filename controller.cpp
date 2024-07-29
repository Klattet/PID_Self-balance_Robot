#include <Wire.h>
#include "Controller.h"

// Variables to store gyro calibration, angles, motor speed, and distance
double gyro_zero;
double angle; // The angle relative to vertical, when laying on ground it will be 90 degrees
double angle_rate; // The rate of rotation
double motor_speed;
double distance_travelled;


// Some code is taken from https://www.pololu.com/docs/0J70/7.1

// Initialize the IMU sensor and perform gyro calibration
void init_imu() {
    Wire.begin(); // Initializes the I2C communication, which is a two-wire serial communication protocol used to connect the microcontroller with the IMU.

    while(!imu.init()) {
        Serial.println("Failed to detect and initialize IMU!");
        delay(1000);
    }
    imu.enableDefault();
    imu.writeReg(LSM6::CTRL2_G, 0b01011000); // 208 Hz, 1000 deg/s

    delay(1000);
}

// Calibrate the gyro by averaging readings
void calibrate_gyro() {
    double gyro_sum = 0;

    for (int i = 0; i < CALIBRATION_ITERATIONS; i++) {
        imu.read();
        gyro_sum += imu.g.y;
        delay(5);
    }
    gyro_zero = gyro_sum / CALIBRATION_ITERATIONS;
}

// Set up the controller by initializing the IMU, calibrating the gyro, and configuring motors
void setup_controller() {
    init_imu();
    calibrate_gyro();

    motors.flipLeftMotor(true);
    motors.flipRightMotor(true);
}

// Variables for PID control
double error = 0;            // Current error term
double integral_error = 0;   // Cumulative sum of error over time
double derivative_error = 0; // Rate of change of error
double previous_error = 0;   // Previous error term initialized to zero

// PID control function to calculate motor speed based on the error terms
double control_pid(double setpoint, double variable) {
    error = setpoint - variable;
    integral_error += error * UPDATE_DELAY / 1000;
    derivative_error = (error - previous_error) / UPDATE_DELAY / 1000;
    previous_error = error;
    return KP * error + KI * integral_error + KD * derivative_error;
}

//Corrects for offset, converts to degree and integrates the angle continuously.
void integrate_gyro() {
    imu.read();

    //The gyro returns an integer value that represents the rate of rotation.
    angle_rate = (imu.g.y - gyro_zero) / 29; //According to the documentation, we can divide by 29 to get millidegrees per second.
    
    angle += angle_rate * UPDATE_DELAY / 1000;
}

bool isBalancingStatus = false;

void update_speed() {
    if (isBalancingStatus) {
        motor_speed += control_pid(TARGET_ANGLE, ANGLE_WEIGHT * angle + DISTANCE_WEIGHT * distance_travelled);
        motor_speed = fmin(fmax(motor_speed, -SPEED_LIMIT), SPEED_LIMIT); // Limit speed
    } else {
        motor_speed = 0;
        if (angle_rate > -2 && angle_rate < 2) {

            // Calculates the pitch angle in degrees based on the accelerometer readings. The pitch angle represents the orientation of the device relative to the ground plane.
            // Positive pitch typically corresponds to tilting the device forward, while negative pitch corresponds to tilting it backward.
            angle = atan2(imu.a.z / 1000, imu.a.x / 1000) * 180 / 3.1415;
        }
    }
    isBalancingStatus = abs(angle) < START_BALANCING_ANGLE;
}

// Continuously estimate how far the robot has traveled from starting location.
void update_distance() {
    distance_travelled += (encoders.getCountsAndResetLeft() + encoders.getCountsAndResetRight()) / 2;
}

void update_controller() {
    integrate_gyro();

    update_speed();
    update_distance();
    
    motors.setSpeeds(
        motor_speed,
        motor_speed
    );
}