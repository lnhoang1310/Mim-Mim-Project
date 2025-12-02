/*
 * uart.c
 *
 *  Created on: Dec 3, 2025
 *      Author: Hoàng Lâm
 */

#include "uart.h"
#include <stdbool.h>
#include <string.h>
#include "fan.h"
#include "main.h"

const Handle_CMD_Typedef handle_cmd[] =
{
	{"FAN SPEED", Fan_Control},
	{"TEMPERATURE SET", set_temperature}
};

uint8_t uart_buffer[UART_BUFFER_LEN];
uint8_t uart_buffer_index = 0;
bool uart_flag_cplt_receive = 0;
uint8_t data_rx;

#include <stddef.h>
#include <string.h>

// T?o b?ng lps (longest prefix suffix)
static void compute_lps(const char *pat, uint16_t m, uint16_t *lps)
{
    uint16_t len = 0;
    lps[0] = 0;

    for (uint16_t i = 1; i < m; ) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

static char* strstr_kmp(const char *text, const char *pat)
{
    uint16_t n = strlen(text);
    uint16_t m = strlen(pat);
    if (m == 0) return (char*)text;
    uint16_t lps[m];
    compute_lps(pat, m, lps);
    uint16_t i = 0;
    uint16_t j = 0;
    while(i < n){
        if(text[i] == pat[j]){
            i++;
            j++;
            if(j == m) return (char*)(text + i - j);
        }else{
            if(j != 0) j = lps[j - 1];
            else i++;

        }
    }
    return NULL;
}


static void uart_receive_data(uint8_t data){
	if(data == '\n'){
		uart_buffer[uart_buffer_index] = '\0';
		uart_buffer_index = 0;
		uart_flag_cplt_receive = true;
		return;
	}
	uart_buffer[uart_buffer_index++] = data;
	if(uart_buffer_index >= UART_BUFFER_LEN) uart_buffer_index = 0;
}

static float parse_number(const char* str){
    float value = 0.0f;
    float decimal_factor = 0.1f;
    bool flag_decimal = false;

    while(*str && (*str < '0' || *str > '9')) str++;

    while((*str >= '0' && *str <= '9') || *str == '.'){
        if(*str == '.'){
            flag_decimal = true;
            str++;
            continue;
        }
        if(!flag_decimal){
            value = value * 10 + (*str - '0');
        }else{
            value += (*str - '0') * decimal_factor;
            decimal_factor *= 0.1f;
        }
        str++;
    }

    return value;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart){
	if(huart->Instance == USART1){
		uart_receive_data(data_rx);
		HAL_UART_Receive_IT(huart, &data_rx, 1);
	}
}

void uart_init(UART_HandleTypeDef* huart){
	HAL_UART_Receive_IT(huart, &data_rx, 1);
}

bool uart_handle(void){
	if(uart_flag_cplt_receive){
		uart_flag_cplt_receive = false;
		for(uint8_t i=0; i<sizeof(handle_cmd)/sizeof(Handle_CMD_Typedef); i++){
			if(strstr_kmp((char*)uart_buffer, handle_cmd[i].cmd) != NULL){
				handle_cmd[i].function(parse_number((char*)uart_buffer));
				if(i == 0) return true;
				break;
			}
		}
	}
	return false;
}


