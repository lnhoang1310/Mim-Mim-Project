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
		if(_speed > 0){
			HAL_GPIO_WritePin(fan.in1_port, fan.in1_pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(fan.in2_port, fan.in2_pin, GPIO_PIN_SET);
		}else{
			HAL_GPIO_WritePin(fan.in1_port, fan.in1_pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(fan.in2_port, fan.in2_pin, GPIO_PIN_RESET);
		}
		pwm_set_duty(fan.htim, fan.channel, _speed);
		Led_Control(_speed);
	}
}

void Fan_Init(TIM_HandleTypeDef* _htim, uint16_t _channel, GPIO_TypeDef* _in1_port, uint16_t _in1_pin, GPIO_TypeDef* _in2_port, uint16_t _in2_pin){
	fan.htim = _htim;
	fan.channel = _channel;
	fan.in1_port = _in1_port;
	fan.in1_pin = _in1_pin;
	fan.in2_port = _in2_port;
	fan.in2_pin = _in2_pin;
	fan.speed = 0;
	HAL_TIM_PWM_Start(fan.htim, fan.channel);
}


