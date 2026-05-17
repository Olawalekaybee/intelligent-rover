#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include "TelemetryData.h"

class SensorManager {
public:
    bool begin();
    void readEnvironment(TelemetryData &data);

private:
    Adafruit_BME280 bme;
    bool bmeReady = false;

    float readMQ135Voltage(int rawValue);
};