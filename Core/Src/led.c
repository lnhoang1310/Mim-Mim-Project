/*
 * led.c
 *
 *  Created on: Dec 3, 2025
 *      Author: Hoàng Lâm
 */

#include "led.h"

uint16_t list_led[] = {LED1_Pin, LED2_Pin, LED3_Pin, LED4_Pin, LED5_Pin, LED6_Pin, LED7_Pin, LED8_Pin};

void Led_Init(void){
	Led_Control(0);
}

void Led_Control(float duty){
	uint32_t odr = 0;
	uint8_t led_on = duty * 8 / 100;
	for(uint8_t i=0; i<8; i++){
		if(i < led_on)
			odr |= (1 << i);   // Bật LED
		else
			odr &= ~(1 << i);  // Tắt LED
	}
	LED_PORT->ODR = (LED_PORT->ODR & ~0xFF) | (odr & 0xFF);
}

