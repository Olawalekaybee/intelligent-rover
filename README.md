# Intelligent Rover  
### AI-Powered Environmental Monitoring, Edge Vision & Remote Inspection Platform

![Platform](https://img.shields.io/badge/platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/framework-Arduino%20%2B%20FreeRTOS-green)
![IDE](https://img.shields.io/badge/IDE-VS%20Code-blue)
![Build](https://img.shields.io/badge/build-PlatformIO-orange)
![AI](https://img.shields.io/badge/AI-Edge%20Inference-red)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

---

# Overview

**Intelligent Rover** is a professional-grade embedded robotics and IoT platform engineered for:

- Real-time environmental monitoring
- Remote visual inspection
- AI-powered object detection
- Live video streaming
- GPS telemetry acquisition
- Cloud-connected sensor analytics
- Bluetooth/gamepad-controlled mobility
- OTA firmware lifecycle management

The system is built around a **dual ESP32-S3 architecture** consisting of:

1. **ESP32-S3 Main Controller**
2. **ESP32-S3 AI Camera Node**

The platform is designed using modern embedded engineering practices including:

- Modular firmware architecture
- RTOS-based task scheduling
- Multi-node communication
- Edge AI inference
- Hardware abstraction
- Structured telemetry pipelines
- CI/CD-ready repository organization

---

# System Architecture

```txt
┌─────────────────────────────────────────────────────────────┐
│                    REMOTE OPERATOR                         │
│                                                             │
│  • Browser-based video monitoring                           │
│  • Telemetry observation                                    │
│  • Bluetooth/Gamepad rover navigation                       │
└─────────────────────────┬───────────────────────────────────┘
                          │
                Internet / Wi-Fi / Bluetooth
                          │
     ┌────────────────────┴────────────────────┐
     │                                         │
     ▼                                         ▼

┌──────────────────────┐          ┌─────────────────────────┐
│ ESP32-S3 MAIN MCU    │          │ ESP32-S3 CAM AI NODE    │
│──────────────────────│          │─────────────────────────│
│ • Motor Control      │          │ • Live Video Streaming  │
│ • Bluetooth Control  │          │ • AI Object Detection   │
│ • Sensor Acquisition │          │ • Edge Inference        │
│ • GPS Processing     │          │ • Detection Events      │
│ • OTA Updates        │          │ • OTA Updates           │
│ • ThingSpeak Upload  │          │ • Remote Streaming      │
│ • RTOS Scheduling    │          │ • Frame Processing      │
└──────────┬───────────┘          └──────────┬──────────────┘
           │                                  │
           │                                  │
     ┌─────▼─────┐                     ┌──────▼──────┐
     │ Sensors   │                     │ AI Camera   │
     └───────────┘                     └─────────────┘
```

---

# Technical Capabilities

## Mobility Subsystem

- 4WD differential drive platform
- PWM-based motor speed control
- Bluetooth/gamepad manual navigation
- RTOS motor scheduling
- High-current motor driver architecture

### Supported Commands

| Command | Function |
|---|---|
| F | Forward |
| B | Reverse |
| L | Turn Left |
| R | Turn Right |
| S | Stop |

---

## Environmental Monitoring Subsystem

### Supported Sensor Data

| Sensor | Measurement |
|---|---|
| BME280 | Temperature |
| BME280 | Humidity |
| BME280 | Atmospheric Pressure |
| MQ-135 | Air Quality / Gas Detection |
| GPS | Latitude / Longitude |
| GPS | Speed / Satellites |

---

## AI Vision Subsystem

### AI Camera Features

- Real-time MJPEG streaming
- Edge-based object detection
- Detection event publishing
- Remote visual inspection
- AI inference pipeline
- OTA-capable AI node

### Planned Detection Models

| Model Type | Status |
|---|---|
| Person Detection | Planned |
| Vehicle Detection | Planned |
| Animal Detection | Planned |
| Fire/Smoke Detection | Future |
| Hazard Detection | Future |

---

# Embedded Software Architecture

## Firmware Design Principles

- Modular architecture
- Non-blocking execution
- RTOS task isolation
- Thread-safe telemetry
- Watchdog recovery
- Hardware abstraction
- OTA-capable deployment
- Structured logging pipeline

---

# RTOS Task Architecture

## Main Controller Tasks

| Task | Priority | Description |
|---|---|---|
| TaskBluetoothControl | High | Receives movement commands |
| TaskMotorControl | High | Controls drive system |
| TaskSensorRead | Medium | Reads environmental sensors |
| TaskGPSRead | Medium | Processes GPS telemetry |
| TaskThingSpeakUpload | Low | Uploads cloud telemetry |
| TaskOTAHandler | Low | Handles OTA updates |
| TaskSystemHealth | Medium | Monitors runtime status |

---

## AI Camera Tasks

| Task | Priority | Description |
|---|---|---|
| TaskFrameCapture | High | Captures camera frames |
| TaskCameraStream | High | Streams video |
| TaskAIInference | High | Performs object detection |
| TaskEventPublisher | Medium | Publishes AI events |
| TaskOTAHandler | Low | Handles OTA updates |

---

# Communication Interfaces

| Interface | Usage |
|---|---|
| Bluetooth | Rover control |
| Wi-Fi | Cloud connectivity |
| UART | GPS communication |
| I2C | Environmental sensors |
| PWM | Motor & servo control |
| ADC | Gas sensor & battery monitoring |

---

# Repository Structure

```txt
intelligent-rover/
│
├── .github/                  # CI/CD workflows
├── .vscode/                  # VS Code settings
├── docs/                     # Technical documentation
├── firmware/
│   ├── shared/               # Shared libraries
│   ├── main-controller/      # Main ESP32-S3 firmware
│   ├── ai-camera/            # AI camera firmware
│   └── tools/                # Firmware utilities
├── cloud/                    # Cloud services
├── ai/                       # AI models & datasets
├── hardware/                 # PCB, schematics, CAD
├── scripts/                  # Build automation
├── assets/                   # Media & renders
└── README.md
```

---

# Hardware Stack

| Component | Function |
|---|---|
| ESP32-S3 | Main embedded controller |
| ESP32-S3 CAM | AI vision processing |
| BTS7960 | Motor driver |
| BME280 | Environmental sensing |
| MQ-135 | Air quality sensing |
| GPS Module | Location tracking |
| MG996R | Camera/sensor positioning |
| Li-Ion Battery Pack | Portable power |

---

# Development Environment

| Tool | Role |
|---|---|
| Visual Studio Code | IDE |
| PlatformIO | Build system |
| Arduino Framework | ESP32 firmware framework |
| FreeRTOS | Multitasking kernel |
| GitHub | Version control |
| ThingSpeak | IoT telemetry platform |

---

# Build & Flash

## Clone Repository

```bash
git clone https://github.com/yourusername/intelligent-rover.git
cd intelligent-rover
```

---

## Build Main Controller

```bash
cd firmware/main-controller
pio run
```

---

## Upload Firmware

```bash
pio run --target upload
```

---

## Serial Monitor

```bash
pio device monitor
```

---

# OTA Firmware Support

The platform supports secure OTA firmware updates for:

- Main controller firmware
- AI camera firmware

OTA deployment architecture enables remote firmware lifecycle management without physical USB access.

---

# Example Telemetry Packet

```json
{
  "temperature": 28.6,
  "humidity": 71.3,
  "pressure": 1008.2,
  "gas": 1830,
  "battery": 11.8,
  "gps": {
    "lat": 9.0765,
    "lng": 7.3986
  },
  "ai_detection": "person"
}
```

---

# Engineering Roadmap

## Phase 1 — Core Rover Platform

- Manual Bluetooth control
- Sensor telemetry
- GPS integration
- ThingSpeak cloud upload

## Phase 2 — AI Vision Integration

- Live streaming
- Object detection
- Remote monitoring

## Phase 3 — Advanced Edge Intelligence

- TinyML optimization
- AI event analytics
- Autonomous assistance
- Hazard recognition

---

# Security Considerations

- OTA authentication
- API key isolation
- Secrets abstraction
- Secure Wi-Fi credential storage
- Cloud token separation

---

# Future Expansion

- Autonomous navigation
- SLAM mapping
- ROS2 bridge
- MQTT infrastructure
- LoRa telemetry
- Edge TPU acceleration
- Web dashboard
- Multi-rover fleet support

---

# License

MIT License

---

# Author

Intelligent Rover Project

---

# Disclaimer

This platform is intended for:

- Robotics research
- Embedded systems development
- Environmental monitoring
- AI experimentation
- Educational engineering applications

Use responsibly and comply with local regulations for remote monitoring and wireless communication systems.