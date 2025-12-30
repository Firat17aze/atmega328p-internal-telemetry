#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define UART_BAUD 9600
#define UART_UBRR ((F_CPU / (16UL * UART_BAUD)) - 1)

#define UART_RX_BUFFER_SIZE 64
#define UART_TX_BUFFER_SIZE 64

void UART_Init(void);
void UART_TxChar(uint8_t data);
void UART_TxString(const char* str);
bool UART_DataAvailable(void);
uint8_t UART_RxChar(void);
void UART_FlushRx(void);

#endif

