#include "MotorControl.h"
#include "pins/PinMap.h"
#include "config/AppConfig.h"

void MotorControl::begin() {
    pinMode(PIN_LEFT_LEN, OUTPUT);
    pinMode(PIN_LEFT_REN, OUTPUT);
    pinMode(PIN_RIGHT_LEN, OUTPUT);
    pinMode(PIN_RIGHT_REN, OUTPUT);

    digitalWrite(PIN_LEFT_LEN, HIGH);
    digitalWrite(PIN_LEFT_REN, HIGH);
    digitalWrite(PIN_RIGHT_LEN, HIGH);
    digitalWrite(PIN_RIGHT_REN, HIGH);

    ledcAttach(PIN_LEFT_LPWM, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);
    ledcAttach(PIN_LEFT_RPWM, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);
    ledcAttach(PIN_RIGHT_LPWM, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);
    ledcAttach(PIN_RIGHT_RPWM, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);

    stop();
}

void MotorControl::forward(uint8_t speed) {
    setLeftMotor(speed);
    setRightMotor(speed);
}

void MotorControl::reverse(uint8_t speed) {
    setLeftMotor(-speed);
    setRightMotor(-speed);
}

void MotorControl::turnLeft(uint8_t speed) {
    setLeftMotor(-speed);
    setRightMotor(speed);
}

void MotorControl::turnRight(uint8_t speed) {
    setLeftMotor(speed);
    setRightMotor(-speed);
}

void MotorControl::stop() {
    setLeftMotor(0);
    setRightMotor(0);
}

void MotorControl::setLeftMotor(int speed) {
    speed = constrain(speed, -255, 255);

    if (speed > 0) {
        ledcWrite(PIN_LEFT_LPWM, speed);
        ledcWrite(PIN_LEFT_RPWM, 0);
    } else if (speed < 0) {
        ledcWrite(PIN_LEFT_LPWM, 0);
        ledcWrite(PIN_LEFT_RPWM, abs(speed));
    } else {
        ledcWrite(PIN_LEFT_LPWM, 0);
        ledcWrite(PIN_LEFT_RPWM, 0);
    }
}

void MotorControl::setRightMotor(int speed) {
    speed = constrain(speed, -255, 255);

    if (speed > 0) {
        ledcWrite(PIN_RIGHT_LPWM, speed);
        ledcWrite(PIN_RIGHT_RPWM, 0);
    } else if (speed < 0) {
        ledcWrite(PIN_RIGHT_LPWM, 0);
        ledcWrite(PIN_RIGHT_RPWM, abs(speed));
    } else {
        ledcWrite(PIN_RIGHT_LPWM, 0);
        ledcWrite(PIN_RIGHT_RPWM, 0);
    }
}