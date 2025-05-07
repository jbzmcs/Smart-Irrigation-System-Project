/**
 * Timer Module Implementation
 * Handles timing for periodic moisture sensing
 */
#include "timer.h"
#include "adc.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * Initialize Timer1 for periodic moisture sensing
 * Sets up timer to overflow approximately every 1 second
 * 
 * With F_CPU = 16MHz and prescaler 1024:
 * Timer frequency = 16MHz/1024 = 15.625kHz
 * Timer period = 1/15.625kHz = 64µs
 * For 16-bit timer, max count = 65536
 * Overflow time = 65536 * 64µs ≈ 4.19 seconds
 */
void Timer1_init(void) {
    // Set Timer1 in normal mode
    TCCR1A = 0;
    
    // Set prescaler to 1024
    TCCR1B = (1 << CS12) | (1 << CS10);
    
    // Load counter value to get approximately 1 second overflow
    // 16MHz/1024 = 15625 counts per second
    // 65536 - 15625 = 49911 for 1 second overflow
    TCNT1 = 49911;
    
    // Enable Timer1 overflow interrupt
    TIMSK1 = (1 << TOIE1);
}

// Timer1 overflow interrupt handler
ISR(TIMER1_OVF_vect) {
    // Start ADC conversion when timer overflows
    ADC_startConversion();
    
    // Reset counter value for next 1 second
    TCNT1 = 49911;
}