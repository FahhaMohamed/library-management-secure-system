#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "mfrc522.h"

// PIR sensor pin
#define PIR_PIN 13

// SPI configuration parameters
#define DMA_CHAN 1

// Debug flag - set to 1 to enable debug output
#define DEBUG_ENABLED 1

// ----- Global Variables -----
static bool pirState = false;
static char lastUID[32] = "";
static spi_device_handle_t spi_global = NULL;

void debug_printf(const char* format, ...) {
    if (DEBUG_ENABLED) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

// ----- MFRC522 Setup -----
void mfrc522_setup(void) {
    debug_printf("Beginning MFRC522 setup...\n");
    
    spi_bus_config_t buscfg = {
        .miso_io_num = 19,
        .mosi_io_num = 23,
        .sclk_io_num = 18,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0,
    };
    
    debug_printf("SPI pins - MISO: %d, MOSI: %d, SCK: %d, SS: %d, RST: %d\n",
                19, 23, 18, MFRC522_SS_PIN, MFRC522_RST_PIN);

    // Initialize the SPI bus
    esp_err_t ret = spi_bus_initialize(HSPI_HOST, &buscfg, DMA_CHAN);
    debug_printf("SPI bus init result: %d (ESP_OK=%d)\n", ret, ESP_OK);
    
    if (ret != ESP_OK) {
        debug_printf("Failed to initialize SPI bus: %d\n", ret);
        return;
    }

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1000000, 
        .mode = 0,                 
        .spics_io_num = MFRC522_SS_PIN,
        .queue_size = 7,
    };

    ret = spi_bus_add_device(HSPI_HOST, &devcfg, &spi_global);
    debug_printf("SPI add device result: %d\n", ret);
    
    if (ret != ESP_OK) {
        debug_printf("Failed to add SPI device: %d\n", ret);
        return;
    }

    // Initialize the MFRC522 module
    debug_printf("Calling mfrc522_init with RST pin: %d\n", MFRC522_RST_PIN);
    ret = mfrc522_init(spi_global, MFRC522_RST_PIN);
    debug_printf("MFRC522 init result: %d\n", ret);
    
    if (ret != ESP_OK) {
        debug_printf("Failed to initialize MFRC522: %d\n", ret);
        return;
    }
    
    debug_printf("Testing MFRC522 communication by reading a register...\n");
    
    
    debug_printf("MFRC522 setup complete\n");
    
    gpio_set_direction(MFRC522_RST_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(MFRC522_RST_PIN, 0);  
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(MFRC522_RST_PIN, 1);  
    vTaskDelay(pdMS_TO_TICKS(50));      
    
    debug_printf("MFRC522 reset cycle completed\n");
}

// ----- RFID Reading Function -----
bool read_rfid_tag(char* tag, size_t max_len) {
    debug_printf("Attempting to detect RFID card...\n");
    
    // Check if a new card is present
    bool card_present = mfrc522_is_card_present(spi_global);
    debug_printf("Card present check result: %s\n", card_present ? "YES" : "NO");
    
    if (!card_present) {
        return false;
    }
    
    debug_printf("Card detected! Attempting to read UID...\n");
    
    uint8_t *uid = NULL;  
    uint8_t uid_len = 0;  
    
    debug_printf("Calling mfrc522_get_uid function...\n");
    
    esp_err_t ret = mfrc522_get_uid(&uid, &uid_len);
    
    debug_printf("mfrc522_get_uid result: %d (ESP_OK=%d)\n", ret, ESP_OK);
    debug_printf("uid pointer: %p, uid_len: %d\n", uid, uid_len);
    
    if (ret != ESP_OK || uid == NULL || uid_len == 0) {
        debug_printf("Failed to get UID: %d\n", ret);
        return false;
    }
    
    debug_printf("Card UID (raw): ");
    for (int i = 0; i < uid_len; i++) {
        debug_printf("%02X ", uid[i]);
    }
    debug_printf("\n");
    
    memset(tag, 0, max_len);
    for (int i = 0; i < uid_len && (i*2 + 2) <= max_len; i++) {
        char hex[3];
        sprintf(hex, "%02X", uid[i]);
        strcat(tag, hex);
    }
    
    debug_printf("Calling mfrc522_halt...\n");
    mfrc522_halt();
    
    debug_printf("Final RFID tag value: %s\n", tag);
    
    if (strcmp(tag, "12345678") == 0) {
        debug_printf("WARNING: Default value '12345678' detected! This suggests a reading problem.\n");
    }
    
    return true;
}

// Sends a message using ESP‑NOW.
void send_message(const char* msg) {
    esp_err_t res = esp_now_send(NULL, (const uint8_t*)msg, strlen(msg));
    if (res == ESP_OK) {
        debug_printf("Sent: %s\n", msg);
    } else {
        debug_printf("Error sending message: %d\n", res);
    }
}

void espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
    debug_printf("Send status: %s\n", (status == ESP_NOW_SEND_SUCCESS) ? "OK" : "FAIL");
}

// Initializes ESP‑NOW and adds the master peer.
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

    if (esp_now_init() != ESP_OK) {
        debug_printf("ESP‑NOW init failed\n");
        return;
    }
    esp_now_register_send_cb(espnow_send_cb);

    // Add the master as a peer.
    uint8_t masterAddress[] = { 0x94, 0x54, 0xC5, 0xB0, 0xA7, 0xAC };
    
    esp_now_peer_info_t peerInfo = {0};
    memcpy(peerInfo.peer_addr, masterAddress, 6);
    peerInfo.channel = 1;
    peerInfo.encrypt = false;
    
    ret = esp_now_add_peer(&peerInfo);
    if (ret != ESP_OK) {
        debug_printf("Failed to add peer: %d\n", ret);
    } else {
        debug_printf("Master peer added successfully\n");
    }
}

// ----- Slave Task -----
// sending corresponding ESP‑NOW messages.
void slave_task(void *pvParameter) {
    char tag[32] = {0};
    debug_printf("RFID reader task started\n");
    
    // Initial delay to ensure system stability
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    while (1) {
        // --- RFID Reading ---
        if (read_rfid_tag(tag, sizeof(tag))) {
            // Only send if we got a non-empty tag ID
            if (strlen(tag) > 0) {
                strcpy(lastUID, tag);
                char msg[64];
                snprintf(msg, sizeof(msg), "RFID:%s", tag);
                send_message(msg);
                debug_printf("Sent RFID: %s\n", tag);
                
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        }

        // --- PIR Sensor ---
        int motion = gpio_get_level(PIR_PIN);
        if (motion && !pirState) {
            pirState = true;
            send_message("PIR");
            debug_printf("Sent PIR\n");
        } else if (!motion && pirState) {
            pirState = false;
        }
        
        // Small delay before next check
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main(void) {
    printf("Starting RFID + PIR slave application\n");
    
    // Initialize PIR sensor GPIO.
    gpio_reset_pin(PIR_PIN);
    gpio_set_direction(PIR_PIN, GPIO_MODE_INPUT);
    printf("PIR sensor initialized on pin %d\n", PIR_PIN);

    // Setup the MFRC522 RFID reader over SPI.
    printf("Setting up MFRC522 module...\n");
    mfrc522_setup();

    // Initialize ESP‑NOW.
    printf("Initializing ESP-NOW...\n");
    init_espnow();
    printf("Slave ready\n");

    // Create the slave task.
    printf("Creating slave task...\n");
    xTaskCreate(slave_task, "slave_task", 4096, NULL, 5, NULL);
    printf("Slave task created\n");
}