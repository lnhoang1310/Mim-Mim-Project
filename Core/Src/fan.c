/*
 * fan.c
 *
 *  Created on: Dec 3, 2025
 *      Author: Hoàng Lâm
 */

#include "fan.h"
#include "led.h"

static Fan_Typedef fan;

static void pwm_set_duty(TIM_HandleTypeDef* htim, uint16_t channel, float duty){
	uint16_t ccr = (duty / 100) * htim->Instance->ARR;
	switch(channel){
		case TIM_CHANNEL_1:
			htim->Instance->CCR1 = ccr;
			break;
		case TIM_CHANNEL_2:
			htim->Instance->CCR2 = ccr;
			break;
		case TIM_CHANNEL_3:
			htim->Instance->CCR3 = ccr;
			break;
		case TIM_CHANNEL_4:
			htim->Instance->CCR4 = ccr;
			break;
	}
}

void Fan_Control(float _speed){
	if(fan.speed != _speed){
		fan.speed = _speed;
		pwm_set_duty(fan.htim, fan.channel, _speed);
		Led_Control(_speed);
	}
}

void Fan_Init(TIM_HandleTypeDef* _htim, uint16_t _channel){
	fan.htim = _htim;
	fan.channel = _channel;
	fan.speed = 0;
	HAL_TIM_PWM_Start(fan.htim, fan.channel);
}


