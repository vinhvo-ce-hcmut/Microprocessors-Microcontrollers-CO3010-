/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Lab 01 - Exercise 8 (one clock LED steps around)
  *
  * Wiring:
  * - LEDs are active-low: LED anode -> +3.3 V, LED cathode -> resistor -> MCU.
  * - 7SEG-COM-ANODE: PB0..PB6 -> a..g, common pin -> +3.3 V.
  * - Analog clock: PA4..PA15 are positions 0..11.
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include <stdint.h>

#define LED_ON           GPIO_PIN_RESET
#define LED_OFF          GPIO_PIN_SET

#define SEGMENT_MASK     (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                          GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6)

#define CLOCK_LED_MASK   (GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_6  | GPIO_PIN_7  | \
                          GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_11 | \
                          GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)

static uint8_t counter = 0U;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

static void clearAllClock(void);
static void setNumberOnClock(int num);
static void runExercise8(void);

static void clearAllClock(void)
{
  HAL_GPIO_WritePin(GPIOA, CLOCK_LED_MASK, LED_OFF);
}

static void setNumberOnClock(int num)
{
  if ((num >= 0) && (num <= 11))
  {
    HAL_GPIO_WritePin(GPIOA, (uint16_t)(GPIO_PIN_4 << num), LED_ON);
  }
}

static void runExercise8(void)
{
  clearAllClock();
  setNumberOnClock((int)counter);
  HAL_Delay(1000U);
  counter = (uint8_t)((counter + 1U) % 12U);
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  while (1)
  {
    runExercise8();
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                               RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* PA13, PA14, PA15 are required for exercises 3 and 6-10. */
  __HAL_AFIO_REMAP_SWJ_DISABLE();

  /* Active-low LEDs and common-anode 7SEG start in the OFF state. */
  HAL_GPIO_WritePin(GPIOA, CLOCK_LED_MASK, LED_OFF);
  HAL_GPIO_WritePin(GPIOB, SEGMENT_MASK, LED_OFF);

  GPIO_InitStruct.Pin = CLOCK_LED_MASK;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SEGMENT_MASK;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  (void)file;
  (void)line;
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
