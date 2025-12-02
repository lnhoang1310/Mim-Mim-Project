/*
 * lcd_no_i2c.h
 *
 *  Created on: Dec 3, 2025
 *      Author: Hoàng Lâm
 */

#ifndef INC_LCD_NO_I2C_H_
#define INC_LCD_NO_I2C_H_

#include "stm32f4xx.h"

#define LCD_PORT GPIOB

typedef struct{
	GPIO_TypeDef* lcd_port;
	uint16_t rw;
	uint16_t rs;
	uint16_t en;
	uint16_t d4;
	uint16_t d5;
	uint16_t d6;
	uint16_t d7;
}LCD_Typedef;

void lcd_init(LCD_Typedef* lcd);
void lcd_clear(LCD_Typedef* lcd);
void lcd_gotoxy(LCD_Typedef* lcd, uint8_t x, uint8_t y);
void lcd_sendchar(LCD_Typedef* lcd, char c);
void lcd_sendstring(LCD_Typedef* lcd, const char* str);
void lcd_sendnumber(LCD_Typedef* lcd, const float num);

#endif /* INC_LCD_NO_I2C_H_ */
