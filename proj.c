#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

// Pin Definitions
#define SENSOR_CHANNEL 0         // ADC0 (pin PC0 / A0)
#define RELAY_PIN PB1            // Digital pin 9 (OC1A)
#define DRY_THRESHOLD 1500

volatile uint16_t adcResult = 0;
volatile uint8_t adcReady = 0;

// UART Initialization
void UART_init(unsigned int ubrr) {
    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_sendChar(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void UART_sendString(const char* str) {
    while (*str) UART_sendChar(*str++);
}

void UART_sendInt(int value) {
    char buffer[10];
    sprintf(buffer, "%d", value);
    UART_sendString(buffer);
}

// ADC Setup
void ADC_init() {
    ADMUX = (1 << REFS0); // AVcc ref, ADC0 by default
    ADCSRA = (1 << ADEN) | (1 << ADIE) | // Enable ADC and interrupt
             (1 << ADPS2) | (1 << ADPS1); // Prescaler 64
}

// Timer1 Setup: overflow every ~1s
void Timer1_init() {
    TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
    TIMSK1 |= (1 << TOIE1);              // Enable overflow interrupt
}

// Interrupt: Timer1 overflow
ISR(TIMER1_OVF_vect) {
    ADCSRA |= (1 << ADSC); // Start ADC conversion
}

// Interrupt: ADC Conversion Complete
ISR(ADC_vect) {
    adcResult = ADC;
    adcReady = 1;
}

int main(void) {
    UART_init(103);      // 9600 baud @ 16MHz
    ADC_init();
    Timer1_init();

    DDRB |= (1 << RELAY_PIN);        // Relay output
    PORTB |= (1 << RELAY_PIN);       // Relay OFF

    sei(); // Enable global interrupts

    while (1) {
        if (adcReady) {
            adcReady = 0;

            UART_sendString("Soil Moisture Value: ");
            UART_sendInt(adcResult);
            UART_sendString("\r\n");

            PORTB &= ~(1 << RELAY_PIN); // Turn relay ON (active LOW)
            _delay_ms(1000); // Keep relay on for 1 second
            PORTB |= (1 << RELAY_PIN);  // Turn relay OFF
            _delay_ms(1000); // Keep relay off for 1 second

            // if (adcResult < DRY_THRESHOLD) {
            //     UART_sendString("Status: DRY - Pump ON\r\n");
            //     PORTB &= ~(1 << RELAY_PIN); // Turn relay ON
            // } else {
            //     UART_sendString("Status: MOIST/WET - Pump OFF\r\n");
            //     PORTB |= (1 << RELAY_PIN);  // Turn relay OFF
            // }
        }
    }
}
