#include "MotorControl.h"
#include "pins/PinMap.h"
#include "config/AppConfig.h"

// ======================================================
// LEDC PWM Channel Assignments
// ESP32 supports LEDC PWM channels
// ======================================================

#define CH_LEFT_LPWM   0
#define CH_LEFT_RPWM   1
#define CH_RIGHT_LPWM  2
#define CH_RIGHT_RPWM  3

#define DIAGONAL_INNER_SPEED 120

// ======================================================
// Initialize Motor Driver & PWM Channels
// ======================================================

void MotorControl::begin() {
    pinMode(PIN_LEFT_LEN, OUTPUT);
    pinMode(PIN_LEFT_REN, OUTPUT);
    pinMode(PIN_RIGHT_LEN, OUTPUT);
    pinMode(PIN_RIGHT_REN, OUTPUT);

    digitalWrite(PIN_LEFT_LEN, HIGH);
    digitalWrite(PIN_LEFT_REN, HIGH);
    digitalWrite(PIN_RIGHT_LEN, HIGH);
    digitalWrite(PIN_RIGHT_REN, HIGH);

    ledcSetup(CH_LEFT_LPWM, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);
    ledcSetup(CH_LEFT_RPWM, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);
    ledcSetup(CH_RIGHT_LPWM, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);
    ledcSetup(CH_RIGHT_RPWM, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);

    ledcAttachPin(PIN_LEFT_LPWM, CH_LEFT_LPWM);
    ledcAttachPin(PIN_LEFT_RPWM, CH_LEFT_RPWM);
    ledcAttachPin(PIN_RIGHT_LPWM, CH_RIGHT_LPWM);
    ledcAttachPin(PIN_RIGHT_RPWM, CH_RIGHT_RPWM);

    stop();

    Serial.println("[MOTOR] Motor controller initialized");
}

// ======================================================
// High-Level Motion Functions
// ======================================================

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

void MotorControl::forwardLeft(uint8_t speed) {
    setLeftMotor(DIAGONAL_INNER_SPEED);
    setRightMotor(speed);
}

void MotorControl::forwardRight(uint8_t speed) {
    setLeftMotor(speed);
    setRightMotor(DIAGONAL_INNER_SPEED);
}

void MotorControl::reverseLeft(uint8_t speed) {
    setLeftMotor(-DIAGONAL_INNER_SPEED);
    setRightMotor(-speed);
}

void MotorControl::reverseRight(uint8_t speed) {
    setLeftMotor(-speed);
    setRightMotor(-DIAGONAL_INNER_SPEED);
}

void MotorControl::stop() {
    setLeftMotor(0);
    setRightMotor(0);
}

// ======================================================
// Low-Level Motor Control Functions
// ======================================================

void MotorControl::setLeftMotor(int speed) {
    speed = constrain(speed, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

    if (speed > 0) {
        ledcWrite(CH_LEFT_LPWM, speed);
        ledcWrite(CH_LEFT_RPWM, 0);
    } else if (speed < 0) {
        ledcWrite(CH_LEFT_LPWM, 0);
        ledcWrite(CH_LEFT_RPWM, abs(speed));
    } else {
        ledcWrite(CH_LEFT_LPWM, 0);
        ledcWrite(CH_LEFT_RPWM, 0);
    }
}

void MotorControl::setRightMotor(int speed) {
    speed = constrain(speed, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

    if (speed > 0) {
        ledcWrite(CH_RIGHT_LPWM, speed);
        ledcWrite(CH_RIGHT_RPWM, 0);
    } else if (speed < 0) {
        ledcWrite(CH_RIGHT_LPWM, 0);
        ledcWrite(CH_RIGHT_RPWM, abs(speed));
    } else {
        ledcWrite(CH_RIGHT_LPWM, 0);
        ledcWrite(CH_RIGHT_RPWM, 0);
    }
}