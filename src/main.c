#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "drivers/uart.h"
#include "drivers/adc.h"
#include "math/kalman.h"

#define CMD_BUFFER_SIZE 32
static char cmd_buffer[CMD_BUFFER_SIZE];
static uint8_t cmd_index = 0;

#define TEMP_OFFSET 324.31f
#define TEMP_GAIN 1.22f

#define KALMAN_PROCESS_NOISE 0.01f
#define KALMAN_MEASUREMENT_NOISE 4.0f

static KalmanState temp_filter;

static float ADC_ToTemperature(uint16_t adc_value)
{
    float temp = ((float)adc_value - TEMP_OFFSET) / TEMP_GAIN;
    return temp;
}

static void ProcessCommand(const char* cmd)
{
    if (strcmp(cmd, "status") == 0) {
        uint16_t raw_temp_adc = ADC_ReadTemperature();
        float raw_temp = ADC_ToTemperature(raw_temp_adc);
        float filtered_temp = Kalman_Update(&temp_filter, raw_temp);
        uint16_t vcc_mv = ADC_CalculateVCC();
        
        UART_TxString("\r\n=== Silicon Vitals Monitor ===\r\n");
        UART_TxString("Raw Temperature:     ");
        
        char temp_str[16];
        int temp_int = (int)(raw_temp * 10);
        int whole = temp_int / 10;
        int frac = abs(temp_int % 10);
        if (raw_temp < 0 && whole == 0) {
            UART_TxString("-");
        }
        itoa(whole, temp_str, 10);
        UART_TxString(temp_str);
        UART_TxString(".");
        itoa(frac, temp_str, 10);
        UART_TxString(temp_str);
        UART_TxString(" C\r\n");
        
        UART_TxString("Filtered Temperature: ");
        temp_int = (int)(filtered_temp * 10);
        whole = temp_int / 10;
        frac = abs(temp_int % 10);
        if (filtered_temp < 0 && whole == 0) {
            UART_TxString("-");
        }
        itoa(whole, temp_str, 10);
        UART_TxString(temp_str);
        UART_TxString(".");
        itoa(frac, temp_str, 10);
        UART_TxString(temp_str);
        UART_TxString(" C\r\n");
        
        UART_TxString("VCC Voltage:         ");
        itoa(vcc_mv / 1000, temp_str, 10);
        UART_TxString(temp_str);
        UART_TxString(".");
        itoa((vcc_mv % 1000) / 100, temp_str, 10);
        UART_TxString(temp_str);
        UART_TxString(" V\r\n");
        UART_TxString("==============================\r\n");
        
    } else if (strcmp(cmd, "help") == 0) {
        UART_TxString("\r\n=== Available Commands ===\r\n");
        UART_TxString("status  - Display sensor readings\r\n");
        UART_TxString("help    - Show this help message\r\n");
        UART_TxString("===========================\r\n");
        
    } else if (strlen(cmd) > 0) {
        UART_TxString("\r\nUnknown command: ");
        UART_TxString(cmd);
        UART_TxString("\r\nType 'help' for available commands.\r\n");
    }
}

static void ProcessUARTInput(void)
{
    while (UART_DataAvailable()) {
        uint8_t ch = UART_RxChar();
        
        UART_TxChar(ch);
        
        if (ch == '\r' || ch == '\n') {
            if (cmd_index > 0) {
                cmd_buffer[cmd_index] = '\0';
                ProcessCommand(cmd_buffer);
                cmd_index = 0;
            }
        }
        else if (ch == '\b' || ch == 0x7F) {
            if (cmd_index > 0) {
                cmd_index--;
                UART_TxString(" \b");
            }
        }
        else if (cmd_index < (CMD_BUFFER_SIZE - 1)) {
            cmd_buffer[cmd_index++] = ch;
        }
    }
}

int main(void)
{
    UART_Init();
    ADC_Init();
    
    uint16_t initial_temp_adc = ADC_ReadTemperature();
    float initial_temp = ADC_ToTemperature(initial_temp_adc);
    Kalman_Init(&temp_filter, initial_temp, 10.0f, 
                KALMAN_PROCESS_NOISE, KALMAN_MEASUREMENT_NOISE);
    
    cmd_index = 0;
    memset(cmd_buffer, 0, CMD_BUFFER_SIZE);
    
    UART_TxString("\r\n\r\n");
    UART_TxString("=== Silicon Vitals Monitor ===\r\n");
    UART_TxString("ATmega328P Internal Sensor Monitor\r\n");
    UART_TxString("Type 'help' for commands.\r\n");
    UART_TxString("> ");
    
    while (1) {
        ProcessUARTInput();
        
        static uint16_t sample_counter = 0;
        if (++sample_counter >= 1000) {
            sample_counter = 0;
            uint16_t temp_adc = ADC_ReadTemperature();
            float temp = ADC_ToTemperature(temp_adc);
            Kalman_Update(&temp_filter, temp);
        }
    }
    
    return 0;
}

