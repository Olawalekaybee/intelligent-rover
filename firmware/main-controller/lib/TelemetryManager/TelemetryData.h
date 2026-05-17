#pragma once

struct TelemetryData {
    float temperatureC = 0.0f;
    float humidityPercent = 0.0f;
    float pressureHpa = 0.0f;

    int mq135Raw = 0;
    float mq135Voltage = 0.0f;

    double latitude = 0.0;
    double longitude = 0.0;
    double gpsSpeedKmph = 0.0;
    int gpsSatellites = 0;
    bool gpsValid = false;

    float batteryVoltage = 0.0f;

    unsigned long timestampMs = 0;
};