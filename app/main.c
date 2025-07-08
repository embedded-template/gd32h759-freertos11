#include "gd32h7xx.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"

#include "main.h"

void cache_enable(void);
void mpu_config(void);

/* FreeRTOS task functions */
void vTask1_LED_Toggle(void *pvParameters);
void vTask2_Print_Counter(void *pvParameters);

/* Task handles */
TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;


task_info_t task_info_all[] = {
    {task_test,"test",TASK_stack_depth_test,5000,3,&pTask_test},
};

#define TASK_NUM (sizeof(task_info_all) / sizeof(task_info_t))


/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* enable the CPU cache */
    cache_enable();
    mpu_config();
    
	for (int i = 0; i < TASK_NUM; i++)
	{

		xTaskCreate(task_info_all[i].pxTaskCode, 
            task_info_all[i].pcName, 
            task_info_all[i].uxStackDepth, 
            &task_info_all[i].time, 
            task_info_all[i].uxPriority, 
            task_info_all[i].pxCreatedTask);
	}

	vTaskStartScheduler();
    while(1);
}


/*!
    \brief      enable the CPU cache
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cache_enable(void)
{
    /* enable i-cache */
    SCB_EnableICache();
    /* enable d-cache */
    SCB_EnableDCache();
}

/*!
    \brief      configure the MPU attributes
    \param[in]  none
    \param[out] none
    \retval     none
*/
void mpu_config(void)
{
    mpu_region_init_struct mpu_init_struct;
    mpu_region_struct_para_init(&mpu_init_struct);

    /* disable the MPU */
    ARM_MPU_Disable();
    ARM_MPU_SetRegion(0, 0);

    /* configure the MPU attributes for the entire 4GB area, Reserved, no access */
    /* This configuration is highly recommended to prevent Speculative Prefetching of external memory, 
       which may cause CPU read locks and even system errors */
    mpu_init_struct.region_base_address  = 0x0;
    mpu_init_struct.region_size          = MPU_REGION_SIZE_4GB;
    mpu_init_struct.access_permission    = MPU_AP_NO_ACCESS;
    mpu_init_struct.access_bufferable    = MPU_ACCESS_NON_BUFFERABLE;
    mpu_init_struct.access_cacheable     = MPU_ACCESS_NON_CACHEABLE;
    mpu_init_struct.access_shareable     = MPU_ACCESS_SHAREABLE;
    mpu_init_struct.region_number        = MPU_REGION_NUMBER0;
    mpu_init_struct.subregion_disable    = 0x87;
    mpu_init_struct.instruction_exec     = MPU_INSTRUCTION_EXEC_NOT_PERMIT;
    mpu_init_struct.tex_type             = MPU_TEX_TYPE0;
    mpu_region_config(&mpu_init_struct);
    mpu_region_enable();

    /* enable the MPU */
    ARM_MPU_Enable(MPU_MODE_PRIV_DEFAULT);
}

#if 0
#ifdef GD_ECLIPSE_GCC
/* retarget the C library printf function to the USART, in Eclipse GCC environment */
int __io_putchar(int ch)
{
    usart_data_transmit(EVAL_COM, (uint8_t) ch );
    while(RESET == usart_flag_get(EVAL_COM, USART_FLAG_TBE));
    return ch;
}
#else
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM, USART_FLAG_TBE));

    return ch;
}
#endif /* GD_ECLIPSE_GCC */
#endif

#if(configCHECK_FOR_STACK_OVERFLOW)
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName)
{
    for (;;) {}
}
#endif

