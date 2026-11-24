# Smart Fire Escape System – Firmware

This folder contains the Arduino firmware for the IoT-Based Smart Fire Detection and Emergency System using the NodeMCU ESP8266.

## Hardware Components
- NodeMCU ESP8266
- MQ-2 Smoke Sensor
- Flame Sensor
- PIR Motion Sensor
- Green LED
- Red LED
- Buzzer

## Pin Configuration

| Component | NodeMCU Pin |
|---|---|
| MQ-2 Smoke Sensor | A0 |
| Flame Sensor | D7 |
| PIR Motion Sensor | D6 |
| Green LED | D5 |
| Red LED | D4 |
| Buzzer | D2 |

## Required Software
- Arduino IDE
- ESP8266 Board Package
- Blynk Library
- ESPAsyncTCP Library
- ESPAsyncWebServer Library

## Credentials Setup

1. Make a copy of `secrets.h.example`.
2. Rename the copy to `secrets.h`.
3. Add your private Blynk authentication token.
4. Never upload `secrets.h` to GitHub.

## Important
This is an academic prototype for demonstration and learning. It is not a certified fire alarm or real-world emergency evacuation system.
