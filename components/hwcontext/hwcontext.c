#include "hwcontext.h"
#include "esp32hwcontext.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "nvs.h"

#include <string.h>

#define READ_TIMEOUT (1000 / portTICK_PERIOD_MS)
#define BYTES_POLLING_INTERVAL (10 / portTICK_PERIOD_MS)
#define MSG_DELIM '$'
#define KEY_MSG_LENGTH 3

#define PRESSED_IDX 0
#define KEY_IDX 1
#define DELIM_IDX 2

#define NVS_NS "ihcbadge"

static char tag[] = "hwcontext";

char hwcontext_get_key_code(void *hwcontext, int *pressed, struct timespec *timestamp)
{
    size_t bytes_available;
    uint8_t buf[KEY_MSG_LENGTH];

    while (1) {
        memset(buf, 0, sizeof(buf));

        // Keep reading until we receive a $
        while (uart_read_bytes(UART_NUM_0, buf, 1, READ_TIMEOUT) <= 0);

        if (buf[0] != MSG_DELIM) {
            continue;
        }

        bytes_available = 0;

        // Keep polling the available bytes until we have a complete key message
        while (1) {
            uart_get_buffered_data_len(UART_NUM_0, &bytes_available);
            if (bytes_available >= KEY_MSG_LENGTH) {
                break;
            }
            vTaskDelay(BYTES_POLLING_INTERVAL);
        }

        // Read the actual key message
        uart_read_bytes(UART_NUM_0, buf, KEY_MSG_LENGTH, READ_TIMEOUT);

        if (buf[2] != MSG_DELIM) {
            // Invalid message
            continue;
        }

        ESP_LOGI(tag, "Received key %c pressed status %c", buf[PRESSED_IDX], buf[KEY_IDX]);

        TickType_t ticks = xTaskGetTickCount();
        timestamp->tv_sec = (ticks * portTICK_PERIOD_MS) / 1000;
        timestamp->tv_nsec = ((ticks * portTICK_PERIOD_MS) % 1000) * 1000000;

        switch (buf[PRESSED_IDX]) {
            case 'D':
                *pressed = 1;
                break;

            case 'U':
                *pressed = 0;
                break;

            default:
                // Invalid
                continue;
        }

        switch (buf[KEY_IDX]) {
            // Valid keys
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
            case '*':
            case '#':
            case 'U':
            case 'D':
            case 'C':
            case 'M':
                return buf[1];
            // Otherwise, we cycle again
        }
    }
}

uint8_t *hwcontext_get_framebuffer(void *hw_context)
{
    struct HWContext *hw = (struct HWContext *) hw_context;
    return u8g2_GetBufferPtr(hw->u8g2);
}

void hwcontext_update_screen(void *hw_context)
{
    struct HWContext *hw = (struct HWContext *) hw_context;
    u8g2_SendBuffer(hw->u8g2);
}

void hwcontext_delay_ms(void *hw_context, int mseconds){
    vTaskDelay(mseconds / portTICK_PERIOD_MS);
}

int hwcontext_set_nv_string(void *hwcontext, const char *key, const char *value)
{
    nvs_handle handle;
    esp_err_t err = nvs_open(NVS_NS, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle\n", esp_err_to_name(err));
        return -1;
    }

    err = nvs_set_str(handle, key, value);
    if (err != ESP_OK) {
        printf("Error (%s) writing NVS string\n", esp_err_to_name(err));
        return -1;
    }

    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing NVS string\n", esp_err_to_name(err));
        return -1;
    }

	return 0;
}

char *hwcontext_get_nv_string(void *hwcontext, const char *key)
{
    nvs_handle handle;
    esp_err_t err = nvs_open(NVS_NS, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle\n", esp_err_to_name(err));
        return NULL;
    }

    size_t required_length;
    err = nvs_get_str(handle, key, NULL, &required_length);
    if (err != ESP_OK) {
        printf("Error (%s) reading NVS string length\n", esp_err_to_name(err));
        return NULL;
    }

    char *ret = malloc(sizeof(char) * required_length);
    if (!ret) {
        printf("Memory error reading NVS string\n");
        return NULL;
    }

    err = nvs_get_str(handle, key, ret, &required_length);
    if (err != ESP_OK) {
        printf("Error (%s) reading NVS string length\n", esp_err_to_name(err));
        return NULL;
    }

    return ret;
}
