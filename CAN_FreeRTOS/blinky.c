#include "FreeRTOS.h"
#include "task.h"
#include "Board_Buttons.h"
#include "Board_Led.h"



TaskHandle_t tid_LED_handler;
TaskHandle_t tid_BUT_handler;

void LED_handler(void *arg);
void BUT_handler(void *arg);

const TickType_t xDelay500 = 500 / portTICK_PERIOD_MS;
const TickType_t xDelay100 = 100 / portTICK_PERIOD_MS;

void app_main(void *arg)
{
    
    xTaskCreate(BUT_handler, "Button handler", 64U, NULL, tskIDLE_PRIORITY+1, &tid_BUT_handler);
    xTaskCreate(LED_handler, "LED handler", 64U, NULL, tskIDLE_PRIORITY+1, &tid_LED_handler);
    
    vTaskDelay (portMAX_DELAY);
    while(1);
}


void LED_handler(void *arg)
{
    uint8_t active_flag = 1U;

    for (;;) {
        if (xTaskNotifyWait (0, 0x0001, NULL, xDelay100) == pdTRUE ) {
            active_flag ^=1U; 
        }

        if (active_flag == 1U){
            LED_On (0U);
            vTaskDelay (xDelay500);                             // Delay 500 ms
            LED_Off (0U);                               // Switch off
            vTaskDelay (xDelay500);                             // Delay 500 ms
        }
    }
}

void BUT_handler(void *arg)
{
    uint8_t last;
    uint8_t state;

    for (;;) {
        state = (Buttons_GetState () & 1U);           // Get pressed button state
        if (state != last){
            if (state == 1){
                xTaskNotify (tid_LED_handler, 0x0001, eSetBits); 
            }
        }
        last = state;
        vTaskDelay (xDelay100);
    }
    
}

