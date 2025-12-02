/*
 * lcd_no_i2c.c
 *
 *  Created on: Dec 3, 2025
 *      Author: Hoàng Lâm
 */

#include "lcd_no_i2c.h"
#include <stdio.h>

static inline void pul_enable(LCD_Typedef* lcd)
{
    HAL_GPIO_WritePin(lcd->lcd_port, lcd->en, GPIO_PIN_SET);
    for(volatile int i=0; i<50; i++);
    HAL_GPIO_WritePin(lcd->lcd_port, lcd->en, GPIO_PIN_RESET);
    for(volatile int i=0; i<50; i++);
}

static void write_4bit(LCD_Typedef* lcd, uint8_t nibble){
	HAL_GPIO_WritePin(lcd->lcd_port, lcd->d4, (nibble & 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->lcd_port, lcd->d5, (nibble & (1 << 1)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->lcd_port, lcd->d6, (nibble & (1 << 2)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->lcd_port, lcd->d7, (nibble & (1 << 3)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	pul_enable(lcd);
}

static void send_command(LCD_Typedef* lcd, uint8_t cmd){
	HAL_GPIO_WritePin(lcd->lcd_port, lcd->rs, GPIO_PIN_RESET);
	write_4bit(lcd, (cmd >> 4) & 0x0F);
	write_4bit(lcd, cmd & 0xF);
	HAL_Delay(1);
}

static void send_data(LCD_Typedef* lcd, uint8_t data){
	HAL_GPIO_WritePin(lcd->lcd_port, lcd->rs, GPIO_PIN_SET);
	write_4bit(lcd, (data >> 4) & 0x0F);
	write_4bit(lcd, data & 0xF);
	HAL_Delay(1);
}

void lcd_init(LCD_Typedef* lcd)
{
    // Reset tất cả các chân
    HAL_GPIO_WritePin(lcd->lcd_port, lcd->d4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lcd->lcd_port, lcd->d5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lcd->lcd_port, lcd->d6, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lcd->lcd_port, lcd->d7, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lcd->lcd_port, lcd->en, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lcd->lcd_port, lcd->rs, GPIO_PIN_RESET);
    if(lcd->rw) HAL_GPIO_WritePin(lcd->lcd_port, lcd->rw, GPIO_PIN_RESET);

    HAL_Delay(50);

    // Khởi tạo 4-bit theo datasheet
    write_4bit(lcd, 0x03);
    HAL_Delay(5);
    write_4bit(lcd, 0x03);
    HAL_Delay(1);
    write_4bit(lcd, 0x03);
    HAL_Delay(1);
    write_4bit(lcd, 0x02);  // Chuyển sang 4-bit mode
    HAL_Delay(1);

    send_command(lcd, 0x28); // 4-bit, 2 line, 5x8 font
    send_command(lcd, 0x08); // display off
    send_command(lcd, 0x01); // clear
    HAL_Delay(2);
    send_command(lcd, 0x06); // entry mode
    send_command(lcd, 0x0C); // display on, cursor off
}

void lcd_clear(LCD_Typedef* lcd){
	send_command(lcd, 0x01);
	HAL_Delay(1);
}

void lcd_gotoxy(LCD_Typedef* lcd, uint8_t col, uint8_t row){
	const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if(row > 3) row = 3;
    send_command(lcd, 0x80 | (col + row_offsets[row]));
}

void lcd_sendchar(LCD_Typedef* lcd, char c){
	send_data(lcd, c);
}

void lcd_sendstring(LCD_Typedef* lcd, const char* str){
	while(*str) send_data(lcd, *str++);
}

void lcd_sendnumber(LCD_Typedef* lcd, const float num){
	if(num == 100){
		lcd_sendstring(lcd, " 100");
		return;
	}
	char buff[5];
	sprintf(buff, "%4.1f", num);
	lcd_sendstring(lcd, buff);
}

