# IoT-Based Smart Fire Detection and Emergency Escape System

An IoT-based fire detection and emergency guidance prototype using the NodeMCU ESP8266, smoke sensor, flame sensor, PIR motion sensor, and a live web dashboard.

## Project Overview

This project detects potential fire hazards using multiple sensors and provides local alerts through LEDs and a buzzer. It also provides a web-based dashboard for monitoring sensor readings and status updates, along with Blynk cloud notifications.

The system is designed as an academic prototype to demonstrate IoT-based fire monitoring and emergency guidance.

## Key Features

- Smoke detection using the MQ-2 sensor.
- Flame detection using a flame sensor.
- Motion detection using a PIR sensor.
- Visual and audio alerts using LEDs and a buzzer.
- Live web dashboard hosted by the ESP8266.
- Real-time dashboard updates using Server-Sent Events (SSE).
- Blynk cloud event notifications during detected fire conditions.
- Demonstration of emergency exit guidance.

## Hardware Components

- NodeMCU ESP8266
- MQ-2 Smoke Sensor
- Flame Sensor
- PIR Motion Sensor
- Green LED
- Red LED
- Buzzer
- Connecting wires and supporting prototype components

## Software and Technologies

- Arduino IDE
- Embedded C++ / Arduino
- ESP8266 Wi-Fi
- Blynk IoT Platform
- ESPAsyncWebServer
- ESPAsyncTCP
- HTML, CSS and JavaScript

## Repository Structure

```text
Smart-Fire-Escape-System/
│
├── firmware/
│   ├── fire_escape_system.ino
│   ├── secrets.h.example
│   └── README.md
│
├── .gitignore
└── README.md
```

## Getting Started

1. Install Arduino IDE.
2. Install the ESP8266 board package.
3. Install the required libraries: Blynk, ESPAsyncTCP and ESPAsyncWebServer.
4. Clone or download this repository.
5. Open `firmware/fire_escape_system.ino`.
6. Create a local `secrets.h` file using `firmware/secrets.h.example` as a template.
7. Add your Wi-Fi and Blynk credentials to the local file.
8. Select the NodeMCU 1.0 (ESP-12E Module) board.
9. Compile and upload the firmware when the hardware is available.

## Pin Configuration

| Component | ESP8266 Pin |
|---|---|
| MQ-2 Smoke Sensor | A0 |
| Flame Sensor | D7 |
| PIR Motion Sensor | D6 |
| Green LED | D5 |
| Red LED | D4 |
| Buzzer | D2 |

## Working Principle

The ESP8266 continuously reads the connected sensors. When smoke or flame is detected, the system activates the red LED and buzzer and sends a notification through Blynk when connected. The local web dashboard displays sensor information and status updates.

## Limitations

- The prototype has not been certified for real-world fire safety.
- Smoke readings are raw analog values and are not calibrated smoke concentrations in ppm.
- Emergency exit guidance is demonstration logic and does not calculate actual safe evacuation routes.
- Actual operation depends on sensor placement, Wi-Fi connectivity and hardware reliability.

## Future Enhancements

- Dynamic evacuation route calculation.
- Integration with additional environmental sensors.
- Improved sensor calibration and alert accuracy.
- Mobile application integration.
- Enhanced emergency monitoring and reporting.

## Disclaimer

This project is developed for academic demonstration and educational purposes only. It is not a replacement for certified fire alarm systems or professional emergency evacuation equipment.
