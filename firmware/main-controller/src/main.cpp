#include <Arduino.h>
#include "config/AppConfig.h"
#include "pins/PinMap.h"

#include "MotorControl.h"
#include "BluetoothControl.h"
#include "SensorManager.h"
#include "GPSManager.h"
#include "TelemetryData.h"
#include "RoverWiFiManager.h"
#include "OTAUpdate.h"
#include "ThingSpeakClient.h"

MotorControl      motor;
BluetoothControl  bluetooth;
SensorManager     sensors;
GPSManager        gpsManager;
TelemetryData     telemetry;
RoverWiFiManager  wifiManager;
OTAUpdate         otaUpdate;
ThingSpeakClient  thingSpeakClient;

static volatile uint8_t currentSpeed = DEFAULT_MOTOR_SPEED;
static volatile uint32_t lastCommandTime = 0;

#define SPEED_MIN  60
#define SPEED_MAX  255
#define COMMAND_TIMEOUT_MS 500

void TaskBluetoothControl(void *pvParameters);
void TaskSensorRead(void *pvParameters);
void TaskGPSRead(void *pvParameters);
void TaskStatusLED(void *pvParameters);
void TaskNetworkService(void *pvParameters);

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000);

    Serial.println();
    Serial.println("=================================");
    Serial.println("  Intelligent Rover Booting");
    Serial.println("  Board: ESP32 + Classic BT");
    Serial.println("=================================");

    pinMode(PIN_STATUS_LED, OUTPUT);

    motor.begin();
    bluetooth.begin();
    sensors.begin();
    gpsManager.begin();

    wifiManager.begin();
    otaUpdate.begin();
    thingSpeakClient.begin();

    lastCommandTime = millis();

    xTaskCreatePinnedToCore(TaskBluetoothControl, "BT_Control", 4096, nullptr, 3, nullptr, 1);
    xTaskCreatePinnedToCore(TaskSensorRead, "Sensor_Read", 4096, nullptr, 2, nullptr, 0);
    xTaskCreatePinnedToCore(TaskGPSRead, "GPS_Read", 4096, nullptr, 2, nullptr, 0);
    xTaskCreatePinnedToCore(TaskStatusLED, "Status_LED", 2048, nullptr, 1, nullptr, 0);
    xTaskCreatePinnedToCore(TaskNetworkService, "Network_Service", 4096, nullptr, 1, nullptr, 0);

    Serial.printf("[SYSTEM] Rover ready. Default speed: %d\n", currentSpeed);
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(1000));
}

void TaskBluetoothControl(void *pvParameters) {
    char speedBuf[8];
    uint8_t speedIdx = 0;
    bool inSpeedCmd = false;
    bool motorsStoppedByFailsafe = false;

    while (true) {
        while (bluetooth.available()) {
            char c = bluetooth.readCommand();
            lastCommandTime = millis();
            motorsStoppedByFailsafe = false;

            if (c == '#') {
                inSpeedCmd = true;
                speedIdx = 0;
                memset(speedBuf, 0, sizeof(speedBuf));
                continue;
            }

            if (inSpeedCmd) {
                if (c == '\n' || c == '\r') {
                    inSpeedCmd = false;

                    if (speedIdx > 0) {
                        int val = atoi(speedBuf);

                        if (val >= SPEED_MIN && val <= SPEED_MAX) {
                            currentSpeed = (uint8_t)val;
                            Serial.printf("[BT] Speed: %d\n", currentSpeed);
                        }
                    }
                } else if (isDigit(c) && speedIdx < (sizeof(speedBuf) - 1)) {
                    speedBuf[speedIdx++] = c;
                }

                continue;
            }

            if (c == '\n' || c == '\r') {
                continue;
            }

            switch (c) {
                case 'F':
                case 'f':
                    motor.forward(currentSpeed);
                    Serial.printf("[BT] Forward spd=%d\n", currentSpeed);
                    break;

                case 'B':
                case 'b':
                    motor.reverse(currentSpeed);
                    Serial.printf("[BT] Reverse spd=%d\n", currentSpeed);
                    break;

                case 'L':
                case 'l':
                    motor.turnLeft(currentSpeed);
                    Serial.printf("[BT] Left spd=%d\n", currentSpeed);
                    break;

                case 'R':
                case 'r':
                    motor.turnRight(currentSpeed);
                    Serial.printf("[BT] Right spd=%d\n", currentSpeed);
                    break;

                case 'G':
                case 'g':
                    motor.forwardLeft(currentSpeed);
                    Serial.printf("[BT] Fwd-Left spd=%d\n", currentSpeed);
                    break;

                case 'I':
                case 'i':
                    motor.forwardRight(currentSpeed);
                    Serial.printf("[BT] Fwd-Right spd=%d\n", currentSpeed);
                    break;

                case 'H':
                case 'h':
                    motor.reverseLeft(currentSpeed);
                    Serial.printf("[BT] Rev-Left spd=%d\n", currentSpeed);
                    break;

                case 'J':
                case 'j':
                    motor.reverseRight(currentSpeed);
                    Serial.printf("[BT] Rev-Right spd=%d\n", currentSpeed);
                    break;

                case 'S':
                case 's':
                case 'Q':
                case 'q':
                    motor.stop();
                    Serial.println("[BT] STOP");
                    break;

                case 'W':
                    Serial.println("[BT] Light ON");
                    break;

                case 'w':
                    Serial.println("[BT] Light OFF");
                    break;

                case 'U':
                    Serial.println("[BT] Horn ON");
                    break;

                case 'u':
                    Serial.println("[BT] Horn OFF");
                    break;

                default:
                    break;
            }
        }

        if ((millis() - lastCommandTime) > COMMAND_TIMEOUT_MS && !motorsStoppedByFailsafe) {
            motor.stop();
            motorsStoppedByFailsafe = true;
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void TaskNetworkService(void *pvParameters) {
    while (true) {
        wifiManager.update();
        otaUpdate.handle();
        thingSpeakClient.update(telemetry);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void TaskSensorRead(void *pvParameters) {
    while (true) {
        sensors.readEnvironment(telemetry);

        telemetry.batteryVoltage = 12.1f;

        Serial.println("[SENSOR] -------------------");
        Serial.printf("[SENSOR] Temp:     %.2f C\n", telemetry.temperatureC);
        Serial.printf("[SENSOR] Humidity: %.2f %%\n", telemetry.humidityPercent);
        Serial.printf("[SENSOR] Pressure: %.2f hPa\n", telemetry.pressureHpa);
        Serial.printf("[SENSOR] MQ135:    %d raw / %.2f V\n",
                      telemetry.mq135Raw, telemetry.mq135Voltage);
        Serial.printf("[SENSOR] Battery:  %.2f V\n", telemetry.batteryVoltage);

        vTaskDelay(pdMS_TO_TICKS(SENSOR_READ_INTERVAL_MS));
    }
}

void TaskGPSRead(void *pvParameters) {
    while (true) {
        gpsManager.update();
        gpsManager.readGPS(telemetry);

        if (telemetry.gpsValid) {
            Serial.printf("[GPS] Lat: %.6f  Lng: %.6f  Spd: %.1f km/h  Sats: %d\n",
                          telemetry.latitude,
                          telemetry.longitude,
                          telemetry.gpsSpeedKmph,
                          telemetry.gpsSatellites);
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