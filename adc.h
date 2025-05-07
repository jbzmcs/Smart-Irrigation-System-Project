/**
 * ADC Module Header
 * Handles analog-to-digital conversion for soil moisture sensor
 */
#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

// Soil moisture sensor ADC channel
#define SENSOR_CHANNEL 0  // ADC0 (pin PC0 / A0)

/**
 * Initialize ADC for soil moisture sensor reading
 */
void ADC_init(void);

/**
 * Start an ADC conversion
 */
void ADC_startConversion(void);

/**
 * Process completed ADC conversion
 * Called from ADC interrupt handler
 */
void ADC_conversionComplete(void);

/**
 * Get the last ADC reading
 * @return Last ADC result
 */
uint16_t ADC_getLastReading(void);

#endif /* ADC_H */