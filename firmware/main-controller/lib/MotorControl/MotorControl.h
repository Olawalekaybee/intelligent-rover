#pragma once

#include <Arduino.h>

class MotorControl {
public:
    void begin();

    void forward(uint8_t speed);
    void reverse(uint8_t speed);
    void turnLeft(uint8_t speed);
    void turnRight(uint8_t speed);
    void stop();

private:
    void setLeftMotor(int speed);
    void setRightMotor(int speed);
};