/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __sysclk_timer__H
#define __sysclk_timer__H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
	 
typedef uint_fast8_t boolean_t;
typedef void (*func_ptr_t)(void);
	 
#ifndef TRUE
	/** Value is true (boolean_t type) */
    #define TRUE        ((boolean_t) 1)
#endif

#ifndef FALSE
	/** Value is false (boolean_t type) */
    #define FALSE       ((boolean_t) 0)
#endif
	 
// Timer IDs (must start at 0 and be incremented by 1)
#define TIMER_ID_MAIN                   0
#define TIMER_ID_BUTTON                 1
#define TIMER_ID_4IMP                   2

// Number of timer IDs (= last timer ID + 1)
#define TIMER_COUNT						3

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/
void Timer_Init(void);
void Timer_Disable(void);
void Timer_Start(uint8_t u8TimerID, uint16_t u16TimeMs, boolean_t bReload, func_ptr_t pfnCallback);
void Timer_Main(void);
uint16_t Timer_Remaining(uint8_t u8TimerID);
boolean_t Timer_IsElapsed(uint8_t u8TimerID, boolean_t bClearElapsedFlag);
void Timer_Wait(uint8_t u8TimerID, uint16_t u16TimeMs, boolean_t bServeHardwareWatchdog); 

#ifdef __cplusplus
}
#endif
#endif 
