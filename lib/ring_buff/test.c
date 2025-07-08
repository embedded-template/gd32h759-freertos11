#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ring_buff.h"

// 重定向FreeRTOS内存管理函数，用于测试
void *pvPortMalloc(size_t xSize) {
    return malloc(xSize);
}

void vPortFree(void *pv) {
    free(pv);
}

// 定义一个大小为16的环形缓冲区
RING_BUFF_PRE_INIT(test, 16)

RING_BUFF_PRE_INIT(test2, 16)

int main() {
    // 初始化环形缓冲区
    ring_buff_t* rb = test_ring_buff();
    if (rb == NULL) {
        printf("环形缓冲区创建失败！\n");
        return -1;
    }
    
    // 测试写入数据
    const uint8_t write_data[] = "Hello World!";
    int32_t write_len = rb->write(write_data, strlen((char*)write_data));
    printf("写入数据长度: %d\n", write_len);
    
    // 测试可用空间
    printf("剩余可用空间: %d\n", rb->available());
    printf("已使用空间: %d\n", rb->used());
    
    // 测试读取数据
    uint8_t read_buffer[16] = {0};
    int32_t read_len = rb->read(read_buffer, sizeof(read_buffer));
    printf("读取数据长度: %d\n", read_len);
    printf("读取的数据: %s\n", read_buffer);
    
    // 测试缓冲区状态
    printf("缓冲区是否为空: %s\n", rb->is_empty() ? "是" : "否");
    printf("缓冲区是否已满: %s\n", rb->is_full() ? "是" : "否");
    
    // 测试写满缓冲区
    const uint8_t fill_data[] = "0123456789ABCDEF";
    write_len = rb->write(fill_data, strlen((char*)fill_data));
    printf("尝试写满缓冲区，写入长度: %d\n", write_len);
    printf("缓冲区是否已满: %s\n", rb->is_full() ? "是" : "否");
    
    // 清空缓冲区
    rb->clear();
    printf("清空后，缓冲区是否为空: %s\n", rb->is_empty() ? "是" : "否");
    
    // 新增测试：写16个数据，读8个数据，再写16个数据
    printf("\n--- 环形缓冲区测试 ---\n");
    const uint8_t test_data_16[] = "1234567890123456"; // 16个字符
    const uint8_t test_data2_16[] = "ABCDEFGHIJKLMNOP"; // 16个字符
    
    // 1. 写入16个数据
    write_len = rb->write(test_data_16, 16);
    printf("第一次写入16个数据，实际写入: %d\n", write_len);
    printf("缓冲区状态 - 已使用: %d, 可用: %d, 是否已满: %s\n", 
           rb->used(), rb->available(), rb->is_full() ? "是" : "否");
    
    // 2. 读取8个数据
    uint8_t read_buffer_8[8] = {0};
    read_len = rb->read(read_buffer_8, 8);
    printf("读取8个数据，实际读取: %d\n", read_len);
    printf("读取的数据: ");
    for (int i = 0; i < read_len; i++) {
        printf("%c", read_buffer_8[i]);
    }
    printf("\n");
    printf("缓冲区状态 - 已使用: %d, 可用: %d\n", rb->used(), rb->available());
    
    // 3. 再写入16个数据
    write_len = rb->write(test_data2_16, 9);
    printf("写入9个数据，实际写入: %d\n", write_len);
    printf("缓冲区状态 - 已使用: %d, 可用: %d, 是否已满: %s\n", 
           rb->used(), rb->available(), rb->is_full() ? "是" : "否");
    
    write_len = rb->write(test_data2_16, 8);
    printf("写入8个数据，实际写入: %d\n", write_len);
    printf("缓冲区状态 - 已使用: %d, 可用: %d, 是否已满: %s\n", 
            rb->used(), rb->available(), rb->is_full() ? "是" : "否");
    
    // 读取剩余所有数据验证
    uint8_t final_buffer[16] = {0};
    read_len = rb->read(final_buffer, sizeof(final_buffer));
    printf("读取剩余数据，读取长度: %d\n", read_len);
    printf("剩余数据: ");
    for (int i = 0; i < read_len; i++) {
        printf("%c", final_buffer[i]);
    }
    printf("\n");
    
    
    ring_buff_t* rb2 = test2_ring_buff();  // 使用 test2_ring_buff() 而不是 test_ring_buff()
    if (rb2 == NULL) {
        printf("test2 环形缓冲区创建失败！\n");
        return -1;
    }
    printf("test2 环形缓冲区创建成功！\n");

    uint8_t read_buffer2[16] = {0};
    rb2->write("1234567890123456", 16);
    printf("test2 写入数据: %s\n", read_buffer2);
    rb2->read(read_buffer2, 8);
    printf("test2 读取数据: %s\n", read_buffer2);

    ring_buff_t* rb2_get = test2_ring_buff_get();
    if (rb2_get == NULL) {
        printf("test3 环形缓冲区创建失败！\n");
        return -1;
    }
    printf("test3 环形缓冲区创建成功！\n");
    
    rb2_get->read(read_buffer2, 8);
    printf("test3 读取数据: %s\n", read_buffer2);

    // 删除缓冲区
    rb->delete();
    printf("测试完成！\n");

    return 0;
}