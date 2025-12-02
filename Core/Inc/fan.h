/*
 * fan.h
 *
 *  Created on: Dec 3, 2025
 *      Author: Hoàng Lâm
 */

#ifndef INC_FAN_H_
#define INC_FAN_H_

#include "stm32f4xx.h"

typedef struct{
	TIM_HandleTypeDef* htim;
	uint16_t channel;
	GPIO_TypeDef* in1_port;
	uint16_t in1_pin;
	GPIO_TypeDef* in2_port;
	uint16_t in2_pin;
	float speed;
}Fan_Typedef;

void Fan_Init(TIM_HandleTypeDef* _htim, uint16_t _channel, GPIO_TypeDef* _in1_port, uint16_t _in1_pin, GPIO_TypeDef* _in2_port, uint16_t _in2_pin);
void Fan_Control(float _speed);

#endif /* INC_FAN_H_ */
