#include "stm32f1xx_hal.h"
#include "main.h"


uint8_t const *sounddata_data = 0;
int sounddata_length = 0;
volatile uint16_t sample = 0;
uint8_t lastSample = 0;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

void startPlayback(uint8_t const *data, int length)
{
  sounddata_data = data;
  sounddata_length = length;

  // Set initial pulse width to the first sample.
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, sounddata_data[0]);

  // Start PWM generation on TIM1
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  // Start TIM2 interrupt
  HAL_TIM_Base_Start_IT(&htim2);

  lastSample = sounddata_data[sounddata_length - 1];
  sample = 0;
}

void stopPlayback()
{
  // Stop TIM2 interrupt
  HAL_TIM_Base_Stop_IT(&htim2);

  // Stop PWM generation on TIM1
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2) // Check if TIM2 triggered this interrupt
  {
    if (sample >= sounddata_length)
    {
      if (sample == sounddata_length + lastSample)
      {
        stopPlayback();
      }
      else
      {
        // Ramp down to zero to reduce the click at the end of playback.
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, sounddata_length + lastSample - sample);
      }
    }
    else
    {
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, sounddata_data[sample]);
    }
    ++sample;
  }
}
