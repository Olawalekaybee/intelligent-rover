#include "RoverWiFiManager.h"

#include "config/AppConfig.h"
#include "config/Secrets.h"

#define WIFI_RECONNECT_INTERVAL_MS 10000

void RoverWiFiManager::begin() {
    Serial.println("[WIFI] Initializing Wi-Fi");

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("[WIFI] Connecting");

    uint8_t retries = 0;

    while (WiFi.status() != WL_CONNECTED && retries < 20) {
        delay(500);
        Serial.print(".");
        retries++;
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("[WIFI] Connected");
        Serial.print("[WIFI] IP Address: ");
        Serial.println(WiFi.localIP());

        Serial.print("[WIFI] RSSI: ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
    } else {
        Serial.println("[WIFI] Connection failed");
    }
}

void RoverWiFiManager::update() {
    if (WiFi.status() == WL_CONNECTED) {
        return;
    }

    uint32_t now = millis();

    if ((now - _lastReconnectAttempt) >= WIFI_RECONNECT_INTERVAL_MS) {
        _lastReconnectAttempt = now;
        reconnect();
    }
}

void RoverWiFiManager::reconnect() {
    Serial.println("[WIFI] Reconnecting...");

    WiFi.disconnect();
    delay(100);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

bool RoverWiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

IPAddress RoverWiFiManager::getIP() {
    return WiFi.localIP();
}

int32_t RoverWiFiManager::getRSSI() {
    return WiFi.RSSI();
}