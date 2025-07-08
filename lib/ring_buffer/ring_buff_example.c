#include "ring_buff.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

// 使用宏定义创建一个名为uart的环形缓冲区，大小为256字节
RING_BUFF_PRE_INIT(uart, 256);

void writer_task(void *pvParameters)
{
    ring_buff_t *rb = uart_ring_buff();
    if (rb == NULL) {
        printf("Failed to create ring buffer\n");
        vTaskDelete(NULL);
        return;
    }

    uint8_t data[] = "Hello, FreeRTOS Ring Buffer!";
    
    while (1) {
        // 写入数据，超时时间为1000毫秒
        int32_t result = rb->write(data, sizeof(data)-1, 1000);
        
        vTaskDelay(pdMS_TO_TICKS(2000)); // 每2秒写入一次
    }
}

void reader_task(void *pvParameters)
{
    ring_buff_t *rb = uart_ring_buff();
    if (rb == NULL) {
        vTaskDelete(NULL);
        return;
    }

    uint8_t buffer[100];
    
    while (1) {
        // 读取数据，超时时间为500毫秒
        int32_t result = rb->read(buffer, sizeof(buffer)-1, 500);
        
        vTaskDelay(pdMS_TO_TICKS(1000)); // 每1秒读取一次
    }
}

void ring_buffer_example_init(void)
{
    // 创建写入任务
    xTaskCreate(writer_task, "Writer", 1024, NULL, 2, NULL);
    
    // 创建读取任务
    xTaskCreate(reader_task, "Reader", 1024, NULL, 2, NULL);
    
}