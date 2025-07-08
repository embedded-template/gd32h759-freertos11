#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"

#include "test.h"

typedef struct task_info_t
{
    TaskFunction_t pxTaskCode;
    const char * const pcName;
    const configSTACK_DEPTH_TYPE uxStackDepth;
    uint32_t time;
    UBaseType_t uxPriority;
    TaskHandle_t * const pxCreatedTask;
} task_info_t;

#define TASK_name_test "test"
#define TASK_stack_depth_test 100
#define TASK_param_test 100
#define TASK_priority_test 3

#define TASK_name_uart1 "debug"
#define TASK_stack_depth_uart1 100
#define TASK_param_uart1 100
#define TASK_priority_uart1 3

