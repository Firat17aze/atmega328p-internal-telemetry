#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile uint8_t rx_buffer[UART_RX_BUFFER_SIZE];
static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_tail = 0;
static volatile uint8_t rx_count = 0;

ISR(USART_RX_vect)
{
    uint8_t data = UDR0;
    uint8_t next_head = (rx_head + 1) % UART_RX_BUFFER_SIZE;
    
    if (next_head != rx_tail) {
        rx_buffer[rx_head] = data;
        rx_head = next_head;
        rx_count++;
    }
}

void UART_Init(void)
{
    UBRR0H = (uint8_t)(UART_UBRR >> 8);
    UBRR0L = (uint8_t)(UART_UBRR);
    
    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
    
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    
    sei();
}

void UART_TxChar(uint8_t data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    
    UDR0 = data;
}

void UART_TxString(const char* str)
{
    while (*str) {
        UART_TxChar(*str++);
    }
}

bool UART_DataAvailable(void)
{
    return (rx_count > 0);
}

uint8_t UART_RxChar(void)
{
    uint8_t data = 0;
    
    if (rx_count > 0) {
        data = rx_buffer[rx_tail];
        rx_tail = (rx_tail + 1) % UART_RX_BUFFER_SIZE;
        
        cli();
        rx_count--;
        sei();
    }
    
    return data;
}

void UART_FlushRx(void)
{
    cli();
    rx_head = 0;
    rx_tail = 0;
    rx_count = 0;
    sei();
}

