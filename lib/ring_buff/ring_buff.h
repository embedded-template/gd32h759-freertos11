/**
 * @file ring_buff.h
 * @brief 环形缓冲区库
 * @author 
 * @date 
 * @version 1.0
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief 环形缓冲区结构体前向声明
 */
struct ring_buff_t;

/**
 * @brief 环形缓冲区写入函数指针类型
 * @param data 要写入的数据指针
 * @param len 要写入的数据长度
 * @return 实际写入的字节数，失败返回负数
 */
typedef int32_t (*ring_buff_write_func_t)(const uint8_t *data, uint16_t len);

/**
 * @brief 环形缓冲区读取函数指针类型
 * @param data 读取数据的缓冲区指针
 * @param len 要读取的数据长度
 * @return 实际读取的字节数
 */
typedef int32_t (*ring_buff_read_func_t)(uint8_t *data, uint16_t len);

/**
 * @brief 获取可用空间函数指针类型
 * @return 可用空间大小
 */
typedef uint16_t (*ring_buff_available_func_t)(void);

/**
 * @brief 获取已使用空间函数指针类型
 * @return 已使用空间大小
 */
typedef uint16_t (*ring_buff_used_func_t)(void);

/**
 * @brief 检查是否为空函数指针类型
 * @return true表示为空，false表示非空
 */
typedef bool (*ring_buff_is_empty_func_t)(void);

/**
 * @brief 检查是否已满函数指针类型
 * @return true表示已满，false表示未满
 */
typedef bool (*ring_buff_is_full_func_t)(void);

/**
 * @brief 清空缓冲区函数指针类型
 */
typedef void (*ring_buff_clear_func_t)(void);

/**
 * @brief 删除缓冲区函数指针类型
 */
typedef void (*ring_buff_delete_func_t)(void);

/**
 * @brief 环形缓冲区结构体
 */
typedef struct ring_buff_t
{
    uint8_t *p_buff;                    /**< 缓冲区指针 */
    uint16_t size;                      /**< 缓冲区大小 */
    uint16_t head;                      /**< 头指针 */
    uint16_t tail;                      /**< 尾指针 */
    bool is_full_flag;                  /**< 满标志位 */
    ring_buff_write_func_t write;       /**< 写入函数指针 */
    ring_buff_read_func_t read;         /**< 读取函数指针 */
    ring_buff_available_func_t available; /**< 获取可用空间函数指针 */
    ring_buff_used_func_t used;         /**< 获取已使用空间函数指针 */
    ring_buff_is_empty_func_t is_empty; /**< 检查是否为空函数指针 */
    ring_buff_is_full_func_t is_full;   /**< 检查是否已满函数指针 */
    ring_buff_clear_func_t clear;       /**< 清空缓冲区函数指针 */
    ring_buff_delete_func_t delete;     /**< 删除缓冲区函数指针 */
} ring_buff_t;

/**
 * @brief 创建基础环形缓冲区的宏
 * @param size 缓冲区大小
 * @return 成功返回环形缓冲区指针，失败返回NULL
 */
#define RING_BUFF_NEW_BASE(size) ({ \
    ring_buff_t *rb = NULL; \
    do { \
        if (size == 0) { \
            break; \
        } \
        rb = (ring_buff_t*)malloc(sizeof(ring_buff_t)); \
        if (rb == NULL) { \
            break; \
        } \
        rb->p_buff = (uint8_t*)malloc(size); \
        if (rb->p_buff == NULL) { \
            free(rb); \
            rb = NULL; \
            break; \
        } \
        rb->size = size; \
        rb->head = 0; \
        rb->tail = 0; \
        rb->is_full_flag = false; \
    } while(0); \
    rb; \
})

/**
 * @brief 声明外部环形缓冲区的宏
 * @param name 环形缓冲区名称
 */
#define RING_BUFF_EXTERN(name) \
    ring_buff_t* name##_ring_buff_get(void);

/**
 * @brief 预初始化环形缓冲区的宏，定义所有相关函数
 * @param name 环形缓冲区名称
 * @param buff_size 缓冲区大小
 */
