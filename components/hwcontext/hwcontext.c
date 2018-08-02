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

#define CMD_HEADER "&&"

char hwcontext_get_key_code(void *hwcontext, int *pressed, struct timespec *timestamp)
{
    return hwcontext_nb_get_key_code(hwcontext, pressed, timestamp, -1);
}

int hwcontext_nb_get_key_code(void *hwcontext, int *pressed, struct timespec *timestamp, int timeoutms)
{
    struct HWContext *hw = (struct HWContext *) hwcontext;
    struct KeyEvent ev;
    TickType_t ticksToWait;
    if (timeoutms < 0) {
        ticksToWait = portMAX_DELAY;
    } else {
        ticksToWait = timeoutms / portTICK_PERIOD_MS;
    }
    if (xQueueReceive(hw->key_events_queue, (void *)&ev, ticksToWait) == pdFALSE) {
        return -1;
    } else {
        *pressed = ev.pressed;
        *timestamp = ev.timestamp;
        return ev.key;
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

static char *default_if_not_null(const char *default_value)
{
    if (default_value) {
        return strdup(default_value);
    } else {
        return NULL;
    }
}

char *hwcontext_get_nv_string(void *hwcontext, const char *key, const char *default_value)
{
    nvs_handle handle;
    esp_err_t err = nvs_open(NVS_NS, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle\n", esp_err_to_name(err));
        return default_if_not_null(default_value);
    }

    size_t required_length;
    err = nvs_get_str(handle, key, NULL, &required_length);
    if (err != ESP_OK) {
        printf("Error (%s) reading NVS string length\n", esp_err_to_name(err));
        return default_if_not_null(default_value);
    }

    char *ret = malloc(sizeof(char) * required_length);
    if (!ret) {
        printf("Memory error reading NVS string\n");
        return default_if_not_null(default_value);
    }

    err = nvs_get_str(handle, key, ret, &required_length);
    if (err != ESP_OK) {
        printf("Error (%s) reading NVS string length\n", esp_err_to_name(err));
        return default_if_not_null(default_value);
    }

    return ret;
}

void hwcontext_send_command(void *hwcontext, const char *command, const char *command_args)
{
    uart_write_bytes(UART_NUM_0, CMD_HEADER, strlen(CMD_HEADER));
    uart_write_bytes(UART_NUM_0, command, strlen(command));
    uart_write_bytes(UART_NUM_0, command_args, strlen(command_args));
}
