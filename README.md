# Intelligent Rover

## AI-Powered Autonomous Inspection, Environmental Monitoring & Edge Vision Platform

![Platform](https://img.shields.io/badge/platform-ESP32%20%2B%20ESP32--S3-blue)
![Framework](https://img.shields.io/badge/framework-Arduino%20%2B%20FreeRTOS-green)
![IDE](https://img.shields.io/badge/IDE-VS%20Code-blue)
![Build](https://img.shields.io/badge/build-PlatformIO-orange)
![AI](https://img.shields.io/badge/AI-Edge%20Inference-red)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

---

# Overview

**Intelligent Rover** is a professional embedded robotics and IoT platform engineered for:

* Autonomous and manual rover mobility
* Real-time environmental monitoring
* Edge AI object detection
* Remote live video streaming
* GPS telemetry acquisition
* Cloud-connected analytics
* Bluetooth/gamepad control
* OTA firmware lifecycle management
* Distributed embedded processing

The platform combines:

* **ESP32 Main Controller** for robotics, sensors, RTOS control, telemetry, and connectivity
* **ESP32-S3 AI Camera Node** for edge AI inference and video streaming

The system is designed using modern embedded systems engineering principles:

* RTOS task isolation
* Modular firmware architecture
* Hardware abstraction layers
* Distributed node architecture
* OTA-capable deployment
* Structured telemetry pipelines
* Scalable IoT integration
* AI-ready edge processing

---

# System Architecture

```txt
┌─────────────────────────────────────────────────────────────┐
│                    REMOTE OPERATOR                         │
│                                                             │
│  • Bluetooth/Gamepad Control                               │
│  • Remote Video Monitoring                                 │
│  • Telemetry Observation                                   │
│  • Cloud Diagnostics                                       │
└─────────────────────────┬───────────────────────────────────┘
                          │
                Internet / Wi‑Fi / Bluetooth
                          │
     ┌────────────────────┴────────────────────┐
     │                                         │
     ▼                                         ▼

┌──────────────────────┐          ┌─────────────────────────┐
│ ESP32 MAIN MCU       │          │ ESP32-S3 AI CAMERA NODE │
│──────────────────────│          │─────────────────────────│
│ • Motor Control      │          │ • Live Video Streaming  │
│ • Bluetooth Control  │          │ • Edge AI Inference     │
│ • Sensor Acquisition │          │ • Object Detection      │
│ • GPS Processing     │          │ • Frame Processing      │
│ • OTA Updates        │          │ • Remote Streaming      │
│ • ThingSpeak Upload  │          │ • OTA Updates           │
│ • RTOS Scheduling    │          │ • AI Event Publishing   │
└──────────┬───────────┘          └──────────┬──────────────┘
           │                                  │
           │                                  │
     ┌─────▼─────┐                     ┌──────▼──────┐
     │ Sensors   │                     │ AI Camera   │
     └───────────┘                     └─────────────┘
```

---

# Core Features

## Rover Mobility System

* 4WD differential drive architecture
* BTS7960 high-current motor driver control
* PWM speed modulation
* Bluetooth Classic gamepad/app control
* Diagonal directional steering
* RTOS-based motion scheduling
* Command failsafe protection
* Dynamic speed control

### Supported Motion Commands

| Command | Action        |
| ------- | ------------- |
| F       | Forward       |
| B       | Reverse       |
| L       | Turn Left     |
| R       | Turn Right    |
| G       | Forward Left  |
| I       | Forward Right |
| H       | Reverse Left  |
| J       | Reverse Right |
| S       | Stop          |

---

# Environmental Monitoring System

## Supported Sensors

| Sensor     | Measurement                 |
| ---------- | --------------------------- |
| BME280     | Temperature                 |
| BME280     | Humidity                    |
| BME280     | Atmospheric Pressure        |
| MQ135      | Air Quality / Gas Detection |
| GPS Module | Latitude / Longitude        |
| GPS Module | Speed / Satellites          |

### Telemetry Capabilities

* Real-time environmental sensing
* Structured telemetry aggregation
* Cloud telemetry upload
* GPS tracking
* Air quality monitoring
* Expandable sensor architecture

---

# AI Vision Subsystem

## ESP32-S3 AI Camera Node

### Features

* Real-time MJPEG video streaming
* Edge AI object detection
* Remote visual inspection
* OTA firmware support
* Frame acquisition pipeline
* AI event publishing
* Future autonomous assistance support

### Planned AI Models

| Model                  | Status  |
| ---------------------- | ------- |
| Person Detection       | Planned |
| Vehicle Detection      | Planned |
| Animal Detection       | Planned |
| Fire / Smoke Detection | Future  |
| Hazard Recognition     | Future  |

---

# Embedded Software Architecture

## Firmware Design Principles

* Modular subsystem isolation
* Non-blocking firmware execution
* RTOS task scheduling
* Dual-core ESP32 optimization
* Structured telemetry handling
* Hardware abstraction layers
* OTA-capable deployment
* Expandable architecture
* Fault-tolerant control design

---

# RTOS Task Architecture

## Main Controller Tasks

| Task                 | Priority | Description                  |
| -------------------- | -------- | ---------------------------- |
| TaskBluetoothControl | High     | Processes rover commands     |
| TaskSensorRead       | Medium   | Reads environmental sensors  |
| TaskGPSRead          | Medium   | Processes GPS telemetry      |
| TaskStatusLED        | Low      | System heartbeat indicator   |
| TaskThingSpeakUpload | Low      | Uploads cloud telemetry      |
| TaskOTAHandler       | Low      | Handles OTA firmware updates |

---

## AI Camera Tasks

| Task               | Priority | Description            |
| ------------------ | -------- | ---------------------- |
| TaskFrameCapture   | High     | Captures camera frames |
| TaskCameraStream   | High     | Streams live video     |
| TaskAIInference    | High     | Performs AI detection  |
| TaskEventPublisher | Medium   | Publishes AI events    |
| TaskOTAHandler     | Low      | Handles OTA updates    |

---

# Communication Interfaces

| Interface         | Usage                    |
| ----------------- | ------------------------ |
| Bluetooth Classic | Rover control            |
| Wi‑Fi             | IoT connectivity         |
| UART              | GPS communication        |
| I2C               | Sensor communication     |
| PWM               | Motor control            |
| ADC               | Gas & battery monitoring |

---

# Repository Structure

```txt
intelligent-rover/
│
├── .github/                  # CI/CD workflows
├── .vscode/                  # VS Code workspace settings
├── docs/                     # Technical documentation
├── firmware/
│   ├── shared/               # Shared libraries
│   ├── main-controller/      # ESP32 rover controller firmware
│   ├── ai-camera/            # ESP32-S3 AI camera firmware
│   └── tools/                # Development utilities
├── cloud/                    # Cloud services & integrations
├── ai/                       # AI models & datasets
├── hardware/                 # PCB, schematics & CAD files
├── scripts/                  # Build & automation scripts
├── assets/                   # Images & media assets
└── README.md
```

---

# Hardware Stack

| Component           | Function                  |
| ------------------- | ------------------------- |
| ESP32               | Main rover controller     |
| ESP32-S3 CAM        | AI vision processing      |
| BTS7960             | Motor driver              |
| BME280              | Environmental sensing     |
| MQ135               | Gas & air quality sensing |
| GPS Module          | Position tracking         |
| Li-Ion Battery Pack | Portable power system     |
| MG996R Servo        | Camera/sensor positioning |

---

# Development Environment

| Tool               | Role                     |
| ------------------ | ------------------------ |
| Visual Studio Code | IDE                      |
| PlatformIO         | Embedded build system    |
| Arduino Framework  | Firmware framework       |
| FreeRTOS           | RTOS scheduler           |
| GitHub             | Version control          |
| ThingSpeak         | Cloud telemetry platform |

---

# Build & Deployment

## Clone Repository

```bash
git clone https://github.com/yourusername/intelligent-rover.git
cd intelligent-rover
```

---

## Build Main Controller Firmware

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

## Open Serial Monitor

```bash
pio device monitor
```

---

# OTA Firmware Updates

The Intelligent Rover platform supports OTA firmware updates for:

* ESP32 main controller
* ESP32-S3 AI camera node

OTA deployment enables:

* Remote firmware maintenance
* Wireless feature updates
* Distributed node management
* Reduced physical maintenance requirements

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

* Bluetooth rover control
* Motor driver integration
* Sensor telemetry
* GPS integration
* RTOS architecture

## Phase 2 — Cloud & OTA Infrastructure

* Wi‑Fi integration
* ThingSpeak telemetry upload
* OTA firmware updates
* Remote diagnostics

## Phase 3 — AI Vision Integration

* Live camera streaming
* Object detection
* AI event publishing
* Remote monitoring

## Phase 4 — Advanced Robotics

* Autonomous navigation
* TinyML optimization
* Hazard recognition
* Edge intelligence
* Multi-node coordination

---

# Security Considerations

* OTA authentication
* Wi‑Fi credential isolation
* API key abstraction
* Secure cloud communication
* Secrets management
* Modular credential storage

---

# Future Expansion

* SLAM mapping
* ROS2 bridge
* MQTT infrastructure
* LoRa telemetry
* Web dashboard
* Multi-rover fleet management
* Autonomous patrol mode
* Edge TPU acceleration

---

# License

MIT License

---

# Disclaimer

This platform is intended for:

* Embedded systems engineering
* Robotics research
* Environmental monitoring
* AI experimentation
* Educational and industrial prototyping

Users are responsible for ensuring compliance with local laws and regulations regarding wireless communication, remote monitoring, and autonomous systems.
