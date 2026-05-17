#pragma once

#include <Arduino.h>

class BluetoothControl {
public:
    void begin();
    bool available();
    char readCommand();
};