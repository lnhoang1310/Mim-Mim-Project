#include "dht11.h"

extern TIM_HandleTypeDef htim3;

float nhiet_do;
float do_am;
uint8_t rh_byte1;
uint8_t rh_byte2;
uint8_t temp_byte1;
uint8_t temp_byte2;
uint16_t sum;
uint8_t presence;
void delay_us(uint32_t time) {
    htim3.Instance->CNT = 0;
    while (htim3.Instance->CNT < time);
}

void dht11_set_output() {
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = DHT11_Pin;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_GPIO_Port, &gpio);
}

void dht11_set_input() {
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = DHT11_Pin;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT11_GPIO_Port, &gpio);
}

void dht11_start() {
    dht11_set_output();
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
    delay_us(30);
    dht11_set_input();
}

uint8_t dht11_check_response() {
    uint8_t response = 0;
    delay_us(40);
    if (!HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)) {
        delay_us(80);
        if (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)) {
            response = 1;
        }
    }
    while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin));
    return response;
}

uint8_t dht11_read(void) {
    uint8_t i = 0;
    for (uint8_t j = 0; j < 8; j++) {
        while (!HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin));
        delay_us(40);
        if (!HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)) {
            i &= ~(1 << (7 - j));
        } else {
            i |= (1 << (7 - j));
        }
        while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin));
    }
    return i;
}

void dht11_get_data(float* temperature, float* humidity) {
    dht11_start();
    presence = dht11_check_response();
    if (presence) {
        rh_byte1 = dht11_read();
        rh_byte2 = dht11_read();
        temp_byte1 = dht11_read();
        temp_byte2 = dht11_read();
        sum = dht11_read();
        if (sum == (rh_byte1 + rh_byte2 + temp_byte1 + temp_byte2)) {
            *temperature = temp_byte1 + temp_byte2 / 10;
            *humidity = rh_byte1 + rh_byte2 / 10;
        } else {
            *temperature = -1;
            *humidity = -1;
        }
    } else {
        *temperature = -1;
        *humidity = -1;
    }
}
