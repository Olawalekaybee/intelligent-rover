#include "OTAUpdate.h"

#include <WiFi.h>
#include <ArduinoOTA.h>

#include "config/Secrets.h"

void OTAUpdate::begin() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[OTA] Wi-Fi not connected, OTA not started");
        return;
    }

    ArduinoOTA.setHostname(OTA_HOSTNAME);
    ArduinoOTA.setPassword(OTA_PASSWORD);

    ArduinoOTA.onStart([]() {
        Serial.println("[OTA] Update started");
    });

    ArduinoOTA.onEnd([]() {
        Serial.println();
        Serial.println("[OTA] Update finished");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("[OTA] Progress: %u%%\r", (progress * 100) / total);
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("[OTA] Error[%u]: ", error);

        if (error == OTA_AUTH_ERROR) {
            Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
        }
    });

    ArduinoOTA.begin();

    _started = true;

    Serial.println("[OTA] OTA service started");
    Serial.print("[OTA] Hostname: ");
    Serial.println(OTA_HOSTNAME);
}

void OTAUpdate::handle() {
    if (_started) {
        ArduinoOTA.handle();
    }
}