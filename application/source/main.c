#include "string.h"
#include "stm32f4xx_hal.h"

#ifdef FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#endif

typedef struct {
    uint16_t pin;
    uint16_t delay;
} pin_toggle_t;

pin_toggle_t pin_toggle[4];

void SystemClock_Config(void);
void Error_Handler(void);
static void GPIO_Init(void);

#ifdef FREERTOS
void CreateTask(void);
#endif

int main(void){
  SystemClock_Config();
  GPIO_Init();

  #ifdef FREERTOS

  CreateTask();

  while(1){}

  #else

  while (1){
   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);

   for (unsigned long i = 0; i < 10000000; i++){
    asm("nop");
   }

      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

   for (unsigned long i = 0; i < 10000000; i++){
    asm("nop");
   }
  }

  #endif
}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

static void GPIO_Init(void){
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
}

void Error_Handler(void){
  while(1){
  }
}

#ifdef FREERTOS
void vBlinkTask (void *pvParameters){
    pin_toggle_t *pin_toggle = (pin_toggle_t *) pvParameters;
    pin_toggle_t toggle;

    memcpy(&toggle, pin_toggle, sizeof(pin_toggle_t));

    while(1){
        HAL_GPIO_WritePin(GPIOD, toggle.pin, GPIO_PIN_SET);
        vTaskDelay(toggle.delay);
        HAL_GPIO_WritePin(GPIOD, toggle.pin, GPIO_PIN_RESET);
        vTaskDelay(toggle.delay);
    }

    vTaskDelete(NULL);
}

void CreateTask(void){
    pin_toggle[0].pin = GPIO_PIN_12;
    pin_toggle[0].delay = 100;
    xTaskCreate(vBlinkTask, "BlinkTask1", configMINIMAL_STACK_SIZE, (void *) &pin_toggle[0], tskIDLE_PRIORITY + 1, NULL);

    pin_toggle[1].pin = GPIO_PIN_13;
    pin_toggle[1].delay = 200;
    xTaskCreate(vBlinkTask, "BlinkTask2", configMINIMAL_STACK_SIZE, (void *) &pin_toggle[1], tskIDLE_PRIORITY + 1, NULL);

    pin_toggle[2].pin = GPIO_PIN_14;
    pin_toggle[2].delay = 400;
    xTaskCreate(vBlinkTask, "BlinkTask3", configMINIMAL_STACK_SIZE, (void *) &pin_toggle[2], tskIDLE_PRIORITY + 1, NULL);

    pin_toggle[3].pin = GPIO_PIN_15;
    pin_toggle[3].delay = 800;
    xTaskCreate(vBlinkTask, "BlinkTask4", configMINIMAL_STACK_SIZE, (void *) &pin_toggle[3], tskIDLE_PRIORITY + 1, NULL);

    vTaskStartScheduler();
}

void vApplicationTickHook(void){

}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName){

}

void vApplicationIdleHook(void){

}

void vApplicationMallocFailedHook(void){

}
#endif
