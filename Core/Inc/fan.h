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
	float speed;
}Fan_Typedef;

void Fan_Init(TIM_HandleTypeDef* _htim, uint16_t _channel);
void Fan_Control(float _speed);

#endif /* INC_FAN_H_ */
