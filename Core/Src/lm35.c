/*
 * lm35.c
 *
 *  Created on: Dec 5, 2025
 *      Author: Hoàng Lâm
 */


#include "lm35.h"

void lm35_init(LM35_Typedef* lm35, ADC_HandleTypeDef* hadc){
	lm35->hadc = hadc;
	lm35->temp = 0.0f;
	HAL_ADC_Start(hadc);
}
void lm35_getTemp(LM35_Typedef* lm35){
	float adc_value = HAL_ADC_GetValue(lm35->hadc);
	float voltage = (adc_value * 3.3f) / 4095.0f;
	lm35->temp = voltage * 100 - 0.4f;
}
