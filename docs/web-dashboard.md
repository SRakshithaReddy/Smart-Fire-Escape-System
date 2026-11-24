# Web Dashboard Documentation

## 1. Introduction

The Smart Fire Escape System includes a web-based dashboard hosted directly on the NodeMCU ESP8266. It allows users to monitor sensor information through a browser connected to the same local Wi-Fi network.

## 2. Dashboard Features

- Displays the smoke sensor's raw analog reading.
- Shows smoke detection status.
- Shows flame detection status.
- Displays PIR motion detection status.
- Provides visual information about the system's monitoring state.
- Displays demonstration-based emergency exit guidance.

## 3. Technology Used

| Technology | Purpose |
|---|---|
| HTML | Dashboard structure |
| CSS | Dashboard styling |
| JavaScript | Dynamic interface updates |
| ESPAsyncWebServer | Hosts the dashboard |
| Server-Sent Events (SSE) | Sends live sensor updates |
| ESP8266 Wi-Fi | Connects the dashboard to the controller |

## 4. Dashboard Communication

The ESP8266 hosts the dashboard on port 80.

The firmware provides two main endpoints:

- `/` – Serves the dashboard webpage.
- `/events` – Provides Server-Sent Events for sensor updates.

The dashboard receives updates for smoke readings, smoke status, flame status and motion status.

## 5. Cloud Notifications

The firmware also integrates with the Blynk IoT platform. When a fire condition is detected and the Blynk connection is available, the system sends a configured event notification.

## 6. Important Limitation

The dashboard's emergency exit recommendations are predefined demonstration logic. They are not based on real-time route sensing, building layouts or verified evacuation conditions.

The dashboard is intended for academic demonstration and monitoring, not as a certified emergency guidance system.
