/**
 * UART Communication Module Header
 * Handles serial communication functionality
 */
#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <stdio.h>

/**
 * Initialize UART communication
 * @param ubrr Baud rate register value
 */
void UART_init(unsigned int ubrr);

/**
 * Send a single character over UART
 * @param c Character to send
 */
void UART_sendChar(char c);

/**
 * Send a string over UART
 * @param str Null-terminated string to send
 */
void UART_sendString(const char* str);

/**
 * Send an integer value as string over UART
 * @param value Integer value to send
 */
void UART_sendInt(int value);

#endif /* UART_H */