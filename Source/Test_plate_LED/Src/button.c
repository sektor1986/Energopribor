#include "button.h"
#include "sysclk_timer.h"

//#define BUTTON_COUNT    (sizeof(m_astcButtonCtrl) / sizeof(m_astcButtonCtrl[0]))
#define BUTTON_COUNT 1

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/
typedef struct stc_button_control
{
    GPIO_TypeDef*       u8PortNumber;
    uint16_t            u8PinMask;
    uint16_t            u16ButtonId;
    en_button_state_t   enCurrState;
    uint8_t             u8DebounceCounter;
    uint16_t			u16stateRepeat;
} stc_button_control_t;


/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
static stc_button_control_t         m_astcButtonCtrl[] = BUTTON_ASSIGNMENT;
static button_callback_func_ptr_t   m_pfnCallback = NULL;

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/
static void ScanTimerCallback(void);

void Button_Init(button_callback_func_ptr_t pfnCallback)
{
    uint_fast8_t fu8ButtonIndex;
	
    for (fu8ButtonIndex = 0; fu8ButtonIndex < BUTTON_COUNT; fu8ButtonIndex++)
    {
        stc_button_control_t* pstcButton = &m_astcButtonCtrl[fu8ButtonIndex];
        // Set pin direction to input
//        DDR(pstcButton->u8PortNumber) &= ~pstcButton->u8PinMask;
        pstcButton->u8DebounceCounter = BUTTON_DEBOUNCE_COUNT - 1;
        //pstcButton->enCurrState = StateInvalid;
    }
    
    m_pfnCallback = pfnCallback;
    
    // Start timer for debouncing buttons
    Timer_Start(TIMER_ID_BUTTON, BUTTON_SCAN_INTERVAL_MS-1, TRUE, ScanTimerCallback);
}

/**
 ******************************************************************************
 ** Returns the current state of a button, independent of it's debounced state.
 **
 ** \param u16ButtonId Button ID (BUTTON_ID_??)
 **
 ** \return (StateLow, StateHigh, StateInvalid)
 *****************************************************************************/
en_button_state_t Button_GetCurrentButtonState(uint16_t u16ButtonId)
{
    uint_fast8_t fu8ButtonIndex;

    for (fu8ButtonIndex = 0; fu8ButtonIndex < BUTTON_COUNT; fu8ButtonIndex++)
    {
        stc_button_control_t* pstcButton = &m_astcButtonCtrl[fu8ButtonIndex];
        
        if (pstcButton->u16ButtonId == u16ButtonId)
        {
            // Get current state of pin
						if (HAL_GPIO_ReadPin(pstcButton->u8PortNumber, pstcButton->u8PinMask) == GPIO_PIN_RESET)
            {
                return StateLow;
            }
            else
            {
                return StateHigh;
            }
        }
    }
    
    return StateInvalid;
}

/**
 ******************************************************************************
 ** Callback function from timer module. Scan all buttons and debounce. Call
 ** callback function (if any).
 **
 ** \param none
 **
 ** \return none
 *****************************************************************************/
static void ScanTimerCallback(void)
{
    uint_fast8_t fu8ButtonIndex;
     
		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_15);
    for (fu8ButtonIndex = 0; fu8ButtonIndex < BUTTON_COUNT; fu8ButtonIndex++)
    {
        en_button_state_t enSampledState;
        stc_button_control_t* pstcButton = &m_astcButtonCtrl[fu8ButtonIndex];

        // Get current state of pin
				//if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
        if (HAL_GPIO_ReadPin(pstcButton->u8PortNumber, pstcButton->u8PinMask) == GPIO_PIN_SET)
        {
            enSampledState = StateLow;
            pstcButton->u16stateRepeat++;
            if (pstcButton->u16stateRepeat > 75)
            {
            	pstcButton->u16stateRepeat = 75;
            	enSampledState = StateLong;
            }
        }
        else
        {
            enSampledState = StateHigh;
            pstcButton->u16stateRepeat = 0;
        }
               
        
        // If the sampled state is different to current state
        if ((enSampledState != pstcButton->enCurrState) || (pstcButton->u16stateRepeat == 75)) 
        {
            // If debounce counter reached 0
            if (pstcButton->enCurrState == StateLong)
            {
            	pstcButton->enCurrState = enSampledState;
            	return;
            }
            if (pstcButton->u8DebounceCounter == 0)
            {
                // Accept new state
                pstcButton->enCurrState = enSampledState;
                // Call callback function (if any)
                if (m_pfnCallback != NULL)
                {
                    m_pfnCallback(pstcButton->u16ButtonId, pstcButton->enCurrState);
                }
            }
            else
            {
                // Count down
                pstcButton->u8DebounceCounter--;
            }
        }
        else
        {
            // state equal -> restart debounce counter
            pstcButton->u8DebounceCounter = BUTTON_DEBOUNCE_COUNT - 1;
        }
    }
}

