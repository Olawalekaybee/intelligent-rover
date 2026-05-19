#include "BluetoothControl.h"
#include "config/AppConfig.h"

// ── RoverServerCallbacks ──────────────────────────────────────────────────────

void RoverServerCallbacks::onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo) {
    _parent->_connected = true;
    Serial.println("[BLE] Client connected");
}

void RoverServerCallbacks::onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, int reason) {
    _parent->_connected = false;
    Serial.println("[BLE] Client disconnected - restarting advertising");
    NimBLEDevice::getAdvertising()->start();
}

// ── RoverCharCallbacks ────────────────────────────────────────────────────────

void RoverCharCallbacks::onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {
    std::string rxValue = pCharacteristic->getValue();
    for (size_t i = 0; i < rxValue.length(); i++) {
        _parent->_parseByte((uint8_t)rxValue[i]);
    }
}

// ── Ring buffer ───────────────────────────────────────────────────────────────

void BluetoothControl::_enqueue(char c) {
    uint8_t next = (_tail + 1) % QUEUE_SIZE;
    if (next != _head) {
        _queue[_tail] = c;
        _tail = next;
    }
}

// ── Dabble packet parser ──────────────────────────────────────────────────────
// Confirmed format: [0xFF][MOD][FN][data bytes...][0x00]  — no SIZE field
//
// Classic mode (fn=0x01): [0]=button value, [1]=face button state
// Joystick mode (fn=0x02): [0]=X, [1]=Y, [2]=button state (optional)

void BluetoothControl::_parseByte(uint8_t byte) {
    switch (_parseState) {

        case ParseState::IDLE:
            if (byte == DABBLE_START) {
                _dataIndex = 0;
                memset(_data, 0, sizeof(_data));
                _parseState = ParseState::MODULE;
            } else if (byte >= 0x41 && byte <= 0x7A) {
                // Single-char fallback — nRF Connect / BLE terminal apps
                _enqueue((char)byte);
            }
            break;

        case ParseState::MODULE:
            _moduleId   = byte;
            _parseState = ParseState::FUNCTION;
            break;

        case ParseState::FUNCTION:
            _functionId = byte;
            _parseState = ParseState::DATA;
            break;

        case ParseState::DATA:
            if (byte == DABBLE_END) {
                _dataSize = _dataIndex;
                _processPacket();
                _parseState = ParseState::IDLE;
            } else {
                if (_dataIndex < sizeof(_data)) {
                    _data[_dataIndex++] = byte;
                }
            }
            break;

        default:
            _parseState = ParseState::IDLE;
            break;
    }
}

// ── Process a complete Dabble packet ─────────────────────────────────────────

void BluetoothControl::_processPacket() {
    if (_moduleId != DABBLE_GAMEPAD_MODULE) return;

    char cmd = 'S';

    switch (_functionId) {
        case DABBLE_CLASSIC_MODE:
            // [0] = direction button (0x01=UP, 0x02=DOWN, 0x03=LEFT, 0x04=RIGHT)
            // [1] = face button state (secondary — ignored for driving)
            if (_dataSize >= 1) {
                cmd = _classicToCommand(_data[0]);
            }
            break;

        case DABBLE_JOYSTICK_MODE:
            // [0]=X, [1]=Y (range ~0-7, center=4)
            // [2]=face button bitmask (optional)
            if (_dataSize >= 2) {
                cmd = _joystickToCommand(_data[0], _data[1]);
            }
            break;

        default:
            return;
    }

    // Suppress continuous stop spam — only enqueue meaningful changes
    static char lastCmd = 'S';
    if (cmd != 'S' || lastCmd != 'S') {
        _enqueue(cmd);
        if (cmd != lastCmd) {
            Serial.print("[DABBLE] ");
            Serial.println(cmd);
        }
    }
    lastCmd = cmd;
}

// ── Classic D-pad → motor command ────────────────────────────────────────────
// Confirmed from hardware capture:
//   UP=0x01 → F (forward)    DOWN=0x02 → B (reverse)
//   LEFT=0x03 → L            RIGHT=0x04 → R
//   Cross/no button=0x00 → S (stop)

char BluetoothControl::_classicToCommand(uint8_t button) {
    switch (button) {
        case DABBLE_BTN_UP:       return 'F';
        case DABBLE_BTN_DOWN:     return 'B';
        case DABBLE_BTN_LEFT:     return 'L';
        case DABBLE_BTN_RIGHT:    return 'R';
        case DABBLE_BTN_TRIANGLE: return 'F';  // bonus: triangle = forward
        case DABBLE_BTN_CROSS:    return 'S';  // cross = emergency stop
        case DABBLE_BTN_NONE:     return 'S';
        default:                  return 'S';
    }
}

// ── Joystick → motor command ─────────────────────────────────────────────────
// Dabble joystick range: 0-7, center ≈ 4
// Dominant axis wins; dead zone applied first.

char BluetoothControl::_joystickToCommand(uint8_t x, uint8_t y) {
    int dx = (int)x - JOY_CENTER;   // positive = right
    int dy = (int)y - JOY_CENTER;   // positive = forward

    if (abs(dx) < JOY_THRESHOLD && abs(dy) < JOY_THRESHOLD) return 'S';

    return (abs(dy) >= abs(dx))
        ? (dy > 0 ? 'F' : 'B')
        : (dx > 0 ? 'R' : 'L');
}

// ── Public API ────────────────────────────────────────────────────────────────

void BluetoothControl::begin() {
    _serverCB = new RoverServerCallbacks(this);
    _charCB   = new RoverCharCallbacks(this);

    NimBLEDevice::init(DEVICE_NAME);
    _pServer = NimBLEDevice::createServer();
    _pServer->setCallbacks(_serverCB);

    NimBLEService *pService = _pServer->createService(BLE_UART_SERVICE_UUID);

    _pTxChar = pService->createCharacteristic(
        BLE_UART_TX_CHAR_UUID, NIMBLE_PROPERTY::NOTIFY);

    _pRxChar = pService->createCharacteristic(
        BLE_UART_RX_CHAR_UUID,
        NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR);
    _pRxChar->setCallbacks(_charCB);

    _pServer->start();

    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(BLE_UART_SERVICE_UUID);
    pAdvertising->start();

    Serial.print("[BLE] Advertising as: ");
    Serial.println(DEVICE_NAME);
}

bool BluetoothControl::available()   { return _head != _tail; }
bool BluetoothControl::isConnected() { return _connected; }

char BluetoothControl::readCommand() {
    if (_head == _tail) return 0;
    char c = _queue[_head];
    _head = (_head + 1) % QUEUE_SIZE;
    return c;
}