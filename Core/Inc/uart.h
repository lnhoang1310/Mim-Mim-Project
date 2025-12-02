/*
 * uart.h
 *
 *  Created on: Dec 3, 2025
 *      Author: Hoàng Lâm
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32f4xx.h"
#include <stdbool.h>

#define UART_BUFFER_LEN 20

typedef void (*function_t)(float);

typedef struct{
	char* cmd;
	function_t function;
}Handle_CMD_Typedef;

void uart_init(UART_HandleTypeDef* huart);
bool uart_handle(void);

#endif /* INC_UART_H_ */
