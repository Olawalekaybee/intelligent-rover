#include "MotorControl.h"
#include "pins/PinMap.h"
#include "config/AppConfig.h"

// ======================================================
// LEDC PWM Channel Assignments
// ESP32-S3 supports multiple LEDC channels
// ======================================================

#define CH_LEFT_LPWM   0
#define CH_LEFT_RPWM   1
#define CH_RIGHT_LPWM  2
#define CH_RIGHT_RPWM  3

// ======================================================
// Initialize Motor Driver & PWM Channels
// ======================================================

void MotorControl::begin() {

    // Configure BTS7960 enable pins
    pinMode(PIN_LEFT_LEN, OUTPUT);
    pinMode(PIN_LEFT_REN, OUTPUT);

    pinMode(PIN_RIGHT_LEN, OUTPUT);
    pinMode(PIN_RIGHT_REN, OUTPUT);

    // Enable BTS7960 drivers
    digitalWrite(PIN_LEFT_LEN, HIGH);
    digitalWrite(PIN_LEFT_REN, HIGH);

    digitalWrite(PIN_RIGHT_LEN, HIGH);
    digitalWrite(PIN_RIGHT_REN, HIGH);

    // --------------------------------------------------
    // Configure LEDC PWM Channels
    // --------------------------------------------------

    ledcSetup(CH_LEFT_LPWM, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);
    ledcSetup(CH_LEFT_RPWM, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);

    ledcSetup(CH_RIGHT_LPWM, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);
    ledcSetup(CH_RIGHT_RPWM, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);

    // --------------------------------------------------
    // Attach PWM Channels to Motor Driver Pins
    // --------------------------------------------------

    ledcAttachPin(PIN_LEFT_LPWM, CH_LEFT_LPWM);
    ledcAttachPin(PIN_LEFT_RPWM, CH_LEFT_RPWM);

    ledcAttachPin(PIN_RIGHT_LPWM, CH_RIGHT_LPWM);
    ledcAttachPin(PIN_RIGHT_RPWM, CH_RIGHT_RPWM);

    // Stop motors during startup
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

void MotorControl::stop() {
    setLeftMotor(0);
    setRightMotor(0);
}

// ======================================================
// Low-Level Motor Control Functions
// ======================================================

void MotorControl::setLeftMotor(int speed) {

    // Limit speed to configured range
    speed = constrain(speed, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

    // Forward
    if (speed > 0) {

        ledcWrite(CH_LEFT_LPWM, speed);
        ledcWrite(CH_LEFT_RPWM, 0);
    }

    // Reverse
    else if (speed < 0) {

        ledcWrite(CH_LEFT_LPWM, 0);
        ledcWrite(CH_LEFT_RPWM, abs(speed));
    }

    // Stop
    else {

        ledcWrite(CH_LEFT_LPWM, 0);
        ledcWrite(CH_LEFT_RPWM, 0);
    }
}

void MotorControl::setRightMotor(int speed) {

    // Limit speed to configured range
    speed = constrain(speed, -MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);

    // Forward
    if (speed > 0) {

        ledcWrite(CH_RIGHT_LPWM, speed);
        ledcWrite(CH_RIGHT_RPWM, 0);
    }

    // Reverse
    else if (speed < 0) {

        ledcWrite(CH_RIGHT_LPWM, 0);
        ledcWrite(CH_RIGHT_RPWM, abs(speed));
    }

    // Stop
    else {

        ledcWrite(CH_RIGHT_LPWM, 0);
        ledcWrite(CH_RIGHT_RPWM, 0);
    }
}