#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t * pTask_test;

void task_test(void *pvParameters)
{
    uint32_t count = 0;
    while (1)
    {
        count++;
        vTaskDelay(pdMS_TO_TICKS(*((uint32_t*)pvParameters)));
    }
}