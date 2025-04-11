#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>
#include "esp_err.h"

#define SSD1306_I2C_ADDR      0x3C
#define SSD1306_WIDTH         128
#define SSD1306_HEIGHT        64
#define SSD1306_BUFFER_SIZE   (SSD1306_WIDTH * SSD1306_HEIGHT / 8)

/**
 * @brief Initialize the SSD1306 display.
 *
 * This function sends the required initialization commands to the SSD1306.
 *
 * @return ESP_OK on success, or an error code from the I2C routines.
 */
esp_err_t ssd1306_init(void);

/**
 * @brief Clear the internal screen buffer.
 *
 * Call ssd1306_update_screen() to update the display.
 */
void ssd1306_clear_screen(void);

/**
 * @brief Write a null-terminated text string into the internal buffer at a given row.
 *
 * Each row corresponds to one page (8 pixels tall). The text is rendered using
 * a fixed 5×7 pixel font with 1-pixel spacing. Only characters in the range 0x20–0x7F are supported.
 *
 * @param text The null‑terminated text string.
 * @param row The row (page index, 0–7) at which to render the text.
 */
void ssd1306_display_text(const char* text, uint8_t row);

/**
 * @brief Send the internal buffer to the display.
 *
 * @return ESP_OK on success, or an error code from the I2C routines.
 */
esp_err_t ssd1306_update_screen(void);

#endif // SSD1306_H
