/* Includes ------------------------------------------------------------------*/
#include "driver_update.h"
#include "spi.h"
#include "sysclk_timer.h"
#include "button.h"
#include "tim.h"

const uint8_t LED_SYMBOL_1[10] = {0xD7, 0x84, 0xE3, 0xE6, 0xB4, 0x76, 0x77, 0xC4, 0xF7, 0xF6};
const uint8_t LED_SYMBOL_2[10] = {0xD7, 0x11, 0xE3, 0x73, 0x35, 0x76, 0xF6, 0x13, 0xF7, 0x77};
const uint8_t LED_SYMBOL_3[10] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};

uint8_t data1_spi[4] = {0x00, 0xFF, 0xFF, 0xFF};
uint8_t data2_spi[4] = {0x00, 0xFF, 0xFF, 0xFF};
uint8_t data3_spi[4] = {0x00, 0xFF, 0xFF, 0xFF};
uint8_t data4_spi[4] = {0x00, 0xFF, 0xFF, 0xFF};


static uint8_t brightness_value = 1;
static uint8_t mode_data = 0;

static void dm134b_enable(void);
static void dm134b_disable(void);
static void dm134_SendData(uint8_t *data);

void SPI_update_task(void);
void LED_update_task(void);
void button_callback(uint16_t u16ButtonId, en_button_state_t enState);

void main_init(void)
{
//	dm134b_enable();
	Button_Init(button_callback);
	Timer_Start(TIMER_ID_MAIN, 1000, TRUE, SPI_update_task);	
	data2_spi[2] = LED_SYMBOL_1[brightness_value / 10];
	data2_spi[3] = LED_SYMBOL_2[brightness_value % 10];
}

void SPI_update_task(void)
{
	static uint8_t sec = 0;
	static uint8_t min = 0;
	static uint8_t hours = 0;
	
	sec++;
	if (sec == 60)
	{
		sec = 0;
		
		
		min++;
		if (min == 60)
		{
			min = 0;
			hours++;
			if (hours == 23)
				hours = 0;
		}
	}
	if (mode_data == 0)
	{
		data3_spi[1] = LED_SYMBOL_3[min / 10];
		data2_spi[1] = LED_SYMBOL_3[min % 10];
		data1_spi[1] = LED_SYMBOL_3[sec / 10];
		data4_spi[1] = LED_SYMBOL_3[sec % 10];	
			
		data1_spi[2] = LED_SYMBOL_1[min / 10];
		data1_spi[3] = LED_SYMBOL_2[min % 10];
		data3_spi[2] = LED_SYMBOL_1[sec / 10];
		data3_spi[3] = LED_SYMBOL_2[sec % 10];
	}
	else if (mode_data == 1)
	{
		data1_spi[1] = 0xFF;
		data1_spi[2] = 0xFF;
		data1_spi[3] = 0xFF;
		data2_spi[1] = 0xFF;
		data2_spi[2] = 0xFF;
		data2_spi[3] = 0xFF;
		data3_spi[1] = 0xFF;
		data3_spi[2] = 0xFF;
		data3_spi[3] = 0xFF;
		data4_spi[1] = 0xFF;		
		data4_spi[2] = 0xFF;	
		data4_spi[3] = 0xFF;	
	}
}
/*
void LED_update_task(void)
{
	static uint8_t num_led = 0;
	
		switch (num_led)
	{
		case 0:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
			dm134_SendData(data1_spi);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			num_led = 1;
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			dm134_SendData(data2_spi);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
			num_led = 2;
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
			dm134_SendData(data3_spi);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
			num_led = 3;
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
			dm134_SendData(data4_spi);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
			num_led = 0;
			break;		
	}

//	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_10);
}
*/
void LedUpdate(void)
{

	static uint8_t num_led = 0;
	
	switch (num_led)
	{
		case 0:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
			dm134_SendData(data1_spi);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);		
			num_led = 1;
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
			dm134_SendData(data2_spi);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
			num_led = 2;
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
			dm134_SendData(data3_spi);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
			num_led = 3;
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
			dm134_SendData(data4_spi);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
			num_led = 0;
			break;		
	}
/*
	static uint8_t num_led = 0;
	
	switch (num_led)
	{
		case 0:
			dm134_SendData(data1_spi);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			num_led = 1;
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			num_led = 2;
			break;
		case 2:
			dm134_SendData(data2_spi);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
			num_led = 3;
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
			num_led = 4;
			break;
		case 4:
			dm134_SendData(data3_spi);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
			num_led = 5;
			break;
		case 5:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
			num_led = 6;
			break;
		case 6:
			dm134_SendData(data4_spi);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
			num_led = 7;
			break;
		case 7:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
			num_led = 0;
			break;
	}
*/	
}

void button_callback(uint16_t u16ButtonId, en_button_state_t enState)
{
	TIM_OC_InitTypeDef sConfigOC;
	
	switch (enState)
	{
		case StateHigh:
			if (brightness_value == 10)
				brightness_value = 1;
			else
				brightness_value++;
			data2_spi[2] = LED_SYMBOL_1[brightness_value / 10];
			data2_spi[3] = LED_SYMBOL_2[brightness_value % 10];
			sConfigOC.OCMode = TIM_OCMODE_PWM1;
			sConfigOC.Pulse = 110 * brightness_value;
			sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
			sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
			sConfigOC.OCIdleState = TIM_OCNIDLESTATE_RESET;
			HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_4);
			HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
//			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_12);
			break;
		case StateLow:
			break;
		case StateLong:
			if (mode_data == 0)
			{
				data3_spi[1] = LED_SYMBOL_3[8];
				data2_spi[1] = LED_SYMBOL_3[8];
				data1_spi[1] = LED_SYMBOL_3[8];
				data4_spi[1] = LED_SYMBOL_3[8];	
					
				data1_spi[2] = LED_SYMBOL_1[8];
				data1_spi[3] = LED_SYMBOL_2[8];
				data3_spi[2] = LED_SYMBOL_1[8];
				data3_spi[3] = LED_SYMBOL_2[8];		
				data2_spi[2] = LED_SYMBOL_1[8];
				data2_spi[3] = LED_SYMBOL_2[8];
				mode_data = 1;
			}
			else if (mode_data == 1)
			{
				data2_spi[2] = LED_SYMBOL_1[brightness_value / 10];
				data2_spi[3] = LED_SYMBOL_2[brightness_value % 10];
				mode_data = 0;
			}
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_14);
			break;
		case StateInvalid:
			break;
	}
}

static void dm134b_enable(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
}

static void dm134b_disable(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
}

static void dm134_SendData(uint8_t *data)

{	
	HAL_SPI_Transmit(&hspi3, data, 2, 20);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);  // LATCH
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);  // LATCH	
}
