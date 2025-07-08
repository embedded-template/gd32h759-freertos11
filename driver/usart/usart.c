#include "usart.h"
#include <stdio.h>

TaskHandle_t * pTask_debug;

void com_usart_init(void)
{
    /* enable COM GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOD);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);

    /* connect port to USART TX */
    gpio_af_set(GPIOD, GPIO_AF_7, GPIO_PIN_5);
    /* connect port to USART RX */
    gpio_af_set(GPIOD, GPIO_AF_7, GPIO_PIN_6);

    /* configure USART TX as alternate function push-pull */
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_5);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_5);

    /* configure USART RX as alternate function push-pull */
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_100_220MHZ, GPIO_PIN_6);

    /* configure USART */
    usart_deinit(USART1);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_baudrate_set(USART1, 115200U);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);

    usart_enable(USART1);
}

/*
int __io_putchar(int ch)
{
    usart_data_transmit(USART1, (uint8_t) ch );
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
    return ch;
}*/


void Task_UART1_Debug(void *pvParameters)
{
    com_usart_init();
    uint8_t ch = 12;
    while(1)
    {
        //printf("Hello, World!\n");
        usart_data_transmit(USART1, ch);
        vTaskDelay(pdMS_TO_TICKS(*((uint32_t*)pvParameters)));
    }
}

