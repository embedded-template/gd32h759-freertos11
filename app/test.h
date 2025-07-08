#pragma once

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t * pTask_test;
void task_test(void *pvParameters);