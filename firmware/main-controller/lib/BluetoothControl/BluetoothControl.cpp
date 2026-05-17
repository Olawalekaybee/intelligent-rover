#include "BluetoothControl.h"
#include "config/AppConfig.h"
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void BluetoothControl::begin() {
    SerialBT.begin(DEVICE_NAME);
    Serial.println("[BT] Bluetooth started");
}

bool BluetoothControl::available() {
    return SerialBT.available();
}

char BluetoothControl::readCommand() {
    return SerialBT.read();
}