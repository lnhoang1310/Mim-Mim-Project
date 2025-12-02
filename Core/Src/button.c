/*
 * button.c
 *
 *  Created on: Dec 3, 2025
 *      Author: Hoàng Lâm
 */

#include "button.h"
#include "main.h"

#define DEBOUNCE_TIME     20
#define HOLD_TIME         1000
#define REPEAT_INTERVAL   50

typedef struct{
    uint32_t last_time;
    uint32_t hold_start;
    uint8_t last_state;
    uint8_t is_held;
}ButtonState;


static ButtonState btn_left  = {0, 0, 1, 0};
static ButtonState btn_right = {0, 0, 1, 0};
static ButtonState btn_middle = {0, 0, 1, 0};

struct{
	GPIO_TypeDef *port;
	uint16_t pin;
	ButtonState *state;
}buttons[] = {
	{BUTTON_MIDDLE_GPIO_Port, BUTTON_MIDDLE_Pin, &btn_middle},
	{BUTTON_LEFT_GPIO_Port, BUTTON_LEFT_Pin, &btn_left},
	{BUTTON_RIGHT_GPIO_Port, BUTTON_RIGHT_Pin, &btn_right}
};

uint16_t Button_Pressing(void){
    uint32_t now = HAL_GetTick();

    for (int i = 0; i < 5; i++){
        uint8_t state = HAL_GPIO_ReadPin(buttons[i].port, buttons[i].pin);
        ButtonState *btn = buttons[i].state;
        if (state != btn->last_state) {
            if(now - btn->last_time > DEBOUNCE_TIME){
                btn->last_time = now;
                btn->last_state = state;

                if(state == GPIO_PIN_RESET) {
                    btn->hold_start = now;
                    btn->is_held = 0;
                    return buttons[i].pin;
                }else{
                    btn->is_held = 0;
                }
            }
        }
        else if(state == GPIO_PIN_RESET){
            if(!btn->is_held && (now - btn->hold_start > HOLD_TIME)){
                btn->is_held = 1;
                btn->last_time = now;
                return buttons[i].pin;
            }

            if(btn->is_held && (now - btn->last_time > REPEAT_INTERVAL)){
                btn->last_time = now;
                return buttons[i].pin;
            }
        }
    }

    return 0;
}

