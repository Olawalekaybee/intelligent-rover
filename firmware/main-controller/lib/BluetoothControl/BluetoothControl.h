#pragma once

#include <Arduino.h>
#include <NimBLEDevice.h>

// BLE UART UUIDs (Nordic UART Service — Dabble compatible)
#define BLE_UART_SERVICE_UUID   "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define BLE_UART_RX_CHAR_UUID   "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define BLE_UART_TX_CHAR_UUID   "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// Dabble protocol — confirmed format: [0xFF][MOD][FN][data...][0x00]
#define DABBLE_START             0xFF
#define DABBLE_END               0x00
#define DABBLE_GAMEPAD_MODULE    0x01

// Function IDs — confirmed from hardware
#define DABBLE_CLASSIC_MODE      0x01
#define DABBLE_JOYSTICK_MODE     0x02

// Classic D-pad button values — confirmed: UP=0x01 → 'F'
// DOWN/LEFT/RIGHT expected as 0x02/0x03/0x04 (Dabble sequential enum)
#define DABBLE_BTN_NONE          0x00
#define DABBLE_BTN_UP            0x01   // confirmed ✓
#define DABBLE_BTN_DOWN          0x02
#define DABBLE_BTN_LEFT          0x03
#define DABBLE_BTN_RIGHT         0x04
#define DABBLE_BTN_TRIANGLE      0x05
#define DABBLE_BTN_CROSS         0x06
#define DABBLE_BTN_CIRCLE        0x07
#define DABBLE_BTN_SQUARE        0x08

// Joystick: Dabble uses range 0-7, center = 4, threshold = 1
#define JOY_CENTER               4
#define JOY_THRESHOLD            1

// Forward declaration
class BluetoothControl;

class RoverServerCallbacks : public NimBLEServerCallbacks {
public:
    RoverServerCallbacks(BluetoothControl *parent) : _parent(parent) {}
    void onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo) override;
    void onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, int reason) override;
private:
    BluetoothControl *_parent;
};

class RoverCharCallbacks : public NimBLECharacteristicCallbacks {
public:
    RoverCharCallbacks(BluetoothControl *parent) : _parent(parent) {}
    void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) override;
private:
    BluetoothControl *_parent;
};

class BluetoothControl {
    friend class RoverServerCallbacks;
    friend class RoverCharCallbacks;

public:
    void begin();
    bool available();
    char readCommand();
    bool isConnected();

private:
    NimBLEServer         *_pServer  = nullptr;
    NimBLECharacteristic *_pTxChar  = nullptr;
    NimBLECharacteristic *_pRxChar  = nullptr;
    RoverServerCallbacks *_serverCB = nullptr;
    RoverCharCallbacks   *_charCB   = nullptr;

    volatile bool _connected = false;

    static const uint8_t QUEUE_SIZE = 16;
    char    _queue[QUEUE_SIZE];
    uint8_t _head = 0;
    uint8_t _tail = 0;

    enum class ParseState : uint8_t { IDLE, MODULE, FUNCTION, DATA };

    ParseState _parseState = ParseState::IDLE;
    uint8_t    _moduleId   = 0;
    uint8_t    _functionId = 0;
    uint8_t    _dataSize   = 0;
    uint8_t    _dataIndex  = 0;
    uint8_t    _data[8]    = {0};

    void _enqueue(char c);
    void _parseByte(uint8_t byte);
    void _processPacket();
    char _classicToCommand(uint8_t button);
    char _joystickToCommand(uint8_t x, uint8_t y);
};