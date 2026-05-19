#include "interfaces/UARTBridge.h"

#include <ArduinoJson.h>

#include "config/AppConfig.h"

void UARTBridge::begin() {

    aiSerial.begin(
        AI_EVENT_UART_BAUD_RATE,
        SERIAL_8N1,
        AI_UART_RX_PIN,
        AI_UART_TX_PIN
    );

    Serial.println("[UART] AI bridge initialized");
}

void UARTBridge::sendDetection(
    const String &label,
    float confidence,
    int x,
    int y,
    int w,
    int h
) {

    StaticJsonDocument<256> doc;

    doc["type"] = "detection";
    doc["label"] = label;
    doc["confidence"] = confidence;

    doc["x"] = x;
    doc["y"] = y;
    doc["w"] = w;
    doc["h"] = h;

    serializeJson(doc, aiSerial);
    aiSerial.println();

    Serial.print("[UART] Detection sent: ");
    serializeJson(doc, Serial);
    Serial.println();
}

void UARTBridge::sendHeartbeat() {

    StaticJsonDocument<128> doc;

    doc["type"] = "heartbeat";
    doc["device"] = DEVICE_NAME;

    serializeJson(doc, aiSerial);
    aiSerial.println();

    Serial.println("[UART] Heartbeat sent");
}