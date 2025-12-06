/*
 * lm35.h
 *
 *  Created on: Dec 5, 2025
 *      Author: Hoàng Lâm
 */

#ifndef SRC_LM35_H_
#define SRC_LM35_H_

#include "stm32f4xx.h"

typedef struct{
	ADC_HandleTypeDef* hadc;
	float temp;
}LM35_Typedef;

void lm35_init(LM35_Typedef* lm35, ADC_HandleTypeDef* hadc);
void lm35_getTemp(LM35_Typedef* lm35);

#endif /* SRC_LM35_H_ */
