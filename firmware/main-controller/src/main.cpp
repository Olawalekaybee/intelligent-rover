#include <Arduino.h>
#include "config/AppConfig.h"
#include "pins/PinMap.h"

#include "MotorControl.h"
#include "BluetoothControl.h"
#include "SensorManager.h"
#include "GPSManager.h"
#include "TelemetryData.h"

MotorControl motor;
BluetoothControl bluetooth;
SensorManager sensors;
GPSManager gpsManager;
TelemetryData telemetry;

void TaskBluetoothControl(void *pvParameters);
void TaskStatusLED(void *pvParameters);
void TaskSensorRead(void *pvParameters);
void TaskGPSRead(void *pvParameters);

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000);

    Serial.println();
    Serial.println("=================================");
    Serial.println(" Intelligent Rover Main Booting");
    Serial.println("=================================");

    pinMode(PIN_STATUS_LED, OUTPUT);

    motor.begin();
    bluetooth.begin();
    sensors.begin();
    gpsManager.begin();

    xTaskCreatePinnedToCore(
        TaskBluetoothControl,
        "BluetoothControl",
        4096,
        nullptr,
        3,
        nullptr,
        1
    );

    xTaskCreatePinnedToCore(
        TaskSensorRead,
        "SensorRead",
        4096,
        nullptr,
        2,
        nullptr,
        0
    );

    xTaskCreatePinnedToCore(
        TaskGPSRead,
        "GPSRead",
        4096,
        nullptr,
        2,
        nullptr,
        0
    );

    xTaskCreatePinnedToCore(
        TaskStatusLED,
        "StatusLED",
        2048,
        nullptr,
        1,
        nullptr,
        0
    );

    Serial.println("[SYSTEM] Main controller ready");
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(1000));
}

void TaskBluetoothControl(void *pvParameters) {
    while (true) {
        if (bluetooth.available()) {
            char command = bluetooth.readCommand();

            Serial.print("[BT] Command received: ");
            Serial.println(command);

            switch (command) {
                case 'F':
                case 'f':
                    motor.forward(DEFAULT_MOTOR_SPEED);
                    break;

                case 'B':
                case 'b':
                    motor.reverse(DEFAULT_MOTOR_SPEED);
                    break;

                case 'L':
                case 'l':
                    motor.turnLeft(DEFAULT_MOTOR_SPEED);
                    break;

                case 'R':
                case 'r':
                    motor.turnRight(DEFAULT_MOTOR_SPEED);
                    break;

                case 'S':
                case 's':
                    motor.stop();
                    break;

                default:
                    motor.stop();
                    break;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void TaskSensorRead(void *pvParameters) {
    while (true) {
        sensors.readEnvironment(telemetry);

        Serial.println("[SENSOR] -------------------");

        Serial.print("[SENSOR] Temp: ");
        Serial.print(telemetry.temperatureC);
        Serial.println(" C");

        Serial.print("[SENSOR] Humidity: ");
        Serial.print(telemetry.humidityPercent);
        Serial.println(" %");

        Serial.print("[SENSOR] Pressure: ");
        Serial.print(telemetry.pressureHpa);
        Serial.println(" hPa");

        Serial.print("[SENSOR] MQ135 Raw: ");
        Serial.println(telemetry.mq135Raw);

        Serial.print("[SENSOR] MQ135 Voltage: ");
        Serial.print(telemetry.mq135Voltage);
        Serial.println(" V");

        vTaskDelay(pdMS_TO_TICKS(SENSOR_READ_INTERVAL_MS));
    }
}

void TaskGPSRead(void *pvParameters) {
    while (true) {
        gpsManager.update();
        gpsManager.readGPS(telemetry);

        if (telemetry.gpsValid) {
            Serial.print("[GPS] Lat: ");
            Serial.print(telemetry.latitude, 6);

            Serial.print(" Lng: ");
            Serial.print(telemetry.longitude, 6);

            Serial.print(" Speed: ");
            Serial.print(telemetry.gpsSpeedKmph);
            Serial.print(" km/h");

            Serial.print(" Satellites: ");
            Serial.println(telemetry.gpsSatellites);
        } else {
            Serial.println("[GPS] Waiting for valid fix...");
        }

        vTaskDelay(pdMS_TO_TICKS(GPS_READ_INTERVAL_MS));
    }
}

void TaskStatusLED(void *pvParameters) {
    while (true) {
        digitalWrite(PIN_STATUS_LED, HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));

        digitalWrite(PIN_STATUS_LED, LOW);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}