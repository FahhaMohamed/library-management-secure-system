#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/i2c.h"

#include "ssd1306.h"  

// GPIO pins
#define BUZZER_PIN 25
#define LED_PIN    26

#define I2C_MASTER_SCL_IO    22
#define I2C_MASTER_SDA_IO    21
#define I2C_MASTER_NUM       I2C_NUM_0
#define I2C_MASTER_FREQ_HZ   100000

#define THRESHOLD_MS 3000

// ----- Global State Variables -----
static char currentTag[64] = "";
static bool eventActive = false;
static bool alarmActive = false;
static bool pirDetected = false;
static uint32_t startTime = 0;

// ----- OLED Display Helper Functions -----
void oled_init(void) {
    // Initialize I2C bus for OLED display
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

    // Initialize the SSD1306 display (the library function may vary)
    ssd1306_init();
    ssd1306_clear_screen();
    ssd1306_display_text("System Starting...", 0);
    ssd1306_update_screen();
}

// Display four lines on the screen (adjust the number of rows as supported)
void display_event_info(const char *line0, const char *line1, const char *line2, const char *line3) {
    ssd1306_clear_screen();
    ssd1306_display_text(line0, 0);
    ssd1306_display_text(line1, 1);
    ssd1306_display_text(line2, 2);
    ssd1306_display_text(line3, 3);
    ssd1306_update_screen();
}

// ----- ESP‑NOW Receive Callback -----
static void espnow_recv_cb(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
    char mac_str[18];
    snprintf(mac_str, sizeof(mac_str), "%02X:%02X:%02X:%02X:%02X:%02X",
             recv_info->src_addr[0], recv_info->src_addr[1], recv_info->src_addr[2],
             recv_info->src_addr[3], recv_info->src_addr[4], recv_info->src_addr[5]);

    char msg[128] = {0};
    memcpy(msg, data, len);
    msg[len] = '\0';
    printf("Master got from %s: %s\n", mac_str, msg);

    // Check if the message is an RFID tag
    if (strncmp(msg, "RFID:", 5) == 0) {
        char tag[64] = {0};
        strncpy(tag, msg + 5, sizeof(tag) - 1);

        if (!eventActive) {
            // Start new event
            strncpy(currentTag, tag, sizeof(currentTag) - 1);
            eventActive = true;
            alarmActive = false;
            pirDetected = false;
            startTime = xTaskGetTickCount() * portTICK_PERIOD_MS;

            // Display event info
            display_event_info("Scanning Tag:", tag, "", "Waiting for motion...");
            printf("Event started for %s\n", tag);
        } else if (eventActive && strcmp(tag, currentTag) == 0) {
            // RFID received again – event complete
            eventActive = false;
            pirDetected = false;
            if (alarmActive) {
                alarmActive = false;
                gpio_set_level(BUZZER_PIN, 0);
                gpio_set_level(LED_PIN, 0);
                display_event_info("Returned (alarm off):", tag, "", "");
                printf("Alarm cancelled for %s\n", tag);
            } else {
                display_event_info("Returned in time:", tag, "", "");
                printf("Returned in time: %s\n", tag);
            }
        } else {
            printf("Ignoring different tag during event\n");
        }
    }
    // Check if the message is from the PIR sensor
    else if (strcmp(msg, "PIR") == 0) {
        pirDetected = true;
    }
}


// ----- Master Task -----
void master_task(void *pvParameter) {
    while (1) {
        if (eventActive && pirDetected && !alarmActive) {
            uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;
            if ((now - startTime) >= THRESHOLD_MS) {
                alarmActive = true;
                gpio_set_level(BUZZER_PIN, 1);
                gpio_set_level(LED_PIN, 1);

                char line0[] = "!!! ALERT !!!";
                char line1[] = "Motion detected!";
                char line2[128];
                snprintf(line2, sizeof(line2), "Tag: %.58s", currentTag);

                display_event_info(line0, line1, line2, "");
                printf("Alarm ON for %s\n", currentTag);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// ----- ESP‑NOW Initialization -----
void init_espnow(void) {
    esp_err_t ret = nvs_flash_init();
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();

    if(esp_now_init() != ESP_OK) {
        printf("ESP-NOW init failed\n");
        return;
    }
    esp_now_register_recv_cb(espnow_recv_cb);
}

void app_main(void) {
    // Initialize GPIO for buzzer and LED
    gpio_reset_pin(BUZZER_PIN);
    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(BUZZER_PIN, 0);

    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);

    // Initialize OLED display
    oled_init();

    // Initialize ESP‑NOW
    init_espnow();
    printf("Master ready\n");

    // Create the master task
    xTaskCreate(master_task, "master_task", 4096, NULL, 5, NULL);
}
