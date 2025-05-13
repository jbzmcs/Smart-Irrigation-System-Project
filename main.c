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

// Threshold for moisture level (adjust based on your sensor behavior)
#define DRY_THRESHOLD     600
#define WATERING_DURATION 5000  // Duration in ms to water when soil is dry
#define AIR_THRESHOLD 950    
#define AIR_DETECTION_COUNT 3
uint8_t air_counter = 0;

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

            // Debug: Indicate logic check is running
            UART_sendString(" | Checking moisture level...");

            if (moisture > AIR_THRESHOLD) { //Checks if the sensor is on air
                air_counter++;
                if (air_counter >= AIR_DETECTION_COUNT) {
                    UART_sendString(" | Status: POSSIBLY IN AIR - Pump OFF\r\n");
                    Relay_off(); // Prevent watering if sensor likely in air
                    continue;    // Skip further checks
                }
            } else {
                air_counter = 0; // Reset if reading drops below air threshold
            }

           if(moisture > DRY_THRESHOLD) {// Check if soil is dry and take appropriate action
                UART_sendString(" | Status: DRY - Pump ON\r\n");

                // Activate irrigation
                Relay_on();

                // Water for the specified duration
                _delay_ms(WATERING_DURATION);

                // Turn off irrigation
                Relay_off();

                UART_sendString("Watering complete\r\n");
            } else{
                UART_sendString(" | Status: MOIST/WET - Pump OFF\r\n");
                // Ensure relay is off
                Relay_off();
            }
        }
    }

    return 0;
}
