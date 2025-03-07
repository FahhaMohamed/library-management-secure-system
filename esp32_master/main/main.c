#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_http_server.h"
#include "driver/gpio.h"

#define WIFI_SSID "ESP32_MASTER_AP"
#define WIFI_PASS "12345678"
#define BUZZER_PIN 27  // GPIO pin for buzzer

static const char *TAG = "MASTER";

// Function to initialize Wi-Fi as an Access Point
void wifi_init_softap() {
    ESP_LOGI(TAG, "Setting up ESP32 as Access Point...");
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .ssid_len = strlen(WIFI_SSID),
            .max_connection = 2,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
        },
    };

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    esp_wifi_start();
    ESP_LOGI(TAG, "ESP32 AP started. SSID: %s", WIFI_SSID);
}

// Function to turn on the buzzer after 30 seconds
void trigger_buzzer_task(void *pvParameter) {
    ESP_LOGI(TAG, "Motion detected! Waiting 30s before buzzing...");
    vTaskDelay(30000 / portTICK_PERIOD_MS);  // 30 seconds delay
    ESP_LOGI(TAG, "Buzzing now!");
    gpio_set_level(BUZZER_PIN, 1);
    vTaskDelay(5000 / portTICK_PERIOD_MS);  // Keep buzzer on for 5 sec
    gpio_set_level(BUZZER_PIN, 0);
    vTaskDelete(NULL);
}

// HTTP handler to receive alerts from Slave ESP32
esp_err_t alert_handler(httpd_req_t *req) {
    ESP_LOGI(TAG, "Alert received from Slave! Starting buzzer task.");
    xTaskCreate(&trigger_buzzer_task, "trigger_buzzer_task", 4096, NULL, 5, NULL);
    httpd_resp_send(req, "Alert received", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Function to start HTTP server
void start_webserver() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t alert_uri = { .uri = "/alert", .method = HTTP_GET, .handler = alert_handler };
        httpd_register_uri_handler(server, &alert_uri);
    }
}

// Main application
void app_main() {
    nvs_flash_init();
    esp_rom_gpio_pad_select_gpio(BUZZER_PIN);
    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);
    wifi_init_softap();
    start_webserver();
}
