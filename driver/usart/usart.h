#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "gd32h7xx.h"
#include "ring_buffer.h"
#include "stream_buffer.h"


extern TaskHandle_t * pTask_debug;

void Task_UART1_Debug(void *pvParameters);