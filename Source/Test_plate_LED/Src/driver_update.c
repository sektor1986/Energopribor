/* Includes ------------------------------------------------------------------*/
#include "driver_update.h"
#include "spi.h"
#include "sysclk_timer.h"
#include "button.h"

static void dm134b_enable(void);
static void dm134b_disable(void);
static void dm134b_LATCH(void);

void SPI_update_task(void);
void LED_update_task(void);
void button_callback(uint16_t u16ButtonId, en_button_state_t enState);

void main_init(void)
{
	dm134b_enable();
	Button_Init(button_callback);
	Timer_Start(TIMER_ID_MAIN, 100, TRUE, SPI_update_task);	
	Timer_Start(TIMER_ID_4IMP, 1000, TRUE, LED_update_task);	
}

void SPI_update_task(void)
{
	uint8_t data_spi[4] = {0xAA, 0xAA, 0x05, 0x05};
	
	HAL_SPI_Transmit(&hspi3, data_spi, 2, 10);
	dm134b_LATCH();	
}

void LED_update_task(void)
{
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_10);
}

void button_callback(uint16_t u16ButtonId, en_button_state_t enState)
{
	switch (enState)
	{
		case StateHigh:
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_12);
			break;
		case StateLow:
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
			break;
		case StateLong:
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_14);
			break;
		case StateInvalid:
			break;
	}
}

static void dm134b_enable(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
}

static void dm134b_disable(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
}

static void dm134b_LATCH(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
}
