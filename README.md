# Smart Library Management System

This repository contains the code and documentation for a Smart Library Management System built with two ESP32 microcontrollers communicating via the ESP-NOW protocol. :contentReference[oaicite:0]{index=0}

## Table of Contents
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Wiring](#wiring)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Features
- Master–slave ESP32 communication over ESP-NOW
- RFID tag scanning (Slave ESP32)
- PIR motion detection (Slave ESP32)
- OLED display updates (Master ESP32)
- Buzzer and LED alarm system (Master ESP32)

## Hardware Requirements
- 2× ESP32 development boards
- RFID reader module
- PIR motion sensor
- SSD1306 OLED display
- Buzzer
- LED

## Wiring
1. **Master ESP32**  
   - GPIO25 → Buzzer  
   - GPIO26 → LED  
   - I2C (GPIO21 = SDA, GPIO22 = SCL) → SSD1306 OLED  
2. **Slave ESP32**  
   - RFID module connections (SPI/I2C/UART as per your reader)  
   - PIR sensor output → GPIO13 pin  
   - ESP-NOW peer setup with master’s MAC address

## Software Requirements
- PlatformIO with the Espressif32 platform :contentReference[oaicite:1]{index=1}
- Arduino framework or ESP-IDF
- SSD1306 library for OLED
- ESP-NOW support (built into ESP32 SDK)

## Installation
1. Clone this repository:
   ```bash
   git clone https://github.com/FahhaMohamed/library-management-secure-system.git
   cd library-management-secure-system