#define RING_BUFF_PRE_INIT(name, buff_size) \
    static ring_buff_t name##_ring_buff_instance; \
    \
    /** @brief 写入数据函数 */ \
    static int32_t name##_write(const uint8_t *data, uint16_t len) { \
        ring_buff_t *rb = &name##_ring_buff_instance; \
        if (rb == NULL || data == NULL || len == 0) { \
            return -1; \
        } \
        if (rb->p_buff == NULL) { \
            return -1; \
        } \
        /* 计算可用空间 */ \
        uint16_t available; \
        if (rb->is_full_flag) { \
            available = 0; \
        } else if (rb->head >= rb->tail) { \
            available = rb->size - (rb->head - rb->tail); \
        } else { \
            available = rb->tail - rb->head; \
        } \
        if (available < len) { \
            return -1; \
        } \
        for (uint16_t i = 0; i < len; i++) { \
            rb->p_buff[rb->head] = data[i]; \
            rb->head = (rb->head + 1) % rb->size; \
            if (rb->head == rb->tail) { \
                rb->is_full_flag = true; \
            } \
        } \
        return len; \
    } \
    \
    /** @brief 读取数据函数 */ \
    static int32_t name##_read(uint8_t *data, uint16_t len) { \
        ring_buff_t *rb = &name##_ring_buff_instance; \
        if (rb == NULL || data == NULL || len == 0) { \
            return 0; \
        } \
        if (rb->p_buff == NULL) { \
            return 0; \
        } \
        /* 计算已使用空间 */ \
        uint16_t used; \
        if (rb->is_full_flag) { \
            used = rb->size; \
        } else if (rb->head >= rb->tail) { \
            used = rb->head - rb->tail; \
        } else { \
            used = rb->size - (rb->tail - rb->head); \
        } \
        uint16_t read_len = (used < len) ? used : len; \
        if (read_len == 0) { \
            return 0; \
        } \
        for (uint16_t i = 0; i < read_len; i++) { \
            data[i] = rb->p_buff[rb->tail]; \
            rb->tail = (rb->tail + 1) % rb->size; \
            rb->is_full_flag = false; /* 读取数据后，缓冲区不再满 */ \
        } \
        return read_len; \
    } \
    \
    /** @brief 获取可用空间函数 */ \
    static uint16_t name##_available(void) { \
        ring_buff_t *rb = &name##_ring_buff_instance; \
        if (rb->is_full_flag) { \
            return 0; \
        } else if (rb->head >= rb->tail) { \
            return rb->size - (rb->head - rb->tail); \
        } else { \
            return rb->tail - rb->head; \
        } \
    } \
    \
    /** @brief 获取已使用空间函数 */ \
    static uint16_t name##_used(void) { \
        ring_buff_t *rb = &name##_ring_buff_instance; \
        if (rb->is_full_flag) { \
            return rb->size; \
        } else if (rb->head >= rb->tail) { \
            return rb->head - rb->tail; \
        } else { \
            return rb->size - (rb->tail - rb->head); \
        } \
    } \
    \
    /** @brief 检查是否为空函数 */ \
    static bool name##_is_empty(void) { \
        ring_buff_t *rb = &name##_ring_buff_instance; \
        return (!rb->is_full_flag && (rb->head == rb->tail)); \
    } \
    \
    /** @brief 检查是否已满函数 */ \
    static bool name##_is_full(void) { \
        ring_buff_t *rb = &name##_ring_buff_instance; \
        return rb->is_full_flag; \
    } \
    \
    /** @brief 清空缓冲区函数 */ \
    static void name##_clear(void) { \
        ring_buff_t *rb = &name##_ring_buff_instance; \
        rb->head = rb->tail = 0; \
        rb->is_full_flag = false; \
    } \
    \
    /** @brief 删除缓冲区函数 */ \
    static void name##_delete(void) { \
        ring_buff_t *rb = &name##_ring_buff_instance; \
        if (rb) { \
            if (rb->p_buff) { \
                free(rb->p_buff); \
                rb->p_buff = NULL; \
            } \
            rb->size = 0; \
            rb->head = 0; \
            rb->tail = 0; \
            rb->is_full_flag = false; \
        } \
    } \
    \
    /** @brief 构建环形缓冲区实例函数 */ \
    ring_buff_t* name##_ring_buff(void) { \
        ring_buff_t *rb = &name##_ring_buff_instance; \
        if (rb->p_buff != NULL) { \
            return rb; \
        } \
        \
        rb->p_buff = (uint8_t*)malloc(buff_size); \
        if (rb->p_buff == NULL) { \
            return NULL; \
        } \
        \
        rb->size = buff_size; \
        rb->head = 0; \
        rb->tail = 0; \
        rb->is_full_flag = false; \
        rb->write = name##_write; \
        rb->read = name##_read; \
        rb->available = name##_available; \
        rb->used = name##_used; \
        rb->is_empty = name##_is_empty; \
        rb->is_full = name##_is_full; \
        rb->clear = name##_clear; \
        rb->delete = name##_delete; \
        \
        return rb; \
    } \
    /** @brief 获取环形缓冲区实例函数 */ \
    ring_buff_t* name##_ring_buff_get(void) { \
        return &name##_ring_buff_instance; \
    }
