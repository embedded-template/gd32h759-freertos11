#pragma once

#include "FreeRTOS.h"
#include "task.h"
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


