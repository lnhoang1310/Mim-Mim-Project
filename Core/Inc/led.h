/*
 * led.h
 *
 *  Created on: Dec 3, 2025
 *      Author: Hoàng Lâm
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "main.h"

#define LED_PORT GPIOB

void Led_Init(void);
void Led_Control(float duty);

#endif /* INC_LED_H_ */
