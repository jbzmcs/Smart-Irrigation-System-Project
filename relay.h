/**
 * Relay Control Module Header
 * Controls the water pump relay
 */
#ifndef RELAY_H
#define RELAY_H

#include <avr/io.h>

// Relay pin definition
#define RELAY_PORT PORTB
#define RELAY_DDR  DDRB
#define RELAY_PIN  PB1      // Digital pin 9 (OC1A)

/**
 * Initialize relay pin as output
 */
void Relay_init(void);

/**
 * Turn relay on (activate water pump)
 */
void Relay_on(void);

/**
 * Turn relay off (deactivate water pump)
 */
void Relay_off(void);

/**
 * Toggle relay state
 */
void Relay_toggle(void);

#endif /* RELAY_H */