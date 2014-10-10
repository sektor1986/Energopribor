/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __button__H
#define __button__H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
	 
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/
/** Button ID for button 1 */
#define BUTTON_ID_B1        ((uint16_t)0xA)
/** Button ID for button 2 */
#define BUTTON_ID_B2        ((uint16_t)0xB)        
/** Button ID for button 3 */
#define BUTTON_ID_B3        ((uint16_t)0xC)
/** Button ID for button 4 */
#define BUTTON_ID_B4        ((uint16_t)0xD)

/**
 ** Assignement of buttons to GPIO pins (GPIO port and appropriate pin number).
 ** Create a list as follows: { {port number, pin mask, button ID, idle state}, {port number, pin mask, button ID, idle state}, ... } */
#define BUTTON_ASSIGNMENT       {   {GPIOA, GPIO_PIN_0, BUTTON_ID_B1, StateHigh}}
                                    
/** Set the scan interval for reading the pin state in milliseconds (20 ... 100) */
#define BUTTON_SCAN_INTERVAL_MS     20

/** Set debounce count i.e. number of scan intervals which must have the same state (1 ... 50) */
#define BUTTON_DEBOUNCE_COUNT       3

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/
typedef enum en_button_state
{
    StateInvalid,
    StateLow,
    StateHigh,
    StateLong
} en_button_state_t;

/**
 ** function pointer type to button callback function
 ** \param u16ButtonId  ID of button that changed its state (BUTTON_ID_???)
 ** \param enState      new state of button
 ** \return none */
typedef void (*button_callback_func_ptr_t)(uint16_t u16ButtonId, en_button_state_t enState);

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

extern void Button_Init(button_callback_func_ptr_t pfnCallback);
extern en_button_state_t Button_GetCurrentButtonState(uint16_t u16ButtonId);
#ifdef __cplusplus
}
#endif
#endif 
