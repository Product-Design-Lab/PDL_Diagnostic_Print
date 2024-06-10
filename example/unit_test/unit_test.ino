#include <Arduino.h>
#include "PDL_Diagnostic_Print.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include <string.h>

#define TASK_STACK_SIZE 1024
#define TASK_PRIORITY 1

// Function prototypes for tasks
void task_1(void *pvParameters);
void task_2(void *pvParameters);
void task_3(void *pvParameters);

// Task handles
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;
TaskHandle_t task3Handle = NULL;

EventGroupHandle_t eventGroup;

void print_to_capital(const char *message)
{
    int len = strlen(message);
    for (int i = 0; i < len; i++)
    {
        if (message[i] >= 'a' && message[i] <= 'z')
        {
            Serial.write(message[i] - 32);
        }
        else
        {
            Serial.write(message[i]);
        }
    }
}

// Function to create tasks
void create_tasks()
{
    xTaskCreate(task_1, "PrintTask1", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &task1Handle);
    xTaskCreate(task_2, "PrintTask2", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &task2Handle);
    xTaskCreate(task_3, "PrintTask3", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &task3Handle);
}

// Function to delete tasks
void delete_tasks()
{
    if (task1Handle != NULL)
    {
        vTaskDelete(task1Handle);
        task1Handle = NULL;
    }
    if (task2Handle != NULL)
    {
        vTaskDelete(task2Handle);
        task2Handle = NULL;
    }
    if (task3Handle != NULL)
    {
        vTaskDelete(task3Handle);
        task3Handle = NULL;
    }
}

void task_1(void *pvParameters)
{
    while (true)
    {
        // wait on event to print
        xEventGroupWaitBits(eventGroup, 0x01, pdFALSE, pdFALSE, portMAX_DELAY);
        diagnostic_printf(" Task_1");
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void task_2(void *pvParameters)
{
    while (true)
    {
        // wait on event to print
        xEventGroupWaitBits(eventGroup, 0x01, pdFALSE, pdFALSE, portMAX_DELAY);
        diagnostic_printf(" Task_2");
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void task_3(void *pvParameters)
{
    while (true)
    {
        // wait on event to print
        xEventGroupWaitBits(eventGroup, 0x01, pdFALSE, pdFALSE, portMAX_DELAY);
        diagnostic_printf(" Task_3");
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void setup()
{
    eventGroup = xEventGroupCreate();
    xEventGroupSetBits(eventGroup, 0x01);
    diagnostic_print_init();
    create_tasks();
    delay(1500);

    diagnostic_println("\n*********** Unit Test Start ***********\n");

    for (int i = 0; i< 15; i++)
    {
        if (i == 10)
        {
            // set print function to print_to_capital
            diagnostic_print_set_print_func(nullptr);
        }
        else if (i == 5)
        {
            // set print function to print_to_capital
            diagnostic_print_set_print_func(print_to_capital);
        }

        diagnostic_printf("\n\ncycle %d\n", i);

        xEventGroupSetBits(eventGroup, 0x01); // start printing
        vTaskDelay(pdMS_TO_TICKS(100));

        xEventGroupClearBits(eventGroup, 0x01); // stop printing
        vTaskDelay(pdMS_TO_TICKS(900));
    }

    diagnostic_printf("\n*********** Unit Test End ***********\n");

    delay(1000);

    delete_tasks();
    diagnostic_print_deinit();

    while (1)
        ;
}

void loop()
{
}
