ESP32 Master AP with Buzzer Alert

Overview
This project sets up an ESP32 as a Wi-Fi Access Point (AP) and runs a simple HTTP server. The ESP32 listens for alerts from a slave ESP32 device and activates a buzzer after a 30-second delay when an alert is received.

Features
- Configures ESP32 as an Access Point (AP)
- Hosts an HTTP server with an `/alert` endpoint
- Triggers a buzzer 30 seconds after receiving an alert

Components Required
- ESP32 Development Board
- Buzzer
- Power Supply

Wiring
| ESP32 GPIO | Component |
|------------|----------|
| GPIO 27    | Buzzer   |

Installation
1. Clone or copy the project to your ESP32 development environment.
2. Ensure you have the ESP-IDF (Espressif IoT Development Framework) installed.
3. Build and flash the firmware to the ESP32:
   ```sh
   idf.py build
   idf.py flash
   ```
4. Monitor the ESP32 logs:
   ```sh
   idf.py monitor
   ```

Configuration
- Update `WIFI_SSID` and `WIFI_PASS` in the code to set your preferred Wi-Fi credentials.

How It Works
1. The ESP32 initializes as an Access Point.
2. It starts an HTTP server that listens for GET requests on `/alert`.
3. When an alert request is received, the ESP32:
   - Logs the alert
   - Waits 30 seconds
   - Activates the buzzer for 5 seconds
4. The buzzer then turns off automatically.

HTTP Endpoint
- URL: `http://<ESP32_AP_IP>/alert`
- Method: GET
- Response: `"Alert received"`

Logs and Debugging
Use the serial monitor to check logs:
```sh
idf.py monitor
```
Look for messages indicating Wi-Fi setup, alert reception, and buzzer activation.

License
This project is open-source and can be modified and distributed under the MIT License.



