#include "SensorManager.h"
#include "pins/PinMap.h"
#include "config/AppConfig.h"

bool SensorManager::begin() {
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

    pinMode(PIN_MQ135_ADC, INPUT);
    analogReadResolution(12);

    bmeReady = bme.begin(0x76);

    if (!bmeReady) {
        Serial.println("[SENSOR] BME280 not found at 0x76, trying 0x77");

        bmeReady = bme.begin(0x77);
    }

    if (!bmeReady) {
        Serial.println("[SENSOR] BME280 initialization failed");
        return false;
    }

    Serial.println("[SENSOR] BME280 initialized");
    return true;
}

void SensorManager::readEnvironment(TelemetryData &data) {
    if (bmeReady) {
        data.temperatureC = bme.readTemperature();
        data.humidityPercent = bme.readHumidity();
        data.pressureHpa = bme.readPressure() / 100.0F;
    }

    data.mq135Raw = analogRead(PIN_MQ135_ADC);
    data.mq135Voltage = readMQ135Voltage(data.mq135Raw);

    data.timestampMs = millis();
}

float SensorManager::readMQ135Voltage(int rawValue) {
    return (rawValue / 4095.0f) * 3.3f;
}