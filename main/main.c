#include <esp_log.h>
#include "driver/uart.h"
#include "esp32hwcontext.h"
#include "shell.h"
#include "u8g2_esp32_hal.h"
#include "nvs_flash.h"

#define PIN_CLK 19
#define PIN_MOSI 23
#define PIN_RESET 33
#define PIN_DC 5
#define PIN_CS 32

#define UART_BUF_SIZE 1024

static void init_display(struct HWContext *hw_context) {
    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
    u8g2_esp32_hal.clk   = PIN_CLK;
    u8g2_esp32_hal.mosi  = PIN_MOSI;
    u8g2_esp32_hal.cs    = PIN_CS;
    u8g2_esp32_hal.dc    = PIN_DC;
    u8g2_esp32_hal.reset = PIN_RESET;
    u8g2_esp32_hal_init(u8g2_esp32_hal);

    u8g2_t *u8g2 = malloc(sizeof(u8g2_t));;
    u8g2_Setup_pcd8544_84x48_f(
        u8g2,
        U8G2_R0,
        u8g2_esp32_spi_byte_cb,
        u8g2_esp32_gpio_and_delay_cb);  // init u8g2 structure

    u8g2_InitDisplay(u8g2); // send init sequence to the display, display is in sleep mode after this,

    u8g2_SetPowerSave(u8g2, 0); // wake up display
    u8g2_SetFont(u8g2, u8g2_font_t0_11_t_all);
    u8g2_ClearBuffer(u8g2);

    hw_context->u8g2 = u8g2;
}

static void init_serial()
{
    const int uart_num = UART_NUM_0;
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, UART_BUF_SIZE * 2, 0, 0, NULL, 0));
}

void init_nvs()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
}

void app_main()
{
    const char tag[] = "app_main";

    ESP_LOGI(tag, "*Nokia tune intensifies*");

    struct HWContext *hw_context = malloc(sizeof(struct HWContext));
    init_display(hw_context);
    init_serial();
    init_nvs();

    xTaskCreate(shell_main, "shell_main", 8192, hw_context, 5, NULL);
}
