#include "PDL_Diagnostic_Print.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Adafruit_TinyUSB.h" // Serial communication for SEEED xiao ble

static QueueHandle_t xQueue = NULL;
static SemaphoreHandle_t xMutex = NULL;
static TaskHandle_t printTaskHandle = NULL;

#define SERIAL_BAUD_RATE 115200
#define QUEUE_LENGTH 10
#define QUEUE_ITEM_SIZE sizeof(char *)
#define TASK_STACK_SIZE 2048
#define TASK_PRIORITY 1

static void default_init_func()
{
    // Initialize the serial port at 115200 bps
    Serial.begin(SERIAL_BAUD_RATE);
}

static void default_deinit_func()
{
    // Deinitialize the serial port
    Serial.end();
}

static void default_print_func(const char *message)
{
    // Print to the serial port
    Serial.print(message);
}

static void_func_t current_init_func = default_init_func;
static void_func_t current_deinit_func = default_deinit_func;
static print_func_t current_print_func = default_print_func;

bool _takeMutex()
{
    if (xMutex == NULL)
    {
        xMutex = xSemaphoreCreateMutex();
        if (xMutex == NULL)
        {
            return false;
        }
    }

    return xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE;
}

bool _giveMutex()
{
    if (xMutex == NULL)
    {
        xMutex = xSemaphoreCreateMutex();
        if (xMutex == NULL)
        {
            return false;
        }
    }

    return xSemaphoreGive(xMutex) == pdTRUE;
}

void _diagnostic_print_task(void *pvParameters)
{
    char *buffer;

    while (1)
    {
        if (xQueueReceive(xQueue, &buffer, portMAX_DELAY) == pdPASS)
        {
            xSemaphoreTake(xMutex, portMAX_DELAY);
            current_print_func(buffer);
            xSemaphoreGive(xMutex);
            free(buffer);
        }
    }

    xSemaphoreGive(xMutex);
    vTaskDelete(NULL);
}

bool diagnostic_print_set_init_func(void_func_t func)
{
    if (_takeMutex() == false)
    {
        return false;
    }

    if (func)
    {
        current_init_func = func;
    }
    else
    {
        current_init_func = default_init_func;
    }

    _giveMutex();
    return true;
}

bool diagnostic_print_set_deinit_func(void_func_t func)
{

    if (_takeMutex() == false)
    {
        return false;
    }

    if (func)
    {
        current_deinit_func = func;
    }
    else
    {
        current_deinit_func = default_deinit_func;
    }

    _giveMutex();
    return true;
}

bool diagnostic_print_set_print_func(print_func_t func)
{
    if (_takeMutex() == false)
    {
        return false;
    }

    if (func)
    {
        current_print_func = func;
    }
    else
    {
        current_print_func = default_print_func;
    }
    _giveMutex();
    return true;
}

bool diagnostic_print_init()
{
    if (xMutex != NULL && xQueue != NULL && printTaskHandle != NULL)
    {
        return true;
    }

    if (_takeMutex() == false) // will generate a mutex if it does not exist
    {
        return false;
    }

    if (!current_init_func || !current_deinit_func || !current_print_func)
    {
        current_init_func = default_init_func;
        current_deinit_func = default_deinit_func;
        current_print_func = default_print_func;
    }

    current_init_func();

    if (xQueue == NULL)
    {
        xQueue = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
        if (xQueue == NULL)
        {
            _giveMutex();
            return false;
        }
    }

    if (printTaskHandle == NULL)
    {
        if (xTaskCreate(_diagnostic_print_task, "DiagPrint", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &printTaskHandle) != pdPASS)
        {
            _giveMutex();
            return false;
        }
    }

    _giveMutex();
    return true;
}

void diagnostic_print_deinit()
{

    if (printTaskHandle != NULL)
    {
        vTaskDelete(printTaskHandle);
        printTaskHandle = NULL;
    }

    if (xQueue != NULL)
    {
        vQueueDelete(xQueue);
        xQueue = NULL;
    }

    if (xMutex != NULL)
    {
        vSemaphoreDelete(xMutex);
        xMutex = NULL;
    }

    if (current_deinit_func)
    {
        current_deinit_func();
    }

}

bool diagnostic_printf(const char *format, ...)
{
    // check status of the module
    if (xQueue == NULL || xMutex == NULL || printTaskHandle == NULL)
    {
        return false;
    }

    va_list args;
    va_start(args, format);
    // Calculate the required buffer size
    int len = vsnprintf(NULL, 0, format, args) + 1;
    va_end(args);

    if (len <= 0)
    {
        return false;
    }

    char *buffer = (char *)malloc(len);
    if (buffer == NULL)
    {
        // Handle memory allocation error
        return false;
    }

    va_start(args, format);
    vsnprintf(buffer, len, format, args);
    va_end(args);

    // Send the pointer to the buffer to the queue
    if (xQueueSend(xQueue, &buffer, portMAX_DELAY) != pdPASS)
    {
        // Handle error
        free(buffer);
        return false;
    }

    return true;
}

bool diagnostic_println(const char *message)
{
    if (xQueue == NULL || xMutex == NULL || printTaskHandle == NULL)
    {
        return false;
    }

    int len = strlen(message) + 2; // Add 2 for newline and null terminator
    char *buffer = (char *)malloc(len);
    if (buffer == NULL)
    {
        return false;
    }

    snprintf(buffer, len, "%s\n", message);
    if (xQueueSend(xQueue, &buffer, portMAX_DELAY) != pdPASS)
    {
        free(buffer);
        return false;
    }

    return true;
}
