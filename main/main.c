#include <esp_log.h>
#include "u8g2_esp32_hal.h"

#define PIN_CLK 19
#define PIN_MOSI 23
#define PIN_RESET 33
#define PIN_DC 5
#define PIN_CS 32

void display_hello() {
    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
    u8g2_esp32_hal.clk   = PIN_CLK;
    u8g2_esp32_hal.mosi  = PIN_MOSI;
    u8g2_esp32_hal.cs    = PIN_CS;
    u8g2_esp32_hal.dc    = PIN_DC;
    u8g2_esp32_hal.reset = PIN_RESET;
    u8g2_esp32_hal_init(u8g2_esp32_hal);

    u8g2_t u8g2;
    u8g2_Setup_pcd8544_84x48_f(
        &u8g2,
        U8G2_R0,
        u8g2_esp32_spi_byte_cb,
        u8g2_esp32_gpio_and_delay_cb);  // init u8g2 structure

    u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,

    u8g2_SetPowerSave(&u8g2, 0); // wake up display
    u8g2_SetFont(&u8g2, u8g2_font_t0_11_t_all);
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawStr(&u8g2, 0, 8, "Hello");
    u8g2_SendBuffer(&u8g2);
}

void app_main()
{
    const char tag[] = "app_main";

    ESP_LOGI(tag, "*Nokia tune intensifies*");
    display_hello();
}
