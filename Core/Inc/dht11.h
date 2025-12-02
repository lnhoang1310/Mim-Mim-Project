/*
 * dht11.h
 *
 *  Created on: Dec 3, 2025
 *      Author: Hoàng Lâm
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "stm32f4xx_hal.h"
#define DHT11_STARTTIME 18000
#define DHT11 0x01

typedef struct{
	TIM_HandleTypeDef* htim;
	uint16_t Pin;
	GPIO_TypeDef* PORT;
	float Temp;
	float Humi;
}DHT_Name;

void DHT_Init(DHT_Name* DHT, TIM_HandleTypeDef* _htim, GPIO_TypeDef* DH_PORT, uint16_t DH_Pin);
uint8_t DHT_ReadTempHum(DHT_Name* DHT);

#endif /* INC_DHT11_H_ */
