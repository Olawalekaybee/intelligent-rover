#include "ThingSpeakClient.h"

#include <WiFi.h>
#include <HTTPClient.h>

#include "config/AppConfig.h"
#include "config/Secrets.h"

#define THINGSPEAK_UPLOAD_INTERVAL_MS 20000

void ThingSpeakClient::begin() {
    Serial.println("[THINGSPEAK] Client initialized");
}

void ThingSpeakClient::update(const TelemetryData &data) {
    if (!canUpload()) {
        return;
    }

    uploadTelemetry(data);
}

bool ThingSpeakClient::canUpload() {
    if (WiFi.status() != WL_CONNECTED) {
        return false;
    }

    uint32_t now = millis();

    if ((now - _lastUploadMs) < THINGSPEAK_UPLOAD_INTERVAL_MS) {
        return false;
    }

    _lastUploadMs = now;
    return true;
}

void ThingSpeakClient::uploadTelemetry(const TelemetryData &data) {
    HTTPClient http;

    String url = "http://api.thingspeak.com/update?api_key=";
    url += THINGSPEAK_API_KEY;

    url += "&field1=";
    url += String(data.temperatureC, 2);

    url += "&field2=";
    url += String(data.humidityPercent, 2);

    url += "&field3=";
    url += String(data.pressureHpa, 2);

    url += "&field4=";
    url += String(data.mq135Raw);

    url += "&field5=";
    url += String(data.batteryVoltage, 2);

    url += "&field6=";
    url += String(data.latitude, 6);

    url += "&field7=";
    url += String(data.longitude, 6);

    url += "&field8=";
    url += String(data.gpsSatellites);

    http.begin(url);

    int httpCode = http.GET();

    if (httpCode > 0) {
        String response = http.getString();

        Serial.print("[THINGSPEAK] HTTP ");
        Serial.print(httpCode);
        Serial.print(" Response: ");
        Serial.println(response);
    } else {
        Serial.print("[THINGSPEAK] Upload failed: ");
        Serial.println(http.errorToString(httpCode));
    }

    http.end();
}