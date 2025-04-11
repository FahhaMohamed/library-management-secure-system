#include "mfrc522.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"


// Initialize the MFRC522.
esp_err_t mfrc522_init(spi_device_handle_t spi, int reset_pin) {
    gpio_set_direction(reset_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(reset_pin, 0);  // Apply reset.
    vTaskDelay(pdMS_TO_TICKS(50));
    gpio_set_level(reset_pin, 1);  // Release reset.
    return ESP_OK;
}

bool mfrc522_is_card_present(spi_device_handle_t spi) {
    return true;  
}

// Read the card serial number
bool mfrc522_read_card_serial(void) {
    return true; 
}

// Get the UID of the card
esp_err_t mfrc522_get_uid(uint8_t** uid, uint8_t* uid_len) {
    static uint8_t dummy_uid[4] = {0x12, 0x34, 0x56, 0x78};  
    *uid = dummy_uid;
    *uid_len = 4;
    return ESP_OK;
}

// Halt communication with the card.
void mfrc522_halt(void) {
    // Send the halt command over SPI to the MFRC522.
}
