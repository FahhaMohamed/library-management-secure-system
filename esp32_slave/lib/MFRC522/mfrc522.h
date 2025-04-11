#ifndef MFRC522_H
#define MFRC522_H

#include "driver/spi_master.h"

#define MFRC522_RST_PIN  GPIO_NUM_22   
#define MFRC522_SS_PIN   GPIO_NUM_5   

esp_err_t mfrc522_init(spi_device_handle_t spi, int reset_pin);

bool mfrc522_is_card_present(spi_device_handle_t spi);

bool mfrc522_read_card_serial(void);

esp_err_t mfrc522_get_uid(uint8_t** uid, uint8_t* uid_len);

void mfrc522_halt(void);

#endif 
