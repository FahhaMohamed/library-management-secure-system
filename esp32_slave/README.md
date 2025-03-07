ESP32 Slave with RFID and Motion Detection

Overview
This project configures an ESP32 as a Wi-Fi Station (STA) that connects to a master ESP32 Access Point (AP). The slave ESP32 reads RFID tags and sends the tag data to a PHP server via HTTP POST requests.

Features
- Connects to an ESP32 AP
- Reads RFID tag data using the RC522 module
- Sends RFID tag data to a PHP server

Components Required
- ESP32 Development Board
- RC522 RFID Module
- PIR Motion Sensor (Optional)
- Power Supply

Wiring
| ESP32 GPIO | Component |
|------------|----------|
| GPIO 13    | PIR Sensor (Optional) |
| SPI Pins   | RC522 RFID Module |

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
- Update `WIFI_SSID` and `WIFI_PASS` to match the master ESP32 AP.
- Update `SERVER_URL` with the actual PHP server endpoint.

How It Works
1. The ESP32 connects to the master ESP32 AP.
2. It continuously reads RFID tags using the RC522 module.
3. When a tag is detected, the ESP32 sends the tag ID to the PHP server using an HTTP POST request.
4. The data is stored on the server for further processing.

Server Communication
- URL: `http://localhost:5000/store_rfid.php`
- Method: POST
- Data Format: `tag_id=<RFID_TAG>`

Logs and Debugging
Use the serial monitor to check logs:
```sh
idf.py monitor
```
Look for messages indicating Wi-Fi connection status, RFID tag detection, and HTTP request results.

License
This project is open-source and can be modified and distributed under the MIT License.


