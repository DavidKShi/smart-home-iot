# Smart Home Monitoring System

A real-time IoT-based smart home monitoring system built using an ESP32, DHT11 temperature and humidity sensor, PIR motion sensor, LED control, and a WiFi-connected dashboard/server.

This project allows remote environmental monitoring and smart device control through HTTP communication between the ESP32 and a backend server.

---

# Features

- Real-time temperature monitoring
- Real-time humidity monitoring
- PIR motion detection system
- Remote LED control through dashboard/server
- Automatic LED activation on motion detection
- HTTP API communication using JSON
- WiFi-enabled ESP32 connectivity
- Efficient sensor polling and debounce logic
- Periodic sensor updates with change detection optimization

---

# Hardware Used

| Component | Quantity |
|---|---|
| ESP32 Dev Board | 1 |
| DHT11 Temperature/Humidity Sensor | 1 |
| PIR Motion Sensor | 1 |
| LED | 1 |
| Resistor | 1 |
| Breadboard + Jumper Wires | 1 Set |

---

# Pin Configuration

| Component | ESP32 Pin |
|---|---|
| DHT11 Data Pin | GPIO 13 |
| PIR Motion Sensor | GPIO 27 |
| LED | GPIO 12 |

---

# System Overview

The ESP32 continuously monitors:

- Temperature
- Humidity
- Motion activity

Sensor data is sent to a backend server using HTTP POST requests in JSON format.

The ESP32 also polls the server for remote LED control updates, allowing a dashboard or web app to control devices remotely.

---

# System Logic

## Motion Detection

- Motion detected → LED turns ON
- No motion → LED turns OFF
- Motion state changes trigger immediate server updates

## Remote LED Control

- Dashboard sends LED state to server
- ESP32 polls server every 300ms
- LED state updates in real time

## Sensor Data Updates

- Temperature and humidity are sent every 5 seconds
- Data is only transmitted when values significantly change
- Reduces unnecessary network traffic

---

# API Communication

## POST `/data`

Sends sensor readings to the backend server.

### Example JSON

```json
{
  "temperature": 24.5,
  "humidity": 61.2,
  "motion": 1
}
```

---

## GET `/get_data`

Retrieves remote LED state from the server.

### Example Response

```json
{
  "led": 1
}
```

---

# Getting Started

## 1. Install Required Libraries

Install the following libraries in Arduino IDE:

```cpp
WiFi.h
HTTPClient.h
DHT.h
ArduinoJson.h
```

---

## 2. Install ESP32 Board Package

In Arduino IDE:

1. Go to:

```text
File → Preferences
```

2. Add this URL in **Additional Boards Manager URLs**:

```text
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

3. Open:

```text
Tools → Board → Boards Manager
```

4. Search for:

```text
ESP32
```

5. Install the package by Espressif Systems.

---

# Configure WiFi and Server

Update the following values inside the code:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverName = "http://YOUR_IP:5000";
```

Replace:

- `YOUR_WIFI_NAME` with your WiFi SSID
- `YOUR_WIFI_PASSWORD` with your WiFi password
- `YOUR_IP` with your backend server IP address

---

# Upload the Code

1. Select your ESP32 board
2. Select the correct COM port
3. Upload the sketch

---

# Serial Monitor Output

Open Serial Monitor at:

```text
115200 baud
```

Example output:

```text
Connecting to WiFi...
Connected!
ESP32 IP: 192.168.x.x
System ready...
```

---

# How It Works

1. ESP32 connects to WiFi
2. DHT11 reads temperature and humidity
3. PIR sensor detects movement
4. ESP32 sends sensor data to backend server
5. Backend dashboard displays live information
6. Dashboard can remotely control LED state
7. ESP32 polls server for LED updates in real time

---

# Safety Notes

- DHT11 accuracy is limited compared to higher-end sensors
- PIR sensors may trigger false positives in noisy environments
- Ensure proper ESP32 voltage compatibility
- Avoid exposing sensors to moisture or extreme temperatures

---

# Possible Applications

- Smart home automation
- Room occupancy monitoring
- Remote environmental monitoring
- IoT-based energy management
- Security and motion detection systems
- Remote appliance control

---

# Future Improvements

- Add MQTT support
- Add OLED display integration
- Implement Firebase or cloud database support
- Add mobile app integration
- Support multiple rooms/sensors
- Add relay modules for appliance control
- Add email/SMS alerts

---

# License

This project is licensed under the MIT License.

---

# Author

Developed by David Shi
