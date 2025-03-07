#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_http_client.h"
#include "driver/gpio.h"
#include "rc522.h"  

#define WIFI_SSID "ESP32_MASTER_AP"
#define WIFI_PASS "12345678"
#define PIR_SENSOR_PIN 13   
#define MASTER_IP "192.168.4.1" 
#define SERVER_URL "http://localhost:5000/store_rfid.php" 

static const char *TAG = "SLAVE";

// Initialize WiFi connection
void wifi_init_sta() {
    ESP_LOGI(TAG, "Connecting to Master AP...");
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
    ESP_LOGI(TAG, "Connected to Master ESP32 AP.");
}

// Function to send RFID tag data to PHP server
void send_rfid_data(const char *rfid_tag) {
    ESP_LOGI(TAG, "Sending RFID tag: %s", rfid_tag);

    char post_data[100];
    snprintf(post_data, sizeof(post_data), "tag_id=%s", rfid_tag);

    esp_http_client_config_t config = {
        .url = SERVER_URL,
        .method = HTTP_METHOD_POST,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}

// RFID reading task
void rfid_task(void *pvParameter) {
    rc522_start();  

    while (1) {
        char tag[20];
        if (rc522_read_tag(tag)) { 
            ESP_LOGI(TAG, "RFID Detected: %s", tag);
            send_rfid_data(tag);
            vTaskDelay(5000 / portTICK_PERIOD_MS); 
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}


void app_main() {
    nvs_flash_init();
    wifi_init_sta();
    rc522_init();  
    xTaskCreate(&rfid_task, "rfid_task", 4096, NULL, 5, NULL);
}
