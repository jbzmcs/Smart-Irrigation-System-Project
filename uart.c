/**
 * UART Communication Module Implementation
 * Handles serial communication functionality
 */
#include "uart.h"
#include <stdio.h>

/**
 * Initialize UART communication
 * @param ubrr Baud rate register value
 */
void UART_init(unsigned int ubrr) {
    // Set baud rate
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    
    // Enable transmitter
    UCSR0B = (1 << TXEN0);
    
    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

/**
 * Send a single character over UART
 * @param c Character to send
 */
void UART_sendChar(char c) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    
    // Put data into buffer, sends the data
    UDR0 = c;
}

/**
 * Send a string over UART
 * @param str Null-terminated string to send
 */
void UART_sendString(const char* str) {
    // Send each character until null terminator
    while (*str) {
        UART_sendChar(*str++);
    }
}

/**
 * Send an integer value as string over UART
 * @param value Integer value to send
 */
void UART_sendInt(int value) {
    char buffer[10]; // Buffer to hold the string representation
    sprintf(buffer, "%d", value);
    UART_sendString(buffer);
}