# Code to make a Balboa 32U4 robot self-balance using internal sensors and a PID controller.

## Goal
Make a two-wheeled Balboa robot balance in an upright position by accelerating forwards or backwards continuously.

## Background
This was an assignment that I was given in college for a Digital Control and Cyber-physical Systems course. The school lent me a Balboa 32U4 robot, and I was tasked to use a PID controller to make the robot stand upright for at least 5-ish seconds. The code is heavily inspired by the examples in the [official Balboa Arduino library](https://github.com/pololu/balboa-32u4-arduino-library/).\
\
I got hooked on the project and experimented beyond the basic assignment requirements. *5 seconds? pffft!* So I spent some extra time on testing and refactoring.\
\
By trial and error, I managed to tune the PID constants to make the robot balance indefinitely, and even resist being pushed over. I also noticed that the robot had a tendency to wander around on the floor, so I added an extra parameter to the PID that introduces a bias towards reducing the distance travelled from the starting location. The result of that was that the robot could reliably stay within approximately a one square meter area.\
\
When it came time to present my solution to the professor, I was so confident that the robot wouldn't fall, that I chose to balance the robot on top of a table while giving my presentation. I even nudged it a little to prove my point.\
\
I believe the professor was impressed, despite pointing out an embarrassing mistake I had made in my PID controller code. If it works, it works, right? Can't argue with the results. I have since remedied that mistake.

## Dependencies
| Dependency | Usecase |
|---|---------|
| [Balboa 32U4](https://www.pololu.com/product/3575) | You need to have a robot to balance. |
| [LSM6 Arduino](https://github.com/pololu/lsm6-arduino) | Functions to interface with the motors and sensor hardware. |
Follow the Arduino library installation guide, and have fun!
