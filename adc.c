/**
 * ADC Module Implementation
 * Handles analog-to-digital conversion for soil moisture sensor
 */
#include "adc.h"
#include <avr/interrupt.h>

// Global variables
volatile uint16_t adcResult = 0;
extern volatile uint8_t check_moisture_flag;

/**
 * Initialize ADC for soil moisture sensor reading
 */
void ADC_init(void) {
    // Set reference voltage to AVcc (5V)
    // Select ADC channel 0 by default
    ADMUX = (1 << REFS0) | (SENSOR_CHANNEL & 0x07);
    
    // Enable ADC, enable ADC interrupt, set prescaler to 64
    // 16MHz/64 = 250kHz ADC clock (ADC requires 50-200kHz)
    ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1);
}

/**
 * Start an ADC conversion
 */
void ADC_startConversion(void) {
    // Start ADC conversion
    ADCSRA |= (1 << ADSC);
}

/**
 * Process completed ADC conversion
 * Called from ADC interrupt handler
 */
void ADC_conversionComplete(void) {
    // Read ADC result from register
    adcResult = ADC;
}

/**
 * Get the last ADC reading
 * @return Last ADC result
 */
uint16_t ADC_getLastReading(void) {
    return adcResult;
}

// ADC conversion complete interrupt handler
ISR(ADC_vect) {
    // Store ADC result
    ADC_conversionComplete();
    
    // Set flag to process moisture reading in main loop
    check_moisture_flag = 1;
}