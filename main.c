/**
 * Smart Irrigation System - Main File
 * Controls soil moisture monitoring and automated irrigation
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "uart.h"
#include "adc.h"
#include "relay.h"
#include "timer.h"

// Thresholds for moisture levels
#define DRY_THRESHOLD 600
#define WATERING_DURATION 5000  // Duration in ms to water when soil is dry

// Global variables - visible to other modules
volatile uint8_t check_moisture_flag = 0;

/**
 * Main function
 */
int main(void) {
    // Initialize all modules
    UART_init(103);      // 9600 baud @ 16MHz
    ADC_init();
    Timer1_init();
    Relay_init();

    // Initial relay state - OFF
    Relay_off();

    // Status message
    UART_sendString("Smart Irrigation System Started\r\n");
    UART_sendString("------------------------\r\n");

    // Enable global interrupts
    sei();

    // Main loop
    while (1) {
        if (check_moisture_flag) {
            check_moisture_flag = 0;
            
            // Get moisture reading
            uint16_t moisture = ADC_getLastReading();
            
            // Send moisture data to serial
            UART_sendString("Soil Moisture Value: ");
            UART_sendInt(moisture);
            
            // Check if soil is dry and take appropriate action
            if (moisture < DRY_THRESHOLD) {
                UART_sendString(" | Status: DRY - Pump ON\r\n");
                
                // Activate irrigation
                Relay_on();
                
                // Water for the specified duration
                _delay_ms(WATERING_DURATION);
                
                // Turn off irrigation
                Relay_off();
                
                UART_sendString("Watering complete\r\n");
            } else {
                UART_sendString(" | Status: MOIST/WET - Pump OFF\r\n");
                // Ensure relay is off
                Relay_off();
            }
        }
    }
    
    return 0;
}

// The ISRs are moved to their respective module files